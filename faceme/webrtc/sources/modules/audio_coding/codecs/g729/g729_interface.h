#ifndef _LK_G729_H
#define _LK_G729_H

#ifdef __cplusplus
extern "C" {
#endif

#include "typedefs.h"
#include "typedef.h"
#include "basic_op.h"
#include "ld8a.h"
#include "octet.h"

// forward declaration
struct G729_encinst_t_ {
	Word16 vad_enable;					// 1静音检测 0 不检测
	Word16 cur_frame_count;				// 当前解码的帧序号
	Word16 prm [PRM_SIZE+1];			// 储存参数分析
	Word16 serial [SERIAL_SIZE];		// 解码后的
};

struct G729_decinst_t_ {
	Word16 synth_buf [L_FRAME+M], *synth;		/* Synthesis                   */
	Word16 serial [SERIAL_SIZE];				/* Serialize				   */
	Word16 parm [PRM_SIZE+2];					/* Synthesis parameters        */
	Word16 Az_dec [MP1*2];						/* Decoded Az for post-filter  */
	Word16 T2 [2];								/* Pitch lag for 2 subframes   */
	Word32 count_frame;
};


int16_t WebRtcG729_CreateEnc(struct G729_encinst_t_** inst);
int16_t WebRtcG729_CreateDec(struct G729_decinst_t_** inst);
int16_t WebRtcG729_FreeEnc(struct G729_encinst_t_* inst);
int16_t WebRtcG729_FreeDec(struct G729_decinst_t_* inst);
int16_t WebRtcG729_Encode(struct G729_encinst_t_* encInst, int16_t* input,
                                      int16_t len, int16_t* output);
int16_t WebRtcG729_EncoderInit(struct G729_encinst_t_* encInst, int16_t mode);
int16_t WebRtcG729_Decode (struct G729_decinst_t_* decInst, WebRtc_Word16* encoded, WebRtc_Word16 len, WebRtc_Word16* decoded, WebRtc_Word16* speechType);
int16_t WebRtcG729_DecodeBwe(struct G729_decinst_t_* state, const WebRtc_UWord16 *encoded, WebRtc_Word32 packet_size,
                                      WebRtc_UWord16 rtp_seq_number, WebRtc_UWord32 send_ts, WebRtc_UWord32 arr_ts);
int16_t WebRtcG729_DecodePlc (struct G729_decinst_t_* decInst, WebRtc_Word16* decoded, WebRtc_Word16 frames);
int16_t WebRtcG729_DecoderInit(struct G729_decinst_t_* decInst);
	

#ifdef __cplusplus
}
#endif

#endif