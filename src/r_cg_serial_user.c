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
 * File Name    : r_cg_serial_user.c
 * Version      : Code Generator for RL78/G12 V1.0.0 [01 Jul 2024]
 * Device(s)    : R5F1026A
 * Tool-Chain   : LLVMRL78
 * Description  : This file implements device driver for Serial module.
 * Creation Date: 2026/05/12
 ***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_serial.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
extern volatile uint8_t *gp_csi00_rx_address; /* csi00 receive buffer address */
extern volatile uint16_t g_csi00_rx_length;   /* csi00 receive data length */
extern volatile uint16_t g_csi00_rx_count;    /* csi00 receive data count */
extern volatile uint8_t *gp_csi00_tx_address; /* csi00 send buffer address */
extern volatile uint16_t g_csi00_send_length; /* csi00 send data length */
extern volatile uint16_t g_csi00_tx_count;    /* csi00 send data count */
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
 * Function Name: r_csi00_interrupt
 * Description  : This function is INTCSI00 interrupt service routine.
 * Arguments    : None
 * Return Value : None
 ***********************************************************************************************************************/
void r_csi00_interrupt(void) {
  volatile uint8_t err_type;

  err_type = (uint8_t)(SSR00 & _0001_SAU_OVERRUN_ERROR);
  SIR00 = (uint16_t)err_type;

  if (1U == err_type) {
    r_csi00_callback_error(err_type); /* overrun error occurs */
  } else {
    if (g_csi00_tx_count > 0U) {
      if (g_csi00_tx_count != (g_csi00_send_length - 1U)) {
        *gp_csi00_rx_address = SIO00;
        gp_csi00_rx_address++;
      }

      SIO00 = *gp_csi00_tx_address;
      gp_csi00_tx_address++;
      g_csi00_tx_count--;
    } else {
      if ((SMR00 & _0001_SAU_BUFFER_EMPTY) == 1U) {
        r_csi00_callback_sendend(); /* complete send */
        *gp_csi00_rx_address = SIO00;
        gp_csi00_rx_address++;

        if ((SSR00 & _0040_SAU_UNDER_EXECUTE) == 0U) {
          *gp_csi00_rx_address = SIO00;
          r_csi00_callback_receiveend(); /* complete receive */
        } else {
          SMR00 &= ~_0001_SAU_BUFFER_EMPTY;
        }
      } else {
        *gp_csi00_rx_address = SIO00;
        r_csi00_callback_receiveend(); /* complete receive */
      }
    }
  }
}

/***********************************************************************************************************************
 * Function Name: r_csi00_callback_receiveend
 * Description  : This function is a callback function when CSI00 finishes
 * reception. Arguments    : None Return Value : None
 ***********************************************************************************************************************/
static void r_csi00_callback_receiveend(void) {
  /* Start user code. Do not edit comment generated here */
  /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
 * Function Name: r_csi00_callback_error
 * Description  : This function is a callback function when CSI00 reception
 * error occurs. Arguments    : err_type - error type value Return Value : None
 ***********************************************************************************************************************/
static void r_csi00_callback_error(uint8_t err_type) {
  /* Start user code. Do not edit comment generated here */
  /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
 * Function Name: r_csi00_callback_sendend
 * Description  : This function is a callback function when CSI00 finishes
 * transmission. Arguments    : None Return Value : None
 ***********************************************************************************************************************/
static void r_csi00_callback_sendend(void) {
  /* Start user code. Do not edit comment generated here */
  /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
 * Function Name: R_SAU0_Create_UserInit
 * Description  : This function adds user code after initializing SAU0.
 * Arguments    : None
 * Return Value : None
 ***********************************************************************************************************************/
void R_SAU0_Create_UserInit(void) {
  /* Start user code. Do not edit comment generated here */
  /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
