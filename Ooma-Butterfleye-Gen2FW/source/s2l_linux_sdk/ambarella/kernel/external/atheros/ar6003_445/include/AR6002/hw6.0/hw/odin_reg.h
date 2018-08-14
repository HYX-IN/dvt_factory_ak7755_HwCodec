// ------------------------------------------------------------------
// Copyright (c) 2004-2007 Atheros Corporation.  All rights reserved.
// 
// The software source and binaries included in this development package are
// licensed, not sold. You, or your company, received the package under one
// or more license agreements. The rights granted to you are specifically
// listed in these license agreement(s). All other rights remain with Atheros
// Communications, Inc., its subsidiaries, or the respective owner including
// those listed on the included copyright notices.  Distribution of any
// portion of this package must be in strict compliance with the license
// agreement(s) terms.
// </copyright>
// 
// <summary>
// 	Wifi driver for AR6002
// </summary>
//
// ------------------------------------------------------------------
//===================================================================
// Author(s): ="Atheros"
//===================================================================

/* Copyright (C) 2010 Denali Software Inc.  All rights reserved              */
/* THIS FILE IS AUTOMATICALLY GENERATED BY DENALI BLUEPRINT, DO NOT EDIT     */


#ifndef _ODIN_REG_REG_H_
#define _ODIN_REG_REG_H_


/* macros for PHY_CTRL0 */
#define PHY_CTRL0_ADDRESS                                                                 0x00000000
#define PHY_CTRL0_OFFSET                                                                  0x00000000
#define PHY_CTRL0_PLL_ICP_MSB                                                                      2
#define PHY_CTRL0_PLL_ICP_LSB                                                                      0
#define PHY_CTRL0_PLL_ICP_MASK                                                            0x00000007
#define PHY_CTRL0_PLL_ICP_GET(x)                                           (((x) & 0x00000007) >> 0)
#define PHY_CTRL0_PLL_ICP_SET(x)                                           (((x) << 0) & 0x00000007)
#define PHY_CTRL0_PLL_RS_MSB                                                                       5
#define PHY_CTRL0_PLL_RS_LSB                                                                       3
#define PHY_CTRL0_PLL_RS_MASK                                                             0x00000038
#define PHY_CTRL0_PLL_RS_GET(x)                                            (((x) & 0x00000038) >> 3)
#define PHY_CTRL0_PLL_RS_SET(x)                                            (((x) << 3) & 0x00000038)
#define PHY_CTRL0_PLL_DIV_MSB                                                                     14
#define PHY_CTRL0_PLL_DIV_LSB                                                                      6
#define PHY_CTRL0_PLL_DIV_MASK                                                            0x00007fc0
#define PHY_CTRL0_PLL_DIV_GET(x)                                           (((x) & 0x00007fc0) >> 6)
#define PHY_CTRL0_PLL_DIV_SET(x)                                           (((x) << 6) & 0x00007fc0)
#define PHY_CTRL0_PLL_MOD_MSB                                                                     17
#define PHY_CTRL0_PLL_MOD_LSB                                                                     15
#define PHY_CTRL0_PLL_MOD_MASK                                                            0x00038000
#define PHY_CTRL0_PLL_MOD_GET(x)                                          (((x) & 0x00038000) >> 15)
#define PHY_CTRL0_PLL_MOD_SET(x)                                          (((x) << 15) & 0x00038000)
#define PHY_CTRL0_PLL_OVERIDE_MSB                                                                 18
#define PHY_CTRL0_PLL_OVERIDE_LSB                                                                 18
#define PHY_CTRL0_PLL_OVERIDE_MASK                                                        0x00040000
#define PHY_CTRL0_PLL_OVERIDE_GET(x)                                      (((x) & 0x00040000) >> 18)
#define PHY_CTRL0_PLL_OVERIDE_SET(x)                                      (((x) << 18) & 0x00040000)
#define PHY_CTRL0_TEST_SPEED_SELECT_MSB                                                           19
#define PHY_CTRL0_TEST_SPEED_SELECT_LSB                                                           19
#define PHY_CTRL0_TEST_SPEED_SELECT_MASK                                                  0x00080000
#define PHY_CTRL0_TEST_SPEED_SELECT_GET(x)                                (((x) & 0x00080000) >> 19)
#define PHY_CTRL0_TEST_SPEED_SELECT_SET(x)                                (((x) << 19) & 0x00080000)
#define PHY_CTRL0_RX_PATTERN_EN_MSB                                                               20
#define PHY_CTRL0_RX_PATTERN_EN_LSB                                                               20
#define PHY_CTRL0_RX_PATTERN_EN_MASK                                                      0x00100000
#define PHY_CTRL0_RX_PATTERN_EN_GET(x)                                    (((x) & 0x00100000) >> 20)
#define PHY_CTRL0_RX_PATTERN_EN_SET(x)                                    (((x) << 20) & 0x00100000)
#define PHY_CTRL0_TX_PATTERN_EN_MSB                                                               21
#define PHY_CTRL0_TX_PATTERN_EN_LSB                                                               21
#define PHY_CTRL0_TX_PATTERN_EN_MASK                                                      0x00200000
#define PHY_CTRL0_TX_PATTERN_EN_GET(x)                                    (((x) & 0x00200000) >> 21)
#define PHY_CTRL0_TX_PATTERN_EN_SET(x)                                    (((x) << 21) & 0x00200000)
#define PHY_CTRL0_ANA_LOOPBACK_EN_MSB                                                             22
#define PHY_CTRL0_ANA_LOOPBACK_EN_LSB                                                             22
#define PHY_CTRL0_ANA_LOOPBACK_EN_MASK                                                    0x00400000
#define PHY_CTRL0_ANA_LOOPBACK_EN_GET(x)                                  (((x) & 0x00400000) >> 22)
#define PHY_CTRL0_ANA_LOOPBACK_EN_SET(x)                                  (((x) << 22) & 0x00400000)
#define PHY_CTRL0_DIG_LOOPBACK_EN_MSB                                                             23
#define PHY_CTRL0_DIG_LOOPBACK_EN_LSB                                                             23
#define PHY_CTRL0_DIG_LOOPBACK_EN_MASK                                                    0x00800000
#define PHY_CTRL0_DIG_LOOPBACK_EN_GET(x)                                  (((x) & 0x00800000) >> 23)
#define PHY_CTRL0_DIG_LOOPBACK_EN_SET(x)                                  (((x) << 23) & 0x00800000)
#define PHY_CTRL0_LOOPBACK_ERR_CNT_MSB                                                            31
#define PHY_CTRL0_LOOPBACK_ERR_CNT_LSB                                                            24
#define PHY_CTRL0_LOOPBACK_ERR_CNT_MASK                                                   0xff000000
#define PHY_CTRL0_LOOPBACK_ERR_CNT_GET(x)                                 (((x) & 0xff000000) >> 24)

/* macros for PHY_CTRL1 */
#define PHY_CTRL1_ADDRESS                                                                 0x00000004
#define PHY_CTRL1_OFFSET                                                                  0x00000004
#define PHY_CTRL1_RX_FILBW_SEL_MSB                                                                 1
#define PHY_CTRL1_RX_FILBW_SEL_LSB                                                                 0
#define PHY_CTRL1_RX_FILBW_SEL_MASK                                                       0x00000003
#define PHY_CTRL1_RX_FILBW_SEL_GET(x)                                      (((x) & 0x00000003) >> 0)
#define PHY_CTRL1_RX_FILBW_SEL_SET(x)                                      (((x) << 0) & 0x00000003)
#define PHY_CTRL1_RX_FORCERXON_MSB                                                                 2
#define PHY_CTRL1_RX_FORCERXON_LSB                                                                 2
#define PHY_CTRL1_RX_FORCERXON_MASK                                                       0x00000004
#define PHY_CTRL1_RX_FORCERXON_GET(x)                                      (((x) & 0x00000004) >> 2)
#define PHY_CTRL1_RX_FORCERXON_SET(x)                                      (((x) << 2) & 0x00000004)
#define PHY_CTRL1_RX_BYPASSEQ_MSB                                                                  3
#define PHY_CTRL1_RX_BYPASSEQ_LSB                                                                  3
#define PHY_CTRL1_RX_BYPASSEQ_MASK                                                        0x00000008
#define PHY_CTRL1_RX_BYPASSEQ_GET(x)                                       (((x) & 0x00000008) >> 3)
#define PHY_CTRL1_RX_BYPASSEQ_SET(x)                                       (((x) << 3) & 0x00000008)
#define PHY_CTRL1_RX_LOWR_PDET_MSB                                                                 4
#define PHY_CTRL1_RX_LOWR_PDET_LSB                                                                 4
#define PHY_CTRL1_RX_LOWR_PDET_MASK                                                       0x00000010
#define PHY_CTRL1_RX_LOWR_PDET_GET(x)                                      (((x) & 0x00000010) >> 4)
#define PHY_CTRL1_RX_LOWR_PDET_SET(x)                                      (((x) << 4) & 0x00000010)
#define PHY_CTRL1_RX_SELIR_100M_MSB                                                                6
#define PHY_CTRL1_RX_SELIR_100M_LSB                                                                5
#define PHY_CTRL1_RX_SELIR_100M_MASK                                                      0x00000060
#define PHY_CTRL1_RX_SELIR_100M_GET(x)                                     (((x) & 0x00000060) >> 5)
#define PHY_CTRL1_RX_SELIR_100M_SET(x)                                     (((x) << 5) & 0x00000060)
#define PHY_CTRL1_RX_SELVREF0P6_MSB                                                                7
#define PHY_CTRL1_RX_SELVREF0P6_LSB                                                                7
#define PHY_CTRL1_RX_SELVREF0P6_MASK                                                      0x00000080
#define PHY_CTRL1_RX_SELVREF0P6_GET(x)                                     (((x) & 0x00000080) >> 7)
#define PHY_CTRL1_RX_SELVREF0P6_SET(x)                                     (((x) << 7) & 0x00000080)
#define PHY_CTRL1_RX_SELVREF0P25_MSB                                                               8
#define PHY_CTRL1_RX_SELVREF0P25_LSB                                                               8
#define PHY_CTRL1_RX_SELVREF0P25_MASK                                                     0x00000100
#define PHY_CTRL1_RX_SELVREF0P25_GET(x)                                    (((x) & 0x00000100) >> 8)
#define PHY_CTRL1_RX_SELVREF0P25_SET(x)                                    (((x) << 8) & 0x00000100)
#define PHY_CTRL1_RX_RSVD_MSB                                                                     11
#define PHY_CTRL1_RX_RSVD_LSB                                                                      9
#define PHY_CTRL1_RX_RSVD_MASK                                                            0x00000e00
#define PHY_CTRL1_RX_RSVD_GET(x)                                           (((x) & 0x00000e00) >> 9)
#define PHY_CTRL1_RX_RSVD_SET(x)                                           (((x) << 9) & 0x00000e00)
#define PHY_CTRL1_NO_PLL_PWD_MSB                                                                  12
#define PHY_CTRL1_NO_PLL_PWD_LSB                                                                  12
#define PHY_CTRL1_NO_PLL_PWD_MASK                                                         0x00001000
#define PHY_CTRL1_NO_PLL_PWD_GET(x)                                       (((x) & 0x00001000) >> 12)
#define PHY_CTRL1_NO_PLL_PWD_SET(x)                                       (((x) << 12) & 0x00001000)
#define PHY_CTRL1_FORCE_SUSPEND_MSB                                                               13
#define PHY_CTRL1_FORCE_SUSPEND_LSB                                                               13
#define PHY_CTRL1_FORCE_SUSPEND_MASK                                                      0x00002000
#define PHY_CTRL1_FORCE_SUSPEND_GET(x)                                    (((x) & 0x00002000) >> 13)
#define PHY_CTRL1_FORCE_SUSPEND_SET(x)                                    (((x) << 13) & 0x00002000)
#define PHY_CTRL1_TX_PATTERN_SEL_MSB                                                              19
#define PHY_CTRL1_TX_PATTERN_SEL_LSB                                                              18
#define PHY_CTRL1_TX_PATTERN_SEL_MASK                                                     0x000c0000
#define PHY_CTRL1_TX_PATTERN_SEL_GET(x)                                   (((x) & 0x000c0000) >> 18)
#define PHY_CTRL1_TX_PATTERN_SEL_SET(x)                                   (((x) << 18) & 0x000c0000)
#define PHY_CTRL1_USE_PLL_LOCKDETECT_MSB                                                          20
#define PHY_CTRL1_USE_PLL_LOCKDETECT_LSB                                                          20
#define PHY_CTRL1_USE_PLL_LOCKDETECT_MASK                                                 0x00100000
#define PHY_CTRL1_USE_PLL_LOCKDETECT_GET(x)                               (((x) & 0x00100000) >> 20)
#define PHY_CTRL1_USE_PLL_LOCKDETECT_SET(x)                               (((x) << 20) & 0x00100000)
#define PHY_CTRL1_USE_PLL_LOCK_DLY_SEL_MSB                                                        22
#define PHY_CTRL1_USE_PLL_LOCK_DLY_SEL_LSB                                                        21
#define PHY_CTRL1_USE_PLL_LOCK_DLY_SEL_MASK                                               0x00600000
#define PHY_CTRL1_USE_PLL_LOCK_DLY_SEL_GET(x)                             (((x) & 0x00600000) >> 21)
#define PHY_CTRL1_USE_PLL_LOCK_DLY_SEL_SET(x)                             (((x) << 21) & 0x00600000)
#define PHY_CTRL1_CLKOBS_SEL_MSB                                                                  25
#define PHY_CTRL1_CLKOBS_SEL_LSB                                                                  23
#define PHY_CTRL1_CLKOBS_SEL_MASK                                                         0x03800000
#define PHY_CTRL1_CLKOBS_SEL_GET(x)                                       (((x) & 0x03800000) >> 23)
#define PHY_CTRL1_CLKOBS_SEL_SET(x)                                       (((x) << 23) & 0x03800000)
#define PHY_CTRL1_ENABLE_REFCLK_GATE_MSB                                                          26
#define PHY_CTRL1_ENABLE_REFCLK_GATE_LSB                                                          26
#define PHY_CTRL1_ENABLE_REFCLK_GATE_MASK                                                 0x04000000
#define PHY_CTRL1_ENABLE_REFCLK_GATE_GET(x)                               (((x) & 0x04000000) >> 26)
#define PHY_CTRL1_ENABLE_REFCLK_GATE_SET(x)                               (((x) << 26) & 0x04000000)
#define PHY_CTRL1_DISABLE_CLK_GATING_MSB                                                          27
#define PHY_CTRL1_DISABLE_CLK_GATING_LSB                                                          27
#define PHY_CTRL1_DISABLE_CLK_GATING_MASK                                                 0x08000000
#define PHY_CTRL1_DISABLE_CLK_GATING_GET(x)                               (((x) & 0x08000000) >> 27)
#define PHY_CTRL1_DISABLE_CLK_GATING_SET(x)                               (((x) << 27) & 0x08000000)
#define PHY_CTRL1_PLL_OBS_MODE_N_MSB                                                              31
#define PHY_CTRL1_PLL_OBS_MODE_N_LSB                                                              31
#define PHY_CTRL1_PLL_OBS_MODE_N_MASK                                                     0x80000000
#define PHY_CTRL1_PLL_OBS_MODE_N_GET(x)                                   (((x) & 0x80000000) >> 31)
#define PHY_CTRL1_PLL_OBS_MODE_N_SET(x)                                   (((x) << 31) & 0x80000000)

/* macros for PHY_CTRL2 */
#define PHY_CTRL2_ADDRESS                                                                 0x00000008
#define PHY_CTRL2_OFFSET                                                                  0x00000008
#define PHY_CTRL2_HSTXBIAS_PS_EN_MSB                                                               0
#define PHY_CTRL2_HSTXBIAS_PS_EN_LSB                                                               0
#define PHY_CTRL2_HSTXBIAS_PS_EN_MASK                                                     0x00000001
#define PHY_CTRL2_HSTXBIAS_PS_EN_GET(x)                                    (((x) & 0x00000001) >> 0)
#define PHY_CTRL2_HSTXBIAS_PS_EN_SET(x)                                    (((x) << 0) & 0x00000001)
#define PHY_CTRL2_HSRXPHASE_PS_EN_MSB                                                              1
#define PHY_CTRL2_HSRXPHASE_PS_EN_LSB                                                              1
#define PHY_CTRL2_HSRXPHASE_PS_EN_MASK                                                    0x00000002
#define PHY_CTRL2_HSRXPHASE_PS_EN_GET(x)                                   (((x) & 0x00000002) >> 1)
#define PHY_CTRL2_HSRXPHASE_PS_EN_SET(x)                                   (((x) << 1) & 0x00000002)
#define PHY_CTRL2_PWD_IPLL_MSB                                                                     7
#define PHY_CTRL2_PWD_IPLL_LSB                                                                     2
#define PHY_CTRL2_PWD_IPLL_MASK                                                           0x000000fc
#define PHY_CTRL2_PWD_IPLL_GET(x)                                          (((x) & 0x000000fc) >> 2)
#define PHY_CTRL2_PWD_IPLL_SET(x)                                          (((x) << 2) & 0x000000fc)
#define PHY_CTRL2_PWD_ISP_MSB                                                                     13
#define PHY_CTRL2_PWD_ISP_LSB                                                                      8
#define PHY_CTRL2_PWD_ISP_MASK                                                            0x00003f00
#define PHY_CTRL2_PWD_ISP_GET(x)                                           (((x) & 0x00003f00) >> 8)
#define PHY_CTRL2_PWD_ISP_SET(x)                                           (((x) << 8) & 0x00003f00)
#define PHY_CTRL2_TX_CAL_EN_MSB                                                                   20
#define PHY_CTRL2_TX_CAL_EN_LSB                                                                   20
#define PHY_CTRL2_TX_CAL_EN_MASK                                                          0x00100000
#define PHY_CTRL2_TX_CAL_EN_GET(x)                                        (((x) & 0x00100000) >> 20)
#define PHY_CTRL2_TX_CAL_EN_SET(x)                                        (((x) << 20) & 0x00100000)
#define PHY_CTRL2_TX_CAL_SEL_MSB                                                                  21
#define PHY_CTRL2_TX_CAL_SEL_LSB                                                                  21
#define PHY_CTRL2_TX_CAL_SEL_MASK                                                         0x00200000
#define PHY_CTRL2_TX_CAL_SEL_GET(x)                                       (((x) & 0x00200000) >> 21)
#define PHY_CTRL2_TX_CAL_SEL_SET(x)                                       (((x) << 21) & 0x00200000)
#define PHY_CTRL2_TX_MAN_CAL_MSB                                                                  25
#define PHY_CTRL2_TX_MAN_CAL_LSB                                                                  22
#define PHY_CTRL2_TX_MAN_CAL_MASK                                                         0x03c00000
#define PHY_CTRL2_TX_MAN_CAL_GET(x)                                       (((x) & 0x03c00000) >> 22)
#define PHY_CTRL2_TX_MAN_CAL_SET(x)                                       (((x) << 22) & 0x03c00000)
#define PHY_CTRL2_TX_LCKDET_OVR_MSB                                                               26
#define PHY_CTRL2_TX_LCKDET_OVR_LSB                                                               26
#define PHY_CTRL2_TX_LCKDET_OVR_MASK                                                      0x04000000
#define PHY_CTRL2_TX_LCKDET_OVR_GET(x)                                    (((x) & 0x04000000) >> 26)
#define PHY_CTRL2_TX_LCKDET_OVR_SET(x)                                    (((x) << 26) & 0x04000000)
#define PHY_CTRL2_TX_RSVD_MSB                                                                     30
#define PHY_CTRL2_TX_RSVD_LSB                                                                     27
#define PHY_CTRL2_TX_RSVD_MASK                                                            0x78000000
#define PHY_CTRL2_TX_RSVD_GET(x)                                          (((x) & 0x78000000) >> 27)
#define PHY_CTRL2_TX_RSVD_SET(x)                                          (((x) << 27) & 0x78000000)
#define PHY_CTRL2_PWD_EXTBIAS_MSB                                                                 31
#define PHY_CTRL2_PWD_EXTBIAS_LSB                                                                 31
#define PHY_CTRL2_PWD_EXTBIAS_MASK                                                        0x80000000
#define PHY_CTRL2_PWD_EXTBIAS_GET(x)                                      (((x) & 0x80000000) >> 31)
#define PHY_CTRL2_PWD_EXTBIAS_SET(x)                                      (((x) << 31) & 0x80000000)

/* macros for PHY_CTRL3 */
#define PHY_CTRL3_ADDRESS                                                                 0x0000000c
#define PHY_CTRL3_OFFSET                                                                  0x0000000c
#define PHY_CTRL3_PWD_ITX_MSB                                                                     18
#define PHY_CTRL3_PWD_ITX_LSB                                                                      0
#define PHY_CTRL3_PWD_ITX_MASK                                                            0x0007ffff
#define PHY_CTRL3_PWD_ITX_GET(x)                                           (((x) & 0x0007ffff) >> 0)
#define PHY_CTRL3_PWD_ITX_SET(x)                                           (((x) << 0) & 0x0007ffff)
#define PHY_CTRL3_TX_DISABLE_SHORT_DET_MSB                                                        21
#define PHY_CTRL3_TX_DISABLE_SHORT_DET_LSB                                                        21
#define PHY_CTRL3_TX_DISABLE_SHORT_DET_MASK                                               0x00200000
#define PHY_CTRL3_TX_DISABLE_SHORT_DET_GET(x)                             (((x) & 0x00200000) >> 21)
#define PHY_CTRL3_TX_DISABLE_SHORT_DET_SET(x)                             (((x) << 21) & 0x00200000)
#define PHY_CTRL3_TX_SELTEST_MSB                                                                  24
#define PHY_CTRL3_TX_SELTEST_LSB                                                                  22
#define PHY_CTRL3_TX_SELTEST_MASK                                                         0x01c00000
#define PHY_CTRL3_TX_SELTEST_GET(x)                                       (((x) & 0x01c00000) >> 22)
#define PHY_CTRL3_TX_SELTEST_SET(x)                                       (((x) << 22) & 0x01c00000)
#define PHY_CTRL3_TX_STARTCAL_MSB                                                                 25
#define PHY_CTRL3_TX_STARTCAL_LSB                                                                 25
#define PHY_CTRL3_TX_STARTCAL_MASK                                                        0x02000000
#define PHY_CTRL3_TX_STARTCAL_GET(x)                                      (((x) & 0x02000000) >> 25)
#define PHY_CTRL3_TX_STARTCAL_SET(x)                                      (((x) << 25) & 0x02000000)

/* macros for PHY_CTRL4 */
#define PHY_CTRL4_ADDRESS                                                                 0x00000010
#define PHY_CTRL4_OFFSET                                                                  0x00000010
#define PHY_CTRL4_PWD_IRX_MSB                                                                     11
#define PHY_CTRL4_PWD_IRX_LSB                                                                      0
#define PHY_CTRL4_PWD_IRX_MASK                                                            0x00000fff
#define PHY_CTRL4_PWD_IRX_GET(x)                                           (((x) & 0x00000fff) >> 0)
#define PHY_CTRL4_PWD_IRX_SET(x)                                           (((x) << 0) & 0x00000fff)

/* macros for PHY_CTRL5 */
#define PHY_CTRL5_ADDRESS                                                                 0x00000014
#define PHY_CTRL5_OFFSET                                                                  0x00000014
#define PHY_CTRL5_TX_BIAS_DELAY_MSB                                                                6
#define PHY_CTRL5_TX_BIAS_DELAY_LSB                                                                0
#define PHY_CTRL5_TX_BIAS_DELAY_MASK                                                      0x0000007f
#define PHY_CTRL5_TX_BIAS_DELAY_GET(x)                                     (((x) & 0x0000007f) >> 0)
#define PHY_CTRL5_TX_BIAS_DELAY_SET(x)                                     (((x) << 0) & 0x0000007f)
#define PHY_CTRL5_EB_WATERMARK_MSB                                                                12
#define PHY_CTRL5_EB_WATERMARK_LSB                                                                 7
#define PHY_CTRL5_EB_WATERMARK_MASK                                                       0x00001f80
#define PHY_CTRL5_EB_WATERMARK_GET(x)                                      (((x) & 0x00001f80) >> 7)
#define PHY_CTRL5_EB_WATERMARK_SET(x)                                      (((x) << 7) & 0x00001f80)
#define PHY_CTRL5_FORCE_IDDQ_MSB                                                                  13
#define PHY_CTRL5_FORCE_IDDQ_LSB                                                                  13
#define PHY_CTRL5_FORCE_IDDQ_MASK                                                         0x00002000
#define PHY_CTRL5_FORCE_IDDQ_GET(x)                                       (((x) & 0x00002000) >> 13)
#define PHY_CTRL5_FORCE_IDDQ_SET(x)                                       (((x) << 13) & 0x00002000)
#define PHY_CTRL5_FORCE_TEST_J_MSB                                                                14
#define PHY_CTRL5_FORCE_TEST_J_LSB                                                                14
#define PHY_CTRL5_FORCE_TEST_J_MASK                                                       0x00004000
#define PHY_CTRL5_FORCE_TEST_J_GET(x)                                     (((x) & 0x00004000) >> 14)
#define PHY_CTRL5_FORCE_TEST_J_SET(x)                                     (((x) << 14) & 0x00004000)
#define PHY_CTRL5_FORCE_TEST_K_MSB                                                                15
#define PHY_CTRL5_FORCE_TEST_K_LSB                                                                15
#define PHY_CTRL5_FORCE_TEST_K_MASK                                                       0x00008000
#define PHY_CTRL5_FORCE_TEST_K_GET(x)                                     (((x) & 0x00008000) >> 15)
#define PHY_CTRL5_FORCE_TEST_K_SET(x)                                     (((x) << 15) & 0x00008000)
#define PHY_CTRL5_FORCE_TEST_SE0_NAK_MSB                                                          16
#define PHY_CTRL5_FORCE_TEST_SE0_NAK_LSB                                                          16
#define PHY_CTRL5_FORCE_TEST_SE0_NAK_MASK                                                 0x00010000
#define PHY_CTRL5_FORCE_TEST_SE0_NAK_GET(x)                               (((x) & 0x00010000) >> 16)
#define PHY_CTRL5_FORCE_TEST_SE0_NAK_SET(x)                               (((x) << 16) & 0x00010000)
#define PHY_CTRL5_TEST_JK_OVERRIDE_MSB                                                            17
#define PHY_CTRL5_TEST_JK_OVERRIDE_LSB                                                            17
#define PHY_CTRL5_TEST_JK_OVERRIDE_MASK                                                   0x00020000
#define PHY_CTRL5_TEST_JK_OVERRIDE_GET(x)                                 (((x) & 0x00020000) >> 17)
#define PHY_CTRL5_TEST_JK_OVERRIDE_SET(x)                                 (((x) << 17) & 0x00020000)
#define PHY_CTRL5_XCVR_SEL_MSB                                                                    19
#define PHY_CTRL5_XCVR_SEL_LSB                                                                    18
#define PHY_CTRL5_XCVR_SEL_MASK                                                           0x000c0000
#define PHY_CTRL5_XCVR_SEL_GET(x)                                         (((x) & 0x000c0000) >> 18)
#define PHY_CTRL5_XCVR_SEL_SET(x)                                         (((x) << 18) & 0x000c0000)
#define PHY_CTRL5_TERM_SEL_MSB                                                                    20
#define PHY_CTRL5_TERM_SEL_LSB                                                                    20
#define PHY_CTRL5_TERM_SEL_MASK                                                           0x00100000
#define PHY_CTRL5_TERM_SEL_GET(x)                                         (((x) & 0x00100000) >> 20)
#define PHY_CTRL5_TERM_SEL_SET(x)                                         (((x) << 20) & 0x00100000)
#define PHY_CTRL5_SUSPEND_N_MSB                                                                   21
#define PHY_CTRL5_SUSPEND_N_LSB                                                                   21
#define PHY_CTRL5_SUSPEND_N_MASK                                                          0x00200000
#define PHY_CTRL5_SUSPEND_N_GET(x)                                        (((x) & 0x00200000) >> 21)
#define PHY_CTRL5_SUSPEND_N_SET(x)                                        (((x) << 21) & 0x00200000)
#define PHY_CTRL5_DP_PULLDOWN_MSB                                                                 22
#define PHY_CTRL5_DP_PULLDOWN_LSB                                                                 22
#define PHY_CTRL5_DP_PULLDOWN_MASK                                                        0x00400000
#define PHY_CTRL5_DP_PULLDOWN_GET(x)                                      (((x) & 0x00400000) >> 22)
#define PHY_CTRL5_DP_PULLDOWN_SET(x)                                      (((x) << 22) & 0x00400000)
#define PHY_CTRL5_DM_PULLDOWN_MSB                                                                 23
#define PHY_CTRL5_DM_PULLDOWN_LSB                                                                 23
#define PHY_CTRL5_DM_PULLDOWN_MASK                                                        0x00800000
#define PHY_CTRL5_DM_PULLDOWN_GET(x)                                      (((x) & 0x00800000) >> 23)
#define PHY_CTRL5_DM_PULLDOWN_SET(x)                                      (((x) << 23) & 0x00800000)
#define PHY_CTRL5_HOST_DISCON_FIX_ON_MSB                                                          24
#define PHY_CTRL5_HOST_DISCON_FIX_ON_LSB                                                          24
#define PHY_CTRL5_HOST_DISCON_FIX_ON_MASK                                                 0x01000000
#define PHY_CTRL5_HOST_DISCON_FIX_ON_GET(x)                               (((x) & 0x01000000) >> 24)
#define PHY_CTRL5_HOST_DISCON_FIX_ON_SET(x)                               (((x) << 24) & 0x01000000)
#define PHY_CTRL5_HOST_DISCON_DETECT_ON_MSB                                                       25
#define PHY_CTRL5_HOST_DISCON_DETECT_ON_LSB                                                       25
#define PHY_CTRL5_HOST_DISCON_DETECT_ON_MASK                                              0x02000000
#define PHY_CTRL5_HOST_DISCON_DETECT_ON_GET(x)                            (((x) & 0x02000000) >> 25)
#define PHY_CTRL5_HOST_DISCON_DETECT_ON_SET(x)                            (((x) << 25) & 0x02000000)
#define PHY_CTRL5_HOST_DISCON_SAMPLE_WIDTH_MSB                                                    28
#define PHY_CTRL5_HOST_DISCON_SAMPLE_WIDTH_LSB                                                    26
#define PHY_CTRL5_HOST_DISCON_SAMPLE_WIDTH_MASK                                           0x1c000000
#define PHY_CTRL5_HOST_DISCON_SAMPLE_WIDTH_GET(x)                         (((x) & 0x1c000000) >> 26)
#define PHY_CTRL5_HOST_DISCON_SAMPLE_WIDTH_SET(x)                         (((x) << 26) & 0x1c000000)

/* macros for PHY_CTRL6 */
#define PHY_CTRL6_ADDRESS                                                                 0x00000018
#define PHY_CTRL6_OFFSET                                                                  0x00000018
#define PHY_CTRL6_AVALID_MSB                                                                       0
#define PHY_CTRL6_AVALID_LSB                                                                       0
#define PHY_CTRL6_AVALID_MASK                                                             0x00000001
#define PHY_CTRL6_AVALID_GET(x)                                            (((x) & 0x00000001) >> 0)
#define PHY_CTRL6_AVALID_SET(x)                                            (((x) << 0) & 0x00000001)
#define PHY_CTRL6_BVALID_MSB                                                                       1
#define PHY_CTRL6_BVALID_LSB                                                                       1
#define PHY_CTRL6_BVALID_MASK                                                             0x00000002
#define PHY_CTRL6_BVALID_GET(x)                                            (((x) & 0x00000002) >> 1)
#define PHY_CTRL6_BVALID_SET(x)                                            (((x) << 1) & 0x00000002)
#define PHY_CTRL6_VBUSVALID_MSB                                                                    2
#define PHY_CTRL6_VBUSVALID_LSB                                                                    2
#define PHY_CTRL6_VBUSVALID_MASK                                                          0x00000004
#define PHY_CTRL6_VBUSVALID_GET(x)                                         (((x) & 0x00000004) >> 2)
#define PHY_CTRL6_VBUSVALID_SET(x)                                         (((x) << 2) & 0x00000004)
#define PHY_CTRL6_SESSEND_MSB                                                                      3
#define PHY_CTRL6_SESSEND_LSB                                                                      3
#define PHY_CTRL6_SESSEND_MASK                                                            0x00000008
#define PHY_CTRL6_SESSEND_GET(x)                                           (((x) & 0x00000008) >> 3)
#define PHY_CTRL6_SESSEND_SET(x)                                           (((x) << 3) & 0x00000008)
#define PHY_CTRL6_IDDIG_MSB                                                                        4
#define PHY_CTRL6_IDDIG_LSB                                                                        4
#define PHY_CTRL6_IDDIG_MASK                                                              0x00000010
#define PHY_CTRL6_IDDIG_GET(x)                                             (((x) & 0x00000010) >> 4)
#define PHY_CTRL6_IDDIG_SET(x)                                             (((x) << 4) & 0x00000010)

/* macros for PHY_STATUS */
#define PHY_STATUS_ADDRESS                                                                0x0000001c
#define PHY_STATUS_OFFSET                                                                 0x0000001c
#define PHY_STATUS_TX_CAL_MSB                                                                      3
#define PHY_STATUS_TX_CAL_LSB                                                                      0
#define PHY_STATUS_TX_CAL_MASK                                                            0x0000000f
#define PHY_STATUS_TX_CAL_GET(x)                                           (((x) & 0x0000000f) >> 0)


#ifndef __ASSEMBLER__

typedef struct odin_reg_reg_s {
  volatile unsigned int PHY_CTRL0;                                     /*        0x0 - 0x4        */
  volatile unsigned int PHY_CTRL1;                                     /*        0x4 - 0x8        */
  volatile unsigned int PHY_CTRL2;                                     /*        0x8 - 0xc        */
  volatile unsigned int PHY_CTRL3;                                     /*        0xc - 0x10       */
  volatile unsigned int PHY_CTRL4;                                     /*       0x10 - 0x14       */
  volatile unsigned int PHY_CTRL5;                                     /*       0x14 - 0x18       */
  volatile unsigned int PHY_CTRL6;                                     /*       0x18 - 0x1c       */
  volatile unsigned int PHY_STATUS;                                    /*       0x1c - 0x20       */
} odin_reg_reg_t;

#endif /* __ASSEMBLER__ */

#endif /* _ODIN_REG_REG_H_ */
