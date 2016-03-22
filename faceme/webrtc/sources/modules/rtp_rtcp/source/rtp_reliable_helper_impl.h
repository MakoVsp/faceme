#ifndef WEBRTC_MODULES_RTP_RELIABLE_HELPER_IMPL_H_
#define WEBRTC_MODULES_RTP_RELIABLE_HELPER_IMPL_H_
#include <list>
#include "rtp_reliable_helper.h"
#include "system_wrappers/interface/critical_section_wrapper.h"
#include "system_wrappers/interface/scoped_ptr.h"


namespace webrtc {

// 缓存已经发送包的信息
struct SENDED_DATA {
	char* m_data;					// 包的发送参数
	int m_size;
	int m_header_size;
	unsigned long m_timestamp;
	int m_resend_times;				// 包的重发次数
	unsigned long m_sequence;

	SENDED_DATA () :m_data(NULL), m_size(0), m_header_size(0), m_timestamp(0), m_resend_times(0), m_sequence(0) {
	}
	SENDED_DATA (const char* data, int size, int header_size, unsigned long timestamp, unsigned long sequence) {
		//m_data = new char [size];
		//memcpy (m_data, data, size);
		m_data = NULL;
		m_size = size;
		m_header_size = header_size;
		m_timestamp = timestamp;
		m_resend_times = 0;
		m_sequence = sequence;
	}
	~SENDED_DATA () {
		delete[] m_data;
	}
};

// 缓存接收包的信息
struct RECVED_DATA {
	char* m_data;					
	int m_size;
	unsigned long m_timestamp;
	unsigned long m_sequence;

	RECVED_DATA () :m_data(NULL), m_size(0), m_timestamp(0), m_sequence(0) {
	}
	RECVED_DATA (const char* data, int size, unsigned long timestamp, unsigned long sequence) {
		m_data = new char [size];
		memcpy (m_data, data, size);
		m_size = size;
		m_timestamp = timestamp;
		m_sequence = sequence;
	}
	~RECVED_DATA () {
		delete[] m_data;
	}
};

class RtpReliableHelperImpl : public RtpReliableHelper
{
public:
	RtpReliableHelperImpl ();
	~RtpReliableHelperImpl ();

	/*
	 * 向网络发送一包，同时缓存在本地，等待确认
	 * 同时重发没有收到确认的包
	 */
	virtual void SendOutgoingPack (const char* data, int size, int header_size, unsigned long timestamp, unsigned long sequence);
	/*
	 * 接收到一包，根据ack_timestamp 删除在本地发送缓存的包
	 * 同时顺序化接收到的包
	 * return false 该包被重复接收，抛弃
	 */
	virtual bool RecvPack (const char* data, int size, unsigned long pack_timestamp, unsigned long sequence, 
		unsigned long ack_sequence, unsigned long ack_sequence2, unsigned int ex_reset);
	/*
	 * 获取一个上次接收到的包的时间戳，作为 ACK 时间戳
	 */
	virtual bool PopACKTimestamp (unsigned long* timestamp);
	/*
	 * 获取一个上次接收到的包的时间戳，作为 ACK 时间戳
	 */
	virtual bool PopACKTimestamp2 (unsigned long* timestamp);
	/*
	 * 清除发送队列
	 */
	virtual void ClearSend ();
	/*
	 * 是否发送队列满
	 */
	virtual bool IsFull ();
	/*
	 * 设置发送缓冲区大小
	 */
	virtual void SetMaxBufferSize (int size);
	/*
	 * 找出一个需要重发的包
	 */
	virtual bool PopResendPack (char** data, int* size, int* header_size, unsigned long* timestamp);
	/*
	 * 返回本地缓存的包 （由于接收到的包顺序号中断导致的）data返回的指针需要被释放
	 * return false	没有
	 */
	virtual bool PopBufferedPack (char** data, int* size);

	/*
	 * 为了避免拷贝数据，加一个外部控制的锁
	 */
	virtual void Lock (bool bLock);

	/*
	 * 控制当前是否可以继续发送数据
	 * @bKeyArrive		当前是否是关键帧的开始
	 * @[in, out] cur_sequence	对当前的sequence进行管理
	 */
	virtual bool is_allow_send (bool bKeyArrive, unsigned int* cur_sequence, bool* reset, unsigned int tmstamp);

	/*
	 * 设置一个回调接口
	 */
	virtual void set_cb (RtpReliableSenderCB* cb, int param) { m_cb = cb; m_cb_params = param; }

protected:
	int m_cursor;
	int m_max_buf_size;
	SENDED_DATA** m_sended;						// 发送后等待确认的包
	bool m_switch;								// 活动开关 （true-打开 可以正常发送 false-关闭 屏蔽所有发送）
	unsigned int m_sealed_sequence;				// 当活动开关被关闭时，保存关闭前的sequence。当活动开关打开时，恢复之前保存的sequence
	unsigned int m_last_tmstamp;				// 上一帧的时间戳
	unsigned int m_key_frame_packs_stmp;		// 相同时间帧的时间戳
	int m_max_switch_time;						// 设定的最长封锁时间
	
	RtpReliableSenderCB* m_cb;					// 回调
	int m_cb_params;							// 回调参数
	bool m_call_keyframe;						// 请求关键帧

	int m_successive_num_send;					// 连续正常发送的包数（作为提高发送带宽的依据）

	std::list<unsigned long> m_recved_tm;		// 接收到的包的时间戳，等待作为ack返回到对方
	scoped_ptr <CriticalSectionWrapper> m_critical;

	unsigned long m_last_recved_sequence;		// 上一次接收到的包的顺序号
	std::list<RECVED_DATA*> m_recved_pack;		// 接收到的包进行缓存，中间缺帧

	void log (const char* fmt, ...);
	void erase_succeed_outgoing_pack (unsigned long sequence);		// 从发送缓存中清除指定时间戳的包（该包已经成功收到响应）
	void slide_send_window ();										// 滑动滑窗
	void buffer_unsuccessive_frame (const char* data, int size, unsigned long pack_timestamp, unsigned long sequence);		// 缓存不连续的帧
	void reset_unsuccessive_frame (unsigned long sequence);																	// 重置不连续的帧缓存

	void free_recved_pack ();										// 释放m_recved_pack占用的内存
	void check_for_increase_bandwidth ();							// 检测是否有必要提高带宽
};
}
#endif
