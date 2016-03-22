#include "rtp_reliable_helper_impl.h"
#include <stdarg.h>
#include <stdio.h>

#if defined(_WIN32)
#include <Windows.h>
#include <time.h>
#else
#include <string>

inline void itoa (int num, char* buf, int n) {
	sprintf (buf, "%d", num);
}
#endif

namespace webrtc {

RtpReliableHelper* RtpReliableHelper::CreateRTPReliableHelper () {
	return new RtpReliableHelperImpl ();
}
void RtpReliableHelper::ReleaseRTPReliableHelper (RtpReliableHelper* rh) {
	delete rh;
}

FILE* gf = NULL;

RtpReliableHelperImpl::RtpReliableHelperImpl () 
: m_critical (CriticalSectionWrapper::CreateCriticalSection())
{
	m_cursor = 0;
	m_max_buf_size = 0;							// 声音500ms
	m_sended = NULL;
	m_last_recved_sequence = -1;
	m_switch = true;
	m_sealed_sequence = -1;
	m_cb = NULL;
	m_call_keyframe = false;
	m_last_tmstamp = -1;
	m_key_frame_packs_stmp = -1;
	m_max_switch_time = -1;
	m_successive_num_send = 0;

	//gf = fopen ("d:\\aaa.txt", "wb");
}

RtpReliableHelperImpl::~RtpReliableHelperImpl () {
	ClearSend ();
	free_recved_pack ();
	delete[] m_sended;

	//fclose (gf);
	//gf = NULL;
}

void RtpReliableHelperImpl::log (const char* fmt, ...) {
	va_list va;
	va_start (va, fmt);

	char l [128] = { 0 };
	vsnprintf (l, sizeof (l) - 1, fmt, va);
	va_end (va);

#ifdef _WIN32
	char tmp [32] = { 0 };
	snprintf (tmp, sizeof (tmp) - 1, "%u  ", GetTickCount ());
	OutputDebugStringA ("\r\n");
	OutputDebugStringA (tmp);
	OutputDebugStringA (l);
#else

#endif

	/*struct tm newtime;
	__int64 ltime;
	char buf[26] = { 0 };
	errno_t err;
	_time64( &ltime );
	err = _gmtime64_s( &newtime, &ltime );
	err = asctime_s(buf, 26, &newtime);*/

	//fwrite ("\r\n", 1, 2, gf);
	//fwrite (buf, 1, strlen (buf), gf);
	//fwrite (l, 1, strlen (l), gf);
}

void RtpReliableHelperImpl::SetMaxBufferSize (int size)	{ 
	delete[] m_sended;
	m_cursor = 0;
	m_max_buf_size = size; 
	m_sended = new SENDED_DATA* [m_max_buf_size + VIDEO_KEY_FRAME_PACK_NUM];
}

bool RtpReliableHelperImpl::IsFull () {
	CriticalSectionScoped lock (m_critical.get());
	return m_cursor < m_max_buf_size;
}

void RtpReliableHelperImpl::SendOutgoingPack (const char* data, int size, int header_size, unsigned long timestamp, unsigned long sequence) {
	CriticalSectionScoped lock (m_critical.get());
	if (m_cursor >= m_max_buf_size + VIDEO_KEY_FRAME_PACK_NUM) {
		log ("error overflow...");
		return;
	}

	timestamp &= 0xFFFFFF;
	sequence &= 0xFFFFFF;

	log ("%%%%%%%%%%%  发送: %u", sequence);

	SENDED_DATA* sd = new SENDED_DATA (data, size, header_size, timestamp, sequence);	// 只能使用3个字节
	m_sended [m_cursor] = sd;
	m_cursor ++;
}

void RtpReliableHelperImpl::erase_succeed_outgoing_pack (unsigned long sequence) {		// 从发送缓存中清除指定时间戳的包（该包已经成功收到响应）
	log ("发送得到响应: %u", sequence);
	for (int i=0; i<m_cursor; i++) {
		if (m_sended [i] == NULL) {
			continue;
		}
		if (m_sended [i]->m_sequence == sequence) {
			//log ("发送得到响应: %d", (int) sequence);
			delete m_sended [i];
			m_sended [i] = NULL;
			slide_send_window ();
			return;
		}
	}
}

void RtpReliableHelperImpl::slide_send_window () {
	int i;
	for (i=0; i<m_cursor; i++) {
		if (m_sended [i] != NULL) {
			break;
		}
	}

	// 当前没有可以滑动的空位置
	if (i == 0) {
		log ("窗口无法滑动: %d", (int) m_cursor);
		return;
	}

	// 向左滑动滑窗
	for (int j=i; j<m_cursor; j++) {
		m_sended [j - i] = m_sended [j];
	}
	m_cursor -= i;
	m_successive_num_send += i;
	//log ("窗口滑动: %d  窗口长度: %d", (int) i, (int) m_cursor);
}

bool RtpReliableHelperImpl::RecvPack (const char* data, int size, unsigned long timestamp, unsigned long sequence, 
									  unsigned long ack_sequence, unsigned long ack_sequence2, unsigned int ex_reset) {
	CriticalSectionScoped lock (m_critical.get());

	// 只使用3个字节
	sequence &= 0xFFFFFF;
	ack_sequence &= 0xFFFFFF;
	ack_sequence2 &= 0xFFFFFF;
	ex_reset &= 0xFFFFFF;


	std::string lg_sended;
	for (int i=0; i<m_cursor; i++) {
		if (m_sended[i] != NULL) {
			char tmp [16] = { 0 };
			itoa (m_sended[i]->m_sequence, tmp, 10);
			lg_sended += " ";
			lg_sended += tmp;
		}
	}
	log ("~~收到回应 : %u %u, 等待回应队列: %s", ack_sequence, ack_sequence2, lg_sended.c_str ());


	// 从发送队列中清除收到该响应的发送包
	if (ack_sequence != 0xFFFFFF) {
		erase_succeed_outgoing_pack (ack_sequence);
	}
	if (ack_sequence2 != 0xFFFFFF) {
		erase_succeed_outgoing_pack (ack_sequence2);
	}

	// 如果当前是重新设置帧，清空该帧之前需要确认的序号
	if (ex_reset == 1) {
		//log ("…………………重置 : %u", sequence);
		while (!m_recved_tm.empty ()) {
			if (m_recved_tm.front () < sequence) {
				log ("抛弃排队响应 : %u", (unsigned int) m_recved_tm.front ());
				m_recved_tm.pop_front ();
			}
			else {
				break;
			}
		}
	}


	// 把本次收到的包的时间戳保存到队列中，去掉可能的重复
	std::list<unsigned long>::iterator it1 = m_recved_tm.begin ();
	std::list<unsigned long>::iterator it2 = m_recved_tm.end ();
	std::list<unsigned long>::iterator it_insert = m_recved_tm.end ();		// 记录插入位置

	for (; it1 != it2; it1++) {
		if (*it1 == sequence) {
			return false;
		}
		else if (*it1 > sequence) {
			it_insert = it1;
			break;
		}
	}

	// 加入待确认接收包的顺序号
	m_recved_tm.insert (it_insert, sequence);

	std::string lg;
	for (it1 = m_recved_tm.begin (); it1 != m_recved_tm.end (); it1++) {
		char tmp [16] = { 0 };
		itoa (*it1, tmp, 10);
		lg += " ";
		lg += tmp;
	}

	log ("添加到接收队列 : %d, 接收队列: %s", (int) sequence, lg.c_str ());

	// 如果是重置帧，清空之前缓存的所有帧
	if (ex_reset == 1) {
		reset_unsuccessive_frame (sequence);
		m_last_recved_sequence = sequence;
		return true;
	}

	// 初始化第一帧
	if (m_last_recved_sequence == -1) {
		m_last_recved_sequence = sequence;
		return true;
	}

	// sequence不连续，数据体被缓存
	else if ( (int) sequence - (int) m_last_recved_sequence > 1) {
		buffer_unsuccessive_frame (data, size, timestamp, sequence);
		//log ("不连续包: %d, 需要的包: %d 缓存大小: %d", (int) sequence, (int) m_last_recved_sequence + 1, (int) m_recved_pack.size ());
		return false;
	}

	// sequence连续，直接取当前的数据
	else if ( (int) sequence - (int) m_last_recved_sequence == 1) {
		m_last_recved_sequence = sequence;
		//log ("recv request: %d, recved: %d", (int) sequence, (int) m_recved_tm.size ());
		return true;
	}

	// 收到了小的sequence, 肯定是无效的重发
	else {
		static int useless_repeat = 0;
		log ("---: %d", (int) ++useless_repeat);
		return false;
	}
}

void RtpReliableHelperImpl::buffer_unsuccessive_frame (const char* data, int size, unsigned long pack_timestamp, unsigned long sequence) {
	std::list<RECVED_DATA*>::iterator it1 = m_recved_pack.begin ();
	std::list<RECVED_DATA*>::iterator it2 = m_recved_pack.end ();
	std::list<RECVED_DATA*>::iterator it_insert = m_recved_pack.end ();

	for (; it1 != it2; it1 ++) {
		if ( (*it1)->m_sequence > sequence) {
			it_insert = it1;
			break;
		}
	}
	m_recved_pack.insert (it_insert, new RECVED_DATA (data, size, pack_timestamp, sequence));

	std::string lg;
	it1 = m_recved_pack.begin ();
	it2 = m_recved_pack.end ();
	for (; it1 != it2; it1++) {
		char tmp [16] = { 0 };
		itoa ((*it1)->m_sequence, tmp, 10);
		lg += " ";
		lg += tmp;
	}

	//log ("缓存的不连续包: %u, 当前位置: %u 内容: %s", sequence, m_last_recved_sequence, lg.c_str ());
}


void RtpReliableHelperImpl::reset_unsuccessive_frame (unsigned long sequence) {
	while (!m_recved_pack.empty ()) {
		RECVED_DATA* rd = m_recved_pack.front ();
		if (rd == NULL 
			|| rd->m_sequence <= sequence) {
			//log ("清空缓存包: %u, reset包: %u ", rd->m_sequence, sequence);
			delete rd;
			m_recved_pack.pop_front ();
		}
		// 安顺寻排的
		else {
			break;
		}
	}

	std::string lg;
	std::list<RECVED_DATA*>::iterator it1 = m_recved_pack.begin ();
	std::list<RECVED_DATA*>::iterator it2 = m_recved_pack.end ();
	for (; it1 != it2; it1++) {
		char tmp [16] = { 0 };
		itoa ((*it1)->m_sequence, tmp, 10);
		lg += " ";
		lg += tmp;
	}

	//log ("清空缓存包后缓存的不连续包 当前位置: %u 内容: %s", m_last_recved_sequence, lg.c_str ());
}

bool RtpReliableHelperImpl::PopACKTimestamp (unsigned long* timestamp) {
	CriticalSectionScoped lock (m_critical.get());
	
	if (m_recved_tm.empty ()) {
		//log ("响应队列为空");
		return false;
	}

	*timestamp = m_recved_tm.front ();
	m_recved_tm.pop_front ();
	log ("------>: %u", *timestamp);
	return true;
}

bool RtpReliableHelperImpl::PopACKTimestamp2 (unsigned long* timestamp) {
	CriticalSectionScoped lock (m_critical.get());
	
	if (m_recved_tm.empty ()) {
		//log ("响应队列为空");
		return false;
	}

	*timestamp = m_recved_tm.front ();
	if (m_recved_tm.size () >= 2) {					// 如果等待队列过长，同时回传两个ack
		m_recved_tm.pop_front ();
	}
	log ("------>: %u", *timestamp);
	return true;
}

void RtpReliableHelperImpl::ClearSend () {
	CriticalSectionScoped lock (m_critical.get());
	for (int i=0; i<m_cursor; i++) {
		delete m_sended [i];
	}
	m_cursor = 0;
	log ("send cleared");
}

bool RtpReliableHelperImpl::PopResendPack (char** data, int* size, int* header_size, unsigned long* timestamp) {
	// 外部锁，因为外部要锁定数据，避免数据拷贝
	//CriticalSectionScoped lock (m_critical.get());

	// 完全关闭重发
	return false;

	// 滑窗为空
	if (m_cursor == 0) {
		return false;
	}

	// 如果当前滑窗已满
	int index = -1;
	if (m_cursor >= m_max_buf_size || !m_switch) {
		index = 0;
	}
	else {
		return false;
	}

	// 开关已经关闭，重发间隔设置为2，开关打开重发间隔设置为5
	int resend_interval = m_switch ? 5 : 2;
	if (m_sended [index]->m_resend_times % resend_interval != 0) {
		m_sended [index]->m_resend_times ++;
		return false;
	}

	// 对该包进行重发
	m_sended [index]->m_resend_times ++;
	*data = m_sended [index]->m_data;
	*size = m_sended [index]->m_size;
	*header_size = m_sended [index]->m_header_size;
	*timestamp = m_sended [index]->m_timestamp;

	std::string lg;
	for (int i=0; i<m_cursor; i++) {
		if (m_sended [i] == NULL) {
			continue;
		}
		char tmp [16] = { 0 };
		itoa (m_sended [i]->m_sequence, tmp, 10);
		lg += " ";
		lg += tmp;
	}

	log ("重发: %d, 发送队列内容: %s", (int) m_sended [index]->m_sequence, lg.c_str ());
	return true;
}

bool RtpReliableHelperImpl::PopBufferedPack (char** data, int* size) {
	CriticalSectionScoped lock (m_critical.get());

	// 没有缓存的
	if (m_recved_pack.empty ()) {
		//log ("pop buffered pack empty");
		return false;
	}

	while (!m_recved_pack.empty ()) {
		if (m_recved_pack.front()->m_sequence <= m_last_recved_sequence) {
			delete m_recved_pack.front();
			m_recved_pack.pop_front ();
		}
		else {
			break;
		}
	}

	// 有缓存的，但是缓存的前面还有中断的
	if ( (int) m_recved_pack.front()->m_sequence - (int) m_last_recved_sequence > 1) {
		//log ("无法取缓存包 : %d  current %d", (int) m_last_recved_sequence, (int) m_recved_pack.front()->m_sequence);
		return false;
	}

	// 弹出最前面的
	RECVED_DATA* pack = m_recved_pack.front();
	m_recved_pack.pop_front ();
	
	m_last_recved_sequence = pack->m_sequence;
	*data = pack->m_data;
	*size = pack->m_size;

	// 该指针由调用方释放
	pack->m_data = NULL;
	delete pack;
	return true;
}

void RtpReliableHelperImpl::free_recved_pack () {
	CriticalSectionScoped lock (m_critical.get());
	std::list<RECVED_DATA*>::iterator it1 = m_recved_pack.begin ();
	std::list<RECVED_DATA*>::iterator it2 = m_recved_pack.end ();

	for (; it1 != it2; it1 ++) {
		delete *it1;
	}
	m_recved_pack.clear ();
}

bool RtpReliableHelperImpl::is_allow_send (bool bKeyArrive, unsigned int* cur_sequence, bool* reset, unsigned int tmstamp) {
	// 当前开关已经关闭
	if (!m_switch) {
		// 重新开始发送必须从第一个Key帧开始，否则会造成花屏
		if (!bKeyArrive) {
			return false;
		}
		
		// key帧开启开关
		m_call_keyframe = false;
		m_switch = true;

		// 清空发送滑动窗口
		for (int i=0; i<m_cursor; i++) {
			delete m_sended [i];
		}
		m_cursor = 0;

		// 新的Sequence应该比以前的大
		*cur_sequence = m_sealed_sequence + 1;		
		*reset = true;

		log ("\r\n ******---- key arrive");
		return true;
	}

	// 当前开关是打开的
	else {
		tmstamp &= 0xFFFFFF;

		// 如果和上一包的时间戳相同（同一个key帧的多个分包），允许自动扩大缓冲区
		if (m_last_tmstamp == tmstamp && m_cursor < m_max_buf_size + VIDEO_KEY_FRAME_PACK_NUM) {
			log ("\r\n ###### 同帧 %u %u ", *cur_sequence, tmstamp);
			m_key_frame_packs_stmp = tmstamp;
			return true;
		}

		// 记录当前的时间戳，作为下次参考的记录
		m_last_tmstamp = tmstamp;

		// 查看当前存在几个相同帧的记录
		int same_packs_num = 0;
		for (int i=0; i<m_cursor; i++) {
			if (m_sended [i] != NULL && m_sended [i]->m_timestamp == m_key_frame_packs_stmp) {
				same_packs_num ++;
			}
		}

		// 相同时间戳的帧看做一帧
		if (same_packs_num > 0) {
			same_packs_num--;
		}

		// 正常包，超过正常缓存，打开开关
		if (m_cursor >= m_max_buf_size + same_packs_num) {


			std::string lg_sended;
			for (int i=0; i<m_cursor; i++) {
				if (m_sended[i] != NULL) {
					char tmp [16] = { 0 };
					itoa (m_sended[i]->m_sequence, tmp, 10);
					lg_sended += " ";
					lg_sended += tmp;
				}
			}
			log ("~~~~~~个数 : %u : %s", m_cursor, lg_sended.c_str ());


			m_switch = false;
			m_sealed_sequence = *cur_sequence;	// 封存sequence
			m_successive_num_send = 0;			// 重新开始计数
			if (m_cb != NULL) {
				m_cb->OnRTPNetChanged (m_cb_params, 5);				// 发送速度降低10%
			}
			log ("\r\n ++++++++++++++++ sequence sealed %u", m_sealed_sequence);
			// 请求关键帧
			m_call_keyframe = true;
			return false;
		}
		
		// 当前开关打开，并且缓冲区没有满
		check_for_increase_bandwidth ();
		return true;
	}
}

void RtpReliableHelperImpl::check_for_increase_bandwidth () {
	if (m_successive_num_send > 20) {
		if (m_cb != NULL) {
			m_cb->OnRTPNetChanged (m_cb_params, -7);				// 发送速度提高7%
			log ("~~~~~~升高带宽 5%%");
		}
		m_successive_num_send = 0;
	}
}

void RtpReliableHelperImpl::Lock (bool bLock)	{ 
	bLock ? m_critical.get()->Enter () : m_critical.get()->Leave (); 

	// 这个请求只能放在这了 ，否则会死锁
	if (!bLock) {
		if (m_call_keyframe) {
			m_cb->RequestKeyFrame (m_cb_params);
			log ("请求关键帧");
			m_call_keyframe = false;
		}
	}
}

}

