/**
 * debug.h
 *
 * Author: Anthony Ginger <hfjiang@ambarella.com>
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

#ifndef __DEBUG_H__
#define __DEBUG_H__

/*===========================================================================*/
#ifndef __ASM__

#define ASSERT(x, format...)						\
		if ((x)) {						\
			printf("%s(%d): ", __func__, __LINE__);		\
			printf("Assertion: "format);			\
			for (;;);					\
		}

#define BUG_ON(x) {							\
		if (x) {						\
			putstr("BUG_ON(");				\
			putstr(__FILE__);				\
			putstr(":");					\
			putdec(__LINE__);				\
			putstr(")");					\
			for (;;);					\
		}							\
	}

#define DIE()

/* ==========================================================================*/
#ifdef __RELEASE_NOPUT_BUILD__
#define putchar(out)
#define puthex(out)
#define putbin(h, bits, show_0)
#define putbyte(out)
#define putdec(out)
#define putstr(out)
#else
#include <ambhw/uart.h>
#define putchar		uart_putchar
#define puthex		uart_puthex
#define putbin		uart_putbin
#define putbyte		uart_putbyte
#define putdec		uart_putdec
#define putstr		uart_putstr
#endif

#define putstrchar(str, c) {putstr(str); putchar(c); putstr("\r\n");}
#define putstrdec(str, d) {putstr(str); putdec(d); putstr("\r\n");}
#define putstrhex(str, h) {putstr(str); puthex(h); putstr("\r\n");}
#define putstrstr(str, s) {putstr(str); putstr(s); putstr("\r\n");}

#endif /* __ASM__ */

/*===========================================================================*/

#endif

