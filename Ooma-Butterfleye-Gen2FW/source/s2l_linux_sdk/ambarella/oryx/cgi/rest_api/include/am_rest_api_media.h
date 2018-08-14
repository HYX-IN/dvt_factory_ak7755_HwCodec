/**
 * am_rest_api_media.h
 *
 *  History:
 *		2015/08/19 - [Huaiqing Wang] created file
 *
 * Copyright (c) 2016 Ambarella, Inc.
 *
 * This file and its contents ("Software") are protected by intellectual
 * property rights including, without limitation, U.S. and/or foreign
 * copyrights. This Software is also the confidential and proprietary
 * information of Ambarella, Inc. and its licensors. You may not use, reproduce,
 * disclose, distribute, modify, or otherwise prepare derivative works of this
 * Software or any portion thereof except pursuant to a signed license agreement
 * or nondisclosure agreement with Ambarella, Inc. or its authorized affiliates.
 * In the absence of such an agreement, you agree to promptly notify and return
 * this Software to Ambarella, Inc.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF NON-INFRINGEMENT,
 * MERCHANTABILITY, AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL AMBARELLA, INC. OR ITS AFFILIATES BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; COMPUTER FAILURE OR MALFUNCTION; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef ORYX_CGI_INCLUDE_AM_REST_API_MEDIA_H_
#define ORYX_CGI_INCLUDE_AM_REST_API_MEDIA_H_

#include <string.h>
#include "am_rest_api_handle.h"
#include "am_api_media.h"

class AMRestAPIMedia: public AMRestAPIHandle
{
  public:
    virtual AM_REST_RESULT  rest_api_handle();

  private:
    AM_REST_RESULT  media_recording_handle();
    AM_REST_RESULT  media_file_recording_handle();
    AM_REST_RESULT  media_event_recording_handle();

  private:
    AM_REST_RESULT  media_audio_playback_handle();
    AM_REST_RESULT  parse_event_arg(AMIApiMediaEvent& event);
    //set audio file to system, it always must be run
    //before start action, except preview action is pause
    AM_REST_RESULT  audio_playback_set_handle(int32_t play_id);
    AM_REST_RESULT  audio_playback_start_handle(int32_t play_id);
    AM_REST_RESULT  audio_playback_stop_handle(int32_t play_id);
    AM_REST_RESULT  audio_playback_pause_handle(int32_t play_id);
    AM_REST_RESULT  media_video_playback_handle();
    AM_REST_RESULT  media_get_all_mp4_file(std::string &path);
};

#endif /* ORYX_CGI_INCLUDE_AM_REST_API_MEDIA_H_ */
