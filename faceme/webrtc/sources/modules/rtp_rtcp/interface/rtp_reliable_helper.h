/*
 * 对rtp传输进行可靠性确认
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
 * RtpReliableHelper 的回调接口
 */
class RtpReliableSenderCB {
public:
	virtual void RequestKeyFrame (int param) = 0;	// 需要立刻发送关键帧
	virtual void OnRTPNetChanged (int param, int lost) = 0;		// 网络情况变化通知
};

/*
 * 提供一个可靠的rtp传输
 */
class RtpReliableHelper {
public:
	static RtpReliableHelper* CreateRTPReliableHelper ();
	static void ReleaseRTPReliableHelper (RtpReliableHelper* rh);

	enum { VIDEO_MAX_BUF = 5,		// 视频缓冲最大长度10帧 （1s）
		AUDIO_MAX_BUF = 100,		// 音频缓冲最大长度100帧
		VIDEO_KEY_FRAME_PACK_NUM = 10,	// 最大帧拆分的包数 
	};

	/*
	 * 向网络发送一包，同时缓存在本地，等待确认
	 * 同时重发没有收到确认的包
	 */
	virtual void SendOutgoingPack (const char* data, int size, int header_size, unsigned long timestamp, unsigned long sequence) = 0;
	/*
	 * 接收到一包，根据ack_timestamp 删除在本地发送缓存的包
	 * 同时顺序化接收到的包
	 * return false 该包被重复接收，抛弃
	 */
	virtual bool RecvPack (const char* data, int size, unsigned long pack_timestamp, unsigned long sequence
		, unsigned long ack_sequence, unsigned long ack_sequence2, unsigned int ex_reset) = 0;
	/*
	 * 获取一个上次接收到的包的时间戳，作为 ACK 时间戳
	 */
	virtual bool PopACKTimestamp (unsigned long* timestamp) = 0;
	/*
	 * 获取一个上次接收到的包的时间戳，作为 ACK 时间戳
	 */
	virtual bool PopACKTimestamp2 (unsigned long* timestamp) = 0;
	/*
	 * 清除发送队列
	 */
	virtual void ClearSend () = 0;
	/*
	 * 是否发送队列满
	 */
	virtual bool IsFull () = 0;
	/*
	 * 设置发送缓冲区大小
	 */
	virtual void SetMaxBufferSize (int size) = 0;
	/*
	 * 找出一个需要重发的包 （data不需要释放）
	 */
	virtual bool PopResendPack (char** data, int* size, int* header_size, unsigned long* timestamp) = 0;
	/*
	 * 返回本地缓存的包 （由于接收到的包顺序号中断导致的）data返回的指针需要被释放
	 * return false	没有
	 */
	virtual bool PopBufferedPack (char** data, int* size) = 0;

	/*
	 * 为了避免拷贝数据，加一个外部控制的锁
	 */
	virtual void Lock (bool bLock) = 0;

	/*
	 * 控制当前是否可以继续发送数据
	 * @bKeyArrive		当前是否是关键帧的开始
	 * @[in, out] cur_sequence	对当前的sequence进行管理
	 * @[out] reset		true-通知对方重置为关键帧
	 */
	virtual bool is_allow_send (bool bKeyArrive, unsigned int* cur_sequence, bool* reset, unsigned int tmstamp) = 0;
	/*
	 * log
	 */
	virtual void log (const char* fmt, ...) = 0;

	/*
	 * 设置一个回调接口
	 */
	virtual void set_cb (RtpReliableSenderCB* cb, int param) = 0;

};
}
#endif