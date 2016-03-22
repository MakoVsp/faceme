#ifndef WEBRTC_MODULES_RTP_RELIABLE_HELPER_IMPL_H_
#define WEBRTC_MODULES_RTP_RELIABLE_HELPER_IMPL_H_
#include <list>
#include "rtp_reliable_helper.h"
#include "system_wrappers/interface/critical_section_wrapper.h"
#include "system_wrappers/interface/scoped_ptr.h"


namespace webrtc {

// �����Ѿ����Ͱ�����Ϣ
struct SENDED_DATA {
	char* m_data;					// ���ķ��Ͳ���
	int m_size;
	int m_header_size;
	unsigned long m_timestamp;
	int m_resend_times;				// �����ط�����
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

// ������հ�����Ϣ
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
	 * �����緢��һ����ͬʱ�����ڱ��أ��ȴ�ȷ��
	 * ͬʱ�ط�û���յ�ȷ�ϵİ�
	 */
	virtual void SendOutgoingPack (const char* data, int size, int header_size, unsigned long timestamp, unsigned long sequence);
	/*
	 * ���յ�һ��������ack_timestamp ɾ���ڱ��ط��ͻ���İ�
	 * ͬʱ˳�򻯽��յ��İ�
	 * return false �ð����ظ����գ�����
	 */
	virtual bool RecvPack (const char* data, int size, unsigned long pack_timestamp, unsigned long sequence, 
		unsigned long ack_sequence, unsigned long ack_sequence2, unsigned int ex_reset);
	/*
	 * ��ȡһ���ϴν��յ��İ���ʱ�������Ϊ ACK ʱ���
	 */
	virtual bool PopACKTimestamp (unsigned long* timestamp);
	/*
	 * ��ȡһ���ϴν��յ��İ���ʱ�������Ϊ ACK ʱ���
	 */
	virtual bool PopACKTimestamp2 (unsigned long* timestamp);
	/*
	 * ������Ͷ���
	 */
	virtual void ClearSend ();
	/*
	 * �Ƿ��Ͷ�����
	 */
	virtual bool IsFull ();
	/*
	 * ���÷��ͻ�������С
	 */
	virtual void SetMaxBufferSize (int size);
	/*
	 * �ҳ�һ����Ҫ�ط��İ�
	 */
	virtual bool PopResendPack (char** data, int* size, int* header_size, unsigned long* timestamp);
	/*
	 * ���ر��ػ���İ� �����ڽ��յ��İ�˳����жϵ��µģ�data���ص�ָ����Ҫ���ͷ�
	 * return false	û��
	 */
	virtual bool PopBufferedPack (char** data, int* size);

	/*
	 * Ϊ�˱��⿽�����ݣ���һ���ⲿ���Ƶ���
	 */
	virtual void Lock (bool bLock);

	/*
	 * ���Ƶ�ǰ�Ƿ���Լ�����������
	 * @bKeyArrive		��ǰ�Ƿ��ǹؼ�֡�Ŀ�ʼ
	 * @[in, out] cur_sequence	�Ե�ǰ��sequence���й���
	 */
	virtual bool is_allow_send (bool bKeyArrive, unsigned int* cur_sequence, bool* reset, unsigned int tmstamp);

	/*
	 * ����һ���ص��ӿ�
	 */
	virtual void set_cb (RtpReliableSenderCB* cb, int param) { m_cb = cb; m_cb_params = param; }

protected:
	int m_cursor;
	int m_max_buf_size;
	SENDED_DATA** m_sended;						// ���ͺ�ȴ�ȷ�ϵİ�
	bool m_switch;								// ����� ��true-�� ������������ false-�ر� �������з��ͣ�
	unsigned int m_sealed_sequence;				// ������ر��ر�ʱ������ر�ǰ��sequence��������ش�ʱ���ָ�֮ǰ�����sequence
	unsigned int m_last_tmstamp;				// ��һ֡��ʱ���
	unsigned int m_key_frame_packs_stmp;		// ��ͬʱ��֡��ʱ���
	int m_max_switch_time;						// �趨�������ʱ��
	
	RtpReliableSenderCB* m_cb;					// �ص�
	int m_cb_params;							// �ص�����
	bool m_call_keyframe;						// ����ؼ�֡

	int m_successive_num_send;					// �����������͵İ�������Ϊ��߷��ʹ�������ݣ�

	std::list<unsigned long> m_recved_tm;		// ���յ��İ���ʱ������ȴ���Ϊack���ص��Է�
	scoped_ptr <CriticalSectionWrapper> m_critical;

	unsigned long m_last_recved_sequence;		// ��һ�ν��յ��İ���˳���
	std::list<RECVED_DATA*> m_recved_pack;		// ���յ��İ����л��棬�м�ȱ֡

	void log (const char* fmt, ...);
	void erase_succeed_outgoing_pack (unsigned long sequence);		// �ӷ��ͻ��������ָ��ʱ����İ����ð��Ѿ��ɹ��յ���Ӧ��
	void slide_send_window ();										// ��������
	void buffer_unsuccessive_frame (const char* data, int size, unsigned long pack_timestamp, unsigned long sequence);		// ���治������֡
	void reset_unsuccessive_frame (unsigned long sequence);																	// ���ò�������֡����

	void free_recved_pack ();										// �ͷ�m_recved_packռ�õ��ڴ�
	void check_for_increase_bandwidth ();							// ����Ƿ��б�Ҫ��ߴ���
};
}
#endif
