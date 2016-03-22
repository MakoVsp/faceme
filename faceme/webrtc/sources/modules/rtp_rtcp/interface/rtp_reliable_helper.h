/*
 * ��rtp������пɿ���ȷ��
 *
 */

#ifndef WEBRTC_MODULES_RTP_RELIABLE_HELPER_H_
#define WEBRTC_MODULES_RTP_RELIABLE_HELPER_H_

#include "modules/interface/module.h"
#include "modules/rtp_rtcp/interface/rtp_rtcp_defines.h"

#define USE_AUDIO_RTP_RELIABLE 0
#define USE_VIDEO_RTP_RELIABLE 1

namespace webrtc {
/*
 * RtpReliableHelper �Ļص��ӿ�
 */
class RtpReliableSenderCB {
public:
	virtual void RequestKeyFrame (int param) = 0;	// ��Ҫ���̷��͹ؼ�֡
	virtual void OnRTPNetChanged (int param, int lost) = 0;		// ��������仯֪ͨ
};

/*
 * �ṩһ���ɿ���rtp����
 */
class RtpReliableHelper {
public:
	static RtpReliableHelper* CreateRTPReliableHelper ();
	static void ReleaseRTPReliableHelper (RtpReliableHelper* rh);

	enum { VIDEO_MAX_BUF = 5,		// ��Ƶ������󳤶�10֡ ��1s��
		AUDIO_MAX_BUF = 100,		// ��Ƶ������󳤶�100֡
		VIDEO_KEY_FRAME_PACK_NUM = 10,	// ���֡��ֵİ��� 
	};

	/*
	 * �����緢��һ����ͬʱ�����ڱ��أ��ȴ�ȷ��
	 * ͬʱ�ط�û���յ�ȷ�ϵİ�
	 */
	virtual void SendOutgoingPack (const char* data, int size, int header_size, unsigned long timestamp, unsigned long sequence) = 0;
	/*
	 * ���յ�һ��������ack_timestamp ɾ���ڱ��ط��ͻ���İ�
	 * ͬʱ˳�򻯽��յ��İ�
	 * return false �ð����ظ����գ�����
	 */
	virtual bool RecvPack (const char* data, int size, unsigned long pack_timestamp, unsigned long sequence
		, unsigned long ack_sequence, unsigned long ack_sequence2, unsigned int ex_reset) = 0;
	/*
	 * ��ȡһ���ϴν��յ��İ���ʱ�������Ϊ ACK ʱ���
	 */
	virtual bool PopACKTimestamp (unsigned long* timestamp) = 0;
	/*
	 * ��ȡһ���ϴν��յ��İ���ʱ�������Ϊ ACK ʱ���
	 */
	virtual bool PopACKTimestamp2 (unsigned long* timestamp) = 0;
	/*
	 * ������Ͷ���
	 */
	virtual void ClearSend () = 0;
	/*
	 * �Ƿ��Ͷ�����
	 */
	virtual bool IsFull () = 0;
	/*
	 * ���÷��ͻ�������С
	 */
	virtual void SetMaxBufferSize (int size) = 0;
	/*
	 * �ҳ�һ����Ҫ�ط��İ� ��data����Ҫ�ͷţ�
	 */
	virtual bool PopResendPack (char** data, int* size, int* header_size, unsigned long* timestamp) = 0;
	/*
	 * ���ر��ػ���İ� �����ڽ��յ��İ�˳����жϵ��µģ�data���ص�ָ����Ҫ���ͷ�
	 * return false	û��
	 */
	virtual bool PopBufferedPack (char** data, int* size) = 0;

	/*
	 * Ϊ�˱��⿽�����ݣ���һ���ⲿ���Ƶ���
	 */
	virtual void Lock (bool bLock) = 0;

	/*
	 * ���Ƶ�ǰ�Ƿ���Լ�����������
	 * @bKeyArrive		��ǰ�Ƿ��ǹؼ�֡�Ŀ�ʼ
	 * @[in, out] cur_sequence	�Ե�ǰ��sequence���й���
	 * @[out] reset		true-֪ͨ�Է�����Ϊ�ؼ�֡
	 */
	virtual bool is_allow_send (bool bKeyArrive, unsigned int* cur_sequence, bool* reset, unsigned int tmstamp) = 0;
	/*
	 * log
	 */
	virtual void log (const char* fmt, ...) = 0;

	/*
	 * ����һ���ص��ӿ�
	 */
	virtual void set_cb (RtpReliableSenderCB* cb, int param) = 0;

};
}
#endif