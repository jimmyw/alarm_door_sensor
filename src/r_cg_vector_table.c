/***********************************************************************************************************************
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only
 * intended for use with Renesas products. No other uses are authorized. This
 * software is owned by Renesas Electronics Corporation and is protected under
 * all applicable laws, including copyright laws. THIS SOFTWARE IS PROVIDED "AS
 * IS" AND RENESAS MAKES NO WARRANTIES REGARDING THIS SOFTWARE, WHETHER EXPRESS,
 * IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.  ALL
 * SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.TO THE MAXIMUM EXTENT PERMITTED NOT
 * PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS
 * AFFILIATED COMPANIES SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL,
 * INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE,
 * EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGES. Renesas reserves the right, without notice, to make changes to
 * this software and to discontinue the availability of this software. By using
 * this software, you agree to the additional terms and conditions found by
 * accessing the following link: http://www.renesas.com/disclaimer
 *
 * Copyright (C) 2024, 2024 Renesas Electronics Corporation. All rights
 * reserved.
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * File Name    : r_cg_vector_table.c
 * Version      : Code Generator for RL78/G12 V1.0.0 [01 Jul 2024]
 * Device(s)    : R5F1026A
 * Tool-Chain   : LLVMRL78
 * Description  : This file implements interrupt vector.
 * Creation Date: 2026/05/10
 ***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Set option bytes */
#define OPTION_SECT __attribute__((section(".option_bytes")))
const uint8_t Option_Bytes[] OPTION_SECT = {0x00U, 0xFFU, 0xE9U, 0x04U};

/* Set security ID */
#define SECURITYID_SECT __attribute__((section(".security_id")))
const uint8_t Security_Id[] SECURITYID_SECT = {
    0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U};

#define VEC __attribute__((section(".vec")))
const void *HardwareVectors[] VEC = {
    // Address 0x0
    PowerON_Reset,
    // Secure for Debugging
    (void *)0xFFFF};

#define VECT_SECT __attribute__((section(".vects")))
const void *Vectors[] VECT_SECT = {
    /*
     * INT_WDTI (0x4)
     */
    INT_WDTI,

    /*
     * INT_LVI (0x6)
     */
    INT_LVI,

    /*
     * INT_P0 (0x8)
     */
    INT_P0,

    /*
     * INT_P1 (0xA)
     */
    INT_P1,

    /*
     * INT_P2 (0xC)
     */
    INT_P2,

    /*
     * INT_P3 (0xE)
     */
    INT_P3,

    /*
     * INT_DMA0 (0x10)
     */
    INT_DMA0,

    /*
     * INT_DMA1 (0x12)
     */
    INT_DMA1,

    /*
     * INT_ST0/INT_CSI00/INT_IIC00 (0x14)
     */
    INT_ST0,
    // INT_CSI00,
    // INT_IIC00,

    /*
     * INT_SR0/INT_CSI01/INT_IIC01 (0x16)
     */
    INT_SR0,
    // INT_CSI01,
    // INT_IIC01,

    /*
     * INT_SRE0 (0x18)
     */
    INT_SRE0,

    /*
     * INT_TM01H (0x1A)
     */
    INT_TM01H,

    /*
     * INT_TM03H (0x1C)
     */
    INT_TM03H,

    /*
     * INT_IICA0 (0x1E)
     */
    INT_IICA0,

    /*
     * INT_TM00 (0x20)
     */
    INT_TM00,

    /*
     * INT_TM01 (0x22)
     */
    INT_TM01,

    /*
     * INT_TM02 (0x24)
     */
    INT_TM02,

    /*
     * INT_TM03 (0x26)
     */
    INT_TM03,

    /*
     * INT_AD (0x28)
     */
    INT_AD,

    /*
     * INT_IT (0x2A)
     */
    INT_IT,

    /*
     * INT_KR (0x2C)
     */
    INT_KR,

    /*
     * INT_MD (0x2E)
     */
    INT_MD,

    /*
     * Padding for reserved interrupt source (0x30)
     */
    (void *)0xFFFF,

    /*
     * Padding for reserved interrupt source (0x32)
     */
    (void *)0xFFFF,

    /*
     * Padding for reserved interrupt source (0x34)
     */
    (void *)0xFFFF,

    /*
     * Padding for reserved interrupt source (0x36)
     */
    (void *)0xFFFF,

    /*
     * Padding for reserved interrupt source (0x38)
     */
    (void *)0xFFFF,

    /*
     * Padding for reserved interrupt source (0x3A)
     */
    (void *)0xFFFF,

    /*
     * Padding for reserved interrupt source (0x3C)
     */
    (void *)0xFFFF,

    /*
     * Padding for reserved interrupt source (0x3E)
     */
    (void *)0xFFFF,

    /*
     * Padding for reserved interrupt source (0x40)
     */
    (void *)0xFFFF,

    /*
     * Padding for reserved interrupt source (0x42)
     */
    (void *)0xFFFF,

    /*
     * Padding for reserved interrupt source (0x44)
     */
    (void *)0xFFFF,

    /*
     * Padding for reserved interrupt source (0x46)
     */
    (void *)0xFFFF,

    /*
     * Padding for reserved interrupt source (0x48)
     */
    (void *)0xFFFF,

    /*
     * Padding for reserved interrupt source (0x4A)
     */
    (void *)0xFFFF,

    /*
     * Padding for reserved interrupt source (0x4C)
     */
    (void *)0xFFFF,

    /*
     * Padding for reserved interrupt source (0x4E)
     */
    (void *)0xFFFF,

    /*
     * Padding for reserved interrupt source (0x50)
     */
    (void *)0xFFFF,

    /*
     * Padding for reserved interrupt source (0x52)
     */
    (void *)0xFFFF,

    /*
     * Padding for reserved interrupt source (0x54)
     */
    (void *)0xFFFF,

    /*
     * Padding for reserved interrupt source (0x56)
     */
    (void *)0xFFFF,

    /*
     * Padding for reserved interrupt source (0x58)
     */
    (void *)0xFFFF,

    /*
     * Padding for reserved interrupt source (0x5A)
     */
    (void *)0xFFFF,

    /*
     * Padding for reserved interrupt source (0x5C)
     */
    (void *)0xFFFF,

    /*
     * Padding for reserved interrupt source (0x5E)
     */
    (void *)0xFFFF,

    /*
     * Padding for reserved interrupt source (0x60)
     */
    (void *)0xFFFF,

    /*
     * Padding for reserved interrupt source (0x62)
     */
    (void *)0xFFFF,

    /*
     * Padding for reserved interrupt source (0x64)
     */
    (void *)0xFFFF,

    /*
     * Padding for reserved interrupt source (0x66)
     */
    (void *)0xFFFF,

    /*
     * Padding for reserved interrupt source (0x68)
     */
    (void *)0xFFFF,

    /*
     * Padding for reserved interrupt source (0x6A)
     */
    (void *)0xFFFF,

    /*
     * Padding for reserved interrupt source (0x6C)
     */
    (void *)0xFFFF,

    /*
     * Padding for reserved interrupt source (0x6E)
     */
    (void *)0xFFFF,

    /*
     * Padding for reserved interrupt source (0x70)
     */
    (void *)0xFFFF,

    /*
     * Padding for reserved interrupt source (0x72)
     */
    (void *)0xFFFF,

    /*
     * Padding for reserved interrupt source (0x74)
     */
    (void *)0xFFFF,

    /*
     * Padding for reserved interrupt source (0x76)
     */
    (void *)0xFFFF,

    /*
     * Padding for reserved interrupt source (0x78)
     */
    (void *)0xFFFF,

    /*
     * Padding for reserved interrupt source (0x7A)
     */
    (void *)0xFFFF,

    /*
     * Padding for reserved interrupt source (0x7C)
     */
    (void *)0xFFFF,

    /*
     * Padding for reserved interrupt source (0x7E)
     */
    (void *)0xFFFF,
};

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
