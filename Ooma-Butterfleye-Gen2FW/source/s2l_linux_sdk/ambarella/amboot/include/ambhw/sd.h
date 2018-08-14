/*
 * ambhw/sd.h
 *
 * History:
 *	2007/01/27 - [Charles Chiou] created file
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

#ifndef __AMBHW__SD_H__
#define __AMBHW__SD_H__

#include <ambhw/chip.h>

/* ==========================================================================*/
#if (CHIP_REV == S3L) || (CHIP_REV == S5L) || (CHIP_REV == CV1)
#define SD_INSTANCES			2
#elif (CHIP_REV == S2L) || (CHIP_REV == S3) || (CHIP_REV == S5)
#define SD_INSTANCES			3
#else
#define SD_INSTANCES			1
#endif

#if (CHIP_REV == S3L) || (CHIP_REV == S5L) || (CHIP_REV == CV1)
#define SD_SUPPORT_SDIO			0
#else
#define SD_SUPPORT_SDIO			1
#endif

#define SD_SUPPORT_SDXC			1

/* ==========================================================================*/
#if (CHIP_REV == CV1)
#define SD0_OFFSET			0x4000
#else
#define SD0_OFFSET			0x2000
#endif

#define SD1_OFFSET			0xC000

#if (CHIP_REV == CV1)
#define SD2_OFFSET			0x5000
#else
#define SD2_OFFSET			0x1F000
#endif

#define SD0_BASE			(AHB_N_BASE + SD0_OFFSET)
#define SD1_BASE			(AHB_N_BASE + SD1_OFFSET)
#define SD2_BASE			(AHB_N_BASE + SD2_OFFSET)
#define SD_BASE(id)			((id == 0) ? SD0_BASE : \
					 (id == 1) ? SD1_BASE : SD2_BASE)

#define SD0_REG(x)			(SD0_BASE + (x))
#define SD2_REG(x)			(SD1_BASE + (x))
#define SD3_REG(x)			(SD2_BASE + (x))

/* ==========================================================================*/
#define SD_DMA_ADDR_OFFSET		0x000
#define SD_BLK_SZ_OFFSET		0x004	/* Half word */
#define SD_BLK_CNT_OFFSET		0x006	/* Half word */
#define SD_ARG_OFFSET			0x008
#define SD_XFR_OFFSET			0x00C	/* Half word */
#define SD_CMD_OFFSET			0x00E	/* Half word */
#define SD_RSP0_OFFSET			0x010
#define SD_RSP1_OFFSET			0x014
#define SD_RSP2_OFFSET			0x018
#define SD_RSP3_OFFSET			0x01C
#define SD_DATA_OFFSET			0x020
#define SD_STA_OFFSET			0x024
#define SD_HOST_OFFSET			0x028	/* Byte */
#define SD_PWR_OFFSET			0x029	/* Byte */
#define SD_GAP_OFFSET			0x02A	/* Byte */
#define SD_WAK_OFFSET			0x02B	/* Byte */
#define SD_CLK_OFFSET			0x02C	/* Half word */
#define SD_TMO_OFFSET			0x02E	/* Byte */
#define SD_RESET_OFFSET			0x02F	/* Byte */
#define SD_NIS_OFFSET			0x030	/* Half word */
#define SD_EIS_OFFSET			0x032	/* Half word */
#define SD_NISEN_OFFSET			0x034	/* Half word */
#define SD_EISEN_OFFSET			0x036	/* Half word */
#define SD_NIXEN_OFFSET			0x038	/* Half word */
#define SD_EIXEN_OFFSET			0x03A	/* Half word */
#define SD_AC12ES_OFFSET		0x03C	/* Half word */
#define SD_HOST2_OFFSET			0x03E	/* Half word */
#define SD_CAP_OFFSET			0x040
#define SD_CUR_OFFSET			0x048
#define SD_ADMA_STA_OFFSET		0x054
#define SD_ADMA_ADDR_OFFSET		0x058
#define SD_XC_CTR_OFFSET		0x060
#define SD_BOOT_CTR_OFFSET		0x070
#define SD_BOOT_STA_OFFSET		0x074
#define SD_VOL_SW_OFFSET		0x07C
#define SD_DELAY_SEL_L			0x0D8
#define SD_DELAY_SEL_H			0x0DC
#define SD_LAT_CTRL_OFFSET		0x0F8
#define SD_SIST_OFFSET			0x0FC	/* Half word */
#define SD_VER_OFFSET			0x0FE	/* Half word */

/* SD_BLK_SZ_REG */
#define SD_BLK_SZ_4KB			0x0000
#define SD_BLK_SZ_8KB			0x1000
#define SD_BLK_SZ_16KB			0x2000
#define SD_BLK_SZ_32KB			0x3000
#define SD_BLK_SZ_64KB			0x4000
#define SD_BLK_SZ_128KB			0x5000
#define SD_BLK_SZ_256KB			0x6000
#define SD_BLK_SZ_512KB			0x7000

/* SD_XFR_REG */
#define SD_XFR_MUL_SEL			0x0020
#define SD_XFR_SGL_SEL			0x0000
#define SD_XFR_CTH_SEL			0x0010
#define SD_XFR_HTC_SEL			0x0000
#define SD_XFR_AC12_EN			0x0004
#define SD_XFR_BLKCNT_EN		0x0002
#define SD_XFR_DMA_EN			0x0001

/* SD_CMD_REG */
#define SD_CMD_IDX(x)			((x) << 8)
#define SD_CMD_NORMAL			0x00000000
#define SD_CMD_SUSPEND			0x00000040
#define SD_CMD_RESUME			0x00000080
#define SD_CMD_ABORT			0x000000C0
#define SD_CMD_DATA			0x00000020
#define SD_CMD_CHKIDX			0x00000010
#define SD_CMD_CHKCRC			0x00000008
#define SD_CMD_RSP_NONE			0x00000000
#define SD_CMD_RSP_136			0x00000001
#define SD_CMD_RSP_48			0x00000002
#define SD_CMD_RSP_48BUSY		0x00000003

/* SD_STA_REG */
#define SD_STA_DAT_LSL(x)		((((x) & 0x1e000000) >> 25) | \
					 (((x) & 0x00f00000) >> 20))
#define SD_STA_CMD_LSL(x)		(((x)  & 0x01000000) >> 24)
#define SD_STA_WPS_PL			0x00080000
#define SD_STA_CDP_L			0x00040000
#define SD_STA_CSS			0x00020000
#define SD_STA_CARD_INSERTED		0x00010000
#define SD_STA_BUFFER_READ_EN		0x00000800
#define SD_STA_BUFFER_WRITE_EN		0x00000400
#define SD_STA_READ_XFR_ACTIVE		0x00000200
#define SD_STA_WRITE_XFR_ACTIVE		0x00000100
#define SD_STA_DAT_ACTIVE		0x00000004
#define SD_STA_CMD_INHIBIT_DAT		0x00000002
#define SD_STA_CMD_INHIBIT_CMD		0x00000001

/* SD_HOST_REG */
#define SD_HOST_ADMA			0x10
#define SD_HOST_8BIT			0x08
#define SD_HOST_HIGH_SPEED		0x04
#define SD_HOST_4BIT			0x02
#define SD_HOST_LED_ON			0x01

/* SD_PWR_REG only care about bit[3] */
#define SD_PWR_3_3V			0x08
#define SD_PWR_3_0V			0x08
#define SD_PWR_1_8V			0x00

#define SD_PWR_ON			0x01
#define SD_PWR_OFF			0x00

/* SD_GAP_REG */
#define SD_GAP_INT_AT_GAP		0x08
#define SD_GAP_READ_WAIT		0x04
#define SD_GAP_CONT_REQ			0x02
#define SD_GAP_STOP_AT_GAP		0x01

/* SD_WAK_REG */
#define SD_WAK_ON_CARD_RMV		0x04
#define SD_WAK_ON_CARD_IST		0x02
#define SD_WAK_ON_CARD_INT		0x01

/* SD_CLK_REG */
#define SD_CLK_DIV_256			0x8000
#define SD_CLK_DIV_128			0x4000
#define SD_CLK_DIV_64			0x2000
#define SD_CLK_DIV_32			0x1000
#define SD_CLK_DIV_16			0x0800
#define SD_CLK_DIV_8			0x0400
#define SD_CLK_DIV_4			0x0200
#define SD_CLK_DIV_2			0x0100
#define SD_CLK_DIV_1			0x0000
#define SD_CLK_EN			0x0004
#define SD_CLK_ICLK_STABLE		0x0002
#define SD_CLK_ICLK_EN			0x0001

/* SD_TMO_REG */

/* SD_RESET_REG */
#define SD_RESET_DAT			0x04
#define SD_RESET_CMD			0x02
#define SD_RESET_ALL			0x01

/* SD_NIS_REG */
#define SD_NIS_ERROR			0x8000
#define SD_NIS_CARD			0x0100
#define SD_NIS_REMOVAL			0x0080
#define SD_NIS_INSERT			0x0040
#define SD_NIS_READ_READY		0x0020
#define SD_NIS_WRITE_READY		0x0010
#define SD_NIS_DMA			0x0008
#define SD_NIS_BLOCK_GAP		0x0004
#define SD_NIS_XFR_DONE			0x0002
#define SD_NIS_CMD_DONE			0x0001

/* SD_EIS_REG */
#define SD_EIS_ADMA_ERR			0x0200
#define SD_EIS_ACMD12_ERR		0x0100
#define SD_EIS_CURRENT_ERR		0x0080
#define SD_EIS_DATA_BIT_ERR		0x0040
#define SD_EIS_DATA_CRC_ERR		0x0020
#define SD_EIS_DATA_TMOUT_ERR		0x0010
#define SD_EIS_CMD_IDX_ERR		0x0008
#define SD_EIS_CMD_BIT_ERR		0x0004
#define SD_EIS_CMD_CRC_ERR		0x0002
#define SD_EIS_CMD_TMOUT_ERR		0x0001

/* SD_NISEN_REG */
#define SD_NISEN_CARD			0x0100
#define SD_NISEN_REMOVAL		0x0080
#define SD_NISEN_INSERT			0x0040
#define SD_NISEN_READ_READY		0x0020
#define SD_NISEN_WRITE_READY		0x0010
#define SD_NISEN_DMA			0x0008
#define SD_NISEN_BLOCK_GAP		0x0004
#define SD_NISEN_XFR_DONE		0x0002
#define SD_NISEN_CMD_DONE		0x0001

/* SD_EISEN_REG */
#define SD_EISEN_ADMA_ERR		0x0200
#define SD_EISEN_ACMD12_ERR		0x0100
#define SD_EISEN_CURRENT_ERR		0x0080
#define SD_EISEN_DATA_BIT_ERR		0x0040
#define SD_EISEN_DATA_CRC_ERR		0x0020
#define SD_EISEN_DATA_TMOUT_ERR		0x0010
#define SD_EISEN_CMD_IDX_ERR		0x0008
#define SD_EISEN_CMD_BIT_ERR		0x0004
#define SD_EISEN_CMD_CRC_ERR		0x0002
#define SD_EISEN_CMD_TMOUT_ERR		0x0001

/* SD_NIXEN_REG */
#define SD_NIXEN_CARD			0x0100
#define SD_NIXEN_REMOVAL		0x0080
#define SD_NIXEN_INSERT			0x0040
#define SD_NIXEN_READ_READY		0x0020
#define SD_NIXEN_WRITE_READY		0x0010
#define SD_NIXEN_DMA			0x0008
#define SD_NIXEN_BLOCK_GAP		0x0004
#define SD_NIXEN_XFR_DONE		0x0002
#define SD_NIXEN_CMD_DONE		0x0001

/* SD_EIXEN_REG */
#define SD_EIXEN_ADMA_ERR		0x0200
#define SD_EIXEN_ACMD12_ERR		0x0100
#define SD_EIXEN_CURRENT_ERR		0x0080
#define SD_EIXEN_DATA_BIT_ERR		0x0040
#define SD_EIXEN_DATA_CRC_ERR		0x0020
#define SD_EIXEN_DATA_TMOUT_ERR		0x0010
#define SD_EIXEN_CMD_IDX_ERR		0x0008
#define SD_EIXEN_CMD_BIT_ERR		0x0004
#define SD_EIXEN_CMD_CRC_ERR		0x0002
#define SD_EIXEN_CMD_TMOUT_ERR		0x0001

/* SD_AC12ES_REG */
#define SD_AC12ES_NOT_ISSUED		0x0080
#define SD_AC12ES_INDEX			0x0010
#define SD_AC12ES_END_BIT		0x0008
#define SD_AC12ES_CRC_ERROR		0x0004
#define SD_AC12ES_TMOUT_ERROR		0x0002
#define SD_AC12ES_NOT_EXECED		0x0001

/* SD_ADMA_STA_REG */
#define SD_ADMA_STA_ST_STOP		0x00000000
#define SD_ADMA_STA_ST_FDS		0x00000001
#define SD_ADMA_STA_ST_TFR		0x00000003
#define SD_ADMA_STA_LEN_ERR		0x00000004

/* SD_CAP_REG */
#define SD_CAP_INTMODE			0x08000000
#define SD_CAP_VOL_1_8V			0x04000000
#define SD_CAP_VOL_3_0V			0x02000000
#define SD_CAP_VOL_3_3V			0x01000000
#define SD_CAP_SUS_RES			0x00800000
#define SD_CAP_DMA			0x00400000
#define SD_CAP_HIGH_SPEED		0x00200000
#define SD_CAP_ADMA_SUPPORT		0x00080000
#define SD_CAP_MAX_512B_BLK		0x00000000
#define SD_CAP_MAX_1KB_BLK		0x00010000
#define SD_CAP_MAX_2KB_BLK		0x00020000
#define SD_CAP_BASE_FREQ(x)		(((x) & 0x3f00) >> 8)
#define SD_CAP_TOCLK_KHZ		0x00000000
#define SD_CAP_TOCLK_MHZ		0x00000080
#define SD_CAP_TOCLK_FREQ(x)		(((x) & 0x3f))

/* SD_XC_CTR_REG */
#define SD_XC_CTR_DDR_EN		0x00008000
#define SD_XC_CTR_VOL_1_8V		0x00000001
#define SD_XC_CTR_VOL_3_3V		0x00000000

/* SD_BOOT_CTR_REG */
#define SD_BOOT_CTR_RST_EN		0x00010000

/* SD_BOOT_STA_REG */
#define SD_BOOT_STA_END_ALT		0x01010000
#define SD_BOOT_STA_BOOT_RDY		0x00000001

/* SD_VOL_SW_REG */
#define SD_VOL_SW_CMD_STAT_H		0x00010000
#define SD_VOL_SW_DAT_STAT_H		0x00000007

/* SD_VER_REG */
#define SD_VER_VENDOR(x)		((x) >> 8)
#define SD_VER_SPEC(x)			((x) & 0xf)

#define SD_ADMA_TBL_LINE_SIZE		(8)
#define SD_ADMA_TBL_LINE_MAX_LEN	(0x40000)
#define SD_ADMA_TBL_ATTR_NOP		(0x0000)
#define SD_ADMA_TBL_ATTR_RSV		(0x0010)
#define SD_ADMA_TBL_ATTR_TRAN		(0x0020)
#define SD_ADMA_TBL_ATTR_LINK		(0x0030)
#define SD_ADMA_TBL_ATTR_WORD		(0x0008)
#define SD_ADMA_TBL_ATTR_INT		(0x0004)
#define SD_ADMA_TBL_ATTR_END		(0x0002)
#define SD_ADMA_TBL_ATTR_VALID		(0x0001)

/* ==========================================================================*/
#define SMIO_2				GPIO(64)
#define SMIO_3				GPIO(65)
#define SMIO_4				GPIO(66)
#define SMIO_5				GPIO(67)
#define SD1_CD				GPIO(67)
#define SMIO_6				GPIO(68)
#define SMIO_38				GPIO(69)
#define SMIO_39				GPIO(70)
#define SMIO_40				GPIO(71)
#define SMIO_41				GPIO(72)
#define SMIO_42				GPIO(73)
#define SMIO_43				GPIO(74)
#define SMIO_44				GPIO(75)
#define SMIO_45				GPIO(76)

/* ==========================================================================*/
#define sd_slot_is_valid(slot)		((SD_SUPPORT_SDXC == 1) && ((slot) == 2) ? 1 : \
					(SD_SUPPORT_SDIO == 1) && ((slot) == 1) ? 1 : \
					((slot) == 0) ? 1 : 0)

/* ==========================================================================*/
#ifndef __ASM__
/* ==========================================================================*/

#if (CHIP_REV == S2L)
#define MINIPIN_EMMC_ALTFUNC		2
#define MINIPIN_EMMC_PIN		{57, 58, 59, 60, 73, 74, 75, 76, \
					 77, 78, 79, 80}
#elif (CHIP_REV == S3)
#define MINIPIN_EMMC_ALTFUNC		2
#define MINIPIN_EMMC_PIN		{118, 119, 120, 121, 134, 135, 136, \
					 137, 138, 139, 140, 141, 154}
#elif (CHIP_REV == S3L)
#define MINIPIN_EMMC_ALTFUNC		2
#define MINIPIN_EMMC_PIN		{58, 59, 60, 61, 74, 75, 76, 77, \
					 78, 79, 80, 81, 94}
#elif (CHIP_REV == S5)
#define MINIPIN_EMMC_ALTFUNC		2
#define MINIPIN_EMMC_PIN		{59, 60, 61, 62, 75, 76, 77, 78, \
					 79, 80, 81, 82, 95}
#elif (CHIP_REV == S5L)
#define MINIPIN_EMMC_ALTFUNC		2
#define MINIPIN_EMMC_PIN		{75, 76, 77, 78, 91, 92, 93, 94, \
					 95, 96, 97, 98, 111}
#elif (CHIP_REV == CV1)
#define MINIPIN_EMMC_ALTFUNC		2
#define MINIPIN_EMMC_PIN		{89, 90, 91, 92, 105, 106, 107, 108, \
					 109, 110, 111, 112, 125}
#else
#error "MINIPIN emmc: not defined!"
#endif

extern int amboot_bsp_sd_slot_init(int slot, int volt) __attribute__ ((weak));
extern int amboot_bsp_sd_phy_init(int slot, int mode) __attribute__ ((weak));

/* ==========================================================================*/
#endif
/* ==========================================================================*/

#endif

