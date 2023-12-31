/*
 * RTEMS generic MPC5200 BSP
 *
 * This file contains the nvram declarations.
 *
 * M93C46 is a serial microwire EEPROM which contains
 * 1Kbit (128 bytes/64 words) of non-volatile memory.
 * The device can be configured for byte- or word-
 * access. The driver provides a file-like interface
 * to this memory.
 *
 * MPC5x00 PIN settings:
 *
 * PSC3_6 (output) -> MC93C46 serial data in    (D)
 * PSC3_7 (input)  -> MC93C46 serial data out   (Q)
 * PSC3_8 (output) -> MC93C46 chip select input (S)
 * PSC3_9 (output) -> MC93C46 serial clock      (C)
 *
 * References: DS1307-based Non-Volatile memory device driver
 * by Victor V. Vengerov.
 */

/*
 * Author: Victor V. Vengerov
 * Copyright (c) 2000 OKTET Ltd.,St.-Petersburg,Russia
 *
 * Copyright (c) 2003 IPR Engineering
 *
 * Copyright (c) 2005 embedded brains GmbH & Co. KG
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rtems.org/license/LICENSE.
 */

#ifndef __NVRAM_H__
#define __NVRAM_H__

#ifdef __cplusplus
extern "C" {
#endif

/* include <bsp.h> */
#include <bsp.h>

/* Macros for TBL read access */
#define TBL_READ(count)    __asm__ volatile ("mftb %0\n" : "=r" (count) : "0" (count))
#define TMBASE_CLOCK       (G2_CLOCK/4)

/* Simple usec delay function prototype */
void wait_usec(unsigned long);

/* nvram_driver_initialize --
 *     Non-volatile memory device driver initialization.
 */
rtems_device_driver
nvram_driver_initialize(rtems_device_major_number major,
                        rtems_device_minor_number minor,
                        void *arg);

/* nvram_driver_open --
 *     Non-volatile memory device driver open primitive.
 */
rtems_device_driver
nvram_driver_open(rtems_device_major_number major,
                  rtems_device_minor_number minor,
                  void *arg);

/* nvram_driver_close --
 *     Non-volatile memory device driver close primitive.
 */
rtems_device_driver
nvram_driver_close(rtems_device_major_number major,
                   rtems_device_minor_number minor,
                   void *arg);

/* nvram_driver_read --
 *     Non-volatile memory device driver read primitive.
 */
rtems_device_driver
nvram_driver_read(rtems_device_major_number major,
                  rtems_device_minor_number minor,
                  void *arg);

/* nvram_driver_write --
 *     Non-volatile memory device driver write primitive.
 */
rtems_device_driver
nvram_driver_write(rtems_device_major_number major,
                   rtems_device_minor_number minor,
                   void *arg);

#ifdef __cplusplus
}
#endif

#endif /* __NVRAM_H__ */
