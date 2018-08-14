/*******************************************************************************
 * am_efm_src_service_msg_map.h
 *
 * History:
 *   2016-05-04 - [Zhi He] created file
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


#ifndef AM_EFM_SRC_SERVICE_MSG_MAP_H_
#define AM_EFM_SRC_SERVICE_MSG_MAP_H_
#include "commands/am_api_cmd_efm_src.h"
void ON_SERVICE_INIT(void *msg_data,
                     int msg_data_size,
                     void *result_addr,
                     int result_max_size);
void ON_SERVICE_DESTROY(void *msg_data,
                     int msg_data_size,
                     void *result_addr,
                     int result_max_size);
void ON_SERVICE_START(void *msg_data,
                     int msg_data_size,
                     void *result_addr,
                     int result_max_size);
void ON_SERVICE_STOP(void *msg_data,
                     int msg_data_size,
                     void *result_addr,
                     int result_max_size);
void ON_SERVICE_RESTART(void *msg_data,
                     int msg_data_size,
                     void *result_addr,
                     int result_max_size);
void ON_SERVICE_STATUS(void *msg_data,
                     int msg_data_size,
                     void *result_addr,
                     int result_max_size);
void ON_FEED_EFM_FROM_LOCAL_ES_FILE(void *msg_data,
                     int  msg_data_size,
                     void *result_addr,
                     int  result_max_size);
void ON_FEED_EFM_FROM_USB_CAMERA(void *msg_data,
                     int  msg_data_size,
                     void *result_addr,
                     int  result_max_size);
void ON_END_FEED_EFM(void *msg_data,
                     int  msg_data_size,
                     void *result_addr,
                     int  result_max_size);

void ON_SETUP_YUV_CAPTURE_PIPELINE(void *msg_data,
                          int msg_data_size,
                          void *result_addr,
                          int result_max_size);

void ON_DESTROY_YUV_CAPTURE_PIPELINE(void *msg_data,
                          int msg_data_size,
                          void *result_addr,
                          int result_max_size);

void ON_YUV_CAPTURE_FOR_EFM(void *msg_data,
                          int msg_data_size,
                          void *result_addr,
                          int result_max_size);

BEGIN_MSG_MAP(API_PROXY_TO_EFM_SRC_SERVICE)
MSG_ACTION(AM_IPC_SERVICE_INIT, ON_SERVICE_INIT)
MSG_ACTION(AM_IPC_SERVICE_DESTROY, ON_SERVICE_DESTROY)
MSG_ACTION(AM_IPC_SERVICE_START, ON_SERVICE_START)
MSG_ACTION(AM_IPC_SERVICE_STOP, ON_SERVICE_STOP)
MSG_ACTION(AM_IPC_SERVICE_RESTART, ON_SERVICE_RESTART)
MSG_ACTION(AM_IPC_SERVICE_STATUS, ON_SERVICE_STATUS)
MSG_ACTION(AM_IPC_MW_CMD_FEED_EFM_FROM_LOCAL_ES_FILE, ON_FEED_EFM_FROM_LOCAL_ES_FILE)
MSG_ACTION(AM_IPC_MW_CMD_FEED_EFM_FROM_USB_CAMERA, ON_FEED_EFM_FROM_USB_CAMERA)
MSG_ACTION(AM_IPC_MW_CMD_END_FEED_EFM, ON_END_FEED_EFM)
MSG_ACTION(AM_IPC_MW_CMD_SETUP_YUV_CAPTURE_PIPELINE, ON_SETUP_YUV_CAPTURE_PIPELINE)
MSG_ACTION(AM_IPC_MW_CMD_DESTROY_YUV_CAPTURE_PIPELINE, ON_DESTROY_YUV_CAPTURE_PIPELINE)
MSG_ACTION(AM_IPC_MW_CMD_YUV_CAPTURE_FOR_EFM, ON_YUV_CAPTURE_FOR_EFM)
END_MSG_MAP()

#endif /* AM_EFM_SRC_SERVICE_MSG_MAP_H_ */
