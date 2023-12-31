/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup RTEMSBSPsARMLPC24XX
 *
 * @brief BSP start EMC static memory configuration.
 */

/*
 * Copyright (c) 2011 embedded brains GmbH & Co. KG
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

#include <bsp/start-config.h>
#include <bsp/lpc24xx.h>

BSP_START_DATA_SECTION const lpc24xx_emc_static_chip_config
   lpc24xx_start_config_emc_static_chip [] = {
#if defined(LPC24XX_EMC_M29W160E)
  /*
   * Static Memory 1: Numonyx M29W160EB
   *
   * 1 clock cycle = 1/72MHz = 13.9ns
   */
  {
    .chip_select = (volatile lpc_emc_static *) EMC_STA_BASE_1,
    .config = {
      /*
       * 16 bit, page mode disabled, active LOW chip select, extended wait
       * disabled, writes not protected, byte lane state LOW/LOW (!).
       */
      .config = 0x81,

      /* 1 clock cycles delay from the chip select 1 to the write enable */
      .waitwen = 0,

      /*
       * 0 clock cycles delay from the chip select 1 or address change
       * (whichever is later) to the output enable
       */
      .waitoen = 0,

      /* 7 clock cycles delay from the chip select 1 to the read access */
      .waitrd = 0x6,

      /*
       * 32 clock cycles delay for asynchronous page mode sequential accesses
       */
      .waitpage = 0x1f,

      /* 5 clock cycles delay from the chip select 1 to the write access */
      .waitwr = 0x3,

      /* 16 bus turnaround cycles */
      .waitrun = 0xf
    }
  }
#elif defined(LPC24XX_EMC_M29W320E70)
  /* Static Memory 0: M29W320E70 */
  {
    .chip_select = (volatile lpc_emc_static *) EMC_STA_BASE_0,
    .config = {
      /*
       * 16 bit, page mode disabled, active LOW chip select, extended wait
       * disabled, writes not protected, byte lane state LOW/LOW.
       */
      .config = 0x81,

      /* 30ns (tWHWL) */
      .waitwen = LPC24XX_PS_TO_EMCCLK(30000, 1),

      /* 0ns */
      .waitoen = LPC24XX_PS_TO_EMCCLK(0, 1),

      /* 70ns (tAVQV, tELQV) */
      .waitrd = LPC24XX_PS_TO_EMCCLK(70000, 1),

      /* 70ns (tAVQV, tELQV) */
      .waitpage = LPC24XX_PS_TO_EMCCLK(70000, 1),

      /* max(30ns (tWHWL) + 45ns (tWLWH), 70ns (tAVAV)) */
      .waitwr = LPC24XX_PS_TO_EMCCLK(75000, 2),

      /* 25ns (tEHQZ) */
      .waitrun = LPC24XX_PS_TO_EMCCLK(25000, 1)
    }
  }
#elif defined(LPC24XX_EMC_SST39VF3201)
  /* Static Memory 0: SST39VF3201 */
  {
    .chip_select = (volatile lpc_emc_static *) EMC_STA_BASE_0,
    .config = {
      /*
       * 16 bit, page mode disabled, active LOW chip select, extended wait
       * disabled, writes not protected, byte lane state LOW/LOW.
       */
      .config = 0x81,

      /* 0ns (tCS, tAS) */
      .waitwen = LPC24XX_PS_TO_EMCCLK(0, 1),

      /* 0ns (tOES) */
      .waitoen = LPC24XX_PS_TO_EMCCLK(0, 1),

      /* 70ns (tRC) */
      .waitrd = LPC24XX_PS_TO_EMCCLK(70000, 1),

      /* 70ns (tRC) */
      .waitpage = LPC24XX_PS_TO_EMCCLK(70000, 1),

      /* 20ns (tCHZ, TOHZ) */
      .waitwr = LPC24XX_PS_TO_EMCCLK(20000, 2),

      /* 20ns (tCHZ, TOHZ) */
      .waitrun = LPC24XX_PS_TO_EMCCLK(20000, 1)
    }
  }
#endif
};

BSP_START_DATA_SECTION const size_t
  lpc24xx_start_config_emc_static_chip_count =
    sizeof(lpc24xx_start_config_emc_static_chip)
      / sizeof(lpc24xx_start_config_emc_static_chip [0]);
