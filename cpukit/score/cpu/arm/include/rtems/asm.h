/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup RTEMSScoreCPUARMASM
 *
 * @brief This header file provides interfaces to address problems caused by
 *   incompatible flavor of assemblers and toolsets.
 *
 * This include file attempts to address the problems
 * caused by incompatible flavors of assemblers and
 * toolsets.  It primarily addresses variations in the
 * use of leading underscores on symbols and the requirement
 * that register names be preceded by a %.
 *
 *
 * NOTE: The spacing in the use of these macros
 *       is critical to them working as advertised.
 */

/*
 *  COPYRIGHT:
 *
 *  This file is based on similar code found in newlib available
 *  from ftp.cygnus.com.  The file which was used had no copyright
 *  notice.  This file is freely distributable as long as the source
 *  of the file is noted.  This file is:
 *
 *  COPYRIGHT (c) 2000 Canon Research Centre France SA.
 *  Emmanuel Raguet, mailto:raguet@crf.canon.fr
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef _RTEMS_ASM_H
#define _RTEMS_ASM_H

/*
 *  Indicate we are in an assembly file and get the basic CPU definitions.
 */

#ifndef ASM
#define ASM
#endif
#include <rtems/score/percpu.h>

/**
 * @defgroup RTEMSScoreCPUARMASM ARM Assembler Support
 *
 * @ingroup RTEMSScoreCPUARM
 *
 * @brief ARM Assembler Support
 */
/**@{**/

/*
 *  Recent versions of GNU cpp define variables which indicate the
 *  need for underscores and percents.  If not using GNU cpp or
 *  the version does not support this, then you will obviously
 *  have to define these as appropriate.
 */

#ifndef __USER_LABEL_PREFIX__
#define __USER_LABEL_PREFIX__ _
#endif

#ifndef __REGISTER_PREFIX__
#define __REGISTER_PREFIX__
#endif

/* Use the right prefix for global labels.  */

#define SYM(x) RTEMS_XCONCAT(__USER_LABEL_PREFIX__, x)

/* Use the right prefix for registers.  */

#define REG(x) RTEMS_XCONCAT(__REGISTER_PREFIX__, x)

/*
 *  define macros for all of the registers on this CPU
 *
 *  EXAMPLE:     #define d0 REG (d0)
 */

#define r0  REG(r0)
#define r1  REG(r1)
#define r2  REG(r2)
#define r3  REG(r3)
#define r4  REG(r4)
#define r5  REG(r5)
#define r6  REG(r6)
#define r7  REG(r7)
#define r8  REG(r8)
#define r9  REG(r9)
#define r10 REG(r10)
#define r11 REG(r11)
#define r12 REG(r12)
#define r13 REG(r13)
#define r14 REG(r14)
#define r15 REG(r15)

#define CPSR REG(CPSR)

#define SPSR REG(SPSR)

#define NUM_IRQ_VECTOR          6       // IRQ number
#define NUM_FIQ_VECTOR          7       // IRQ number
										// 										//
#define CPSR_IRQ_DISABLE        0x80    // FIQ disabled when =1
#define CPSR_FIQ_DISABLE        0x40    // FIQ disabled when =1
#define CPSR_THUMB_ENABLE       0x20    // Thumb mode when =1
#define CPSR_FIQ_MODE           0x11
#define CPSR_IRQ_MODE           0x12
#define CPSR_SUPERVISOR_MODE    0x13
#define CPSR_UNDEF_MODE         0x1B

#define CPSR_MODE_BITS          0x1F

/*
 *  Define macros to handle section beginning and ends.
 */


#define BEGIN_CODE_DCL .text
#define END_CODE_DCL
#define BEGIN_DATA_DCL .data
#define END_DATA_DCL
#define BEGIN_CODE .text
#define END_CODE
#define BEGIN_DATA
#define END_DATA
#define BEGIN_BSS
#define END_BSS
#define END

/*
 *  Following must be tailor for a particular flavor of the C compiler.
 *  They may need to put underscores in front of the symbols.
 */

#define PUBLIC(sym) .globl SYM (sym)
#define EXTERN(sym) .globl SYM (sym)

#define FUNCTION_THUMB_ENTRY(name) \
  .thumb; \
  .thumb_func; \
  .align 2; \
  .globl name; \
  .type name, %function; \
  name:

#define FUNCTION_ENTRY(name) \
  .align 2; \
  .globl name; \
  .type name, %function; \
  name:

#define FUNCTION_END(name) \
  .size name, . - name

#if defined(ARM_MULTILIB_ARCH_V7M)
  #define DEFINE_FUNCTION_ARM(name) \
    .thumb_func ; .globl name ; name:
#elif defined(__thumb__)
  #define DEFINE_FUNCTION_ARM(name) \
    .thumb_func ; .globl name ; name: ; bx pc ; \
    .arm ; .globl name ## _arm ; name ## _arm:
#else
  #define DEFINE_FUNCTION_ARM(name) \
    .globl name ; name: ; .globl name ## _arm ; name ## _arm:
#endif

.macro SWITCH_FROM_THUMB_TO_ARM
#ifdef __thumb__
.align 2
	bx	pc
.arm
#endif /* __thumb__ */
.endm

.macro SWITCH_FROM_ARM_TO_THUMB REG
#ifdef __thumb__
	add	\REG, pc, #1
	bx	\REG
.thumb
#endif /* __thumb__ */
.endm

.macro SWITCH_FROM_THUMB_2_TO_ARM
#ifdef __thumb2__
.align 2
	bx	pc
.arm
#endif /* __thumb__ */
.endm

.macro SWITCH_FROM_ARM_TO_THUMB_2 REG
#ifdef __thumb2__
	add	\REG, pc, #1
	bx	\REG
.thumb
#endif /* __thumb__ */
.endm

.macro BLX_TO_THUMB_1 TARGET
#if defined(__thumb__) && !defined(__thumb2__)
	add	lr, pc, #1
	bx	lr
.thumb
	bl	\TARGET
.align 2
	bx	pc
.arm
#else
	bl	\TARGET
#endif
.endm

.macro GET_SELF_CPU_CONTROL REG
#ifdef RTEMS_SMP
	/* Use PL1 only Thread ID Register (TPIDRPRW) */
	mrc	p15, 0, \REG, c13, c0, 4
#else
	ldr	\REG, =_Per_CPU_Information
#endif
.endm

/** @} */

#endif /* _RTEMS_ASM_H */
