/* SPDX-License-Identifier: BSD-2-Clause */

/*
 * Copyright (c) 2016 embedded brains GmbH & Co. KG
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

#include <bsp/atsam-spi.h>
#include <bsp/spi.h>

/** SPI0 MISO pin */
#define PIN_SPI0_MISO {PIO_PD20B_SPI0_MISO, PIOD, ID_PIOD, PIO_PERIPH_B, PIO_DEFAULT}
/** SPI0 MOSI pin */
#define PIN_SPI0_MOSI {PIO_PD21B_SPI0_MOSI, PIOD, ID_PIOD, PIO_PERIPH_B, PIO_DEFAULT}
/** SPI0 CS0 pin */
#define PIN_SPI0_NPCS0 {PIO_PB2D_SPI0_NPCS0, PIOB, ID_PIOB, PIO_PERIPH_D, PIO_DEFAULT}
/** SPI0 CS1_1 pin */
#define PIN_SPI0_NPCS1_1 {PIO_PA31A_SPI0_NPCS1, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
/** SPI0 CS1_2 pin */
#define PIN_SPI0_NPCS1_2 {PIO_PD25B_SPI0_NPCS1, PIOD, ID_PIOD, PIO_PERIPH_B, PIO_DEFAULT}
/** SPI0 CS2 pin */
#define PIN_SPI0_NPCS2 {PIO_PD12C_SPI0_NPCS2, PIOD, ID_PIOD, PIO_PERIPH_C, PIO_DEFAULT}
/** SPI0 CS3 pin */
#define PIN_SPI0_NPCS3 {PIO_PD27B_SPI0_NPCS3, PIOD, ID_PIOD, PIO_PERIPH_B, PIO_DEFAULT}
/** SPI0 Clock pin */
#define PIN_SPI0_CLOCK {PIO_PD22B_SPI0_SPCK, PIOD, ID_PIOD, PIO_PERIPH_B, PIO_DEFAULT}

/** SPI1 MISO pin */
#define PIN_SPI1_MISO {PIO_PC26C_SPI1_MISO, PIOC, ID_PIOC, PIO_PERIPH_C, PIO_DEFAULT}
/** SPI1 MOSI pin */
#define PIN_SPI1_MOSI {PIO_PC27C_SPI1_MOSI, PIOC, ID_PIOC, PIO_PERIPH_C, PIO_DEFAULT}
/** SPI1 CS0 pin */
#define PIN_SPI1_NPCS0 {PIO_PC25C_SPI1_NPCS0, PIOC, ID_PIOC, PIO_PERIPH_C, PIO_DEFAULT}
/** SPI1 CS1_1 pin */
#define PIN_SPI1_NPCS1_1 {PIO_PC28C_SPI1_NPCS1, PIOC, ID_PIOC, PIO_PERIPH_C, PIO_DEFAULT}
/** SPI1 CS1_2 pin */
#define PIN_SPI1_NPCS1_2 {PIO_PD0C_SPI1_NPCS1, PIOD, ID_PIOD, PIO_PERIPH_C, PIO_DEFAULT}
/** SPI1 CS2_1 pin */
#define PIN_SPI1_NPCS2_1 {PIO_PC29C_SPI1_NPCS2, PIOC, ID_PIOC, PIO_PERIPH_C, PIO_DEFAULT}
/** SPI1 CS2_2 pin */
#define PIN_SPI1_NPCS2_2 {PIO_PD1C_SPI1_NPCS2, PIOD, ID_PIOD, PIO_PERIPH_C, PIO_DEFAULT}
/** SPI1 CS3_1 pin */
#define PIN_SPI1_NPCS3_1 {PIO_PC30C_SPI1_NPCS3, PIOC, ID_PIOC, PIO_PERIPH_C, PIO_DEFAULT}
/** SPI1 CS3_2 pin */
#define PIN_SPI1_NPCS3_2 {PIO_PD2C_SPI1_NPCS3, PIOD, ID_PIOD, PIO_PERIPH_C, PIO_DEFAULT}
/** SPI1 Clock pin */
#define PIN_SPI1_CLOCK {PIO_PC24C_SPI1_SPCK, PIOC, ID_PIOC, PIO_PERIPH_C, PIO_DEFAULT}

int atsam_register_spi_0(void)
{
  static const Pin pins[] = {
    PIN_SPI0_MISO,
    PIN_SPI0_MOSI,
    PIN_SPI0_NPCS0,
    PIN_SPI0_NPCS1_1,
    PIN_SPI0_NPCS1_2,
    PIN_SPI0_NPCS2,
    PIN_SPI0_NPCS3,
    PIN_SPI0_CLOCK
  };

  static const atsam_spi_config config = {
    .spi_peripheral_id = ID_SPI0,
    .spi_regs = SPI0,
    .pins = pins,
    .pin_count = RTEMS_ARRAY_SIZE(pins),
    .chip_select_decode = false
  };

  return spi_bus_register_atsam(
    ATSAM_SPI_0_BUS_PATH,
    &config
  );
}

int atsam_register_spi_1(void)
{
  static const Pin pins[] = {
    PIN_SPI1_MISO,
    PIN_SPI1_MOSI,
    PIN_SPI1_NPCS0,
    PIN_SPI1_NPCS1_1,
    PIN_SPI1_NPCS1_2,
    PIN_SPI1_NPCS2_1,
    PIN_SPI1_NPCS2_2,
    PIN_SPI1_NPCS3_1,
    PIN_SPI1_NPCS3_2,
    PIN_SPI1_CLOCK
  };

  static const atsam_spi_config config = {
    .spi_peripheral_id = ID_SPI1,
    .spi_regs = SPI1,
    .pins = pins,
    .pin_count = RTEMS_ARRAY_SIZE(pins),
    .chip_select_decode = false
  };

  return spi_bus_register_atsam(
    ATSAM_SPI_1_BUS_PATH,
    &config
  );
}
