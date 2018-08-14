/*******************************************************************************
 * lib_smartrc.h
 *
 * History:
 *   2015/11/23 - [ypxu] created file
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
#ifndef _LIB_SMARTRC_H_
#define _LIB_SMARTRC_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "basetypes.h"
#include "lib_smartrc_common.h"
#include "mdet_diff.h"

AMBA_API int smartrc_init(init_t *rc_init);	//init smart rate control library with iav file handle and etc.
AMBA_API int smartrc_param_config(param_config_t *config);	//init smart rate control library parameters, must call before auto run this library
AMBA_API int smartrc_deinit(void);	//deinit smart rate control library
AMBA_API int smartrc_set_base_param(u32 gop_type, base_param_t *base_param,
		u32 quality_map, u32 param_map, u32 profile_map);
AMBA_API int smartrc_get_base_param(u32 gop_type, base_param_t *base_param,
		u32 quality_map, u32 param_map, u32 profile_map);
AMBA_API int smartrc_dynamic_gop_enable(u8 enable);
AMBA_API int smartrc_apply_sync_frame(int pts);
/* All smartrc APIs can be called on the fly
 * If IAV state is not right, then the IAV call will fail. but no harm
 */
AMBA_API int smartrc_set_stream_quality(quality_level_t quality_level, u32 stream_id);
AMBA_API int smartrc_get_stream_quality(quality_level_t *quality_level, u32 stream_id);
AMBA_API int smartrc_set_bitrate_target(bitrate_target_t *bitrate_target, u32 stream_id);
AMBA_API int smartrc_get_bitrate_target(bitrate_target_t *bitrate_target, u32 stream_id);
AMBA_API int smartrc_set_style(style_t style, u32 stream_id);	//let smartrc to setup stream size and etc.
AMBA_API int smartrc_get_style(style_t *style, u32 stream_id);
AMBA_API int smartrc_set_motion_level(motion_level_t motion_level, u32 stream_id); //update smartrc about motion level
AMBA_API int smartrc_get_motion_level(motion_level_t *motion_level, u32 stream_id);
AMBA_API int smartrc_set_noise_level(noise_level_t noise_level, u32 stream_id);
AMBA_API int smartrc_get_noise_level(noise_level_t *noise_level, u32 stream_id);
AMBA_API int smartrc_set_log_level(u32 log);
AMBA_API int smartrc_get_log_level(u32 *plog);
AMBA_API int smartrc_get_para_cfg(void *buffer, u32 buf_size, u32 dsp_pts, u32 stream_id);
AMBA_API int smartrc_get_version(version_t *version);

//smartrc roi APIs
AMBA_API int smartrc_start_roi(roi_session_t *session, u32 stream_id);
AMBA_API int smartrc_stop_roi(roi_session_t *session, u32 stream_id);
AMBA_API int smartrc_update_roi(roi_session_t *session, u32 stream_id);
AMBA_API int smartrc_apply_sync_frame(int pts);

#ifdef __cplusplus
}
#endif

#endif /* _LIB_SMARTRC_H_ */
