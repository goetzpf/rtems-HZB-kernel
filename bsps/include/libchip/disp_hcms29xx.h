/* SPDX-License-Identifier: BSD-2-Clause */

/*
 * Display driver for HCMS29xx
 *
 * This file declares the SPI based driver for a HCMS29xx 4 digit
 * alphanumeric LED display
 */

/*
 * Copyright (c) 2008 embedded brains GmbH & Co. KG
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

#ifndef _DISP_HCMS29XX_H
#define _DISP_HCMS29XX_H
#include <rtems.h>
#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif
#define DISP_HCMS29XX_TEXT_CNT (128)

  typedef struct {
    rtems_device_minor_number minor;   /* minor device number            */
    /*
     * in the disp_buffer, the string to be displayed is placed
     */
    char disp_buffer[DISP_HCMS29XX_TEXT_CNT];
    int  disp_buf_cnt; /* number of valid chars in disp_buffer */
    /*
     * in the trns buffer the string is transfered to display task
     */
    char trns_buffer[DISP_HCMS29XX_TEXT_CNT];
    /*
     * in the dev_buffer, characters will be accumulated before display...
     */
    char dev_buffer[DISP_HCMS29XX_TEXT_CNT];
    int  dev_buf_cnt; /* number of valid chars in dev_buffer */

    rtems_id trns_sema_id;  /* ID of disp trns buffer sema   */
    rtems_id task_id;       /* ID of disp task               */
    bool rotate;            /* FLAG: display is upside down       */
  } spi_disp_hcms29xx_param_t;

  typedef struct {
    rtems_libi2c_drv_t        libi2c_drv_entry;
    spi_disp_hcms29xx_param_t disp_param;
  } disp_hcms29xx_drv_t;
  /*
   * pass this descriptor pointer to rtems_libi2c_register_drv
   */
  extern rtems_libi2c_drv_t *disp_hcms29xx_driver_descriptor;

/*=========================================================================*\
| Function:                                                                 |
\*-------------------------------------------------------------------------*/
rtems_device_driver disp_hcms29xx_dev_initialize
  (
/*-------------------------------------------------------------------------*\
| Purpose:                                                                  |
|   prepare the display device driver to accept write calls                 |
|   register device with its name                                           |
+---------------------------------------------------------------------------+
| Input Parameters:                                                         |
\*-------------------------------------------------------------------------*/
  rtems_device_major_number  major,
  rtems_device_minor_number  minor,
  void                      *arg
   );
/*-------------------------------------------------------------------------*\
| Return Value:                                                             |
|    rtems_status_code                                                      |
\*=========================================================================*/

/*=========================================================================*\
| Function:                                                                 |
\*-------------------------------------------------------------------------*/
rtems_device_driver disp_hcms29xx_dev_open
(
/*-------------------------------------------------------------------------*\
| Purpose:                                                                  |
|   open the display device                                                 |
+---------------------------------------------------------------------------+
| Input Parameters:                                                         |
\*-------------------------------------------------------------------------*/
  rtems_device_major_number  major,
  rtems_device_minor_number  minor,
  void                      *arg
 );
/*-------------------------------------------------------------------------*\
| Return Value:                                                             |
|    rtems_status_code                                                      |
\*=========================================================================*/

/*=========================================================================*\
| Function:                                                                 |
\*-------------------------------------------------------------------------*/
rtems_device_driver disp_hcms29xx_dev_write
(
/*-------------------------------------------------------------------------*\
| Purpose:                                                                  |
|   write to display device                                                 |
+---------------------------------------------------------------------------+
| Input Parameters:                                                         |
\*-------------------------------------------------------------------------*/
  rtems_device_major_number  major,
  rtems_device_minor_number  minor,
  void                      *arg
 );
/*-------------------------------------------------------------------------*\
| Return Value:                                                             |
|    rtems_status_code                                                      |
\*=========================================================================*/

/*=========================================================================*\
| Function:                                                                 |
\*-------------------------------------------------------------------------*/
rtems_device_driver disp_hcms29xx_dev_close
(
/*-------------------------------------------------------------------------*\
| Purpose:                                                                  |
|   close the display device                                                |
+---------------------------------------------------------------------------+
| Input Parameters:                                                         |
\*-------------------------------------------------------------------------*/
  rtems_device_major_number  major,
  rtems_device_minor_number  minor,
  void                      *arg
 );
/*-------------------------------------------------------------------------*\
| Return Value:                                                             |
|    rtems_status_code                                                      |
\*=========================================================================*/

#define DISP_HCMS29XX_DRIVER {			\
    disp_hcms29xx_dev_initialize,		\
      disp_hcms29xx_dev_open,			\
      NULL,					\
      disp_hcms29xx_dev_write,			\
      NULL,					\
      disp_hcms29xx_dev_close}
  
    
#ifdef __cplusplus
}
#endif

#endif /* _DISP_HCMS29XX_H */
