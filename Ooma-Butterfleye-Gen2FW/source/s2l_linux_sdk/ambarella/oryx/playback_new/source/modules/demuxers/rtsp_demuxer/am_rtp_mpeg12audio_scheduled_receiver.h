/**
 * am_rtp_mpeg12audio_scheduled_receiver.h
 *
 * History:
 *    2013/11/24 - [Zhi He] create file
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
 */

#ifndef __AM_RTP_MPEG12AUDIO_SCHEDULED_RECEIVER_H__
#define __AM_RTP_MPEG12AUDIO_SCHEDULED_RECEIVER_H__

//-----------------------------------------------------------------------
//
// CRTPMpeg12AudioScheduledReceiver
//
//-----------------------------------------------------------------------
class CRTPMpeg12AudioScheduledReceiver
  : public CObject
  , public IScheduledClient
  , public IScheduledRTPReceiver
{
  typedef CObject inherited;

public:
  CRTPMpeg12AudioScheduledReceiver(TUint index, const volatile SPersistMediaConfig *pPersistMediaConfig, IMsgSink *pMsgSink);
  virtual ~CRTPMpeg12AudioScheduledReceiver();

public:
  virtual EECode Scheduling(TUint times = 1, TU32 inout_mask = 0);
  virtual TInt IsPassiveMode() const;
  virtual TSchedulingHandle GetWaitHandle() const;
  virtual TSchedulingUnit HungryScore() const;
  virtual EECode EventHandling(EECode err);
  virtual TU8 GetPriority() const;

public:
  virtual CObject *GetObject0() const;

public:
  virtual EECode Initialize(SRTPContext *context, SStreamCodecInfo *p_stream_info, TUint number_of_content = 1);
  virtual EECode ReInitialize(SRTPContext *context, SStreamCodecInfo *p_stream_info, TUint number_of_content = 1);
  virtual EECode Flush();

  virtual EECode Purge();
  virtual EECode SetExtraData(TU8 *p_extradata, TU32 extradata_size, TU32 index);

  virtual void SetVideoDataPostProcessingCallback(void *callback_context, void *callback);
  virtual void SetAudioDataPostProcessingCallback(void *callback_context, void *callback);

public:
  void PrintStatus();

public:
  void writeRR();
  void updateRR();
  void sendRR();
  void receiveSR();
  void checkSRsendRR();

private:
  TU8 mTrackID;
  TU8 mbRun;
  TU8 mbInitialized;
  TU8 mbEnableRCTP;

  StreamType mType;
  StreamFormat mFormat;

  IMsgSink *mpMsgSink;
  const volatile SPersistMediaConfig *mpPersistMediaConfig;

  COutputPin *mpOutputPin;
  CBufferPool *mpBufferPool;
  IMemPool *mpMemPool;

  TInt mRTSPSocket;
  TInt mRTPSocket;
  TInt mRTCPSocket;

  ISimpleQueue *mpCmdSimpleQueue;

private:
  struct sockaddr mSrcAddr;
  struct sockaddr mSrcRTCPAddr;
  socklen_t mFromLen;

private:
  TUint msState;

private:
  TInt mRTPHeaderLength;

  TU8 *mpMemoryStart;
  TInt mReadLen;

private:
  TU32 mRTPTimeStamp;
  TU16 mRTPCurrentSeqNumber;
  TU16 mRTPLastSeqNumber;

  CIBuffer *mpBuffer;

  TInt mRequestMemorySize;

  TUint mCmd;

private:
  TU8 mDebugWaitMempoolFlag;
  TU8 mDebugWaitBufferpoolFlag;
  TU8 mDebugWaitReadSocketFlag;
  TU8 mbGetSSRC;

private:
  TU8 mAudioChannelNumber;
  TU8 mAudioSampleFormat;
  TU8 mbAudioChannelInterlave;
  TU8 mReserved5;

  TU32 mAudioSamplerate;
  TU32 mAudioBitrate;
  TU32 mAudioFrameSize;

private:
  TU8 mbSendSyncPointBuffer;
  TU8 mPriority;
  TU16 mBufferSessionNumber;

private:
  TU8 mReservedData[32];

private:
  TU8 *mpExtraData;
  TUint mnExtraDataSize;

private:
  TTime mLastRtcpNtpTime;
  TTime mFirstRtcpNtpTime;
  TTime mLastRtcpTimeStamp;
  TTime mRtcpTimeStampOffset;

  TUint mPacketCount;
  TUint mOctetCount;
  TUint mLastOctetCount;

  TU32 mServerSSRC;
  TU32 mSSRC;
  TU32 mPacketSSRC;

  TU32 mRTCPCurrentTick;
  TU32 mRTCPCoolDown;

  TChar mCName[128];

  TU8 mRTCPBuffer[256];
  TUint mRTCPDataLen;

  TU8 mRTCPReadBuffer[256];
  TUint mRTCPReadDataLen;

  SRTPStatistics mRTPStatistics;

private:
  TTime mFakePts;
  TU32 mLastPrintLostCount;
};


#endif

