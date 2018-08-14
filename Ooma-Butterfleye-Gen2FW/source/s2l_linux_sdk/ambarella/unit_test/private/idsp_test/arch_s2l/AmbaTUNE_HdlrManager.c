/*
 *
 * idsp_test
 *
 * History:
 *	2013/09/05 - [Dylan] Created this file
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

#include <stdio.h>
#include "AmbaTUNE_TextHdlr.h" // Note: Tune_Func_s Need
#include "AmbaTUNE_HdlrManager.h"

typedef struct _AMBA_KAL_MUTEX_t_ {
    unsigned char Reserved[52];
} AMBA_KAL_MUTEX_t;
#define AmbaPrint(format, arg...)	printf(format"\n", ##arg)

#define MNG_DEBF(fmt, arg...) AmbaPrint("[TUNE MNG][DEBUG]"fmt, ##arg)
#define MNG_WARF(fmt, arg...) AmbaPrint("[TUNE MNG][WARNING]"fmt, ##arg)
#define MNG_ERRF(fmt, arg...) AmbaPrint("[TUNE MNG][ERROR]"fmt, ##arg)

#define OSAL_MUTEX_CREATE()// do {AmbaKAL_MutexCreate(gTune_Hdlr.Parser_Mutex);} while (0)
#define OSAL_MUTEX_LOCK()// do {AmbaKAL_MutexTake(gTune_Hdlr.Parser_Mutex, AMBA_KAL_WAIT_FOREVER);} while(0)
#define OSAL_MUTEX_UNLOCK()// do {AmbaKAL_MutexGive(gTune_Hdlr.Parser_Mutex);} while(0)

#ifndef NULL
#define NULL (void*)0
#endif

typedef struct {
    int (*Init)(void);
    int (*Save_IDSP)(amba_img_dsp_mode_cfg_t Mode, char *Filepath);
    int (*Load_IDSP)(char *Filepath);
    int (*get_ituner_stream)(char* ituner_stream);
    int (*set_ituner_stream)(u8* ituner_stream,int size);
    int (*Save_Data)(Ituner_Ext_File_Type_e Ext_File_Type, Ituner_Ext_File_Param_s *Ext_File_Param);
    int (*Load_Data)(Ituner_Ext_File_Type_e Ext_File_Type, Ituner_Ext_File_Param_s *Ext_File_Param);
    int (*Execute_IDSP)(int fd_iav, amba_img_dsp_mode_cfg_t Mode, AMBA_ITUNER_PROC_INFO_s *ProcInfo);
    int (*Get_SystemInfo)(ITUNER_SYSTEM_s *System);
    int (*Set_SystemInfo)(ITUNER_SYSTEM_s *System);
    int (*Get_ItunerInfo)(ITUNER_INFO_s *ItunerInfo);
} Tune_Func_s;


static int _Fake_USBHdlr_Init(void){
    MNG_ERRF("%s() %d, Not yet Implement", __func__, __LINE__);
    return 0;
}
static int _Fake_USBHdlr_Save_IDSP(amba_img_dsp_mode_cfg_t Mode, char *Filepath){
    MNG_ERRF("%s() %d, Not yet Implement", __func__, __LINE__);
    return 0;
}
static int _Fake_USBHdlr_Load_IDSP(char *Filepath){
    MNG_ERRF("%s() %d, Not yet Implement", __func__, __LINE__);
    return 0;
}
static int _Fake_USBHdlr_Save_Data(Ituner_Ext_File_Type_e Ext_File_Type, Ituner_Ext_File_Param_s *Ext_File_Param){
    MNG_ERRF("%s() %d, Not yet Implement", __func__, __LINE__);
    return 0;
}
static int _Fake_USBHdlr_Load_Data(Ituner_Ext_File_Type_e Ext_File_Type, Ituner_Ext_File_Param_s *Ext_File_Param){
    MNG_ERRF("%s() %d, Not yet Implement", __func__, __LINE__);
    return 0;
}
static int _Fake_USBHdlr_Execute_IDSP(int fd_iav, amba_img_dsp_mode_cfg_t Mode, AMBA_ITUNER_PROC_INFO_s *ProcInfo){
    MNG_ERRF("%s() %d, Not yet Implement", __func__, __LINE__);
    return 0;
}
static int _Fake_USBHdlr_Get_SystemInfo(ITUNER_SYSTEM_s *System){
    MNG_ERRF("%s() %d, Not yet Implement", __func__, __LINE__);
    return 0;
}
static int _Fake_USBHdlr_Set_SystemInfo(ITUNER_SYSTEM_s *System){
    MNG_ERRF("%s() %d, Not yet Implement", __func__, __LINE__);
    return 0;
}
static int _Fake_USBHdlr_Get_ItunerInfo(ITUNER_INFO_s *ItunerInfo){
    MNG_ERRF("%s() %d, Not yet Implement", __func__, __LINE__);
    return 0;
}

static const Tune_Func_s gUSBHdlr_Func = {
    .Init = _Fake_USBHdlr_Init,
    .Save_IDSP = _Fake_USBHdlr_Save_IDSP,
    .Load_IDSP = _Fake_USBHdlr_Load_IDSP,
    .Save_Data = _Fake_USBHdlr_Save_Data,
    .Load_Data = _Fake_USBHdlr_Load_Data,
    .Execute_IDSP = _Fake_USBHdlr_Execute_IDSP,
    .Get_SystemInfo = _Fake_USBHdlr_Get_SystemInfo,
    .Set_SystemInfo = _Fake_USBHdlr_Set_SystemInfo,
    .Get_ItunerInfo = _Fake_USBHdlr_Get_ItunerInfo,
};

static const Tune_Func_s gTextHdlr_Func = {
    .Init = TextHdlr_Init,
    .Save_IDSP = TextHdlr_Save_IDSP,
    .Load_Data = TextHdlr_Load_Data,
    .get_ituner_stream =TextHdlr_Load_ituner,
    .set_ituner_stream =TextHdlr_set_ituner,
    .Save_Data = TextHdlr_Save_Data,
    .Load_IDSP = TextHdlr_Load_IDSP,
    .Execute_IDSP = TextHdlr_Execute_IDSP,
    .Get_SystemInfo = TextHdlr_Get_SystemInfo,
    .Set_SystemInfo = TextHdlr_Set_SystemInfo,
    .Get_ItunerInfo = TextHdlr_Get_ItunerInfo,
};

typedef enum {
    TUNE_VALID = 0,
    TUNE_INVALID,
} TuneHdlr_Status_e;
typedef struct {
    TuneHdlr_Mode_e Active_Parser_Mode;
    TuneHdlr_Status_e Parser_Status;
    AMBA_KAL_MUTEX_t *Parser_Mutex;
} TuneHdlr_s;
static const Tune_Func_s *gTune_Func[MAX_TUNE] = {
    &gTextHdlr_Func,
    &gUSBHdlr_Func,
};
static AMBA_KAL_MUTEX_t gTune_Mutex;
static TuneHdlr_s gTune_Hdlr = {
    .Active_Parser_Mode = TEXT_TUNE,
    .Parser_Status = TUNE_INVALID,
    .Parser_Mutex = NULL,
};

static void _AmbaTUNE_Check_Mutex_Created(void)
{
    if (gTune_Hdlr.Parser_Mutex == NULL) {
        MNG_DEBF("%s() %d, Create Mutex", __func__, __LINE__);
        gTune_Hdlr.Parser_Mutex = &gTune_Mutex;
        OSAL_MUTEX_CREATE();
    }
}

int AmbaTUNE_Init(void)
{
    int Ret = 0;
    _AmbaTUNE_Check_Mutex_Created();
    OSAL_MUTEX_LOCK();
    gTune_Hdlr.Parser_Status = TUNE_VALID;
    Ret = gTune_Func[gTune_Hdlr.Active_Parser_Mode]->Init();
    OSAL_MUTEX_UNLOCK();
    return Ret;
}

void AmbaTUNE_Change_Parser_Mode(TuneHdlr_Mode_e Parser_Mode)
{
    _AmbaTUNE_Check_Mutex_Created();
    OSAL_MUTEX_LOCK();
    do {
        if (gTune_Hdlr.Active_Parser_Mode == Parser_Mode) {
            MNG_DEBF("%s() %d, Active Parser = %d Do not need to change parser mode", __func__, __LINE__, gTune_Hdlr.Active_Parser_Mode);
            break;
        }
	gTune_Hdlr.Active_Parser_Mode = Parser_Mode;
        gTune_Hdlr.Parser_Status = TUNE_INVALID;
    } while (0);
    OSAL_MUTEX_UNLOCK();

}
int AmbaTUNE_Save_IDSP(amba_img_dsp_mode_cfg_t Mode, char *Filepath)
{
    int Ret = 0;
    _AmbaTUNE_Check_Mutex_Created();
    OSAL_MUTEX_LOCK();
    do {
        if (gTune_Hdlr.Parser_Status == TUNE_INVALID) {
            MNG_WARF("%s() %d, Parser_Status Invalid", __func__, __LINE__);
            Ret = -1;
            break;
        }
        Ret = gTune_Func[gTune_Hdlr.Active_Parser_Mode]->Save_IDSP(Mode, Filepath);
    } while (0);
    OSAL_MUTEX_UNLOCK();
    return Ret;
}
int AmbaTUNE_load_ituner_stream(char *ituner_stream)
{
    int Ret = 0;
    _AmbaTUNE_Check_Mutex_Created();
    OSAL_MUTEX_LOCK();
    do {
        if (gTune_Hdlr.Parser_Status == TUNE_INVALID) {
            MNG_WARF("%s() %d, Parser_Status Invalid", __func__, __LINE__);
            Ret = -1;
            break;
        }
        Ret = gTune_Func[gTune_Hdlr.Active_Parser_Mode]->get_ituner_stream(ituner_stream);
    } while (0);
    OSAL_MUTEX_UNLOCK();
    return Ret;
}

int AmbaTUNE_Load_IDSP(char *Filepath)
{
    int Ret = 0;
    _AmbaTUNE_Check_Mutex_Created();
    OSAL_MUTEX_LOCK();
    do {
        if (gTune_Hdlr.Parser_Status == TUNE_INVALID) {
            MNG_WARF("%s() %d, Parser_Status Invalid", __func__, __LINE__);
            Ret = -1;
            break;
        }
        Ret = gTune_Func[gTune_Hdlr.Active_Parser_Mode]->Load_IDSP(Filepath);
    } while (0);
    OSAL_MUTEX_UNLOCK();
    return Ret;
}
int AmbaTUNE_apply_ituner_stream(u8* ituner_stream,int size)
{
    int Ret = 0;
    _AmbaTUNE_Check_Mutex_Created();
    OSAL_MUTEX_LOCK();
    do {
        if (gTune_Hdlr.Parser_Status == TUNE_INVALID) {
            MNG_WARF("%s() %d, Parser_Status Invalid", __func__, __LINE__);
            Ret = -1;
            break;
        }
        Ret = gTune_Func[gTune_Hdlr.Active_Parser_Mode]->set_ituner_stream(ituner_stream,size);
    } while (0);
    OSAL_MUTEX_UNLOCK();
    return Ret;
}

int AmbaTUNE_Save_Data(Ituner_Ext_File_Type_e Ext_File_Type, Ituner_Ext_File_Param_s *Ext_File_Param)
{
    int Ret = 0;
    _AmbaTUNE_Check_Mutex_Created();
    OSAL_MUTEX_LOCK();
    do {
        if (gTune_Hdlr.Parser_Status == TUNE_INVALID) {
            MNG_WARF("%s() %d, Parser_Status Invalid", __func__, __LINE__);
            Ret = -1;
            break;
        }
        Ret = gTune_Func[gTune_Hdlr.Active_Parser_Mode]->Save_Data(Ext_File_Type, Ext_File_Param);
    } while (0);
    OSAL_MUTEX_UNLOCK();
    return Ret;
}

int AmbaTUNE_Load_Data(Ituner_Ext_File_Type_e Ext_File_Type, Ituner_Ext_File_Param_s *Ext_File_Param)
{
    int Ret = 0;
    _AmbaTUNE_Check_Mutex_Created();
    OSAL_MUTEX_LOCK();
    do {
        if (gTune_Hdlr.Parser_Status == TUNE_INVALID) {
            MNG_WARF("%s() %d, Parser_Status Invalid", __func__, __LINE__);
            Ret = -1;
            break;
        }
        Ret = gTune_Func[gTune_Hdlr.Active_Parser_Mode]->Load_Data(Ext_File_Type, Ext_File_Param);
    } while(0);
    OSAL_MUTEX_UNLOCK();
    return Ret;
}

int AmbaTUNE_Execute_IDSP(int fd_iav, amba_img_dsp_mode_cfg_t Mode, AMBA_ITUNER_PROC_INFO_s *ProcInfo)
{
    int Ret = 0;
    _AmbaTUNE_Check_Mutex_Created();
    OSAL_MUTEX_LOCK();
    do {
        if (gTune_Hdlr.Parser_Status == TUNE_INVALID) {
            MNG_WARF("%s() %d, Parser_Status Invalid", __func__, __LINE__);
            Ret = -1;
            break;
        }
        Ret = gTune_Func[gTune_Hdlr.Active_Parser_Mode]->Execute_IDSP(fd_iav, Mode, ProcInfo);
    } while(0);
    OSAL_MUTEX_UNLOCK();
    return Ret;
}

int AmbaTUNE_Get_SystemInfo(ITUNER_SYSTEM_s *System)
{
    int Ret = 0;
    _AmbaTUNE_Check_Mutex_Created();
    OSAL_MUTEX_LOCK();
    do {
        if (gTune_Hdlr.Parser_Status == TUNE_INVALID) {
            MNG_WARF("%s() %d, Parser_Status Invalid", __func__, __LINE__);
            Ret = -1;
            break;
        }
        Ret = gTune_Func[gTune_Hdlr.Active_Parser_Mode]->Get_SystemInfo(System);
    } while(0);
    OSAL_MUTEX_UNLOCK();
    return Ret;
}

int AmbaTUNE_Set_SystemInfo(ITUNER_SYSTEM_s *System)
{
    int Ret = 0;
    _AmbaTUNE_Check_Mutex_Created();
    OSAL_MUTEX_LOCK();
    do {
        if (gTune_Hdlr.Parser_Status == TUNE_INVALID) {
            MNG_WARF("%s() %d, Parser_Status Invalid", __func__, __LINE__);
            Ret = -1;
            break;
        }
        Ret = gTune_Func[gTune_Hdlr.Active_Parser_Mode]->Set_SystemInfo(System);
    } while (0);
    OSAL_MUTEX_UNLOCK();
    return Ret;
}

int AmbaTUNE_Get_ItunerInfo(ITUNER_INFO_s *ItunerInfo)
{
    int Ret = 0;
    _AmbaTUNE_Check_Mutex_Created();
    OSAL_MUTEX_LOCK();
    do {
        if (gTune_Hdlr.Parser_Status == TUNE_INVALID) {
            MNG_WARF("%s() %d, Parser_Status Invalid", __func__, __LINE__);
            Ret = -1;
            break;
        }
        Ret = gTune_Func[gTune_Hdlr.Active_Parser_Mode]->Get_ItunerInfo(ItunerInfo);
    } while (0);
    OSAL_MUTEX_UNLOCK();
    return Ret;
}
