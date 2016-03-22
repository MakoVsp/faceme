#include "g729_interface.h"

#if 1

int16_t WebRtcG729_CreateEnc(struct G729_encinst_t_** inst) {
	return 0; 
}
int16_t WebRtcG729_CreateDec(struct G729_decinst_t_** inst)	 { 
	return 0; 
}
int16_t WebRtcG729_FreeEnc(struct G729_encinst_t_* inst)		 { 
	return 0; 
}
int16_t WebRtcG729_FreeDec(struct G729_decinst_t_* inst)			 { 
	return 0; 
}
int16_t WebRtcG729_Encode(struct G729_encinst_t_* encInst, int16_t* input,
                                      int16_t len, int16_t* output)		 { 
										  return 0; 
}
int16_t WebRtcG729_EncoderInit(struct G729_encinst_t_* encInst, int16_t mode)	 { 
	return 0; 
}
int16_t WebRtcG729_Decode (struct G729_decinst_t_* decInst, WebRtc_Word16* encoded, WebRtc_Word16 len, WebRtc_Word16* decoded, WebRtc_Word16* speechType)	 { 
	return 0; 
}
int16_t WebRtcG729_DecodeBwe(struct G729_decinst_t_* state, const WebRtc_UWord16 *encoded, WebRtc_Word32 packet_size,
                                      WebRtc_UWord16 rtp_seq_number, WebRtc_UWord32 send_ts, WebRtc_UWord32 arr_ts)		 { 
										  return 0; 
}
int16_t WebRtcG729_DecodePlc (struct G729_decinst_t_* decInst, WebRtc_Word16* decoded, WebRtc_Word16 frames)	 { 
	return 0; 
}
int16_t WebRtcG729_DecoderInit(struct G729_decinst_t_* decInst)		 { 
	return 0; 
}

#else
#include <stdio.h>
#include <string.h>
#include "dtx.h"


Word16 bad_lsf;        /* bad LSF indicator   */

int16_t WebRtcG729_CreateEnc(struct G729_encinst_t_** inst) {
	if (inst == NULL) {
		return -1;
	}

	*inst = (struct G729_encinst_t_*) malloc (sizeof (struct G729_encinst_t_));
	memset (*inst, 0, sizeof (struct G729_encinst_t_));

	(*inst)->vad_enable = 0;

	return 0;
}

int16_t WebRtcG729_CreateDec(struct G729_decinst_t_** inst) {
	if (inst == NULL) {
		return -1;
	}

	*inst = (struct G729_decinst_t_*) malloc (sizeof (struct G729_decinst_t_));
	memset (*inst, 0, sizeof (struct G729_decinst_t_));
	return 0;
}

int16_t WebRtcG729_FreeEnc(struct G729_encinst_t_* inst) {
	free (inst);
	return 0;
}

int16_t WebRtcG729_FreeDec(struct G729_decinst_t_* inst) {
	free (inst);
	return 0;
}

int16_t WebRtcG729_EncoderInit(struct G729_encinst_t_* encInst, int16_t mode) {
	Init_Pre_Process();
	Init_Coder_ld8a();
	Set_zero(encInst->prm, PRM_SIZE+1);

	/* for G.729B */
	//Init_Cod_cng();
	return 0;
}

void dump_block (char* inf, void* mem, int size) {
	int i=0;
	char tmp[2048] = { 0 };
	sprintf (tmp, "\r\n g729 %s:  ", inf);
	for (; i<size; i++) {
		unsigned char* cm = (unsigned char* ) mem;
		char h [7] = { 0 };
		sprintf (h, " %02x", cm[i]);
		strcat (tmp, h);
    }
}

/**
* Converts from bitstream (ITU bits) to word16 value
* @param no_of_bits number of bits to read
* @param bitstream array containing bits
* @retval decimal value of bit pattern
*/
static Word16 bin2int(Word16 no_of_bits, const Word16 *bitstream)
{
	Word16   value, i;
	Word16 bit;
	
	value = 0;
	for(i = 0; i < no_of_bits; i++){
		value <<= 1;
		bit = *bitstream++;
		if (bit == BIT_1){
			value += 1;
		}
	}
	return(value);
}


/**
* Writes Word16 value to bitstream
* @param value decimal value to write
* @param no_of_bits number of bits from value to write
* @param bitstream pointer to the destination stream (ITU bits)
*/
static void int2bin(Word16 value, Word16 no_of_bits, Word16 *bitstream)
{
	Word16 *pt_bitstream;
	Word16   i, bit;

	pt_bitstream = bitstream + no_of_bits;

	for (i = 0; i < no_of_bits; i++){
		bit = value & (Word16)0x0001;      /* get lsb */
		if (bit == 0){
			*--pt_bitstream = BIT_0;
		}
		else{
			*--pt_bitstream = BIT_1;
		}
		value >>= 1;
	}
}

/**
* Pack ITU bits into RTP stream
* @param ituBits ITU stream to pack (80 shorts)
* @param bitstream RTP bitstream (80 bits, 5 shorts, 10 bytes)
*/
static void pack_G729 (const Word16 ituBits[], uint8_t bitstream[]) {  
	Word16 word16, i;
	for(i=0; i<5; i++){
		word16 = bin2int(16, (Word16*)&ituBits[i*16]);
		bitstream[i*2] = word16>>8, bitstream[(i*2)+1] = (word16 & 0xFF);
	}
} 

static void pack_SID (const Word16 ituBits[], uint8_t bitstream[]) {  
	Word16 word16 = bin2int(16, ituBits);
	bitstream[0] = word16>>8, bitstream[1] = (word16 & 0xFF);
}

int16_t WebRtcG729_Encode(struct G729_encinst_t_* encInst, int16_t* input, int16_t len, int16_t* output) {
	if (len != L_FRAME) {
		log_n ("g729 enc input %d error", len);	
		return 0;
	}

	if (encInst->cur_frame_count == 32767) encInst->cur_frame_count = 256;
    else encInst->cur_frame_count++;

	extern Word16 *new_speech;
	memcpy (new_speech, input, sizeof (Word16) * L_FRAME);
	Pre_Process (new_speech, L_FRAME);

	Coder_ld8a (encInst->prm, encInst->cur_frame_count, encInst->vad_enable);
	prm2bits_ld8k (encInst->prm, encInst->serial);

	int size;
	if (encInst->serial[1] == RATE_8000) {
		pack_G729 (&encInst->serial [2], (uint8_t*) output);
		size = 10;
	}
	else if(encInst->serial[1] == RATE_SID_OCTET) {
		pack_SID (&encInst->serial [2], (uint8_t*) output);
		size = 2;
	}
	else {
		size = 0;
	}

	return size;
}

int16_t WebRtcG729_DecoderInit(struct G729_decinst_t_* decInst) {
	int i;
	for (i=0; i<M; i++) {
		decInst->synth_buf[i] = 0;
	}
	decInst->synth = decInst->synth_buf + M;
	bad_lsf = 0;
	decInst->count_frame = 0;

	Init_Decod_ld8a();
	Init_Post_Filter();
	Init_Post_Process();

	/* for G.729b */
	//Init_Dec_cng();
	return 0;
}

/**
* UnPack RTP bitstream containing SID frame as unpacked ITU stream
* @param bitstream RTP bitstream to unpack
* @param bits ITU bitstream used as destination (0 - BIT_0, 1 - BIT_1)
*/
static void unpack_SID(const uint8_t bitstream[], Word16 bits[]) {  
   *bits++ = SYNC_WORD; 
   *bits++ = RATE_SID_OCTET;
   int2bin((Word16)bitstream[0], 8, &bits[0]);
   int2bin((Word16)bitstream[1], 8, &bits[8]);
}


/**
* UnPack RTP bitstream as unpacked ITU stream
* @param bitstream RTP bitstream to unpack
* @param bits ITU bitstream used as destination (0 - BIT_0, 1 - BIT_1)
* @param len length of the RTP bitstream
*/
static void unpack_G729(const uint8_t bitstream[], Word16 bits[], int len)  
{  
	Word16 i;  
	*bits++ = SYNC_WORD;     /* bit[0], at receiver this bits indicates BFI */
	switch(len){
		case 10:
			*bits++ = SIZE_WORD;
			break;
		case 8: // RATE_6400
		case 15: //RATE_11800
		default:
			log_n ("unpack_G729 invalid lenght %d ", len);
			return;
	} 

	for(i=0; i<len; i++){
		int2bin(bitstream[i], 8, &bits[i*8]);
	}
}

int16_t WebRtcG729_Decode(struct G729_decinst_t_* decInst, WebRtc_Word16* encoded, WebRtc_Word16 len, WebRtc_Word16* decoded, WebRtc_Word16* speechType) {
	if (len <= 0 
		|| (len % 10 != 0 && len % 10 != 2)) {
		log_n ("g729 recv packsize err %d", len);	
		return 0;
	}

	// 填充
	if (len == 2) {
		unpack_SID (encoded, decInst->serial);
	}
	else {
		unpack_G729 (encoded, decInst->serial, 10);
	}

	bits2prm_ld8k (&decInst->serial [1], decInst->parm);

	/* This part was modified for version V1.3 */
	/* for speech and SID frames, the hardware detects frame erasures
	by checking if all bits are set to zero */
	/* for untransmitted frames, the hardware detects frame erasures
	by testing serial[0] */
	/* 对于第一个字节应该是兼容性考虑 否则为0 */
	decInst->parm[0] = 0;           /* No frame erasure */
	if(decInst->serial[1] != 0) {
		int j;
		for (j=0; j < decInst->serial[1]; j++){
			if (decInst->serial[j+2] == 0){
				decInst->parm[0] = 1;  /* frame erased     */
				break;
			}
		}
	}
	else if(decInst->serial[0] != SYNC_WORD){
		decInst->parm[0] = 1;
	}
	if(decInst->parm[1] == 1) {
		decInst->parm[5] = Check_Parity_Pitch (decInst->parm[4], decInst->parm[5]);
	}

	// 解码
	Word16 Vad;
	Decod_ld8a (decInst->parm, decInst->synth, decInst->Az_dec, decInst->T2, &Vad);
	Post_Filter (decInst->synth, decInst->Az_dec, decInst->T2, Vad);
	Post_Process (decInst->synth, L_FRAME);
	memcpy (decoded, decInst->synth, L_FRAME * sizeof (short));
	return L_FRAME;
}

int16_t WebRtcG729_DecodePlc (struct G729_decinst_t_* decInst, WebRtc_Word16* decoded, WebRtc_Word16 frames) {
	return 0;
}

/* * This function updates the estimate of the bandwidth.*/
int16_t WebRtcG729_DecodeBwe(struct G729_decinst_t_* state, const WebRtc_UWord16 *encoded, WebRtc_Word32 packet_size,
							 WebRtc_UWord16 rtp_seq_number, WebRtc_UWord32 send_ts, WebRtc_UWord32 arr_ts) {
	 return 0;
}
#endif
