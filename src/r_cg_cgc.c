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
 * File Name    : r_cg_cgc.c
 * Version      : Code Generator for RL78/G12 V1.0.0 [01 Jul 2024]
 * Device(s)    : R5F1026A
 * Tool-Chain   : LLVMRL78
 * Description  : This file implements device driver for CGC module.
 * Creation Date: 2026/05/12
 ***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
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

/***********************************************************************************************************************
 * Function Name: R_CGC_Create
 * Description  : This function initializes the clock generator.
 * Arguments    : None
 * Return Value : None
 ***********************************************************************************************************************/
void R_CGC_Create(void) {
  /* Set fMX */
  CMC = _00_CGC_HISYS_PORT | _00_CGC_SYSOSC_DEFAULT;
  MSTOP = 1U;
  /* Set fMAIN */
  MCM0 = 0U;
  OSMC = _00_CGC_IT_CLK_NO;
  /* Set fIH */
  HIOSTOP = 0U;

  R_CGC_Create_UserInit();
}

/***********************************************************************************************************************
 * Function Name: R_CGC_Set_ClockMode
 * Description  : This function changes clock generator operation mode.
 * Arguments    : mode -
 *                    clock generator operation mode
 * Return Value : status -
 *                    MD_OK, MD_ARGERROR, MD_ERROR1 or MD_ERROR2
 ***********************************************************************************************************************/
MD_STATUS R_CGC_Set_ClockMode(clock_mode_t mode) {
  MD_STATUS status = MD_OK;
  clock_mode_t old_mode;
  uint8_t temp_stab_set;
  uint8_t temp_stab_wait;
  volatile uint32_t w_count;

  if (1U == MCS) {
    if ((CMC & _C0_CGC_HISYS_PIN) == _C0_CGC_HISYS_EXT) {
      old_mode = SYSEXTCLK;
    } else if ((CMC & _C0_CGC_HISYS_PIN) == _40_CGC_HISYS_OSC) {
      old_mode = SYSX1CLK;
    } else {
      /* Not run */
    }
  } else {
    old_mode = HIOCLK;
  }

  if (mode != old_mode) {
    switch (mode) {
    case HIOCLK:

      if (1U == HIOSTOP) {
        HIOSTOP = 0U;

        /* Change the waiting time according to the system */
        for (w_count = 0U; w_count <= CGC_FIHWAITTIME; w_count++) {
          NOP();
        }
      }

      MCM0 = 0U;

      break;

    case SYSX1CLK:

      if ((SYSEXTCLK == old_mode) ||
          ((CMC & _C0_CGC_HISYS_PIN) != _40_CGC_HISYS_OSC)) {
        status = MD_ERROR1;
      } else {
        if (1U == MSTOP) {
          MSTOP = 0U;
          temp_stab_set = (uint8_t)~(0x7FU >> OSTS);

          do {
            temp_stab_wait = OSTC;
            temp_stab_wait &= temp_stab_set;
          } while (temp_stab_wait != temp_stab_set);
        }

        MCM0 = 1U;
      }

      break;

    case SYSEXTCLK:

      if ((SYSX1CLK == old_mode) ||
          ((CMC & _C0_CGC_HISYS_PIN) != _C0_CGC_HISYS_EXT)) {
        status = MD_ERROR2;
      } else {
        if (1U == MSTOP) {
          MSTOP = 0U;
        }

        MCM0 = 1U;
      }

      break;

    default:

      status = MD_ARGERROR;

      break;
    }
  }

  return (status);
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
