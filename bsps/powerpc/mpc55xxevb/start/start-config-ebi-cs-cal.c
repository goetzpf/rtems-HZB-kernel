/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup RTEMSBSPsPowerPCMPC55XX
 *
 * @brief EBI calibration chip-select configuration.
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

#ifdef MPC55XX_HAS_EBI

const struct EBI_CAL_CS_tag mpc55xx_start_config_ebi_cal_cs [] = {
#if defined(MPC55XX_BOARD_MPC5674FEVB)
  /* External SRAM */
  {
    .BR = {
      .B = {
        .BA = 0x20000000 >> 15,
        .PS = 0,
        .AD_MUX = 1,
        .BL = 1,
        .WEBS = 0,
        .TBDIP = 1,
        .SETA = 0,
        .BI = 0,
        .V = 1
      }
    },
    .OR = {
      .B = {
        .AM = 0xfff80000 >> 15,
        .SCY = 0,
        .BSCY = 0
      }
    }
  },
  /* External Ethernet controller */
  {
    .BR = {
      .B = {
        .BA = 0x3fff8000 >> 15,
        .PS = 0,
        .AD_MUX = 1,
        .BL = 0,
        .WEBS = 0,
        .TBDIP = 0,
        .SETA = 0,
        .BI = 1,
        .V = 1
      }
    },
    .OR = {
      .B = {
        .AM = 0xfff80000 >> 15,
        .SCY = 1,
        .BSCY = 0
      }
    }
  }
#elif defined(MPC55XX_BOARD_MPC5674F_ECU508) \
  && defined(MPC55XX_NEEDS_LOW_LEVEL_INIT)
  /* D_CS0 for external SRAM */
  {
    .BR = {
      .B = {
        .BA = 0x20000000 >> 15,
        .PS = 0,
        .AD_MUX = 1,
        .BL = 1,
        .WEBS = 0,
        .TBDIP = 1,
        .SETA = 0,
        .BI = 0,
        .V = 1
      }
    },
    .OR = {
      .B = {
        .AM = 0xffe00000 >> 15,
        .SCY = 0,
        .BSCY = 0
      }
    }
  },

  /* D_CS1 for Ethernet Controller */
  {
    .BR = {
      .B = {
        .BA = 0x3fff8000 >> 15,
        .PS = 0,
        .AD_MUX = 1,
        .BL = 0,
        .WEBS = 0,
        .TBDIP = 0,
        .SETA = 0,
        .BI = 1,
        .V = 1
      }
    },
    .OR = {
      .B = {
        .AM = 0xffff8000 >> 15,
        .SCY = 1,
        .BSCY = 0
      }
    }
  },

  /* D_CS2 unused */
  {
    .BR = {
      .B = {
        .BA = 0x20000000 >> 15,
        .PS = 0,
        .AD_MUX = 1,
        .BL = 0,
        .WEBS = 0,
        .TBDIP = 0,
        .SETA = 0,
        .BI = 1,
        .V = 0
      }
    },
    .OR = {
      .B = {
        .AM = 0xfff80000 >> 15,
        .SCY = 0,
        .BSCY = 0
      }
    }
  },

  /* D_CS3 for MRAM, ARCNET */
  {
    .BR = {
      .B = {
        .BA = 0x3ff80000 >> 15,
        .PS = 1,
        .AD_MUX = 1,
        .BL = 0,
        .WEBS = 1,
        .TBDIP = 0,
        .SETA = 0,
        .BI = 1,
        .V = 1
    }
  },
    .OR = {
      .B = {
        .AM = 0xfff80000 >> 15,
        .SCY = 1,
        .BSCY = 0
      }
    }
  }
#elif defined(MPC55XX_BOARD_MPC5674F_RSM6)
  /* D_CS0 for MRAM */
  {
    .BR = {
      .B = {
        .BA = 0x20000000 >> 15,
        .PS = 0,
        .AD_MUX = 1,
        .BL = 0,
        .WEBS = 1,
        .TBDIP = 0,
        .SETA = 0,
        .BI = 1,
        .V = 1
      }
    },
    .OR = {
      .B = {
        .AM = 0xffc00000 >> 15,
        .SCY = 4,
        .BSCY = 0
      }
    }
  },

  /* D_CS1 for FPGA */
  {
    .BR = {
      .B = {
        .BA = 0x21000000 >> 15,
        .PS = 0,
        .AD_MUX = 1,
        .BL = 0,
        .WEBS = 0,
        .TBDIP = 0,
        .SETA = 0,
        .BI = 1,
        .V = 1
      }
    },
    .OR = {
      .B = {
        .AM = 0xff800000 >> 15,
        .SCY = 0,
        .BSCY = 0
      }
    }
  },

  /* D_CS2 unused */
  {
    .BR = { .R = 0x20000002 },
    .OR = { .R = 0xe0000000 }
  },

  /* D_CS3 for Ethernet Controller */
  {
    .BR = {
      .B = {
        .BA = 0x23000000 >> 15,
        .PS = 1,
        .AD_MUX = 1,
        .BL = 0,
        .WEBS = 1,
        .TBDIP = 0,
        .SETA = 0,
        .BI = 1,
        .V = 1
    }
  },
    .OR = {
      .B = {
        .AM = 0xfff80000 >> 15,
        .SCY = 8,
        .BSCY = 0
      }
    }
  }
#endif
};

const size_t mpc55xx_start_config_ebi_cal_cs_count [] = {
  RTEMS_ARRAY_SIZE(mpc55xx_start_config_ebi_cal_cs)
};

#endif /* MPC55XX_HAS_EBI */
