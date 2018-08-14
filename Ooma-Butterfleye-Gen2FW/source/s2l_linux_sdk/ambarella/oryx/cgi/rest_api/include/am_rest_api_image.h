/**
 * am_rest_api_image.h
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
#ifndef ORYX_CGI_INCLUDE_AM_REST_API_IMAGE_H_
#define ORYX_CGI_INCLUDE_AM_REST_API_IMAGE_H_

#include "am_rest_api_handle.h"

class AMRestAPIImage: public AMRestAPIHandle
{
  public:
    virtual AM_REST_RESULT  rest_api_handle();

  private:
    AM_REST_RESULT  image_style_handle();
    AM_REST_RESULT  image_denoise_handle();
    AM_REST_RESULT  image_ae_handle();
    AM_REST_RESULT  image_awb_handle();
    AM_REST_RESULT  image_af_handle();

  private:
    AM_REST_RESULT  style_set_handle();
    AM_REST_RESULT  style_get_handle();
    AM_REST_RESULT  denoise_set_handle();
    AM_REST_RESULT  denoise_get_handle();
    AM_REST_RESULT  ae_set_handle();
    AM_REST_RESULT  ae_get_handle();
    AM_REST_RESULT  awb_set_handle();
    AM_REST_RESULT  awb_get_handle();
    AM_REST_RESULT  af_set_handle();
    AM_REST_RESULT  af_get_handle();
};

#endif /* ORYX_CGI_INCLUDE_AM_REST_API_IMAGE_H_ */
