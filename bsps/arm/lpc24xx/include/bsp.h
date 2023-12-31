/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup RTEMSBSPsARMLPC24XX
 *
 * @brief Global BSP definitions.
 */

/*
 * Copyright (C) 2008, 2014 embedded brains GmbH & Co. KG
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
 */

#ifndef LIBBSP_ARM_LPC24XX_BSP_H
#define LIBBSP_ARM_LPC24XX_BSP_H

/**
 * @defgroup RTEMSBSPsARMLPC24XX NXP LPC17XX, LPC23XX, LPC24XX and LPC40XX
 *
 * @ingroup RTEMSBSPsARM
 *
 * @brief NXP LPC17XX, LPC23XX, LPC24XX and LPC40XX Board Support Package.
 *
 * @{
 */

#include <bspopts.h>

#define BSP_FEATURE_IRQ_EXTENSION

#define LPC24XX_PCLK (LPC24XX_CCLK / LPC24XX_PCLKDIV)

#define LPC24XX_EMCCLK (LPC24XX_CCLK / LPC24XX_EMCCLKDIV)

#define LPC24XX_MPU_REGION_COUNT 8

#define BSP_ARMV7M_IRQ_PRIORITY_DEFAULT (29 << 3)

#define BSP_ARMV7M_SYSTICK_PRIORITY (30 << 3)

#define BSP_ARMV7M_SYSTICK_FREQUENCY LPC24XX_CCLK

#ifndef ASM

#include <rtems.h>

#include <bsp/default-initial-extension.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

struct rtems_bsdnet_ifconfig;

struct rtems_termios_device_context;

/**
 * @brief Network driver attach and detach function.
 */
int lpc_eth_attach_detach(
  struct rtems_bsdnet_ifconfig *config,
  int attaching
);

/**
 * @brief Standard network driver attach and detach function.
 */
#define RTEMS_BSP_NETWORK_DRIVER_ATTACH lpc_eth_attach_detach

/**
 * @brief Standard network driver name.
 */
#define RTEMS_BSP_NETWORK_DRIVER_NAME "eth0"

/**
 * @brief Optimized idle task.
 *
 * This idle task sets the power mode to idle.  This causes the processor clock
 * to be stopped, while on-chip peripherals remain active.  Any enabled
 * interrupt from a peripheral or an external interrupt source will cause the
 * processor to resume execution.
 *
 * To enable the idle task use the following in the system configuration:
 *
 * @code
 * #include <bsp.h>
 *
 * #define CONFIGURE_INIT
 *
 * #define CONFIGURE_IDLE_TASK_BODY bsp_idle_thread
 *
 * #include <confdefs.h>
 * @endcode
 */
void *bsp_idle_thread(uintptr_t ignored);

#ifdef ARM_MULTILIB_ARCH_V4
  #define BSP_CONSOLE_UART_BASE 0xe000c000
#else
  #define BSP_CONSOLE_UART_BASE 0x4000c000
#endif

void bsp_restart(void *addr);

bool lpc24xx_uart_probe_1(struct rtems_termios_device_context *context);

bool lpc24xx_uart_probe_2(struct rtems_termios_device_context *context);

bool lpc24xx_uart_probe_3(struct rtems_termios_device_context *context);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* ASM */

/** @} */

#endif /* LIBBSP_ARM_LPC24XX_BSP_H */
