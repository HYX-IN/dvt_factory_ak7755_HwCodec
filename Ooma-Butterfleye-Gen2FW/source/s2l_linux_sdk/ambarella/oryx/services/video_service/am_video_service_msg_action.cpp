/*******************************************************************************
 * am_video_service_msg_action.cpp
 *
 * History:
 *   Sep 18, 2015 - [ypchang] created file
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

#include "am_base_include.h"
#include "am_define.h"
#include "am_log.h"
#include <uuid/uuid.h>

#include "am_api_event.h"
#include "am_api_video.h"
#include "am_video_types.h"
#include "am_video_utility.h"
#include "am_service_frame_if.h"
#include "am_video_camera_if.h"
/* Video Plugin Interface */
#include "am_encode_warp_if.h"
#include "am_low_bitrate_control_if.h"
#include "am_smartrc_if.h"
#include "am_encode_eis_if.h"
#include "am_encode_overlay_if.h"
#include "am_motion_detect_if.h"
#include "am_face_detect_if.h"
#include "am_dptz_if.h"
#include "am_vca_framework_if.h"
#include <signal.h>
#include "commands/am_api_cmd_common.h"

extern AMIVideoCameraPtr  g_video_camera;
extern AMIServiceFrame   *g_service_frame;
extern AM_SERVICE_STATE   g_service_state;
extern AMIPCSyncCmdServer g_ipc;

void ON_SERVICE_INIT(void *msg_data,
                     int msg_data_size,
                     void *result_addr,
                     int result_max_size)
{
  am_service_result_t *svc_ret = (am_service_result_t*)result_addr;
  svc_ret->ret = 0;
  svc_ret->state = g_service_state;
  if (AM_LIKELY(svc_ret->state == AM_SERVICE_STATE_INIT_DONE)) {
    INFO("Video Service init done!");
  }
}

void ON_SERVICE_DESTROY(void *msg_data,
                        int msg_data_size,
                        void *result_addr,
                        int result_max_size)
{
  PRINTF("ON VIDEO SERVICE DESTROY.");
  g_video_camera->stop();
  g_service_frame->quit();
}

void ON_SERVICE_START(void *msg_data,
                      int msg_data_size,
                      void *result_addr,
                      int result_max_size)
{
  int32_t ret = 0;
  PRINTF("ON VIDEO SERVICE START.");
  if (AM_LIKELY(g_video_camera)) {
    if (AM_UNLIKELY(g_service_state != AM_SERVICE_STATE_STARTED)) {
      if (AM_UNLIKELY(AM_RESULT_OK != g_video_camera->start())) {
        ERROR("Video Service: Failed to start!");
        g_service_state = AM_SERVICE_STATE_ERROR;
        ret = -2;
      } else {
        g_service_state = AM_SERVICE_STATE_STARTED;
      }
    }
  } else {
    ERROR("Video Service: Failed to get AMVideoCamera instance!");
    ret = -1;
    g_service_state = AM_SERVICE_STATE_NOT_INIT;
  }
  ((am_service_result_t*)result_addr)->ret = ret;
  ((am_service_result_t*)result_addr)->state = g_service_state;
}

void ON_SERVICE_STOP(void *msg_data,
                     int msg_data_size,
                     void *result_addr,
                     int result_max_size)
{
  PRINTF("ON VIDEO SERVICE STOP.");
  int32_t ret = 0;
  if (AM_LIKELY(g_video_camera)) {
    if (AM_UNLIKELY(AM_RESULT_OK != g_video_camera->stop())) {
      ERROR("Video Service: Failed to stop!");
      ret = -2;
      g_service_state = AM_SERVICE_STATE_ERROR;
    } else {
      g_service_state = AM_SERVICE_STATE_STOPPED;
    }
  } else {
    ERROR("Video Service: Failed to get AMVideoCamera instance!");
    ret = -1;
    g_service_state = AM_SERVICE_STATE_NOT_INIT;
  }
  ((am_service_result_t*)result_addr)->ret = ret;
  ((am_service_result_t*)result_addr)->state = g_service_state;
}

void ON_SERVICE_RESTART(void *msg_data,
                        int msg_data_size,
                        void *result_addr,
                        int result_max_size)
{
  NOTICE("Not implemented!");
}

void ON_SERVICE_STATUS(void *msg_data,
                       int msg_data_size,
                       void *result_addr,
                       int result_max_size)
{
  ((am_service_result_t*)result_addr)->ret = 0;
  ((am_service_result_t*)result_addr)->state = g_service_state;
  INFO("Video Service: Get status!");
}

void ON_CFG_ALL_LOAD(void *msg_data,
                     int msg_data_size,
                     void *result_addr,
                     int result_max_size)
{
  am_service_result_t *svc_ret = (am_service_result_t*)result_addr;
  memset(svc_ret, 0, sizeof(am_service_result_t));
  INFO("Video Service: ON_CFG_ALL_LOAD");

  if (AM_UNLIKELY((svc_ret->ret = g_video_camera->load_config_all())
                  != AM_RESULT_OK)) {
    ERROR("Failed to load all configuration!");
  }
}

void ON_CFG_FEATURE_GET(void *msg_data,
                        int msg_data_size,
                        void *result_addr,
                        int result_max_size)
{
  am_service_result_t *svc_ret = (am_service_result_t*)result_addr;
  memset(svc_ret, 0, sizeof(am_service_result_t));
  INFO("Video Service: ON_CFG_FEATURE_GET");
  do {
    am_feature_config_t *feature = (am_feature_config_t *)svc_ret->data;
    AMFeatureParam param;
    if (AM_UNLIKELY((svc_ret->ret = g_video_camera->get_feature_config(param))
        != AM_RESULT_OK)) {
      ERROR("Failed to get feature config!");
      break;
    }
    feature->version = param.version.second;
    feature->mode = param.mode.second;
    feature->hdr = param.hdr_types.second.at(AM_VIN_0);
    feature->iso = param.iso.second;
    feature->dewarp_func = param.dewarp_func.second;
    feature->dptz = param.dptz.second;
    feature->bitrate_ctrl = param.bitrate_ctrl.second;
    feature->overlay = param.overlay.second;
    feature->hevc = param.hevc.second;
  } while (0);
}

void ON_CFG_FEATURE_SET(void *msg_data,
                        int msg_data_size,
                        void *result_addr,
                        int result_max_size)
{
  am_service_result_t *svc_ret = (am_service_result_t*)result_addr;
  memset(svc_ret, 0, sizeof(am_service_result_t));
  INFO("Video Service: ON_CFG_FEATURE_SET");

  do {
    am_feature_config_t *feature = (am_feature_config_t *)msg_data;
    AMFeatureParam param;
    if (TEST_BIT(feature->enable_bits, AM_FEATURE_CONFIG_MODE_EN_BIT)) {
      param.mode.first = true;
      param.mode.second = AM_ENCODE_MODE(feature->mode);
    }
    if (TEST_BIT(feature->enable_bits, AM_FEATURE_CONFIG_HDR_EN_BIT)) {
      param.hdr_types.first = true;
      param.hdr_types.second[AM_VIN_0] = AM_HDR_TYPE(feature->hdr);
      /* For vsrc1 hdr */
      param.hdr_types.second[AM_VIN_1] = AM_HDR_TYPE(feature->hdr);
    }
    if (TEST_BIT(feature->enable_bits, AM_FEATURE_CONFIG_ISO_EN_BIT)) {
      param.iso.first = true;
      param.iso.second = AM_IMAGE_ISO_TYPE(feature->iso);
    }
    if (TEST_BIT(feature->enable_bits, AM_FEATURE_CONFIG_DEWARP_EN_BIT)) {
      param.dewarp_func.first = true;
      param.dewarp_func.second = AM_DEWARP_FUNC_TYPE(feature->dewarp_func);
    }
    if (TEST_BIT(feature->enable_bits, AM_FEATURE_CONFIG_DPTZ_EN_BIT)) {
      param.dptz.first = true;
      param.dptz.second = AM_DPTZ_TYPE(feature->dptz);
    }
    if (TEST_BIT(feature->enable_bits, AM_FEATURE_CONFIG_BITRATECTRL_EN_BIT)) {
      param.bitrate_ctrl.first = true;
      param.bitrate_ctrl.second = AM_BITRATE_CTRL_METHOD(feature->bitrate_ctrl);
    }
    if (TEST_BIT(feature->enable_bits, AM_FEATURE_CONFIG_OVERLAY_EN_BIT)) {
      param.overlay.first = true;
      param.overlay.second = AM_OVERLAY_TYPE(feature->overlay);
    }
    if (TEST_BIT(feature->enable_bits, AM_FEATURE_CONFIG_HEVC_EN_BIT)) {
      param.hevc.first = true;
      param.hevc.second = AM_HEVC_CLOCK_TYPE(feature->hevc);
    }
    if (TEST_BIT(feature->enable_bits, AM_FEATURE_CONFIG_IAV_VERSION_EN_BIT)) {
      param.version.first = true;
      param.version.second = feature->version;
    }
    if (AM_UNLIKELY((svc_ret->ret = g_video_camera->set_feature_config(param))
        != AM_RESULT_OK)) {
      ERROR("Failed to get feature config!");
      break;
    }

  } while (0);
}


void ON_CFG_VIN_GET(void *msg_data,
                    int msg_data_size,
                    void *result_addr,
                    int result_max_size)
{
  am_service_result_t *svc_ret = (am_service_result_t*)result_addr;
  memset(svc_ret, 0, sizeof(am_service_result_t));
  INFO("Video Service: ON_CFG_VIN_GET");

  if (!msg_data) {
    svc_ret->ret = AM_RESULT_ERR_DATA_POINTER;
    ERROR("msg_data is NULL");
  } else {
    do {
      am_vin_config_t *air_config = (am_vin_config_t*)svc_ret->data;
      uint32_t vin_id = air_config->vin_id;
      AMVinParamMap vin_param_map;
      if (AM_UNLIKELY((svc_ret->ret = g_video_camera->
          get_vin_config(vin_param_map)) != AM_RESULT_OK)) {
        ERROR("Failed to get vin configuration!");
        break;
      }
      AMVinParam vin_param = vin_param_map[AM_VIN_ID(vin_id)];
      air_config->width =
          AMVinTrans::mode_to_resolution(vin_param.mode.second).width;
      air_config->height =
          AMVinTrans::mode_to_resolution(vin_param.mode.second).height;
      switch (vin_param.flip.second) {
        case AM_VIDEO_FLIP_VERTICAL:
          air_config->flip = 1;
          break;
        case AM_VIDEO_FLIP_HORIZONTAL:
          air_config->flip = 2;
          break;
        case AM_VIDEO_FLIP_VH_BOTH:
          air_config->flip = 3;
          break;
        case AM_VIDEO_FLIP_AUTO:
          air_config->flip = 255;
          break;
        case AM_VIDEO_FLIP_NONE:
        default:
          air_config->flip = 0;
          break;
      }
      air_config->fps = vin_param.flip.second;
      switch (vin_param.bayer_pattern.second) {
        case AM_VIN_BAYER_PATTERN_RG:
          air_config->bayer_pattern = 1;
          break;
        case AM_VIN_BAYER_PATTERN_BG:
          air_config->bayer_pattern = 2;
          break;
        case AM_VIN_BAYER_PATTERN_GR:
          air_config->bayer_pattern = 3;
          break;
        case AM_VIN_BAYER_PATTERN_GB:
          air_config->bayer_pattern = 4;
          break;
        case AM_VIN_BAYER_PATTERN_AUTO:
        default:
          air_config->bayer_pattern = 0;
          break;
      }
    } while (0);
  }
}

void ON_CFG_VIN_SET(void *msg_data,
                    int msg_data_size,
                    void *result_addr,
                    int result_max_size)
{
  am_service_result_t *svc_ret = (am_service_result_t*)result_addr;

  memset(svc_ret, 0, sizeof(*svc_ret));
  INFO("Video Service: VIN set");

  if (AM_UNLIKELY(!msg_data)) {
    svc_ret->ret = AM_RESULT_ERR_DATA_POINTER;
    ERROR("Invalid parameter! msg_data is NULL!");
  } else {
    //only normal mode can do set manipulation
    AM_WORKING_MODE mode;
    if (AM_UNLIKELY((svc_ret->ret = g_video_camera->get_working_mode(mode))
                    != AM_RESULT_OK)) {
      return;
    }
    if (mode != AM_WORKING_MODE_NORAML) {
      return;
    }

    am_vin_config_t *vin_conf = (am_vin_config_t*)msg_data;
    AMVinParamMap    vin_param_map;
    if (AM_UNLIKELY((svc_ret->ret = g_video_camera->
        get_vin_config(vin_param_map)) != AM_RESULT_OK)) {
      ERROR("Failed to get VIN configuration!");
    } else {
      AMVinParam &vin_param = vin_param_map[AM_VIN_ID(vin_conf->vin_id)];
      if (TEST_BIT(vin_conf->enable_bits, AM_VIN_CONFIG_WIDTH_HEIGHT_EN_BIT)) {

      }
      if (TEST_BIT(vin_conf->enable_bits, AM_VIN_CONFIG_FLIP_EN_BIT)) {
        vin_param.flip.first = true;
        vin_param.flip.second = AM_VIDEO_FLIP(vin_conf->flip);
      }
      if (TEST_BIT(vin_conf->enable_bits, AM_VIN_CONFIG_FPS_EN_BIT)) {
        vin_param.fps.first = true;
        vin_param.fps.second = AM_VIDEO_FPS(vin_conf->fps);
      }
      if (TEST_BIT(vin_conf->enable_bits, AM_VIN_CONFIG_BAYER_PATTERN_EN_BIT)) {
        vin_param.bayer_pattern.first = true;
        vin_param.bayer_pattern.second =
            AM_VIN_BAYER_PATTERN(vin_conf->bayer_pattern);
      }
      svc_ret->ret = g_video_camera->set_vin_config(vin_param_map);
    }
  }
}

void ON_CFG_VOUT_GET(void *msg_data,
                    int msg_data_size,
                    void *result_addr,
                    int result_max_size)
{
  am_service_result_t *svc_ret = (am_service_result_t*)result_addr;
  memset(svc_ret, 0, sizeof(am_service_result_t));
  INFO("Video Service: ON_CFG_VOUT_GET");

  if (!msg_data) {
    svc_ret->ret = AM_RESULT_ERR_DATA_POINTER;
    ERROR("msg_data is NULL");
  } else {
    do {
      am_vout_config_t *config = (am_vout_config_t*)svc_ret->data;
      AMVoutParamMap vout_param_map;
      if (AM_UNLIKELY((svc_ret->ret = g_video_camera->
          get_vout_config(vout_param_map)) != AM_RESULT_OK)) {
        ERROR("Failed to get vout configuration!");
        break;
      }

      AM_VOUT_ID *id = (AM_VOUT_ID *)msg_data;
      AMVoutParamMap::iterator it = vout_param_map.find(*id);
      if (it == vout_param_map.end()) {
        svc_ret->ret = AM_RESULT_ERR_INVALID;
        ERROR("vout%d config is null", *id);
        break;
      }
      AMVoutParam param = vout_param_map[*id];

      config->type = uint32_t(param.type.second);
      config->video_type = uint32_t(param.video_type.second);
      switch (param.flip.second) {
        case AM_VIDEO_FLIP_VERTICAL:
          config->flip = 1;
          break;
        case AM_VIDEO_FLIP_HORIZONTAL:
          config->flip = 2;
          break;
        case AM_VIDEO_FLIP_VH_BOTH:
          config->flip = 3;
          break;
        case AM_VIDEO_FLIP_AUTO:
          config->flip = 0;
          break;
        case AM_VIDEO_FLIP_NONE:
        default:
          config->flip = 0;
          break;
      }
      config->rotate = uint32_t(param.rotate.second);
      std::string mode = AMVoutTrans::mode_enum_to_str(param.mode.second);
      strncpy(config->mode, mode.c_str(), mode.size());
    } while (0);
  }
}

void ON_CFG_VOUT_SET(void *msg_data,
                     int msg_data_size,
                     void *result_addr,
                     int result_max_size)
{
  am_service_result_t *svc_ret = (am_service_result_t*)result_addr;

  memset(svc_ret, 0, sizeof(*svc_ret));
  INFO("Video Service: VOUT set");

  if (AM_UNLIKELY(!msg_data)) {
    svc_ret->ret = AM_RESULT_ERR_DATA_POINTER;
    ERROR("Invalid parameter! msg_data is NULL!");
  } else {
    //only normal mode can do set manipulation
    AM_WORKING_MODE mode;
    if (AM_UNLIKELY((svc_ret->ret = g_video_camera->get_working_mode(mode))
                    != AM_RESULT_OK)) {
      return;
    }
    if (mode != AM_WORKING_MODE_NORAML) {
      return;
    }

    am_vout_config_t *config = (am_vout_config_t*)msg_data;
    AMVoutParamMap    param_map;
    AMVoutParam &param = param_map[AM_VOUT_ID(config->vout_id)];
    if (TEST_BIT(config->enable_bits, AM_VOUT_CONFIG_TYPE_EN_BIT)) {
      param.type.first = true;
      param.type.second = AM_VOUT_TYPE(config->type);
    }
    if (TEST_BIT(config->enable_bits, AM_VOUT_CONFIG_VIDEO_TYPE_EN_BIT)) {
      param.video_type.first = true;
      param.video_type.second = AM_VOUT_VIDEO_TYPE(config->video_type);
    }
    if (TEST_BIT(config->enable_bits, AM_VOUT_CONFIG_MODE_EN_BIT)) {
      param.mode.first = true;
      param.mode.second = AMVoutTrans::mode_str_to_enum(config->mode);
    }
    if (TEST_BIT(config->enable_bits, AM_VOUT_CONFIG_FLIP_EN_BIT)) {
      param.flip.first = true;
      param.flip.second = AM_VIDEO_FLIP(config->flip);
    }
    if (TEST_BIT(config->enable_bits, AM_VOUT_CONFIG_ROTATE_EN_BIT)) {
      param.rotate.first = true;
      param.rotate.second = AM_VIDEO_ROTATE(config->rotate);
    }
    svc_ret->ret = g_video_camera->set_vout_config(param_map);
  }
}

void ON_CFG_BUFFER_GET(void *msg_data,
                       int msg_data_size,
                       void *result_addr,
                       int result_max_size)
{
  am_service_result_t *svc_ret = (am_service_result_t*)result_addr;

  memset(svc_ret, 0, sizeof(*svc_ret));
  INFO("Video Service: Buffer Format Get");

  if (AM_UNLIKELY(!msg_data)) {
    svc_ret->ret = AM_RESULT_ERR_DATA_POINTER;
    ERROR("Invalid parameter! msg_data is NULL!");
  } else {
    AMBufferParamMap buffer_param_map;
    am_buffer_fmt_t *buffer_fmt = (am_buffer_fmt_t*)svc_ret->data;
    AM_SOURCE_BUFFER_ID id = AM_SOURCE_BUFFER_ID((*(uint32_t*)msg_data));

    do {
      if (AM_UNLIKELY((svc_ret->ret = g_video_camera->
          get_buffer_config(buffer_param_map)) != AM_RESULT_OK)) {
        ERROR("Failed to get buffer config!");
        break;
      }
      AMBufferConfigParam &buf_conf = buffer_param_map[id];
      buffer_fmt->buffer_id      = id;
      buffer_fmt->type           = buf_conf.type.second;
      buffer_fmt->input_width    = buf_conf.input.second.size.width;
      buffer_fmt->input_height   = buf_conf.input.second.size.height;
      buffer_fmt->input_offset_x = buf_conf.input.second.offset.x;
      buffer_fmt->input_offset_y = buf_conf.input.second.offset.y;
#if !defined(CONFIG_AMBARELLA_ORYX_VIDEO_IAV4)
      buffer_fmt->input_crop = buf_conf.platform_config.input_crop.second ? 1 : 0;
      buffer_fmt->width      = buf_conf.platform_config.size.second.width;
      buffer_fmt->height     = buf_conf.platform_config.size.second.height;
#endif
#if defined(CONFIG_AMBARELLA_ORYX_VIDEO_IAV1)
      buffer_fmt->prewarp        = buf_conf.platform_config.prewarp.second;
#elif defined(CONFIG_AMBARELLA_ORYX_VIDEO_IAV3)
      buffer_fmt->cap_skip_itvl  = buf_conf.platform_config.cap_skip_itvl.second;
      buffer_fmt->auto_stop      = buf_conf.platform_config.auto_stop.second;
#endif
    } while(0);
  }
}

void ON_CFG_BUFFER_SET(void *msg_data,
                       int msg_data_size,
                       void *result_addr,
                       int result_max_size)
{
  am_service_result_t *svc_ret = (am_service_result_t*)result_addr;

  memset(svc_ret, 0, sizeof(*svc_ret));
  INFO("Video Service: Buffer Format Set");

  if (AM_UNLIKELY(!msg_data)) {
    svc_ret->ret = AM_RESULT_ERR_DATA_POINTER;
    ERROR("Invalid parameter! msg_data is NULL!");
  } else {
    //only normal mode can do set manipulation
    AM_WORKING_MODE mode;
    if (AM_UNLIKELY((svc_ret->ret = g_video_camera->get_working_mode(mode))
                    != AM_RESULT_OK)) {
      return;
    }
    if (mode != AM_WORKING_MODE_NORAML) {
      return;
    }

    am_buffer_fmt_t *buffer_fmt = ((am_buffer_fmt_t*)msg_data);
    AM_SOURCE_BUFFER_ID id = AM_SOURCE_BUFFER_ID(buffer_fmt->buffer_id);
    AMBufferParamMap buffer_param_map;
    AMBufferConfigParam buf_conf;

    do {
      if (TEST_BIT(buffer_fmt->enable_bits, AM_BUFFER_FMT_TYPE_EN_BIT)) {
        buf_conf.type.first = true;
        buf_conf.type.second = AM_SOURCE_BUFFER_TYPE(buffer_fmt->type);
      }
#if !defined(CONFIG_AMBARELLA_ORYX_VIDEO_IAV4)
      if (TEST_BIT(buffer_fmt->enable_bits, AM_BUFFER_FMT_INPUT_CROP_EN_BIT)) {
        buf_conf.platform_config.input_crop.first = true;
        buf_conf.platform_config.input_crop.second = buffer_fmt->input_crop;
      }

      if (TEST_BIT(buffer_fmt->enable_bits, AM_BUFFER_FMT_WIDTH_EN_BIT)) {
        buf_conf.platform_config.size.first = true;
        buf_conf.platform_config.size.second.width = buffer_fmt->width;
      }

      if (TEST_BIT(buffer_fmt->enable_bits, AM_BUFFER_FMT_HEIGHT_EN_BIT)) {
        buf_conf.platform_config.size.first = true;
        buf_conf.platform_config.size.second.height = buffer_fmt->height;
      }
#endif
      if (TEST_BIT(buffer_fmt->enable_bits, AM_BUFFER_FMT_INPUT_WIDTH_EN_BIT)) {
        buf_conf.input.first = true;
        buf_conf.input.second.size.width = buffer_fmt->input_width;
      }

      if (TEST_BIT(buffer_fmt->enable_bits,
                   AM_BUFFER_FMT_INPUT_HEIGHT_EN_BIT)) {
        buf_conf.input.first = true;
        buf_conf.input.second.size.height = buffer_fmt->input_height;
      }

      if (TEST_BIT(buffer_fmt->enable_bits, AM_BUFFER_FMT_INPUT_X_EN_BIT)) {
        buf_conf.input.first = true;
        buf_conf.input.second.offset.x = buffer_fmt->input_offset_x;
      }

      if (TEST_BIT(buffer_fmt->enable_bits, AM_BUFFER_FMT_INPUT_Y_EN_BIT)) {
        buf_conf.input.first = true;
        buf_conf.input.second.offset.y = buffer_fmt->input_offset_y;
      }

#if defined(CONFIG_AMBARELLA_ORYX_VIDEO_IAV1)
      if (TEST_BIT(buffer_fmt->enable_bits, AM_BUFFER_FMT_PREWARP_EN_BIT)) {
        buf_conf.platform_config.prewarp.first = true;
        buf_conf.platform_config.prewarp.second = buffer_fmt->prewarp;
      }
#elif defined(CONFIG_AMBARELLA_ORYX_VIDEO_IAV3)
      if (TEST_BIT(buffer_fmt->enable_bits, AM_BUFFER_FMT_CAP_SKIP_ITVL_EN_BIT)) {
        buf_conf.platform_config.cap_skip_itvl.first = true;
        buf_conf.platform_config.cap_skip_itvl.second = buffer_fmt->cap_skip_itvl;
      }

      if (TEST_BIT(buffer_fmt->enable_bits, AM_BUFFER_FMT_AUTO_STOP_EN_BIT)) {
        buf_conf.platform_config.auto_stop.first = true;
        buf_conf.platform_config.auto_stop.second = buffer_fmt->auto_stop;
      }
#endif
      buffer_param_map[id] = buf_conf;
      if (AM_UNLIKELY((svc_ret->ret = g_video_camera->
          set_buffer_config(buffer_param_map)) != AM_RESULT_OK )) {
        ERROR("Failed to set buffer config!");
        break;
      }
    } while(0);
  }
}

void ON_CFG_STREAM_FMT_GET(void *msg_data,
                           int msg_data_size,
                           void *result_addr,
                           int result_max_size)
{
  am_service_result_t *svc_ret = (am_service_result_t*)result_addr;

  memset(svc_ret, 0, sizeof(*svc_ret));
  INFO("Video Service: Stream Format Get");

  if (AM_UNLIKELY(!msg_data)) {
    svc_ret->ret = AM_RESULT_ERR_DATA_POINTER;
    ERROR("Invalid parameter! msg_data is NULL!");
  } else {
    uint32_t stream_id = *((uint32_t*)msg_data);
    am_stream_fmt_t *fmt = (am_stream_fmt_t*)svc_ret->data;
    do {
      AMStreamParamMap stream_param_map;
      if (AM_UNLIKELY((svc_ret->ret = g_video_camera->
          get_stream_config(stream_param_map)) != AM_RESULT_OK)) {
        ERROR("Failed to get stream configuration!");
        break;
      }
      AMStreamFormatConfig &stream_fmt =
          stream_param_map[AM_STREAM_ID(stream_id)].stream_format.second;
      fmt->enable = stream_fmt.enable.second;
      switch(stream_fmt.type.second) {
        case AM_STREAM_TYPE_NONE: {
          fmt->type = 0;
        }break;
        case AM_STREAM_TYPE_H264: {
          fmt->type = 1;
        }break;
        case AM_STREAM_TYPE_H265: {
          fmt->type = 2;
        }break;
        case AM_STREAM_TYPE_MJPEG: {
          fmt->type = 3;
        }break;
        default: {
          fmt->type = 0;
          ERROR("Wrong video type: %d, reset to none!", stream_fmt.type.second);
        }break;
      }
      fmt->source         = stream_fmt.source.second;
      fmt->frame_rate     = stream_fmt.fps.second;
      fmt->width          = stream_fmt.enc_win.second.size.width;
      fmt->height         = stream_fmt.enc_win.second.size.height;
      fmt->offset_x       = stream_fmt.enc_win.second.offset.x;
      fmt->offset_y       = stream_fmt.enc_win.second.offset.y;
      switch(stream_fmt.flip.second) {
        case AM_VIDEO_FLIP_VERTICAL: {
          fmt->hflip = 0;
          fmt->vflip = 1;
        } break;
        case AM_VIDEO_FLIP_HORIZONTAL: {
          fmt->hflip = 1;
          fmt->vflip = 0;
        } break;
        case AM_VIDEO_FLIP_VH_BOTH: {
          fmt->hflip = 1;
          fmt->vflip = 1;
        } break;
        case AM_VIDEO_FLIP_NONE:
        case AM_VIDEO_FLIP_AUTO:
        default: {
          fmt->hflip = 0;
          fmt->vflip = 0;
        } break;
      }
      fmt->rotate = stream_fmt.rotate_90_cw.second ? 1 : 0;
    } while(0);
  }
}

void ON_CFG_STREAM_FMT_SET(void *msg_data,
                           int msg_data_size,
                           void *result_addr,
                           int result_max_size)
{
  am_service_result_t *svc_ret = (am_service_result_t*)result_addr;

  memset(svc_ret, 0, sizeof(*svc_ret));
  INFO("Video Service: Stream Format Set");

  if (AM_UNLIKELY(!msg_data)) {
    svc_ret->ret = AM_RESULT_ERR_DATA_POINTER;
    ERROR("Invalid parameter! msg_data is NULL!");
  } else {
    //only normal mode can do set manipulation
    AM_WORKING_MODE mode;
    if (AM_UNLIKELY((svc_ret->ret = g_video_camera->get_working_mode(mode))
                    != AM_RESULT_OK)) {
      return;
    }
    if (mode != AM_WORKING_MODE_NORAML) {
      return;
    }

    am_stream_fmt_t *fmt = (am_stream_fmt_t*)msg_data;
    do {
      AMStreamParamMap stream_param_map;
      AMStreamConfigParam stream_cfg;
      AMStreamFormatConfig &stream_fmt = stream_cfg.stream_format.second;

      stream_cfg.stream_format.first = true;

      if (TEST_BIT(fmt->enable_bits, AM_STREAM_FMT_ENABLE_EN_BIT)) {
        stream_fmt.enable.first = true;
        stream_fmt.enable.second = fmt->enable;
      }

      if (TEST_BIT(fmt->enable_bits, AM_STREAM_FMT_TYPE_EN_BIT)) {
        stream_fmt.type.first = true;
        stream_fmt.type.second = AM_STREAM_TYPE(fmt->type);
      }

      if (TEST_BIT(fmt->enable_bits, AM_STREAM_FMT_SOURCE_EN_BIT)) {
        stream_fmt.source.first = true;
        stream_fmt.source.second = AM_SOURCE_BUFFER_ID(fmt->source);
      }

      if (TEST_BIT(fmt->enable_bits, AM_STREAM_FMT_FRAME_RATE_EN_BIT)) {
        stream_fmt.fps.first = true;
        stream_fmt.fps.second = fmt->frame_rate;
      }

      if (TEST_BIT(fmt->enable_bits, AM_STREAM_FMT_WIDTH_EN_BIT)) {
        stream_fmt.enc_win.first = true;
        stream_fmt.enc_win.second.size.width = ROUND_UP(fmt->width, 16);
      }

      if (TEST_BIT(fmt->enable_bits, AM_STREAM_FMT_HEIGHT_EN_BIT)) {
        stream_fmt.enc_win.first = true;
        stream_fmt.enc_win.second.size.height = ROUND_UP(fmt->height, 8);
      }

      if (TEST_BIT(fmt->enable_bits, AM_STREAM_FMT_OFFSET_X_EN_BIT)) {
        stream_fmt.enc_win.first = true;
        stream_fmt.enc_win.second.offset.x = ROUND_UP(fmt->offset_x, 2);
      }

      if (TEST_BIT(fmt->enable_bits, AM_STREAM_FMT_OFFSET_Y_EN_BIT)) {
        stream_fmt.enc_win.first = true;
        stream_fmt.enc_win.second.offset.y = ROUND_UP(fmt->offset_y, 2);
      }

      if (TEST_BIT(fmt->enable_bits, AM_STREAM_FMT_ROTATE_EN_BIT)) {
        stream_fmt.rotate_90_cw.first = true;
        stream_fmt.rotate_90_cw.second = fmt->rotate;
      }

      if (TEST_BIT(fmt->enable_bits, AM_STREAM_FMT_HFLIP_EN_BIT) ||
          TEST_BIT(fmt->enable_bits, AM_STREAM_FMT_VFLIP_EN_BIT)) {
        stream_fmt.flip.first = true;
        if (fmt->hflip && fmt->vflip) {
          stream_fmt.flip.second = AM_VIDEO_FLIP_VH_BOTH;
        } else if (fmt->hflip) {
          stream_fmt.flip.second = AM_VIDEO_FLIP_HORIZONTAL;
        } else if (fmt->vflip) {
          stream_fmt.flip.second = AM_VIDEO_FLIP_VERTICAL;
        } else {
          /* Should never come here */
          stream_fmt.flip.second = AM_VIDEO_FLIP_AUTO;
        }
      }

      stream_param_map[AM_STREAM_ID(fmt->stream_id)] = stream_cfg;
      if (AM_UNLIKELY((svc_ret->ret = g_video_camera->
          set_stream_config(stream_param_map)) != AM_RESULT_OK)) {
        ERROR("Failed to set stream format!");
        break;
      }
    } while(0);
  }
}

void ON_CFG_STREAM_H26x_GET(void *msg_data,
                            int msg_data_size,
                            void *result_addr,
                            int result_max_size)
{
  am_service_result_t *svc_ret = (am_service_result_t*)result_addr;
  memset(svc_ret, 0, sizeof(am_service_result_t));
  INFO("Video Service: Stream Config Get");

  if (AM_UNLIKELY(!msg_data)) {
    svc_ret->ret = AM_RESULT_ERR_DATA_POINTER;
    ERROR("Invalid parameter! msg_data is NULL!");
  } else {
    am_h26x_cfg_t *air_cfg = (am_h26x_cfg_t*)svc_ret->data;
    AM_STREAM_ID stream_id = AM_STREAM_ID(*((uint32_t*)msg_data));
    do {
      AMStreamParamMap stream_param_map;
      if (AM_UNLIKELY((svc_ret->ret = g_video_camera->
          get_stream_config(stream_param_map)) != AM_RESULT_OK)) {
        ERROR("Failed to get stream configuration!");
        break;
      }
      AMStreamH26xConfig &h26x_cfg =
          stream_param_map[stream_id].h26x_config.second;

      /* H.264 Config */
      air_cfg->stream_id = stream_id;
      air_cfg->bitrate_ctrl = h26x_cfg.bitrate_control.second;
      air_cfg->profile = h26x_cfg.profile_level.second;
      air_cfg->au_type = h26x_cfg.au_type.second;
      air_cfg->chroma = h26x_cfg.chroma_format.second;
      air_cfg->M = h26x_cfg.M.second;
      air_cfg->N = h26x_cfg.N.second;
      air_cfg->idr_interval = h26x_cfg.idr_interval.second;
      air_cfg->target_bitrate = h26x_cfg.target_bitrate.second;
      air_cfg->mv_threshold = h26x_cfg.mv_threshold.second;
      air_cfg->flat_area_improve = h26x_cfg.flat_area_improve.second ? 1 : 0;
      air_cfg->multi_ref_p = h26x_cfg.multi_ref_p.second;
      air_cfg->fast_seek_intvl = h26x_cfg.fast_seek_intvl.second;
      /* HEVC */
      air_cfg->slice_num = h26x_cfg.slice_num.second;
      air_cfg->slices_per_info = h26x_cfg.slices_per_info.second;
    }while(0);
  }
}

void ON_CFG_STREAM_H26x_SET(void *msg_data,
                            int msg_data_size,
                            void *result_addr,
                            int result_max_size)
{
  am_service_result_t *svc_ret = (am_service_result_t*)result_addr;

  memset(svc_ret, 0, sizeof(*svc_ret));
  INFO("Video Service: Stream Config Set");

  if (AM_UNLIKELY(!msg_data)) {
    svc_ret->ret = AM_RESULT_ERR_DATA_POINTER;
    ERROR("Invalid parameter! msg_data is NULL!");
  } else {
    //only normal mode can do set manipulation
    AM_WORKING_MODE mode;
    if (AM_UNLIKELY((svc_ret->ret = g_video_camera->get_working_mode(mode))
                    != AM_RESULT_OK)) {
      return;
    }
    if (mode != AM_WORKING_MODE_NORAML) {
      return;
    }

    am_h26x_cfg_t *air_cfg = (am_h26x_cfg_t*)msg_data;
    do {
      AMStreamParamMap stream_param_map;
      AMStreamConfigParam stream_cfg;
      AMStreamH26xConfig &h26x_cfg = stream_cfg.h26x_config.second;
      stream_cfg.h26x_config.first = true;

      /* H.264 Config */
      if (TEST_BIT(air_cfg->enable_bits, AM_H26x_CFG_BITRATE_CTRL_EN_BIT)) {
        h26x_cfg.bitrate_control.first = true;
        h26x_cfg.bitrate_control.second =
            AM_RATE_CONTROL(air_cfg->bitrate_ctrl);
      }

      if (TEST_BIT(air_cfg->enable_bits, AM_H26x_CFG_PROFILE_EN_BIT)) {
        h26x_cfg.profile_level.first = true;
        h26x_cfg.profile_level.second = AM_PROFILE(air_cfg->profile);
      }

      if (TEST_BIT(air_cfg->enable_bits, AM_H26x_CFG_AU_TYPE_EN_BIT)) {
        h26x_cfg.au_type.first = true;
        h26x_cfg.au_type.second = AM_AU_TYPE(air_cfg->au_type);
      }

      if (TEST_BIT(air_cfg->enable_bits, AM_H26x_CFG_CHROMA_EN_BIT)) {
        h26x_cfg.chroma_format.first = true;
        h26x_cfg.chroma_format.second = AM_CHROMA_FORMAT(air_cfg->chroma);
      }

      if (TEST_BIT(air_cfg->enable_bits, AM_H26x_CFG_M_EN_BIT)) {
        h26x_cfg.M.first = true;
        h26x_cfg.M.second = air_cfg->M;
      }

      if (TEST_BIT(air_cfg->enable_bits, AM_H26x_CFG_N_EN_BIT)) {
        h26x_cfg.N.first = true;
        h26x_cfg.N.second = air_cfg->N;
      }

      if (TEST_BIT(air_cfg->enable_bits, AM_H26x_CFG_IDR_EN_BIT)) {
        h26x_cfg.idr_interval.first = true;
        h26x_cfg.idr_interval.second = air_cfg->idr_interval;
      }

      if (TEST_BIT(air_cfg->enable_bits, AM_H26x_CFG_SLICE_NUM_EN_BIT)) {
        h26x_cfg.slice_num.first = true;
        h26x_cfg.slice_num.second = air_cfg->slice_num;
      }

      if (TEST_BIT(air_cfg->enable_bits, AM_H26x_CFG_SLICES_PER_INFO_EN_BIT)) {
        h26x_cfg.slices_per_info.first = true;
        h26x_cfg.slices_per_info.second = air_cfg->slices_per_info;
      }

      if (TEST_BIT(air_cfg->enable_bits, AM_H26x_CFG_BITRATE_EN_BIT)) {
        h26x_cfg.target_bitrate.first = true;
        h26x_cfg.target_bitrate.second = air_cfg->target_bitrate;
      }

      if (TEST_BIT(air_cfg->enable_bits, AM_H26x_CFG_MV_THRESHOLD_EN_BIT)) {
        h26x_cfg.mv_threshold.first = true;
        h26x_cfg.mv_threshold.second = air_cfg->mv_threshold;
      }

      if (TEST_BIT(air_cfg->enable_bits, AM_H26x_CFG_FLAT_AREA_IMPROVE_EN_BIT)) {
        h26x_cfg.flat_area_improve.first = true;
        h26x_cfg.flat_area_improve.second =
            (air_cfg->flat_area_improve == 0) ? 0 : 1;
      }

      if (TEST_BIT(air_cfg->enable_bits, AM_H26x_CFG_MULTI_REF_P_EN_BIT)) {
        h26x_cfg.multi_ref_p.first = true;
        h26x_cfg.multi_ref_p.second = air_cfg->multi_ref_p;
      }

      if (TEST_BIT(air_cfg->enable_bits,
                   AM_H26x_CFG_FAST_SEEK_INTVL_EN_BIT)) {
        h26x_cfg.fast_seek_intvl.first = true;
        h26x_cfg.fast_seek_intvl.second = air_cfg->fast_seek_intvl;
      }

      stream_param_map[AM_STREAM_ID(air_cfg->stream_id)] = stream_cfg;
      if (AM_UNLIKELY((svc_ret->ret = g_video_camera->
          set_stream_config(stream_param_map)) != AM_RESULT_OK)) {
        ERROR("Failed to set stream configuration!");
        break;
      }
    } while(0);
  }
}

void ON_CFG_STREAM_MJPEG_GET(void *msg_data,
                             int msg_data_size,
                             void *result_addr,
                             int result_max_size)
{
  am_service_result_t *svc_ret = (am_service_result_t*)result_addr;
  memset(svc_ret, 0, sizeof(*svc_ret));
  INFO("Video Service: Stream Config Get");

  if (AM_UNLIKELY(!msg_data)) {
    svc_ret->ret = AM_RESULT_ERR_DATA_POINTER;
    ERROR("Invalid parameter! msg_data is NULL!");
  } else {
    am_mjpeg_cfg_t *air_cfg = (am_mjpeg_cfg_t*)svc_ret->data;
    do {
      AMStreamParamMap stream_param_map;
      AM_STREAM_ID stream_id = AM_STREAM_ID(*((uint32_t*)msg_data));
      if (AM_UNLIKELY((svc_ret->ret = g_video_camera->
          get_stream_config(stream_param_map)) != AM_RESULT_OK)) {
        ERROR("Failed to get stream configuration!");
        break;
      }
      AMStreamMJPEGConfig &mjpeg_cfg =
          stream_param_map[stream_id].mjpeg_config.second;

      /* MJpeg Config */
      air_cfg->quality = mjpeg_cfg.quality.second;
      air_cfg->chroma = mjpeg_cfg.chroma_format.second;
    } while(0);
  }
}

void ON_CFG_STREAM_MJPEG_SET(void *msg_data,
                             int msg_data_size,
                             void *result_addr,
                             int result_max_size)
{
  am_service_result_t *svc_ret = (am_service_result_t*)result_addr;

  memset(svc_ret, 0, sizeof(*svc_ret));
  INFO("Video Service: Stream Config Set");

  if (AM_UNLIKELY(!msg_data)) {
    svc_ret->ret = AM_RESULT_ERR_DATA_POINTER;
    ERROR("Invalid parameter! msg_data is NULL!");
  } else {
    //only normal mode can do set manipulation
    AM_WORKING_MODE mode;
    if (AM_UNLIKELY((svc_ret->ret = g_video_camera->get_working_mode(mode))
                    != AM_RESULT_OK)) {
      return;
    }
    if (mode != AM_WORKING_MODE_NORAML) {
      return;
    }

    am_mjpeg_cfg_t *air_cfg = (am_mjpeg_cfg_t*)msg_data;
    do {
      AMStreamParamMap stream_param_map;
      AMStreamConfigParam stream_cfg;
      AMStreamMJPEGConfig &mjpeg_cfg = stream_cfg.mjpeg_config.second;

      stream_cfg.mjpeg_config.first = true;
      /* MJpeg Config */
      if (TEST_BIT(air_cfg->enable_bits, AM_MJPEG_CFG_QUALITY_EN_BIT)) {
        mjpeg_cfg.quality.first = true;
        mjpeg_cfg.quality.second = air_cfg->quality;
      }

      if (TEST_BIT(air_cfg->enable_bits, AM_MJPEG_CFG_CHROMA_EN_BIT)) {
        mjpeg_cfg.chroma_format.first = true;
        mjpeg_cfg.chroma_format.second = AM_CHROMA_FORMAT(air_cfg->chroma);
      }

      stream_param_map[AM_STREAM_ID(air_cfg->stream_id)] = stream_cfg;
      if (AM_UNLIKELY((svc_ret->ret = g_video_camera->
          set_stream_config(stream_param_map)) != AM_RESULT_OK)) {
        ERROR("Failed to set stream configuration!");
        break;
      }
    } while(0);
  }
}

void ON_DYN_VOUT_HALT(void *msg_data,
                      int msg_data_size,
                      void *result_addr,
                      int result_max_size)
{
  do {
    am_service_result_t *svc_ret = (am_service_result_t*)result_addr;
    memset(svc_ret, 0, sizeof(*svc_ret));
    INFO("Video Service: Vout Halt");
    if (AM_UNLIKELY(!msg_data)) {
      ERROR("NULL pointer!\n");
      svc_ret->ret = AM_RESULT_ERR_DATA_POINTER;
      break;
    }
    //only normal mode can do set manipulation
    AM_WORKING_MODE mode;
    if (AM_UNLIKELY((svc_ret->ret = g_video_camera->get_working_mode(mode))
                    != AM_RESULT_OK)) {
      return;
    }
    if (mode != AM_WORKING_MODE_NORAML) {
      return;
    }

    AM_VOUT_ID *id = (AM_VOUT_ID*) msg_data;
    if (AM_UNLIKELY((svc_ret->ret = g_video_camera->
        halt_vout(*id)) != AM_RESULT_OK)) {
      ERROR("Failed to stop vout!");
    }
  } while(0);

  return;
}

void ON_DYN_BUFFER_STATE_GET(void *msg_data,
                             int msg_data_size,
                             void *result_addr,
                             int result_max_size)
{
  INFO("video service ON_DYN_BUFFER_STATE_GET");
  int32_t ret = 0;
  am_service_result_t *result = (am_service_result_t*)result_addr;
  memset(result, 0, sizeof(am_service_result_t));

  do {
    if (!result_addr) {
      ret = AM_RESULT_ERR_DATA_POINTER;
      ERROR("result_addr is null!");
      break;
    }

    if (!msg_data) {
      ret = AM_RESULT_ERR_DATA_POINTER;
      ERROR("msg_data is null!");
      break;
    }
    //iav unloaded mode can't manipulate iav
    AM_WORKING_MODE mode;
    if (AM_UNLIKELY((result->ret = g_video_camera->get_working_mode(mode))
                    != AM_RESULT_OK)) {
      return;
    }
    if (mode == AM_WORKING_MODE_IAV_UNLOADED) {
      return;
    }

    AM_SRCBUF_STATE state;
    AM_SOURCE_BUFFER_ID *id = (AM_SOURCE_BUFFER_ID*)msg_data;
    if (AM_UNLIKELY((ret = g_video_camera->get_buffer_state(*id, state))
                    != AM_RESULT_OK)) {
      ERROR("Failed to get buffer state!");
      break;
    }
    am_buffer_state_t *buf_state = (am_buffer_state_t*)result->data;
    buf_state->buffer_id = *id;
    switch (state) {
      case AM_SRCBUF_STATE_ERROR:
        buf_state->state = 2;
        break;
      case AM_SRCBUF_STATE_IDLE:
        buf_state->state = 0;
        break;
      case AM_SRCBUF_STATE_BUSY:
        buf_state->state = 1;
        break;
      case AM_SRCBUF_STATE_UNKNOWN:
      default:
        buf_state->state = 3;
        break;
    }
  } while (0);
  result->ret = ret;
}

void ON_DYN_BUFFER_FMT_GET(void *msg_data,
                           int msg_data_size,
                           void *result_addr,
                           int result_max_size)
{
  INFO("video service ON_DYN_BUFFER_GET");
  int32_t ret = 0;
  am_service_result_t *result = (am_service_result_t*)result_addr;
  memset(result, 0, sizeof(am_service_result_t));

  do {
    if (!result_addr) {
      ret = AM_RESULT_ERR_DATA_POINTER;
      ERROR("result_addr is null!");
      break;
    }

    if (!msg_data) {
      ret = AM_RESULT_ERR_DATA_POINTER;
      ERROR("msg_data is null!");
      break;
    }
    //iav unloaded mode can't manipulate iav
    AM_WORKING_MODE mode;
    if (AM_UNLIKELY((result->ret = g_video_camera->get_working_mode(mode))
                    != AM_RESULT_OK)) {
      return;
    }
    if (mode == AM_WORKING_MODE_IAV_UNLOADED) {
      return;
    }

    am_buffer_id_t *buffer_id = (am_buffer_id_t*)msg_data;
    AMBufferConfigParam param;
#if defined(CONFIG_AMBARELLA_ORYX_VIDEO_IAV4)
    param.platform_config.channel_id.second =
        AM_MULTI_VIN_CHAN_ID(buffer_id->channel_id);
#endif
    param.id = AM_SOURCE_BUFFER_ID(buffer_id->buffer_id);
    if (AM_UNLIKELY((ret = g_video_camera->get_buffer_format(param))
                    != AM_RESULT_OK)) {
      ERROR("Failed to get buffer format!");
      break;
    }

    am_buffer_fmt_t *buffer = (am_buffer_fmt_t*)result->data;
    buffer->buffer_id = param.id;
    buffer->type = param.type.second;
    buffer->input_offset_x = param.input.second.offset.x;
    buffer->input_offset_y = param.input.second.offset.y;
    buffer->input_width = param.input.second.size.width;
    buffer->input_height = param.input.second.size.height;
#if defined(CONFIG_AMBARELLA_ORYX_VIDEO_IAV4)
    buffer->channel_id = param.platform_config.channel_id.second;
    buffer->canvas_id = param.platform_config.canvas_id.second;
    buffer->width = param.platform_config.output.second.size.width;
    buffer->height = param.platform_config.output.second.size.height;
    buffer->output_offset_x = param.platform_config.output.second.offset.x;
    buffer->output_offset_y = param.platform_config.output.second.offset.y;
#else
    buffer->width = param.platform_config.size.second.width;
    buffer->height = param.platform_config.size.second.height;
#endif
#if defined(CONFIG_AMBARELLA_ORYX_VIDEO_IAV3)
    buffer->cap_skip_itvl = param.platform_config.cap_skip_itvl.second;
    buffer->auto_stop = param.platform_config.auto_stop.second;
#endif
  } while (0);
  result->ret = ret;
}

void ON_DYN_BUFFER_FMT_SET(void *msg_data,
                           int msg_data_size,
                           void *result_addr,
                           int result_max_size)
{
  INFO("video service ON_DYN_BUFFER_SET");
  int32_t ret = 0;
  am_service_result_t *result = (am_service_result_t*)result_addr;
  memset(result, 0, sizeof(am_service_result_t));

  do {
    if (!result_addr) {
      ret = AM_RESULT_ERR_DATA_POINTER;
      ERROR("result_addr is null!");
      break;
    }

    if (!msg_data) {
      ret = AM_RESULT_ERR_DATA_POINTER;
      ERROR("msg_data is null!");
      break;
    }

    //only normal mode can do set manipulation
    AM_WORKING_MODE mode;
    if (AM_UNLIKELY((result->ret = g_video_camera->get_working_mode(mode))
                    != AM_RESULT_OK)) {
      return;
    }
    if (mode != AM_WORKING_MODE_NORAML) {
      return;
    }

    am_buffer_fmt_t *buffer = (am_buffer_fmt_t*)msg_data;
    AMBufferConfigParam param;
    param.id = AM_SOURCE_BUFFER_ID(buffer->buffer_id);
    if (TEST_BIT(buffer->enable_bits, AM_BUFFER_FMT_INPUT_X_EN_BIT)) {
      param.input.first = true;
      param.input.second.offset.x = ROUND_UP(buffer->input_offset_x, 2);
#if !defined(CONFIG_AMBARELLA_ORYX_VIDEO_IAV4)
      param.platform_config.input_crop.first = true;
      param.platform_config.input_crop.second = true;
#endif
    }

    if (TEST_BIT(buffer->enable_bits, AM_BUFFER_FMT_INPUT_Y_EN_BIT)) {
      param.input.first = true;
      param.input.second.offset.y = ROUND_UP(buffer->input_offset_y, 4);
#if !defined(CONFIG_AMBARELLA_ORYX_VIDEO_IAV4)
      param.platform_config.input_crop.first = true;
      param.platform_config.input_crop.second = true;
#endif
    }

    if (TEST_BIT(buffer->enable_bits, AM_BUFFER_FMT_INPUT_WIDTH_EN_BIT)) {
      param.input.first = true;
      param.input.second.size.width = ROUND_UP(buffer->input_width, 2);
#if !defined(CONFIG_AMBARELLA_ORYX_VIDEO_IAV4)
      param.platform_config.input_crop.first = true;
      param.platform_config.input_crop.second = true;
#endif
    }

    if (TEST_BIT(buffer->enable_bits, AM_BUFFER_FMT_INPUT_HEIGHT_EN_BIT)) {
      param.input.first = true;
      param.input.second.size.height = ROUND_UP(buffer->input_height, 4);
#if !defined(CONFIG_AMBARELLA_ORYX_VIDEO_IAV4)
      param.platform_config.input_crop.first = true;
      param.platform_config.input_crop.second = true;
#endif
    }

    if (TEST_BIT(buffer->enable_bits, AM_BUFFER_FMT_WIDTH_EN_BIT)) {
#if defined(CONFIG_AMBARELLA_ORYX_VIDEO_IAV4)
      param.platform_config.output.first = true;
      param.platform_config.output.second.size.width =
                                               ROUND_UP(buffer->width, 16);
#else
      param.platform_config.size.first = true;
      param.platform_config.size.second.width = ROUND_UP(buffer->width, 16);
#endif
    }

    if (TEST_BIT(buffer->enable_bits, AM_BUFFER_FMT_HEIGHT_EN_BIT)) {
#if defined(CONFIG_AMBARELLA_ORYX_VIDEO_IAV4)
      param.platform_config.output.first = true;
      param.platform_config.output.second.size.height =
                                               ROUND_UP(buffer->height, 8);
#else
      param.platform_config.size.first = true;
      param.platform_config.size.second.height = ROUND_UP(buffer->height, 8);
#endif
    }

#if defined(CONFIG_AMBARELLA_ORYX_VIDEO_IAV4)
    param.platform_config.need_apply.second = true;
    param.platform_config.channel_id.second =
        AM_MULTI_VIN_CHAN_ID(buffer->channel_id);

    if (TEST_BIT(buffer->enable_bits, AM_BUFFER_FMT_OUTPUT_X_EN_BIT)) {
      param.platform_config.output.first = true;
      param.platform_config.output.second.offset.x =
                                        ROUND_UP(buffer->output_offset_x, 16);
    }

    if (TEST_BIT(buffer->enable_bits, AM_BUFFER_FMT_OUTPUT_Y_EN_BIT)) {
      param.platform_config.output.first = true;
      param.platform_config.output.second.offset.y =
                                        ROUND_UP(buffer->output_offset_y, 8);
    }
#endif

    if (!TEST_BIT(buffer->enable_bits, AM_BUFFER_FMT_SAVE_CFG_EN_BIT)) {
      if (AM_UNLIKELY((ret = g_video_camera->set_buffer_format(param))
                      != AM_RESULT_OK)) {
        ERROR("Failed to set buffer format!");
        break;
      }
    }
    if (TEST_BIT(buffer->enable_bits, AM_BUFFER_FMT_SAVE_CFG_EN_BIT)) {
      if (AM_UNLIKELY((ret = g_video_camera->save_buffer_config())
                  != AM_RESULT_OK)) {
        ERROR("Failed to save current buffer config!");
        break;
      }
    }
  } while (0);
  result->ret = ret;
}

void ON_DYN_STREAM_MAX_NUM_GET(void *msg_data,
                               int msg_data_size,
                               void *result_addr,
                               int result_max_size)
{
  INFO("video service ON_DYN_STREAM_MAX_NUM_GET\n");
  am_service_result_t *svc_ret = (am_service_result_t*) result_addr;
  memset(svc_ret, 0, sizeof(am_service_result_t));

  uint32_t *val = (uint32_t*)svc_ret->data;
  *val = g_video_camera->get_encode_stream_max_num();

  svc_ret->ret = AM_RESULT_OK;
}

void ON_DYN_CANVAS_STATE_GET(void *msg_data,
                             int msg_data_size,
                             void *result_addr,
                             int result_max_size)
{
  INFO("video service ON_DYN_CANVAS_STATE_GET");
  int32_t ret = 0;
  am_service_result_t *svc_ret = (am_service_result_t*) result_addr;
  memset(svc_ret, 0, sizeof(am_service_result_t));

  do {
    if (AM_UNLIKELY(nullptr == result_addr)) {
      ret = AM_RESULT_ERR_DATA_POINTER;
      ERROR("result_addr is null!");
      break;
    }
    if (AM_UNLIKELY(nullptr == msg_data)) {
      ret = AM_RESULT_ERR_DATA_POINTER;
      ERROR("msg_data is null!");
      break;
    }

    //iav unloaded mode can't manipulate iav
    AM_WORKING_MODE mode;
    if (AM_UNLIKELY((svc_ret->ret = g_video_camera->get_working_mode(mode))
                    != AM_RESULT_OK)) {
      return;
    }
    if (mode == AM_WORKING_MODE_IAV_UNLOADED) {
      return;
    }

    uint8_t *canvas_id = (uint8_t*)msg_data;
    AM_MULTI_VIN_CANVAS_STATE state = AM_MULTI_VIN_CANVAS_STATE_UNKNOWN;

    if (AM_UNLIKELY((ret = g_video_camera->get_canvas_state(*canvas_id, state))
                    != AM_RESULT_OK)) {
      ERROR("Failed to get canvas state!");
      break;
    }
    am_canvas_state_t *canvas_state = (am_canvas_state_t*)svc_ret->data;
    canvas_state->canvas_id = *canvas_id;
    canvas_state->state = state;

  } while(0);

  svc_ret->ret = ret;
}

void ON_DYN_CANVAS_INFO_GET(void *msg_data,
                            int msg_data_size,
                            void *result_addr,
                            int result_max_size)
{
  INFO("video service ON_DYN_CANVAS_INFO_GET");
  int32_t ret = 0;
  am_service_result_t *svc_ret = (am_service_result_t*) result_addr;
  memset(svc_ret, 0, sizeof(am_service_result_t));

  do {
    if (AM_UNLIKELY(nullptr == result_addr)) {
      ret = AM_RESULT_ERR_DATA_POINTER;
      ERROR("result_addr is null!");
      break;
    }

    //iav unloaded mode can't manipulate iav
    AM_WORKING_MODE mode;
    if (AM_UNLIKELY((svc_ret->ret = g_video_camera->get_working_mode(mode))
                    != AM_RESULT_OK)) {
      return;
    }
    if (mode == AM_WORKING_MODE_IAV_UNLOADED) {
      return;
    }

    AMMultiVinCanvasParam canvas;
    if (AM_UNLIKELY((ret = g_video_camera->get_canvas_info(canvas))
                    != AM_RESULT_OK)) {
      ERROR("Failed to get canvas info!");
      break;
    }

    am_canvas_info_t *canvas_info = (am_canvas_info_t*)svc_ret->data;
    for (uint32_t i = 0; i < canvas.canvas.second.size(); ++i) {
      canvas_info[i].canvas_id = i;
      canvas_info[i].canvas_type = canvas.canvas.second.at(i).type.second;
      canvas_info[i].width = canvas.canvas.second.at(i).size.second.width;
      canvas_info[i].height = canvas.canvas.second.at(i).size.second.height;
    }
  } while(0);

  svc_ret->ret = ret;
}

void ON_DYN_CHANNEL_NUM_GET(void *msg_data,
                            int msg_data_size,
                            void *result_addr,
                            int result_max_size)
{
  INFO("video service ON_DYN_CHANNEL_NUM_GET");
  am_service_result_t *svc_ret = (am_service_result_t*) result_addr;
  memset(svc_ret, 0, sizeof(am_service_result_t));

  uint32_t *val = (uint32_t*)svc_ret->data;
  *val = g_video_camera->get_channel_num();

  svc_ret->ret = AM_RESULT_OK;
}

void ON_DYN_BUFFER_MAX_NUM_GET(void *msg_data,
                               int msg_data_size,
                               void *result_addr,
                               int result_max_size)
{
  INFO("video service ON_DYN_BUFFER_MAX_NUM_GET\n");
  am_service_result_t *svc_ret = (am_service_result_t*) result_addr;
  memset(svc_ret, 0, sizeof(am_service_result_t));

  uint32_t *val = (uint32_t*)svc_ret->data;
  *val = g_video_camera->get_source_buffer_max_num();

  svc_ret->ret = AM_RESULT_OK;
}

void ON_DYN_STREAM_STATUS_GET(void *msg_data,
                              int msg_data_size,
                              void *result_addr,
                              int result_max_size)
{
  INFO("Video Service: ON_DYN_STREAM_STATUS_GET");
  am_service_result_t *svc_ret = (am_service_result_t*)result_addr;
  memset(svc_ret, 0, sizeof(*svc_ret));
  if (!msg_data) {
    svc_ret->ret = AM_RESULT_ERR_DATA_POINTER;
    return;
  }
  //iav unloaded mode can't manipulate iav
  AM_WORKING_MODE mode;
  if (AM_UNLIKELY((svc_ret->ret = g_video_camera->get_working_mode(mode))
                  != AM_RESULT_OK)) {
    return;
  }
  if (mode == AM_WORKING_MODE_IAV_UNLOADED) {
    return;
  }

  am_stream_status_t *status = (am_stream_status_t*)svc_ret->data;
  uint32_t stream_num_max = g_video_camera->get_encode_stream_max_num();
  for (uint32_t id = AM_STREAM_ID_0; id < stream_num_max; ++id) {
    AM_STREAM_STATE state;
    if (AM_LIKELY((svc_ret->ret = g_video_camera->
        get_stream_status(AM_STREAM_ID(id), state)) == AM_RESULT_OK)) {
      if (state == AM_STREAM_STATE_ENCODING) {
        status->status |= 1 << id;
      }
    }
  }
}

#define MAX_UUID_NUM 6
static uint8_t uuids[MAX_UUID_NUM][16] = {0};
static uint8_t lock_state[MAX_UUID_NUM] = {0};

void ON_DYN_STREAM_LOCK_STATE_GET(void *msg_data,
                                  int msg_data_size,
                                  void *result_addr,
                                  int result_max_size)
{
  INFO("Video Service: ON_DYN_STREAM_LOCK_STATE");
  am_service_result_t *svc_ret = (am_service_result_t*)result_addr;
  memset(svc_ret, 0, sizeof(*svc_ret));

  uint32_t *lock_bitmap = (uint32_t*)svc_ret->data;
  bzero(lock_bitmap, sizeof(uint32_t));

  for (uint32_t i = 0; i < MAX_UUID_NUM; ++i) {
    if (lock_state[i]) {
      *lock_bitmap |= 1 << i;
    }
  }
}

void ON_DYN_STREAM_LOCK(void *msg_data,
                        int msg_data_size,
                        void *result_addr,
                        int result_max_size)
{
  INFO("Video Service: ON_DYN_STREAM_TEST_AND_LOCK");
  am_service_result_t *svc_ret = (am_service_result_t*)result_addr;
  memset(svc_ret, 0, sizeof(*svc_ret));

  do {
    if (!msg_data) {
      svc_ret->ret = AM_RESULT_ERR_DATA_POINTER;
      ERROR("msg_data is null!");
      break;
    }
    am_stream_lock_t *lock = (am_stream_lock_t*)msg_data;
    if (lock->operation == 255) { //Reset lock state
      bzero(lock_state, MAX_UUID_NUM);
      break;
    }
    if (lock->stream_id >= MAX_UUID_NUM) {
      lock->op_result = 1;
      ERROR("Stream ID is out of range: %d", MAX_UUID_NUM);
      break;
    }
    if (uuid_is_null(lock->uuid)) {
      svc_ret->ret = AM_RESULT_ERR_INVALID;
      ERROR("UUID is null, please check!");
    }
    am_stream_lock_t *lock_result = (am_stream_lock_t*)svc_ret->data;
    *lock_result = *lock;
    if (!lock_state[lock->stream_id]) { //stream is unlock
      if (lock->operation == 1) { //try to lock stream
        memcpy(uuids[lock->stream_id], lock->uuid, 16);
        lock_state[lock->stream_id] = 1;
      }
      lock_result->op_result = 0;
    } else if (lock_state[lock->stream_id]) { //stream is locked
      if (!uuid_compare(lock->uuid, uuids[lock->stream_id])) { //UUID matched
        if (lock->operation == 0) { //try to unlock stream
          lock_state[lock->stream_id] = 0;
        }
        lock_result->op_result = 0;
      } else {
        lock_result->op_result = 1;
      }
    }
  } while (0);
}

void ON_DYN_STREAM_START(void *msg_data,
                         int msg_data_size,
                         void *result_addr,
                         int result_max_size)
{
  INFO("Video Service: ON_DYN_STREAM_START");
  am_service_result_t *svc_ret = (am_service_result_t*)result_addr;
  memset(svc_ret, 0, sizeof(*svc_ret));
  do {
    if (!msg_data) {
      svc_ret->ret = AM_RESULT_ERR_DATA_POINTER;
      break;
    }
    //only normal mode can do set manipulation
    AM_WORKING_MODE mode;
    if (AM_UNLIKELY((svc_ret->ret = g_video_camera->get_working_mode(mode))
                    != AM_RESULT_OK)) {
      return;
    }
    if (mode != AM_WORKING_MODE_NORAML) {
      return;
    }

    AM_STREAM_ID id = *((AM_STREAM_ID*)msg_data);
    AM_STREAM_STATE state;
    if (AM_UNLIKELY((svc_ret->ret = g_video_camera->
        get_stream_status(id, state)) != AM_RESULT_OK)) {
      break;
    }
    if ((state == AM_STREAM_STATE_IDLE) ||
        (state == AM_STREAM_STATE_STOPPING)) {
      if (AM_UNLIKELY((svc_ret->ret = g_video_camera->start_stream(id))
                      != AM_RESULT_OK)) {
        break;
      }
    }
  } while (0);
}

void ON_DYN_STREAM_STOP(void *msg_data,
                        int msg_data_size,
                        void *result_addr,
                        int result_max_size)
{
  INFO("Video Service: ON_DYN_STREAM_STOP");
  am_service_result_t *svc_ret = (am_service_result_t*)result_addr;
  memset(svc_ret, 0, sizeof(*svc_ret));
  do {
    if (!msg_data) {
      svc_ret->ret = AM_RESULT_ERR_DATA_POINTER;
      break;
    }
    //only normal mode can do set manipulation
    AM_WORKING_MODE mode;
    if (AM_UNLIKELY((svc_ret->ret = g_video_camera->get_working_mode(mode))
                    != AM_RESULT_OK)) {
      return;
    }
    if (mode != AM_WORKING_MODE_NORAML) {
      return;
    }

    AM_STREAM_ID id = *((AM_STREAM_ID*)msg_data);
    AM_STREAM_STATE state;
    if (AM_UNLIKELY((svc_ret->ret = g_video_camera->
        get_stream_status(id, state)) != AM_RESULT_OK)) {
      break;
    }
    if ((state == AM_STREAM_STATE_ENCODING) ||
        (state == AM_STREAM_STATE_STARTING)) {
      if (AM_UNLIKELY((svc_ret->ret = g_video_camera->stop_stream(id))
                      != AM_RESULT_OK)) {
        break;
      }
    }
  } while (0);
}

void ON_DYN_STREAM_FORCE_IDR(void *msg_data,
                             int msg_data_size,
                             void *result_addr,
                             int result_max_size)
{
  INFO("video service ON_DYN_STREAM_FORCE_IDR\n");
  am_service_result_t *svc_ret = (am_service_result_t*) result_addr;
  memset(svc_ret, 0, sizeof(am_service_result_t));

  if (!msg_data) {
    ERROR("NULL pointer!\n");
    svc_ret->ret = AM_RESULT_ERR_DATA_POINTER;
    return;
  }
  //only normal mode can do set manipulation
  AM_WORKING_MODE mode;
  if (AM_UNLIKELY((svc_ret->ret = g_video_camera->get_working_mode(mode))
                  != AM_RESULT_OK)) {
    return;
  }
  if (mode != AM_WORKING_MODE_NORAML) {
    return;
  }

  AM_STREAM_ID id = *(AM_STREAM_ID*)msg_data;
  svc_ret->ret = g_video_camera->force_idr(id);
}

void ON_DYN_STREAM_PARAMETERS_SET(void *msg_data,
                                  int msg_data_size,
                                  void *result_addr,
                                  int result_max_size)
{
  INFO("Video Service: ON_DYN_STREAM_PARAMETERS_SET");
  am_service_result_t *svc_ret = (am_service_result_t*)result_addr;
  memset(svc_ret, 0, sizeof(*svc_ret));
  do {
    if (!msg_data) {
      svc_ret->ret = AM_RESULT_ERR_DATA_POINTER;
      break;
    }
    //only normal mode can do set manipulation
    AM_WORKING_MODE mode;
    if (AM_UNLIKELY((svc_ret->ret = g_video_camera->get_working_mode(mode))
                    != AM_RESULT_OK)) {
      return;
    }
    if (mode != AM_WORKING_MODE_NORAML) {
      return;
    }

    am_stream_parameter_t *params = (am_stream_parameter_t *)msg_data;

    AM_STREAM_ID id = AM_STREAM_ID(params->stream_id);

    if (TEST_BIT(params->enable_bits, AM_STREAM_DYN_CTRL_SOURCE_EN_BIT)) {
      AM_SOURCE_BUFFER_ID buf;
      if (AM_UNLIKELY((svc_ret->ret = g_video_camera->
          get_stream_source(id, buf)) != AM_RESULT_OK)) {
        break;
      }
      if (buf != int32_t(params->source)) {
        buf = AM_SOURCE_BUFFER_ID(params->source);
        if (AM_UNLIKELY((svc_ret->ret = g_video_camera->
            set_stream_source(id, buf)) != AM_RESULT_OK)) {
          break;
        }
      }
    }

    if (TEST_BIT(params->enable_bits, AM_STREAM_DYN_CTRL_TYPE_EN_BIT)) {
      AM_STREAM_TYPE type;
      if (AM_UNLIKELY((svc_ret->ret = g_video_camera->get_stream_type(id, type))
                      != AM_RESULT_OK)) {
        break;
      }
      if (type != params->type) {
        type = AM_STREAM_TYPE(params->type);
        if (AM_UNLIKELY((svc_ret->ret = g_video_camera->
            set_stream_type(id, type)) != AM_RESULT_OK)) {
          break;
        }
      }
    }

    if (TEST_BIT(params->enable_bits, AM_STREAM_DYN_CTRL_FRAMERATE_EN_BIT)) {
      AMFramerate fps;
      fps.stream_id = id;
      if (AM_UNLIKELY((svc_ret->ret = g_video_camera->get_framerate(fps))
                      != AM_RESULT_OK)) {
        break;
      }
      if (fps.fps != int32_t(params->fps)) {
        fps.fps = params->fps;
        if (AM_UNLIKELY((svc_ret->ret = g_video_camera->set_framerate(fps))
                        != AM_RESULT_OK)) {
          break;
        }
      }
    }

    if ((TEST_BIT(params->enable_bits, AM_STREAM_DYN_CTRL_I_FRAME_SIZE_EN_BIT))
        || (TEST_BIT(params->enable_bits, AM_STREAM_DYN_CTRL_BITRATE_EN_BIT))) {
      bool setting = false;
      AMBitrate br;
      br.stream_id = id;
      if (AM_UNLIKELY((svc_ret->ret = g_video_camera->get_bitrate(br))
                      != AM_RESULT_OK)) {
        break;
      }
      if ((TEST_BIT(params->enable_bits, AM_STREAM_DYN_CTRL_I_FRAME_SIZE_EN_BIT))
          && (br.i_frame_max_size != int32_t(params->i_frame_max_size))){
        br.i_frame_max_size = params->i_frame_max_size;
        setting = true;
      }
      if ((TEST_BIT(params->enable_bits, AM_STREAM_DYN_CTRL_BITRATE_EN_BIT)) &&
          (br.target_bitrate != int32_t(params->bitrate))) {
        br.target_bitrate = params->bitrate;
        setting = true;
      }
      if (setting && (AM_UNLIKELY((svc_ret->ret = g_video_camera->
          set_bitrate(br)) != AM_RESULT_OK))) {
        break;
      }
    }

    if (TEST_BIT(params->enable_bits, AM_STREAM_DYN_CTRL_ABS_BITRATE_EN_BIT)) {
      bool flag = false;
      if (AM_UNLIKELY((svc_ret->ret = g_video_camera->get_abs_bitrate(id, flag))
                      != AM_RESULT_OK)) {
        break;
      }
      if (flag != bool(params->abs_bitrate)) {
        if (AM_UNLIKELY((svc_ret->ret = g_video_camera->
            set_abs_bitrate(id, params->abs_bitrate))
                        != AM_RESULT_OK)) {
          break;
        }
      }
    }

    if (TEST_BIT(params->enable_bits, AM_STREAM_DYN_CTRL_SIZE_EN_BIT)) {
      AMResolution res;
      if (AM_UNLIKELY((svc_ret->ret = g_video_camera->get_stream_size(id, res))
                      != AM_RESULT_OK)) {
        break;
      }
      params->width = ROUND_UP(params->width, 16);
      params->height = ROUND_UP(params->height, 8);
      if ((res.width != int32_t(params->width)) ||
          (res.height != int32_t(params->height))) {
        res.width = (params->width != 0) ? params->width : res.width;
        res.height = (params->height != 0) ? params->height : res.height;
        if (AM_UNLIKELY((svc_ret->ret = g_video_camera->
            set_stream_size(id, res)) != AM_RESULT_OK)) {
          break;
        }
      }
    }

    if (TEST_BIT(params->enable_bits, AM_STREAM_DYN_CTRL_OFFSET_EN_BIT)) {
      AMOffset offset;
      if (AM_UNLIKELY((svc_ret->ret = g_video_camera->
          get_stream_offset(id, offset)) != AM_RESULT_OK)) {
        break;
      }
      params->offset_x = ROUND_UP(params->offset_x, 2);
      params->offset_y = ROUND_UP(params->offset_y, 2);
      if (offset.x != int32_t(params->offset_x) ||
          (offset.y != int32_t(params->offset_y))) {
        offset.x = params->offset_x;
        offset.y = params->offset_y;
        if (AM_UNLIKELY((svc_ret->ret = g_video_camera->
            set_stream_offset(id, offset)) != AM_RESULT_OK)) {
          break;
        }
      }
    }

    if (TEST_BIT(params->enable_bits, AM_STREAM_DYN_CTRL_FLIP_EN_BIT)) {
      AM_VIDEO_FLIP flip;
      if (AM_UNLIKELY((svc_ret->ret = g_video_camera->get_stream_flip(id, flip))
                      != AM_RESULT_OK)) {
        break;
      }
      if (flip != int32_t(params->flip)) {
        flip = AM_VIDEO_FLIP(params->flip);
        if (AM_UNLIKELY((svc_ret->ret = g_video_camera->
            set_stream_flip(id, flip)) != AM_RESULT_OK)) {
          break;
        }
      }
    }

    if (TEST_BIT(params->enable_bits, AM_STREAM_DYN_CTRL_ROTATE_EN_BIT)) {
      AM_VIDEO_ROTATE rot;
      if (AM_UNLIKELY((svc_ret->ret = g_video_camera->
          get_stream_rotate(id, rot)) != AM_RESULT_OK)) {
        break;
      }
      if (rot != int32_t(params->rotate)) {
        rot = AM_VIDEO_ROTATE(params->rotate);
        if (AM_UNLIKELY((svc_ret->ret = g_video_camera->
            set_stream_rotate(id, rot)) != AM_RESULT_OK)) {
          break;
        }
      }
    }

    if (TEST_BIT(params->enable_bits, AM_STREAM_DYN_CTRL_PROFILE_EN_BIT)) {
      AM_PROFILE profile;
      if (AM_UNLIKELY((svc_ret->ret = g_video_camera->
          get_stream_profile(id, profile)) != AM_RESULT_OK)) {
        break;
      }
      if (profile != int32_t(params->profile)) {
        profile = AM_PROFILE(params->profile);
        if (AM_UNLIKELY((svc_ret->ret = g_video_camera->
            set_stream_profile(id, profile)) != AM_RESULT_OK)) {
          break;
        }
      }
    }

    if (TEST_BIT(params->enable_bits, AM_STREAM_DYN_CTRL_GOP_N_EN_BIT) ||
        TEST_BIT(params->enable_bits, AM_STREAM_DYN_CTRL_GOP_IDR_EN_BIT)) {
      bool setting = false;
      AMGOP info;
      info.stream_id = id;
      if (AM_UNLIKELY((svc_ret->ret = g_video_camera->get_h26x_gop(info))
                      != AM_RESULT_OK)) {
        break;
      }
      if ((TEST_BIT(params->enable_bits, AM_STREAM_DYN_CTRL_GOP_N_EN_BIT)) &&
          (info.N != params->gop_n)) {
        info.N = params->gop_n;
        setting = true;
      }
      if ((TEST_BIT(params->enable_bits, AM_STREAM_DYN_CTRL_GOP_IDR_EN_BIT)) &&
          (info.idr_interval != params->idr_interval)) {
        info.idr_interval = params->idr_interval;
        setting = true;
      }
      if (setting && AM_UNLIKELY((svc_ret->ret = g_video_camera->
          set_h26x_gop(info)) != AM_RESULT_OK)) {
        break;
      }
    }

    if (TEST_BIT(params->enable_bits, AM_STREAM_DYN_CTRL_ENC_QUALITY_EN_BIT)) {
      AMMJpegInfo info;
      info.stream_id = id;
      if (AM_UNLIKELY((svc_ret->ret = g_video_camera->get_mjpeg_info(info))
                      != AM_RESULT_OK)) {
        break;
      }
      if (info.quality != params->quality) {
        info.quality = params->quality;
        if (AM_UNLIKELY((svc_ret->ret = g_video_camera->set_mjpeg_info(info))
                        != AM_RESULT_OK)) {
          break;
        }
      }
    }

    if (TEST_BIT(params->enable_bits, AM_STREAM_DYN_CTRL_SAVE_CFG_EN_BIT)) {
      if (AM_UNLIKELY((svc_ret->ret = g_video_camera->save_stream_config())
                      != AM_RESULT_OK)) {
        ERROR("Failed to save current stream config!");
        break;
      }
    }

  } while (0);
}

void ON_DYN_STREAM_PARAMETERS_GET(void *msg_data,
                                  int msg_data_size,
                                  void *result_addr,
                                  int result_max_size)
{
  INFO("Video Service: ON_DYN_STREAM_PARAMETERS_GET");
  am_service_result_t *svc_ret = (am_service_result_t*)result_addr;
  memset(svc_ret, 0, sizeof(*svc_ret));
  do {
    if (!msg_data) {
      svc_ret->ret = -1;
      break;
    }
    //iav unloaded mode can't manipulate iav
    AM_WORKING_MODE mode;
    if (AM_UNLIKELY((svc_ret->ret = g_video_camera->get_working_mode(mode))
                    != AM_RESULT_OK)) {
      return;
    }
    if (mode == AM_WORKING_MODE_IAV_UNLOADED) {
      return;
    }

    AM_STREAM_ID id = *((AM_STREAM_ID*)msg_data);
    am_stream_parameter_t *params = (am_stream_parameter_t*)svc_ret->data;
    params->stream_id = id;

    AM_SOURCE_BUFFER_ID buf;
    if (AM_UNLIKELY((svc_ret->ret = g_video_camera->
        get_stream_source(id, buf)) != AM_RESULT_OK)) {
      break;
    }
    params->source = buf;

    AM_STREAM_TYPE type;
    if (AM_UNLIKELY((svc_ret->ret = g_video_camera->get_stream_type(id, type))
                    != AM_RESULT_OK)) {
      break;
    }
    params->type = type;

    AMFramerate fps;
    fps.stream_id = id;
    if (AM_UNLIKELY((svc_ret->ret = g_video_camera->get_framerate(fps))
                    != AM_RESULT_OK)) {
      break;
    }
    params->fps = fps.fps;

    AMResolution res;
    if (AM_UNLIKELY((svc_ret->ret = g_video_camera->get_stream_size(id, res))
                    != AM_RESULT_OK)) {
      break;
    }
    params->width = res.width;
    params->height = res.height;

    AMOffset offset;
    if (AM_UNLIKELY((svc_ret->ret = g_video_camera->
        get_stream_offset(id, offset)) != AM_RESULT_OK)) {
      break;
    }
    params->offset_x = offset.x;
    params->offset_y = offset.y;

    AM_VIDEO_FLIP flip;
    if (AM_UNLIKELY((svc_ret->ret = g_video_camera->get_stream_flip(id, flip))
                    != AM_RESULT_OK)) {
      break;
    }
    params->flip = flip;

    AM_VIDEO_ROTATE rot;
    if (AM_UNLIKELY((svc_ret->ret = g_video_camera->
        get_stream_rotate(id, rot)) != AM_RESULT_OK)) {
      break;
    }
    params->rotate = rot;

    if ((type == AM_STREAM_TYPE_H264) || (type == AM_STREAM_TYPE_H265)) {
      AM_PROFILE profile;
      if (AM_UNLIKELY((svc_ret->ret = g_video_camera->
          get_stream_profile(id, profile)) != AM_RESULT_OK)) {
        break;
      }
      params->profile = profile;

      AMBitrate br;
      br.stream_id = id;
      if (AM_UNLIKELY((svc_ret->ret = g_video_camera->get_bitrate(br))
                      != AM_RESULT_OK)) {
        break;
      }
      params->bitrate = br.target_bitrate;
      params->i_frame_max_size = br.i_frame_max_size;

      bool flag = false;
      if (AM_UNLIKELY((svc_ret->ret = g_video_camera->get_abs_bitrate(id, flag))
                      != AM_RESULT_OK)) {
        break;
      }
      params->abs_bitrate = flag;

      AMGOP gop;
      gop.stream_id = id;
      if (AM_UNLIKELY((svc_ret->ret = g_video_camera->get_h26x_gop(gop))
                      != AM_RESULT_OK)) {
        break;
      }
      params->gop_n = gop.N;
      params->idr_interval = gop.idr_interval;
    } else if (type == AM_STREAM_TYPE_MJPEG) {
      AMMJpegInfo jpeg;
      jpeg.stream_id = id;
      if (AM_UNLIKELY((svc_ret->ret = g_video_camera->get_mjpeg_info(jpeg))
                      != AM_RESULT_OK)) {
        break;
      }
      params->quality = jpeg.quality;
    }
  } while (0);
}

void ON_DYN_CPU_CLK_GET(void *msg_data,
                        int msg_data_size,
                        void *result_addr,
                        int result_max_size)
{
  INFO("Video Service: ON_DYN_CPU_CLK_GET");
  am_service_result_t *svc_ret = (am_service_result_t*)result_addr;
  memset(svc_ret, 0, sizeof(*svc_ret));
  do {
    std::map<int32_t, int32_t> cpu_clk;
    int32_t clk_len = -1;
    if (AM_UNLIKELY((svc_ret->ret = g_video_camera->
        get_avail_cpu_clks(cpu_clk)) != AM_RESULT_OK)) {
      break;
    }
    clk_len = cpu_clk.size();
    svc_ret->data[0] = (uint8_t) clk_len;
    for (int i = 0; i < clk_len; i++) {
      svc_ret->data[i + 1] = (uint8_t) (cpu_clk[i] / 24000);
    }
  } while(0);
}

void ON_DYN_CUR_CPU_CLK_GET(void *msg_data,
                            int msg_data_size,
                            void *result_addr,
                            int result_max_size)
{
  INFO("Video Service: ON_DYN_CUR_CPU_CLK_GET");
  am_service_result_t *svc_ret = (am_service_result_t*)result_addr;
  memset(svc_ret, 0, sizeof(*svc_ret));
  do {
    int32_t clk = -1;
    if (AM_UNLIKELY(svc_ret->ret = g_video_camera->
                    get_cur_cpu_clk(clk) != AM_RESULT_OK)) {
      break;
    }
    svc_ret->data[0] = (uint8_t) (clk / 24000);
  } while(0);
}

void ON_DYN_CPU_CLK_SET(void *msg_data,
                        int msg_data_size,
                        void *result_addr,
                        int result_max_size)
{
  INFO("Video Service: ON_DYN_CPU_CLK_SET");
  am_service_result_t *svc_ret = (am_service_result_t*)result_addr;
  memset(svc_ret, 0, sizeof(*svc_ret));
  int32_t *index = (int32_t*) msg_data;
  do {
    if (AM_UNLIKELY((svc_ret->ret = g_video_camera->
        set_cpu_clk(*index)) != AM_RESULT_OK)) {
      break;
    }

  } while(0);
}


void ON_DYN_VIN_GET(void *msg_data,
                    int msg_data_size,
                    void *result_addr,
                    int result_max_size)
{
  INFO("Video Service: ON_DYN_VIN_GET");
  am_service_result_t *svc_ret = (am_service_result_t*)result_addr;
  memset(svc_ret, 0, sizeof(*svc_ret));
  AMVinInfo info = {0};
  do {
    if (!msg_data) {
      svc_ret->ret = AM_RESULT_ERR_DATA_POINTER;
      break;
    }
    //iav unloaded mode can't manipulate iav
    AM_WORKING_MODE mode;
    if (AM_UNLIKELY((svc_ret->ret = g_video_camera->get_working_mode(mode))
                    != AM_RESULT_OK)) {
      return;
    }
    if (mode == AM_WORKING_MODE_IAV_UNLOADED) {
      return;
    }

    am_vin_info_s *params = (am_vin_info_s*)svc_ret->data;
    am_vin_info_s &tem = *params;

    info.vin_id = *((uint32_t*)msg_data);
    if ((svc_ret->ret = g_video_camera->get_vin_status(info)) != AM_RESULT_OK) {
      break;
    }
    tem.vin_id = info.vin_id;
    tem.height = info.height;
    tem.width = info.width;
    tem.fps = info.fps;
    tem.type = info.type;
    tem.bits = info.bits;
    tem.ratio = info.ratio;
    tem.system = info.system;
    tem.hdr_mode = info.hdr_mode;
  } while(0);
}

void ON_DYN_CHANNEL_TO_VSRC(void *msg_data,
                            int msg_data_size,
                            void *result_addr,
                            int result_max_size)
{
  INFO("Video Service: ON_DYN_CHANNEL_TO_VSRC");
  am_service_result_t *svc_ret = (am_service_result_t*)result_addr;
  memset(svc_ret, 0, sizeof(am_service_result_t));

  do {
    if (nullptr == msg_data) {
      svc_ret->ret = AM_RESULT_ERR_DATA_POINTER;
      break;
    }
    //iav unloaded mode can't manipulate iav
    AM_WORKING_MODE mode;
    if (AM_UNLIKELY((svc_ret->ret = g_video_camera->get_working_mode(mode))
                    != AM_RESULT_OK)) {
      break;
    }
    if (mode == AM_WORKING_MODE_IAV_UNLOADED) {
      break;
    }

    uint32_t *val = (uint32_t*)svc_ret->data;

    *val = g_video_camera->get_vsrc_id(*((uint32_t*)msg_data));

    svc_ret->ret = AM_RESULT_OK;

  } while(0);

}

static int32_t send_state_cmd_to_image_service(bool is_start)
{
  int32_t cmd_result = 0;
  am_service_notify_payload payload;
  INFO("Send init command to image service\n");

  memset(&payload, 0, sizeof(am_service_notify_payload));
  SET_BIT(payload.dest_bits, AM_SERVICE_TYPE_IMAGE);//for notify event_svc
  payload.msg_id = AM_IPC_MW_CMD_COMMON_IMAGE_STATE_CONTROL;
  payload.data_size = sizeof(bool);
  bool *data = (bool*)payload.data;
  *data = is_start;

  cmd_result = g_ipc.notify(AM_IPC_SERVICE_NOTIF, &payload,
                            payload.header_size() + payload.data_size);
  if (cmd_result != AM_IPC_CMD_SUCCESS) {
    ERROR("notify image svc init failed");
  }

  return cmd_result;
}

void ON_DYN_VCA_MODE(void *msg_data,
                     int msg_data_size,
                     void *result_addr,
                     int result_max_size)
{
  INFO("Video Service: ON_DYN_VCA_MODE");
  am_service_result_t *svc_ret = (am_service_result_t*)result_addr;
  memset(svc_ret, 0, sizeof(*svc_ret));
  do {
    if (!msg_data) {
      svc_ret->ret = AM_RESULT_ERR_DATA_POINTER;
      break;
    }
    AM_WORKING_MODE mode;
    if (AM_UNLIKELY((svc_ret->ret = g_video_camera->get_working_mode(mode))
                    != AM_RESULT_OK)) {
      break;
    }
    if (mode == AM_WORKING_MODE_NORAML) {
      if (AM_UNLIKELY((svc_ret->ret =
          g_video_camera->goto_vca_mode()) != AM_RESULT_OK)) {
        break;
      }
    }
  } while (0);
}

void ON_DYN_IAV_UNLOADED_MODE(void *msg_data,
                             int msg_data_size,
                             void *result_addr,
                             int result_max_size)
{
  INFO("Video Service: ON_DYN_IAV_UNLOADED_MODE");
  am_service_result_t *svc_ret = (am_service_result_t*)result_addr;
  memset(svc_ret, 0, sizeof(*svc_ret));
  do {
    if (!msg_data) {
      svc_ret->ret = AM_RESULT_ERR_DATA_POINTER;
      break;
    }
    AM_WORKING_MODE mode;
    if (AM_UNLIKELY((svc_ret->ret = g_video_camera->get_working_mode(mode))
                    != AM_RESULT_OK)) {
      break;
    }
    if (mode == AM_WORKING_MODE_NORAML) {
      if (AM_UNLIKELY((svc_ret->ret =
          g_video_camera->goto_iav_unloaded_mode()) != AM_RESULT_OK)) {
        break;
      }
      if (send_state_cmd_to_image_service(false) != AM_IPC_CMD_SUCCESS) {
        svc_ret->ret = AM_RESULT_ERR_MODULE_STATE;
        break;
      }
    }
  } while (0);
}

void ON_DYN_IAV_CURRENT_MODE(void *msg_data,
                             int msg_data_size,
                             void *result_addr,
                             int result_max_size)
{
  INFO("Video Service: ON_DYN_IAV_CURRENT_MODE");
  am_service_result_t *svc_ret = (am_service_result_t*)result_addr;
  memset(svc_ret, 0, sizeof(*svc_ret));
  do {
    if (!msg_data) {
      svc_ret->ret = AM_RESULT_ERR_DATA_POINTER;
      break;
    }
    AM_WORKING_MODE mode;
    if (AM_UNLIKELY((svc_ret->ret = g_video_camera->get_working_mode(mode))
                    != AM_RESULT_OK)) {
      break;
    }
    if (mode == AM_WORKING_MODE_IAV_UNLOADED) {
        if (send_state_cmd_to_image_service(true) != AM_IPC_CMD_SUCCESS) {
          svc_ret->ret = AM_RESULT_ERR_MODULE_STATE;
          break;
        }
    }
    if (AM_UNLIKELY((svc_ret->ret =
        g_video_camera->goto_iav_current_mode()) != AM_RESULT_OK)) {
      break;
    }
  } while (0);
}

void ON_DYN_WORKING_MODE_GET(void *msg_data,
                             int msg_data_size,
                             void *result_addr,
                             int result_max_size)
{
  INFO("Video Service: ON_DYN_WORKING_MODE_GET");
  am_service_result_t *svc_ret = (am_service_result_t*)result_addr;
  memset(svc_ret, 0, sizeof(*svc_ret));
  do {
    if (!msg_data) {
      svc_ret->ret = AM_RESULT_ERR_DATA_POINTER;
      break;
    }
    AM_WORKING_MODE *mode = (AM_WORKING_MODE*)(svc_ret->data);
    if (AM_UNLIKELY((svc_ret->ret = g_video_camera->get_working_mode(*mode))
                    != AM_RESULT_OK)) {
      break;
    }
  } while (0);
}

void ON_VIN_STOP(void *msg_data,
                 int msg_data_size,
                 void *result_addr,
                 int result_max_size)
{
  am_service_result_t *svc_ret = (am_service_result_t*)result_addr;
  memset(svc_ret, 0, sizeof(*svc_ret));
  INFO("Video Service: Vin Stop");
  //only normal mode can do set manipulation
  AM_WORKING_MODE mode;
  if (AM_UNLIKELY((svc_ret->ret = g_video_camera->get_working_mode(mode))
                  != AM_RESULT_OK)) {
    return;
  }
  if (mode != AM_WORKING_MODE_NORAML) {
    return;
  }

  if (AM_UNLIKELY((svc_ret->ret = g_video_camera->stop_vin())
                  != AM_RESULT_OK)) {
    ERROR("Failed to stop vin!");
  }

  return;
}

void ON_DYN_DPTZ_RATIO_SET(void *msg_data,
                           int msg_data_size,
                           void *result_addr,
                           int result_max_size)
{
  am_service_result_t *svc_ret = (am_service_result_t*) result_addr;
  memset(svc_ret, 0, sizeof(am_service_result_t));
  INFO("video service ON_DYN_DPTZ_RATIO_SET");

  do {
    AMIDPTZ *dptz = (AMIDPTZ*)g_video_camera->\
        get_video_plugin(VIDEO_PLUGIN_DPTZ);
    if (!dptz) {
      NOTICE("Video Plugin \"%s\" is not loaded!", VIDEO_PLUGIN_DPTZ);
      svc_ret->ret = AM_RESULT_ERR_PLUGIN_LOAD;
      break;
    }
    if (AM_UNLIKELY(!msg_data)) {
      ERROR("NULL pointer!\n");
      svc_ret->ret = AM_RESULT_ERR_DATA_POINTER;
      break;
    }

    am_dptz_ratio_t *param = (am_dptz_ratio_t*) msg_data;

    AMDPTZRatio ratio;
    AM_SOURCE_BUFFER_ID buf_id = AM_SOURCE_BUFFER_ID(param->buffer_id);
    AM_MULTI_VIN_CHAN_ID chan_id = AM_MULTI_VIN_CHAN_ID(param->channel_id);

    if (TEST_BIT(param->enable_bits, AM_DPTZ_PAN_RATIO_EN_BIT)) {
      ratio.pan.first = true;
      ratio.pan.second = param->pan_ratio;
    }

    if (TEST_BIT(param->enable_bits, AM_DPTZ_TILT_RATIO_EN_BIT)) {
      ratio.tilt.first = true;
      ratio.tilt.second = param->tilt_ratio;
    }

    if (TEST_BIT(param->enable_bits, AM_DPTZ_ZOOM_RATIO_EN_BIT)) {
      ratio.zoom.first = true;
      ratio.zoom.second = param->zoom_ratio;
    }

    if ((svc_ret->ret = dptz->set_ratio(chan_id, buf_id, ratio)) !=
        AM_RESULT_OK) {
      ERROR("set ratio failed!\n");
      break;
    }

    /* if do dptz_I, check whether ldc is enabled, if so, clear ldc,
     * because both of dptz_I and ldc's source are vin*/
    if (buf_id == AM_SOURCE_BUFFER_MAIN) {
      bool state = false;
      if ((g_video_camera->get_ldc_state(chan_id, state) == AM_RESULT_OK) &&
          state) {
        AMIEncodeWarp *warp =
            (AMIEncodeWarp*)g_video_camera->get_video_plugin(VIDEO_PLUGIN_WARP);
        if (!warp) {
          break;
        }
        if (warp->set_ldc_strength(chan_id, 0) != AM_RESULT_OK) {
          break;
        }
        warp->apply(chan_id);
      }
    }
  } while (0);
}

void ON_DYN_DPTZ_RATIO_GET(void *msg_data,
                           int msg_data_size,
                           void *result_addr,
                           int result_max_size)
{
  am_service_result_t *svc_ret = (am_service_result_t*) result_addr;
  memset(svc_ret, 0, sizeof(am_service_result_t));
  am_dptz_ratio_t *dptz_param = (am_dptz_ratio_t*) svc_ret->data;
  AMDPTZRatio ratio;
  INFO("video service ON_DYN_DPTZ_RATIO_GET");

  do {
    AMIDPTZ *dptz = (AMIDPTZ*)g_video_camera->\
        get_video_plugin(VIDEO_PLUGIN_DPTZ);
    if (!dptz) {
      NOTICE("Video Plugin \"%s\" is not loaded!", VIDEO_PLUGIN_DPTZ);
      svc_ret->ret = AM_RESULT_ERR_PLUGIN_LOAD;
      break;
    }
    if (AM_UNLIKELY(!msg_data)) {
      ERROR("NULL pointer!\n");
      svc_ret->ret = AM_RESULT_ERR_DATA_POINTER;
      break;
    }
    am_buffer_id_t *buffer = (am_buffer_id_t*) msg_data;
    if ((svc_ret->ret = dptz->get_ratio((AM_MULTI_VIN_CHAN_ID)buffer->channel_id,
                                        (AM_SOURCE_BUFFER_ID)buffer->buffer_id,
                                        ratio)) != AM_RESULT_OK) {
      ERROR("get ratio failed!\n");
      break;
    }

    dptz_param->channel_id = buffer->channel_id;
    dptz_param->buffer_id = buffer->buffer_id;
    dptz_param->pan_ratio = ratio.pan.second;
    dptz_param->tilt_ratio = ratio.tilt.second;
    dptz_param->zoom_ratio = ratio.zoom.second;
  } while (0);
}

void ON_DYN_DPTZ_SIZE_SET(void *msg_data,
                          int msg_data_size,
                          void *result_addr,
                          int result_max_size)
{
  am_service_result_t *svc_ret = (am_service_result_t*) result_addr;
  memset(svc_ret, 0, sizeof(am_service_result_t));
  INFO("video service ON_DYN_DPTZ_SIZE_SET");

  do {
    AMIDPTZ *dptz = (AMIDPTZ*)g_video_camera->\
        get_video_plugin(VIDEO_PLUGIN_DPTZ);
    if (!dptz) {
      NOTICE("Video Plugin \"%s\" is not loaded!", VIDEO_PLUGIN_DPTZ);
      svc_ret->ret = AM_RESULT_ERR_PLUGIN_LOAD;
      break;
    }
    if (AM_UNLIKELY(!msg_data)) {
      ERROR("NULL pointer!\n");
      svc_ret->ret = AM_RESULT_ERR_DATA_POINTER;
      break;
    }

    am_dptz_size_t *param = (am_dptz_size_t*) msg_data;

    AMDPTZSize rect;
    AM_SOURCE_BUFFER_ID buf_id = AM_SOURCE_BUFFER_ID(param->buffer_id);
    AM_MULTI_VIN_CHAN_ID chan_id = AM_MULTI_VIN_CHAN_ID(param->channel_id);

    if (TEST_BIT(param->enable_bits, AM_DPTZ_SIZE_X_EN_BIT)) {
      rect.x.first = true;
      rect.x.second = param->x;
    }

    if (TEST_BIT(param->enable_bits, AM_DPTZ_SIZE_Y_EN_BIT)) {
      rect.y.first = true;
      rect.y.second = param->y;
    }

    if (TEST_BIT(param->enable_bits, AM_DPTZ_SIZE_W_EN_BIT)) {
      rect.w.first = true;
      rect.w.second = param->w;
    }

    if (TEST_BIT(param->enable_bits, AM_DPTZ_SIZE_H_EN_BIT)) {
      rect.h.first = true;
      rect.h.second = param->h;
    }

    if ((svc_ret->ret = dptz->set_size(chan_id, buf_id, rect)) != AM_RESULT_OK) {
      ERROR("set ratio failed!\n");
      break;
    }

    /* if do dptz_I, check whether ldc is enabled, if so, clear ldc,
     * because both of dptz_I and ldc's source are vin*/
    if (buf_id == AM_SOURCE_BUFFER_MAIN) {
      bool state = false;
      if ((g_video_camera->get_ldc_state(chan_id, state) == AM_RESULT_OK) &&
          state) {
        AMIEncodeWarp *warp =
            (AMIEncodeWarp*)g_video_camera->get_video_plugin(VIDEO_PLUGIN_WARP);
        if (!warp) {
          break;
        }
        if (warp->set_ldc_strength(chan_id, 0) != AM_RESULT_OK) {
          break;
        }
        warp->apply(chan_id);
      }
    }
  } while (0);
}

void ON_DYN_DPTZ_SIZE_GET(void *msg_data,
                          int msg_data_size,
                          void *result_addr,
                          int result_max_size)
{
  am_service_result_t *svc_ret = (am_service_result_t*) result_addr;
  memset(svc_ret, 0, sizeof(am_service_result_t));
  am_dptz_size_t *dptz_param = (am_dptz_size_t*) svc_ret->data;
  AMDPTZSize size;
  INFO("video service ON_DYN_DPTZ_SIZE_GET");

  do {
    AMIDPTZ *dptz = (AMIDPTZ*)g_video_camera->\
        get_video_plugin(VIDEO_PLUGIN_DPTZ);
    if (!dptz) {
      NOTICE("Video Plugin \"%s\" is not loaded!", VIDEO_PLUGIN_DPTZ);
      svc_ret->ret = AM_RESULT_ERR_PLUGIN_LOAD;
      break;
    }
    if (AM_UNLIKELY(!msg_data)) {
      ERROR("NULL pointer!\n");
      svc_ret->ret = AM_RESULT_ERR_DATA_POINTER;
      break;
    }
    am_buffer_id_t *buffer = (am_buffer_id_t*) msg_data;
    if ((svc_ret->ret = dptz->get_size((AM_MULTI_VIN_CHAN_ID)buffer->channel_id,
                                       (AM_SOURCE_BUFFER_ID)buffer->buffer_id,
                                       size)) != AM_RESULT_OK) {
      ERROR("get size failed!\n");
      break;
    }

    dptz_param->channel_id = buffer->channel_id;
    dptz_param->buffer_id = buffer->buffer_id;
    dptz_param->w = size.w.second;
    dptz_param->h = size.h.second;
    dptz_param->x = size.x.second;
    dptz_param->y = size.y.second;
  } while (0);

}

void ON_DYN_WARP_SET(void *msg_data,
                     int msg_data_size,
                     void *result_addr,
                     int result_max_size)
{
  am_service_result_t *svc_ret = (am_service_result_t*)result_addr;
  memset(svc_ret, 0, sizeof(am_service_result_t));
  INFO("video service ON_WARP_SET!\n");

  do {
    bool need_apply = true;
    am_warp_t *warp_param = nullptr;
    AMIEncodeWarp *warp =
        (AMIEncodeWarp*)g_video_camera->get_video_plugin(VIDEO_PLUGIN_WARP);
    AM_WARP_MODE mode = AM_WARP_MODE_NO_TRANSFORM;
    float ldc_strength = 0.0;
    float pano_hfov_degree = 0.0;
    float ver_pano_fov_degree = 0.0;
    int max_radius = 0;
    int warp_region_yaw = 0;
    int warp_region_pitch = 0;
    int warp_region_rotate = 0;
    float pan_angle = 0.0;
    float tilt_angle = 0.0;
    AMPointF sub_roi_offset;
    AMFrac cur_hor, hor, cur_ver, ver, cur_zoom, zoom;
    if (AM_UNLIKELY(!msg_data)) {
      ERROR("NULL pointer!\n");
      svc_ret->ret = AM_RESULT_ERR_DATA_POINTER;
      break;
    }

    if (AM_UNLIKELY(!warp)) {
      WARN("Video Plugin %s is not loaded!", VIDEO_PLUGIN_WARP_SO);
      svc_ret->ret = AM_RESULT_ERR_PLUGIN_LOAD;
      break;
    }

    warp_param = (am_warp_t*)msg_data;
    if (TEST_BIT(warp_param->enable_bits, AM_WARP_LDC_WARP_MODE_EN_BIT)) {
      if (AM_UNLIKELY((svc_ret->ret = warp->get_ldc_mode(
          warp_param->region_id, mode)) != AM_RESULT_OK)) {
        break;
      }
      if ((warp_param->warp_mode != mode) &&
          AM_UNLIKELY((svc_ret->ret = warp->set_ldc_mode(
          warp_param->region_id, AM_WARP_MODE(warp_param->warp_mode)))
                      != AM_RESULT_OK)) {
        break;
      }
    }

    if (TEST_BIT(warp_param->enable_bits, AM_WARP_MAX_RADIUS_EN_BIT)) {
      if (AM_UNLIKELY((svc_ret->ret = warp->get_max_radius(
          warp_param->region_id, max_radius)) != AM_RESULT_OK)) {
        break;
      }
      if ((warp_param->max_radius != max_radius) &&
          AM_UNLIKELY((svc_ret->ret = warp->set_max_radius(
            warp_param->region_id, warp_param->max_radius)) != AM_RESULT_OK)) {
        break;
      }
    }

    if (TEST_BIT(warp_param->enable_bits, AM_WARP_LDC_STRENGTH_EN_BIT)) {
      if (AM_UNLIKELY((svc_ret->ret = warp->get_ldc_strength(
          warp_param->region_id, ldc_strength)) != AM_RESULT_OK)) {
        break;
      }
      if ((warp_param->ldc_strength != ldc_strength) &&
          AM_UNLIKELY((svc_ret->ret = warp->set_ldc_strength(
          warp_param->region_id, warp_param->ldc_strength)) != AM_RESULT_OK)) {
        break;
      }
    }

    if (TEST_BIT(warp_param->enable_bits, AM_WARP_PANO_HFOV_DEGREE_EN_BIT)) {
      if (AM_UNLIKELY((svc_ret->ret = warp->get_pano_hfov_degree(
          warp_param->region_id, pano_hfov_degree)) != AM_RESULT_OK)) {
        break;
      }
      if ((warp_param->pano_hfov_degree != pano_hfov_degree) &&
          AM_UNLIKELY((svc_ret->ret = warp->set_pano_hfov_degree(
          warp_param->region_id, warp_param->pano_hfov_degree)) != AM_RESULT_OK)) {
        break;
      }
    }
    if (TEST_BIT(warp_param->enable_bits, AM_WARP_VER_PANO_FOV_DEGREE_EN_BIT)) {
      if (AM_UNLIKELY((svc_ret->ret = warp->get_ver_pano_fov_degree(
          warp_param->region_id, ver_pano_fov_degree)) != AM_RESULT_OK)) {
        break;
      }
      if ((warp_param->ver_pano_fov_degree != ver_pano_fov_degree) &&
          AM_UNLIKELY((svc_ret->ret = warp->set_ver_pano_fov_degree(
              warp_param->region_id, warp_param->ver_pano_fov_degree))
                      != AM_RESULT_OK)) {
        ERROR("Failed to set vertical panorama fov degree.");
        break;
      }
    }

    if (TEST_BIT(warp_param->enable_bits, AM_WARP_REGION_YAW_PITCH_EN_BIT)) {
      if (AM_UNLIKELY((svc_ret->ret = warp->get_warp_region_yaw_pitch(
          warp_param->region_id, warp_region_yaw, warp_region_pitch)) != AM_RESULT_OK)) {
        break;
      }
      if (((warp_param->warp_region_yaw != warp_region_yaw) ||
          (warp_param->warp_region_pitch != warp_region_pitch)) &&
          AM_UNLIKELY((svc_ret->ret = warp->set_warp_region_yaw_pitch(
          warp_param->region_id, warp_param->warp_region_yaw,
          warp_param->warp_region_pitch)) != AM_RESULT_OK)) {
        break;
      }
    }

    if (TEST_BIT(warp_param->enable_bits, AM_WARP_REGION_ROTATE_EN_BIT)) {
      if (AM_UNLIKELY((svc_ret->ret = warp->get_warp_rotate_degree(
          warp_param->region_id, warp_region_rotate)) != AM_RESULT_OK)) {
        break;
      }
      if ((warp_param->warp_region_rotate != warp_region_rotate) &&
          AM_UNLIKELY((svc_ret->ret = warp->set_warp_rotate_degree(
          warp_param->region_id, warp_param->warp_region_rotate))
                      != AM_RESULT_OK)) {
        break;
      }
    }

    if (TEST_BIT(warp_param->enable_bits, AM_WARP_ZOOM_EN_BIT)) {
      if (AM_UNLIKELY((svc_ret->ret = warp->get_warp_zoom(
                       warp_param->region_id, cur_zoom)) != AM_RESULT_OK)) {
        break;
      }
      zoom.num = warp_param->warp_zoom >> 16;
      zoom.denom = warp_param->warp_zoom & 0xffff;
      if (zoom.num <= 0 || zoom.denom <= 0) {
        zoom.num = cur_zoom.num;
        zoom.denom = cur_zoom.denom;
      }
      if (AM_UNLIKELY((svc_ret->ret = warp->set_warp_zoom(
                       warp_param->region_id, zoom)) != AM_RESULT_OK)) {
        break;
      }
    }

    if (TEST_BIT(warp_param->enable_bits, AM_WARP_HOR_VER_ZOOM_EN_BIT)) {
      if (AM_UNLIKELY((svc_ret->ret = warp->get_hor_ver_zoom(
          warp_param->region_id, cur_hor, cur_ver)) != AM_RESULT_OK)) {
        break;
      }
      hor.num = warp_param->hor_zoom >> 16;
      hor.denom = warp_param->hor_zoom & 0xffff;
      if (hor.num<= 0 || hor.denom <= 0) {
        hor.num = cur_hor.num;
        hor.denom = cur_hor.denom;
      }
      ver.num = warp_param->ver_zoom >> 16;
      ver.denom = warp_param->ver_zoom & 0xffff;
      if (ver.num <= 0 || ver.denom <= 0) {
        ver.num = cur_ver.num;
        ver.denom = cur_ver.denom;
      }
      if (AM_UNLIKELY((svc_ret->ret = warp->set_hor_ver_zoom(
          warp_param->region_id, hor, ver)) != AM_RESULT_OK)) {
        break;
      }
    }

    /* set Warp DPTZ */
    if(TEST_BIT(warp_param->enable_bits, AM_WARP_REGION_DPTZ_EN_BIT)) {
      AMWarpRegionDPTZ *warp_dptz =
          (AMWarpRegionDPTZ*)warp_param->warp_region_dptz;
      if (AM_UNLIKELY((svc_ret->ret = warp->set_warp_region_dptz(
                       warp_param->buffer_id,
                       MAX_WARP_AREAS,
                       warp_dptz)) !=  AM_RESULT_OK)){
        ERROR("set_warp_region_dptz error!");
        break;
      } else {
        need_apply = false; //no need to apply
      }
    }

    if ((TEST_BIT(warp_param->enable_bits, AM_WARP_PAN_ANGLE_EN_BIT) ||
        TEST_BIT(warp_param->enable_bits, AM_WARP_TILT_ANGLE_EN_BIT)) &&
        TEST_BIT(warp_param->enable_bits, AM_WARP_SUB_ROI_OFFSET_EN_BIT)) {
      ERROR("Should not set pan tile and roi offset at same time.");
      svc_ret->ret = AM_RESULT_ERR_INVALID;
      break;
    }
    if (TEST_BIT(warp_param->enable_bits, AM_WARP_PAN_ANGLE_EN_BIT)) {
      if (AM_UNLIKELY((svc_ret->ret = warp->get_pan_angle(
                       warp_param->region_id, pan_angle)) != AM_RESULT_OK)) {
        break;
      }
      if (AM_UNLIKELY((svc_ret->ret = warp->set_pan_angle(
            warp_param->region_id, warp_param->pan_angle)) != AM_RESULT_OK)) {
        break;
      }
    }

    if (TEST_BIT(warp_param->enable_bits, AM_WARP_TILT_ANGLE_EN_BIT)) {
      if (AM_UNLIKELY((svc_ret->ret = warp->get_tilt_angle(
                       warp_param->region_id, tilt_angle)) != AM_RESULT_OK)) {
        break;
      }
      if (AM_UNLIKELY((svc_ret->ret = warp->set_tilt_angle(
            warp_param->region_id, warp_param->tilt_angle)) != AM_RESULT_OK)) {
        break;
      }
    }

    if (TEST_BIT(warp_param->enable_bits, AM_WARP_SUB_ROI_OFFSET_EN_BIT)) {
      if (AM_UNLIKELY((svc_ret->ret = warp->get_sub_roi_offset(
                       warp_param->region_id, sub_roi_offset)) != AM_RESULT_OK)) {
        break;
      }
      AMPointF roi_offset;
      roi_offset.x = warp_param->sub_roi_offset_x;
      roi_offset.y = warp_param->sub_roi_offset_y;
      if (AM_UNLIKELY((svc_ret->ret = warp->set_sub_roi_offset(
            warp_param->region_id, roi_offset)) != AM_RESULT_OK)) {
        break;
      }
    }

    if (need_apply && (AM_RESULT_OK != (svc_ret->ret =
                                        warp->apply(warp_param->region_id)))) {
      ERROR("Failed to apply warp parameters! Reset to last one!");
      if (TEST_BIT(warp_param->enable_bits, AM_WARP_LDC_WARP_MODE_EN_BIT)) {
        if (AM_UNLIKELY((warp->set_ldc_mode(
            warp_param->region_id, mode)) != AM_RESULT_OK)) {
          break;
        }
      }
      if (TEST_BIT(warp_param->enable_bits, AM_WARP_MAX_RADIUS_EN_BIT)) {
        if (AM_UNLIKELY((svc_ret->ret = warp->set_max_radius(
            warp_param->region_id, max_radius)) != AM_RESULT_OK)) {
          break;
        }
      }
      if (TEST_BIT(warp_param->enable_bits, AM_WARP_LDC_STRENGTH_EN_BIT)) {
        if (AM_UNLIKELY((warp->set_ldc_strength(
            warp_param->region_id, ldc_strength)) != AM_RESULT_OK)) {
          break;
        }
      }
      if (TEST_BIT(warp_param->enable_bits, AM_WARP_PANO_HFOV_DEGREE_EN_BIT)) {
        if (AM_UNLIKELY((warp->set_pano_hfov_degree(
            warp_param->region_id, pano_hfov_degree)) != AM_RESULT_OK)) {
          break;
        }
      }
      if (TEST_BIT(warp_param->enable_bits, AM_WARP_VER_PANO_FOV_DEGREE_EN_BIT)) {
        if (AM_UNLIKELY((warp->set_ver_pano_fov_degree(
            warp_param->region_id, ver_pano_fov_degree)) != AM_RESULT_OK)) {
          break;
        }
      }
      if (TEST_BIT(warp_param->enable_bits, AM_WARP_REGION_YAW_PITCH_EN_BIT)) {
        if (AM_UNLIKELY((warp->set_warp_region_yaw_pitch(
            warp_param->region_id, warp_region_yaw, warp_region_yaw))
                        != AM_RESULT_OK)) {
          break;
        }
      }
      if (TEST_BIT(warp_param->enable_bits, AM_WARP_REGION_ROTATE_EN_BIT)) {
        if (AM_UNLIKELY((warp->set_warp_rotate_degree(
            warp_param->region_id, warp_region_rotate))!= AM_RESULT_OK)) {
          break;
        }
      }
      if (TEST_BIT(warp_param->enable_bits, AM_WARP_HOR_VER_ZOOM_EN_BIT)) {
        if (AM_UNLIKELY((svc_ret->ret = warp->set_hor_ver_zoom(
            warp_param->region_id, cur_hor, cur_ver)) != AM_RESULT_OK)) {
          break;
        }
      }
      if (TEST_BIT(warp_param->enable_bits, AM_WARP_PAN_ANGLE_EN_BIT)) {
        if (AM_UNLIKELY((svc_ret->ret = warp->set_pan_angle(
              warp_param->region_id, pan_angle)) != AM_RESULT_OK)) {
          break;
        }
      }
      if (TEST_BIT(warp_param->enable_bits, AM_WARP_TILT_ANGLE_EN_BIT)) {
        if (AM_UNLIKELY((svc_ret->ret = warp->set_tilt_angle(
              warp_param->region_id, tilt_angle)) != AM_RESULT_OK)) {
          break;
        }
      }
      if (TEST_BIT(warp_param->enable_bits, AM_WARP_SUB_ROI_OFFSET_EN_BIT)) {
        if (AM_UNLIKELY((svc_ret->ret = warp->set_sub_roi_offset(
              warp_param->region_id, sub_roi_offset)) != AM_RESULT_OK)) {
          break;
        }
      }
      warp->apply(warp_param->region_id);
      break;
    }
    if (TEST_BIT(warp_param->enable_bits, AM_WARP_SAVE_CFG_EN_BIT)) {
      if (AM_UNLIKELY((warp->save_config() != AM_RESULT_OK))) {
        ERROR("Failed to save current warp config!");
        break;
      }
    }
  } while (0);
}

void ON_DYN_WARP_GET(void *msg_data,
                     int msg_data_size,
                     void *result_addr,
                     int result_max_size)
{
  INFO("video service ON_WARP_GET");
  am_service_result_t *svc_ret = (am_service_result_t*) result_addr;
  memset(svc_ret, 0, sizeof(am_service_result_t));
  am_warp_t *warp_param = (am_warp_t*) svc_ret->data;

  do {
    AMIEncodeWarp *warp =
        (AMIEncodeWarp*)g_video_camera->get_video_plugin(VIDEO_PLUGIN_WARP);

    if (AM_UNLIKELY(!msg_data)) {
      ERROR("NULL pointer!\n");
      svc_ret->ret = AM_RESULT_ERR_DATA_POINTER;
      break;
    }

    if (AM_UNLIKELY(!warp)) {
      WARN("Video Plugin %s is not loaded!", VIDEO_PLUGIN_WARP_SO);
      svc_ret->ret = AM_RESULT_ERR_PLUGIN_LOAD;
      break;
    }

    int region_id = *((int*)msg_data);
    AM_WARP_MODE mode;
    if (AM_UNLIKELY((svc_ret->ret = warp->get_ldc_mode(
        region_id, mode)) != AM_RESULT_OK)) {
      ERROR("Failed to get LDC warp mode!");
      break;
    }
    warp_param->warp_mode = mode;

    if (AM_UNLIKELY((svc_ret->ret = warp->get_max_radius(region_id,
                     warp_param->max_radius)) != AM_RESULT_OK)) {
      ERROR("Failed to get warp max radius!");
      break;
    }

    if (AM_UNLIKELY((svc_ret->ret = warp->get_ldc_strength(region_id,
        warp_param->ldc_strength)) != AM_RESULT_OK)) {
      ERROR("Failed to get warp LDC strength!");
      break;
    }

    if (AM_UNLIKELY((svc_ret->ret = warp->get_pano_hfov_degree(region_id,
        warp_param->pano_hfov_degree)) != AM_RESULT_OK)) {
      ERROR("Failed to get warp pano hfov degree!");
      break;
    }

    if (AM_UNLIKELY((svc_ret->ret = warp->get_ver_pano_fov_degree(region_id,
                       warp_param->ver_pano_fov_degree)) != AM_RESULT_OK)) {
      ERROR("Failed to get warp vertical pano fov degree!");
      break;
    }

    if (AM_UNLIKELY((svc_ret->ret = warp->get_warp_region_yaw_pitch(region_id,
        warp_param->warp_region_yaw, warp_param->warp_region_pitch))
                    != AM_RESULT_OK)) {
      ERROR("Failed to get warp region yaw/pitch!");
      break;
    }

    if (AM_UNLIKELY((svc_ret->ret = warp->get_warp_rotate_degree(region_id,
        warp_param->warp_region_rotate)) != AM_RESULT_OK)) {
      ERROR("Failed to get warp region rotate!");
      break;
    }

    if (AM_UNLIKELY((svc_ret->ret = warp->get_pan_angle(region_id,
                                            warp_param->pan_angle))
                    != AM_RESULT_OK)) {
      ERROR("Failed to get warp pan angle!");
      break;
    }
    if (AM_UNLIKELY((svc_ret->ret = warp->get_tilt_angle(region_id,
                                                         warp_param->tilt_angle))
                    != AM_RESULT_OK)) {
      ERROR("Failed to get warp tilt angle!");
      break;
    }
    AMPointF roi;
    if (AM_UNLIKELY((svc_ret->ret = warp->get_sub_roi_offset(region_id, roi))
                    != AM_RESULT_OK)) {
      ERROR("Failed to get warp sub region roi offset!");
      break;
    } else {
      warp_param->sub_roi_offset_x = roi.x;
      warp_param->sub_roi_offset_y = roi.y;
    }
    AMFrac hor, ver;
    if (AM_UNLIKELY(warp->get_hor_ver_zoom(
                     region_id, hor, ver) != AM_RESULT_OK)) {
      WARN("Failed to get warp horizontal and vertical zoom, Maybe they are not "
          "supported in this platform, set them into 0!");
      warp_param->hor_zoom = 0;
      warp_param->ver_zoom = 0;
    } else {
      warp_param->hor_zoom = (hor.num << 16) | (hor.denom & 0xffff);
      warp_param->ver_zoom = (ver.num << 16) | (ver.denom & 0xffff);
    }
  } while (0);
}

void ON_DYN_LBR_SET(void *msg_data,
                    int msg_data_size,
                    void *result_addr,
                    int result_max_size)
{
  INFO("video service ON_ENCODE_LBR_SET");
  am_service_result_t *svc_ret = (am_service_result_t*)result_addr;
  memset(svc_ret, 0, sizeof(*svc_ret));
  svc_ret->ret = AM_RESULT_OK;

  do {
    am_encode_lbr_ctrl_t *lbr_param = (am_encode_lbr_ctrl_t*)msg_data;
    AMILBRControl *lbr =
        (AMILBRControl*)g_video_camera->get_video_plugin(VIDEO_PLUGIN_LBR);

    if (AM_UNLIKELY(!lbr_param)) {
      ERROR("Invalid data pointer!");
      svc_ret->ret = AM_RESULT_ERR_DATA_POINTER;
      break;
    }

    if (AM_UNLIKELY(!lbr)) {
      WARN("Video Plugin %s is not loaded!", VIDEO_PLUGIN_LBR_SO);
      svc_ret->ret = AM_RESULT_ERR_PLUGIN_LOAD;
      break;
    }

    if (TEST_BIT(lbr_param->enable_bits,
                 AM_ENCODE_LBR_ENABLE_LBR_EN_BIT)) {
      lbr->set_enable(lbr_param->stream_id, lbr_param->enable_lbr);
    }
    if (TEST_BIT(lbr_param->enable_bits,
                 AM_ENCODE_LBR_AUTO_BITRATE_CEILING_EN_BIT)) {
      uint32_t ceiling = 0;
      bool is_auto = false;
      lbr->get_bitrate_ceiling(lbr_param->stream_id, ceiling, is_auto);

      if (is_auto != lbr_param->auto_bitrate_ceiling) {
        if (TEST_BIT(lbr_param->enable_bits,
                     AM_ENCODE_LBR_BITRATE_CEILING_EN_BIT)) {
          lbr->set_bitrate_ceiling(lbr_param->stream_id,
                                   lbr_param->bitrate_ceiling,
                                   lbr_param->auto_bitrate_ceiling);
        } else {
          lbr->set_bitrate_ceiling(lbr_param->stream_id,
                                   ceiling,
                                   lbr_param->auto_bitrate_ceiling);
        }
      }
    }
    if (TEST_BIT(lbr_param->enable_bits,
                 AM_ENCODE_LBR_BITRATE_CEILING_EN_BIT)) {
      uint32_t ceiling = 0;
      bool is_auto = false;
      lbr->get_bitrate_ceiling(lbr_param->stream_id, ceiling, is_auto);
      lbr->set_bitrate_ceiling(lbr_param->stream_id,
                               lbr_param->bitrate_ceiling,
                               is_auto);
    }
    if (TEST_BIT(lbr_param->enable_bits,
                 AM_ENCODE_LBR_DROP_FRAME_EN_BIT)) {
      lbr->set_drop_frame_enable(lbr_param->stream_id, lbr_param->drop_frame);
    }
    if (TEST_BIT(lbr_param->enable_bits,
                 AM_ENCODE_LBR_SAVE_CURRENT_CONFIG_EN_BIT)) {
      lbr->save_current_config();
    }
  } while(0);
}

void ON_DYN_LBR_GET(void *msg_data,
                    int msg_data_size,
                    void *result_addr,
                    int result_max_size)
{
  INFO("video service ON_ENCODE_LBR_GET");
  am_service_result_t *svc_ret = (am_service_result_t*)result_addr;
  memset(svc_ret, 0, sizeof(*svc_ret));

  svc_ret->ret = AM_RESULT_OK;

  do {
    am_encode_lbr_ctrl_t *lbr_param = (am_encode_lbr_ctrl_t*)svc_ret->data;
    uint32_t stream_id = *((uint32_t*)msg_data);
    AMILBRControl *lbr =
        (AMILBRControl*)g_video_camera->get_video_plugin(VIDEO_PLUGIN_LBR);

    if (AM_UNLIKELY(!lbr)) {
      WARN("Video Plugin %s is not loaded!", VIDEO_PLUGIN_LBR_SO);
      svc_ret->ret = AM_RESULT_ERR_PLUGIN_LOAD;
      break;
    }
    lbr_param->stream_id = stream_id;
    lbr_param->enable_lbr = lbr->get_enable(stream_id);
    lbr_param->drop_frame = lbr->get_drop_frame_enable(stream_id);
    lbr->get_bitrate_ceiling(stream_id, lbr_param->bitrate_ceiling,
                             lbr_param->auto_bitrate_ceiling);
  } while(0);
}

void ON_VIDEO_ENCODE_START(void *msg_data,
                           int msg_data_size,
                           void *result_addr,
                           int result_max_size)
{
  am_service_result_t *svc_ret = (am_service_result_t*)result_addr;

  memset(svc_ret, 0, sizeof(*svc_ret));
  INFO("Video Service: Video Encode Start");

  if (AM_UNLIKELY((svc_ret->ret = g_video_camera->start()) != AM_RESULT_OK)) {
    ERROR("Video Service: Failed to start encoding!");
  }
}

void ON_VIDEO_ENCODE_STOP(void *msg_data,
                          int msg_data_size,
                          void *result_addr,
                          int result_max_size)
{
  am_service_result_t *svc_ret = (am_service_result_t*)result_addr;

  memset(svc_ret, 0, sizeof(*svc_ret));
  INFO("Video Service: Video Encode Stop");

  if (AM_UNLIKELY((svc_ret->ret = g_video_camera->stop()) != AM_RESULT_OK)) {
    ERROR("Video Service: Failed to stop encoding!");
  }
}

void ON_COMMON_GET_EVENT(void *msg_data,
                         int msg_data_size,
                         void *result_addr,
                         int result_max_size)
{

}

void ON_DYN_VIDEO_OVERLAY_GET_MAX_NUM(void *msg_data,
                                  int msg_data_size,
                                  void *result_addr,
                                  int result_max_size)
{
  PRINTF("video service ON_DYN_VIDEO_OVERLAY_GET_MAX_NUM\n");
  am_service_result_t *svc_ret = (am_service_result_t*) result_addr;
  memset(svc_ret, 0, sizeof(am_service_result_t));

  AMIEncodeOverlay *ol =
      (AMIEncodeOverlay*)g_video_camera->get_video_plugin(VIDEO_PLUGIN_OVERLAY);
  if (ol) {
    AMOverlayUserDefLimitVal user_val;
    am_overlay_limit_val_t *limit = (am_overlay_limit_val_t*)svc_ret->data;

    ol->get_user_defined_limit_value(user_val);
    limit->platform_stream_num_max = g_video_camera->
        get_encode_stream_max_num();
    limit->platform_overlay_area_num_max = ol->get_area_max_num();
    limit->user_def_stream_num_max = user_val.s_num_max.second;
    limit->user_def_overlay_area_num_max = user_val.a_num_max.second;
  } else {
    svc_ret->ret = AM_RESULT_ERR_PLUGIN_LOAD;
  }
}

void ON_DYN_VIDEO_OVERLAY_DESTROY(void *msg_data,
                              int msg_data_size,
                              void *result_addr,
                              int result_max_size)
{
  PRINTF("video service ON_DYN_VIDEO_OVERLAY_DESTROY\n");
  am_service_result_t *svc_ret = (am_service_result_t*) result_addr;
  memset(svc_ret, 0, sizeof(am_service_result_t));

  AMIEncodeOverlay *ol =
      (AMIEncodeOverlay*)g_video_camera->get_video_plugin(VIDEO_PLUGIN_OVERLAY);
  if (ol) {
    svc_ret->ret = ol->destroy_overlay();
  } else {
    NOTICE("Video Plugin \"%s\" is not loaded!", VIDEO_PLUGIN_OVERLAY);
    svc_ret->ret = AM_RESULT_ERR_PLUGIN_LOAD;
  }
}

void ON_DYN_VIDEO_OVERLAY_SAVE(void *msg_data,
                           int msg_data_size,
                           void *result_addr,
                           int result_max_size)
{
  PRINTF("video service ON_DYN_VIDEO_OVERLAY_SAVE\n");
  am_service_result_t *svc_ret = (am_service_result_t*) result_addr;
  memset(svc_ret, 0, sizeof(am_service_result_t));

  AMIEncodeOverlay *ol =
      (AMIEncodeOverlay*)g_video_camera->get_video_plugin(VIDEO_PLUGIN_OVERLAY);
  if (ol) {
    svc_ret->ret = ol->save_param_to_config();
  } else {
    NOTICE("Video Plugin \"%s\" is not loaded!", VIDEO_PLUGIN_OVERLAY);
    svc_ret->ret = AM_RESULT_ERR_PLUGIN_LOAD;
  }
}

void ON_DYN_VIDEO_OVERLAY_INIT(void *msg_data,
                           int msg_data_size,
                           void *result_addr,
                           int result_max_size)
{
  PRINTF("video service ON_DYN_VIDEO_OVERLAY_INIT!\n");
  am_service_result_t *svc_ret = (am_service_result_t*) result_addr;
  memset(svc_ret, 0, sizeof(am_service_result_t));
  do {
    AMIEncodeOverlay *ol = (AMIEncodeOverlay*)g_video_camera->\
              get_video_plugin(VIDEO_PLUGIN_OVERLAY);
    if (!ol) {
      NOTICE("Video Plugin \"%s\" is not loaded!", VIDEO_PLUGIN_OVERLAY);
      svc_ret->ret = AM_RESULT_ERR_PLUGIN_LOAD;
      break;
    }

    if (!msg_data) {
      ERROR("NULL pointer!\n");
      svc_ret->ret = AM_RESULT_ERR_DATA_POINTER;
      break;
    }
    int32_t *area_id = (int32_t *)svc_ret->data;
    *area_id = -1;
    am_overlay_area_t *param = (am_overlay_area_t*) msg_data;

    if (TEST_BIT(param->enable_bits, AM_OVERLAY_INIT_EN_BIT)) {
      AMOverlayAreaAttr attr;
      attr.enable = 0;
      AM_STREAM_ID stream_id = AM_STREAM_ID(param->stream_id);
      AMRect &rect = attr.rect;
      if (TEST_BIT(param->enable_bits, AM_OVERLAY_RECT_EN_BIT)) {
        rect.size.width = param->width;
        rect.size.height = param->height;
        rect.offset.x = param->offset_x;
        rect.offset.y = param->offset_y;
      }
      if (TEST_BIT(param->enable_bits, AM_OVERLAY_ROTATE_EN_BIT)) {
        attr.rotate = param->rotate;
      }
      if (TEST_BIT(param->enable_bits, AM_OVERLAY_BUF_NUM_EN_BIT)) {
        attr.buf_num = param->buf_num;
      }
      if (TEST_BIT(param->enable_bits, AM_OVERLAY_BG_COLOR_EN_BIT)) {
        AMOverlayCLUT &clut = attr.bg_color;
        clut.v = uint8_t((param->bg_color >> 24) & 0xff);
        clut.u = uint8_t((param->bg_color >> 16) & 0xff);
        clut.y = uint8_t((param->bg_color >> 8) & 0xff);
        clut.a = uint8_t(param->bg_color & 0xff);
      }

      *area_id = ol->init_area(stream_id, attr);
      if (*area_id < 0) {
        ERROR("Video Service: failed to init a overlay area for stream%d",
              stream_id);
        svc_ret->ret = AM_RESULT_ERR_INVALID;
      }
    }
  } while(0);

  return;
}

void ON_DYN_VIDEO_OVERLAY_DATA_ADD(void *msg_data,
                               int msg_data_size,
                               void *result_addr,
                               int result_max_size)
{
  PRINTF("video service ON_DYN_VIDEO_OVERLAY_DATA_ADD!\n");
  am_service_result_t *svc_ret = (am_service_result_t*) result_addr;
  memset(svc_ret, 0, sizeof(am_service_result_t));
  do {
    AMIEncodeOverlay *ol = (AMIEncodeOverlay*)g_video_camera->\
        get_video_plugin(VIDEO_PLUGIN_OVERLAY);
    if (!ol) {
      NOTICE("Video Plugin \"%s\" is not loaded!", VIDEO_PLUGIN_OVERLAY);
      svc_ret->ret = AM_RESULT_ERR_PLUGIN_LOAD;
      break;
    }

    if (!msg_data) {
      ERROR("NULL pointer!\n");
      svc_ret->ret = AM_RESULT_ERR_DATA_POINTER;
      break;
    }
    am_overlay_data_t *param = (am_overlay_data_t*) msg_data;
    AM_STREAM_ID stream_id = AM_STREAM_ID(param->stream_id);
    int32_t area_id = param->area_id;
    uint8_t data_num = param->data_num;
    am_overlay_id_t index_vector;
    AMOverlayAreaData data;
    if (param->type >= AM_OVERLAY_DATA_TYPE_NUM) {
      ERROR("Wrong overlay data type:%d\n",param->type);
      svc_ret->ret = AM_RESULT_ERR_INVALID;
      break;
    }
    data.type = AM_OVERLAY_DATA_TYPE(param->type);
    if (AM_OVERLAY_DATA_TYPE_STRING == data.type) {
      uint8_t count = 0;
      for (auto &m : param->str) {
        if ((++ count) > data_num) {
          break;
        }
        if (TEST_BIT(m.enable_bits, AM_OVERLAY_DATA_ADD_EN_BIT)) {
          AMRect &rect = data.rect;
          if (TEST_BIT(m.enable_bits, AM_OVERLAY_RECT_EN_BIT)) {
            rect.size.width = m.width;
            rect.size.height = m.height;
            rect.offset.x = m.offset_x;
            rect.offset.y = m.offset_y;
          }
          AMOverlayTextBox text;
          if (TEST_BIT(m.enable_bits, AM_OVERLAY_BG_COLOR_EN_BIT)) {
            AMOverlayCLUT &clut = text.background_color;
            clut.v = uint8_t((m.bg_color >> 24) & 0xff);
            clut.u = uint8_t((m.bg_color >> 16) & 0xff);
            clut.y = uint8_t((m.bg_color >> 8) & 0xff);
            clut.a = uint8_t(m.bg_color & 0xff);
          }
          if (TEST_BIT(m.enable_bits, AM_OVERLAY_STRING_EN_BIT)) {
            text.str = m.str;
          }
          if (TEST_BIT(m.enable_bits, AM_OVERLAY_CHAR_SPACING_EN_BIT)) {
            text.spacing = m.spacing;
          }
          if (TEST_BIT(m.enable_bits, AM_OVERLAY_FONT_COLOR_EN_BIT)) {
            if (m.font_color < AM_OVERLAY_COLOR_NUM) {
              text.font_color.id = m.font_color;
            } else {
              text.font_color.id = AM_OVERLAY_COLOR_CUSTOM;
              AMOverlayCLUT &clut = text.font_color.color;
              clut.v = uint8_t((m.font_color >> 24) & 0xff);
              clut.u = uint8_t((m.font_color >> 16) & 0xff);
              clut.y = uint8_t((m.font_color >> 8) & 0xff);
              clut.a = uint8_t(m.font_color & 0xff);
            }
          }
          AMOverlayFont &font = text.font;
          if (TEST_BIT(m.enable_bits, AM_OVERLAY_FONT_TYPE_EN_BIT)) {
            font.ttf_name = m.font_type;
          }
          if (TEST_BIT(m.enable_bits, AM_OVERLAY_FONT_SIZE_EN_BIT)) {
            font.width = m.font_size;
          }
          if (TEST_BIT(m.enable_bits, AM_OVERLAY_FONT_OUTLINE_EN_BIT)) {
            font.outline_width = m.font_outline_w;
            AMOverlayCLUT &clut = text.outline_color;
            clut.v = uint8_t((m.font_outline_color >> 24) & 0xff);
            clut.u = uint8_t((m.font_outline_color >> 16) & 0xff);
            clut.y = uint8_t((m.font_outline_color >> 8) & 0xff);
            clut.a = uint8_t(m.font_outline_color & 0xff);
          }
          if (TEST_BIT(m.enable_bits, AM_OVERLAY_FONT_BOLD_EN_BIT)) {
            font.hor_bold = m.font_hor_bold;
            font.ver_bold = m.font_ver_bold;
          }
          if (TEST_BIT(m.enable_bits, AM_OVERLAY_FONT_ITALIC_EN_BIT)) {
            font.italic = m.font_italic;
          }
          data.text = text;
          if ((index_vector.data_index[index_vector.index_num ++] =
              ol->add_data_to_area(stream_id, area_id, data)) < 0) {
            ERROR("Video Service: failed to add a data block to overlay area");
            svc_ret->ret = AM_RESULT_ERR_INVALID;
            break;
          }
        }
      }
    } else if (AM_OVERLAY_DATA_TYPE_TIME == data.type) {
      uint8_t count = 0;
      for (auto &m : param->time) {
        if ((++ count) > data_num) {
          break;
        }
        if (TEST_BIT(m.enable_bits, AM_OVERLAY_DATA_ADD_EN_BIT)) {
          AMRect &rect = data.rect;
          if (TEST_BIT(m.enable_bits, AM_OVERLAY_RECT_EN_BIT)) {
            rect.size.width = m.width;
            rect.size.height = m.height;
            rect.offset.x = m.offset_x;
            rect.offset.y = m.offset_y;
          }
          AMOverlayTextBox text;
          if (TEST_BIT(m.enable_bits, AM_OVERLAY_BG_COLOR_EN_BIT)) {
            AMOverlayCLUT &clut = text.background_color;
            clut.v = uint8_t((m.bg_color >> 24) & 0xff);
            clut.u = uint8_t((m.bg_color >> 16) & 0xff);
            clut.y = uint8_t((m.bg_color >> 8) & 0xff);
            clut.a = uint8_t(m.bg_color & 0xff);
          }
          if (TEST_BIT(m.enable_bits, AM_OVERLAY_CHAR_SPACING_EN_BIT)) {
            text.spacing = m.spacing;
          }
          if (TEST_BIT(m.enable_bits, AM_OVERLAY_FONT_COLOR_EN_BIT)) {
            if (m.font_color < AM_OVERLAY_COLOR_NUM) {
              text.font_color.id = m.font_color;
            } else {
              text.font_color.id = AM_OVERLAY_COLOR_CUSTOM;
              AMOverlayCLUT &clut = text.font_color.color;
              clut.v = uint8_t((m.font_color >> 24) & 0xff);
              clut.u = uint8_t((m.font_color >> 16) & 0xff);
              clut.y = uint8_t((m.font_color >> 8) & 0xff);
              clut.a = uint8_t(m.font_color & 0xff);
            }
          }
          AMOverlayFont &font = text.font;
          if (TEST_BIT(m.enable_bits, AM_OVERLAY_FONT_TYPE_EN_BIT)) {
            font.ttf_name = m.font_type;
          }
          if (TEST_BIT(m.enable_bits, AM_OVERLAY_FONT_SIZE_EN_BIT)) {
            font.width = m.font_size;
          }
          if (TEST_BIT(m.enable_bits, AM_OVERLAY_FONT_OUTLINE_EN_BIT)) {
            font.outline_width = m.font_outline_w;
            AMOverlayCLUT &clut = text.outline_color;
            clut.v = uint8_t((m.font_outline_color >> 24) & 0xff);
            clut.u = uint8_t((m.font_outline_color >> 16) & 0xff);
            clut.y = uint8_t((m.font_outline_color >> 8) & 0xff);
            clut.a = uint8_t(m.font_outline_color & 0xff);
          }
          if (TEST_BIT(m.enable_bits, AM_OVERLAY_FONT_BOLD_EN_BIT)) {
            font.hor_bold = m.font_hor_bold;
            font.ver_bold = m.font_ver_bold;
          }
          if (TEST_BIT(m.enable_bits, AM_OVERLAY_FONT_ITALIC_EN_BIT)) {
            font.italic = m.font_italic;
          }
          if (TEST_BIT(m.enable_bits, AM_OVERLAY_TIME_EN_BIT)) {
            data.time.pre_str = m.pre_str;
            data.time.suf_str = m.suf_str;
            data.time.en_msec = m.msec_en;
            data.time.format = m.time_format;
            data.time.is_12h = m.is_12h;
          }
            data.time.text = text;
          if ((index_vector.data_index[index_vector.index_num ++] =
              ol->add_data_to_area(stream_id, area_id, data)) < 0) {
            ERROR("Video Service: failed to add a data block to overlay area");
            svc_ret->ret = AM_RESULT_ERR_INVALID;
            break;
          }
        }
      }
    } else if ((AM_OVERLAY_DATA_TYPE_PICTURE == data.type) ||
               (AM_OVERLAY_DATA_TYPE_ANIMATION == data.type)) {
      uint8_t count = 0;
      for (auto &m : param->pic) {
        if ((++ count) > data_num) {
          break;
        }
        if (TEST_BIT(m.enable_bits, AM_OVERLAY_DATA_ADD_EN_BIT)) {
          AMRect &rect = data.rect;
          if (TEST_BIT(m.enable_bits, AM_OVERLAY_RECT_EN_BIT)) {
            rect.offset.x = m.offset_x;
            rect.offset.y = m.offset_y;
          }
          AMOverlayPicture pic;
          if (TEST_BIT(m.enable_bits, AM_OVERLAY_BMP_EN_BIT)) {
            pic.filename = m.bmp;
          }
          if (TEST_BIT(m.enable_bits, AM_OVERLAY_BMP_COLOR_EN_BIT))
          {
            AMOverlayCLUT &clut = pic.colorkey.color;
            clut.v = uint8_t((m.color_key  >> 24) & 0xff);
            clut.u = uint8_t((m.color_key  >> 16) & 0xff);
            clut.y = uint8_t((m.color_key  >> 8) & 0xff);
            clut.a = uint8_t(m.color_key & 0xff);

            pic.colorkey.range = m.color_range;
          }
          if (AM_OVERLAY_DATA_TYPE_ANIMATION == data.type) {
            if (TEST_BIT(m.enable_bits, AM_OVERLAY_ANIMATION_EN_BIT)) {
              data.anim.num = m.bmp_num;
              data.anim.interval = m.interval;
            }
            data.anim.pic = pic;
          } else {
            data.pic = pic;
          }
          if ((index_vector.data_index[index_vector.index_num ++] =
              ol->add_data_to_area(stream_id, area_id, data)) < 0) {
            ERROR("Video Service: failed to add a data block to overlay area");
            svc_ret->ret = AM_RESULT_ERR_INVALID;
            break;
          }
        }
      }
    } else if (AM_OVERLAY_DATA_TYPE_LINE == data.type) {
      uint8_t count = 0;
      for (auto &m : param->line) {
        if ((++ count) > data_num) {
          break;
        }
        if (TEST_BIT(m.enable_bits, AM_OVERLAY_DATA_ADD_EN_BIT)) {
          AMRect &rect = data.rect;
          if (TEST_BIT(m.enable_bits, AM_OVERLAY_RECT_EN_BIT)) {
            rect.size.width = m.width;
            rect.size.height = m.height;
            rect.offset.x = m.offset_x;
            rect.offset.y = m.offset_y;
          } else {
            //TODO:fixme
            break;
          }
          INFO("x=%d y=%d w=%d h=%d\n",rect.offset.x, rect.offset.y,
                 rect.size.width, rect.size.height);
          AMOverlayLine line;
          if (TEST_BIT(m.enable_bits, AM_OVERLAY_LINE_COLOR_EN_BIT)) {
            if (m.line_color < AM_OVERLAY_COLOR_NUM) {
              line.color.id = m.line_color;
            } else {
              line.color.id = AM_OVERLAY_COLOR_CUSTOM;
              AMOverlayCLUT &clut = line.color.color;
              clut.v = uint8_t((m.line_color >> 24) & 0xff);
              clut.u = uint8_t((m.line_color >> 16) & 0xff);
              clut.y = uint8_t((m.line_color >> 8) & 0xff);
              clut.a = uint8_t(m.line_color & 0xff);
            }
          }
          if (TEST_BIT(m.enable_bits, AM_OVERLAY_LINE_THICKNESS_EN_BIT))
          {
            line.thickness = m.line_tn;
          }
          if (TEST_BIT(m.enable_bits, AM_OVERLAY_LINE_POINTS_EN_BIT))
          {
            int32_t max = AM_MIN(m.p_n, OVERLAY_MAX_POINT);
            for (int32_t n = 0; n < max; ++n) {
              line.point.push_back(AMPoint(m.p_x[n], m.p_y[n]));
            }
          }
          data.line = line;
          if ((index_vector.data_index[index_vector.index_num ++] =
              ol->add_data_to_area(stream_id, area_id, data)) < 0) {
            ERROR("Video Service: failed to add a data block to overlay area");
            svc_ret->ret = AM_RESULT_ERR_INVALID;
            break;
          }
        }
      }
    } else if (AM_OVERLAY_DATA_TYPE_RECTANGLE == data.type) {
      uint8_t count = 0;
      for (auto &m : param->rtg) {
        if ((++ count) > data_num) {
          break;
        }
        if (TEST_BIT(m.enable_bits, AM_OVERLAY_DATA_ADD_EN_BIT)) {
          AMRect &rect = data.rect;
          if (TEST_BIT(m.enable_bits, AM_OVERLAY_RECT_EN_BIT)) {
            rect.size.width = m.width;
            rect.size.height = m.height;
            rect.offset.x = m.offset_x;
            rect.offset.y = m.offset_y;
          } else {
            //TODO:fixme
            break;
          }
          INFO("x=%d y=%d w=%d h=%d\n",rect.offset.x, rect.offset.y,
               rect.size.width, rect.size.height);
          AMOverlayRectangle rtg;
          if (m.rtg_type == AM_OVERLAY_RTG_TYPE_HOLLOW) {
            if (TEST_BIT(m.enable_bits, AM_OVERLAY_RTG_COLOR_EN_BIT)) {
              if (m.rtg_color < AM_OVERLAY_COLOR_NUM) {
                rtg.h_color.id = m.rtg_color;
              } else {
                rtg.h_color.id = AM_OVERLAY_COLOR_CUSTOM;
                AMOverlayCLUT &clut = rtg.h_color.color;
                clut.v = uint8_t((m.rtg_color >> 24) & 0xff);
                clut.u = uint8_t((m.rtg_color >> 16) & 0xff);
                clut.y = uint8_t((m.rtg_color >> 8) & 0xff);
                clut.a = uint8_t(m.rtg_color & 0xff);
              }
            }
            if (TEST_BIT(m.enable_bits, AM_OVERLAY_RTG_THICKNESS_EN_BIT)) {
              rtg.h_thickness = m.rtg_line_tn;
            }
            if (TEST_BIT(m.enable_bits, AM_OVERLAY_RTG_POINTS_EN_BIT))
            {
              for (int32_t n = 0; n < m.point_pair_num; ++ n) {
                AMPointPair pair;
                pair.p1.x = m.first_x[n];
                pair.p1.y = m.first_y[n];
                pair.p2.x = m.second_x[n];
                pair.p2.y = m.second_y[n];
                rtg.h_rect.push_back(pair);
              }
            }
          } else if (m.rtg_type == AM_OVERLAY_RTG_TYPE_SOLID) {
            if (TEST_BIT(m.enable_bits, AM_OVERLAY_RTG_COLOR_EN_BIT)) {
              if (m.rtg_color < AM_OVERLAY_COLOR_NUM) {
                rtg.s_color.id = m.rtg_color;
              } else {
                rtg.s_color.id = AM_OVERLAY_COLOR_CUSTOM;
                AMOverlayCLUT &clut = rtg.s_color.color;
                clut.v = uint8_t((m.rtg_color >> 24) & 0xff);
                clut.u = uint8_t((m.rtg_color >> 16) & 0xff);
                clut.y = uint8_t((m.rtg_color >> 8) & 0xff);
                clut.a = uint8_t(m.rtg_color & 0xff);
              }
            }
            if (TEST_BIT(m.enable_bits, AM_OVERLAY_RTG_THICKNESS_EN_BIT))
            {
              rtg.s_thickness = m.rtg_line_tn;
            }
            if (TEST_BIT(m.enable_bits, AM_OVERLAY_RTG_POINTS_EN_BIT))
            {
              for (int32_t n = 0; n < m.point_pair_num; ++ n) {
                AMPoint point;
                point.x = m.first_x[n];
                point.y = m.first_y[n];
                rtg.s_rect.push_back(point);
              }
            }
          } else {
            ERROR("Invalid rectangle type %d", m.rtg_type);
            break;
          }
          data.rtg = rtg;
          if ((index_vector.data_index[index_vector.index_num ++] =
              ol->add_data_to_area(stream_id, area_id, data)) < 0) {
            ERROR("Video Service: failed to add a data block to overlay area");
            svc_ret->ret = AM_RESULT_ERR_INVALID;
            break;
          }
        }
      }
    } else {
      ERROR("Invalid overlay type %d", data.type);
    }
    memcpy(svc_ret->data, &index_vector, sizeof(index_vector));
  } while(0);
  return;
}

void ON_DYN_VIDEO_OVERLAY_DATA_UPDATE(void *msg_data,
                                  int msg_data_size,
                                  void *result_addr,
                                  int result_max_size)
{
  PRINTF("video service ON_DYN_VIDEO_OVERLAY_DATA_UPDATE!\n");
  am_service_result_t *svc_ret = (am_service_result_t*) result_addr;
  memset(svc_ret, 0, sizeof(am_service_result_t));
  am_overlay_id_t opt_ret_value;
  do {
    AMIEncodeOverlay *ol = (AMIEncodeOverlay*)g_video_camera->\
        get_video_plugin(VIDEO_PLUGIN_OVERLAY);

    if (!ol) {
      NOTICE("Video Plugin \"%s\" is not loaded!", VIDEO_PLUGIN_OVERLAY);
      svc_ret->ret = AM_RESULT_ERR_PLUGIN_LOAD;
      break;
    }
    if (!msg_data) {
      ERROR("NULL pointer!\n");
      svc_ret->ret = AM_RESULT_ERR_DATA_POINTER;
      break;
    }
    am_overlay_data_t *param = (am_overlay_data_t*) msg_data;
    AM_STREAM_ID stream_id = AM_STREAM_ID(param->stream_id);
    int32_t area_id = param->area_id;
    uint8_t data_num = param->data_num;
    if (param->type >= AM_OVERLAY_DATA_TYPE_NUM) {
      ERROR("Wrong overlay data type:%d\n",param->type);
      svc_ret->ret = AM_RESULT_ERR_INVALID;
      break;
    }
    AM_OVERLAY_DATA_TYPE type = AM_OVERLAY_DATA_TYPE(param->type);
    AMOverlayTextBox *text = nullptr;
    AMOverlayPicture *pic = nullptr;
    AMOverlayLine *line = nullptr;
    AMOverlayRectangle *rtg = nullptr;
    opt_ret_value.stream_id = stream_id;
    opt_ret_value.area_id = area_id;
    opt_ret_value.index_num = data_num;
    if (AM_OVERLAY_DATA_TYPE_STRING == type) {
      uint8_t count = 0;
      for (auto &m : param->str) {
        if ((++ count) > data_num) {
          break;
        }
        if (TEST_BIT(m.enable_bits, AM_OVERLAY_DATA_UPDATE_EN_BIT)) {
          int32_t index = m.data_index;
          opt_ret_value.data_index[count - 1] = index;
          AMOverlayAreaData data;
          if (AM_UNLIKELY(ol->get_area_data_param(
              stream_id, area_id, index, data) != AM_RESULT_OK)) {
            ERROR("Failed to get area data param, stream%d area%d data "
                "block%d type is %d", stream_id, area_id, index, data.type);
            svc_ret->ret = AM_RESULT_ERR_INVALID;
            opt_ret_value.index_ret[count - 1] = -1;
            continue;
          }
          if (data.type != AM_OVERLAY_DATA_TYPE_STRING) {
            ERROR("Wrong data type%d, stream%d area%d data block%d type is %d\n",
                  type, stream_id, area_id, index, data.type);
            svc_ret->ret = AM_RESULT_ERR_INVALID;
            opt_ret_value.index_ret[count - 1] = -1;
            continue;
          }
          text = &(data.text);
          if (TEST_BIT(m.enable_bits, AM_OVERLAY_BG_COLOR_EN_BIT)) {
            AMOverlayCLUT &clut = text->background_color;
            clut.v = uint8_t((m.bg_color >> 24) & 0xff);
            clut.u = uint8_t((m.bg_color >> 16) & 0xff);
            clut.y = uint8_t((m.bg_color >> 8) & 0xff);
            clut.a = uint8_t(m.bg_color & 0xff);
          }
          if (TEST_BIT(m.enable_bits, AM_OVERLAY_STRING_EN_BIT)) {
            text->str = m.str;
          }
          if (TEST_BIT(m.enable_bits, AM_OVERLAY_CHAR_SPACING_EN_BIT)) {
            text->spacing = m.spacing;
          }
          if (TEST_BIT(m.enable_bits, AM_OVERLAY_FONT_COLOR_EN_BIT)) {
            if (m.font_color < 8) {
              text->font_color.id = m.font_color;
            } else {
              text->font_color.id = 8;
              AMOverlayCLUT &clut = text->font_color.color;
              clut.v = uint8_t((m.font_color >> 24) & 0xff);
              clut.u = uint8_t((m.font_color >> 16) & 0xff);
              clut.y = uint8_t((m.font_color >> 8) & 0xff);
              clut.a = uint8_t(m.font_color & 0xff);
            }
          }
          AMOverlayFont &font = text->font;
          if (TEST_BIT(m.enable_bits, AM_OVERLAY_FONT_TYPE_EN_BIT)) {
            font.ttf_name = m.font_type;
          }
          if (TEST_BIT(m.enable_bits, AM_OVERLAY_FONT_SIZE_EN_BIT)) {
            font.width = m.font_size;
          }
          if (TEST_BIT(m.enable_bits, AM_OVERLAY_FONT_OUTLINE_EN_BIT)) {
            font.outline_width = m.font_outline_w;
            AMOverlayCLUT &clut = text->outline_color;
            clut.v = uint8_t((m.font_outline_color >> 24) & 0xff);
            clut.u = uint8_t((m.font_outline_color >> 16) & 0xff);
            clut.y = uint8_t((m.font_outline_color >> 8) & 0xff);
            clut.a = uint8_t(m.font_outline_color & 0xff);
          }
          if (TEST_BIT(m.enable_bits, AM_OVERLAY_FONT_BOLD_EN_BIT)) {
            font.hor_bold = m.font_hor_bold;
            font.ver_bold = m.font_ver_bold;
          }
          if (TEST_BIT(m.enable_bits, AM_OVERLAY_FONT_ITALIC_EN_BIT)) {
            font.italic = m.font_italic;
          }
          if (AM_UNLIKELY(ol->update_area_data(
              stream_id, area_id, index, data) != AM_RESULT_OK)) {
            ERROR("Update area data error, stream%d area%d data block%d type "
                "is %d", stream_id, area_id, index, data.type);;
            svc_ret->ret = AM_RESULT_ERR_INVALID;
            opt_ret_value.index_ret[count - 1] = -1;
            continue;
          }
        }
      }
    } else if (AM_OVERLAY_DATA_TYPE_TIME == type) {
      uint8_t count = 0;
      for (auto &m : param->time) {
        if ((++ count) > data_num) {
          break;
        }
        if (TEST_BIT(m.enable_bits, AM_OVERLAY_DATA_UPDATE_EN_BIT)) {
          int32_t index = m.data_index;
          AMOverlayAreaData data;
          opt_ret_value.data_index[count - 1] = index;
          if (AM_UNLIKELY(ol->get_area_data_param(
              stream_id, area_id, index, data) != AM_RESULT_OK)) {
            ERROR("Failed to get area data param, stream%d area%d data "
                "block%d type is %d", stream_id, area_id, index, data.type);
            opt_ret_value.index_ret[count - 1] = -1;
            svc_ret->ret = AM_RESULT_ERR_INVALID;
            continue;
          }
          if (data.type != AM_OVERLAY_DATA_TYPE_TIME) {
            ERROR("Wrong data type%d, stream%d area%d data block%d type is %d\n",
                  type, stream_id, area_id, index, data.type);
            svc_ret->ret = AM_RESULT_ERR_INVALID;
            opt_ret_value.index_ret[count - 1] = -1;
            continue;
          }
          text = &(data.time.text);
          if (TEST_BIT(m.enable_bits, AM_OVERLAY_BG_COLOR_EN_BIT)) {
            AMOverlayCLUT &clut = text->background_color;
            clut.v = uint8_t((m.bg_color >> 24) & 0xff);
            clut.u = uint8_t((m.bg_color >> 16) & 0xff);
            clut.y = uint8_t((m.bg_color >> 8) & 0xff);
            clut.a = uint8_t(m.bg_color & 0xff);
          }
          if (TEST_BIT(m.enable_bits, AM_OVERLAY_CHAR_SPACING_EN_BIT)) {
            text->spacing = m.spacing;
          }
          if (TEST_BIT(m.enable_bits, AM_OVERLAY_FONT_COLOR_EN_BIT)) {
            if (m.font_color < 8) {
              text->font_color.id = m.font_color;
            } else {
              text->font_color.id = 8;
              AMOverlayCLUT &clut = text->font_color.color;
              clut.v = uint8_t((m.font_color >> 24) & 0xff);
              clut.u = uint8_t((m.font_color >> 16) & 0xff);
              clut.y = uint8_t((m.font_color >> 8) & 0xff);
              clut.a = uint8_t(m.font_color & 0xff);
            }
          }
          AMOverlayFont &font = text->font;
          if (TEST_BIT(m.enable_bits, AM_OVERLAY_FONT_TYPE_EN_BIT)) {
            font.ttf_name = m.font_type;
          }
          if (TEST_BIT(m.enable_bits, AM_OVERLAY_FONT_SIZE_EN_BIT)) {
            font.width = m.font_size;
          }
          if (TEST_BIT(m.enable_bits, AM_OVERLAY_FONT_OUTLINE_EN_BIT)) {
            font.outline_width = m.font_outline_w;
            AMOverlayCLUT &clut = text->outline_color;
            clut.v = uint8_t((m.font_outline_color >> 24) & 0xff);
            clut.u = uint8_t((m.font_outline_color >> 16) & 0xff);
            clut.y = uint8_t((m.font_outline_color >> 8) & 0xff);
            clut.a = uint8_t(m.font_outline_color & 0xff);
          }
          if (TEST_BIT(m.enable_bits, AM_OVERLAY_FONT_BOLD_EN_BIT)) {
            font.hor_bold = m.font_hor_bold;
            font.ver_bold = m.font_ver_bold;
          }
          if (TEST_BIT(m.enable_bits, AM_OVERLAY_FONT_ITALIC_EN_BIT)) {
            font.italic = m.font_italic;
          }
          if (TEST_BIT(m.enable_bits, AM_OVERLAY_TIME_EN_BIT)) {
            data.time.pre_str = m.pre_str;
            data.time.suf_str = m.suf_str;
            data.time.en_msec = m.msec_en;
            data.time.format = m.time_format;
            data.time.is_12h = m.is_12h;
          }
          if (AM_UNLIKELY(ol->update_area_data(
              stream_id, area_id, index, data) != AM_RESULT_OK)) {
            ERROR("Update area data error, stream%d area%d data block%d type "
                "is %d",stream_id, area_id, index, data.type);
            svc_ret->ret = AM_RESULT_ERR_INVALID;
            opt_ret_value.index_ret[count - 1] = -1;
            continue;
          }
        }
      }
    } else if ((AM_OVERLAY_DATA_TYPE_PICTURE == type) ||
        (AM_OVERLAY_DATA_TYPE_ANIMATION == type)) {
      uint8_t count = 0;
      for (auto &m : param->pic) {
        if ((++ count) > data_num) {
          break;
        }
        if (TEST_BIT(m.enable_bits, AM_OVERLAY_DATA_UPDATE_EN_BIT)) {
          int32_t index = m.data_index;
          AMOverlayAreaData data;
          opt_ret_value.data_index[count -1] = index;
          if (AM_UNLIKELY(ol->get_area_data_param(
              stream_id, area_id, index, data) != AM_RESULT_OK)) {
            ERROR("Failed to get area data param, stream%d area%d data "
                "block%d type is %d",stream_id, area_id, index, data.type);
            svc_ret->ret = AM_RESULT_ERR_INVALID;
            opt_ret_value.index_ret[count - 1] = -1;
            continue;
          }
          if (data.type != AM_OVERLAY_DATA_TYPE_PICTURE &&
              data.type != AM_OVERLAY_DATA_TYPE_ANIMATION) {
            ERROR("Wrong data type%d, stream%d area%d data block%d type is %d\n",
                  type, stream_id, area_id, index, data.type);
            svc_ret->ret = AM_RESULT_ERR_INVALID;
            opt_ret_value.index_ret[count - 1] = -1;
            continue;
          }
          if (data.type == AM_OVERLAY_DATA_TYPE_PICTURE) {
            pic = &(data.pic);
          } else if (data.type == AM_OVERLAY_DATA_TYPE_ANIMATION) {
            pic = &(data.anim.pic);
          }
          if (TEST_BIT(m.enable_bits, AM_OVERLAY_BMP_EN_BIT)) {
            pic->filename = m.bmp;
          }
          if (TEST_BIT(m.enable_bits, AM_OVERLAY_BMP_COLOR_EN_BIT))
          {
            AMOverlayCLUT &clut = pic->colorkey.color;
            clut.v = uint8_t((m.color_key  >> 24) & 0xff);
            clut.u = uint8_t((m.color_key  >> 16) & 0xff);
            clut.y = uint8_t((m.color_key  >> 8) & 0xff);
            clut.a = uint8_t(m.color_key & 0xff);

            pic->colorkey.range = m.color_range;
          }
          if (TEST_BIT(m.enable_bits, AM_OVERLAY_ANIMATION_EN_BIT)) {
            data.anim.num = m.bmp_num;
            data.anim.interval = m.interval;
          }

          if (AM_UNLIKELY(ol->update_area_data(
              stream_id, area_id, index, data) != AM_RESULT_OK)) {
            ERROR("Update area data error, stream%d area%d data block%d type "
                "is %d",stream_id, area_id, index, data.type);
            svc_ret->ret = AM_RESULT_ERR_INVALID;
            opt_ret_value.index_ret[count - 1] = -1;
            continue;
          }
        }
      }
    } else if (AM_OVERLAY_DATA_TYPE_LINE == type) {
      uint8_t count = 0;
      for (auto &m : param->line) {
        if ((++ count) > data_num) {
          break;
        }
        if (TEST_BIT(m.enable_bits, AM_OVERLAY_DATA_UPDATE_EN_BIT)) {
          int32_t index = m.data_index;
          AMOverlayAreaData data;
          if (AM_UNLIKELY(ol->get_area_data_param(
              stream_id, area_id, index, data) != AM_RESULT_OK)) {
            ERROR("Failed to get area data param, stream%d area%d data "
                "block%d type is %d",stream_id, area_id, index, data.type);
            svc_ret->ret = AM_RESULT_ERR_INVALID;
            opt_ret_value.index_ret[count - 1] = -1;
            continue;
          }
          if (data.type != AM_OVERLAY_DATA_TYPE_LINE) {
            ERROR("Wrong data type%d, stream%d area%d data block%d type is %d\n",
                  type, stream_id, area_id, index, data.type);
            svc_ret->ret = AM_RESULT_ERR_INVALID;
            opt_ret_value.index_ret[count - 1] = -1;
            continue;
          }
          line = &(data.line);
          if (TEST_BIT(m.enable_bits, AM_OVERLAY_LINE_COLOR_EN_BIT)) {
            if (m.line_color < AM_OVERLAY_COLOR_NUM) {
              line->color.id = m.line_color;
            } else {
              line->color.id = AM_OVERLAY_COLOR_CUSTOM;
              AMOverlayCLUT &clut = line->color.color;
              clut.v = uint8_t((m.line_color >> 24) & 0xff);
              clut.u = uint8_t((m.line_color >> 16) & 0xff);
              clut.y = uint8_t((m.line_color >> 8) & 0xff);
              clut.a = uint8_t(m.line_color & 0xff);
            }
          }
          if (TEST_BIT(m.enable_bits, AM_OVERLAY_LINE_THICKNESS_EN_BIT)) {
            line->thickness = m.line_tn;
          }
          if (TEST_BIT(m.enable_bits, AM_OVERLAY_LINE_POINTS_EN_BIT)) {
            line->point.clear();
            for (uint32_t n = 0; n < m.p_n; ++n) {
              line->point.push_back(AMPoint(m.p_x[n], m.p_y[n]));
            }
          }
          if (AM_UNLIKELY(ol->update_area_data(
              stream_id, area_id, index, data) != AM_RESULT_OK)) {
            ERROR("Update area data error, stream%d area%d data block%d type "
                "is %d",stream_id, area_id, index, data.type);
            svc_ret->ret = AM_RESULT_ERR_INVALID;
            opt_ret_value.index_ret[count - 1] = -1;
            continue;
          }
        }
      }
    } else if (AM_OVERLAY_DATA_TYPE_RECTANGLE == type) {
      uint8_t count = 0;
      for (auto &m : param->rtg) {
        if ((++ count) > data_num) {
          break;
        }
        if (TEST_BIT(m.enable_bits, AM_OVERLAY_DATA_UPDATE_EN_BIT)) {
          int32_t index = m.data_index;
          AMOverlayAreaData data;
          if (AM_UNLIKELY(ol->get_area_data_param(
              stream_id, area_id, index, data) != AM_RESULT_OK)) {
            ERROR("Failed to get area data param, stream%d area%d data "
                "block%d type is %d",stream_id, area_id, index, data.type);
            svc_ret->ret = AM_RESULT_ERR_INVALID;
            opt_ret_value.index_ret[count - 1] = -1;
            continue;
          }
          if (data.type != AM_OVERLAY_DATA_TYPE_RECTANGLE) {
            ERROR("Wrong data type%d, stream%d area%d data block%d type is %d\n",
                  type, stream_id, area_id, index, data.type);
            svc_ret->ret = AM_RESULT_ERR_INVALID;
            opt_ret_value.index_ret[count - 1] = -1;
            continue;
          }
          rtg = &(data.rtg);
          if (m.rtg_type == AM_OVERLAY_RTG_TYPE_HOLLOW) {
            if (TEST_BIT(m.enable_bits, AM_OVERLAY_RTG_COLOR_EN_BIT)) {
              if (m.rtg_color < AM_OVERLAY_COLOR_NUM) {
                rtg->h_color.id = m.rtg_color;
              } else {
                rtg->h_color.id = AM_OVERLAY_COLOR_CUSTOM;
                AMOverlayCLUT &clut = rtg->h_color.color;
                clut.v = uint8_t((m.rtg_color >> 24) & 0xff);
                clut.u = uint8_t((m.rtg_color >> 16) & 0xff);
                clut.y = uint8_t((m.rtg_color >> 8) & 0xff);
                clut.a = uint8_t(m.rtg_color & 0xff);
              }
            }
            if (TEST_BIT(m.enable_bits, AM_OVERLAY_RTG_THICKNESS_EN_BIT)) {
              rtg->h_thickness = m.rtg_line_tn;
            }
            if (TEST_BIT(m.enable_bits, AM_OVERLAY_RTG_POINTS_EN_BIT))
            {
              rtg->h_rect.clear();
              for (int32_t n = 0; n < m.point_pair_num; ++ n) {
                AMPointPair pair;
                pair.p1.x = m.first_x[n];
                pair.p1.y = m.first_y[n];
                pair.p2.x = m.second_x[n];
                pair.p2.y = m.second_y[n];
                rtg->h_rect.push_back(pair);
              }
            }
          } else if (m.rtg_type == AM_OVERLAY_RTG_TYPE_SOLID) {
            if (TEST_BIT(m.enable_bits, AM_OVERLAY_RTG_COLOR_EN_BIT)) {
              if (m.rtg_color < AM_OVERLAY_COLOR_NUM) {
                rtg->s_color.id = m.rtg_color;
              } else {
                rtg->s_color.id = AM_OVERLAY_COLOR_CUSTOM;
                AMOverlayCLUT &clut = rtg->s_color.color;
                clut.v = uint8_t((m.rtg_color >> 24) & 0xff);
                clut.u = uint8_t((m.rtg_color >> 16) & 0xff);
                clut.y = uint8_t((m.rtg_color >> 8) & 0xff);
                clut.a = uint8_t(m.rtg_color & 0xff);
              }
            }
            if (TEST_BIT(m.enable_bits, AM_OVERLAY_RTG_THICKNESS_EN_BIT))
            {
              rtg->s_thickness = m.rtg_line_tn;
            }
            if (TEST_BIT(m.enable_bits, AM_OVERLAY_RTG_POINTS_EN_BIT))
            {
              rtg->s_rect.clear();
              for (int32_t n = 0; n < m.point_pair_num; ++ n) {
                AMPoint point;
                point.x = m.first_x[n];
                point.y = m.first_y[n];
                rtg->s_rect.push_back(point);
              }
            }
          } else {
            ERROR("Invalid rectangle type %d", m.rtg_type);
            break;
          }
          if (AM_UNLIKELY(ol->update_area_data(
              stream_id, area_id, index, data) != AM_RESULT_OK)) {
            ERROR("Update area data error, stream%d area%d data block%d type "
                "is %d",stream_id, area_id, index, data.type);
            svc_ret->ret = AM_RESULT_ERR_INVALID;
            opt_ret_value.index_ret[count - 1] = -1;
            continue;
          }
        }
      }
    } else {
      ERROR("Invalid overlay type %d", type);
    }
  } while(0);
  memcpy(svc_ret->data, &opt_ret_value, sizeof(opt_ret_value));
  return;
}

void ON_DYN_VIDEO_OVERLAY_SET(void *msg_data,
                          int msg_data_size,
                          void *result_addr,
                          int result_max_size)
{
  PRINTF("video service ON_DYN_VIDEO_OVERLAY_SET\n");
  am_service_result_t *svc_ret = (am_service_result_t*) result_addr;
  memset(svc_ret, 0, sizeof(am_service_result_t));

  do {
    AMIEncodeOverlay *ol = (AMIEncodeOverlay*)g_video_camera->\
        get_video_plugin(VIDEO_PLUGIN_OVERLAY);

    if (!ol) {
      NOTICE("Video Plugin \"%s\" is not loaded!", VIDEO_PLUGIN_OVERLAY);
      svc_ret->ret = AM_RESULT_ERR_PLUGIN_LOAD;
      break;
    }

    if (!msg_data) {
      svc_ret->ret = AM_RESULT_ERR_DATA_POINTER;
      break;
    }
    am_overlay_id_t *id = (am_overlay_id_t*)msg_data;

    //remove area
    if (TEST_BIT(id->enable_bits, AM_OVERLAY_REMOVE_EN_BIT)) {
      AM_STREAM_ID stream_id = AM_STREAM_ID(id->stream_id);
      int32_t area_id = id->area_id;
      if (AM_UNLIKELY((svc_ret->ret = ol->change_state(
          stream_id, area_id, AM_OVERLAY_DELETE)) != AM_RESULT_OK)) {
        ERROR("Video Service: failed to remove overlay area\n");
        break;
      }
    }

    //enable
    if (TEST_BIT(id->enable_bits, AM_OVERLAY_ENABLE_EN_BIT)) {
      AM_STREAM_ID stream_id = AM_STREAM_ID(id->stream_id);
      int32_t area_id = id->area_id;
      if (AM_UNLIKELY((svc_ret->ret = ol->change_state(
          stream_id, area_id, AM_OVERLAY_ENABLE)) != AM_RESULT_OK)) {
        ERROR("Video Service: failed to enable overlay area\n");
        break;
      }
    }

    //disable
    if (TEST_BIT(id->enable_bits, AM_OVERLAY_DISABLE_EN_BIT)) {
      AM_STREAM_ID stream_id = AM_STREAM_ID(id->stream_id);
      int32_t area_id = id->area_id;
      if (AM_UNLIKELY((svc_ret->ret = ol->change_state(
          stream_id, area_id, AM_OVERLAY_DISABLE)) != AM_RESULT_OK)) {
        ERROR("Video Service: failed to disable overlay area\n");
        break;
      }
    }

    //remove data block
    if (TEST_BIT(id->enable_bits, AM_OVERLAY_DATA_REMOVE_EN_BIT)) {
      AM_STREAM_ID stream_id = AM_STREAM_ID(id->stream_id);
      int32_t area_id = id->area_id;
      uint16_t index_num = id->index_num;
      uint8_t count = 0;
      am_overlay_id_t opt_ret_value;
      opt_ret_value.stream_id = stream_id;
      opt_ret_value.area_id = area_id;
      opt_ret_value.index_num = index_num;
      for (auto &m : id->data_index) {
        if ((++ count) >index_num) {
          break;
        }
        opt_ret_value.data_index[count - 1] = m;
        if (AM_UNLIKELY(ol->delete_area_data(
            stream_id, area_id, m) != AM_RESULT_OK)) {
          ERROR("Delete area data error, stream%d area%d data block%d",
                stream_id, area_id, m);
          svc_ret->ret = -1;
          opt_ret_value.index_ret[count - 1] = -1;
          continue;
        }
      }
      memcpy(svc_ret->data, &opt_ret_value, sizeof(opt_ret_value));
    }
  } while(0);

  return;
}

void ON_DYN_VIDEO_OVERLAY_GET(void *msg_data,
                          int msg_data_size,
                          void *result_addr,
                          int result_max_size)
{
  PRINTF("video service ON_DYN_VIDEO_OVERLAY_GET!\n");
  am_service_result_t *svc_ret = (am_service_result_t*) result_addr;
  memset(svc_ret, 0, sizeof(am_service_result_t));

  do {
    AMIEncodeOverlay *ol = (AMIEncodeOverlay*)g_video_camera->\
        get_video_plugin(VIDEO_PLUGIN_OVERLAY);

    if (!ol) {
      NOTICE("Video Plugin \"%s\" is not loaded!", VIDEO_PLUGIN_OVERLAY);
      svc_ret->ret = AM_RESULT_ERR_PLUGIN_LOAD;
      break;
    }

    if (!msg_data) {
      ERROR("NULL pointer!\n");
      svc_ret->ret = AM_RESULT_ERR_DATA_POINTER;
      break;
    }

    am_overlay_area_t *area = (am_overlay_area_t*)svc_ret->data;
    AM_STREAM_ID stream_id = AM_STREAM_ID(((am_overlay_id_t*)msg_data)
                                          ->stream_id);
    uint32_t area_id = ((am_overlay_id_t*)msg_data)->area_id;

    AMOverlayAreaParam param;
    if (AM_UNLIKELY((svc_ret->ret = ol->get_area_param(
        stream_id, area_id, param)) != AM_RESULT_OK)) {
      ERROR("Overlay get area param error.");
      break;
    }
    if (param.attr.enable < 0) {
     area->enable = 255;
     break;
    }

    area->enable = param.attr.enable;
    area->width = param.attr.rect.size.width;
    area->height = param.attr.rect.size.height;
    area->offset_x = param.attr.rect.offset.x;
    area->offset_y = param.attr.rect.offset.y;
    area->rotate = param.attr.rotate;
    AMOverlayCLUT &c = param.attr.bg_color;
    area->bg_color = (c.v << 24) | (c.u << 16) | (c.y << 8) | c.a;
    area->buf_num = param.attr.buf_num;
    area->data_num = param.num;
  } while(0);

  return;
}

void ON_DYN_VIDEO_OVERLAY_DATA_GET(void *msg_data,
                               int msg_data_size,
                               void *result_addr,
                               int result_max_size)
{
  PRINTF("video service ON_DYN_VIDEO_OVERLAY_DATA_GET!\n");
  am_service_result_t *svc_ret = (am_service_result_t*) result_addr;
  memset(svc_ret, 0, sizeof(am_service_result_t));
  do {
    AMIEncodeOverlay *ol = (AMIEncodeOverlay*)g_video_camera->\
        get_video_plugin(VIDEO_PLUGIN_OVERLAY);
    if (!ol) {
      NOTICE("Video Plugin \"%s\" is not loaded!", VIDEO_PLUGIN_OVERLAY);
      svc_ret->ret = AM_RESULT_ERR_PLUGIN_LOAD;
      break;
    }
    if (!msg_data) {
      ERROR("NULL pointer!\n");
      svc_ret->ret = AM_RESULT_ERR_DATA_POINTER;
      break;
    }
    am_overlay_data_get_t *return_data = (am_overlay_data_get_t*)svc_ret->data;
    AM_STREAM_ID stream_id = AM_STREAM_ID(((am_overlay_id_t*)msg_data)
                                          ->stream_id);
    uint32_t area_id = ((am_overlay_id_t*)msg_data)->area_id;
    uint32_t index = ((am_overlay_id_t*)msg_data)->data_index[0];
    AMOverlayAreaData param;
    if (AM_UNLIKELY((svc_ret->ret = ol->get_area_data_param(
        stream_id, area_id, index, param)) != AM_RESULT_OK)) {
      break;
    }
    if (param.type == AM_OVERLAY_DATA_TYPE_NONE) {
      return_data->type = 255;
      break;
    }
    return_data->type = uint8_t(param.type);
    return_data->width = param.rect.size.width;
    return_data->height = param.rect.size.height;
    return_data->offset_x = param.rect.offset.x;
    return_data->offset_y = param.rect.offset.y;
    return_data->index = index;
    //Just provide basic information currently, all information can be writen
    //into a tmp file and return the file name
#if 0
    am_overlay_data_s data;
    if (AM_OVERLAY_DATA_TYPE_STRING == param.type) {
      am_overlay_str_data_t &str_data = data.str[0];
      str_data.width = param.rect.size.width;
      str_data.height = param.rect.size.height;
      str_data.offset_x = param.rect.offset.x;
      str_data.offset_y = param.rect.offset.y;
      AMOverlayTextBox text;
      text = param.text;
      str_data.spacing = text.spacing;
      AMOverlayCLUT c;
      c = text.background_color;
      str_data.bg_color = (c.v << 24) | (c.u << 16) | (c.y << 8) | c.a;
      uint32_t tmp = text.font_color.id;
      if (tmp < AM_OVERLAY_COLOR_NUM) {
        str_data.font_color = tmp;
      } else {
        c = text.font_color.color;
        str_data.font_color = (c.v << 24) | (c.u << 16) | (c.y << 8) | c.a;
      }
      AMOverlayFont &font = text.font;
      str_data.font_size = font.width;
      str_data.font_outline_w = font.outline_width;
      c = text.outline_color;
      str_data.font_outline_color = (c.v << 24) | (c.u << 16) | (c.y << 8) | c.a;
      str_data.font_hor_bold = font.hor_bold;
      str_data.font_ver_bold = font.ver_bold;
      str_data.font_italic = font.italic;
    } else if (AM_OVERLAY_DATA_TYPE_TIME == param.type) {
      am_overlay_time_data_t &time_data = data.time[0];
      time_data.width = param.rect.size.width;
      time_data.height = param.rect.size.height;
      time_data.offset_x = param.rect.offset.x;
      time_data.offset_y = param.rect.offset.y;
      AMOverlayTextBox text;
      time_data.msec_en = param.time.en_msec;
      time_data.time_format = param.time.format;
      time_data.is_12h = param.time.is_12h;
      text = param.time.text;
      time_data.spacing = text.spacing;
      AMOverlayCLUT c;
      c = text.background_color;
      time_data.bg_color = (c.v << 24) | (c.u << 16) | (c.y << 8) | c.a;
      uint32_t tmp = text.font_color.id;
      if (tmp < AM_OVERLAY_COLOR_NUM) {
        time_data.font_color = tmp;
      } else {
        c = text.font_color.color;
        time_data.font_color = (c.v << 24) | (c.u << 16) | (c.y << 8) | c.a;
      }
      AMOverlayFont &font = text.font;
      time_data.font_size = font.width;
      time_data.font_outline_w = font.outline_width;
      c = text.outline_color;
      time_data.font_outline_color = (c.v << 24) | (c.u << 16) | (c.y << 8) | c.a;
      time_data.font_hor_bold = font.hor_bold;
      time_data.font_ver_bold = font.ver_bold;
      time_data.font_italic = font.italic;
    } else if ((AM_OVERLAY_DATA_TYPE_PICTURE == param.type) ||
        (AM_OVERLAY_DATA_TYPE_ANIMATION == param.type)) {
      am_overlay_pic_data_t &pic_data = data.pic[0];
      pic_data.width = param.rect.size.width;
      pic_data.height = param.rect.size.height;
      pic_data.offset_x = param.rect.offset.x;
      pic_data.offset_y = param.rect.offset.y;
      AMOverlayPicture pic;
      if (AM_OVERLAY_DATA_TYPE_ANIMATION == param.type) {
        pic_data.bmp_num = param.anim.num;
        pic_data.interval = param.anim.interval;
        pic = param.anim.pic;
      } else {
        pic = param.pic;
      }
      const AMOverlayCLUT &c = param.pic.colorkey.color;
      pic_data.color_key = (c.v << 24) | (c.u << 16) | (c.y << 8) | c.a;
      pic_data.color_range = pic.colorkey.range;
    } else if (AM_OVERLAY_DATA_TYPE_LINE == param.type) {
      am_overlay_line_data_t &line_data = data.line[0];
      line_data.width = param.rect.size.width;
      line_data.height = param.rect.size.height;
      line_data.offset_x = param.rect.offset.x;
      line_data.offset_y = param.rect.offset.y;
      AMOverlayLine &line = param.line;
      uint32_t tmp = line.color.id;
      if (tmp < AM_OVERLAY_COLOR_NUM) {
        line_data.line_color = tmp;
      } else {
        const AMOverlayCLUT &c = line.color.color;
        line_data.line_color = (c.v << 24) | (c.u << 16) | (c.y << 8) | c.a;
      }
      line_data.line_tn = line.thickness;
      line_data.p_n = line.point.size();
      for (uint32_t n = 0; n < line_data.p_n; ++n) {
        line_data.p_x[n] = line.point[n].x;
        line_data.p_y[n] = line.point[n].y;
      }
    } else if (AM_OVERLAY_DATA_TYPE_RECTANGLE == param.type) {
      am_overlay_rtg_data_t &rtg = data.rtg[0];
      rtg.width = param.rect.size.width;
      rtg.height = param.rect.size.height;
      rtg.offset_x = param.rect.offset.x;
      rtg.offset_y = param.rect.offset.y;
      AMOverlayRectangle &rectangle = param.rtg;
      if (rectangle.h_rect.size() > 0) {
        rtg.rtg_type = 0;//hollow
        uint32_t tmp = rectangle.h_color.id;
        if (tmp < AM_OVERLAY_COLOR_NUM) {
          rtg.rtg_color = tmp;
        } else {
          const AMOverlayCLUT &c = rectangle.h_color.color;
          rtg.rtg_color = (c.v << 24) | (c.u << 16) | (c.y << 8) | c.a;
        }
        rtg.rtg_line_tn = rectangle.h_thickness;
        rtg.point_pair_num = rectangle.h_rect.size();
        for (uint32_t i = 0; i < rtg.point_pair_num; ++ i) {
          rtg.first_x[i] = (uint16_t)rectangle.h_rect[i].p1.x;
          rtg.first_y[i] = (uint16_t)rectangle.h_rect[i].p1.y;
          rtg.second_x[i] = (uint16_t)rectangle.h_rect[i].p2.x;
          rtg.second_y[i] = (uint16_t)rectangle.h_rect[i].p2.y;
        }
      } else if (rectangle.s_rect.size() > 0) {
        rtg.rtg_type = 1;//solid
        uint32_t tmp = rectangle.s_color.id;
        if (tmp < AM_OVERLAY_COLOR_NUM) {
          rtg.rtg_color = tmp;
        } else {
          const AMOverlayCLUT &c = rectangle.s_color.color;
          rtg.rtg_color = (c.v << 24) | (c.u << 16) | (c.y << 8) | c.a;
        }
        rtg.rtg_line_tn = rectangle.s_thickness;
        rtg.point_pair_num = rectangle.s_rect.size();
        for (uint32_t i = 0; i < rtg.point_pair_num; ++ i) {
          rtg.first_x[i] = rectangle.s_rect[i].x;
          rtg.first_y[i] = rectangle.s_rect[i].y;
        }
      }
    }
#endif
  } while(0);

  return;
}

void ON_VIDEO_EIS_SET(void *msg_data,
                      int msg_data_size,
                      void *result_addr,
                      int result_max_size)
{
  am_service_result_t *svc_ret = (am_service_result_t*)result_addr;
  memset(svc_ret, 0, sizeof(am_service_result_t));
  INFO("video service ON_VIDEO_EIS_SET!\n");

  do {
    am_encode_eis_ctrl_t *eis_config = nullptr;
    AMIEncodeEIS *eis =
        (AMIEncodeEIS*)g_video_camera->get_video_plugin(VIDEO_PLUGIN_EIS);
    int32_t eis_mode = 0;
    if (AM_UNLIKELY(!msg_data)) {
      ERROR("NULL pointer of msg_data!\n");
      svc_ret->ret = AM_RESULT_ERR_DATA_POINTER;
      break;
    }

    if (AM_UNLIKELY(!eis)) {
      WARN("Video Plugin %s is not loaded!", VIDEO_PLUGIN_EIS_SO);
      svc_ret->ret = AM_RESULT_ERR_PLUGIN_LOAD;
      break;
    }

    eis_config = (am_encode_eis_ctrl_t*)msg_data;
    if (TEST_BIT(eis_config->enable_bits, AM_ENCODE_EIS_MODE_EN_BIT)) {
      if (AM_UNLIKELY((svc_ret->ret = eis->get_eis_mode(eis_mode))
                      != AM_RESULT_OK)) {
        ERROR("Get EIS mode error");
        break;
      }
      if (AM_UNLIKELY((svc_ret->ret = eis->set_eis_mode(eis_config->eis_mode))
                      != AM_RESULT_OK)) {
        ERROR("Set EIS mode error");
        break;
      }
    }
    if (TEST_BIT(eis_config->enable_bits,AM_ENCODE_EIS_SAVE_CFG_EN_BIT)) {
      if (AM_UNLIKELY((svc_ret->ret = eis->save_config()) != AM_RESULT_OK)) {
        ERROR("Faile to save current eis config");
        break;
      }
    }
    if (AM_UNLIKELY((svc_ret->ret = eis->apply(HIGH_PRIORITY))
                    != AM_RESULT_OK)) {
      ERROR("Failed to apply eis parameters! Reset to last one!");
      if (TEST_BIT(eis_config->enable_bits, AM_ENCODE_EIS_MODE_EN_BIT)) {
        if (AM_UNLIKELY((svc_ret->ret = eis->set_eis_mode(eis_mode))
                        != AM_RESULT_OK)) {
          ERROR("Reset EIS mode error");
          break;
        }
      }
      break;
    }
  } while(0);
}

void ON_VIDEO_EIS_GET(void *msg_data,
                      int msg_data_size,
                      void *result_addr,
                      int result_max_size)
{
  INFO("video service ON_EIS_GET");
  am_service_result_t *svc_ret = (am_service_result_t*)result_addr;
  memset(svc_ret, 0, sizeof(am_service_result_t));
  am_encode_eis_ctrl_t *eis_config = (am_encode_eis_ctrl_t*)svc_ret->data;

  do {
    AMIEncodeEIS *eis =
        (AMIEncodeEIS*)g_video_camera->get_video_plugin(VIDEO_PLUGIN_EIS);

    if (AM_UNLIKELY(!eis)) {
      WARN("Video Plugin %s is not loaded!", VIDEO_PLUGIN_EIS_SO);
      svc_ret->ret = AM_RESULT_ERR_PLUGIN_LOAD;
      break;
    }

    if (AM_UNLIKELY((svc_ret->ret = eis->get_eis_mode(eis_config->eis_mode))
                    != AM_RESULT_OK)) {
      ERROR("Failed to get EIS mode!");
      break;
    }
  } while (0);

}

void ON_VIDEO_MOTION_DETECT_SET(void *msg_data,
                                int msg_data_size,
                                void *result_addr,
                                int result_max_size)
{
  INFO("video service set motion detect config\n");
  AM_RESULT ret = AM_RESULT_OK;

  do {
    if (!msg_data || msg_data_size == 0) {
      ret = AM_RESULT_ERR_DATA_POINTER;
      ERROR("Invalid parameters!");
      break;
    }

    am_video_md_config_s *md_config = (am_video_md_config_s*) msg_data;
    AMMDConfig config = {0};

    AMIVCA *vca_fw =
        (AMIVCA*)g_video_camera->get_video_plugin(VIDEO_PLUGIN_VCA);
    if (AM_UNLIKELY(!vca_fw)) {
      WARN("Video Plugin %s is not loaded\n", VIDEO_PLUGIN_VCA_SO);
      ret = AM_RESULT_ERR_PLUGIN_LOAD;
      break;
    }

    AMIMotionDetect *video_md =
        (AMIMotionDetect*)vca_fw->get_plugin_interface(VCA_PLUGIN_MD);
    if (AM_UNLIKELY(!video_md)) {
      WARN("VCA Plugin %s is not loaded\n", VCA_PLUGIN_MD_SO);
      ret = AM_RESULT_ERR_PLUGIN_LOAD;
      break;
    }

    if (TEST_BIT(md_config->enable_bits, AM_VIDEO_MD_CONFIG_ENABLE)) {
      config.key = AM_MD_ENABLE;
      config.value = malloc(sizeof(int32_t));
      *(uint32_t *)config.value = (uint32_t)md_config->enable;
      if (!video_md->set_config(&config)) {
        ERROR("failed to set video motion detect config\n");
        ret = AM_RESULT_ERR_INVALID;
        break;
      }
      free(config.value);
    }

    if (TEST_BIT(md_config->enable_bits, AM_VIDEO_MD_CONFIG_THRESHOLD0)) {
      config.key = AM_MD_THRESHOLD;
      config.value = malloc(sizeof(AMMDThreshold));
      ((AMMDThreshold *) config.value)->roi_id =
          md_config->threshold.roi_id;
      if (!video_md->get_config(&config)) {
        ERROR("failed to get video motion detect config\n");
        ret = AM_RESULT_ERR_INVALID;
        break;
      }
      ((AMMDThreshold *) config.value)->threshold[0] =
          md_config->threshold.threshold[0];
      if (!video_md->set_config(&config)) {
        ERROR("failed to set video motion detect config\n");
        ret = AM_RESULT_ERR_INVALID;
        break;
      }
      free(config.value);
    }

    if (TEST_BIT(md_config->enable_bits, AM_VIDEO_MD_CONFIG_THRESHOLD1)) {
      config.key = AM_MD_THRESHOLD;
      config.value = malloc(sizeof(AMMDThreshold));
      ((AMMDThreshold *) config.value)->roi_id =
          md_config->threshold.roi_id;
      if (!video_md->get_config(&config)) {
        ERROR("failed to get video motion detect config\n");
        ret = AM_RESULT_ERR_INVALID;
        break;
      }
      ((AMMDThreshold *) config.value)->threshold[1] =
          md_config->threshold.threshold[1];
      if (!video_md->set_config(&config)) {
        ERROR("failed to set video motion detect config\n");
        ret = AM_RESULT_ERR_INVALID;
        break;
      }
      free(config.value);
    }

    if (TEST_BIT(md_config->enable_bits,
                 AM_VIDEO_MD_CONFIG_LEVEL0_CHANGE_DELAY)) {
      config.key = AM_MD_LEVEL_CHANGE_DELAY;
      config.value = malloc(sizeof(AMMDLevelChangeDelay));
      ((AMMDLevelChangeDelay *) config.value)->roi_id =
          md_config->level_change_delay.roi_id;
      if (!video_md->get_config(&config)) {
        ERROR("failed to get video motion detect config\n");
        ret = AM_RESULT_ERR_INVALID;
        break;
      }
      ((AMMDLevelChangeDelay *) config.value)->mt_level_change_delay[0] =
          md_config->level_change_delay.lc_delay[0];
      if (!video_md->set_config(&config)) {
        ERROR("failed to set video motion detect config\n");
        ret = AM_RESULT_ERR_INVALID;
        break;
      }
      free(config.value);
    }

    if (TEST_BIT(md_config->enable_bits,
                 AM_VIDEO_MD_CONFIG_LEVEL1_CHANGE_DELAY)) {
      config.key = AM_MD_LEVEL_CHANGE_DELAY;
      config.value = malloc(sizeof(AMMDLevelChangeDelay));
      ((AMMDLevelChangeDelay *) config.value)->roi_id =
          md_config->level_change_delay.roi_id;
      if (!video_md->get_config(&config)) {
        ERROR("failed to get video motion detect config\n");
        ret = AM_RESULT_ERR_INVALID;
        break;
      }
      ((AMMDLevelChangeDelay *) config.value)->mt_level_change_delay[1] =
          md_config->level_change_delay.lc_delay[1];
      if (!video_md->set_config(&config)) {
        ERROR("failed to set video motion detect config\n");
        ret = AM_RESULT_ERR_INVALID;
        break;
      }
      free(config.value);
    }

    if (TEST_BIT(md_config->enable_bits, AM_VIDEO_MD_CONFIG_BUFFER_ID)) {
      config.key = AM_MD_BUFFER_ID;
      config.value = malloc(sizeof(int32_t));
      *(uint32_t *)config.value = md_config->buffer_id;
      if (!video_md->set_config(&config)) {
        ERROR("failed to set video motion detect config\n");
        ret = AM_RESULT_ERR_INVALID;
        break;
      }
      free(config.value);
    }

    if (TEST_BIT(md_config->enable_bits, AM_VIDEO_MD_CONFIG_BUFFER_TYPE)) {
      config.key = AM_MD_BUFFER_TYPE;
      config.value = malloc(sizeof(int32_t));
      *(uint32_t *)config.value = md_config->buffer_type;
      if (!video_md->set_config(&config)) {
        ERROR("failed to set video motion detect config\n");
        ret = AM_RESULT_ERR_INVALID;
        break;
      }
      free(config.value);
    }

    if (TEST_BIT(md_config->enable_bits, AM_VIDEO_MD_CONFIG_ROI)) {
      config.key = AM_MD_ROI;
      config.value = malloc(sizeof(AMMDRoi));
      ((AMMDRoi *)config.value)->roi_id = md_config->roi.roi_id;
      ((AMMDRoi *)config.value)->valid = md_config->roi.valid;
      ((AMMDRoi *)config.value)->left = md_config->roi.left;
      ((AMMDRoi *)config.value)->right = md_config->roi.right;
      ((AMMDRoi *)config.value)->top = md_config->roi.top;
      ((AMMDRoi *)config.value)->bottom = md_config->roi.bottom;
      if (!video_md->set_config(&config)) {
        ERROR("failed to set video motion detect config\n");
        ret = AM_RESULT_ERR_INVALID;
        break;
      }
      free(config.value);
    }

    //sync and save to config file
    if (TEST_BIT(md_config->enable_bits, AM_VIDEO_MD_SAVE_CURRENT_CONFIG)) {
      if (video_md->save_config() != AM_RESULT_OK) {
        ERROR("failed to save video motion detect config\n");
        ret = AM_RESULT_ERR_INVALID;
        break;
      }
    }

  } while (0);

  am_service_result_t *service_result = (am_service_result_t *) result_addr;
  service_result->ret = ret;
}

void ON_VIDEO_MOTION_DETECT_GET(void *msg_data,
                                int msg_data_size,
                                void *result_addr,
                                int result_max_size)
{
  INFO("event service get event motion detect config\n");
  AM_RESULT ret = AM_RESULT_OK;
  am_service_result_t *service_result = (am_service_result_t*) result_addr;
  memset(service_result, 0, sizeof(am_service_result_t));

  do {
    if (!msg_data || msg_data_size == 0) {
      ret = AM_RESULT_ERR_DATA_POINTER;
      ERROR("Invalid parameters!");
      break;
    }

    am_video_md_config_s *md_config = (am_video_md_config_s*) service_result
        ->data;
    AMMDConfig config =
    { 0 };
    uint32_t roi_id = *(uint32_t*) msg_data;
    AMIVCA *vca_fw =
        (AMIVCA*)g_video_camera->get_video_plugin(VIDEO_PLUGIN_VCA);
    if (AM_UNLIKELY(!vca_fw)) {
      WARN("Video Plugin %s is not loaded\n", VIDEO_PLUGIN_VCA_SO);
      ret = AM_RESULT_ERR_PLUGIN_LOAD;
      break;
    }

    AMIMotionDetect *video_md =
        (AMIMotionDetect*)vca_fw->get_plugin_interface(VCA_PLUGIN_MD);
    if (AM_UNLIKELY(!video_md)) {
      WARN("VCA Plugin %s is not loaded\n", VCA_PLUGIN_MD_SO);
      ret = AM_RESULT_ERR_PLUGIN_LOAD;
      break;
    }
    config.key = AM_MD_ENABLE;
    config.value = malloc(sizeof(bool));
    if (!video_md->get_config(&config)) {
      ERROR("failed to get video motion detect config\n");
      ret = AM_RESULT_ERR_INVALID;
      break;
    }
    md_config->enable = *(bool *) config.value;
    free(config.value);

    config.key = AM_MD_THRESHOLD;
    config.value = malloc(sizeof(AMMDThreshold));
    ((AMMDThreshold*) config.value)->roi_id = roi_id;
    if (!video_md->get_config(&config)) {
      ERROR("failed to get video motion detect config\n");
      ret = AM_RESULT_ERR_INVALID;
      break;
    }
    memcpy(&md_config->threshold, config.value, sizeof(AMMDThreshold));
    free(config.value);

    config.key = AM_MD_LEVEL_CHANGE_DELAY;
    config.value = malloc(sizeof(AMMDLevelChangeDelay));
    ((AMMDLevelChangeDelay*) config.value)->roi_id = roi_id;
    if (!video_md->get_config(&config)) {
      ERROR("failed to get video motion detect config\n");
      ret = AM_RESULT_ERR_INVALID;
      break;
    }
    memcpy(&md_config->level_change_delay,
           config.value,
           sizeof(AMMDLevelChangeDelay));
    free(config.value);

    config.key = AM_MD_BUFFER_ID;
    config.value = malloc(sizeof(int32_t));
    if (!video_md->get_config(&config)) {
      ERROR("failed to get video motion detect config\n");
      ret = AM_RESULT_ERR_INVALID;
      break;
    }
    md_config->buffer_id = *(uint32_t *) config.value;
    free(config.value);

    config.key = AM_MD_BUFFER_TYPE;
    config.value = malloc(sizeof(int32_t));
    if (!video_md->get_config(&config)) {
      ERROR("failed to get video motion detect config\n");
      ret = AM_RESULT_ERR_INVALID;
      break;
    }
    md_config->buffer_type = *(uint32_t *) config.value;
    free(config.value);

    config.key = AM_MD_ROI;
    config.value = malloc(sizeof(AMMDRoi));
    ((AMMDRoi *) config.value)->roi_id = roi_id;
    if (!video_md->get_config(&config)) {
      ERROR("failed to get video motion detect config\n");
      ret = AM_RESULT_ERR_INVALID;
      break;
    }
    md_config->roi.valid = ((AMMDRoi *) config.value)->valid;
    md_config->roi.left = ((AMMDRoi *) config.value)->left;
    md_config->roi.right = ((AMMDRoi *) config.value)->right;
    md_config->roi.top = ((AMMDRoi *) config.value)->top;
    md_config->roi.bottom = ((AMMDRoi *) config.value)->bottom;
    free(config.value);

  } while (0);

  service_result->ret = ret;
}

void ON_VIDEO_MOTION_DETECT_STOP(void *msg_data,
                                 int msg_data_size,
                                 void *result_addr,
                                 int result_max_size)
{
  INFO("on video service stop motion detect\n");
  AM_RESULT ret = AM_RESULT_OK;

  do {
    AMIVCA *vca_fw =
        (AMIVCA*)g_video_camera->get_video_plugin(VIDEO_PLUGIN_VCA);
    if (AM_UNLIKELY(!vca_fw)) {
      WARN("Video Plugin %s is not loaded\n", VIDEO_PLUGIN_VCA_SO);
      ret = AM_RESULT_ERR_PLUGIN_LOAD;
      break;
    }

    AMIMotionDetect *video_md =
        (AMIMotionDetect*)vca_fw->get_plugin_interface(VCA_PLUGIN_MD);
    if (AM_UNLIKELY(!video_md)) {
      WARN("VCA Plugin %s is not loaded\n", VCA_PLUGIN_MD_SO);
      ret = AM_RESULT_ERR_PLUGIN_LOAD;
      break;
    }

    if (!video_md->stop()) {
      ERROR("stop motion detect failed");
      ret = AM_RESULT_ERR_INVALID;
      break;
    }
  } while (0);

  am_service_result_t *service_result = (am_service_result_t *) result_addr;
  service_result->ret = ret;
}

void ON_VIDEO_MOTION_DETECT_START(void *msg_data,
                                  int msg_data_size,
                                  void *result_addr,
                                  int result_max_size)
{
  INFO("on video service start motion detect\n");
  AM_RESULT ret = AM_RESULT_OK;

  do {
    AMIVCA *vca_fw =
        (AMIVCA*)g_video_camera->get_video_plugin(VIDEO_PLUGIN_VCA);
    if (AM_UNLIKELY(!vca_fw)) {
      WARN("Video Plugin %s is not loaded\n", VIDEO_PLUGIN_VCA_SO);
      ret = AM_RESULT_ERR_PLUGIN_LOAD;
      break;
    }

    AMIMotionDetect *video_md =
        (AMIMotionDetect*)vca_fw->get_plugin_interface(VCA_PLUGIN_MD);
    if (AM_UNLIKELY(!video_md)) {
      WARN("VCA Plugin %s is not loaded\n", VCA_PLUGIN_MD_SO);
      ret = AM_RESULT_ERR_PLUGIN_LOAD;
      break;
    }

    if (!video_md->start()) {
      ERROR("start motion detect failed");
      ret = AM_RESULT_ERR_INVALID;
      break;
    }
  } while (0);

  am_service_result_t *service_result = (am_service_result_t *) result_addr;
  service_result->ret = ret;
}

void ON_VIDEO_FACE_RECOGNITION_START(void *msg_data,
                                     int msg_data_size,
                                     void *result_addr,
                                     int result_max_size)
{
  INFO("on video service start face recognition\n");
  AM_RESULT ret = AM_RESULT_OK;

  do {
    AMIVCA *vca_fw =
        (AMIVCA*)g_video_camera->get_video_plugin(VIDEO_PLUGIN_VCA);
    if (AM_UNLIKELY(!vca_fw)) {
      WARN("Video Plugin %s is not loaded\n", VIDEO_PLUGIN_VCA_SO);
      ret = AM_RESULT_ERR_PLUGIN_LOAD;
      break;
    }

    AMIFaceDetect *video_fd =
        (AMIFaceDetect*)vca_fw->get_plugin_interface(VCA_PLUGIN_FD);
    if (AM_UNLIKELY(!video_fd)) {
      WARN("VCA Plugin %s is not loaded\n", VCA_PLUGIN_FD_SO);
      ret = AM_RESULT_ERR_PLUGIN_LOAD;
      break;
    }

    if (!video_fd->enable_recognition(true)) {
      ERROR("start face recognition failed");
      ret = AM_RESULT_ERR_INVALID;
      break;
    }
  } while (0);

  am_service_result_t *service_result = (am_service_result_t *) result_addr;
  service_result->ret = ret;

}

void ON_VIDEO_FACE_RECOGNITION_STOP(void *msg_data,
                                     int msg_data_size,
                                     void *result_addr,
                                     int result_max_size)
{
  INFO("on video service stop face recognition\n");
  AM_RESULT ret = AM_RESULT_OK;

  do {
    AMIVCA *vca_fw =
        (AMIVCA*)g_video_camera->get_video_plugin(VIDEO_PLUGIN_VCA);
    if (AM_UNLIKELY(!vca_fw)) {
      WARN("Video Plugin %s is not loaded\n", VIDEO_PLUGIN_VCA_SO);
      ret = AM_RESULT_ERR_PLUGIN_LOAD;
      break;
    }

    AMIFaceDetect *video_fd =
        (AMIFaceDetect*)vca_fw->get_plugin_interface(VCA_PLUGIN_FD);
    if (AM_UNLIKELY(!video_fd)) {
      WARN("VCA Plugin %s is not loaded\n", VCA_PLUGIN_FD_SO);
      ret = AM_RESULT_ERR_PLUGIN_LOAD;
      break;
    }

    if (!video_fd->enable_recognition(false)) {
      ERROR("start face recognition failed");
      ret = AM_RESULT_ERR_INVALID;
      break;
    }
  } while (0);

  am_service_result_t *service_result = (am_service_result_t *) result_addr;
  service_result->ret = ret;

}

