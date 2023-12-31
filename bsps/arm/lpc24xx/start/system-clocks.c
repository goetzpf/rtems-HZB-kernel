/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup RTEMSBSPsARMLPC24XX_clocks
 *
 * @brief System clocks.
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

#include <rtems/counter.h>

#include <bsp.h>
#include <bsp/lpc24xx.h>
#include <bsp/system-clocks.h>

/**
 * @brief Internal RC oscillator frequency in [Hz].
 */
#define LPC24XX_OSCILLATOR_INTERNAL 4000000U

#ifndef LPC24XX_OSCILLATOR_MAIN
  #error "unknown main oscillator frequency"
#endif

#ifndef LPC24XX_OSCILLATOR_RTC
  #error "unknown RTC oscillator frequency"
#endif

void lpc24xx_timer_initialize(void)
{
  /* Reset timer */
  T1TCR = TCR_RST;

  /* Set timer mode */
  T1CTCR = 0;

  /* Set prescaler to zero */
  T1PR = 0;

  /* Reset all interrupt flags */
  T1IR = 0xff;

  /* Do not stop on a match */
  T1MCR = 0;

  /* No captures */
  T1CCR = 0;

  /* Start timer */
  T1TCR = TCR_EN;
}

uint32_t _CPU_Counter_frequency(void)
{
  return LPC24XX_PCLK;
}

CPU_Counter_ticks _CPU_Counter_read(void)
{
  return lpc24xx_timer();
}

void lpc24xx_micro_seconds_delay(unsigned us)
{
  unsigned start = lpc24xx_timer();
  unsigned delay = us * (LPC24XX_PCLK / 1000000);
  unsigned elapsed = 0;

  do {
    elapsed = lpc24xx_timer() - start;
  } while (elapsed < delay);
}

#ifdef ARM_MULTILIB_ARCH_V7M
  static unsigned lpc17xx_sysclk(unsigned clksrcsel)
  {
    return (clksrcsel & LPC17XX_SCB_CLKSRCSEL_CLKSRC) != 0 ?
      LPC24XX_OSCILLATOR_MAIN
        : LPC24XX_OSCILLATOR_INTERNAL;
  }
#endif

unsigned lpc24xx_pllclk(void)
{
  #ifdef ARM_MULTILIB_ARCH_V4
    unsigned clksrc = GET_CLKSRCSEL_CLKSRC(CLKSRCSEL);
    unsigned pllinclk = 0;
    unsigned pllclk = 0;

    /* Get PLL input frequency */
    switch (clksrc) {
      case 0:
        pllinclk = LPC24XX_OSCILLATOR_INTERNAL;
        break;
      case 1:
        pllinclk = LPC24XX_OSCILLATOR_MAIN;
        break;
      case 2:
        pllinclk = LPC24XX_OSCILLATOR_RTC;
        break;
      default:
        return 0;
    }

    /* Get PLL output frequency */
    if ((PLLSTAT & PLLSTAT_PLLC) != 0) {
      uint32_t pllcfg = PLLCFG;
      unsigned n = GET_PLLCFG_NSEL(pllcfg) + 1;
      unsigned m = GET_PLLCFG_MSEL(pllcfg) + 1;

      pllclk = (pllinclk / n) * 2 * m;
    } else {
      pllclk = pllinclk;
    }
  #else
    volatile lpc17xx_scb *scb = &LPC17XX_SCB;
    unsigned sysclk = lpc17xx_sysclk(scb->clksrcsel);
    unsigned pllstat = scb->pll_0.stat;
    unsigned pllclk = 0;
    unsigned enabled_and_locked = LPC17XX_PLL_STAT_PLLE
      | LPC17XX_PLL_STAT_PLOCK;

    if ((pllstat & enabled_and_locked) == enabled_and_locked) {
      unsigned m = LPC17XX_PLL_SEL_MSEL_GET(pllstat) + 1;

      pllclk = sysclk * m;
    }
  #endif

  return pllclk;
}

unsigned lpc24xx_cclk(void)
{
  #ifdef ARM_MULTILIB_ARCH_V4
    /* Get PLL output frequency */
    unsigned pllclk = lpc24xx_pllclk();

    /* Get CPU frequency */
    unsigned cclk = pllclk / (GET_CCLKCFG_CCLKSEL(CCLKCFG) + 1);
  #else
    volatile lpc17xx_scb *scb = &LPC17XX_SCB;
    unsigned cclksel = scb->cclksel;
    unsigned cclk_in = 0;
    unsigned cclk = 0;

    if ((cclksel & LPC17XX_SCB_CCLKSEL_CCLKSEL) != 0) {
      cclk_in = lpc24xx_pllclk();
    } else {
      cclk_in = lpc17xx_sysclk(scb->clksrcsel);
    }

    cclk = cclk_in / LPC17XX_SCB_CCLKSEL_CCLKDIV_GET(cclksel);
  #endif

  return cclk;
}
