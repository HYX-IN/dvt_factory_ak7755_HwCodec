/*******************************************************************************
 * am_muxer_if.h
 *
 * History:
 *   2014-12-29 - [ypchang] created file
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
#ifndef ORYX_STREAM_INCLUDE_RECORD_AM_MUXER_IF_H_
#define ORYX_STREAM_INCLUDE_RECORD_AM_MUXER_IF_H_
#include "am_base_include.h"
#include "am_record_msg.h"

extern const AM_IID IID_AMIMuxer;

enum AM_MUXER_TYPE
{
  AM_MUXER_TYPE_NONE,
  AM_MUXER_TYPE_DIRECT,
  AM_MUXER_TYPE_FILE,
};
struct AMImage3AInfo;
struct AMEventStruct;
class AMIMuxer: public AMIInterface
{
  public:
    DECLARE_INTERFACE(AMIMuxer, IID_AMIMuxer);
    virtual AM_STATE start()                                                = 0;
    virtual AM_STATE stop()                                                 = 0;
    virtual bool start_file_recording(uint32_t muxer_id_bit_map)            = 0;
    virtual bool stop_file_recording(uint32_t muxer_id_bit_map)             = 0;
    virtual bool set_file_operation_callback(uint32_t muxer_id_bit_map,
                                             AM_FILE_OPERATION_CB_TYPE type,
                                             AMFileOperationCB callback)    = 0;
    virtual bool set_muxer_param(AMMuxerParam &param)                       = 0;
    virtual bool update_image_3a_info(AMImage3AInfo *image_3a)              = 0;
    virtual bool start_send_normal_pkt()                                    = 0;
    virtual bool is_ready_for_event(AMEventStruct& event)                   = 0;
    virtual uint32_t version()                                              = 0;
    virtual uint32_t get_video_id()                                         = 0;
    virtual AM_MUXER_TYPE type()                                            = 0;
};

#ifdef __cplusplus
extern "C" {
#endif
ORYX_API AMIInterface *create_filter(AMIEngine *engine, const char *config,
                                       uint32_t input_num, uint32_t output_num);
#ifdef __cplusplus
}
#endif

#endif /* ORYX_STREAM_INCLUDE_RECORD_AM_MUXER_IF_H_ */
