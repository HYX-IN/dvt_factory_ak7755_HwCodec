/*******************************************************************************
 * am_video_encoder.h
 *
 * History:
 *    2014/10/29 - [Zhi He] create file
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
 ******************************************************************************/

#ifndef __AM_VIDEO_ENCODER_V2_H__
#define __AM_VIDEO_ENCODER_V2_H__

//-----------------------------------------------------------------------
//
// CVideoEncoderV2
//
//-----------------------------------------------------------------------
class CVideoEncoderV2
  : public CActiveFilter
{
  typedef CActiveFilter inherited;

public:
  static IFilter *Create(const TChar *pName, const volatile SPersistMediaConfig *pPersistMediaConfig, IMsgSink *pEngineMsgSink, TU32 index);

protected:
  CVideoEncoderV2(const TChar *pName, const volatile SPersistMediaConfig *pPersistMediaConfig, IMsgSink *pEngineMsgSink, TU32 index);

  EECode Construct();
  virtual ~CVideoEncoderV2();

public:
  virtual void Delete();

public:
  virtual EECode Initialize(TChar *p_param = NULL);
  virtual EECode Finalize();

  virtual EECode Start();
  virtual EECode Stop();

  virtual EECode SwitchInput(TComponentIndex focus_input_index = 0);
  virtual EECode FlowControl(EBufferType flowcontrol_type);

  virtual EECode AddOutputPin(TUint &index, TUint &sub_index, TUint type);
  virtual EECode AddInputPin(TUint &index, TUint type);

  virtual IOutputPin *GetOutputPin(TUint index, TUint sub_index = 0);
  virtual IInputPin *GetInputPin(TUint index);

  virtual EECode FilterProperty(EFilterPropertyType property_type, TUint set_or_get, void *p_param);

  virtual void GetInfo(INFO &info);
  virtual void PrintStatus();

  virtual void EventNotify(EEventType type, TU64 param1, TPointer param2);

protected:
  virtual void OnRun();

private:
  void processCmd(SCMD &cmd);
  void flushData();
  void sendFlowControlBuffer(CIBuffer *buffer);

private:
  IVideoEncoder *mpEncoder;
  CIClockReference *mpSystemClockReference;
  EVideoEncoderModuleID mCurEncoderID;

private:
  TU8 mbEncoderContentSetup;
  TU8 mReserved0;
  TU8 mReserved1;
  TU8 mbEncoderStarted;
  TU32 mStreamID;

private:
  TU32 mnCurrentInputPinNumber;
  CQueueInputPin *mpInputPins[EConstVideoDecoderMaxInputPinNumber];
  CQueueInputPin *mpCurInputPin;

  TU32 mnCurrentOutputPinNumber;
  //COutputPin *mpOutputPin;
  CBufferPool *mpBufferPool;
  COutputPin *mpOutputPins[DMaxStreamNumber];

private:
  CIBuffer *mpInputBuffer;
  CIBuffer *mpBuffer;
  CIBuffer *mpCachedInputBuffer;

private:
  CIBuffer mPersistEOSBuffer;

};

#endif

