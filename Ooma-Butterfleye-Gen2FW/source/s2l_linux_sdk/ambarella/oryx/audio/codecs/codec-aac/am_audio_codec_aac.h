/*******************************************************************************
 * am_audio_codec_aac.h
 *
 * History:
 *   2014-11-3 - [ypchang] created file
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
#ifndef AM_AUDIO_CODEC_AAC_H_
#define AM_AUDIO_CODEC_AAC_H_

class  AMAudioCodecAacConfig;
struct AudioCodecAacConfig;

class AMAudioCodecAac: public AMAudioCodec
{
    typedef AMAudioCodec inherited;

  public:
    static AMIAudioCodec* create(const char *config);

  public:
    virtual void destroy();
    virtual bool initialize(AM_AUDIO_INFO *srcAudioInfo,
                            AM_AUDIO_CODEC_MODE mode);
    virtual bool finalize();
    virtual AM_AUDIO_INFO* get_codec_audio_info();
    virtual uint32_t get_codec_output_size();
    virtual bool check_encode_src_parameter(AM_AUDIO_INFO &info);
    virtual uint32_t get_encode_required_chunk_size(AM_AUDIO_INFO &info);
    /* encode: return generated bytes,
     * generated data size is also stored in out_data_size
     */
    virtual uint32_t encode(uint8_t *input,  uint32_t in_data_size,
                            uint8_t *output, uint32_t *out_data_size);
    /* decode: return consumed bytes,
     * generated data size is stored in out_data_size
     */
    virtual uint32_t decode(uint8_t *input,  uint32_t in_data_size,
                            uint8_t *output, uint32_t *out_data_size);

  protected:
    AMAudioCodecAac();
    virtual ~AMAudioCodecAac();
    bool init(const char *config);

  private:
    AudioCodecAacConfig   *m_aac_config     = nullptr; /* No need to delete */
    AMAudioCodecAacConfig *m_config         = nullptr;
    au_aacenc_config_t    *m_enc_conf       = nullptr;
    au_aacdec_config_t    *m_dec_conf       = nullptr;
    uint8_t               *m_enc_buffer     = nullptr;
    uint32_t              *m_dec_buffer     = nullptr;
    int32_t               *m_dec_out_buffer = nullptr;
};

#endif /* AM_AUDIO_CODEC_AAC_H_ */
