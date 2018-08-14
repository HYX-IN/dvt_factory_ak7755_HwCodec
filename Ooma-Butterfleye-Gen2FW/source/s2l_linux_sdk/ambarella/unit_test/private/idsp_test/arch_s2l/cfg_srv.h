/*
 *
 * idsp_test
 *
 * History:
 *	2015/09/10 - [Jingyang Qiu] Created this file
 *
 * Description :
 *	Load the tuned file which generated by Img tuning tool.
 *
 * Copyright (C) 2015 Ambarella, Inc.
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
 */

#ifndef CFG_SRV_H
#define CFG_SRV_H

//str field must be the last one, and only 1 str field is allowed, since "get" uses int_ptr++
typedef struct{
	int sensor_id;
	int video_mode;
	int hdr_mode;
	int width;
	int height;
	int fps;
	int bit_resolution;
	int mirror;
	int bayer_pattern;
	char sensor_name[64];
}vin_t;

typedef struct{
	int enable;
	int input;
	int mode;
	int width;
	int height;
	int device;
	int lcd_model_index;
}vout_t;

typedef struct{
	int from_mixer_a;
	int from_mixer_b;
}osd_t;

typedef struct{
	int enc_mode;
	int raw2enc;
	int raw_cap;
	int raw_max_width;
	int raw_max_height;
	int exposure_num;
	int duration;
	char next_cfg_file[64];
}pipeline_t;

typedef struct{
	int input_width;
	int input_height;
	int input_offset_x;
	int input_offset_y;
	int max_width;
	int max_height;
	int width;
	int height;
	int type;
}buffer_t;

typedef struct{
	int width;
	int height;
	int max_width;
	int max_height;
	int m;
	int n;
	int bit_rate;
	int quality;
	int h_flip;
	int v_flip;
	int rotate;
	int fps_ratio;
	int source;
	int type;
}stream_t;

#endif
