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
 * File Name    : r_main.c
 * Version      : Code Generator for RL78/G12 V1.0.0 [01 Jul 2024]
 * Device(s)    : R5F1026A
 * Tool-Chain   : LLVMRL78
 * Description  : This file implements main function.
 * Creation Date: 2026/05/10
 ***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "interrupt_handlers.h"
#include "r_cg_cgc.h"
#include "r_cg_macrodriver.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
void R_MAIN_UserInit(void);

/***********************************************************************************************************************
 * Function Name: main
 * Description  : This function implements main function.
 * Arguments    : None
 * Return Value : None
 ***********************************************************************************************************************/
int main(void) {
  R_MAIN_UserInit();
  // while (1) {
  //  STOP(); // deep sleep, wakes on IT interrupt
  //}
  return 0;
}

/***********************************************************************************************************************
 * Function Name: R_MAIN_UserInit
 * Description  : This function adds user code before implementing main
 * function. Arguments    : None Return Value : None
 ***********************************************************************************************************************/
void R_MAIN_UserInit(void) {
  EI();

  ADPC = 0x01U; // Turn all outputs into Digital

  P2 = 0xff;
  PM2 = 0x00; // all output

  ///* Allow LOCO (low-speed oscillator) to keep running in STOP mode */
  OSMC = _10_CGC_IT_CLK_FIL; // WUTMMCK0=1: IT runs on LOCO during STOP
  //
  ///* Enable clock to interval timer */
  TMKAEN = 1U; // PER0 bit5 — TAU-KA clock on
  //
  ///* Configure Interval Timer for 500ms using LOCO @ 15kHz */make -C
  TMKAMK = 1U;            // mask IT interrupt while configuring
  TMKAIF = 0U;            // clear any pending flag
  ITMC = 0x8000U | 7499U; // bit15=enable, lower 15 bits = compare value
  TMKAMK = 0U;            // unmask — ready to fire
  P2 = 0x00;              // turn on LED on P2.2
}

/* IT interrupt handler — fires every 500ms */
void INT_IT(void) {
  if (P2 & 0x1) {
    P2 = 0x00; // turn on LED on P2.0
  } else {
    P2 = 0xff; // turn off LED on P2.0
  }
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
