#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "media_stream.h"

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>

#include<ctype.h>

#include "voe_base.h"
#include "voe_codec.h"
#include "voe_file.h"
#include "voe_network.h"
#include "voe_audio_processing.h"
#include "voe_volume_control.h"
#include "voe_hardware.h"
#include "voe_rtp_rtcp.h"
#include "voe_encryption.h"
#include "trace.h"
#include "common_types.h"
#include "voe_dtmf.h"
#include "voe_external_media.h"
#include <string>
#include "std_log.h"

using namespace webrtc;
void open_webrtc_trace (voip_log_fun_t pLog);

static voip_log_fun_t g_pLog = NULL;

void log_write (const char* c)
{
    if(g_pLog != NULL)
     (*g_pLog)(c);
    //printf("---------------------------------------------------\r\n");
 //   printf(c);
 //   printf("---------------------------------------------------\r\n");
}


extern "C" void init_log(voip_log_fun_t pLog)
{
    g_pLog = pLog;
 //  open_webrtc_trace(pLog);
}

// 指定标准日志输出函数
void voipLog(const std::string& str, int nType)
{
	if (g_pLog != NULL)
		(*g_pLog)(str.c_str());
}

/*
// yinbin add 2015.6.24
static bool rtpChannelLink(const char* szSvrIp
                           , int nSvrPort
                           , const char* szCliIp
                          , int nCliPort);
*/

typedef struct {
    enum NsModes noise; bool bnoise;		// 去噪 kNsDefault kNsConference kNsLowSuppression kNsModerateSuppression kNsHighSuppression kNsVeryHighSuppression
    enum EcModes aec;	bool baec;			// 回音抑制 kEcDefault kEcConference kEcAec kEcAecm
    enum AecmModes aecm;bool baecm_cng;
    bool bhipass_filter;					// 高频滤波
} APM_PARAMETERS;

APM_PARAMETERS* gAPM = NULL;

void init_default_apm_parameters () {
    if (gAPM != NULL)  {
        return;
    }

    gAPM = (APM_PARAMETERS*) malloc (sizeof (APM_PARAMETERS));
    memset (gAPM, 0, sizeof (APM_PARAMETERS));

    gAPM->noise = kNsDefault;
    gAPM->bnoise = false; //false
    gAPM->aec = kEcAecm; //kEcAecm
    gAPM->baec = true;
    gAPM->aecm = kAecmEarpiece;
    gAPM->baecm_cng = false;
    gAPM->bhipass_filter = false;
}

void free_apm_parameters () {
    if (gAPM == NULL) {
        return;
    }

    free (gAPM);
    gAPM = NULL;
}

// ----------------------------------------------------------------------------
//    VoEMediaProcess
// ----------------------------------------------------------------------------

class MediaProcessImpl : public VoEMediaProcess
{
public:
    MediaProcessImpl();
    virtual void Process(const int channel,
                         const ProcessingTypes type,
                         WebRtc_Word16 audio_10ms[],
                         const int length,
                         const int samplingFreqHz,
                         const bool stereo);
};

MediaProcessImpl::MediaProcessImpl()
{
}

void MediaProcessImpl::Process(const int channel,
                               const ProcessingTypes type,
                               WebRtc_Word16 audio_10ms[],
                               const int length,
                               const int samplingFreqHz,
                               const bool stereo)
{
    int x = rand() % 100;

    for (int i = 0; i < length; i++) {
        if (channel == -1) {
            if (type == kPlaybackAllChannelsMixed){
                // playout: scale up
                if (!stereo){
                    audio_10ms[i] = (audio_10ms[i] << 2);
                }
                else {
                    audio_10ms[2*i] = (audio_10ms[2*i] << 2);
                    audio_10ms[2*i+1] = (audio_10ms[2*i+1] << 2);
                }
            }
            else {
                // recording: emulate packet loss by "dropping" 10% of the packets
                if (x >= 0 && x < 10) {
                    if (!stereo) {
                        audio_10ms[i] = 0;
                    }
                    else {
                        audio_10ms[2*i] = 0;
                        audio_10ms[2*i+1] = 0;
                    }
                }
            }
        }
        else {
            if (type == kPlaybackPerChannel) {
                // playout: mute
                if (!stereo) {
                    audio_10ms[i] = 0;
                }
                else {
                    audio_10ms[2*i] = 0;
                    audio_10ms[2*i+1] = 0;
                }
            }
            else {
                // recording: emulate packet loss by "dropping" 50% of the packets
                if (x >= 0 && x < 50) {
                    if (!stereo) {
                        audio_10ms[i] = 0;
                    }
                    else {
                        audio_10ms[2*i] = 0;
                        audio_10ms[2*i+1] = 0;
                    }
                }
            }
        }
    }
}

typedef struct {
    VoiceEngine* ve;		// 对象指针
    VoEBase* base;			// 访问接口
    VoECodec* codec;
    VoEFile* file;
    VoENetwork* netw;
    VoEAudioProcessing* apm;
    VoEVolumeControl* volume;
    VoEHardware* hardware;
    VoERTP_RTCP* rtp_rtcp;
    VoEEncryption* encrypt;
    VoEDtmf* dtmf;
    VoEExternalMedia* veExternalMediaPtr;
    MediaProcessImpl * _externalMediaPtr;
    // 当前使用的通道
    int channel;
    int speaker;
    bool mute;
}VOICE_ENGINE_ENTRY;

VOICE_ENGINE_ENTRY* gVERecv = NULL;		// 接收

VOICE_ENGINE_ENTRY* new_voice_engine_entry () {
    VOICE_ENGINE_ENTRY* ve = (VOICE_ENGINE_ENTRY*) malloc (sizeof (VOICE_ENGINE_ENTRY));
    memset (ve, 0, sizeof (VOICE_ENGINE_ENTRY));
    ve->ve = VoiceEngine::Create();

    ve->file = VoEFile::GetInterface(ve->ve);
    ve->base = VoEBase::GetInterface(ve->ve);
    ve->codec = VoECodec::GetInterface(ve->ve);
    ve->netw = VoENetwork::GetInterface(ve->ve);
    ve->hardware = VoEHardware::GetInterface(ve->ve);
    ve->rtp_rtcp = VoERTP_RTCP::GetInterface(ve->ve);
    ve->encrypt = VoEEncryption::GetInterface(ve->ve);
    ve->apm = VoEAudioProcessing::GetInterface(ve->ve);
    ve->volume = VoEVolumeControl::GetInterface(ve->ve);
    ve->dtmf = VoEDtmf::GetInterface(ve->ve);
    ve->veExternalMediaPtr = VoEExternalMedia::GetInterface(ve->ve);
    ve->_externalMediaPtr = new MediaProcessImpl();

    ve->speaker = false;
    ve->mute = false;
    return ve;
}

#define SAFE_RELEASE(p)	\
    if (p != NULL) {	\
    p->Release ();	\
    }

void free_voice_engine_entry (VOICE_ENGINE_ENTRY* ve) {
    if (ve == NULL) {
        return;
    }

    SAFE_RELEASE (ve->file);
    SAFE_RELEASE (ve->base);
    SAFE_RELEASE (ve->codec);
    SAFE_RELEASE (ve->netw);
    SAFE_RELEASE (ve->hardware);
    SAFE_RELEASE (ve->rtp_rtcp);
    SAFE_RELEASE (ve->encrypt);
    SAFE_RELEASE (ve->apm);
    SAFE_RELEASE (ve->volume);
    SAFE_RELEASE (ve->dtmf);
    SAFE_RELEASE (ve->veExternalMediaPtr);

    VoiceEngine::Delete(ve->ve);
    free (ve);
}


/*
 * 设置声音去噪属性
 * @param name				名称 ("noise" "aecm" "hipass_filter"
 * @param value				值 (noise ("Unchanged Default Conference LowSuppression ModerateSuppression HighSuppression VeryHighSuppression")
 *								aecm ("QuietEarpieceOrHeadset Earpiece LoudEarpiece Speakerphone LoudSpeakerphone")
 *								hipass_filter ("1 0")
 * @param sended			1= 设置我自己的音频处理 0=设置远端的音频处理
 */
extern "C" int set_amp_params(const char* name, const char* value, int sended) {
    if (gAPM == NULL || gVERecv == NULL) {
        return -1;
    }

    if (strcmp (name, "noise") == 0) {
        if (strcmp (value, "Unchanged") == 0) {
            gAPM->noise = kNsUnchanged;
            gAPM->bnoise = false;
        }
        else if (strcmp (value, "Default") == 0) {
            gAPM->noise = kNsDefault;
            gAPM->bnoise = true;
        }
        else if (strcmp (value, "Conference") == 0) {
            gAPM->noise = kNsConference;
            gAPM->bnoise = true;
        }
        else if (strcmp (value, "LowSuppression") == 0) {
            gAPM->noise = kNsLowSuppression;
            gAPM->bnoise = true;
        }
        else if (strcmp (value, "ModerateSuppression") == 0) {
            gAPM->noise = kNsModerateSuppression;
            gAPM->bnoise = true;
        }
        else if (strcmp (value, "HighSuppression") == 0) {
            gAPM->noise = kNsHighSuppression;
            gAPM->bnoise = true;
        }
        else if (strcmp (value, "VeryHighSuppression") == 0) {
            gAPM->noise = kNsVeryHighSuppression;
            gAPM->bnoise = true;
        }
        else {
            return -1;
        }
        gVERecv->apm->SetNsStatus(gAPM->bnoise, gAPM->noise);
        return 0;
    }

    else if (strcmp (name, "aecm") == 0) {
        if (strcmp (value, "QuietEarpieceOrHeadset") == 0) {
            gAPM->aecm = kAecmQuietEarpieceOrHeadset;
            gAPM->baecm_cng = true;
        }
        else if (strcmp (value, "Earpiece") == 0) {
            gAPM->aecm = kAecmEarpiece;
            gAPM->baecm_cng = true;
        }
        else if (strcmp (value, "LoudEarpiece") == 0) {
            gAPM->aecm = kAecmLoudEarpiece;
            gAPM->baecm_cng = true;
        }
        else if (strcmp (value, "Speakerphone") == 0) {
            gAPM->aecm = kAecmSpeakerphone;
            gAPM->baecm_cng = true;
        }
        else if (strcmp (value, "LoudSpeakerphone") == 0) {
            gAPM->aecm = kAecmLoudSpeakerphone;
            gAPM->baecm_cng = true;
        }
        else {
            return -1;
        }
        gVERecv->apm->SetAecmMode (gAPM->aecm, gAPM->baecm_cng);
        return 0;
    }

    else if (strcmp (name, "hipass_filter") == 0) {
        int enable = atoi (value);
        gAPM->bhipass_filter = enable;
        gVERecv->apm->EnableHighPassFilter (gAPM->bhipass_filter);
        return 0;
    }
    return -1;
}

class TestTraceCallback : public TraceCallback {
    virtual void Print(const TraceLevel level, const char* traceString, const int length) {
        if (traceString) {
            char* cmd_print = new char[length+1];
            memcpy(cmd_print, traceString, length);
            cmd_print[length] = '\0';
            log_write (cmd_print);
            delete[] cmd_print;
        }
    }
} _gTrace;

void open_webrtc_trace (voip_log_fun_t pLog) {
    //Trace::CreateTrace ();
    //Trace::SetLevelFilter (webrtc::kTraceAll);
    //Trace::SetTraceCallback (&_gTrace);
    Trace::IniLog(pLog);
}

void close_webrtc_trace () {
    Trace::ReturnTrace ();
}

/*
 * 设置默认声音处理参数
 */
void set_apm_parameters (VoEAudioProcessing* apm, int channel) {
    apm->SetNsStatus(gAPM->bnoise, gAPM->noise);
    apm->SetEcStatus (gAPM->baec, gAPM->aec);
    apm->SetAecmMode (gAPM->aecm, gAPM->baecm_cng);
    apm->EnableHighPassFilter (gAPM->bhipass_filter);
    apm->SetAgcStatus(true);

    char tmp [128];
    sprintf (tmp, "~~~~~~~~ noice:%s aec:%s hipass filter:%s",
             gAPM->bnoise ? "true" : "false",
             gAPM->baec ? "true" : "false",
             gAPM->bhipass_filter ? "true" : "false");
    log_write (tmp);
}

#define FAIL_RETURN(ret_code, log)                          \
    if (ret_code < 0) {										\
    char tmp [1256];									\
    sprintf (tmp, "~~~~~~~~~~~~~ failed %s   err-code:%d", log, ret_code);  \
    log_write (tmp);										\
    return -1;                                          \
    }

class my_transportation : public Transport
{
public :
    my_transportation(VoENetwork* vo):netw(vo)
    {
    }

    int SendPacket(int channel,const void *data,int len);
    int SendRTCPPacket(int channel, const void *data, int len);
private :
    VoENetwork* netw ;
};

int my_transportation::SendPacket(int channel,const void *data,int len)
{
    netw->ReceivedRTPPacket(channel, data, len);
    return len;
}

int my_transportation::SendRTCPPacket(int channel, const void *data, int len)
{
    netw->ReceivedRTCPPacket(channel, data, len);
    return len;
}


extern "C" bool init_media(bool & bCancle)
{
    //return true;
    // 设置默认参数
    init_default_apm_parameters();
    log_write("\nafter init_default_apm_parameters\n");
	logE << "logE nafter init_default_apm_parameters";
    if (bCancle)
    {
        free_apm_parameters();
        return false;
    }

    gVERecv = new_voice_engine_entry();
    if (bCancle)
    {
        free_voice_engine_entry(gVERecv);
        gVERecv = NULL;
        free_apm_parameters();
        return false;
    }

    log_write("\nafter new_voice_engine_entry\n");
    FAIL_RETURN(gVERecv->base->Init(), "recv Init");
    log_write("\nInit suc\n");

    return true;
}

extern "C" void destroy_media()
{
    // return;
    if (gVERecv != NULL) {
        gVERecv->base->Terminate();
        free_voice_engine_entry (gVERecv);
        gVERecv = NULL;
    }

    // 关闭trace
    close_webrtc_trace ();
    free_apm_parameters ();
}


void clean_string(char *str)
{
    char *start = str - 1;
    char *end = str;
    char *p = str;
    while(*p)
    {
        switch(*p)
        {
        case ' ':
        case '\r':
        case '\n':
        {
            if(start + 1==p)
                start = p;
        }
            break;
        default:
            break;
        }
        ++p;
    }
    //现在来到了字符串的尾部 反向向前
    --p;
    ++start;
    if(*start == 0)
    {
        //已经到字符串的末尾了
        *str = 0 ;
        return;
    }
    end = p + 1;
    while(p > start)
    {
        switch(*p)
        {
        case ' ':
        case '\r':
        case '\n':
        {
            if(end - 1 == p)
                end = p;
        }
            break;
        default:
            break;
        }
        --p;
    }
    memmove(str,start,end-start);
    *(str + (int)end - (int)start) = 0;
}

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
                           , int local_port)
{

    if (gVERecv == NULL) {
        log_write ("设备未初始化-------------------------------error");
        //log_write("try start audio---------- 忽略重复打开\n");
        return -1;
    }
    // yinbin add 2015.6.24
  // if (!rtpChannelLink(dest_host, dest_port, local_ip, local_port))
   // {
     //   log_write("rtp channel link test fial.");
     //   return -1;
  //  }

    // 开启webrtc的log
    char ip_infor[20] = { 0 };
    snprintf(ip_infor, sizeof (ip_infor),"%s",dest_host);
    clean_string(ip_infor);

    char debug_infor[128] = { 0 };
    snprintf(debug_infor, sizeof (debug_infor), "try start audio: pl %d,%s dest_host:%s,ip_infor:%s local:%d", payload, payload_name, dest_host,ip_infor, dest_port, local_port);
    log_write (debug_infor);

    //init_default_apm_parameters();

    //gVERecv = new_voice_engine_entry();

    //   FAIL_RETURN(gVERecv->base->Init(), "recv Init");

    // gVERecv->speaker = false;
    //gVERecv->mute = false;

    gVERecv->channel = gVERecv->base->CreateChannel();
    log_write("\nafter CreateChannel\n");


    FAIL_RETURN(gVERecv->base->SetLocalReceiver(gVERecv->channel, local_port), "recv SetLocalReceiver");
    log_write("\nafter SetLocalReceiver\n");
    FAIL_RETURN(gVERecv->base->SetSendDestination(gVERecv->channel,dest_port , ip_infor), "recv SetSendDestination");
    log_write("\nafter SetSendDestination\n");

    //  FAIL_RETURN(gVERecv->hardware->SetLoudspeakerStatus(gVERecv->speaker), "recv SetLoudspeakerStatus");
    // log_write("\nafter SetLoudspeakerStatus\n");
     FAIL_RETURN(gVERecv->volume->SetSystemInputMute(gVERecv->mute), "SetSystemInputMute");
    log_write("\nafter SetSystemInputMute\n");
    FAIL_RETURN(gVERecv->volume->SetSpeakerVolume(204), "recv SetSpeakerVolume");
    log_write("\nafter SetSpeakerVolume\n");
    FAIL_RETURN(gVERecv->dtmf->SetSendTelephoneEventPayloadType(gVERecv->channel, 101), "SetSendTelephoneEventPayloadType 101");
    log_write("\nafter SetSendTelephoneEventPayloadType\n");
    set_apm_parameters(gVERecv->apm, gVERecv->channel);
    log_write("\nafter set_apm_parameters\n");

    // 设置编解码
    int codecs = gVERecv->codec->NumOfCodecs();
    log_write("\nafter NumOfCodecs\n");
    // char debug_code[128] = { 0 };
    CodecInst codec;
    for (int i = 0; i < codecs; i++) {
        //		CodecInst codec;
        gVERecv->codec->GetCodec(i, codec);
        //log_write ("++++++++");
        log_write("\n++++++++\n");
        //log_write (codec.plname);
        // memset(debug_code,0,128);

        // snprintf(debug_code, sizeof (debug_code), "codec.pltype: %d,codec.plname :%s: codec.plfreq :%d,codec.pacsize :%d ,codec.channels :%d,codec.rate :%d", codec.pltype,codec.plname,codec.plfreq,codec.pacsize,codec.channels,codec.rate);
        //log_write (debug_code);

        if (strcasecmp(codec.plname, payload_name) == 0) {
            codec.pltype = payload;											// codec 的 payload 和默认的可能并不相同
            gVERecv->codec->SetSendCodec(gVERecv->channel, codec);			// 所以需要重新注册类型
            gVERecv->codec->SetRecPayloadType(gVERecv->channel, codec);
            //log_write ("==========");
            log_write("\n==========\n");
            log_write (codec.plname);

            break;
        }
    }


   int ret = gVERecv->base->StartReceive(gVERecv->channel);
    if(ret >= 0) {
           log_write("\nafter StartReceive----success\n");
    }
    else {
           log_write("\nafter StartReceive---error\n");
           return -1;
    }

     ret = gVERecv->base->StartSend(gVERecv->channel);
    if(ret >= 0) {
           log_write("\nafter StartSend----success\n");
    }
    else {
           log_write("\nafter StartSend---error\n");
           return -1;
    }
    ret = gVERecv->base->StartPlayout(gVERecv->channel);
    if(ret >= 0) {
           log_write("\nafter StartPlayout----success\n");
    }
    else {
           log_write("\nafter StartPlayout---error\n");
           return -1;
    }

    return 0;
}

/*
 * 切换声音的输入和输出设备
 * @param record_source android AudioRecord中的streamType
 * @param track_source	android AudioTrack中的streamType
 */
extern "C" int switch_speaker(int open) {

    if (gVERecv == NULL) {
        return -1;
    }
  //  if (gVERecv->speaker == open) {
       // return 0;
   // }
    if(open == 0) {
         log_write("\nswitch_speaker-----------------------------------------------------------0");
    } else if (open == 2){
        log_write("\nswitch_speaker-----------------------------------------------------------2");
    } else if(open == 3){
        log_write("\nswitch_speaker-----------------------------------------------------------3");
    }  else if(open == 5){
            log_write("\nswitch_speaker-----------------------------------------------------------5");
        }else {
        log_write("\nswitch_speaker-----------------------------------------------------------error");
    }
    gVERecv->speaker = open;

    FAIL_RETURN (gVERecv->hardware->SetLoudspeakerStatus (gVERecv->speaker), "recv SetLoudspeakerStatus");

    // 如果开启扬声器，默认自动开启中度回音抑制 （如果不够，顶层手动调用新的回音抑制）
    if (open) {
        gAPM->aecm = kAecmSpeakerphone;
    }
    // 使用默认的听筒回音抑制
    else {
        gAPM->aecm = kAecmEarpiece;
    }

    gAPM->baecm_cng = true;
    gVERecv->apm->SetAecmMode (gAPM->aecm, gAPM->baecm_cng);
    return 1;
}



extern "C" void mute_audio(char mute) {
    if (gVERecv == NULL || gVERecv->volume == NULL) {
        return;
    }
    if (gVERecv->mute == mute) {
        return;
    }
    gVERecv->mute = mute;
    gVERecv->volume->SetSystemInputMute(mute);
}

/*
 * 关闭音频流
 */
extern "C" int stop_audio() {

    /*   if (gVERecv != NULL) {
        //ndk_log ("try close audio channel");
        gVERecv->base->DeleteChannel (gVERecv->channel);
        gVERecv->base->Terminate();
        free_voice_engine_entry (gVERecv);
        gVERecv = NULL;
    }

    // 关闭trace
    close_webrtc_trace ();
    free_apm_parameters ();
    return 0;
*/
    if (gVERecv != NULL){

          //gVERecv->mute = false;
         // gVERecv->volume->SetSystemInputMute(gVERecv->mute);

        //  gVERecv->speaker = 0;// 0 听筒
        // gVERecv->hardware->SetLoudspeakerStatus (gVERecv->speaker);

        log_write("\nstop------------------------------------------------------------------------------------------1");
        (gVERecv->base->StopSend(gVERecv->channel));
        log_write("\nstop------------------------------------------------------------------------------------------2");
        (gVERecv->base->StopPlayout(gVERecv->channel));
        log_write("\nstop------------------------------------------------------------------------------------------3");
        (gVERecv->base->StopReceive(gVERecv->channel));
        log_write("\nstop------------------------------------------------------------------------------------------4");
        gVERecv->base->DeleteChannel (gVERecv->channel);
        log_write("\nstop------------------------------------------------------------------------------------------5");

    }
    return 0;
}

extern "C" int dtmf(char* c)
{
    if (gVERecv == NULL
            || gVERecv->dtmf == NULL
            || gVERecv->channel < 0) {
        return -1;
    }

    int ecode;
    if (*c >= '0' && *c <= '9') {
        ecode = *c - '0';
    }
    else if (*c == '*') {
        ecode = 10;
    }
    else if (*c == '#') {
        ecode = 11;
    }
    else if (*c >= 'A' && *c <= 'D') {
        ecode = *c - 'A' + 12;
    }
    else if (*c == 'F') {
        ecode = 16;
    }
    else {
        log_write ("DTMF UNSUPPORT");
        return -1;
    }

    gVERecv->dtmf->SendTelephoneEvent (gVERecv->channel, ecode, false);
    return 0;
}


struct s_rtp_one{
    int nType;
    int nMediaIp;
    int nMediaPort;
}__attribute__((aligned(4)));




/* Auxiliary function that waits on the socket. */
static int wait_on_socket(int sockfd, int for_recv, long timeout_ms)
{
    struct timeval tv;
    fd_set infd, outfd, errfd;
    int res;

    tv.tv_sec = timeout_ms / 1000;
    tv.tv_usec = (timeout_ms % 1000) * 1000;

    FD_ZERO(&infd);
    FD_ZERO(&outfd);
    FD_ZERO(&errfd);

    FD_SET(sockfd, &errfd); /* always check for error */

    if (for_recv)
    {
        FD_SET(sockfd, &infd);
    }
    else
    {
        FD_SET(sockfd, &outfd);
    }

    /* select() returns the number of signalled sockets or -1 */
    res = select(sockfd + 1, &infd, &outfd, &errfd, &tv);
    return res;
}

bool rtpChannelLink(const char* szSvrIp
                    , int nSvrPort
                    , const char* szCliIp
                    , int nCliPort)
{
    log_write("begin rtp channel link");
    log_write(szSvrIp);
    log_write(szCliIp);
    const int fd = socket(AF_INET, SOCK_DGRAM, 0);

    // client
    struct sockaddr_in cliAddr;
    memset(&cliAddr, 0, sizeof(cliAddr));
    cliAddr.sin_family = AF_INET;
    cliAddr.sin_port = htons(nCliPort);		///local端口
    cliAddr.sin_addr.s_addr = INADDR_ANY;	///localip

    // server
    struct sockaddr_in svrAddr;
    memset(&svrAddr, 0, sizeof(svrAddr));
    svrAddr.sin_family = AF_INET;
    svrAddr.sin_port = htons(nSvrPort);		// port
    svrAddr.sin_addr.s_addr = inet_addr(szSvrIp);	// ip

    int opt = 1;

    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) != 0)
        {
         perror("Server setsockopt failed");
         return 1;
        }

    if (bind(fd, (sockaddr *)&cliAddr, sizeof(sockaddr)) != 0) {
        log_write("bind udp socket error");
        log_write(strerror(errno));
        shutdown(fd, SHUT_WR);
        close(fd);
        return false;
    }

    char sendbuf[13];
    char recvbuf[13];

    memset((char*)&sendbuf, 0, sizeof(sendbuf));
    memset((char*)&recvbuf, 0, sizeof(recvbuf));

    // build packet
    {
        s_rtp_one sro;
        sro.nType = 0xffffffff;
        sro.nMediaIp = inet_addr(szCliIp);
        sro.nMediaPort = htonl(nCliPort);
        memcpy(&sendbuf[0], (char*)&sro, sizeof(sro));
    }

    // set out of time
    struct timeval timeout = { 2, 0 };
    int sendret = setsockopt(fd, SOL_SOCKET, SO_SNDTIMEO, (const char*)&timeout, sizeof(timeout));
    if (sendret != 0)
    {
        log_write("设置发送超时失败");
        log_write(strerror(errno));
    }
    int recvret = setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout));
    if (recvret != 0)
    {
        log_write("设置接收超时失败");
        log_write(strerror(errno));
    }


    // send packet
    socklen_t svrAddrLen = sizeof(svrAddr);
    int nSendSize = sendto(fd, &sendbuf, sizeof(sendbuf), 0, (struct sockaddr *)&svrAddr, svrAddrLen);
    if (sizeof(sendbuf) != nSendSize)
    {
        log_write("发送rtp穿透包失败");
        log_write(strerror(errno));
        shutdown(fd, SHUT_WR);
        close(fd);
        return false;
    }
    log_write("rtp channel send suc");

    // wait for rcv.
    if(wait_on_socket(fd, 1, 1000) < 0)
    {
        log_write("select失败");
        log_write(strerror(errno));
        shutdown(fd, SHUT_WR);
        close(fd);
        return false;
    }

    // rcv packet
    int nRcvSize = recvfrom(fd, recvbuf, sizeof(s_rtp_one), 0, (struct sockaddr *)&svrAddr, &svrAddrLen);
    if (nRcvSize <= 0)
    {
        log_write("接收rtp穿透包失败");
        log_write(strerror(errno));
        shutdown(fd, SHUT_WR);
        close(fd);
        return false;
    }

    log_write("rtp channel rcv suc");

    shutdown(fd, SHUT_WR);
    close(fd);

    return true;
}
