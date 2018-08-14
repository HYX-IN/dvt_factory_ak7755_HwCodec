/*******************************************************************************
 * am_audio_player.cpp
 *
 * History:
 *   Jul 8, 2016 - [ypchang] created file
 *
 * Copyright (c) 2015 Ambarella, Inc.
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
 *
 ******************************************************************************/

#include "am_base_include.h"
#include "am_define.h"
#include "am_log.h"

#include "am_audio_player_if.h"

#ifdef BUILD_AMBARELLA_ORYX_AUDIO_PLAYER_PULSE
#include "am_audio_player_pulse.h"
#endif

#ifdef BUILD_AMBARELLA_ORYX_AUDIO_PLAYER_ALSA
#include "am_audio_player_alsa.h"
#endif

AMIAudioPlayer* create_audio_player(const std::string& interface,
                                    const std::string& name,
                                    void *owner,
                                    AudioPlayerCallback callback)
{
  AMIAudioPlayer *player = nullptr;
#ifdef BUILD_AMBARELLA_ORYX_AUDIO_PLAYER_PULSE
  if (is_str_equal(interface.c_str(), "pulse")) {
    player = AMAudioPlayerPulse::create(owner, name, callback);
  } else
#endif
#ifdef BUILD_AMBARELLA_ORYX_AUDIO_PLAYER_ALSA
  if (is_str_equal(interface.c_str(), "alsa")) {
    player = AMAudioPlayerAlsa::create(owner, name, callback);
  } else
#endif
  {
    ERROR("Unknown audio interface type: %S", interface.c_str());
  }

  return player;
}
