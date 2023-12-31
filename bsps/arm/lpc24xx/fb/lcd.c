/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup RTEMSBSPsARMLPC24XX_lcd
 *
 * @brief LCD support.
 */

/*
 * Copyright (C) 2010, 2012 embedded brains GmbH & Co. KG
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

#include <assert.h>

#include <bsp/lpc24xx.h>
#include <bsp/lcd.h>
#include <bsp/lpc-lcd.h>
#include <bsp/utility.h>
#include <bsp/system-clocks.h>

#ifdef ARM_MULTILIB_ARCH_V4
  #define LCD_ENABLE BSP_BIT32(0)
#endif

rtems_status_code lpc24xx_lcd_set_mode(
  lpc24xx_lcd_mode mode,
  const lpc24xx_pin_range *pins
)
{
  rtems_status_code sc = RTEMS_SUCCESSFUL;
  bool enable = false;

  switch (mode) {
    case LCD_MODE_STN_4_BIT:
    case LCD_MODE_STN_8_BIT:
    case LCD_MODE_STN_DUAL_PANEL_4_BIT:
    case LCD_MODE_STN_DUAL_PANEL_8_BIT:
    case LCD_MODE_TFT_12_BIT_4_4_4:
    case LCD_MODE_TFT_16_BIT_5_6_5:
    case LCD_MODE_TFT_16_BIT_1_5_5_5:
    case LCD_MODE_TFT_24_BIT:
      enable = true;
      break;
    case LCD_MODE_DISABLED:
      enable = false;
      break;
    default:
      sc = RTEMS_IO_ERROR;
      break;
  }

  if (sc == RTEMS_SUCCESSFUL) {
    if (enable) {
      sc = lpc24xx_module_enable(LPC24XX_MODULE_LCD, LPC24XX_MODULE_PCLK_DEFAULT);
      assert(sc == RTEMS_SUCCESSFUL);

      #ifdef ARM_MULTILIB_ARCH_V4
        PINSEL11 = BSP_FLD32(mode, 1, 3) | LCD_ENABLE;
      #endif

      sc = lpc24xx_pin_config(pins, LPC24XX_PIN_SET_FUNCTION);
      assert(sc == RTEMS_SUCCESSFUL);
    } else {
      if (lpc24xx_lcd_current_mode() != LCD_MODE_DISABLED) {
        uint32_t lcd_ctrl = LCD_CTRL;

        /* Disable power */
        lcd_ctrl &= ~BSP_BIT32(11);
        LCD_CTRL = lcd_ctrl;

        lpc24xx_micro_seconds_delay(100000);

        /* Disable all signals */
        lcd_ctrl &= ~BSP_BIT32(0);
        LCD_CTRL = lcd_ctrl;
      }

      sc = lpc24xx_pin_config(pins, LPC24XX_PIN_SET_INPUT);
      assert(sc == RTEMS_SUCCESSFUL);

      #ifdef ARM_MULTILIB_ARCH_V4
        PINSEL11 = 0;
      #endif

      sc = lpc24xx_module_disable(LPC24XX_MODULE_LCD);
      assert(sc == RTEMS_SUCCESSFUL);
    }
  }

  return sc;
}

lpc24xx_lcd_mode lpc24xx_lcd_current_mode(void)
{
  #ifdef ARM_MULTILIB_ARCH_V4
    uint32_t pinsel11 = PINSEL11;

    if ((PCONP & BSP_BIT32(20)) != 0 && (pinsel11 & LCD_ENABLE) != 0) {
      return BSP_FLD32GET(pinsel11, 1, 3);
    } else {
      return LCD_MODE_DISABLED;
    }
  #else
    volatile lpc17xx_scb *scb = &LPC17XX_SCB;

    if ((scb->pconp & LPC17XX_SCB_PCONP_LCD) != 0) {
      return LCD_CTRL & 0xae;
    } else {
      return LCD_MODE_DISABLED;
    }
  #endif
}
