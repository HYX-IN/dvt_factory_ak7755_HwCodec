/*******************************************************************************
 * rtp_rtcp_sink_video_v2.h
 *
 * History:
 *    2014/08/25 - [Zhi He] create file
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

#ifndef __RTP_RTCP_SINK_VIDEO_V2__
#define __RTP_RTCP_SINK_VIDEO_V2__

DCONFIG_COMPILE_OPTION_HEADERFILE_BEGIN
DCODE_DELIMITER;

//-----------------------------------------------------------------------
//
// CVideoRTPRTCPSinkV2
//
//-----------------------------------------------------------------------
class CVideoRTPRTCPSinkV2: public CObject, public IStreamingTransmiter
{
    typedef CObject inherited;

public:
    static IStreamingTransmiter *Create(const TChar *pname, const volatile SPersistMediaConfig *pPersistMediaConfig, IMsgSink *pMsgSink, TComponentIndex index);
    virtual void Destroy();
    virtual void Delete();

protected:
    CVideoRTPRTCPSinkV2(const TChar *pname, const volatile SPersistMediaConfig *pPersistMediaConfig, IMsgSink *pMsgSink, TComponentIndex index);
    EECode Construct();
    virtual ~CVideoRTPRTCPSinkV2();

public:
    virtual EECode SetupContext(StreamTransportType type, ProtocolType protocol_type, const SStreamCodecInfos *infos, TTime start_time = 0);
    virtual EECode DestroyContext();
    virtual EECode UpdateStreamFormat(StreamType type, StreamFormat format);

    virtual EECode Start();
    virtual EECode Stop();

    virtual EECode SetExtraData(TU8 *pdata, TMemSize size);
    virtual EECode SendData(CIBuffer *pBuffer);

    virtual EECode AddSubSession(SSubSessionInfo *p_sub_session);
    virtual EECode RemoveSubSession(SSubSessionInfo *p_sub_session);
    virtual EECode SetSrcPort(TSocketPort port, TSocketPort port_ext);
    virtual EECode GetSrcPort(TSocketPort &port, TSocketPort &port_ext);

private:
    EECode setupUDPSocket();
    void udpSendData(TU8 *p_data, TMemSize size, TU32 key_frame);
    void tcpSendData(TU8 *p_data, TMemSize size, TU32 key_frame);

    void sendDataH264(TU8 *p_data, TMemSize data_size, TU32 key_frame);
    void sendDataH264Extradata(TU8 *p_data, TMemSize data_size);
    void sendDataH264TCP(TU8 *p_data, TMemSize data_size, TU32 key_frame);
    void sendDataH264TCPExtradata(TU8 *p_data, TMemSize data_size);

    void processRTCP(SSubSessionInfo *sub_session, TTime ntp_time, TU32 timestamp);
    void udpSendSr(SSubSessionInfo *sub_session);
    void tcpSendSr(SSubSessionInfo *sub_session);

    void buildSr(TUint ssrc, TS64 ntp_time, TUint timestamp, TUint packet_count, TUint octet_count);
    void updateSr(TUint ssrc, TS64 ntp_time, TUint timestamp, TUint packet_count, TUint octet_count);
    void buildBye(TUint ssrc);
    void processBYE(SSubSessionInfo *sub_session);
    void updateTimeStamp();

private:
    StreamFormat mFormat;
    CIClockReference   *mpClockReference;

private:
    const volatile SPersistMediaConfig *mpConfig;
    IMsgSink *mpMsgSink;

private:
    CIDoubleLinkedList *mpSubSessionList;
    CIDoubleLinkedList *mpTCPSubSessionList;

    TU8 mbRTPSocketSetup;
    TU8 mPayloadType;
    TU8 mbSrBuilt;
    TU8 mbUsePulseTransfer;

private:
    TInt mRTPSocket;
    TInt mRTCPSocket;
    TU16 mRTPPort;
    TU16 mRTCPPort;

private:
    TU8 *mpRTPBuffer;
    TMemSize mRTPBufferLength;
    TMemSize mRTPBufferTotalLength;

private:
    TU8 *mpRTCPBuffer;
    TMemSize mRTCPBufferLength;
    TMemSize mRTCPBufferTotalLength;

private:
    TTime mCurBufferLinearTime;
    TTime mCurBufferNativeTime;
};

DCONFIG_COMPILE_OPTION_HEADERFILE_END

#endif
