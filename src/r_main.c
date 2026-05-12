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
 * Creation Date: 2026/05/12
 ***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "cc1101.h"
#include "interrupt_handlers.h"
#include "r_cg_cgc.h"
#include "r_cg_macrodriver.h"
#include "r_cg_port.h"
#include "r_cg_serial.h"
#include "uart_sw.h"
#include <stddef.h>
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/* Unique device serial number — change per device */
static const uint8_t DEVICE_ID[4] = {0xDE, 0xAD, 0x00, 0x01};

/*
 * Packet format (7 bytes):
 *   [0..3] Device ID  (4 bytes)
 *   [4]    Tamper switch  (P4.1: 0=closed/OK, 1=open/tampered)
 *   [5]    Reed switch    (P13.7: 0=closed/window shut, 1=open/window open)
 *   [6]    Checksum       (XOR of bytes 0..5)
 */
#define PKT_LEN 7

static uint8_t build_packet(uint8_t *pkt) {
  P2_bit.no0 = 0; // Pullup needs to be high to read switch state
  pkt[0] = DEVICE_ID[0];
  pkt[1] = DEVICE_ID[1];
  pkt[2] = DEVICE_ID[2];
  pkt[3] = DEVICE_ID[3];
  pkt[4] = P4_bit.no1;  /* tamper switch */
  pkt[5] = P13_bit.no7; /* reed switch */
  uint8_t chk = 0;
  for (uint8_t i = 0; i < 6; i++)
    chk ^= pkt[i];
  pkt[6] = chk;
  P2_bit.no0 = 1; // restore LED state
  return PKT_LEN;
}

void R_MAIN_UserInit(void);

/***********************************************************************************************************************
 * Function Name: main
 * Description  : This function implements main function.
 * Arguments    : None
 * Return Value : None
 ***********************************************************************************************************************/
int main(void) {
  R_MAIN_UserInit();
  while (1) {
    STOP(); // deep sleep, wakes on IT interrupt
  }
  return 0;
}

void LED_INIT() {
  P2_bit.no2 = 1;  // LED off
  PM2_bit.no2 = 0; // output
}
void LED_ON() { P2_bit.no2 = 0; }

void LED_OFF() { P2_bit.no2 = 1; }

MD_STATUS R_CSI00_Send_Receive_Sync(uint8_t *const tx_buf, uint16_t tx_num,
                                    uint8_t *const rx_buf) {
  if (tx_num < 1U) {
    return MD_ARGERROR;
  }

  CSIMK00 = 1U;                     // disable CSI00 interrupt — we poll instead
  SMR00 &= ~_0001_SAU_BUFFER_EMPTY; // flag means "transfer complete", not
                                    // "buffer empty"

  for (uint16_t i = 0; i < tx_num; i++) {
    CSIIF00 = 0U;                              // clear flag
    SIO00 = (tx_buf != 0) ? tx_buf[i] : 0xFFU; // send byte
    while (CSIIF00 == 0U)
      ; // wait for transfer to finish
    if (rx_buf != 0) {
      rx_buf[i] = (uint8_t)SIO00; // read received byte
    }
  }

  return MD_OK;
}

/***********************************************************************************************************************
 * Function Name: R_MAIN_UserInit
 * Description  : This function adds user code before implementing main
 * function. Arguments    : None Return Value : None
 ***********************************************************************************************************************/
void R_MAIN_UserInit(void) {
  EI();

  uartsw_init();
  uartsw_puts("Hello, world!\r\n");

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

  PM4_bit.no1 = 1; /* P4.1 input — tamper switch */

  R_CSI00_Start();
  cc1101_init();
  uartsw_puts("CC1101 init OK\r\n");
}

/* IT interrupt handler — fires every 500ms */
void INT_IT(void) {
  static uint8_t count = 0;
  if (++count < 20)
    return; /* 20 × 500ms = 10s */
  count = 0;

  uint8_t pkt[PKT_LEN];
  build_packet(pkt);
  cc1101_tx_packet(pkt, PKT_LEN);

  uartsw_puts("TX T:");
  uartsw_puthex(pkt[4]);
  uartsw_puts(" R:");
  uartsw_puthex(pkt[5]);
  uartsw_puts("\r\n");
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
