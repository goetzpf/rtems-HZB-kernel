/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @brief Driver Manager Configuration file.
 */

/*
 * COPYRIGHT (c) 2009 Cobham Gaisler AB.
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

/*
 *  The configuration consist of an array with function pointers that
 *  register one or more drivers that will be used by the Driver Manger.
 *
 *  The Functions are called in the order they are declared.
 *
 */

#ifndef _DRIVER_MANAGER_CONFDEFS_H_
#define _DRIVER_MANAGER_CONFDEFS_H_

#include "drvmgr.h"

#ifdef __cplusplus
extern "C" {
#endif

extern drvmgr_drv_reg_func drvmgr_drivers[];

#ifdef CONFIGURE_INIT

/*** AMBA Plug & Play Drivers ***/
extern void gptimer_register_drv(void);
extern void apbuart_cons_register_drv(void);
extern void greth_register_drv(void);
extern void grspw_register_drv(void);
extern void grspw2_register_drv(void);
extern void grcan_register_drv(void);
extern void occan_register_drv(void);
extern void gr1553_register(void);
extern void gr1553bc_register(void);
extern void gr1553bm_register(void);
extern void gr1553rt_register(void);
extern void b1553brm_register_drv(void);
extern void b1553rt_register_drv(void);
extern void grtm_register_drv(void);
extern void grtc_register_drv(void);
extern void pcif_register_drv(void);
extern void grpci_register_drv(void);
extern void mctrl_register_drv(void);
extern void l2cache_register_drv(void);
extern void griommu_register_drv(void);
extern void grpci2_register_drv(void);
extern void spictrl_register_drv(void);
extern void i2cmst_register_drv(void);
extern void grgpio_register_drv(void);
extern void grpwm_register_drv(void);
extern void gradcdac_register_drv(void);
extern void spwcuc_register(void);
extern void grctm_register(void);
extern void router_register_drv(void);
extern void ahbstat_register_drv(void);
extern void memscrub_register_drv(void);
extern void l4stat_register_drv(void);
extern void spwtdp_register_drv(void);


/*** LEON2 AMBA Hard coded bus Drivers ***/
extern void at697pci_register_drv(void);
extern void ambapp_leon2_register(void);


/*** PCI Bus Drivers (PCI Target drivers) ***/
extern void gr_rasta_adcdac_register_drv(void);
extern void gr_rasta_io_register_drv(void);
extern void gr_rasta_tmtc_register_drv(void);
extern void gr701_register_drv(void);
extern void gr_tmtc_1553_register_drv(void);
extern void gr_rasta_spw_router_register_drv(void);
extern void gr_cpci_leon4_n2x_register_drv(void);
extern void gr_cpci_gr740_register_drv(void);


/* CONFIGURE DRIVER MANAGER */
drvmgr_drv_reg_func drvmgr_drivers[] = {
	/*** AMBA Plug & Play Drivers ***/
#ifdef CONFIGURE_DRIVER_AMBAPP_GAISLER_GPTIMER
	gptimer_register_drv,
#endif
#ifdef CONFIGURE_DRIVER_AMBAPP_GAISLER_APBUART
	apbuart_cons_register_drv,
#endif
#ifdef CONFIGURE_DRIVER_AMBAPP_GAISLER_GRETH
	greth_register_drv,
#endif
#ifdef CONFIGURE_DRIVER_AMBAPP_GAISLER_GRSPW
	grspw_register_drv,
#endif
#ifdef CONFIGURE_DRIVER_AMBAPP_GAISLER_GRSPW2
	grspw2_register_drv,
#endif
#ifdef CONFIGURE_DRIVER_AMBAPP_GAISLER_GRCAN
	grcan_register_drv,
#endif
#ifdef CONFIGURE_DRIVER_AMBAPP_GAISLER_OCCAN
	occan_register_drv,
#endif
#ifdef CONFIGURE_DRIVER_AMBAPP_GAISLER_GR1553B
	gr1553_register,
#endif
#ifdef CONFIGURE_DRIVER_AMBAPP_GAISLER_GR1553BC
	gr1553bc_register,
#endif
#ifdef CONFIGURE_DRIVER_AMBAPP_GAISLER_GR1553BM
	gr1553bm_register,
#endif
#ifdef CONFIGURE_DRIVER_AMBAPP_GAISLER_GR1553RT
	gr1553rt_register,
#endif
#ifdef CONFIGURE_DRIVER_AMBAPP_GAISLER_B1553BRM
	b1553brm_register_drv,
#endif
#ifdef CONFIGURE_DRIVER_AMBAPP_GAISLER_B1553RT
	b1553rt_register_drv,
#endif
#ifdef CONFIGURE_DRIVER_AMBAPP_GAISLER_GRTM
	grtm_register_drv,
#endif
#ifdef CONFIGURE_DRIVER_AMBAPP_GAISLER_GRTC
	grtc_register_drv,
#endif
#ifdef CONFIGURE_DRIVER_AMBAPP_GAISLER_PCIF
	pcif_register_drv,
#endif
#ifdef CONFIGURE_DRIVER_AMBAPP_GAISLER_GRPCI
	grpci_register_drv,
#endif
#ifdef CONFIGURE_DRIVER_AMBAPP_GAISLER_GRPCI2
	grpci2_register_drv,
#endif
#ifdef CONFIGURE_DRIVER_AMBAPP_MCTRL
	mctrl_register_drv,
#endif
#ifdef CONFIGURE_DRIVER_AMBAPP_GAISLER_L2CACHE
	l2cache_register_drv,
#endif
#ifdef CONFIGURE_DRIVER_AMBAPP_GAISLER_GRIOMMU
	griommu_register_drv,
#endif
#ifdef CONFIGURE_DRIVER_AMBAPP_GAISLER_SPICTRL
	spictrl_register_drv,
#endif
#ifdef CONFIGURE_DRIVER_AMBAPP_GAISLER_I2CMST
	i2cmst_register_drv,
#endif
#ifdef CONFIGURE_DRIVER_AMBAPP_GAISLER_GRGPIO
	grgpio_register_drv,
#endif
#ifdef CONFIGURE_DRIVER_AMBAPP_GAISLER_GRPWM
	grpwm_register_drv,
#endif
#ifdef CONFIGURE_DRIVER_AMBAPP_GAISLER_GRADCDAC
	gradcdac_register_drv,
#endif
#ifdef CONFIGURE_DRIVER_AMBAPP_GAISLER_SPWCUC
	spwcuc_register,
#endif
#ifdef CONFIGURE_DRIVER_AMBAPP_GAISLER_GRCTM
	grctm_register,
#endif
#ifdef CONFIGURE_DRIVER_AMBAPP_GAISLER_SPW_ROUTER
	router_register_drv,
#endif
#ifdef CONFIGURE_DRIVER_AMBAPP_GAISLER_AHBSTAT
	ahbstat_register_drv,
#endif
#ifdef CONFIGURE_DRIVER_AMBAPP_GAISLER_MEMSCRUB
	memscrub_register_drv,
#endif
#ifdef CONFIGURE_DRIVER_AMBAPP_GAISLER_L4STAT
	l4stat_register_drv,
#endif
#ifdef CONFIGURE_DRIVER_AMBAPP_GAISLER_SPWTDP
	spwtdp_register_drv,
#endif

	/*** LEON2 AMBA Drivers ***/
#ifdef CONFIGURE_DRIVER_LEON2_AT697PCI
	at697pci_register_drv,
#endif
#ifdef CONFIGURE_DRIVER_LEON2_AMBAPP
	ambapp_leon2_register,
#endif

	/*** PCI Target Drivers ***/
#ifdef CONFIGURE_DRIVER_PCI_GR_RASTA_ADCDAC
	gr_rasta_adcdac_register_drv,
#endif
#ifdef CONFIGURE_DRIVER_PCI_GR_RASTA_IO
	gr_rasta_io_register_drv,
#endif
#ifdef CONFIGURE_DRIVER_PCI_GR_RASTA_TMTC
	gr_rasta_tmtc_register_drv,
#endif
#ifdef CONFIGURE_DRIVER_PCI_GR_701
	gr701_register_drv,
#endif
#ifdef CONFIGURE_DRIVER_PCI_GR_TMTC_1553
	gr_tmtc_1553_register_drv,
#endif
#ifdef CONFIGURE_DRIVER_PCI_GR_RASTA_SPW_ROUTER
	gr_rasta_spw_router_register_drv,
#endif
#ifdef CONFIGURE_DRIVER_PCI_GR_LEON4_N2X
	gr_cpci_leon4_n2x_register_drv,
#endif
#ifdef CONFIGURE_DRIVER_PCI_GR_CPCI_GR740
	gr_cpci_gr740_register_drv,
#endif


/* Macros for adding custom drivers without needing to recompile
 * kernel.
 */
#ifdef CONFIGURE_DRIVER_CUSTOM1
	DRIVER_CUSTOM1_REG,
#endif
#ifdef CONFIGURE_DRIVER_CUSTOM2
	DRIVER_CUSTOM2_REG,
#endif
#ifdef CONFIGURE_DRIVER_CUSTOM3
	DRIVER_CUSTOM3_REG,
#endif
#ifdef CONFIGURE_DRIVER_CUSTOM4
	DRIVER_CUSTOM4_REG,
#endif
#ifdef CONFIGURE_DRIVER_CUSTOM5
	DRIVER_CUSTOM5_REG,
#endif
#ifdef CONFIGURE_DRIVER_CUSTOM6
	DRIVER_CUSTOM6_REG,
#endif
#ifdef CONFIGURE_DRIVER_CUSTOM7
	DRIVER_CUSTOM7_REG,
#endif
#ifdef CONFIGURE_DRIVER_CUSTOM8
	DRIVER_CUSTOM8_REG,
#endif
#ifdef CONFIGURE_DRIVER_CUSTOM9
	DRIVER_CUSTOM9_REG,
#endif

	/* End array with NULL */
	NULL
};

#endif /* CONFIGURE_INIT */

#ifdef __cplusplus
}
#endif

#endif /* _DRIVER_MANAGER_CONFDEFS_H_ */
