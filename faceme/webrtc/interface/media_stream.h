
#ifndef _MEDIA_STREAM_H
#define _MEDIA_STREAM_H


/*日志函数*/
typedef void(*voip_log_fun_t)(const char* str);

/*初始化日志函数.*/
extern "C" void init_log(voip_log_fun_t pLog);


extern "C" bool init_media(bool & bCancle);

extern "C" void destroy_media();

/*
 * 开启音频
 * @param payload           payload id
 * @param payload_name      "PCMU", ...
 * @param clock_rate        时钟频率 (8000, ...)
 * @param dest_host         接受方ip地址
 * @param dest_port         接受方端口
 * @param local_port        本地绑定端口
 * return                   0 成功
 */
extern "C" int start_audio(int payload
                           , const char* payload_name
                           , int clock_rate
                           , const char* dest_host
                           , int dest_port
                           , const char* local_ip
                           , int local_port);

/*
         * 切换声音的外放
         * @param open				0-切换到扬声器 2-切换到听筒 3 耳机
         */
extern "C" int switch_speaker(int open);

/*
 * 设置声音去噪属性
 * @param name				名称 ("noise" "aecm" "hipass_filter"
 * @param value				值 (noise ("Unchanged Default Conference LowSuppression ModerateSuppression HighSuppression VeryHighSuppression")
 *								aecm ("Earpiece LoudEarpiece Speakerphone LoudSpeakerphone")
 *								hipass_filter ("1 0")
 * @param sended			1= 设置我自己的音频处理 0=设置远端的音频处理
 */
extern "C" int set_amp_params(const char* name, const char* value, int sended);

/*
 * 对捕获音频静音
 * @param mute              mute = true 静音音频捕捉设备 false 放开静音
 */
extern "C" void mute_audio(char mute);

/*
 * 关闭音频流
 */
extern "C" int stop_audio();

extern "C" int dtmf(char* c);

/*
 *
 */
extern "C" bool rtpChannelLink(const char* szSvrIp
                    , int nSvrPort
                    , const char* szCliIp
                    , int nCliPort);
#endif
