/*
 * ambhw/timer.h
 *
 * History:
 *	2006/12/27 - [Charles Chiou] created file
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

#ifndef __AMBHW__TIMER_H__
#define __AMBHW__TIMER_H__

#include <ambhw/chip.h>

/* ==========================================================================*/
#if (CHIP_REV == CV1)
#define TIMER_OFFSET			0x5000
#else
#define TIMER_OFFSET			0xB000
#endif
#define TIMER_BASE			(APB_N_BASE + TIMER_OFFSET)
#define TIMER_REG(x)			(TIMER_BASE + (x))

#if (CHIP_REV == CV1)
#define TIMER1_OFFSET			0x6000
#else
#define TIMER1_OFFSET			0xF000
#endif
#define TIMER1_BASE			(APB_N_BASE + TIMER1_OFFSET)
#define TIMER1_REG(x)			(TIMER1_BASE + (x))

/* ==========================================================================*/
#if (CHIP_REV == S3) || (CHIP_REV == S5) || (CHIP_REV == CV1)
#define INTERVAL_TIMER_INSTANCES		20
#else
#define INTERVAL_TIMER_INSTANCES		8
#endif

/* ==========================================================================*/
#define TIMER1_STATUS_OFFSET		0x00
#define TIMER1_RELOAD_OFFSET		0x04
#define TIMER1_MATCH1_OFFSET		0x08
#define TIMER1_MATCH2_OFFSET		0x0c
#define TIMER2_STATUS_OFFSET		0x10
#define TIMER2_RELOAD_OFFSET		0x14
#define TIMER2_MATCH1_OFFSET		0x18
#define TIMER2_MATCH2_OFFSET		0x1c
#define TIMER3_STATUS_OFFSET		0x20
#define TIMER3_RELOAD_OFFSET		0x24
#define TIMER3_MATCH1_OFFSET		0x28
#define TIMER3_MATCH2_OFFSET		0x2c

#if (INTERVAL_TIMER_INSTANCES > 3)
#define TIMER4_STATUS_OFFSET		0x34
#define TIMER4_RELOAD_OFFSET		0x38
#define TIMER4_MATCH1_OFFSET		0x3c
#define TIMER4_MATCH2_OFFSET		0x40
#define TIMER5_STATUS_OFFSET		0x44
#define TIMER5_RELOAD_OFFSET		0x48
#define TIMER5_MATCH1_OFFSET		0x4c
#define TIMER5_MATCH2_OFFSET		0x50
#define TIMER6_STATUS_OFFSET		0x54
#define TIMER6_RELOAD_OFFSET		0x58
#define TIMER6_MATCH1_OFFSET		0x5c
#define TIMER6_MATCH2_OFFSET		0x60
#define TIMER7_STATUS_OFFSET		0x64
#define TIMER7_RELOAD_OFFSET		0x68
#define TIMER7_MATCH1_OFFSET		0x6c
#define TIMER7_MATCH2_OFFSET		0x70
#define TIMER8_STATUS_OFFSET		0x74
#define TIMER8_RELOAD_OFFSET		0x78
#define TIMER8_MATCH1_OFFSET		0x7c
#define TIMER8_MATCH2_OFFSET		0x80
#endif

#if (INTERVAL_TIMER_INSTANCES > 8)
#define TIMER9_STATUS_OFFSET		0x88
#define TIMER9_RELOAD_OFFSET		0x8c
#define TIMER9_MATCH1_OFFSET		0x90
#define TIMER9_MATCH2_OFFSET		0x94
#define TIMER10_STATUS_OFFSET		0x98
#define TIMER10_RELOAD_OFFSET		0x9c
#define TIMER10_MATCH1_OFFSET		0xa0
#define TIMER10_MATCH2_OFFSET		0xa4

/* belong to the 2nd timer instance */
#define TIMER11_STATUS_OFFSET		0x00
#define TIMER11_RELOAD_OFFSET		0x04
#define TIMER11_MATCH1_OFFSET		0x08
#define TIMER11_MATCH2_OFFSET		0x0c
#define TIMER12_STATUS_OFFSET		0x10
#define TIMER12_RELOAD_OFFSET		0x14
#define TIMER12_MATCH1_OFFSET		0x18
#define TIMER12_MATCH2_OFFSET		0x1c
#define TIMER13_STATUS_OFFSET		0x20
#define TIMER13_RELOAD_OFFSET		0x24
#define TIMER13_MATCH1_OFFSET		0x28
#define TIMER13_MATCH2_OFFSET		0x2c
#define TIMER14_STATUS_OFFSET		0x34
#define TIMER14_RELOAD_OFFSET		0x38
#define TIMER14_MATCH1_OFFSET		0x3c
#define TIMER14_MATCH2_OFFSET		0x40
#define TIMER15_STATUS_OFFSET		0x44
#define TIMER15_RELOAD_OFFSET		0x48
#define TIMER15_MATCH1_OFFSET		0x4c
#define TIMER15_MATCH2_OFFSET		0x50
#define TIMER16_STATUS_OFFSET		0x54
#define TIMER16_RELOAD_OFFSET		0x58
#define TIMER16_MATCH1_OFFSET		0x5c
#define TIMER16_MATCH2_OFFSET		0x60
#define TIMER17_STATUS_OFFSET		0x64
#define TIMER17_RELOAD_OFFSET		0x68
#define TIMER17_MATCH1_OFFSET		0x6c
#define TIMER17_MATCH2_OFFSET		0x70
#define TIMER18_STATUS_OFFSET		0x74
#define TIMER18_RELOAD_OFFSET		0x78
#define TIMER18_MATCH1_OFFSET		0x7c
#define TIMER18_MATCH2_OFFSET		0x80
#define TIMER19_STATUS_OFFSET		0x88
#define TIMER19_RELOAD_OFFSET		0x8c
#define TIMER19_MATCH1_OFFSET		0x90
#define TIMER19_MATCH2_OFFSET		0x94
#define TIMER20_STATUS_OFFSET		0x98
#define TIMER20_RELOAD_OFFSET		0x9c
#define TIMER20_MATCH1_OFFSET		0xa0
#define TIMER20_MATCH2_OFFSET		0xa4
#endif

#define TIMER_CTR_OFFSET		0x30
#define TIMER_CTR1_OFFSET		0x84

#define TIMER1_STATUS_REG		TIMER_REG(TIMER1_STATUS_OFFSET)
#define TIMER1_RELOAD_REG		TIMER_REG(TIMER1_RELOAD_OFFSET)
#define TIMER1_MATCH1_REG		TIMER_REG(TIMER1_MATCH1_OFFSET)
#define TIMER1_MATCH2_REG		TIMER_REG(TIMER1_MATCH2_OFFSET)
#define TIMER2_STATUS_REG		TIMER_REG(TIMER2_STATUS_OFFSET)
#define TIMER2_RELOAD_REG		TIMER_REG(TIMER2_RELOAD_OFFSET)
#define TIMER2_MATCH1_REG		TIMER_REG(TIMER2_MATCH1_OFFSET)
#define TIMER2_MATCH2_REG		TIMER_REG(TIMER2_MATCH2_OFFSET)
#define TIMER3_STATUS_REG		TIMER_REG(TIMER3_STATUS_OFFSET)
#define TIMER3_RELOAD_REG		TIMER_REG(TIMER3_RELOAD_OFFSET)
#define TIMER3_MATCH1_REG		TIMER_REG(TIMER3_MATCH1_OFFSET)
#define TIMER3_MATCH2_REG		TIMER_REG(TIMER3_MATCH2_OFFSET)
#if (INTERVAL_TIMER_INSTANCES > 3)
#define TIMER4_STATUS_REG		TIMER_REG(TIMER4_STATUS_OFFSET)
#define TIMER4_RELOAD_REG		TIMER_REG(TIMER4_RELOAD_OFFSET)
#define TIMER4_MATCH1_REG		TIMER_REG(TIMER4_MATCH1_OFFSET)
#define TIMER4_MATCH2_REG		TIMER_REG(TIMER4_MATCH2_OFFSET)
#define TIMER5_STATUS_REG		TIMER_REG(TIMER5_STATUS_OFFSET)
#define TIMER5_RELOAD_REG		TIMER_REG(TIMER5_RELOAD_OFFSET)
#define TIMER5_MATCH1_REG		TIMER_REG(TIMER5_MATCH1_OFFSET)
#define TIMER5_MATCH2_REG		TIMER_REG(TIMER5_MATCH2_OFFSET)
#define TIMER6_STATUS_REG		TIMER_REG(TIMER6_STATUS_OFFSET)
#define TIMER6_RELOAD_REG		TIMER_REG(TIMER6_RELOAD_OFFSET)
#define TIMER6_MATCH1_REG		TIMER_REG(TIMER6_MATCH1_OFFSET)
#define TIMER6_MATCH2_REG		TIMER_REG(TIMER6_MATCH2_OFFSET)
#define TIMER7_STATUS_REG		TIMER_REG(TIMER7_STATUS_OFFSET)
#define TIMER7_RELOAD_REG		TIMER_REG(TIMER7_RELOAD_OFFSET)
#define TIMER7_MATCH1_REG		TIMER_REG(TIMER7_MATCH1_OFFSET)
#define TIMER7_MATCH2_REG		TIMER_REG(TIMER7_MATCH2_OFFSET)
#define TIMER8_STATUS_REG		TIMER_REG(TIMER8_STATUS_OFFSET)
#define TIMER8_RELOAD_REG		TIMER_REG(TIMER8_RELOAD_OFFSET)
#define TIMER8_MATCH1_REG		TIMER_REG(TIMER8_MATCH1_OFFSET)
#define TIMER8_MATCH2_REG		TIMER_REG(TIMER8_MATCH2_OFFSET)
#endif
#if (INTERVAL_TIMER_INSTANCES > 8)
#define TIMER9_STATUS_REG		TIMER_REG(TIMER9_STATUS_OFFSET)
#define TIMER9_RELOAD_REG		TIMER_REG(TIMER9_RELOAD_OFFSET)
#define TIMER9_MATCH1_REG		TIMER_REG(TIMER9_MATCH1_OFFSET)
#define TIMER9_MATCH2_REG		TIMER_REG(TIMER9_MATCH2_OFFSET)
#define TIMER10_STATUS_REG		TIMER_REG(TIMER10_STATUS_OFFSET)
#define TIMER10_RELOAD_REG		TIMER_REG(TIMER10_RELOAD_OFFSET)
#define TIMER10_MATCH1_REG		TIMER_REG(TIMER10_MATCH1_OFFSET)
#define TIMER10_MATCH2_REG		TIMER_REG(TIMER10_MATCH2_OFFSET)

/* belong to the 2nd timer instance */
#define TIMER11_STATUS_REG		TIMER1_REG(TIMER11_STATUS_OFFSET)
#define TIMER11_RELOAD_REG		TIMER1_REG(TIMER11_RELOAD_OFFSET)
#define TIMER11_MATCH1_REG		TIMER1_REG(TIMER11_MATCH1_OFFSET)
#define TIMER11_MATCH2_REG		TIMER1_REG(TIMER11_MATCH2_OFFSET)
#define TIMER12_STATUS_REG		TIMER1_REG(TIMER12_STATUS_OFFSET)
#define TIMER12_RELOAD_REG		TIMER1_REG(TIMER12_RELOAD_OFFSET)
#define TIMER12_MATCH1_REG		TIMER1_REG(TIMER12_MATCH1_OFFSET)
#define TIMER12_MATCH2_REG		TIMER1_REG(TIMER12_MATCH2_OFFSET)
#define TIMER13_STATUS_REG		TIMER1_REG(TIMER13_STATUS_OFFSET)
#define TIMER13_RELOAD_REG		TIMER1_REG(TIMER13_RELOAD_OFFSET)
#define TIMER13_MATCH1_REG		TIMER1_REG(TIMER13_MATCH1_OFFSET)
#define TIMER13_MATCH2_REG		TIMER1_REG(TIMER13_MATCH2_OFFSET)
#define TIMER14_STATUS_REG		TIMER1_REG(TIMER14_STATUS_OFFSET)
#define TIMER14_RELOAD_REG		TIMER1_REG(TIMER14_RELOAD_OFFSET)
#define TIMER14_MATCH1_REG		TIMER1_REG(TIMER14_MATCH1_OFFSET)
#define TIMER14_MATCH2_REG		TIMER1_REG(TIMER14_MATCH2_OFFSET)
#define TIMER15_STATUS_REG		TIMER1_REG(TIMER15_STATUS_OFFSET)
#define TIMER15_RELOAD_REG		TIMER1_REG(TIMER15_RELOAD_OFFSET)
#define TIMER15_MATCH1_REG		TIMER1_REG(TIMER15_MATCH1_OFFSET)
#define TIMER15_MATCH2_REG		TIMER1_REG(TIMER15_MATCH2_OFFSET)
#define TIMER16_STATUS_REG		TIMER1_REG(TIMER16_STATUS_OFFSET)
#define TIMER16_RELOAD_REG		TIMER1_REG(TIMER16_RELOAD_OFFSET)
#define TIMER16_MATCH1_REG		TIMER1_REG(TIMER16_MATCH1_OFFSET)
#define TIMER16_MATCH2_REG		TIMER1_REG(TIMER16_MATCH2_OFFSET)
#define TIMER17_STATUS_REG		TIMER1_REG(TIMER17_STATUS_OFFSET)
#define TIMER17_RELOAD_REG		TIMER1_REG(TIMER17_RELOAD_OFFSET)
#define TIMER17_MATCH1_REG		TIMER1_REG(TIMER17_MATCH1_OFFSET)
#define TIMER17_MATCH2_REG		TIMER1_REG(TIMER17_MATCH2_OFFSET)
#define TIMER18_STATUS_REG		TIMER1_REG(TIMER18_STATUS_OFFSET)
#define TIMER18_RELOAD_REG		TIMER1_REG(TIMER18_RELOAD_OFFSET)
#define TIMER18_MATCH1_REG		TIMER1_REG(TIMER18_MATCH1_OFFSET)
#define TIMER18_MATCH2_REG		TIMER1_REG(TIMER18_MATCH2_OFFSET)
#define TIMER19_STATUS_REG		TIMER1_REG(TIMER19_STATUS_OFFSET)
#define TIMER19_RELOAD_REG		TIMER1_REG(TIMER19_RELOAD_OFFSET)
#define TIMER19_MATCH1_REG		TIMER1_REG(TIMER19_MATCH1_OFFSET)
#define TIMER19_MATCH2_REG		TIMER1_REG(TIMER19_MATCH2_OFFSET)
#define TIMER20_STATUS_REG		TIMER1_REG(TIMER20_STATUS_OFFSET)
#define TIMER20_RELOAD_REG		TIMER1_REG(TIMER20_RELOAD_OFFSET)
#define TIMER20_MATCH1_REG		TIMER1_REG(TIMER20_MATCH1_OFFSET)
#define TIMER20_MATCH2_REG		TIMER1_REG(TIMER20_MATCH2_OFFSET)
#endif

#define TIMER_CTR_REG			TIMER_REG(TIMER_CTR_OFFSET)
#define TIMER_CTR1_REG			TIMER_REG(TIMER_CTR1_OFFSET)
#define TIMER1_CTR_REG			TIMER1_REG(TIMER_CTR_OFFSET)
#define TIMER1_CTR1_REG			TIMER1_REG(TIMER_CTR1_OFFSET)

#define TIMER_CTR_EN1			0x00000001
#define TIMER_CTR_CSL1			0x00000002
#define TIMER_CTR_OF1			0x00000004
#define TIMER_CTR_EN2			0x00000010
#define TIMER_CTR_CSL2			0x00000020
#define TIMER_CTR_OF2			0x00000040
#define TIMER_CTR_EN3			0x00000100
#define TIMER_CTR_CSL3			0x00000200
#define TIMER_CTR_OF3			0x00000400
#if (INTERVAL_TIMER_INSTANCES > 3)
#define TIMER_CTR_EN4			0x00001000
#define TIMER_CTR_CSL4			0x00002000
#define TIMER_CTR_OF4			0x00004000
#define TIMER_CTR_EN5			0x00010000
#define TIMER_CTR_CSL5			0x00020000
#define TIMER_CTR_OF5			0x00040000
#define TIMER_CTR_EN6			0x00100000
#define TIMER_CTR_CSL6			0x00200000
#define TIMER_CTR_OF6			0x00400000
#define TIMER_CTR_EN7			0x01000000
#define TIMER_CTR_CSL7			0x02000000
#define TIMER_CTR_OF7			0x04000000
#define TIMER_CTR_EN8			0x10000000
#define TIMER_CTR_CSL8			0x20000000
#define TIMER_CTR_OF8			0x40000000
#endif
#if (INTERVAL_TIMER_INSTANCES > 8)
#define TIMER_CTR_EN9			0x00000001
#define TIMER_CTR_OF9			0x00000004
#define TIMER_CTR_EN10			0x00000010
#define TIMER_CTR_OF10			0x00000040
#endif

#define TIMER_STATUS_OFFSET		0x00
#define TIMER_RELOAD_OFFSET		0x04
#define TIMER_MATCH1_OFFSET		0x08
#define TIMER_MATCH2_OFFSET		0x0c

#define TIMER_CTRL_OF			0x4
#define TIMER_CTRL_CSL			0x2
#define TIMER_CTRL_EN			0x1

/* ==========================================================================*/
#ifndef __ASM__
/* ==========================================================================*/

/* ==========================================================================*/
#endif
/* ==========================================================================*/

#endif

