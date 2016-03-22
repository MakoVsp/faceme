/*
 *  Copyright (c) 2012 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include "modules/bitrate_controller/send_side_bandwidth_estimation.h"

#include <math.h>  // sqrt()

#include "system_wrappers/interface/trace.h"

namespace webrtc {

SendSideBandwidthEstimation::SendSideBandwidthEstimation()
    : critsect_(CriticalSectionWrapper::CreateCriticalSection()),
      accumulate_lost_packets_Q8_(0),
      accumulate_expected_packets_(0),
      bitrate_(0),
      min_bitrate_configured_(0),
      max_bitrate_configured_(0),
      last_fraction_loss_(0),
      last_round_trip_time_(0),
      bwe_incoming_(0),
      time_last_increase_(0),
      time_last_decrease_(0) {
	  loss_times = 0;
	  last_loss_interv = 0x7fffffff;
}

SendSideBandwidthEstimation::~SendSideBandwidthEstimation() {
    delete critsect_;
}

void SendSideBandwidthEstimation::SetSendBitrate(const uint32_t bitrate) {
  CriticalSectionScoped cs(critsect_);
  bitrate_ = bitrate;
}

void SendSideBandwidthEstimation::SetMinMaxBitrate(const uint32_t min_bitrate,
                                                   const uint32_t max_bitrate) {
  CriticalSectionScoped cs(critsect_);
  min_bitrate_configured_ = min_bitrate;
  if (max_bitrate == 0) {
    // no max configured use 1Gbit/s
    max_bitrate_configured_ = 1000000000;
  } else {
    max_bitrate_configured_ = max_bitrate;
  }
}

bool SendSideBandwidthEstimation::UpdateBandwidthEstimate(
    const uint32_t bandwidth,
    uint32_t* new_bitrate,
    uint8_t* fraction_lost,
    uint16_t* rtt) {
  *new_bitrate = 0;
  CriticalSectionScoped cs(critsect_);

  bwe_incoming_ = bandwidth;

  if (bitrate_ == 0) {
    // SendSideBandwidthEstimation off
    return false;
  }
  if (bwe_incoming_ > 0 && bitrate_ > bwe_incoming_) {
    bitrate_ = bwe_incoming_;
    *new_bitrate = bitrate_;
    *fraction_lost = last_fraction_loss_;
    *rtt = last_round_trip_time_;
    return true;
  }
  return false;
}

bool SendSideBandwidthEstimation::UpdatePacketLoss(
    const int number_of_packets,
    const uint32_t rtt,
    const uint32_t now_ms,
    uint8_t* loss,
    uint32_t* new_bitrate) {
  CriticalSectionScoped cs(critsect_);

  if (bitrate_ == 0) {
    // SendSideBandwidthEstimation off
    return false;
  }
  // Update RTT.
  last_round_trip_time_ = rtt;

  // Check sequence number diff and weight loss report
  if (number_of_packets > 0) {
    // Calculate number of lost packets.
    const int num_lost_packets_Q8 = *loss * number_of_packets;
    // Accumulate reports.
    accumulate_lost_packets_Q8_ += num_lost_packets_Q8;
    accumulate_expected_packets_ += number_of_packets;

    // Report loss if the total report is based on sufficiently many packets.
    if (accumulate_expected_packets_ >= kLimitNumPackets) {
      *loss = accumulate_lost_packets_Q8_ / accumulate_expected_packets_;

      // Reset accumulators
      accumulate_lost_packets_Q8_ = 0;
      accumulate_expected_packets_ = 0;
    } else {
      // Report zero loss until we have enough data to estimate
      // the loss rate.
      return false;
    }
  }
  // Keep for next time.
  last_fraction_loss_ = *loss;
  uint32_t bitrate = 0;
  if (!ShapeSimple(*loss, rtt, now_ms, &bitrate)) {
    // No change.
    return false;
  }
  bitrate_ = bitrate;
  *new_bitrate = bitrate;
  return true;
}

bool SendSideBandwidthEstimation::AvailableBandwidth(
    uint32_t* bandwidth) const {
  CriticalSectionScoped cs(critsect_);
  if (bitrate_ == 0) {
    return false;
  }
  *bandwidth = bitrate_;
  return true;
}

/*
 * Calculate the rate that TCP-Friendly Rate Control (TFRC) would apply.
 * The formula in RFC 3448, Section 3.1, is used.
 */
uint32_t SendSideBandwidthEstimation::CalcTFRCbps(uint16_t rtt, uint8_t loss) {
  if (rtt == 0 || loss == 0) {
    // input variables out of range
    return 0;
  }
  double R = static_cast<double>(rtt) / 1000;  // RTT in seconds
  int b = 1;  // number of packets acknowledged by a single TCP acknowledgement;
              // recommended = 1
  double t_RTO = 4.0 * R;  // TCP retransmission timeout value in seconds
                           // recommended = 4*R
  double p = static_cast<double>(loss) / 255;  // packet loss rate in [0, 1)
  double s = static_cast<double>(kAvgPacketSizeBytes);

  // calculate send rate in bytes/second
  double X = s / (R * sqrt(2 * b * p / 3) +
      (t_RTO * (3 * sqrt(3 * b * p / 8) * p * (1 + 32 * p * p))));

  return (static_cast<uint32_t>(X * 8));  // bits/second
}

//extern "C" void ndk_log_ii (const char* fmt, int n1, int n2, int n3, int n4);

// 根据两个原则 （丢包后，在一定时间段内不增加带宽，这个时间段随时间增加而扩大）
bool SendSideBandwidthEstimation::in_threshold () {
	return (last_loss_interv / (loss_times + 1)) < 5;	// 门限
}

bool SendSideBandwidthEstimation::ShapeSimple(const uint8_t loss,
                                              const uint32_t rtt,
                                              const uint32_t now_ms,
                                              uint32_t* bitrate) {
  uint32_t new_bitrate = 0;
  bool reducing = false;

  if (loss <= 0) {
	  last_loss_interv ++;
  }
  else {
	  if (last_loss_interv > 1) {
		  loss_times ++;
	  }
	  last_loss_interv = 0;
  }

  // Limit the rate increases to once a kBWEIncreaseIntervalMs.
  if (loss <= 0 && !in_threshold ()) {
    if ((now_ms - time_last_increase_) < kBWEIncreaseIntervalMs) {
      return false;
    }
    time_last_increase_ = now_ms;
  }
  // Limit the rate decreases to once a kBWEDecreaseIntervalMs + rtt.
  if (loss > 0) {
    if ((now_ms - time_last_decrease_) < kBWEDecreaseIntervalMs + rtt) {
      return false;
    }
    time_last_decrease_ = now_ms;
  }

  if (loss <= 0 && in_threshold ()) {
    // 2% - 10%
    new_bitrate = bitrate_;
  } else if (loss > 0) {
    // 26/256 ~= 10%
    // reduce rate: newRate = rate * (1 - 0.5*lossRate)
    // packetLoss = 256*lossRate

	  // 接收到对方反馈的丢包信息时，已经有时间差，所以消除该时间差
	  if (bitrate_ < 400000) {
			new_bitrate = (bitrate_ - 1000) / 1.08;						// 先恢复为上一次
	  }
	  else {
		  new_bitrate = bitrate_ - 10000;
	  }

		if (loss < 100) {											// 1 - lossRate
			new_bitrate = static_cast<uint32_t>((new_bitrate *			
				static_cast<double>(256 - loss)) / 256.0);
		}
		else {														// 1 - 0.5 * lossrate
			new_bitrate = static_cast<uint32_t>((new_bitrate *			
				static_cast<double>(512 - loss)) / 512.0);
		}
		reducing = true;
  } else {
	  // 如果当前位速小于 400k, 采用倍速方式上涨
	  if (bitrate_ < 400000) {
			// increase rate by 8%
			new_bitrate = static_cast<uint32_t>(bitrate_ * 1.08 + 0.5);

			// add 1 kbps extra, just to make sure that we do not get stuck
			// (gives a little extra increase at low rates, negligible at higher rates)
			new_bitrate += 1000;
	  }
	  // 如果当前位速大于 400k, 采用等量增速(10k)方式上涨
	  else {
		  new_bitrate = bitrate_ + 10000;
	  }
  }
  if (reducing) {
    // Calculate what rate TFRC would apply in this situation
    // scale loss to Q0 (back to [0, 255])
    /*uint32_t tfrc_bitrate = CalcTFRCbps(rtt, loss);
    if (tfrc_bitrate > new_bitrate) {
      // do not reduce further if rate is below TFRC rate
      new_bitrate = tfrc_bitrate;
    }*/
  }
  if (bwe_incoming_ > 0 && new_bitrate > bwe_incoming_) {
    new_bitrate = bwe_incoming_;
  }
  if (new_bitrate > max_bitrate_configured_) {
    new_bitrate = max_bitrate_configured_;
  }
  if (new_bitrate < min_bitrate_configured_) {
    WEBRTC_TRACE(kTraceWarning, kTraceRtpRtcp, -1,
                 "The configured min bitrate (%u kbps) is greater than the "
                 "estimated available bandwidth (%u kbps).\n",
                 min_bitrate_configured_ / 1000, new_bitrate / 1000);
    new_bitrate = min_bitrate_configured_;
  }
  *bitrate = new_bitrate;
  //ndk_log_ii ("loss %d  bitrate new %d last_loss_interv %d loss_times %d", loss, new_bitrate, last_loss_interv, loss_times);

  // 长时间过后，如果由于丢包导致位速降低并稳定在到较低的水平，则尝试再次提高位速，尝试重新适应
  if (last_loss_interv < 0x7fffffff
	  && last_loss_interv > 20
	  && loss_times > 5
	  && new_bitrate < 250000) {
	  loss_times = 1;
  }

  return true;
}
}  // namespace webrtc

/* 原有的带宽估算算法允许丢包在2-10%之间，不屏蔽丢帧后的非关键帧，
这样的优点是 显示更加流畅，缺点是可能会有马赛克出现。
我改为了丢帧后丢弃后继所有的非关键帧，同时周期发送关键帧请求，直到接收到关键帧位置
这样做的优点是没有马赛克，缺点丢帧后会导致界面卡顿，
这种方法中必须要坚决降低丢帧的概率，所以把降低丢帧率为主要考虑对象。
bool SendSideBandwidthEstimation::ShapeSimple(const uint8_t loss,
                                              const uint32_t rtt,
                                              const uint32_t now_ms,
                                              uint32_t* bitrate) {
  uint32_t new_bitrate = 0;
  bool reducing = false;

  // Limit the rate increases to once a kBWEIncreaseIntervalMs.
  if (loss <= 5) {
    if ((now_ms - time_last_increase_) < kBWEIncreaseIntervalMs) {
      return false;
    }
    time_last_increase_ = now_ms;
  }
  // Limit the rate decreases to once a kBWEDecreaseIntervalMs + rtt.
  if (loss > 26) {
    if ((now_ms - time_last_decrease_) < kBWEDecreaseIntervalMs + rtt) {
      return false;
    }
    time_last_decrease_ = now_ms;
  }

  if (loss > 5 && loss <= 26) {
    // 2% - 10%
    new_bitrate = bitrate_;
  } else if (loss > 26) {
    // 26/256 ~= 10%
    // reduce rate: newRate = rate * (1 - 0.5*lossRate)
    // packetLoss = 256*lossRate
    new_bitrate = static_cast<uint32_t>((bitrate_ *
        static_cast<double>(512 - loss)) / 512.0);
    reducing = true;
  } else {
    // increase rate by 8%
    new_bitrate = static_cast<uint32_t>(bitrate_ * 1.08 + 0.5);

    // add 1 kbps extra, just to make sure that we do not get stuck
    // (gives a little extra increase at low rates, negligible at higher rates)
    new_bitrate += 1000;
  }
  if (reducing) {
    // Calculate what rate TFRC would apply in this situation
    // scale loss to Q0 (back to [0, 255])
    uint32_t tfrc_bitrate = CalcTFRCbps(rtt, loss);
    if (tfrc_bitrate > new_bitrate) {
      // do not reduce further if rate is below TFRC rate
      new_bitrate = tfrc_bitrate;
    }
  }
  if (bwe_incoming_ > 0 && new_bitrate > bwe_incoming_) {
    new_bitrate = bwe_incoming_;
  }
  if (new_bitrate > max_bitrate_configured_) {
    new_bitrate = max_bitrate_configured_;
  }
  if (new_bitrate < min_bitrate_configured_) {
    WEBRTC_TRACE(kTraceWarning, kTraceRtpRtcp, -1,
                 "The configured min bitrate (%u kbps) is greater than the "
                 "estimated available bandwidth (%u kbps).\n",
                 min_bitrate_configured_ / 1000, new_bitrate / 1000);
    new_bitrate = min_bitrate_configured_;
  }
  *bitrate = new_bitrate;
  return true;
}*/
