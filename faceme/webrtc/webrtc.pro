#-------------------------------------------------
#
# Project created by QtCreator 2014-07-29T19:46:32
#
#-------------------------------------------------
# This is needed for using syberos-application module
include(../syberos.pri)

TEMPLATE = lib
TARGET = facemewebrtc

QT       -= gui
QT       -= core

#CONFIG += create_pc create_prl no_install_prl
DEFINES += FACEMEWEBRTC_LIBRARY

DEFINES += QT_NO_WARNING_OUTPUT
DEFINES += QT_NO_DEBUG_OUTPUT
DEFINES += MEDIA_STREAM_LIBRARY
DEFINES += WEBRTC_DTMF_DETECTION
DEFINES += WEBRTC_VOICE_ENGINE_EXTERNAL_MEDIA_API
DEFINES += WEBRTC_VOICE_ENGINE_FILE_API
DEFINES += WEBRTC_VOICE_ENGINE_CODEC_API
DEFINES += WEBRTC_VOICE_ENGINE_NETWORK_API
DEFINES += WEBRTC_VOICE_ENGINE_HARDWARE_API
DEFINES += WEBRTC_VOICE_ENGINE_RTP_RTCP_API
DEFINES += WEBRTC_VOICE_ENGINE_ENCRYPTION_API
DEFINES += WEBRTC_VOICE_ENGINE_AUDIO_PROCESSING_API
DEFINES += WEBRTC_VOICE_ENGINE_VOLUME_CONTROL_API
DEFINES += WEBRTC_VOICE_ENGINE_DTMF_API
DEFINES += WEBRTC_VOICE_ENGINE_CALL_REPORT_API
DEFINES += WEBRTC_LINUX
DEFINES += LINUX_PULSE
DEFINES += YUANXIN
DEFINES += __SSE__
DEFINES += __SSE2__
DEFINES += PJ_IS_BIG_ENDIAN
DEFINES += PJ_IS_LITTLE_ENDIAN
DEFINES += FIXED_POINT
DEFINES += WEBRTC_TARGET_PC
DEFINES += WEBRTC_THREAD_RR
DEFINES += WEBRTC_CLOCK_TYPE_REALTIME
DEFINES += WEBRTC_ARCH_ARM
DEFINES += WEBRTC_NS_FIXED
DEFINES += WEBRTC_CODEC_G729
DEFINES += NETEQ_G729_CODEC
DEFINES += WEBRTC_CODEC_ILBC
DEFINES += NETEQ_ILBC_CODEC
DEFINES += _USE_BIT64


LIBS += -ldl -lcrypt -g -fPIC  -lrt
LIBS += -lpthread
LIBS += -laudio-manager

#指定目标文件(obj)的存放目录
OBJECTS_DIR += ./tmp

#LIBS += /user/lib/

INCLUDEPATH += . \
          interface\
          sources\
          sources/common_audio/signal_processing/include\
          sources/common_audio/resampler/include\
          sources/common_audio/vad/include\
          sources/modules/audio_device/include\
          sources/modules/audio_device\
          sources/modules/audio_device/android	\
          sources/modules/audio_device/linux \
          sources/modules/audio_device/dummy\
          sources/modules/audio_processing/agc/include\
          sources/modules/audio_processing/agc\
          sources/modules/audio_processing/ns/include\
          sources/modules/audio_processing/aec/include\
          sources/modules/audio_processing/include\
          sources/modules/audio_processing/utility\
          sources/modules/audio_processing/aecm/include\
          sources/modules/bitrate_controller\
          sources/modules/bitrate_controller/include\
          sources/modules/interface\
          sources/modules/remote_bitrate_estimator\
          sources/modules/remote_bitrate_estimator/include\
          sources/modules/utility/interface \
          sources/system_wrappers/interface\
          sources/modules/audio_coding/main/interface \
          sources/modules/media_file/interface \
          sources/modules/rtp_rtcp/interface \
          sources/modules/udp_transport/interface \
          sources/modules/system_wrappers/interface\
          sources/modules/audio_conference_mixer/interface\
          sources/voice_engine/include\
          sources/modules\
          sources/modules/audio_coding/codecs/cng/include\
          sources/modules/audio_coding/codecs/g711/include \
          sources/modules/audio_coding/codecs/g722/include \
          sources/modules/audio_coding/codecs/g729 \
          sources/modules/audio_coding/codecs/ilbc/interface \
          sources/modules/audio_coding/codecs/isac/main/interface \
          sources/modules/audio_coding/codecs/isac/fix/interface \
          sources/modules/audio_coding/codecs/pcm16b/include \
          sources/modules/audio_coding/neteq/interface\
          sources/system_wrappers/source/spreadsortlib\
          vpx/include\
          sources/modules/audio_coding/codecs/ilbc

# Input
HEADERS += interface/media_stream.h \
           sources/common_types.h \
           sources/engine_configurations.h \
           sources/typedefs.h \
           sources/test/test_suite.h \
           sources/tools/simple_command_line_parser.h \
           sources/voice_engine/channel.h \
           sources/voice_engine/channel_manager.h \
           sources/voice_engine/channel_manager_base.h \
           sources/voice_engine/dtmf_inband.h \
           sources/voice_engine/dtmf_inband_queue.h \
           sources/voice_engine/level_indicator.h \
           sources/voice_engine/monitor_module.h \
           sources/voice_engine/output_mixer.h \
           sources/voice_engine/output_mixer_internal.h \
           sources/voice_engine/shared_data.h \
           sources/voice_engine/statistics.h \
           sources/voice_engine/transmit_mixer.h \
           sources/voice_engine/utility.h \
           sources/voice_engine/voe_audio_processing_impl.h \
           sources/voice_engine/voe_base_impl.h \
           sources/voice_engine/voe_call_report_impl.h \
           sources/voice_engine/voe_codec_impl.h \
           sources/voice_engine/voe_dtmf_impl.h \
           sources/voice_engine/voe_encryption_impl.h \
           sources/voice_engine/voe_external_media_impl.h \
           sources/voice_engine/voe_file_impl.h \
           sources/voice_engine/voe_hardware_impl.h \
           sources/voice_engine/voe_neteq_stats_impl.h \
           sources/voice_engine/voe_network_impl.h \
           sources/voice_engine/voe_rtp_rtcp_impl.h \
           sources/voice_engine/voe_video_sync_impl.h \
           sources/voice_engine/voe_volume_control_impl.h \
           sources/voice_engine/voice_engine_defines.h \
           sources/voice_engine/voice_engine_impl.h \
           sources/common_audio/signal_processing/resample_by_2_internal.h \
           sources/common_audio/vad/vad_core.h \
           sources/common_audio/vad/vad_filterbank.h \
           sources/common_audio/vad/vad_gmm.h \
           sources/common_audio/vad/vad_sp.h \
           sources/common_audio/vad/vad_unittest.h \
           sources/modules/audio_device/audio_device_buffer.h \
           sources/modules/audio_device/audio_device_config.h \
           sources/modules/audio_device/audio_device_generic.h \
           sources/modules/audio_device/audio_device_impl.h \
           sources/modules/audio_device/audio_device_utility.h \
           sources/modules/audio_processing/audio_buffer.h \
           sources/modules/audio_processing/audio_processing_impl.h \
           sources/modules/audio_processing/echo_cancellation_impl.h \
           sources/modules/audio_processing/echo_control_mobile_impl.h \
           sources/modules/audio_processing/gain_control_impl.h \
           sources/modules/audio_processing/high_pass_filter_impl.h \
           sources/modules/audio_processing/level_estimator_impl.h \
           sources/modules/audio_processing/noise_suppression_impl.h \
           sources/modules/audio_processing/processing_component.h \
           sources/modules/audio_processing/splitting_filter.h \
           sources/modules/audio_processing/voice_detection_impl.h \
           sources/modules/bitrate_controller/bitrate_controller_impl.h \
           sources/modules/bitrate_controller/send_side_bandwidth_estimation.h \
           sources/modules/interface/module.h \
           sources/modules/interface/module_common_types.h \
           sources/modules/remote_bitrate_estimator/bitrate_estimator.h \
           sources/modules/remote_bitrate_estimator/overuse_detector.h \
           sources/modules/remote_bitrate_estimator/remote_bitrate_estimator_multi_stream.h \
           sources/modules/remote_bitrate_estimator/remote_bitrate_estimator_single_stream.h \
           sources/modules/remote_bitrate_estimator/remote_rate_control.h \
           sources/system_wrappers/interface/aligned_malloc.h \
           sources/system_wrappers/interface/atomic32.h \
           sources/system_wrappers/interface/compile_assert.h \
           sources/system_wrappers/interface/condition_variable_wrapper.h \
           sources/system_wrappers/interface/constructor_magic.h \
           sources/system_wrappers/interface/cpu_features_wrapper.h \
           sources/system_wrappers/interface/cpu_info.h \
           sources/system_wrappers/interface/cpu_wrapper.h \
           sources/system_wrappers/interface/critical_section_wrapper.h \
           sources/system_wrappers/interface/data_log.h \
           sources/system_wrappers/interface/data_log_c.h \
           sources/system_wrappers/interface/data_log_impl.h \
           sources/system_wrappers/interface/event_wrapper.h \
           sources/system_wrappers/interface/file_wrapper.h \
           sources/system_wrappers/interface/fix_interlocked_exchange_pointer_win.h \
           sources/system_wrappers/interface/list_wrapper.h \
           sources/system_wrappers/interface/map_wrapper.h \
           sources/system_wrappers/interface/ref_count.h \
           sources/system_wrappers/interface/rw_lock_wrapper.h \
           sources/system_wrappers/interface/scoped_ptr.h \
           sources/system_wrappers/interface/scoped_refptr.h \
           sources/system_wrappers/interface/sleep.h \
           sources/system_wrappers/interface/sort.h \
           sources/system_wrappers/interface/static_instance.h \
           sources/system_wrappers/interface/thread_wrapper.h \
           sources/system_wrappers/interface/tick_util.h \
           sources/system_wrappers/interface/trace.h \
           sources/system_wrappers/source/condition_variable_posix.h \
           sources/system_wrappers/source/condition_variable_win.h \
           sources/system_wrappers/source/cpu_linux.h \
           sources/system_wrappers/source/cpu_mac.h \
           sources/system_wrappers/source/cpu_measurement_harness.h \
           sources/system_wrappers/source/cpu_win.h \
           sources/system_wrappers/source/critical_section_posix.h \
           sources/system_wrappers/source/critical_section_win.h \
           sources/system_wrappers/source/data_log_c_helpers_unittest.h \
           sources/system_wrappers/source/event_posix.h \
           sources/system_wrappers/source/event_win.h \
           sources/system_wrappers/source/file_impl.h \
           sources/system_wrappers/source/list_no_stl.h \
           sources/system_wrappers/source/list_stl.h \
           sources/system_wrappers/source/map_no_stl.h \
           sources/system_wrappers/source/rw_lock_generic.h \
           sources/system_wrappers/source/rw_lock_posix.h \
           sources/system_wrappers/source/rw_lock_win.h \
           sources/system_wrappers/source/set_thread_name_win.h \
           sources/system_wrappers/source/thread_posix.h \
           sources/system_wrappers/source/thread_win.h \
           sources/system_wrappers/source/trace_impl.h \
           sources/system_wrappers/source/trace_posix.h \
           sources/system_wrappers/source/trace_win.h \
           sources/system_wrappers/source/unittest_utilities.h \
           sources/test/testsupport/fileutils.h \
           sources/test/testsupport/frame_reader.h \
           sources/test/testsupport/frame_writer.h \
           sources/test/testsupport/gtest_prod_util.h \
           sources/test/testsupport/packet_reader.h \
           sources/test/testsupport/unittest_utils.h \
           sources/tools/converter/converter.h \
           sources/tools/frame_analyzer/video_quality_analysis.h \
           sources/voice_engine/include/voe_audio_processing.h \
           sources/voice_engine/include/voe_base.h \
           sources/voice_engine/include/voe_call_report.h \
           sources/voice_engine/include/voe_codec.h \
           sources/voice_engine/include/voe_dtmf.h \
           sources/voice_engine/include/voe_encryption.h \
           sources/voice_engine/include/voe_errors.h \
           sources/voice_engine/include/voe_external_media.h \
           sources/voice_engine/include/voe_file.h \
           sources/voice_engine/include/voe_hardware.h \
           sources/voice_engine/include/voe_neteq_stats.h \
           sources/voice_engine/include/voe_network.h \
           sources/voice_engine/include/voe_rtp_rtcp.h \
           sources/voice_engine/include/voe_video_sync.h \
           sources/voice_engine/include/voe_volume_control.h \
           sources/common_audio/resampler/include/resampler.h \
           sources/common_audio/signal_processing/include/real_fft.h \
           sources/common_audio/signal_processing/include/signal_processing_library.h \
           sources/common_audio/signal_processing/include/spl_inl.h \
           sources/common_audio/signal_processing/include/spl_inl_armv7.h \
           sources/common_audio/vad/include/webrtc_vad.h \
           sources/modules/audio_coding/neteq/automode.h \
           sources/modules/audio_coding/neteq/buffer_stats.h \
           sources/modules/audio_coding/neteq/codec_db.h \
           sources/modules/audio_coding/neteq/codec_db_defines.h \
           sources/modules/audio_coding/neteq/delay_logging.h \
           sources/modules/audio_coding/neteq/dsp.h \
           sources/modules/audio_coding/neteq/dsp_helpfunctions.h \
           sources/modules/audio_coding/neteq/dtmf_buffer.h \
           sources/modules/audio_coding/neteq/dtmf_tonegen.h \
           sources/modules/audio_coding/neteq/mcu.h \
           sources/modules/audio_coding/neteq/mcu_dsp_common.h \
           sources/modules/audio_coding/neteq/neteq_defines.h \
           sources/modules/audio_coding/neteq/neteq_error_codes.h \
           sources/modules/audio_coding/neteq/neteq_statistics.h \
           sources/modules/audio_coding/neteq/packet_buffer.h \
           sources/modules/audio_coding/neteq/rtcp.h \
           sources/modules/audio_coding/neteq/rtp.h \
           sources/modules/audio_conference_mixer/interface/audio_conference_mixer.h \
           sources/modules/audio_conference_mixer/interface/audio_conference_mixer_defines.h \
           sources/modules/audio_conference_mixer/source/audio_conference_mixer_impl.h \
           sources/modules/audio_conference_mixer/source/audio_frame_manipulator.h \
           sources/modules/audio_conference_mixer/source/level_indicator.h \
           sources/modules/audio_conference_mixer/source/memory_pool.h \
           sources/modules/audio_conference_mixer/source/memory_pool_posix.h \
           sources/modules/audio_conference_mixer/source/memory_pool_win.h \
           sources/modules/audio_conference_mixer/source/time_scheduler.h \
           sources/modules/audio_device/android/audio_device_android_jni.h \
           sources/modules/audio_device/android/audio_device_android_opensles.h \
           sources/modules/audio_device/android/audio_device_utility_android.h \
           sources/modules/audio_device/dummy/audio_device_dummy.h \
           sources/modules/audio_device/dummy/audio_device_utility_dummy.h \
           sources/modules/audio_device/include/audio_device.h \
           sources/modules/audio_device/include/audio_device_defines.h \
           sources/modules/audio_device/ios/audio_device_ios.h \
           sources/modules/audio_device/ios/audio_device_utility_ios.h \
           sources/modules/audio_device/linux/alsasymboltable_linux.h \
           sources/modules/audio_device/linux/audio_device_alsa_linux.h \
           sources/modules/audio_device/linux/audio_device_pulse_linux.h \
           sources/modules/audio_device/linux/audio_device_utility_linux.h \
           sources/modules/audio_device/linux/audio_mixer_manager_alsa_linux.h \
           sources/modules/audio_device/linux/audio_mixer_manager_pulse_linux.h \
           sources/modules/audio_device/linux/latebindingsymboltable_linux.h \
           sources/modules/audio_device/linux/pulseaudiosymboltable_linux.h \
           sources/modules/audio_device/mac/audio_device_mac.h \
           sources/modules/audio_device/mac/audio_device_utility_mac.h \
           sources/modules/audio_device/mac/audio_mixer_manager_mac.h \
           sources/modules/audio_device/test/audio_device_test_defines.h \
           sources/modules/audio_device/test/func_test_manager.h \
           sources/modules/audio_device/win/audio_device_core_win.h \
           sources/modules/audio_device/win/audio_device_utility_win.h \
           sources/modules/audio_device/win/audio_device_wave_win.h \
           sources/modules/audio_device/win/audio_mixer_manager_win.h \
           sources/modules/audio_processing/aec/aec_core.h \
           sources/modules/audio_processing/aec/aec_rdft.h \
           sources/modules/audio_processing/aec/aec_resampler.h \
#           sources/modules/audio_processing/aec/echo_cancellation_internal.h \
           sources/modules/audio_processing/aecm/aecm_core.h \
           sources/modules/audio_processing/aecm/aecm_core_neon_offsets.h \
           sources/modules/audio_processing/aecm/aecm_defines.h \
           sources/modules/audio_processing/agc/analog_agc.h \
           sources/modules/audio_processing/agc/digital_agc.h \
           sources/modules/audio_processing/include/audio_processing.h \
           sources/modules/audio_processing/ns/defines.h \
           sources/modules/audio_processing/ns/ns_core.h \
           sources/modules/audio_processing/ns/nsx_core.h \
           sources/modules/audio_processing/ns/nsx_core_neon_offsets.h \
           sources/modules/audio_processing/ns/nsx_defines.h \
           sources/modules/audio_processing/ns/windows_private.h \
           sources/modules/audio_processing/utility/delay_estimator.h \
           sources/modules/audio_processing/utility/delay_estimator_internal.h \
           sources/modules/audio_processing/utility/delay_estimator_wrapper.h \
           sources/modules/audio_processing/utility/fft4g.h \
           sources/modules/audio_processing/utility/ring_buffer.h \
           sources/modules/bitrate_controller/include/bitrate_controller.h \
           sources/modules/media_file/interface/media_file.h \
           sources/modules/media_file/interface/media_file_defines.h \
           sources/modules/media_file/source/avi_file.h \
           sources/modules/media_file/source/media_file_impl.h \
           sources/modules/media_file/source/media_file_utility.h \
           sources/modules/remote_bitrate_estimator/include/bwe_defines.h \
           sources/modules/remote_bitrate_estimator/include/remote_bitrate_estimator.h \
           sources/modules/remote_bitrate_estimator/include/rtp_to_ntp.h \
           sources/modules/rtp_rtcp/interface/rtp_reliable_helper.h \
           sources/modules/rtp_rtcp/interface/rtp_rtcp.h \
           sources/modules/rtp_rtcp/interface/rtp_rtcp_defines.h \
           sources/modules/rtp_rtcp/mocks/mock_rtp_rtcp.h \
           sources/modules/rtp_rtcp/source/Bitrate.h \
           sources/modules/rtp_rtcp/source/dtmf_queue.h \
           sources/modules/rtp_rtcp/source/fec_private_tables_bursty.h \
           sources/modules/rtp_rtcp/source/fec_private_tables_random.h \
           sources/modules/rtp_rtcp/source/fec_test_helper.h \
           sources/modules/rtp_rtcp/source/forward_error_correction.h \
           sources/modules/rtp_rtcp/source/forward_error_correction_internal.h \
           sources/modules/rtp_rtcp/source/producer_fec.h \
           sources/modules/rtp_rtcp/source/receiver_fec.h \
           sources/modules/rtp_rtcp/source/rtcp_receiver.h \
           sources/modules/rtp_rtcp/source/rtcp_receiver_help.h \
           sources/modules/rtp_rtcp/source/rtcp_sender.h \
           sources/modules/rtp_rtcp/source/rtcp_utility.h \
           sources/modules/rtp_rtcp/source/rtp_format_vp8.h \
           sources/modules/rtp_rtcp/source/rtp_format_vp8_test_helper.h \
           sources/modules/rtp_rtcp/source/rtp_header_extension.h \
           sources/modules/rtp_rtcp/source/rtp_packet_history.h \
           sources/modules/rtp_rtcp/source/rtp_receiver.h \
           sources/modules/rtp_rtcp/source/rtp_receiver_audio.h \
           sources/modules/rtp_rtcp/source/rtp_receiver_video.h \
           sources/modules/rtp_rtcp/source/rtp_reliable_helper_impl.h \
           sources/modules/rtp_rtcp/source/rtp_rtcp_config.h \
           sources/modules/rtp_rtcp/source/rtp_rtcp_impl.h \
           sources/modules/rtp_rtcp/source/rtp_sender.h \
           sources/modules/rtp_rtcp/source/rtp_sender_audio.h \
           sources/modules/rtp_rtcp/source/rtp_sender_video.h \
           sources/modules/rtp_rtcp/source/rtp_utility.h \
           sources/modules/rtp_rtcp/source/ssrc_database.h \
           sources/modules/rtp_rtcp/source/tmmbr_help.h \
           sources/modules/rtp_rtcp/source/transmission_bucket.h \
           sources/modules/rtp_rtcp/source/video_codec_information.h \
           sources/modules/rtp_rtcp/source/vp8_partition_aggregator.h \
           sources/modules/udp_transport/interface/udp_transport.h \
           sources/modules/udp_transport/source/traffic_control_windows.h \
           sources/modules/udp_transport/source/udp_socket2_manager_windows.h \
           sources/modules/udp_transport/source/udp_socket2_windows.h \
           sources/modules/udp_transport/source/udp_socket_manager_posix.h \
           sources/modules/udp_transport/source/udp_socket_manager_wrapper.h \
           sources/modules/udp_transport/source/udp_socket_posix.h \
           sources/modules/udp_transport/source/udp_socket_wrapper.h \
           sources/modules/udp_transport/source/udp_transport_impl.h \
           sources/modules/utility/interface/audio_frame_operations.h \
           sources/modules/utility/interface/file_player.h \
           sources/modules/utility/interface/file_recorder.h \
           sources/modules/utility/interface/process_thread.h \
           sources/modules/utility/interface/rtp_dump.h \
           sources/modules/utility/source/coder.h \
           sources/modules/utility/source/file_player_impl.h \
           sources/modules/utility/source/file_recorder_impl.h \
           sources/modules/utility/source/frame_scaler.h \
           sources/modules/utility/source/process_thread_impl.h \
           sources/modules/utility/source/rtp_dump_impl.h \
           sources/modules/utility/source/video_coder.h \
           sources/modules/utility/source/video_frames_queue.h \
           sources/system_wrappers/source/android/cpu-features.h \
           sources/system_wrappers/source/spreadsortlib/constants.hpp \
           sources/system_wrappers/source/spreadsortlib/spreadsort.hpp \
           sources/test/libtest/include/bit_flip_encryption.h \
           sources/test/libtest/include/random_encryption.h \
           sources/test/testsupport/mac/run_threaded_main_mac.h \
           sources/test/testsupport/metrics/video_metrics.h \
           sources/test/testsupport/mock/mock_frame_reader.h \
           sources/test/testsupport/mock/mock_frame_writer.h \
           sources/voice_engine/include/mock/mock_voe_connection_observer.h \
           sources/voice_engine/include/mock/mock_voe_observer.h \
           sources/voice_engine/test/auto_test/automated_mode.h \
           sources/voice_engine/test/auto_test/resource_manager.h \
           sources/voice_engine/test/auto_test/voe_cpu_test.h \
           sources/voice_engine/test/auto_test/voe_extended_test.h \
           sources/voice_engine/test/auto_test/voe_standard_test.h \
           sources/voice_engine/test/auto_test/voe_stress_test.h \
           sources/voice_engine/test/auto_test/voe_test_defines.h \
           sources/voice_engine/test/auto_test/voe_test_interface.h \
           sources/voice_engine/test/auto_test/voe_unit_test.h \
           sources/voice_engine/test/win_test/Resource.h \
           sources/voice_engine/test/win_test/stdafx.h \
           sources/voice_engine/test/win_test/WinTest.h \
           sources/voice_engine/test/win_test/WinTestDlg.h \
           sources/modules/audio_coding/codecs/cng/cng_helpfuns.h \
           sources/modules/audio_coding/codecs/g711/g711.h \
           sources/modules/audio_coding/codecs/g722/g722_enc_dec.h \
           sources/modules/audio_coding/codecs/g729/basic_op.h \
           sources/modules/audio_coding/codecs/g729/dtx.h \
           sources/modules/audio_coding/codecs/g729/g729_interface.h \
           sources/modules/audio_coding/codecs/g729/ld8a.h \
           sources/modules/audio_coding/codecs/g729/octet.h \
           sources/modules/audio_coding/codecs/g729/oper_32b.h \
           sources/modules/audio_coding/codecs/g729/sid.h \
           sources/modules/audio_coding/codecs/g729/tab_dtx.h \
           sources/modules/audio_coding/codecs/g729/tab_ld8a.h \
           sources/modules/audio_coding/codecs/g729/typedef.h \
           sources/modules/audio_coding/codecs/g729/vad.h \
           sources/modules/audio_coding/codecs/ilbc/abs_quant.h \
           sources/modules/audio_coding/codecs/ilbc/abs_quant_loop.h \
           sources/modules/audio_coding/codecs/ilbc/augmented_cb_corr.h \
           sources/modules/audio_coding/codecs/ilbc/bw_expand.h \
           sources/modules/audio_coding/codecs/ilbc/cb_construct.h \
           sources/modules/audio_coding/codecs/ilbc/cb_mem_energy.h \
           sources/modules/audio_coding/codecs/ilbc/cb_mem_energy_augmentation.h \
           sources/modules/audio_coding/codecs/ilbc/cb_mem_energy_calc.h \
           sources/modules/audio_coding/codecs/ilbc/cb_search.h \
           sources/modules/audio_coding/codecs/ilbc/cb_search_core.h \
           sources/modules/audio_coding/codecs/ilbc/cb_update_best_index.h \
           sources/modules/audio_coding/codecs/ilbc/chebyshev.h \
           sources/modules/audio_coding/codecs/ilbc/comp_corr.h \
           sources/modules/audio_coding/codecs/ilbc/constants.h \
           sources/modules/audio_coding/codecs/ilbc/create_augmented_vec.h \
           sources/modules/audio_coding/codecs/ilbc/decode.h \
           sources/modules/audio_coding/codecs/ilbc/decode_residual.h \
           sources/modules/audio_coding/codecs/ilbc/decoder_interpolate_lsf.h \
           sources/modules/audio_coding/codecs/ilbc/defines.h \
           sources/modules/audio_coding/codecs/ilbc/do_plc.h \
           sources/modules/audio_coding/codecs/ilbc/encode.h \
           sources/modules/audio_coding/codecs/ilbc/energy_inverse.h \
           sources/modules/audio_coding/codecs/ilbc/enh_upsample.h \
           sources/modules/audio_coding/codecs/ilbc/enhancer.h \
           sources/modules/audio_coding/codecs/ilbc/enhancer_interface.h \
           sources/modules/audio_coding/codecs/ilbc/filtered_cb_vecs.h \
           sources/modules/audio_coding/codecs/ilbc/frame_classify.h \
           sources/modules/audio_coding/codecs/ilbc/gain_dequant.h \
           sources/modules/audio_coding/codecs/ilbc/gain_quant.h \
           sources/modules/audio_coding/codecs/ilbc/get_cd_vec.h \
           sources/modules/audio_coding/codecs/ilbc/get_lsp_poly.h \
           sources/modules/audio_coding/codecs/ilbc/get_sync_seq.h \
           sources/modules/audio_coding/codecs/ilbc/hp_input.h \
           sources/modules/audio_coding/codecs/ilbc/hp_output.h \
           sources/modules/audio_coding/codecs/ilbc/index_conv_dec.h \
           sources/modules/audio_coding/codecs/ilbc/index_conv_enc.h \
           sources/modules/audio_coding/codecs/ilbc/init_decode.h \
           sources/modules/audio_coding/codecs/ilbc/init_encode.h \
           sources/modules/audio_coding/codecs/ilbc/interpolate.h \
           sources/modules/audio_coding/codecs/ilbc/interpolate_samples.h \
           sources/modules/audio_coding/codecs/ilbc/lpc_encode.h \
           sources/modules/audio_coding/codecs/ilbc/lsf_check.h \
           sources/modules/audio_coding/codecs/ilbc/lsf_interpolate_to_poly_dec.h \
           sources/modules/audio_coding/codecs/ilbc/lsf_interpolate_to_poly_enc.h \
           sources/modules/audio_coding/codecs/ilbc/lsf_to_lsp.h \
           sources/modules/audio_coding/codecs/ilbc/lsf_to_poly.h \
           sources/modules/audio_coding/codecs/ilbc/lsp_to_lsf.h \
           sources/modules/audio_coding/codecs/ilbc/my_corr.h \
           sources/modules/audio_coding/codecs/ilbc/nearest_neighbor.h \
           sources/modules/audio_coding/codecs/ilbc/pack_bits.h \
           sources/modules/audio_coding/codecs/ilbc/poly_to_lsf.h \
           sources/modules/audio_coding/codecs/ilbc/poly_to_lsp.h \
           sources/modules/audio_coding/codecs/ilbc/refiner.h \
           sources/modules/audio_coding/codecs/ilbc/simple_interpolate_lsf.h \
           sources/modules/audio_coding/codecs/ilbc/simple_lpc_analysis.h \
           sources/modules/audio_coding/codecs/ilbc/simple_lsf_dequant.h \
           sources/modules/audio_coding/codecs/ilbc/simple_lsf_quant.h \
           sources/modules/audio_coding/codecs/ilbc/smooth.h \
           sources/modules/audio_coding/codecs/ilbc/smooth_out_data.h \
           sources/modules/audio_coding/codecs/ilbc/sort_sq.h \
           sources/modules/audio_coding/codecs/ilbc/split_vq.h \
           sources/modules/audio_coding/codecs/ilbc/state_construct.h \
           sources/modules/audio_coding/codecs/ilbc/state_search.h \
           sources/modules/audio_coding/codecs/ilbc/swap_bytes.h \
           sources/modules/audio_coding/codecs/ilbc/unpack_bits.h \
           sources/modules/audio_coding/codecs/ilbc/vq3.h \
           sources/modules/audio_coding/codecs/ilbc/vq4.h \
           sources/modules/audio_coding/codecs/ilbc/window32_w32.h \
           sources/modules/audio_coding/codecs/ilbc/xcorr_coef.h \
           sources/modules/audio_coding/main/interface/audio_coding_module.h \
           sources/modules/audio_coding/main/interface/audio_coding_module_typedefs.h \
           sources/modules/audio_coding/main/source/acm_amr.h \
           sources/modules/audio_coding/main/source/acm_amrwb.h \
           sources/modules/audio_coding/main/source/acm_celt.h \
           sources/modules/audio_coding/main/source/acm_cng.h \
           sources/modules/audio_coding/main/source/acm_codec_database.h \
           sources/modules/audio_coding/main/source/acm_common_defs.h \
           sources/modules/audio_coding/main/source/acm_dtmf_detection.h \
           sources/modules/audio_coding/main/source/acm_dtmf_playout.h \
           sources/modules/audio_coding/main/source/acm_g722.h \
           sources/modules/audio_coding/main/source/acm_g7221.h \
           sources/modules/audio_coding/main/source/acm_g7221c.h \
           sources/modules/audio_coding/main/source/acm_g729.h \
           sources/modules/audio_coding/main/source/acm_g7291.h \
           sources/modules/audio_coding/main/source/acm_generic_codec.h \
           sources/modules/audio_coding/main/source/acm_gsmfr.h \
           sources/modules/audio_coding/main/source/acm_ilbc.h \
           sources/modules/audio_coding/main/source/acm_isac.h \
           sources/modules/audio_coding/main/source/acm_isac_macros.h \
           sources/modules/audio_coding/main/source/acm_neteq.h \
           sources/modules/audio_coding/main/source/acm_opus.h \
           sources/modules/audio_coding/main/source/acm_pcm16b.h \
           sources/modules/audio_coding/main/source/acm_pcma.h \
           sources/modules/audio_coding/main/source/acm_pcmu.h \
           sources/modules/audio_coding/main/source/acm_red.h \
           sources/modules/audio_coding/main/source/acm_resampler.h \
           sources/modules/audio_coding/main/source/acm_speex.h \
           sources/modules/audio_coding/main/source/audio_coding_module_impl.h \
           sources/modules/audio_coding/main/test/ACMTest.h \
           sources/modules/audio_coding/main/test/APITest.h \
           sources/modules/audio_coding/main/test/Channel.h \
           sources/modules/audio_coding/main/test/EncodeDecodeTest.h \
           sources/modules/audio_coding/main/test/iSACTest.h \
           sources/modules/audio_coding/main/test/PCMFile.h \
           sources/modules/audio_coding/main/test/RTPFile.h \
           sources/modules/audio_coding/main/test/SpatialAudio.h \
           sources/modules/audio_coding/main/test/TestAllCodecs.h \
           sources/modules/audio_coding/main/test/TestFEC.h \
           sources/modules/audio_coding/main/test/TestStereo.h \
           sources/modules/audio_coding/main/test/TestVADDTX.h \
           sources/modules/audio_coding/main/test/TimedTrace.h \
           sources/modules/audio_coding/main/test/TwoWayCommunication.h \
           sources/modules/audio_coding/main/test/utility.h \
           sources/modules/audio_coding/neteq/interface/webrtc_neteq.h \
           sources/modules/audio_coding/neteq/interface/webrtc_neteq_help_macros.h \
           sources/modules/audio_coding/neteq/interface/webrtc_neteq_internal.h \
           sources/modules/audio_coding/neteq/test/NETEQTEST_CodecClass.h \
           sources/modules/audio_coding/neteq/test/NETEQTEST_DummyRTPpacket.h \
           sources/modules/audio_coding/neteq/test/NETEQTEST_NetEQClass.h \
           sources/modules/audio_coding/neteq/test/NETEQTEST_RTPpacket.h \
           sources/modules/audio_coding/neteq/test/PayloadTypes.h \
           sources/modules/audio_conference_mixer/test/FunctionTest/functionTest.h \
           sources/modules/audio_device/mac/portaudio/pa_memorybarrier.h \
           sources/modules/audio_device/mac/portaudio/pa_ringbuffer.h \
           sources/modules/audio_processing/aec/include/echo_cancellation.h \
           sources/modules/audio_processing/aec/include/emmintrin.h \
#           sources/modules/audio_processing/aec/include/xmmintrin.h \
           sources/modules/audio_processing/aecm/include/echo_control_mobile.h \
           sources/modules/audio_processing/agc/include/gain_control.h \
           sources/modules/audio_processing/ns/include/noise_suppression.h \
           sources/modules/audio_processing/ns/include/noise_suppression_x.h \
           sources/modules/remote_bitrate_estimator/include/mock/mock_remote_bitrate_observer.h \
           sources/modules/rtp_rtcp/source/H264/bitstream_builder.h \
           sources/modules/rtp_rtcp/source/H264/bitstream_parser.h \
           sources/modules/rtp_rtcp/source/H264/h264_information.h \
           sources/modules/rtp_rtcp/source/H264/rtp_sender_h264.h \
           sources/modules/rtp_rtcp/source/mock/mock_rtp_receiver_video.h \
           sources/modules/rtp_rtcp/test/BWEStandAlone/BWEConvergenceTest.h \
           sources/modules/rtp_rtcp/test/BWEStandAlone/BWEStabilityTest.h \
           sources/modules/rtp_rtcp/test/BWEStandAlone/BWETestBase.h \
           sources/modules/rtp_rtcp/test/BWEStandAlone/BWETwoWayLimitFinding.h \
           sources/modules/rtp_rtcp/test/BWEStandAlone/MatlabPlot.h \
           sources/modules/rtp_rtcp/test/BWEStandAlone/TestLoadGenerator.h \
           sources/modules/rtp_rtcp/test/BWEStandAlone/TestSenderReceiver.h \
           sources/modules/rtp_rtcp/test/testAPI/test_api.h \
           sources/voice_engine/test/auto_test/fakes/fake_external_transport.h \
           sources/voice_engine/test/auto_test/fakes/fake_media_process.h \
           sources/voice_engine/test/auto_test/fixtures/after_initialization_fixture.h \
           sources/voice_engine/test/auto_test/fixtures/after_streaming_fixture.h \
           sources/voice_engine/test/auto_test/fixtures/before_initialization_fixture.h \
           sources/modules/audio_coding/codecs/cng/include/webrtc_cng.h \
           sources/modules/audio_coding/codecs/cng/test/StdAfx.h \
           sources/modules/audio_coding/codecs/g711/include/g711_interface.h \
           sources/modules/audio_coding/codecs/g722/include/g722_interface.h \
           sources/modules/audio_coding/codecs/ilbc/interface/ilbc.h \
           sources/modules/audio_coding/codecs/pcm16b/include/pcm16b.h \
           sources/voice_engine/test/android/android_test/jni/org_webrtc_voiceengine_test_AndroidTest.h \
           sources/modules/audio_coding/codecs/isac/fix/interface/isacfix.h \
           sources/modules/audio_coding/codecs/isac/fix/source/arith_routins.h \
           sources/modules/audio_coding/codecs/isac/fix/source/bandwidth_estimator.h \
           sources/modules/audio_coding/codecs/isac/fix/source/codec.h \
           sources/modules/audio_coding/codecs/isac/fix/source/entropy_coding.h \
           sources/modules/audio_coding/codecs/isac/fix/source/fft.h \
           sources/modules/audio_coding/codecs/isac/fix/source/filterbank_internal.h \
           sources/modules/audio_coding/codecs/isac/fix/source/filterbank_tables.h \
           sources/modules/audio_coding/codecs/isac/fix/source/lpc_masking_model.h \
           sources/modules/audio_coding/codecs/isac/fix/source/lpc_tables.h \
           sources/modules/audio_coding/codecs/isac/fix/source/pitch_estimator.h \
           sources/modules/audio_coding/codecs/isac/fix/source/pitch_gain_tables.h \
           sources/modules/audio_coding/codecs/isac/fix/source/pitch_lag_tables.h \
           sources/modules/audio_coding/codecs/isac/fix/source/settings.h \
           sources/modules/audio_coding/codecs/isac/fix/source/spectrum_ar_model_tables.h \
           sources/modules/audio_coding/codecs/isac/fix/source/structs.h \
           sources/modules/audio_coding/codecs/isac/main/interface/isac.h \
           sources/modules/audio_coding/codecs/isac/main/source/arith_routines.h \
           sources/modules/audio_coding/codecs/isac/main/source/bandwidth_estimator.h \
           sources/modules/audio_coding/codecs/isac/main/source/codec.h \
           sources/modules/audio_coding/codecs/isac/main/source/crc.h \
           sources/modules/audio_coding/codecs/isac/main/source/encode_lpc_swb.h \
           sources/modules/audio_coding/codecs/isac/main/source/entropy_coding.h \
           sources/modules/audio_coding/codecs/isac/main/source/fft.h \
           sources/modules/audio_coding/codecs/isac/main/source/filterbank_tables.h \
           sources/modules/audio_coding/codecs/isac/main/source/lpc_analysis.h \
           sources/modules/audio_coding/codecs/isac/main/source/lpc_gain_swb_tables.h \
           sources/modules/audio_coding/codecs/isac/main/source/lpc_shape_swb12_tables.h \
           sources/modules/audio_coding/codecs/isac/main/source/lpc_shape_swb16_tables.h \
           sources/modules/audio_coding/codecs/isac/main/source/lpc_tables.h \
           sources/modules/audio_coding/codecs/isac/main/source/os_specific_inline.h \
           sources/modules/audio_coding/codecs/isac/main/source/pitch_estimator.h \
           sources/modules/audio_coding/codecs/isac/main/source/pitch_gain_tables.h \
           sources/modules/audio_coding/codecs/isac/main/source/pitch_lag_tables.h \
           sources/modules/audio_coding/codecs/isac/main/source/settings.h \
           sources/modules/audio_coding/codecs/isac/main/source/spectrum_ar_model_tables.h \
           sources/modules/audio_coding/codecs/isac/main/source/structs.h \
           sources/modules/audio_coding/codecs/isac/main/test/debugUtility.h \
           sources/modules/audio_coding/codecs/isac/main/util/utility.h \
           sources/modules/audio_device/test/android/audio_device_android_test/jni/org_webrtc_voiceengine_test_AudioDeviceAndroidTest.h


SOURCES +=  sources/modules/audio_processing/utility/fft4g.c\
            sources/modules/audio_processing/utility/ring_buffer.c\
            sources/modules/audio_processing/utility/delay_estimator.c\
            sources/modules/audio_processing/utility/delay_estimator_wrapper.c\
            #	interface/assert.c\
            sources/modules/audio_processing/ns/noise_suppression_x.c\
            sources/modules/audio_processing/ns/nsx_core.c\
            sources/common_audio/signal_processing/auto_corr_to_refl_coef.c \
            sources/common_audio/signal_processing/auto_correlation.c \
            sources/common_audio/signal_processing/complex_fft.c \
            sources/common_audio/signal_processing/copy_set_operations.c \
            sources/common_audio/signal_processing/cross_correlation.c \
            sources/common_audio/signal_processing/division_operations.c \
            sources/common_audio/signal_processing/dot_product_with_scale.c \
            sources/common_audio/signal_processing/downsample_fast.c \
            sources/common_audio/signal_processing/energy.c \
            sources/common_audio/signal_processing/filter_ar.c \
            sources/common_audio/signal_processing/filter_ma_fast_q12.c \
            sources/common_audio/signal_processing/get_hanning_window.c \
            sources/common_audio/signal_processing/get_scaling_square.c \
            sources/common_audio/signal_processing/ilbc_specific_functions.c \
            sources/common_audio/signal_processing/levinson_durbin.c \
            sources/common_audio/signal_processing/lpc_to_refl_coef.c \
            sources/common_audio/signal_processing/min_max_operations.c \
            sources/common_audio/signal_processing/randomization_functions.c \
            sources/common_audio/signal_processing/real_fft.c \
            sources/common_audio/signal_processing/refl_coef_to_lpc.c \
            sources/common_audio/signal_processing/resample.c \
            sources/common_audio/signal_processing/resample_48khz.c \
            sources/common_audio/signal_processing/resample_by_2.c \
            sources/common_audio/signal_processing/resample_by_2_internal.c \
            sources/common_audio/signal_processing/resample_fractional.c \
            sources/common_audio/signal_processing/spl_init.c \
            sources/common_audio/signal_processing/spl_sqrt.c \
            sources/common_audio/signal_processing/spl_version.c \
            sources/common_audio/signal_processing/spl_sqrt_floor.c \
            sources/common_audio/signal_processing/splitting_filter.c \
            sources/common_audio/signal_processing/sqrt_of_one_minus_x_squared.c \
            sources/common_audio/signal_processing/vector_scaling_operations.c \
            sources/common_audio/signal_processing/complex_bit_reverse.c \
            sources/common_audio/signal_processing/filter_ar_fast_q12.c\
            sources/common_audio/signal_processing/filter_ar_fast_q12_armv7.s\
            sources/common_audio/signal_processing/complex_bit_reverse_arm.s \
            sources/common_audio/signal_processing/spl_sqrt_floor_arm.s\
            sources/common_audio/resampler/resampler.cc\
            sources/common_audio/vad/webrtc_vad.c\
            sources/common_audio/vad/vad_core.c \
            sources/common_audio/vad/vad_filterbank.c\
            sources/common_audio/vad/vad_gmm.c\
            sources/common_audio/vad/vad_sp.c\
            sources/modules/audio_processing/audio_buffer.cc\
            sources/modules/audio_processing/audio_processing_impl.cc\
            sources/modules/audio_processing/echo_cancellation_impl.cc\
            sources/modules/audio_processing/echo_control_mobile_impl.cc\
            sources/modules/audio_processing/gain_control_impl.cc\
            sources/modules/audio_processing/high_pass_filter_impl.cc\
            sources/modules/audio_processing/level_estimator_impl.cc\
            sources/modules/audio_processing/noise_suppression_impl.cc\
            sources/modules/audio_processing/splitting_filter_lo.cc\
            sources/modules/audio_processing/processing_component.cc\
            sources/modules/audio_processing/voice_detection_impl.cc\
            sources/modules/audio_device/linux/audio_device_alsa_linux.cc\
            sources/modules/audio_device/linux/audio_mixer_manager_alsa_linux.cc\
            sources/modules/audio_device/linux/alsasymboltable_linux.cc\
            sources/modules/audio_device/linux/latebindingsymboltable_linux.cc\
            sources/modules/audio_device/linux/audio_device_pulse_linux.cc\
            sources/modules/audio_device/linux/audio_mixer_manager_pulse_linux.cc\
            sources/modules/audio_device/linux/pulseaudiosymboltable_linux.cc\
            sources/voice_engine/channel.cc \
            sources/voice_engine/channel_manager.cc \
            sources/voice_engine/channel_manager_base.cc \
            sources/voice_engine/dtmf_inband.cc \
            sources/voice_engine/dtmf_inband_queue.cc \
            sources/voice_engine/level_indicator.cc \
            sources/voice_engine/monitor_module.cc \
            sources/voice_engine/output_mixer.cc \
            sources/voice_engine/shared_data.cc \
            sources/voice_engine/statistics.cc \
            sources/voice_engine/transmit_mixer.cc \
            sources/voice_engine/utility.cc \
            sources/voice_engine/voe_audio_processing_impl.cc \
            sources/voice_engine/voe_base_impl.cc \
            sources/voice_engine/voe_call_report_impl.cc \
            sources/voice_engine/voe_codec_impl.cc \
            sources/voice_engine/voe_dtmf_impl.cc \
            sources/voice_engine/voe_encryption_impl.cc \
            sources/voice_engine/voe_external_media_impl.cc \
            sources/voice_engine/voe_file_impl.cc \
            sources/voice_engine/voe_hardware_impl.cc \
            sources/voice_engine/voe_neteq_stats_impl.cc \
            sources/voice_engine/voe_network_impl.cc \
            sources/voice_engine/voe_rtp_rtcp_impl.cc \
            sources/voice_engine/voe_video_sync_impl.cc \
            sources/voice_engine/voe_volume_control_impl.cc \
            sources/voice_engine/voice_engine_impl.cc \
            sources/voice_engine/output_mixer_internal.cc\
            sources/modules/audio_coding/main/source/acm_cng.cc \
            sources/modules/audio_coding/main/source/acm_codec_database.cc \
            sources/modules/audio_coding/main/source/acm_dtmf_detection.cc \
            sources/modules/audio_coding/main/source/acm_dtmf_playout.cc \
            sources/modules/audio_coding/main/source/acm_generic_codec.cc \
            sources/modules/audio_coding/main/source/acm_neteq.cc \
            sources/modules/audio_coding/main/source/acm_pcma.cc \
            sources/modules/audio_coding/main/source/acm_pcmu.cc \
            sources/modules/audio_coding/main/source/acm_red.cc \
            sources/modules/audio_coding/main/source/acm_resampler.cc \
            sources/modules/audio_coding/main/source/audio_coding_module.cc \
            sources/modules/audio_coding/main/source/audio_coding_module_impl.cc \
            sources/modules/audio_coding/main/source/acm_g729.cc \
            sources/modules/audio_coding/main/source/acm_ilbc.cc\
            sources/modules/audio_processing/aecm/echo_control_mobile.c \
            sources/modules/audio_processing/aecm/aecm_core.c\
            sources/modules/audio_processing/aec/echo_cancellation.c \
            sources/modules/audio_processing/aec/aec_resampler.c \
            sources/modules/audio_processing/aec/aec_core.c \
            sources/modules/audio_processing/aec/aec_rdft.c \
#            sources/modules/audio_processing/aec/aec_core_sse2.c \
#            sources/modules/audio_processing/aec/aec_rdft_sse2.c\
            sources/modules/audio_device/audio_device_buffer.cc \
            sources/modules/audio_device/audio_device_generic.cc \
            sources/modules/audio_device/audio_device_utility.cc \
            sources/modules/audio_device/audio_device_impl.cc \
            sources/modules/audio_device/linux/audio_device_utility_linux.cc \
            sources/modules/audio_device/android/audio_device_utility_android.cc\
            sources/modules/media_file/source/avi_file.cc \
            sources/modules/media_file/source/media_file_impl.cc \
            sources/modules/media_file/source/media_file_utility.cc\
            sources/modules/udp_transport/source/udp_transport_impl.cc \
            sources/modules/udp_transport/source/udp_socket_wrapper.cc \
            sources/modules/udp_transport/source/udp_socket_manager_wrapper.cc \
            sources/modules/udp_transport/source/udp_socket_manager_posix.cc \
            sources/modules/udp_transport/source/udp_socket_posix.cc\
            sources/modules/utility/source/coder.cc \
            sources/modules/utility/source/file_player_impl.cc \
            sources/modules/utility/source/file_recorder_impl.cc \
            sources/modules/utility/source/process_thread_impl.cc \
            sources/modules/utility/source/rtp_dump_impl.cc \
            sources/modules/utility/source/frame_scaler.cc \
            sources/modules/utility/source/audio_frame_operations.cc\
            sources/system_wrappers/source/map.cc \
            sources/system_wrappers/source/rw_lock_generic.cc \
            sources/system_wrappers/source/sort.cc \
            sources/system_wrappers/source/aligned_malloc.cc \
            sources/system_wrappers/source/atomic32_posix.cc \
            sources/system_wrappers/source/condition_variable.cc \
            sources/system_wrappers/source/cpu_no_op.cc \
            sources/system_wrappers/source/cpu_features.cc \
            sources/system_wrappers/source/cpu_info.cc \
            sources/system_wrappers/source/critical_section.cc \
            sources/system_wrappers/source/event.cc \
            sources/system_wrappers/source/file_impl.cc \
            sources/system_wrappers/source/list_no_stl.cc \
            sources/system_wrappers/source/rw_lock.cc \
            sources/system_wrappers/source/thread.cc \
            sources/system_wrappers/source/trace_impl.cc \
            sources/system_wrappers/source/condition_variable_posix.cc \
            sources/system_wrappers/source/cpu_linux.cc \
            sources/system_wrappers/source/critical_section_posix.cc \
            sources/system_wrappers/source/event_posix.cc \
            sources/system_wrappers/source/sleep.cc \
            sources/system_wrappers/source/thread_posix.cc \
            sources/system_wrappers/source/trace_posix.cc \
            sources/system_wrappers/source/rw_lock_posix.cc\
            sources/modules/audio_coding/neteq/accelerate.c \
            sources/modules/audio_coding/neteq/automode.c \
            sources/modules/audio_coding/neteq/bgn_update.c \
            sources/modules/audio_coding/neteq/bufstats_decision.c \
            sources/modules/audio_coding/neteq/cng_internal.c \
            sources/modules/audio_coding/neteq/codec_db.c \
            sources/modules/audio_coding/neteq/correlator.c \
            sources/modules/audio_coding/neteq/dsp.c \
            sources/modules/audio_coding/neteq/dsp_helpfunctions.c \
            sources/modules/audio_coding/neteq/dtmf_buffer.c \
            sources/modules/audio_coding/neteq/dtmf_tonegen.c \
            sources/modules/audio_coding/neteq/expand.c \
            sources/modules/audio_coding/neteq/mcu_address_init.c \
            sources/modules/audio_coding/neteq/mcu_dsp_common.c \
            sources/modules/audio_coding/neteq/mcu_reset.c \
            sources/modules/audio_coding/neteq/merge.c \
            sources/modules/audio_coding/neteq/min_distortion.c \
            sources/modules/audio_coding/neteq/mix_voice_unvoice.c \
            sources/modules/audio_coding/neteq/mute_signal.c \
            sources/modules/audio_coding/neteq/normal.c \
            sources/modules/audio_coding/neteq/packet_buffer.c \
            sources/modules/audio_coding/neteq/peak_detection.c \
            sources/modules/audio_coding/neteq/preemptive_expand.c \
            sources/modules/audio_coding/neteq/random_vector.c \
            sources/modules/audio_coding/neteq/recin.c \
            sources/modules/audio_coding/neteq/recout.c \
            sources/modules/audio_coding/neteq/rtcp.c \
            sources/modules/audio_coding/neteq/rtp.c \
            sources/modules/audio_coding/neteq/set_fs.c \
            sources/modules/audio_coding/neteq/signal_mcu.c \
            sources/modules/audio_coding/neteq/split_and_insert.c \
            sources/modules/audio_coding/neteq/unmute_signal.c \
            sources/modules/audio_coding/neteq/webrtc_neteq.c\
            sources/modules/audio_processing/agc/analog_agc.c\
            sources/modules/audio_processing/agc/digital_agc.c\
            sources/modules/audio_conference_mixer/source/audio_frame_manipulator.cc \
            sources/modules/audio_conference_mixer/source/level_indicator_mix.cc \
            sources/modules/audio_conference_mixer/source/audio_conference_mixer_impl.cc \
            sources/modules/audio_conference_mixer/source/time_scheduler.cc\
            sources/modules/audio_coding/codecs/cng/webrtc_cng.c \
            sources/modules/audio_coding/codecs/cng/cng_helpfuns.c\
            sources/modules/audio_coding/codecs/g711/g711_interface.c \
            sources/modules/audio_coding/codecs/g711/g711.c \
            sources/modules/audio_coding/codecs/g729/g729.c\
            sources/modules/rtp_rtcp/source/bitrate.cc \
            sources/modules/rtp_rtcp/source/rtp_rtcp_impl.cc \
            sources/modules/rtp_rtcp/source/rtcp_receiver.cc \
            sources/modules/rtp_rtcp/source/rtcp_receiver_help.cc \
            sources/modules/rtp_rtcp/source/rtcp_sender.cc \
            sources/modules/rtp_rtcp/source/rtcp_utility.cc \
            sources/modules/rtp_rtcp/source/rtp_receiver.cc \
            sources/modules/rtp_rtcp/source/rtp_sender.cc \
            sources/modules/rtp_rtcp/source/rtp_utility.cc \
            sources/modules/rtp_rtcp/source/rtp_header_extension.cc \
            sources/modules/rtp_rtcp/source/ssrc_database.cc \
            sources/modules/rtp_rtcp/source/tmmbr_help.cc \
            sources/modules/rtp_rtcp/source/dtmf_queue.cc \
            sources/modules/rtp_rtcp/source/rtp_receiver_audio.cc \
            sources/modules/rtp_rtcp/source/rtp_sender_audio.cc \
            sources/modules/rtp_rtcp/source/forward_error_correction.cc \
            sources/modules/rtp_rtcp/source/forward_error_correction_internal.cc \
            sources/modules/rtp_rtcp/source/rtp_packet_history.cc \
            sources/modules/rtp_rtcp/source/receiver_fec.cc \
            sources/modules/rtp_rtcp/source/rtp_receiver_video.cc \
            sources/modules/rtp_rtcp/source/rtp_format_vp8.cc \
            sources/modules/rtp_rtcp/source/transmission_bucket.cc \
            sources/modules/rtp_rtcp/source/vp8_partition_aggregator.cc \
            sources/modules/rtp_rtcp/source/rtp_sender_video.cc \
            sources/modules/rtp_rtcp/source/producer_fec.cc \
            sources/modules/rtp_rtcp/source/rtp_reliable_helper_impl.cc\
            sources/modules/bitrate_controller/bitrate_controller_impl.cc \
            sources/modules/bitrate_controller/send_side_bandwidth_estimation.cc \
            sources/modules/remote_bitrate_estimator/bitrate_estimator.cc \
            sources/modules/remote_bitrate_estimator/overuse_detector.cc \
            sources/modules/remote_bitrate_estimator/remote_bitrate_estimator_multi_stream.cc \
            sources/modules/remote_bitrate_estimator/remote_bitrate_estimator_single_stream.cc \
            sources/modules/remote_bitrate_estimator/remote_rate_control.cc \
            sources/modules/remote_bitrate_estimator/rtp_to_ntp.cc\
            sources/modules/audio_coding/codecs/ilbc/abs_quant.c \
            sources/modules/audio_coding/codecs/ilbc/abs_quant_loop.c \
            sources/modules/audio_coding/codecs/ilbc/augmented_cb_corr.c \
            sources/modules/audio_coding/codecs/ilbc/bw_expand.c \
            sources/modules/audio_coding/codecs/ilbc/cb_construct.c \
            sources/modules/audio_coding/codecs/ilbc/cb_mem_energy.c \
            sources/modules/audio_coding/codecs/ilbc/cb_mem_energy_augmentation.c \
            sources/modules/audio_coding/codecs/ilbc/cb_mem_energy_calc.c \
            sources/modules/audio_coding/codecs/ilbc/cb_search.c \
            sources/modules/audio_coding/codecs/ilbc/cb_search_core.c \
            sources/modules/audio_coding/codecs/ilbc/cb_update_best_index.c \
            sources/modules/audio_coding/codecs/ilbc/chebyshev.c \
            sources/modules/audio_coding/codecs/ilbc/comp_corr.c \
            sources/modules/audio_coding/codecs/ilbc/constants.c \
            sources/modules/audio_coding/codecs/ilbc/create_augmented_vec.c \
            sources/modules/audio_coding/codecs/ilbc/decode.c \
            sources/modules/audio_coding/codecs/ilbc/decode_residual.c \
            sources/modules/audio_coding/codecs/ilbc/decoder_interpolate_lsf.c \
            sources/modules/audio_coding/codecs/ilbc/do_plc.c \
            sources/modules/audio_coding/codecs/ilbc/encode.c \
            sources/modules/audio_coding/codecs/ilbc/energy_inverse.c \
            sources/modules/audio_coding/codecs/ilbc/enh_upsample.c \
            sources/modules/audio_coding/codecs/ilbc/enhancer.c \
            sources/modules/audio_coding/codecs/ilbc/enhancer_interface.c \
            sources/modules/audio_coding/codecs/ilbc/filtered_cb_vecs.c \
            sources/modules/audio_coding/codecs/ilbc/frame_classify.c \
            sources/modules/audio_coding/codecs/ilbc/gain_dequant.c \
            sources/modules/audio_coding/codecs/ilbc/gain_quant.c \
            sources/modules/audio_coding/codecs/ilbc/get_cd_vec.c \
            sources/modules/audio_coding/codecs/ilbc/get_lsp_poly.c \
            sources/modules/audio_coding/codecs/ilbc/get_sync_seq.c \
            sources/modules/audio_coding/codecs/ilbc/hp_input.c \
            sources/modules/audio_coding/codecs/ilbc/hp_output.c \
            sources/modules/audio_coding/codecs/ilbc/ilbc.c \
            sources/modules/audio_coding/codecs/ilbc/index_conv_dec.c \
            sources/modules/audio_coding/codecs/ilbc/index_conv_enc.c \
            sources/modules/audio_coding/codecs/ilbc/init_decode.c \
            sources/modules/audio_coding/codecs/ilbc/init_encode.c \
            sources/modules/audio_coding/codecs/ilbc/interpolate.c \
            sources/modules/audio_coding/codecs/ilbc/interpolate_samples.c \
            sources/modules/audio_coding/codecs/ilbc/lpc_encode.c \
            sources/modules/audio_coding/codecs/ilbc/lsf_check.c \
            sources/modules/audio_coding/codecs/ilbc/lsf_interpolate_to_poly_dec.c \
            sources/modules/audio_coding/codecs/ilbc/lsf_interpolate_to_poly_enc.c \
            sources/modules/audio_coding/codecs/ilbc/lsf_to_lsp.c \
            sources/modules/audio_coding/codecs/ilbc/lsf_to_poly.c \
            sources/modules/audio_coding/codecs/ilbc/lsp_to_lsf.c \
            sources/modules/audio_coding/codecs/ilbc/my_corr.c \
            sources/modules/audio_coding/codecs/ilbc/nearest_neighbor.c \
            sources/modules/audio_coding/codecs/ilbc/pack_bits.c \
            sources/modules/audio_coding/codecs/ilbc/poly_to_lsf.c \
            sources/modules/audio_coding/codecs/ilbc/poly_to_lsp.c \
            sources/modules/audio_coding/codecs/ilbc/refiner.c \
            sources/modules/audio_coding/codecs/ilbc/simple_interpolate_lsf.c \
            sources/modules/audio_coding/codecs/ilbc/simple_lpc_analysis.c \
            sources/modules/audio_coding/codecs/ilbc/simple_lsf_dequant.c \
            sources/modules/audio_coding/codecs/ilbc/simple_lsf_quant.c \
            sources/modules/audio_coding/codecs/ilbc/smooth.c \
            sources/modules/audio_coding/codecs/ilbc/smooth_out_data.c \
            sources/modules/audio_coding/codecs/ilbc/sort_sq.c \
            sources/modules/audio_coding/codecs/ilbc/split_vq.c \
            sources/modules/audio_coding/codecs/ilbc/state_construct.c \
            sources/modules/audio_coding/codecs/ilbc/state_search.c \
            sources/modules/audio_coding/codecs/ilbc/swap_bytes.c \
            sources/modules/audio_coding/codecs/ilbc/unpack_bits.c \
            sources/modules/audio_coding/codecs/ilbc/vq3.c \
            sources/modules/audio_coding/codecs/ilbc/vq4.c \
            sources/modules/audio_coding/codecs/ilbc/window32_w32.c \
            sources/modules/audio_coding/codecs/ilbc/xcorr_coef.c \
            interface/media_stream.cpp \
#            sources/modules/audio_processing/aec/echo_cancellation.c \
#            sources/modules/audio_processing/aec/aec_core.c \
#            sources/modules/audio_processing/aec/aec_resampler.c \
#            sources/modules/audio_processing/aec/aec_rdft.c

headers.files += interface/media_stream.h

target.path = $$INSTALL_DIR/libs/
headers.path = /usr/include/eim/eimwebrtc/

#QMAKE_PKGCONFIG_NAME = eimwebrtc
#QMAKE_PKGCONFIG_DESCRIPTION = eimwebrtc library
#QMAKE_PKGCONFIG_PREFIX = $$headers.path
#QMAKE_PKGCONFIG_LIBDIR = $$target.path
#QMAKE_PKGCONFIG_INCDIR = $$headers.path
#QMAKE_PKGCONFIG_DESTDIR = pkgconfig

#pkgconfig.files = $$TARGET.pc
#pkgconfig.path = $$target.path/pkgconfig

#INSTALLS += target headers pkgconfig
INSTALLS += target headers

