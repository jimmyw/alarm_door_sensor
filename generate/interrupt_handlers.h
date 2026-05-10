/*
* Copyright (c) 2012 (2025) Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/
/************************************************************************/
/* Header file generated from device file:                              */
/*    DR5F1026A.DVF                                                     */
/*    V1.12 (2012/04/03)                                                */
/* Tool Version: 4.0.26                                                 */
/* Date Generated: 2025/09/30                                           */
/************************************************************************/
#ifndef INTERRUPT_HANDLERS_H
#define INTERRUPT_HANDLERS_H

/*
 * INT_WDTI (0x4)
 */
void INT_WDTI(void) __attribute__ ((interrupt));

/*
 * INT_LVI (0x6)
 */
void INT_LVI(void) __attribute__ ((interrupt));

/*
 * INT_P0 (0x8)
 */
void INT_P0(void) __attribute__ ((interrupt));

/*
 * INT_P1 (0xA)
 */
void INT_P1(void) __attribute__ ((interrupt));

/*
 * INT_P2 (0xC)
 */
void INT_P2(void) __attribute__ ((interrupt));

/*
 * INT_P3 (0xE)
 */
void INT_P3(void) __attribute__ ((interrupt));

/*
 * INT_DMA0 (0x10)
 */
void INT_DMA0(void) __attribute__ ((interrupt));

/*
 * INT_DMA1 (0x12)
 */
void INT_DMA1(void) __attribute__ ((interrupt));

/*
 * INT_CSI00/INT_IIC00/INT_ST0 (0x14)
 */
void INT_ST0(void) __attribute__ ((interrupt));
//void INT_CSI00(void) __attribute__ ((interrupt));
//void INT_IIC00(void) __attribute__ ((interrupt));

/*
 * INT_CSI01/INT_IIC01/INT_SR0 (0x16)
 */
void INT_SR0(void) __attribute__ ((interrupt));
//void INT_CSI01(void) __attribute__ ((interrupt));
//void INT_IIC01(void) __attribute__ ((interrupt));

/*
 * INT_SRE0 (0x18)
 */
void INT_SRE0(void) __attribute__ ((interrupt));

/*
 * INT_TM01H (0x1A)
 */
void INT_TM01H(void) __attribute__ ((interrupt));

/*
 * INT_TM03H (0x1C)
 */
void INT_TM03H(void) __attribute__ ((interrupt));

/*
 * INT_IICA0 (0x1E)
 */
void INT_IICA0(void) __attribute__ ((interrupt));

/*
 * INT_TM00 (0x20)
 */
void INT_TM00(void) __attribute__ ((interrupt));

/*
 * INT_TM01 (0x22)
 */
void INT_TM01(void) __attribute__ ((interrupt));

/*
 * INT_TM02 (0x24)
 */
void INT_TM02(void) __attribute__ ((interrupt));

/*
 * INT_TM03 (0x26)
 */
void INT_TM03(void) __attribute__ ((interrupt));

/*
 * INT_AD (0x28)
 */
void INT_AD(void) __attribute__ ((interrupt));

/*
 * INT_IT (0x2A)
 */
void INT_IT(void) __attribute__ ((interrupt));

/*
 * INT_KR (0x2C)
 */
void INT_KR(void) __attribute__ ((interrupt));

/*
 * INT_MD (0x2E)
 */
void INT_MD(void) __attribute__ ((interrupt));

/*
 * INT_FL (0x30)
 */
void INT_FL(void) __attribute__ ((interrupt));

/*
 * INT_BRK_I (0x7E)
 */
void INT_BRK_I(void) __attribute__ ((interrupt));

//Hardware Vectors
//PowerON_Reset (0x0)
void PowerON_Reset(void) __attribute__ ((interrupt));
#endif
