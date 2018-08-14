/*******************************************************************************
 * am_player_config.h
 *
 * History:
 *   2014-9-11 - [ypchang] created file
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
 *
 ******************************************************************************/
#ifndef AM_PLAYER_CONFIG_H_
#define AM_PLAYER_CONFIG_H_

#include "am_filter_config.h"
#include "am_audio_utility.h"

struct AudioPlayerConfig
{
  uint32_t               buffer_delay_ms = 100;
  int32_t                initial_volume  = 75;
  AM_AUDIO_SAMPLE_FORMAT default_format  = AM_SAMPLE_INVALID;
  bool                   enable_aec      = true;
  std::string            interface       = "pulse";
  AudioPlayerConfig() {}
  AudioPlayerConfig(const AudioPlayerConfig &config) :
    buffer_delay_ms(config.buffer_delay_ms),
    initial_volume(config.initial_volume),
    default_format(config.default_format),
    enable_aec(config.enable_aec),
    interface(config.interface)
  {}
  ~AudioPlayerConfig(){}
};

struct PlayerConfig: public AMFilterConfig
{
    uint32_t pause_check_interval;
    AudioPlayerConfig audio;
    PlayerConfig() :
     pause_check_interval(0)
    {}
    ~PlayerConfig(){}
};

class AMConfig;
class AMPlayer;
class AMPlayerConfig
{
    friend class AMPlayer;
  public:
    AMPlayerConfig();
    virtual ~AMPlayerConfig();
    PlayerConfig* get_config(const std::string& config);

  private:
    AMConfig     *m_config;
    PlayerConfig *m_player_config;
};

#endif /* AM_PLAYER_CONFIG_H_ */
