/*******************************************************************************
 * am_mp_wifi_stat.h
 *
 * History:
 *   Mar 18, 2015 - [longli] created file
 *
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
 */


#ifndef _AM_MP_WIFI_STAT_H_
#define _AM_MP_WIFI_STAT_H_

#define BUFF_LEN 1000
#define CMD_LEN 300


enum {
    CMD_SETUP = 0,
    CMD_SCAN,
    CMD_CONNECT,
    CMD_GET_COUNTRY_CODE,
    CMD_SET_COUNTRY_CODE,
    CMD_SIGNAL_STRENGTH,
    CMD_THROUGHPUT_TEST
}WifiCommand;

enum {
    WIFI_INTF_NOT_UP = 0,
    WIFI_INTF_UP,
    WIFI_AP_CONNECTED,
};

typedef struct {
    char ssid[100];
    char psswd[100];
}tConnectAp;

typedef struct wifi_state {
    int state;
    char ssid_name[100];
    char password[100];
    char ip_addr[100];
    char netmask[100];
}tWifiState;

/* Handler to process request relating to wifi issues from client. */
am_mp_err_t mptool_wifi_test_handler(am_mp_msg_t *from, am_mp_msg_t *to);

#endif /* _AM_WIFI_STAT_H_ */
