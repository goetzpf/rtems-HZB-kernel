/* SPDX-License-Identifier: BSD-2-Clause */

/*  AHB Status register driver
 *
 *  COPYRIGHT (c) 2009 - 2017.
 *  Cobham Gaisler AB.
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

#include <inttypes.h>
#include <string.h>
#include <rtems.h>
#include <rtems/bspIo.h>
#include <drvmgr/drvmgr.h>
#include <grlib/ambapp_bus.h>

#include <grlib/ahbstat.h>

#include <grlib/grlib_impl.h>

#define REG_WRITE(addr, val) (*(volatile uint32_t *)(addr) = (uint32_t)(val))
#define REG_READ(addr) (*(volatile uint32_t *)(addr))

void ahbstat_isr(void *arg);

/* AHB fail interrupt callback to user. This function is declared weak so that
 * the user can define a function pointer variable containing the address
 * responsible for handling errors
 *
 * minor              Index of AHBSTAT hardware
 * regs               Register address of AHBSTAT
 * status             AHBSTAT status register at IRQ
 * failing_address    AHBSTAT Failing address register at IRQ
 *
 * * User return 
 *  0: print error onto terminal with printk and reenable AHBSTAT
 *  1: just re-enable AHBSTAT
 *  2: just print error
 *  3: do nothing, let user do custom handling
 */
int (*ahbstat_error)(
	int minor,
	struct ahbstat_regs *regs,
	uint32_t status,
	uint32_t failing_address
	) __attribute__((weak)) = NULL;

#define AHBSTAT_STS_ME_BIT 13
#define AHBSTAT_STS_FW_BIT 12
#define AHBSTAT_STS_CF_BIT 11
#define AHBSTAT_STS_AF_BIT 10
#define AHBSTAT_STS_CE_BIT 9
#define AHBSTAT_STS_NE_BIT 8
#define AHBSTAT_STS_HW_BIT 7
#define AHBSTAT_STS_HM_BIT 3
#define AHBSTAT_STS_HS_BIT 0

#define AHBSTAT_STS_ME (1 << AHBSTAT_STS_ME_BIT)
#define AHBSTAT_STS_FW (1 << AHBSTAT_STS_FW_BIT)
#define AHBSTAT_STS_CF (1 << AHBSTAT_STS_CF_BIT)
#define AHBSTAT_STS_AF (1 << AHBSTAT_STS_AF_BIT)
#define AHBSTAT_STS_CE (1 << AHBSTAT_STS_CE_BIT)
#define AHBSTAT_STS_NE (1 << AHBSTAT_STS_NE_BIT)
#define AHBSTAT_STS_HW (1 << AHBSTAT_STS_HW_BIT)
#define AHBSTAT_STS_HM (0xf << AHBSTAT_STS_HM_BIT)
#define AHBSTAT_STS_HS (0x7 << AHBSTAT_STS_HS_BIT)

enum { DEVNAME_LEN = 9 };
struct ahbstat_priv {
	struct drvmgr_dev *dev;
	struct ahbstat_regs *regs;
	char devname[DEVNAME_LEN];
	int minor;
	/* Cached error */
	uint32_t last_status;
	uint32_t last_address;
	/* Spin-lock ISR protection */
	SPIN_DECLARE(devlock);
};

static int ahbstat_init2(struct drvmgr_dev *dev);

struct drvmgr_drv_ops ahbstat_ops =
{
	.init = {NULL, ahbstat_init2, NULL, NULL},
	.remove = NULL,
	.info = NULL
};

struct amba_dev_id ahbstat_ids[] =
{
	{VENDOR_GAISLER, GAISLER_AHBSTAT},
	{0, 0}		/* Mark end of table */
};

struct amba_drv_info ahbstat_drv_info =
{
	{
		DRVMGR_OBJ_DRV,			/* Driver */
		NULL,				/* Next driver */
		NULL,				/* Device list */
		DRIVER_AMBAPP_GAISLER_AHBSTAT_ID,/* Driver ID */
		"AHBSTAT_DRV",			/* Driver Name */
		DRVMGR_BUS_TYPE_AMBAPP,		/* Bus Type */
		&ahbstat_ops,
		NULL,				/* Funcs */
		0,				/* No devices yet */
		sizeof(struct ahbstat_priv),
	},
	&ahbstat_ids[0]
};

void ahbstat_register_drv (void)
{
	drvmgr_drv_register(&ahbstat_drv_info.general);
}

static int ahbstat_init2(struct drvmgr_dev *dev)
{
	struct ahbstat_priv *priv;
	struct amba_dev_info *ambadev;

	priv = dev->priv;
	if (!priv)
		return DRVMGR_NOMEM;
	priv->dev = dev;

	/* Get device information from AMBA PnP information */
	ambadev = (struct amba_dev_info *)dev->businfo;
	if (ambadev == NULL)
		return DRVMGR_FAIL;
	priv->regs = (struct ahbstat_regs *)ambadev->info.apb_slv->start;
	priv->minor = dev->minor_drv;

	strncpy(&priv->devname[0], "ahbstat0", DEVNAME_LEN);
	priv->devname[7] += priv->minor;
	/*
	 * Initialize spinlock for AHBSTAT Device. It is used to protect user
	 * API calls involivng priv structure from updates in ISR.
	 */
	SPIN_INIT(&priv->devlock, priv->devname);

	/* Initialize hardware */
	REG_WRITE(&priv->regs->status, 0);

	/* Install IRQ handler */
	drvmgr_interrupt_register(dev, 0, priv->devname, ahbstat_isr, priv);

	return DRVMGR_OK;
}

void ahbstat_isr(void *arg)
{
	struct ahbstat_priv *priv = arg;
	uint32_t fadr, status;
	int rc;
	SPIN_ISR_IRQFLAGS(lock_context);

	/* Get hardware status */
	status = REG_READ(&priv->regs->status);
	if ((status & AHBSTAT_STS_NE) == 0)
		return;

	/* IRQ generated by AHBSTAT core... handle it here */

	/* Get Failing address */
	fadr = REG_READ(&priv->regs->failing);

	SPIN_LOCK(&priv->devlock, lock_context);
	priv->last_status = status;
	priv->last_address = fadr;
	SPIN_UNLOCK(&priv->devlock, lock_context);

	/* Let user handle error, default to print the error and reenable HW
	 *
	 * User return 
	 *  0: print error and reenable AHBSTAT
	 *  1: just reenable AHBSTAT
	 *  2: just print error
	 *  3: do nothing
	 */
	rc = 0;
	if (ahbstat_error != NULL)
		rc = ahbstat_error(priv->minor, priv->regs, status, fadr);

	if ((rc & 0x1) == 0) {
		printk("\n### AHBSTAT: %s %s error of size %" PRId32
			" by master %" PRId32 " at 0x%08" PRIx32 "\n",
			status & AHBSTAT_STS_CE ? "single" : "non-correctable",
			status & AHBSTAT_STS_HW ? "write" : "read",
			(status & AHBSTAT_STS_HS) >> AHBSTAT_STS_HS_BIT,
			(status & AHBSTAT_STS_HM) >> AHBSTAT_STS_HM_BIT,
			fadr);
	}

	if ((rc & 0x2) == 0) {
		/* Trigger new interrupts */
		REG_WRITE(&priv->regs->status, 0);
	}
}

/* Get Last received AHB Error
 *
 * Return
 *   0: No error received
 *   1: Error Received, last status and address stored into argument pointers
 *  -1: No such AHBSTAT device
 */
int ahbstat_last_error(int minor, uint32_t *status, uint32_t *address)
{
	struct drvmgr_dev *dev;
	struct ahbstat_priv *priv;
	uint32_t last_status;
	uint32_t last_address;
	SPIN_IRQFLAGS(lock_context);

	if (drvmgr_get_dev(&ahbstat_drv_info.general, minor, &dev)) {
		return -1;
	}
	priv = (struct ahbstat_priv *)dev->priv;

	/* Read information cached by ISR */
	SPIN_LOCK_IRQ(&priv->devlock, lock_context);
	last_status = REG_READ(&priv->last_status);
	last_address = REG_READ(&priv->last_address);
	SPIN_UNLOCK_IRQ(&priv->devlock, lock_context);

	*status = last_status;
	*address = last_address;

	return (last_status & AHBSTAT_STS_NE) >> AHBSTAT_STS_NE_BIT;
}

/* Get AHBSTAT registers address from minor. NULL returned if no such device */
struct ahbstat_regs *ahbstat_get_regs(int minor)
{
	struct drvmgr_dev *dev;
	struct ahbstat_priv *priv;

	if (drvmgr_get_dev(&ahbstat_drv_info.general, minor, &dev)) {
		return NULL;
	}
	priv = (struct ahbstat_priv *)dev->priv;

	return priv->regs;
}
