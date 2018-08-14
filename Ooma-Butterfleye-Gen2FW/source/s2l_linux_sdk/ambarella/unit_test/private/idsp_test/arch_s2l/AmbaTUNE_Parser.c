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
#include "AmbaTUNE_Parser.h"
#include "stdio.h" // sprintf need
#include "string.h"
#include "stdlib.h"
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

//#include "AmbaPrint.h"

#define AmbaPrint(format, arg...)	printf(format"\n", ##arg)
#define PASER_DEBF(fmt, arg...) AmbaPrint("[PASER][DEBUG]"fmt, ##arg)
#define PASER_WARF(fmt, arg...) AmbaPrint("[PASER][WARNING]"fmt, ##arg)
#define PASER_ERRF(fmt, arg...) AmbaPrint("[PASER][ERROR]"fmt, ##arg)

#define MAX_REGS (200)
#ifndef NULL
    #define NULL                  ((void*)0)
#endif

static REG_s* _Reg_List[MAX_REGS];
static char LineBuf[MAX_LINE_CHARS];
static TUNE_Parser_Object_t Parser_Object[1];
static OpMode_e gOpMode;
typedef void (*pfunc_conv_t)(int, void*, void*);

static void _str_to_str(int N, void *Src, void *Dst);
static void _str_to_u8(int N, void *Src, void *Dst);
static void _str_to_s8(int N, void *Src, void *Dst);
static void _str_to_u16(int N, void *Src, void *Dst);
static void _str_to_s16(int N, void *Src, void *Dst);
static void _str_to_u32(int N, void *Src, void *Dst);
static void _str_to_s32(int N, void *Src, void *Dst);
static void _u8_to_str(int N, void *Src, void *Dst);
static void _s8_to_str(int N, void *Src, void *Dst);
static void _u16_to_str(int N, void *Src, void *Dst);
static void _s16_to_str(int N, void *Src, void *Dst);
static void _u32_to_str(int N, void *Src, void *Dst);
static void _s32_to_str(int N, void *Src, void *Dst);

static const pfunc_conv_t _strconvs[TYPE_MAX] = {
    _str_to_str,
    _str_to_str,
    _str_to_u8,
    _str_to_s8,
    _str_to_u16,
    _str_to_s16,
    _str_to_u32,
    _str_to_s32,
};

static const pfunc_conv_t _convstrs[TYPE_MAX] = {
    _str_to_str,
    _str_to_str,
    _u8_to_str,
    _s8_to_str,
    _u16_to_str,
    _s16_to_str,
    _u32_to_str,
    _s32_to_str,
};

void TUNE_Parser_Enc_Proc(PARAM_s* pParam, void* pField) {
    _convstrs[pParam->ValType](pParam->ValCount, pField, pParam->ValStr);
}

void TUNE_Parser_Dec_Proc(PARAM_s* pParam, void* pField) {
    _strconvs[pParam->ValType](pParam->ValCount, pParam->ValStr, pField);
}

/* string conversion functions */

static void _str_to_str(int N, void *Src, void *Dst)
{
    strcpy((char*)Dst, (char*)Src);
}

static void _str_to_u8(int N, void *Src, void *Dst)
{
    int i = 0;
    UINT8 *pU8 = (UINT8*)Dst, Value = 0;
    char *pStr = (char*)Src, *Token = 0, Seps[] = " \t";
    Token = strtok(pStr, Seps);
    while(Token && i<N) {
        Value = atoi(Token);
        memcpy(pU8+i, &Value, sizeof(UINT8));
        Token = strtok(0, Seps); i++;
    }
}
static void _str_to_s8(int N, void *Src, void *Dst)
{
    int i = 0;
    INT8 *pS8 = (INT8*)Dst, Value = 0;
    char *pStr = (char*)Src, *Token = 0, Seps[] = " \t";
    Token = strtok(pStr, Seps);
    while(Token && i<N) {
        Value = atoi(Token);
        memcpy(pS8+i, &Value, sizeof(INT8));
        Token = strtok(0, Seps); i++;
    }
}
static void _str_to_u16(int N, void *Src, void *Dst)
{
    int i = 0;
    UINT16 *pU16 = (UINT16*)Dst, Value = 0;
    char *pStr = (char*)Src, *Token = 0, Seps[] = " \t";
    Token = strtok(pStr, Seps);
    while(Token && i<N) {
        Value = atoi(Token);
        memcpy(pU16+i, &Value, sizeof(UINT16));
        Token = strtok(0, Seps); i++;
    }
}
static void _str_to_s16(int N, void *Src, void *Dst)
{
    int i = 0;
    INT16 *pS16 = (INT16*)Dst, Value = 0;
    char *pStr = (char*)Src, *Token = 0, Seps[] = " \t";
    Token = strtok(pStr, Seps);
    while(Token && i<N) {
        Value = atoi(Token);
        memcpy(pS16+i, &Value, sizeof(INT16));
        Token = strtok(0, Seps); i++;
    }
}
static void _str_to_u32(int N, void *Src, void *Dst)
{
    int i = 0;
    UINT32 *pU32 = (UINT32*)Dst, Value = 0;
    char *pStr = (char*)Src, *Token = 0, Seps[] = " \t";
    Token = strtok(pStr, Seps);
    while(Token && i<N) {
        Value = atoi(Token);
        memcpy(pU32+i, &Value, sizeof(UINT32));
        Token = strtok(0, Seps); i++;
    }
}
static void _str_to_s32(int N, void *Src, void *Dst)
{
    int i = 0;
    INT32 *pS32 = (INT32*)Dst, Value = 0;
    char *pStr = (char*)Src, *Token = 0, Seps[] = " \t";
    Token = strtok(pStr, Seps);
    while(Token && i<N) {
        Value = atoi(Token);
        memcpy(pS32+i, &Value, sizeof(INT32));
        Token = strtok(0, Seps); i++;
    }
}
static void _u8_to_str(int N, void *Src, void *Dst)
{
    int i = 0;
    char *pStr = (char*)Dst, OneStr[16];
    UINT8  *pU8 = (UINT8 *)Src, Value = 0;
    memcpy(&Value, pU8+i, sizeof(UINT8 ));
    sprintf(OneStr, "%d", Value);
    strcpy(pStr, OneStr);
    for(i=1; i<N; i++) {
        memcpy(&Value, pU8+i, sizeof(UINT8 ));
        sprintf(OneStr, " %d", Value);
        strcat(pStr, OneStr);
    }
}
static void _s8_to_str(int N, void *Src, void *Dst)
{
    int i = 0;
    char *pStr = (char*)Dst, OneStr[16];
    INT8  *pS8 = (INT8 *)Src, Value = 0;
    memcpy(&Value, pS8+i, sizeof(INT8 ));
    sprintf(OneStr, "%d", Value);
    strcpy(pStr, OneStr);
    for(i=1; i<N; i++) {
        memcpy(&Value, pS8+i, sizeof(INT8 ));
        sprintf(OneStr, " %d", Value);
        strcat(pStr, OneStr);
    }
}
static void _u16_to_str(int N, void *Src, void *Dst)
{
    int i = 0;
    char *pStr = (char*)Dst, OneStr[16];
    UINT16 *pU16 = (UINT16*)Src, Value = 0;
    memcpy(&Value, pU16+i, sizeof(UINT16));
    sprintf(OneStr, "%d", Value);
    strcpy(pStr, OneStr);
    for(i=1; i<N; i++) {
        memcpy(&Value, pU16+i, sizeof(UINT16));
        sprintf(OneStr, " %d", Value);
        strcat(pStr, OneStr);
    }
}
static void _s16_to_str(int N, void *Src, void *Dst)
{
    int i = 0;
    char *pStr = (char*)Dst, OneStr[16];
    INT16 *pS16 = (INT16*)Src, Value = 0;
    memcpy(&Value, pS16+i, sizeof(INT16));
    sprintf(OneStr, "%d", Value);
    strcpy(pStr, OneStr);
    for(i=1; i<N; i++) {
        memcpy(&Value, pS16+i, sizeof(INT16));
        sprintf(OneStr, " %d", Value);
        strcat(pStr, OneStr);
    }
}
static void _u32_to_str(int N, void *Src, void *Dst)
{
    int i = 0;
    char *pStr = (char*)Dst, OneStr[16];
    UINT32 *pU32 = (UINT32*)Src, Value = 0;
    memcpy(&Value, pU32+i, sizeof(UINT32));
    sprintf(OneStr, "%d", Value);
    strcpy(pStr, OneStr);
    for(i=1; i<N; i++) {
        memcpy(&Value, pU32+i, sizeof(UINT32));
        sprintf(OneStr, " %d", Value);
        strcat(pStr, OneStr);
    }
}
static void _s32_to_str(int N, void *Src, void *Dst)
{
    int i = 0;
    char *pStr = (char*)Dst, OneStr[16];
    INT32 *pS32 = (INT32*)Src, Value = 0;
    memcpy(&Value, pS32+i, sizeof(INT32));
    sprintf(OneStr, "%d", Value);
    strcpy(pStr, OneStr);
    for(i=1; i<N; i++) {
        memcpy(&Value, pS32+i, sizeof(INT32));
        sprintf(OneStr, " %d", Value);
        strcat(pStr, OneStr);
    }
}

int TUNE_Parser_Create(TUNE_Parser_Object_t** Object)
{
    if (Object == NULL) {
        PASER_ERRF("%s() %d, Object = NULL", __func__, __LINE__);
        return -1;
    }
    int i = 0;
    for (i = MAX_REGS-1; i >= 0; i--) {
        _Reg_List[i] = NULL;
    }
    Parser_Object[0].RegList = _Reg_List;
    Parser_Object[0].RegCount = 0;
    Parser_Object[0].LineBuf = LineBuf;
    Parser_Object[0].BufSize = MAX_LINE_CHARS;
    *Object = &Parser_Object[0];
    return 0;
}

int TUNE_Parser_Destory(TUNE_Parser_Object_t* Object)
{
    if (Object == NULL) {
        PASER_ERRF("%s() %d, Object = NULL", __func__, __LINE__);
        return -1;
    }
    Object->RegList = NULL;
    Object->RegCount = 0;
    Object->LineBuf = NULL;
    Object->BufSize = 0;
    return 0;
}

int TUNE_Parser_Add_Reg(REG_s *pReg, TUNE_Parser_Object_t* Object)
{
    int Ret = -1;
    if (Object->RegCount < MAX_REGS) {
        Object->RegList[Object->RegCount] = pReg;
        Ret = Object->RegCount;
        Object->RegCount++;
    } else {
        PASER_WARF("%s() %d, Exceed maximum REG number", __func__, __LINE__);
    }
    return Ret;
}

int TUNE_Parser_Set_Reglist_Valid(INT64 Valid, TUNE_Parser_Object_t* Object) {
    if ((Object == NULL) || (Object->RegList == NULL) || (Object->RegCount <= 0)) {
        PASER_ERRF("%s() %d, invalid Object", __func__, __LINE__);
        return -1;
    }
    int i;
    for( i = Object->RegCount - 1; i >= 0; i--) {
        Object->RegList[i]->Valid = Valid;
    }
    return 0;
}

int TUNER_Parser_Set_Reg_Valid(const char* Name, INT64 Valid, TUNE_Parser_Object_t* Object) {
    if (Object == NULL) {
        PASER_ERRF("%s() %d, Object = NULL", __func__, __LINE__);
        return -1;
    }
    REG_s* Reg = TUNE_Parser_Lookup_Reg(Name, Object);
    if (Reg == NULL) {
        PASER_WARF("%s() %d, %s Not Exist", __func__, __LINE__, Name);
        return -1;
    }
    Reg->Valid = Valid;
    return 0;
}
REG_s* TUNE_Parser_Get_Reg(int RegIdx, TUNE_Parser_Object_t* Object) {
    if (Object == NULL) {
        PASER_ERRF("%s() %d, Object = NULL", __func__, __LINE__);
        return NULL;
    }
    if (RegIdx >= Object->RegCount) {
        PASER_WARF("%s() %d, RegIdx %d > RegCount %d", __func__, __LINE__, RegIdx, Object->RegCount);
        return NULL;
    }
    return Object->RegList[RegIdx];
}
PARAM_s* TUNE_Parser_Get_Param(REG_s* Reg, int ParamIdx) {
    if (Reg == NULL) {
        PASER_ERRF("%s() %d, Reg = NULL", __func__, __LINE__);
        return NULL;
    }
    if (ParamIdx >= Reg->ParamCount) {
        PASER_ERRF("%s() %d, Reg = NULL", __func__, __LINE__);
        return NULL;
    }
    return &Reg->ParamList[ParamIdx];
}
REG_s* TUNE_Parser_Lookup_Reg(const char* Name, TUNE_Parser_Object_t* Object) {
    if (Object == NULL) {
        PASER_ERRF("%s() %d, Object = NULL", __func__, __LINE__);
        return NULL;
    }
    REG_s *Ret = NULL;
    int i;
    for (i = Object->RegCount - 1; i >= 0; i--) {
        if (strcmp(Name, Object->RegList[i]->Name) == 0) {
            Ret = Object->RegList[i];
            break;
        }
    }
    if (Ret == NULL) {
        PASER_WARF("%s() %d, Unknown REG: %s", __func__, __LINE__, Name);
    }
    return Ret;
}

PARAM_s* TUNE_Parser_Lookup_Param(const char* Name, REG_s* Reg) {
    if (Reg == NULL) {
        PASER_WARF("%s() %d, Reg = NULL", __func__, __LINE__);
        return NULL;
    }
    PARAM_s *Ret = NULL;
    int i;
    for (i = Reg->ParamCount - 1; i >= 0; i--) {
        if (strcmp(Name, Reg->ParamList[i].Name) == 0) {
            Ret = &Reg->ParamList[i];
            break;
        }
    }
    if (Ret == NULL) {
        PASER_WARF("%s() %d, Reg  %s Unknown Param %s", __func__, __LINE__, Reg->Name, Name);
    }
    return Ret;
}

static int _skip_line(const char* Linebuf)
{
    return ((*Linebuf == 0) || (*Linebuf == '#')) ? 1 : 0;
}

static int _split_param_value(char* ParamStr, char **Param, char **Val)
{
    unsigned int Pos = 0;
    char Seps[] = " \t";
    Pos = strcspn(ParamStr, Seps);
    if (Pos == strlen(ParamStr)) return 0;
    ParamStr[Pos] = 0;
    *Param = ParamStr;
    *Val = ParamStr + Pos + 1;
    return 1;
}
static int _split_reg_param(char* Linebuf, char **Reg, char **Param)
{
    unsigned int Pos = 0;
    char Seps[] = "[.";

    Pos = strcspn(Linebuf, Seps);
    if (Pos == strlen(Linebuf)) return 0;
    Linebuf[Pos] = 0;
    *Reg = Linebuf;
    *Param = Linebuf + Pos + 1;
    return 1;
}

int TUNE_Parser_Generate_Line(int RegIdx, int ParamIdx, TUNE_Parser_Object_t* Object)
{
    if (Object == NULL) {
        PASER_ERRF("%s() %d, Object = NULL", __func__, __LINE__);
        return -1;
    }
    REG_s* Reg = NULL;
    PARAM_s* Param = NULL;
    if (RegIdx >= Object->RegCount) {
        PASER_WARF("%s() %d, RegIdx %d >= RegCount %d", __func__, __LINE__, RegIdx, Object->RegCount);
        return -1;
    }
    Reg = Object->RegList[RegIdx];

    if (ParamIdx >= Reg->ParamCount) {
        PASER_WARF("%s() %d, ParamIdx %d >= ParamCount %d", __func__, __LINE__, ParamIdx, Reg->ParamCount);
        return -1;
    }
    Param = &Reg->ParamList[ParamIdx];
    // TODO: Avoid bufer overflow
    strcpy(Object->LineBuf, Reg->Name);
    strcat(Object->LineBuf, ".");
    strcat(Object->LineBuf, Param->Name);
    strcat(Object->LineBuf, " ");
    Param->ValStr = Object->LineBuf + strlen(Object->LineBuf);
    Reg->Process(ParamIdx, Param);
    strcat(Object->LineBuf, "\n");
    if (Reg->Valid & (0x1ULL << ParamIdx)) {
        return 0;
    } else {
        return 1;
    }
}

int TUNE_Parser_Get_LineBuf(TUNE_Parser_Object_t* Object, char** Line, int* Size)
{
    if (Object == NULL) {
        PASER_ERRF("%s() %d, Object = NULL", __func__, __LINE__);
        return -1;
    }
    if ((Object->LineBuf == NULL) || (Object->BufSize == 0)){
        PASER_ERRF("%s() %d, LineBuf = %s, Size = %d", __func__, __LINE__, Object->LineBuf, Object->BufSize);
        return -1;
    }
    *Line = Object->LineBuf;
    *Size = Object->BufSize;
    return 0;
}

int TUNE_Parser_Parse_Line(TUNE_Parser_Object_t* Object)
{
    char *RegStr = 0, *ParamValStr = 0, *ParamStr = 0, *ValStr = 0;
    //char *RegIdStr = 0;
    REG_s *pReg;
    PARAM_s *pParam;
    //int RegId = 0;
    if (_skip_line(Object->LineBuf)) return 0;
    if (_split_reg_param(Object->LineBuf, &RegStr, &ParamValStr)) {
        pReg = TUNE_Parser_Lookup_Reg(RegStr, Object);
        if (pReg == 0) return 0;
        /* Check if multiple reg instances*/
        if (pReg->RegNum > 1) {
            PASER_ERRF("%s() %d, Not Implement!", __func__, __LINE__);
            return -1;
        }

        if (_split_param_value(ParamValStr, &ParamStr, &ValStr)) {
            pParam = TUNE_Parser_Lookup_Param(ParamStr, pReg);
            if (pParam == 0) return 0;
            pParam->ValStr = ValStr;
            pReg->Valid |= (*pReg->Process)(pParam->Index, pParam);
        }
    }
    return 0;
}

void TUNE_Parser_Param_Proc(PARAM_s *pParam, void* pField)
{
    if (gOpMode) {
        TUNE_Parser_Dec_Proc(pParam, pField);
    } else {
        TUNE_Parser_Enc_Proc(pParam, pField);
    }
}

void TUNE_Parser_Set_Opmode(OpMode_e OpMode)
{
    gOpMode = OpMode ;
}

OpMode_e TUNE_Parser_Get_Opmode(void)
{
    return gOpMode ;
}
