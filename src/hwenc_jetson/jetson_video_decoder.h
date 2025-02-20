/*
 *  Copyright (c) 2015 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 *
 */

#ifndef JETSON_VIDEO_DECODER_H_
#define JETSON_VIDEO_DECODER_H_

// WebRTC
#include <api/video_codecs/video_decoder.h>
#include <common_video/include/video_frame_buffer_pool.h>
#include <rtc_base/platform_thread.h>

// Jetson Linux Multimedia API
#include <NvVideoDecoder.h>

class JetsonVideoDecoder : public webrtc::VideoDecoder {
 public:
  JetsonVideoDecoder(uint32_t input_format);
  ~JetsonVideoDecoder() override;

  int32_t InitDecode(const webrtc::VideoCodec* codec_settings,
                     int32_t number_of_cores) override;

  int32_t Decode(const webrtc::EncodedImage& input_image,
                 bool missing_frames,
                 int64_t render_time_ms) override;

  int32_t RegisterDecodeCompleteCallback(
      webrtc::DecodedImageCallback* callback) override;

  int32_t Release() override;

  const char* ImplementationName() const override;

 private:
  int32_t JetsonConfigure();
  bool JetsonRelease();
  void SendEOS(NvV4l2Element* element);
  static void CaptureLoopFunction(void* obj);
  void CaptureLoop();
  int32_t SetCapture();

  uint32_t input_format_;
  NvVideoDecoder* decoder_;
  webrtc::DecodedImageCallback* decode_complete_callback_;
  webrtc::VideoFrameBufferPool buffer_pool_;
  rtc::PlatformThread capture_loop_;
  std::atomic<bool> eos_;
  std::atomic<bool> got_error_;
  int dst_dma_fd_;
};

#endif  // JETSON_VIDEO_DECODER_H_
