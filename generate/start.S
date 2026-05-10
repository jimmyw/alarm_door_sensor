/*
* Copyright (c) 2025 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/
/************************************************************************/
/*    File Version: V1.10e                                              */
/*    Date Generated: 2025/10/02                                        */
/************************************************************************/

	/*reset_program.asm*/

#ifdef CPPAPP
___dso_handle:
	.global	___dso_handle
#endif

#if __clang_major__  <= 10
;; If you are using LLVM RL V10.0.0.202301 to V10.0.0.202312, please comment out the following three lines.
	#define HIGHW(x) %hi16(x)
	#define LOWW(x)  x
	#define LOW(x)  x
#endif

	.extern	_HardwareSetup /*! external Sub-routine to initialise Hardware*/
	.extern	__data
	.extern	__mdata
	.extern	__ebss
	.extern	__bss
	.extern	__edata
	.extern	_main 
	.extern	__stack

	.text

	.global	_PowerON_Reset  /*! global Start routine */
	.type	_PowerON_Reset,@function
/* call to _PowerON_Reset */
_PowerON_Reset:
/* initialise user stack pointer */
	movw	sp,#__stack /* Set stack pointer */


/* load data section from ROM to RAM */
;; block move to initialize .data

	mov	es, #LOW(HIGHW(__romdatastart))
	movw	bc, #LOWW(__romdatacopysize)
1:
	movw	ax, bc
	cmpw	ax, #0
	bz	$1f
	decw	bc
	decw	bc
	movw	ax, es:LOWW(__romdatastart)[bc]
	movw	LOWW(__datastart)[bc], ax
	br	$1b

;; if you used variables having initial value (far) then you would have to uncomment this.
;; For example: char __far variable = 1;
;1:
;	mov	es, #LOW(HIGHW(__romdatafstart))
;	movw	bc, #LOWW(__romdatafcopysize)
;1:
;	movw	ax, bc
;	cmpw	ax, #0
;	bz	$1f
;	decw	bc
;	decw	bc
;	movw	ax, es:LOWW(__romdatafstart)[bc]
;	movw	LOWW(__datafstart)[bc], ax
;	br	$1b

1:
	mov	es, #LOW(HIGHW(__romsdatastart))
	movw	bc, #LOWW(__romsdatacopysize)
1:
	movw	ax, bc
	cmpw	ax, #0
	bz	$1f
	decw	bc
	decw	bc
	movw	ax, es:LOWW(__romsdatastart)[bc]
	movw	LOWW(__sdatastart)[bc], ax
	br	$1b

;; block fill to .sbss
1:
#ifdef __OPTIMIZE_SIZE__
	movw de, #LOWW(__sbsssize)
	movw ax, #LOWW(__sbssstart)
	clrw bc
	call !!_memset
#else
	movw	bc, #LOWW(__sbsssize)
	clrw	ax
	cmpw	ax, bc
	bz	$_sbss_zero_done
1:
	decw	bc
	decw	bc
	movw	LOWW(__sbssstart)[bc], ax
	cmpw	ax, bc
	bnz	$1b

_sbss_zero_done:
#endif

;; block fill to .bss

#ifdef __OPTIMIZE_SIZE__
	movw de, #LOWW(__bsssize)
	movw ax, #LOWW(__bssstart)
	clrw bc
	call !!_memset
#else
	movw	bc, #LOWW(__bsssize)
	clrw	ax
	cmpw	ax, bc
	bz	$_bss_zero_done
1:
	decw	bc
	decw	bc
	movw	LOWW(__bssstart)[bc], ax
	cmpw	ax, bc
	bnz	$1b

_bss_zero_done:
#endif

;; block fill to .bssf

;; if you used variables which doesn't have initial value (far) then you would have to uncomment this'
;; For example: char __far variable;
;#ifdef __OPTIMIZE_SIZE__
;	movw de, #LOWW(__bssfsize)
;	movw ax, #LOWW(__bssfstart)
;	clrw bc
;	call !!_memset
;#else
;1:
;	movw	bc, #LOWW(__bssfsize)
;	clrw	ax
;	cmpw	ax, bc
;	bz	$_bssf_zero_done
;1:
;	decw	bc
;	decw	bc
;	movw	LOWW(__bssfstart)[bc], ax
;	cmpw	ax, bc
;	bnz	$1b

;_bssf_zero_done:
;#endif

	call	!!__rl78_init


/* call the hardware initialiser */
	call	!!_HardwareSetup
	nop


/* start user program */

	clrw	ax	/* argv */
	clrw	bc	/* argc */
	call	!!_main


	call	!!__rl78_fini

	.global	_exit
	.type	_exit,@function
/* call to exit*/
_exit:
	br	$_exit


/*  ;; HL = start of list
	;; DE = end of list
	;; BC = step direction (+2 or -2)
*/
	.global	_rl78_run_init_array
	.type	_rl78_run_init_array,@function
_rl78_run_init_array:
	movw	hl, #__init_array_start
	movw	de, #__init_array_end
#ifdef __RL78_MEDIUM__
	movw	bc, #4
#else
	movw	bc, #2
#endif
	br	$_rl78_run_inilist

	.global	_rl78_run_fini_array
	.type	_rl78_run_fini_array,@function
_rl78_run_fini_array:
	movw	hl, #__fini_array_start
	movw	de, #__fini_array_end
#ifdef __RL78_MEDIUM__
	movw	bc, #4
#else
	movw	bc, #2
#endif
/* fall through */

	;; HL = start of list
	;; DE = end of list
	;; BC = step direction (+2 or -2)
_rl78_run_inilist:
next_inilist:
	movw	ax, hl
	cmpw	ax, de
	bz	$done_inilist
	movw	ax, [hl]
	cmpw	ax, #-1
	bz	$skip_inilist
	cmpw	ax, #0
	bz	$skip_inilist
	push	ax
	push	bc
	push	de
	push	hl
#ifdef __RL78_MEDIUM__
	push    ax
	mov		a, [hl+2]
	mov     cs, a
	pop     ax
#endif
	call	ax
	pop	hl
	pop	de
	pop	bc
	pop	ax
skip_inilist:
	movw	ax, hl
	addw	ax, bc
	movw	hl, ax
	br	$next_inilist
done_inilist:
	ret


	.global __rl78_init
	.type	__rl78_init,@function
__rl78_init:
	call	!!_rl78_run_init_array
	ret


	.global __rl78_fini
	.type	__rl78_fini,@function
__rl78_fini:
	call	!!_rl78_run_fini_array
	ret


.end
