/*******************************************************************************
 * am_signature_verify.h
 *
 * History:
 *   2016-9-23 - [ccjing] created file
 *
 * Copyright (c) 2016 Ambarella, Inc.
 *
 * This file and its contents (“Software”) are protected by intellectual
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
#ifndef AM_SIGNATURE_VERIFY_H_
#define AM_SIGNATURE_VERIFY_H_
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include "am_signature_verify_if.h"

class AMSignatureVerify : public AMISignatureVerify
{
  public :
    virtual bool init_sig_verify();
    virtual bool verify_update(const uint8_t *data, uint32_t data_len);
    virtual bool verify_final(const uint8_t *sig_value, uint32_t sig_len);
    virtual void clean_md_ctx();
    virtual void destroy();
  public :
    static AMSignatureVerify* create(const char *key_file);
  private :
    AMSignatureVerify();
    virtual ~AMSignatureVerify();
    bool init(const char *key_file);
  private :
    EVP_PKEY   *m_evp_key = nullptr;
#if OPENSSL_VERSION_NUMBER >= 0x10100000L
    EVP_MD_CTX *m_md_ctx  = nullptr;
#else
    EVP_MD_CTX  m_md_ctx;
#endif
};


#endif /* AM_SIGNATURE_VERIFY_H_ */