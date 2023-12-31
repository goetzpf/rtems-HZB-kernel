/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup RTEMSBSPsPowerPCMPC55XX
 *
 * @brief MMU configuration.
 */

/*
 * Copyright (C) 2008, 2012 embedded brains GmbH & Co. KG
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

#include <bsp/mpc55xx-config.h>

const struct MMU_tag mpc55xx_start_config_mmu [] = {
#if defined(MPC55XX_BOARD_GWLCFM)
  /* External Ethernet Controller 64k */
  MPC55XX_MMU_TAG_INITIALIZER(5, 0x3fff8000, MPC55XX_MMU_64K, 0, 1, 1, 1)
#elif defined(MPC55XX_BOARD_PHYCORE_MPC5554)
  /* Arguments macro:       idx,  addr,      size,              x, w, r, io */
  MPC55XX_MMU_TAG_INITIALIZER(8, 0x20000000, MPC55XX_MMU_8M,    1, 0, 1, 0), /* External FLASH 8M */
  MPC55XX_MMU_TAG_INITIALIZER(2, 0x21000000, MPC55XX_MMU_4M,    0, 1, 1, 0), /* Lower half SRAM */
  MPC55XX_MMU_TAG_INITIALIZER(5, 0x21400000, MPC55XX_MMU_4M,    1, 1, 1, 0), /* Upper half SRAM ("debug") */
  MPC55XX_MMU_TAG_INITIALIZER(6, 0x22000000, MPC55XX_MMU_16M,   0, 1, 1, 1), /* LAN91C111 */
  MPC55XX_MMU_TAG_INITIALIZER(7, 0x23000000, MPC55XX_MMU_16M,   0, 1, 1, 1), /* FPGA */
#elif defined(MPC55XX_BOARD_MPC5566EVB)
  /* Internal flash 3M */
  MPC55XX_MMU_TAG_INITIALIZER(1, 0x00000000, MPC55XX_MMU_64K, 1, 0, 1, 0),
  MPC55XX_MMU_TAG_INITIALIZER(6, 0x00010000, MPC55XX_MMU_64K, 1, 0, 1, 0),
  MPC55XX_MMU_TAG_INITIALIZER(7, 0x00020000, MPC55XX_MMU_64K, 1, 0, 1, 0),
  MPC55XX_MMU_TAG_INITIALIZER(8, 0x00030000, MPC55XX_MMU_64K, 1, 0, 1, 0),
  MPC55XX_MMU_TAG_INITIALIZER(9, 0x00040000, MPC55XX_MMU_256K, 1, 0, 1, 0),
  MPC55XX_MMU_TAG_INITIALIZER(10, 0x00080000, MPC55XX_MMU_256K, 1, 0, 1, 0),
  MPC55XX_MMU_TAG_INITIALIZER(11, 0x000c0000, MPC55XX_MMU_256K, 1, 0, 1, 0),
  MPC55XX_MMU_TAG_INITIALIZER(12, 0x00100000, MPC55XX_MMU_1M, 1, 0, 1, 0),
  MPC55XX_MMU_TAG_INITIALIZER(13, 0x00200000, MPC55XX_MMU_1M, 1, 0, 1, 0),
  /* External SRAM 512k */
  MPC55XX_MMU_TAG_INITIALIZER(2, 0x20000000, MPC55XX_MMU_256K, 0, 1, 1, 0),
  MPC55XX_MMU_TAG_INITIALIZER(14, 0x20040000, MPC55XX_MMU_256K, 0, 1, 1, 0),
  /* Internal SRAM 128k */
  MPC55XX_MMU_TAG_INITIALIZER(3, 0x40000000, MPC55XX_MMU_64K, 0, 1, 1, 0),
  MPC55XX_MMU_TAG_INITIALIZER(5, 0x40010000, MPC55XX_MMU_64K, 0, 1, 1, 0),
  /* External Ethernet Controller 64k */
  MPC55XX_MMU_TAG_INITIALIZER(15, 0x3fff8000, MPC55XX_MMU_64K, 0, 1, 1, 1)
#elif defined(MPC55XX_BOARD_MPC5674FEVB)
  /* Internal flash 4M */
  MPC55XX_MMU_TAG_INITIALIZER(1, 0x00000000, MPC55XX_MMU_64K, 1, 0, 1, 0),
  MPC55XX_MMU_TAG_INITIALIZER(5, 0x00010000, MPC55XX_MMU_64K, 1, 0, 1, 0),
  MPC55XX_MMU_TAG_INITIALIZER(6, 0x00020000, MPC55XX_MMU_128K, 1, 0, 1, 0),
  MPC55XX_MMU_TAG_INITIALIZER(7, 0x00040000, MPC55XX_MMU_256K, 1, 0, 1, 0),
  MPC55XX_MMU_TAG_INITIALIZER(8, 0x00080000, MPC55XX_MMU_512K, 1, 0, 1, 0),
  MPC55XX_MMU_TAG_INITIALIZER(9, 0x00100000, MPC55XX_MMU_1M, 1, 0, 1, 0),
  MPC55XX_MMU_TAG_INITIALIZER(10, 0x00200000, MPC55XX_MMU_2M, 1, 0, 1, 0),
  /* External SRAM 512k */
  MPC55XX_MMU_TAG_INITIALIZER(2, 0x20000000, MPC55XX_MMU_512K, 0, 1, 1, 0),
  /* Internal SRAM 256k */
  MPC55XX_MMU_TAG_INITIALIZER(3, 0x40000000, MPC55XX_MMU_128K, 0, 1, 1, 0),
  MPC55XX_MMU_TAG_INITIALIZER(11, 0x40020000, MPC55XX_MMU_64K, 0, 1, 1, 0),
  MPC55XX_MMU_TAG_INITIALIZER(12, 0x40030000, MPC55XX_MMU_32K, 0, 1, 1, 0),
  MPC55XX_MMU_TAG_INITIALIZER(13, 0x40038000, MPC55XX_MMU_16K, 0, 1, 1, 0),
  MPC55XX_MMU_TAG_INITIALIZER(14, 0x4003c000, MPC55XX_MMU_16K, 0, 1, 1, 1),
  /* External Ethernet controller */
  MPC55XX_MMU_TAG_INITIALIZER(15, 0x3fff8000, MPC55XX_MMU_64K, 0, 1, 1, 1)
#elif defined(MPC55XX_BOARD_MPC5674F_ECU508)
  #if defined(MPC55XX_NEEDS_LOW_LEVEL_INIT)
    /* Arguments macro:       idx,  addr,       size,             x, w, r, io */

    /* Internal flash 4M */
    /* First 64k unused, to detect NULL pointer access */
    MPC55XX_MMU_TAG_INITIALIZER(1,  0x00000000, MPC55XX_MMU_64K,  1, 0, 1, 0),
    MPC55XX_MMU_TAG_INITIALIZER(5,  0x00010000, MPC55XX_MMU_64K,  1, 0, 1, 0),
    MPC55XX_MMU_TAG_INITIALIZER(6,  0x00020000, MPC55XX_MMU_128K, 1, 0, 1, 0),
    MPC55XX_MMU_TAG_INITIALIZER(7,  0x00040000, MPC55XX_MMU_256K, 1, 0, 1, 0),
    MPC55XX_MMU_TAG_INITIALIZER(8,  0x00080000, MPC55XX_MMU_512K, 1, 0, 1, 0),
    MPC55XX_MMU_TAG_INITIALIZER(9,  0x00100000, MPC55XX_MMU_1M,   1, 0, 1, 0),
    MPC55XX_MMU_TAG_INITIALIZER(10, 0x00200000, MPC55XX_MMU_2M,   1, 0, 1, 0),
    /* External SRAM 2M */
    #ifndef BSP_DATA_CACHE_USE_WRITE_THROUGH
      MPC55XX_MMU_TAG_INITIALIZER(2, 0x20000000, MPC55XX_MMU_2M,  0, 1, 1, 0),
    #else
      MPC55XX_MMU_TAG_INITIALIZER(2, 0x20000000, MPC55XX_MMU_2M,  0, 1, 1, 2),
    #endif
    /* Internal SRAM 256k */
    MPC55XX_MMU_TAG_INITIALIZER(3,  0x40000000, MPC55XX_MMU_256K, 0, 1, 1, 0),
    MPC55XX_MMU_TAG_INITIALIZER(11, 0x40020000, MPC55XX_MMU_64K,  0, 1, 1, 0),
    MPC55XX_MMU_TAG_INITIALIZER(12, 0x40030000, MPC55XX_MMU_32K,  0, 1, 1, 0),
    MPC55XX_MMU_TAG_INITIALIZER(13, 0x40038000, MPC55XX_MMU_16K,  0, 1, 1, 0),
    /* Used as cache-inhibited area (ADC, DSPI queues) */
    MPC55XX_MMU_TAG_INITIALIZER(14, 0x4003c000, MPC55XX_MMU_16K,  0, 1, 1, 1),
    /* External Ethernet controller */
    MPC55XX_MMU_TAG_INITIALIZER(15, 0x3fff8000, MPC55XX_MMU_1K,   0, 1, 1, 1),
    /* External MRAM 128k */
    MPC55XX_MMU_TAG_INITIALIZER(16, 0x3ffa0000, MPC55XX_MMU_128K, 0, 1, 1, 0),
    /* External ARCNET controller */
    MPC55XX_MMU_TAG_INITIALIZER(17, 0x3ffc0000, MPC55XX_MMU_1K,   0, 1, 1, 1)
    /* Peripheral Bridge A-Registers on MMU-table pos 4 */
    /* Peripheral Bridge B-Registers on MMU-table pos 0 */
  #else
    /* Used as cache-inhibited area (ADC, DSPI queues) */
    MPC55XX_MMU_TAG_INITIALIZER(14, 0x4003c000, MPC55XX_MMU_16K,  0, 1, 1, 1)
  #endif
#elif defined(MPC55XX_BOARD_MPC5674F_RSM6)
  /* Arguments macro:        idx, addr,       size,             x, w, r, io */

  /* Internal flash 4M */
  /* First 64k unused, to detect NULL pointer access */
  MPC55XX_MMU_TAG_INITIALIZER(1,  0x00000000, MPC55XX_MMU_64K,  1, 0, 1, 0),
  MPC55XX_MMU_TAG_INITIALIZER(5,  0x00010000, MPC55XX_MMU_64K,  1, 0, 1, 0),
  MPC55XX_MMU_TAG_INITIALIZER(6,  0x00020000, MPC55XX_MMU_128K, 1, 0, 1, 0),
  MPC55XX_MMU_TAG_INITIALIZER(7,  0x00040000, MPC55XX_MMU_256K, 1, 0, 1, 0),
  MPC55XX_MMU_TAG_INITIALIZER(8,  0x00080000, MPC55XX_MMU_512K, 1, 0, 1, 0),
  MPC55XX_MMU_TAG_INITIALIZER(9,  0x00100000, MPC55XX_MMU_1M,   1, 0, 1, 0),
  MPC55XX_MMU_TAG_INITIALIZER(10, 0x00200000, MPC55XX_MMU_2M,   1, 0, 1, 0),
  /* External MRAM 4M */
  MPC55XX_MMU_TAG_INITIALIZER(2,  0x20000000, MPC55XX_MMU_4M,   0, 1, 1, 0),
  /* Internal SRAM 256k */
  MPC55XX_MMU_TAG_INITIALIZER(3,  0x40000000, MPC55XX_MMU_256K, 0, 1, 1, 0),
  MPC55XX_MMU_TAG_INITIALIZER(11, 0x40020000, MPC55XX_MMU_64K,  0, 1, 1, 0),
  MPC55XX_MMU_TAG_INITIALIZER(12, 0x40030000, MPC55XX_MMU_32K,  0, 1, 1, 0),
  MPC55XX_MMU_TAG_INITIALIZER(13, 0x40038000, MPC55XX_MMU_16K,  0, 1, 1, 0),
  /* Used as cache-inhibited area (ADC, DSPI queues) */
  MPC55XX_MMU_TAG_INITIALIZER(14, 0x4003c000, MPC55XX_MMU_16K,  0, 1, 1, 1),
  /* External FPGA */
  MPC55XX_MMU_TAG_INITIALIZER(15, 0x21000000, MPC55XX_MMU_8M,   0, 1, 1, 1),
  /* External Ethernet controller */
  MPC55XX_MMU_TAG_INITIALIZER(16, 0x23000000, MPC55XX_MMU_1K,   0, 1, 1, 1)
#elif MPC55XX_CHIP_FAMILY == 564
  /* Internal flash 1M */
  MPC55XX_MMU_TAG_INITIALIZER(0, 0x00000000, MPC55XX_MMU_1M, 1, 0, 1, 0),
  /* IO */
  MPC55XX_MMU_TAG_INITIALIZER(1, 0xffe00000, MPC55XX_MMU_2M, 0, 1, 1, 1),
  MPC55XX_MMU_TAG_INITIALIZER(2, 0xc3f00000, MPC55XX_MMU_1M, 0, 1, 1, 1),
  /* Internal SRAM 64k + 64k */
  MPC55XX_MMU_TAG_INITIALIZER(3, 0x40000000, MPC55XX_MMU_64K, 0, 1, 1, 0),
  MPC55XX_MMU_TAG_INITIALIZER(4, 0x50000000, MPC55XX_MMU_64K, 0, 1, 1, 0)
#endif
};

const size_t mpc55xx_start_config_mmu_count [] = {
  RTEMS_ARRAY_SIZE(mpc55xx_start_config_mmu)
};
