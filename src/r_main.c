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
static const uint8_t DEVICE_ID[4] = {0xDE, 0xAD, 0x00, 0x02};
/* Previous pin states for change detection */
static uint8_t prev_reed = 0;
static uint8_t prev_tamp = 0;

// With debug on, we use the led as uart.
#define DEBUG
#define STATUS_INTERVAL_S 900 // 15 minutes
#define IT_TICK_MS 273        // 4096/15kHz ≈ 273ms per tick

/*
 * Measure VDD using the internal reference voltage (~1.45V).
 * Returns supply voltage in millivolts.
 */
static uint16_t read_vdd_mv(void) {
  ADCEN = 1U; /* enable ADC peripheral clock */
  ADMK = 1U;  /* mask ADC interrupt — we poll */

  ADM0 = 0x38U; /* Normal 1 mode, fCLK/2 */
  ADM1 = 0x20U; /* software trigger, one-shot */
  ADM2 = 0x00U; /* VDD/VSS ref, 10-bit */
  ADS = 0x81U;  /* internal reference voltage (1.45V) */
  ADLL = 0x00U;
  ADUL = 0xFFU;

  ADCE = 1U;
  {
    volatile uint16_t d;
    for (d = 0; d < 100; d++)
      NOP(); /* 1µs stabilisation */
  }

  /* Dummy conversion (first after channel change is undefined) */
  ADIF = 0U;
  ADCS = 1U;
  while (ADIF == 0U)
    ;

  /* Real conversion */
  ADIF = 0U;
  ADCS = 1U;
  while (ADIF == 0U)
    ;

  uint16_t result = ADCR >> 6; /* right-justify 10-bit result */

  /* Shut down */
  ADCE = 0U;
  ADCEN = 0U;

  if (result == 0)
    return 0xFFFF;
  return (uint16_t)(((uint32_t)1024 * 1450) / result);
}

/*
 * Packet format (9 bytes):
 *   [0..3] Device ID  (4 bytes)
 *   [4]    Tamper switch  (P4.1: 0=closed/OK, 1=open/tampered)
 *   [5]    Reed switch    (P13.7: 0=closed/window shut, 1=open/window open)
 *   [6..7] Battery voltage in mV, big-endian
 *   [8]    Checksum       (XOR of bytes 0..7)
 */
#define PKT_LEN 9

static uint8_t build_packet(uint8_t *pkt, uint8_t reed, uint8_t tamp,
                            uint16_t vdd) {

  pkt[0] = DEVICE_ID[0];
  pkt[1] = DEVICE_ID[1];
  pkt[2] = DEVICE_ID[2];
  pkt[3] = DEVICE_ID[3];
  pkt[4] = tamp;                  /* tamper switch */
  pkt[5] = reed;                  /* reed switch */
  pkt[6] = (uint8_t)(vdd >> 8);   /* battery mV high */
  pkt[7] = (uint8_t)(vdd & 0xFF); /* battery mV low */
  uint8_t chk = 0;
  for (uint8_t i = 0; i < 8; i++)
    chk ^= pkt[i];
  pkt[8] = chk;

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

#ifdef DEBUG
  uartsw_init();
  uartsw_puts("Hello, world!\r\n");
  uartsw_puts("Device ID: ");
  uartsw_puthex(DEVICE_ID[0]);
  uartsw_puthex(DEVICE_ID[1]);
  uartsw_puthex(DEVICE_ID[2]);
  uartsw_puthex(DEVICE_ID[3]);
  uartsw_puts("\r\n");
#else
  LED_OFF();
#endif
  ///* Allow LOCO (low-speed oscillator) to keep running in STOP mode */
  OSMC = _10_CGC_IT_CLK_FIL; // WUTMMCK0=1: IT runs on LOCO during STOP
  //
  ///* Enable clock to interval timer */
  TMKAEN = 1U; // PER0 bit5 — TAU-KA clock on
  //
  ///* Configure Interval Timer for ~273ms using LOCO @ 15kHz */
  TMKAMK = 1U;            // mask IT interrupt while configuring
  TMKAIF = 0U;            // clear any pending flag
  ITMC = 0x8000U | 4095U; // bit15=enable, 12-bit max: 4096/15kHz ≈ 273ms
  TMKAMK = 0U;            // unmask — ready to fire

  PM4_bit.no1 = 1; /* P4.1 input — tamper switch */
  PU1_bit.no1 = 1; /* pull-up on MISO (P1.1) — needed for CC1101 sleep wakeup */

  R_CSI00_Start();
  cc1101_init();
  cc1101_powerdown();
  R_CSI00_Stop();
#ifdef DEBUG
  uartsw_puts("CC1101 init OK\r\n");
#endif
  /* Take initial pin snapshot with pull-up enabled */
  P2_bit.no0 = 1;
  {
    volatile uint8_t d;
    for (d = 0; d < 100; d++)
      NOP();
  }
  prev_reed = P13_bit.no7;
  prev_tamp = P4_bit.no1;
  P2_bit.no0 = 0;
}

static void send_status(const char *reason, uint8_t reed, uint8_t tamp,
                        uint16_t vdd) {
  uint8_t pkt[PKT_LEN];
  build_packet(pkt, reed, tamp, vdd);

  R_CSI00_Start(); /* wake SPI */
  cc1101_wakeup(); /* wake CC1101 from sleep */
  cc1101_tx_packet(pkt, PKT_LEN);
  cc1101_powerdown(); /* CC1101 sleep (~0.2µA) */
  R_CSI00_Stop();     /* stop SPI clock */

#ifdef DEBUG
  uartsw_puts(reason);
  uartsw_puts(" T:");
  uartsw_puthex(tamp);
  uartsw_puts(" R:");
  uartsw_puthex(reed);
  uartsw_puts(" V:");
  uartsw_putd16(vdd);
  uartsw_puts(" mV\r\n");
#endif
}

/* IT interrupt handler — 500ms tick, polls switch pins */
void INT_IT(void) {
  static uint16_t hb_count =
      INT16_MAX; /* force immediate heartbeat on startup */

  /* Enable pull-up and let it settle (~50µs) */
  P2_bit.no0 = 1;
  {
    volatile uint8_t d;
    for (d = 0; d < 10; d++)
      NOP();
  }

  uint8_t reed = P13_bit.no7;
  uint8_t tamp = P4_bit.no1;
  /* Disable pull-up after all reads/sends are done */
  P2_bit.no0 = 0;

  /* Send on change (pull-up stays on so build_packet reads correct values) */
  const char *reason = NULL;

  if (reed != prev_reed && tamp != prev_tamp) {
    reason = "REED+TAMP";
  } else if (reed != prev_reed) {
    reason = "REED";
  } else if (tamp != prev_tamp) {
    reason = "TAMP";
  } else if (hb_count >= (STATUS_INTERVAL_S * 1000UL / IT_TICK_MS)) {
    /* Heartbeat every ~10 min */
    reason = "HB";
  }

  if (reason) {
    uint16_t vdd = read_vdd_mv();
    send_status(reason, reed, tamp, vdd);
    prev_reed = reed;
    prev_tamp = tamp;
    hb_count = 0;
  } else {
    hb_count++;
  }
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
