/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @brief Inter-Integrated Circuit (I2C) Driver API
 *
 * @ingroup I2C
 */

/*
 * Copyright (C) 2014, 2017 embedded brains GmbH & Co. KG
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

#ifndef _DEV_I2C_I2C_H
#define _DEV_I2C_I2C_H

#include <linux/i2c.h>
#include <linux/i2c-dev.h>

#include <rtems.h>
#include <rtems/seterr.h>
#include <rtems/thread.h>

#include <sys/ioctl.h>
#include <sys/stat.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef struct i2c_msg i2c_msg;

typedef struct i2c_bus i2c_bus;

typedef struct i2c_dev i2c_dev;

typedef struct i2c_rdwr_ioctl_data i2c_rdwr_ioctl_data;

/**
 * @defgroup I2C Inter-Integrated Circuit (I2C) Driver
 *
 * @ingroup RTEMSDeviceDrivers
 *
 * @brief Inter-Integrated Circuit (I2C) bus and device driver support.
 *
 * @{
 */

/**
 * @defgroup I2CBus I2C Bus Driver
 *
 * @ingroup I2C
 *
 * @{
 */

/**
 * @name I2C IO Control Commands
 *
 * @{
 */

/**
 * @brief Obtains the bus.
 *
 * This command has no argument.
 */
#define I2C_BUS_OBTAIN 0x800

/**
 * @brief Releases the bus.
 *
 * This command has no argument.
 */
#define I2C_BUS_RELEASE 0x801

/**
 * @brief Gets the bus control.
 *
 * The argument type is a pointer to i2c_bus pointer.
 */
#define I2C_BUS_GET_CONTROL 0x802

/**
 * @brief Sets the bus clock in Hz.
 *
 * The argument type is unsigned long.
 */
#define I2C_BUS_SET_CLOCK 0x803

/** @} */

/**
 * @brief Default I2C bus clock in Hz.
 */
#define I2C_BUS_CLOCK_DEFAULT 100000

/**
 * @brief I2C bus control.
 */
struct i2c_bus {
  /**
   * @brief Transfers I2C messages.
   *
   * @param[in] bus The bus control.
   * @param[in] msgs The messages to transfer.
   * @param[in] msg_count The count of messages to transfer.  It must be
   * positive.
   *
   * @retval 0 Successful operation.
   * @retval negative Negative error number in case of an error.
   */
  int (*transfer)(i2c_bus *bus, i2c_msg *msgs, uint32_t msg_count);

  /**
   * @brief Sets the bus clock.
   *
   * @param[in] bus The bus control.
   * @param[in] clock The desired bus clock in Hz.
   *
   * @retval 0 Successful operation.
   * @retval negative Negative error number in case of an error.
   */
  int (*set_clock)(i2c_bus *bus, unsigned long clock);

  /**
   * @brief Destroys the bus.
   *
   * @param[in] bus The bus control.
   */
  void (*destroy)(i2c_bus *bus);

  /**
   * @brief Mutex to protect the bus access.
   */
  rtems_recursive_mutex mutex;

  /**
   * @brief Default slave device address.
   */
  uint16_t default_address;

  /**
   * @brief Use 10-bit addresses.
   */
  bool ten_bit_address;

  /**
   * @brief Use SMBus PEC.
   */
  bool use_pec;

  /**
   * @brief Transfer retry count.
   */
  unsigned long retries;

  /**
   * @brief Transaction timeout in ticks.
   */
  rtems_interval timeout;

  /**
   * @brief Controller functionality.
   */
  unsigned long functionality;
};

/**
 * @brief Initializes a bus control.
 *
 * After a sucessful initialization the bus control must be destroyed via
 * i2c_bus_destroy().  A registered bus control will be automatically destroyed
 * in case the device file is unlinked.  Make sure to call i2c_bus_destroy() in
 * a custom destruction handler.
 *
 * @param[in] bus The bus control.
 *
 * @retval 0 Successful operation.
 * @retval -1 An error occurred.  The errno is set to indicate the error.
 *
 * @see i2c_bus_register()
 */
int i2c_bus_init(i2c_bus *bus);

/**
 * @brief Allocates a bus control from the heap and initializes it.
 *
 * After a sucessful allocation and initialization the bus control must be
 * destroyed via i2c_bus_destroy_and_free().  A registered bus control will be
 * automatically destroyed in case the device file is unlinked.  Make sure to
 * call i2c_bus_destroy_and_free() in a custom destruction handler.
 *
 * @param[in] size The size of the bus control.  This enables the addition of
 * bus controller specific data to the base bus control.  The bus control is
 * zero initialized.
 *
 * @retval non-NULL The new bus control.
 * @retval NULL An error occurred.  The errno is set to indicate the error.
 *
 * @see i2c_bus_register()
 */
i2c_bus *i2c_bus_alloc_and_init(size_t size);

/**
 * @brief Destroys a bus control.
 *
 * @param[in] bus The bus control.
 */
void i2c_bus_destroy(i2c_bus *bus);

/**
 * @brief Destroys a bus control and frees its memory.
 *
 * @param[in] bus The bus control.
 */
void i2c_bus_destroy_and_free(i2c_bus *bus);

/**
 * @brief Registers a bus control.
 *
 * This function claims ownership of the bus control regardless if the
 * registration is successful or not.
 *
 * @param[in] bus The bus control.
 * @param[in] bus_path The path to the bus device file.
 *
 * @retval 0 Successful operation.
 * @retval -1 An error occurred.  The errno is set to indicate the error.
 */
int i2c_bus_register(
  i2c_bus *bus,
  const char *bus_path
);

/**
 * @brief Try to obtain the bus.
 *
 * @param[in] bus The bus control.
 *
 * @retval 0 Successful operation.
 * @retval EBUSY if mutex is already locked.
 */
int i2c_bus_try_obtain(i2c_bus *bus);

/**
 * @brief Obtains the bus.
 *
 * @param[in] bus The bus control.
 */
void i2c_bus_obtain(i2c_bus *bus);

/**
 * @brief Releases the bus.
 *
 * @param[in] bus The bus control.
 */
void i2c_bus_release(i2c_bus *bus);

/**
 * @brief Transfers I2C messages.
 *
 * The bus is obtained before the transfer and released afterwards. This is the
 * same like calling @ref i2c_bus_do_transfer with flags set to 0.
 *
 * @param[in] bus The bus control.
 * @param[in] msgs The messages to transfer.
 * @param[in] msg_count The count of messages to transfer.  It must be
 * positive.
 *
 * @retval 0 Successful operation.
 * @retval negative Negative error number in case of an error.
 */
int i2c_bus_transfer(i2c_bus *bus, i2c_msg *msgs, uint32_t msg_count);

/**
 * @brief Transfers I2C messages with optional flags.
 *
 * The bus is obtained before the transfer and released afterwards. If the flag
 * I2C_BUS_NOBLOCK is set and the bus is already obtained, nothing will be
 * transfered and the function returns with an -EAGAIN.
 *
 * @param[in] bus The bus control.
 * @param[in] msgs The messages to transfer.
 * @param[in] msg_count The count of messages to transfer.  It must be
 * positive.
 * @param[in] flags Options for the whole transfer.
 *
 * @retval 0 Successful operation.
 * @retval -EAGAIN if @ref I2C_BUS_NOBLOCK is set and the bus is already
 * obtained.
 * @retval negative Negative error number in case of an error.
 */
int i2c_bus_do_transfer(
  i2c_bus *bus,
  i2c_msg *msgs,
  uint32_t msg_count,
  uint32_t flags
);

/**
 * @brief I2C bus transfer flag to indicate that the task should not block if
 * the bus is busy on a new transfer.
 */
#define I2C_BUS_NOBLOCK (1u << 0)

/** @} */

/**
 * @defgroup I2CDevice I2C Device Driver
 *
 * @ingroup I2C
 *
 * @{
 */

/**
 * @brief Base number for device IO control commands.
 */
#define I2C_DEV_IO_CONTROL 0x900

/**
 * @brief I2C slave device control.
 */
struct i2c_dev {
  /**
   * @brief Reads from the device.
   *
   * @retval non-negative Bytes transferred from device.
   * @retval negative Negative error number in case of an error.
   */
  ssize_t (*read)(i2c_dev *dev, void *buf, size_t n, off_t offset);

  /**
   * @brief Writes to the device.
   *
   * @retval non-negative Bytes transferred to device.
   * @retval negative Negative error number in case of an error.
   */
  ssize_t (*write)(i2c_dev *dev, const void *buf, size_t n, off_t offset);

  /**
   * @brief Device IO control.
   *
   * @retval 0 Successful operation.
   * @retval negative Negative error number in case of an error.
   */
  int (*ioctl)(i2c_dev *dev, ioctl_command_t command, void *arg);

  /**
   * @brief Gets the file size.
   */
  off_t (*get_size)(i2c_dev *dev);

  /**
   * @brief Gets the file block size.
   */
  blksize_t (*get_block_size)(i2c_dev *dev);

  /**
   * @brief Destroys the device.
   */
  void (*destroy)(i2c_dev *dev);

  /**
   * @brief The bus control.
   */
  i2c_bus *bus;

  /**
   * @brief The device address.
   */
  uint16_t address;

  /**
   * @brief File descriptor of the bus.
   *
   * This prevents destruction of the bus since we hold a reference to it with
   * this.
   */
  int bus_fd;
};


/**
 * @brief Initializes a device control.
 *
 * After a sucessful initialization the device control must be destroyed via
 * i2c_dev_destroy().  A registered device control will be automatically
 * destroyed in case the device file is unlinked.  Make sure to call
 * i2c_dev_destroy_and_free() in a custom destruction handler.
 *
 * @param[in] device The device control.
 * @param[in] bus_path The path to the bus device file.
 * @param[in] address The address of the device.
 *
 * @retval 0 Successful operation.
 * @retval -1 An error occurred.  The errno is set to indicate the error.
 *
 * @see i2c_dev_register()
 */
int i2c_dev_init(i2c_dev *dev, const char *bus_path, uint16_t address);

/**
 * @brief Allocates a device control from the heap and initializes it.
 *
 * After a sucessful allocation and initialization the device control must be
 * destroyed via i2c_dev_destroy_and_free().  A registered device control will
 * be automatically destroyed in case the device file is unlinked.  Make sure
 * to call i2c_dev_destroy_and_free() in a custom destruction handler.
 *
 * @param[in] size The size of the device control.  This enables the addition
 * of device specific data to the base device control.  The device control is
 * zero initialized.
 * @param[in] bus_path The path to the bus device file.
 * @param[in] address The address of the device.
 *
 * @retval non-NULL The new device control.
 * @retval NULL An error occurred.  The errno is set to indicate the error.
 *
 * @see i2c_dev_register()
 */
i2c_dev *i2c_dev_alloc_and_init(
  size_t size,
  const char *bus_path,
  uint16_t address
);

/**
 * @brief Destroys a device control.
 *
 * @param[in] dev The device control.
 */
void i2c_dev_destroy(i2c_dev *dev);

/**
 * @brief Destroys a device control and frees its memory.
 *
 * @param[in] dev The device control.
 */
void i2c_dev_destroy_and_free(i2c_dev *dev);

/**
 * @brief Registers a device control.
 *
 * This function claims ownership of the device control regardless if the
 * registration is successful or not.
 *
 * @param[in] dev The dev control.
 * @param[in] dev_path The path to the device file of the device.
 *
 * @retval 0 Successful operation.
 * @retval -1 An error occurred.  The errno is set to indicate the error.
 */
int i2c_dev_register(
  i2c_dev *dev,
  const char *dev_path
);

/** @} */  /* end of i2c device driver */

/** @} */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _DEV_I2C_I2C_H */
