/**
 * bld/memcpy.c
 *
 * History:
 *    2005/01/27 - [Charles Chiou] created file
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

#include <bldfunc.h>

/*-------------------------------------------------------------------------
 void *memcpy(void* dst, const void* src, unsigned int n)

 Copy block of memory

 Input	: void* dst		: destination address
	  const void* src	: source address
 	  unsigned int n	: memory size to be copied
 Output	: none
 Return	: destination address
 Note	: none
-------------------------------------------------------------------------*/
void *memcpy(void* dst, const void* src, unsigned int n)
{
	if (n)
	{
		switch (((uintptr_t)src | (uintptr_t)dst | n) & 3)
		{
		case 0: {
			unsigned int *xp = (unsigned int *)src;
			unsigned int *wp = (unsigned int *)dst;
			do {
				*wp++ = *xp++;
			} while ((n -= 4) != 0);
			break;
		}

		case 2: {
			unsigned short *xp = (unsigned short *)src;
			unsigned short *wp = (unsigned short *)dst;
			do {
				*wp++ = *xp++;
			} while ((n -= 2) != 0);
			break;
		}

		default: {
			unsigned char *xp = (unsigned char *)src;
			unsigned char *wp = (unsigned char *)dst;
			do {
				*wp++ = *xp++;
			} while ((n -= 1) != 0);
			break;
		}
		}
	}

	return dst;
}
