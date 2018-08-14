/*
 * bpi_yuv_rotate.h
 *
 * History:
 *       2016/09/01 - [ShengJiang] created file
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
 *
 */

#ifndef __BPI_YUV_ROTATE_H_
#define __BPI_YUV_ROTATE_H_
class BPIYUVRotate {
public:
    static void BPIYUV420Rotate(unsigned char* dstImg, unsigned char* srcImgY,
                                unsigned char* srcImgUV, int width, int height,
                                int pitch, int rotate);
    static void NeonRotateYUV420Y90(unsigned char* dstImg,
                                    unsigned char* srcImg, int width,
                                    int height, int pitch);
    static void NeonRotateYUV420UV90(unsigned char* dstImg,
                                     unsigned char* srcImg, int width,
                                     int height, int pitch);
    static void NeonRotateYUV420Y180(unsigned char* dstImg,
                                     unsigned char* srcImg, int width,
                                     int height, int pitch);
    static void NeonRotateYUV420UV180(unsigned char* dstImg,
                                      unsigned char* srcImg, int width,
                                      int height, int pitch);
    static void NeonRotateYUV420Y270(unsigned char* dstImg,
                                     unsigned char* srcImg, int width,
                                     int height, int pitch);
    static void NeonRotateYUV420UV270(unsigned char* dstImg,
                                      unsigned char* srcImg, int width,
                                      int height, int pitch);
};
#endif //__BPI_YUV_ROTATE_H_
