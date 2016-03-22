/*
 *  Copyright (c) 2012 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include "rtp_receiver_audio.h"

#include <stdio.h> 
#include <cassert> //assert
#include <cstring> // memcpy()
#include <math.h>    // pow()

#include "critical_section_wrapper.h"

namespace webrtc {
RTPReceiverAudio::RTPReceiverAudio(const WebRtc_Word32 id):
    _id(id),
    _lastReceivedFrequency(8000),
    _telephoneEvent(false),
    _telephoneEventForwardToDecoder(false),
    _telephoneEventDetectEndOfTone(false),
    _telephoneEventPayloadType(-1),
    _cngNBPayloadType(-1),
    _cngWBPayloadType(-1),
    _cngSWBPayloadType(-1),
    _cngPayloadType(-1),
    _G722PayloadType(-1),
    _lastReceivedG722(false),
    _criticalSectionFeedback(CriticalSectionWrapper::CreateCriticalSection()),
    _cbAudioFeedback(NULL)
{
}

RTPReceiverAudio::~RTPReceiverAudio()
{
    delete _criticalSectionFeedback;
}

WebRtc_Word32
RTPReceiverAudio::RegisterIncomingAudioCallback(RtpAudioFeedback* incomingMessagesCallback)
{
    CriticalSectionScoped lock(_criticalSectionFeedback);
    _cbAudioFeedback = incomingMessagesCallback;
    return 0;
}

WebRtc_UWord32
RTPReceiverAudio::AudioFrequency() const
{
    if(_lastReceivedG722)
    {
        return 8000;
    }
    return _lastReceivedFrequency;
}

// Outband TelephoneEvent(DTMF) detection
WebRtc_Word32
RTPReceiverAudio::SetTelephoneEventStatus(const bool enable,
                                          const bool forwardToDecoder,
                                          const bool detectEndOfTone)
{
    _telephoneEvent= enable;
    _telephoneEventDetectEndOfTone = detectEndOfTone;
    _telephoneEventForwardToDecoder = forwardToDecoder;
    return 0;
}

 // Is outband TelephoneEvent(DTMF) turned on/off?
bool
RTPReceiverAudio::TelephoneEvent() const
{
    return _telephoneEvent;
}

// Is forwarding of outband telephone events turned on/off?
bool
RTPReceiverAudio::TelephoneEventForwardToDecoder() const
{
    return _telephoneEventForwardToDecoder;
}

bool
RTPReceiverAudio::TelephoneEventPayloadType(const WebRtc_Word8 payloadType) const
{
    return (_telephoneEventPayloadType == payloadType)?true:false;
}

bool
RTPReceiverAudio::CNGPayloadType(const WebRtc_Word8 payloadType,
                                 WebRtc_UWord32& frequency)
{
    //  we can have three CNG on 8000Hz, 16000Hz and 32000Hz
    if(_cngNBPayloadType == payloadType)
    {
        frequency = 8000;
        if ((_cngPayloadType != -1) &&(_cngPayloadType !=_cngNBPayloadType))
        {
            ResetStatistics();
        }
        _cngPayloadType = _cngNBPayloadType;
        return true;
    } else if(_cngWBPayloadType == payloadType)
    {
        // if last received codec is G.722 we must use frequency 8000
        if(_lastReceivedG722)
        {
            frequency = 8000;
        } else
        {
            frequency = 16000;
        }
        if ((_cngPayloadType != -1) &&(_cngPayloadType !=_cngWBPayloadType))
        {
            ResetStatistics();
        }
        _cngPayloadType = _cngWBPayloadType;
        return true;
    }else if(_cngSWBPayloadType == payloadType)
    {
        frequency = 32000;
        if ((_cngPayloadType != -1) &&(_cngPayloadType !=_cngSWBPayloadType))
        {
            ResetStatistics();
        }
        _cngPayloadType = _cngSWBPayloadType;
        return true;
    }else
    {
        //  not CNG
        if(_G722PayloadType == payloadType)
        {
            _lastReceivedG722 = true;
        }else
        {
            _lastReceivedG722 = false;
        }
    }
    return false;
}

/*
   Sample based or frame based codecs based on RFC 3551

   NOTE! There is one error in the RFC, stating G.722 uses 8 bits/samples.
   The correct rate is 4 bits/sample.

   name of                              sampling              default
   encoding  sample/frame  bits/sample      rate  ms/frame  ms/packet

   Sample based audio codecs
   DVI4      sample        4                var.                   20
   G722      sample        4              16,000                   20
   G726-40   sample        5               8,000                   20
   G726-32   sample        4               8,000                   20
   G726-24   sample        3               8,000                   20
   G726-16   sample        2               8,000                   20
   L8        sample        8                var.                   20
   L16       sample        16               var.                   20
   PCMA      sample        8                var.                   20
   PCMU      sample        8                var.                   20

   Frame based audio codecs
   G723      frame         N/A             8,000        30         30
   G728      frame         N/A             8,000       2.5         20
   G729      frame         N/A             8,000        10         20
   G729D     frame         N/A             8,000        10         20
   G729E     frame         N/A             8,000        10         20
   GSM       frame         N/A             8,000        20         20
   GSM-EFR   frame         N/A             8,000        20         20
   LPC       frame         N/A             8,000        20         20
   MPA       frame         N/A              var.      var.

   G7221     frame         N/A
*/

ModuleRTPUtility::Payload* RTPReceiverAudio::RegisterReceiveAudioPayload(
    const char payloadName[RTP_PAYLOAD_NAME_SIZE],
    const WebRtc_Word8 payloadType,
    const WebRtc_UWord32 frequency,
    const WebRtc_UWord8 channels,
    const WebRtc_UWord32 rate) {
  if (ModuleRTPUtility::StringCompare(payloadName, "telephone-event", 15)) {
    _telephoneEventPayloadType = payloadType;
  }
  if (ModuleRTPUtility::StringCompare(payloadName, "cn", 2)) {
    //  we can have three CNG on 8000Hz, 16000Hz and 32000Hz
    if(frequency == 8000){
      _cngNBPayloadType = payloadType;
    } else if(frequency == 16000) {
      _cngWBPayloadType = payloadType;
    } else if(frequency == 32000) {
      _cngSWBPayloadType = payloadType;
    } else {
      assert(false);
      return NULL;
    }
  }

  ModuleRTPUtility::Payload* payload = new ModuleRTPUtility::Payload;
  payload->name[RTP_PAYLOAD_NAME_SIZE - 1] = 0;
  strncpy(payload->name, payloadName, RTP_PAYLOAD_NAME_SIZE - 1);
printf( "payloadName = %s\n", payloadName );
  payload->typeSpecific.Audio.frequency = frequency;
  payload->typeSpecific.Audio.channels = channels;
  payload->typeSpecific.Audio.rate = rate;
  payload->audio = true;
  return payload;
}

// we are not allowed to have any critsects when calling CallbackOfReceivedPayloadData
WebRtc_Word32
RTPReceiverAudio::ParseAudioCodecSpecific(WebRtcRTPHeader* rtpHeader,
                                          const WebRtc_UWord8* payloadData,
                                          const WebRtc_UWord16 payloadLength,
                                          const ModuleRTPUtility::AudioPayload& audioSpecific,
                                          const bool isRED)
{
//		printf("RTPReceiverAudio::ParseAudioCodecSpecific\n");
//		printf("file = %s, func = %s, line = %d \n", __FILE__, __FUNCTION__, __LINE__ );
    WebRtc_UWord8 newEvents[MAX_NUMBER_OF_PARALLEL_TELEPHONE_EVENTS];
    WebRtc_UWord8 removedEvents[MAX_NUMBER_OF_PARALLEL_TELEPHONE_EVENTS];
    WebRtc_UWord8 numberOfNewEvents = 0;
    WebRtc_UWord8 numberOfRemovedEvents = 0;
    bool telephoneEventPacket = TelephoneEventPayloadType(rtpHeader->header.payloadType);

//		printf("file = %s, func = %s, line = %d \n", __FILE__, __FUNCTION__, __LINE__ );
    if(payloadLength == 0)
    {
        return 0;
    }

    {
        CriticalSectionScoped lock(_criticalSectionFeedback);

//		printf("file = %s, func = %s, line = %d \n", __FILE__, __FUNCTION__, __LINE__ );
        if(telephoneEventPacket)
        {
            // RFC 4733 2.3
            /*
                0                   1                   2                   3
                0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
                +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
                |     event     |E|R| volume    |          duration             |
                +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
            */
//		printf("file = %s, func = %s, line = %d \n", __FILE__, __FUNCTION__, __LINE__ );
            if(payloadLength % 4 != 0)
            {
                return -1;
            }
            WebRtc_UWord8 numberOfEvents = payloadLength / 4;

//		printf("file = %s, func = %s, line = %d \n", __FILE__, __FUNCTION__, __LINE__ );
            // sanity
            if(numberOfEvents >= MAX_NUMBER_OF_PARALLEL_TELEPHONE_EVENTS)
            {
                numberOfEvents = MAX_NUMBER_OF_PARALLEL_TELEPHONE_EVENTS;
            }
            for (int n = 0; n < numberOfEvents; n++)
            {
                bool end = (payloadData[(4*n)+1] & 0x80)? true:false;

                std::set<WebRtc_UWord8>::iterator event =
                    _telephoneEventReported.find(payloadData[4*n]);

//		printf("file = %s, func = %s, line = %d \n", __FILE__, __FUNCTION__, __LINE__ );
                if(event != _telephoneEventReported.end())
                {
                    // we have already seen this event
//		printf("file = %s, func = %s, line = %d \n", __FILE__, __FUNCTION__, __LINE__ );
                    if(end)
                    {
                        removedEvents[numberOfRemovedEvents]= payloadData[4*n];
                        numberOfRemovedEvents++;
                        _telephoneEventReported.erase(payloadData[4*n]);
                    }
                }else
                {
                    if(end)
                    {
//		printf("file = %s, func = %s, line = %d \n", __FILE__, __FUNCTION__, __LINE__ );
                        // don't add if it's a end of a tone
                    }else
                    {
//		printf("file = %s, func = %s, line = %d \n", __FILE__, __FUNCTION__, __LINE__ );
                        newEvents[numberOfNewEvents] = payloadData[4*n];
                        numberOfNewEvents++;
                        _telephoneEventReported.insert(payloadData[4*n]);
                    }
                }
            }

//		printf("file = %s, func = %s, line = %d \n", __FILE__, __FUNCTION__, __LINE__ );
            // RFC 4733 2.5.1.3 & 2.5.2.3 Long-Duration Events
            // should not be a problem since we don't care about the duration

            // RFC 4733 See 2.5.1.5. & 2.5.2.4.  Multiple Events in a Packet
        }

//		printf("file = %s, func = %s, line = %d \n", __FILE__, __FUNCTION__, __LINE__ );
        if(_telephoneEvent && _cbAudioFeedback)
        {
//		printf("file = %s, func = %s, line = %d \n", __FILE__, __FUNCTION__, __LINE__ );
            for (int n = 0; n < numberOfNewEvents; n++)
            {
//		printf("file = %s, func = %s, line = %d \n", __FILE__, __FUNCTION__, __LINE__ );
                _cbAudioFeedback->OnReceivedTelephoneEvent(_id, newEvents[n], false);
//		printf("file = %s, func = %s, line = %d \n", __FILE__, __FUNCTION__, __LINE__ );
            }
            if(_telephoneEventDetectEndOfTone)
            {
//		printf("file = %s, func = %s, line = %d \n", __FILE__, __FUNCTION__, __LINE__ );
                for (int n = 0; n < numberOfRemovedEvents; n++)
                {
//		printf("file = %s, func = %s, line = %d \n", __FILE__, __FUNCTION__, __LINE__ );
                    _cbAudioFeedback->OnReceivedTelephoneEvent(_id, removedEvents[n], true);
//		printf("file = %s, func = %s, line = %d \n", __FILE__, __FUNCTION__, __LINE__ );
                }
            }
        }
    }
//		printf("file = %s, func = %s, line = %d \n", __FILE__, __FUNCTION__, __LINE__ );
    if(! telephoneEventPacket )
    {
//		printf("file = %s, func = %s, line = %d \n", __FILE__, __FUNCTION__, __LINE__ );
        _lastReceivedFrequency = audioSpecific.frequency;
    }

//		printf("file = %s, func = %s, line = %d \n", __FILE__, __FUNCTION__, __LINE__ );
    // Check if this is a CNG packet, receiver might want to know
    WebRtc_UWord32 dummy;
    if(CNGPayloadType(rtpHeader->header.payloadType, dummy))
    {
//		printf("file = %s, func = %s, line = %d \n", __FILE__, __FUNCTION__, __LINE__ );
        rtpHeader->type.Audio.isCNG=true;
        rtpHeader->frameType = kAudioFrameCN;
    }else
    {
//		printf("file = %s, func = %s, line = %d \n", __FILE__, __FUNCTION__, __LINE__ );
        rtpHeader->frameType = kAudioFrameSpeech;
        rtpHeader->type.Audio.isCNG=false;
    }

//		printf("file = %s, func = %s, line = %d \n", __FILE__, __FUNCTION__, __LINE__ );
    // check if it's a DTMF event, hence something we can playout
    if(telephoneEventPacket)
    {
//		printf("file = %s, func = %s, line = %d \n", __FILE__, __FUNCTION__, __LINE__ );
        if(!_telephoneEventForwardToDecoder)
        {
//		printf("file = %s, func = %s, line = %d \n", __FILE__, __FUNCTION__, __LINE__ );
            // don't forward event to decoder
            return 0;
        }
        std::set<WebRtc_UWord8>::iterator first =
            _telephoneEventReported.begin();
        if(first != _telephoneEventReported.end() && *first > 15)
        {
//		printf("file = %s, func = %s, line = %d \n", __FILE__, __FUNCTION__, __LINE__ );
            // don't forward non DTMF events
            return 0;
        }
    }
    if(isRED && !(payloadData[0] & 0x80))
    {
//		printf("file = %s, func = %s, line = %d \n", __FILE__, __FUNCTION__, __LINE__ );
        // we recive only one frame packed in a RED packet remove the RED wrapper
        rtpHeader->header.payloadType = payloadData[0];

        // only one frame in the RED strip the one byte to help NetEq
        return CallbackOfReceivedPayloadData(payloadData+1,
                                             payloadLength-1,
                                             rtpHeader);
    }

//		printf("file = %s, func = %s, line = %d \n", __FILE__, __FUNCTION__, __LINE__ );
    rtpHeader->type.Audio.channel = audioSpecific.channels;
    return CallbackOfReceivedPayloadData(payloadData, payloadLength, rtpHeader);
}
} // namespace webrtc
