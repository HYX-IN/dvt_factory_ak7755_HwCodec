/*******************************************************************************
 * am_mp4_file_combiner.h
 *
 * History:
 *   2015-10-10 - [ccjing] created file
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
#ifndef AM_MP4_FILE_COMBINER_H_
#define AM_MP4_FILE_COMBINER_H_
#include "am_mp4_file_combiner_if.h"
class AMIMp4FileParser;
class AMMp4FileCombiner : public AMIMp4FileCombiner
{
  public :
    static AMMp4FileCombiner* create(const char *first_mp4_file_path,
                                     const char *second_mp4_file_path);
  public :
    virtual void destroy();
    virtual bool get_combined_mp4_file(const char *file_path);
    virtual ~AMMp4FileCombiner();
  private :
    AMMp4FileCombiner();
    bool init(const char *first_mp4_file_path, const char *second_mp4_file_path);
  private :
    AMIMp4FileParser *m_first_mp4_parser;
    AMIMp4FileParser *m_second_mp4_parser;
};

#endif /* AM_MP4_FILE_COMBINER_H_ */
