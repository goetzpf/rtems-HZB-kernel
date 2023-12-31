/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup RTEMSAPIProfiling
 *
 * @brief This header file provides the Profiling Support API.
 */

/*
 * Copyright (c) 2014 embedded brains GmbH & Co. KG
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

#ifndef _RTEMS_PROFILING_H
#define _RTEMS_PROFILING_H

#include <stdint.h>

#include <rtems/print.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @defgroup RTEMSAPIProfiling Profiling Support
 *
 * @ingroup RTEMSAPI
 *
 * @brief The profiling support offers functions to report profiling
 * information available in the system.
 *
 * Profiling support is by default disabled.  It must be enabled via the
 * configure command line with the <tt>--enable-profiling</tt> option.  In this
 * case the RTEMS_PROFILING pre-processor symbol is defined and profiling
 * statistics will be gathered during system run-time.  The profiling support
 * increases the time of critical sections and has some memory overhead.  The
 * overhead should be acceptable for most applications.  The aim of the
 * profiling implementation is to be available even for production systems so
 * that verification is simplified.
 *
 * Profiling information includes critical timing values such as the maximum
 * time of disabled thread dispatching which is a measure for the thread
 * dispatch latency.  On SMP configurations statistics of all SMP locks in the
 * system are available.
 *
 * Profiling information can be retrieved via rtems_profiling_iterate() and
 * reported as an XML dump via rtems_profiling_report_xml().  These functions
 * are always available, but actual profiling data is only available if enabled
 * at build configuration time.
 *
 * @{
 */

/**
 * @brief Type of profiling data.
 */
typedef enum {
  /**
   * @brief Type of per-CPU profiling data.
   *
   * @see rtems_profiling_per_cpu.
   */
  RTEMS_PROFILING_PER_CPU,

  /**
   * @brief Type of SMP lock profiling data.
   *
   * @see rtems_profiling_smp_lock.
   */
  RTEMS_PROFILING_SMP_LOCK
} rtems_profiling_type;

/**
 * @brief The profiling data header.
 */
typedef struct {
  /**
   * @brief The profiling data type.
   */
  rtems_profiling_type type;
} rtems_profiling_header;

/**
 * @brief Per-CPU profiling data.
 *
 * Theoretically all values in this structure can overflow, but the integer
 * types are chosen so that they cannot overflow in practice.  On systems with
 * a 1GHz CPU counter, the 64-bit integers can overflow in about 58 years.
 * Since the system should not spend most of the time in critical sections the
 * actual system run-time is much longer.  Several other counters in the system
 * will overflow before we get a problem in the profiling area.
 */
typedef struct {
  /**
   * @brief The profiling data header.
   */
  rtems_profiling_header header;

  /**
   * @brief The processor index of this profiling data.
   */
  uint32_t processor_index;

  /**
   * @brief The maximum time of disabled thread dispatching in nanoseconds.
   */
  uint32_t max_thread_dispatch_disabled_time;

  /**
   * @brief Count of times when the thread dispatch disable level changes from
   * zero to one in thread context.
   *
   * This value may overflow.
   */
  uint64_t thread_dispatch_disabled_count;

  /**
   * @brief Total time of disabled thread dispatching in nanoseconds.
   *
   * The average time of disabled thread dispatching is the total time of
   * disabled thread dispatching divided by the thread dispatch disabled
   * count.
   *
   * This value may overflow.
   */
  uint64_t total_thread_dispatch_disabled_time;

  /**
   * @brief The maximum interrupt delay in nanoseconds if supported by the
   * hardware.
   *
   * The interrupt delay is the time interval from the recognition of an
   * interrupt signal by the hardware up to the execution start of the
   * corresponding high-level handler.  The interrupt delay is the main
   * contributor to the interrupt latency.  To measure this time hardware
   * support is required.  A time stamp unit must capture the interrupt signal
   * recognition time.  If no hardware support is available, then this field
   * will have a constant value of zero.
   */
  uint32_t max_interrupt_delay;

  /**
   * @brief The maximum time spent to process a single sequence of nested
   * interrupts in nanoseconds.
   *
   * This is the time interval between the change of the interrupt nest level
   * from zero to one and the change back from one to zero.  It is the measured
   * worst-case execution time of interrupt service routines.  Please note that
   * in case of nested interrupts this time includes the combined execution
   * time and not the maximum time of an individual interrupt service routine.
   */
  uint32_t max_interrupt_time;

  /**
   * @brief Count of times when the interrupt nest level changes from zero to
   * one.
   *
   * This value may overflow.
   */
  uint64_t interrupt_count;

  /**
   * @brief Total time of interrupt processing in nanoseconds.
   *
   * The average time of interrupt processing is the total time of interrupt
   * processing divided by the interrupt count.
   *
   * This value may overflow.
   */
  uint64_t total_interrupt_time;
} rtems_profiling_per_cpu;

/**
 * @brief Count of lock contention counters for SMP lock profiling.
 */
#define RTEMS_PROFILING_SMP_LOCK_CONTENTION_COUNTS 4

/**
 * @brief SMP lock profiling data.
 *
 * The lock acquire attempt instant is the point in time right after the
 * interrupt disable action in the lock acquire sequence.
 *
 * The lock acquire instant is the point in time right after the lock
 * acquisition.  This is the begin of the critical section code execution.
 *
 * The lock acquire time is the time elapsed between the lock acquire attempt
 * instant and the lock acquire instant.
 *
 * The lock release instant is the point in time right before the interrupt
 * enable action in the lock release sequence.
 *
 * The lock section time is the time elapsed between the lock acquire instant
 * and the lock release instant.
 */
typedef struct {
  /**
   * @brief The profiling data header.
   */
  rtems_profiling_header header;

  /**
   * @brief The lock name.
   */
  const char *name;

  /**
   * @brief The maximum lock acquire time in nanoseconds.
   */
  uint32_t max_acquire_time;

  /**
   * @brief The maximum lock section time in nanoseconds.
   */
  uint32_t max_section_time;

  /**
   * @brief The count of lock uses.
   *
   * This value may overflow.
   */
  uint64_t usage_count;

  /**
   * @brief Total lock acquire time in nanoseconds.
   *
   * The average lock acquire time is the total acquire time divided by the
   * lock usage count.  The ration of the total section and total acquire times
   * gives a measure for the lock contention.
   *
   * This value may overflow.
   */
  uint64_t total_acquire_time;

  /**
   * @brief Total lock section time in nanoseconds.
   *
   * The average lock section time is the total section time divided by the
   * lock usage count.
   *
   * This value may overflow.
   */
  uint64_t total_section_time;

  /**
   * @brief The counts of lock acquire operations by contention.
   *
   * The contention count for index N corresponds to a lock acquire attempt
   * with an initial queue length of N.  The last index corresponds to all
   * lock acquire attempts with an initial queue length greater than or equal
   * to RTEMS_PROFILING_SMP_LOCK_CONTENTION_COUNTS minus one.
   *
   * The values may overflow.
   */
  uint64_t contention_counts[RTEMS_PROFILING_SMP_LOCK_CONTENTION_COUNTS];
} rtems_profiling_smp_lock;

/**
 * @brief Collection of profiling data.
 */
typedef union {
  /**
   * @brief Header to specify the actual profiling data.
   */
  rtems_profiling_header header;

  /**
   * @brief Per-CPU profiling data if indicated by the header.
   */
  rtems_profiling_per_cpu per_cpu;

  /**
   * @brief SMP lock profiling data if indicated by the header.
   */
  rtems_profiling_smp_lock smp_lock;
} rtems_profiling_data;

/**
 * @brief Visitor function for the profiling iteration.
 *
 * @param[in, out] arg The visitor argument.
 * @param[in] data The current profiling data.
 *
 * @see rtems_profiling_iterate().
 */
typedef void (*rtems_profiling_visitor)(
  void *arg,
  const rtems_profiling_data *data
);

/**
 * @brief Iterates through all profiling data of the system.
 *
 * @param[in] visitor The visitor.
 * @param[in, out] visitor_arg The visitor argument.
 */
void rtems_profiling_iterate(
  rtems_profiling_visitor visitor,
  void *visitor_arg
);

/**
 * @brief Reports profiling data as XML.
 *
 * @param[in] name The name of the profiling report.
 * @param[in] printer The RTEMS printer to send the output too.
 * @param[in] indentation_level The current indentation level.
 * @param[in] indentation The string used for indentation.
 *
 * @returns As specified by printf().
 */
int rtems_profiling_report_xml(
  const char *name,
  const rtems_printer *printer,
  uint32_t indentation_level,
  const char *indentation
);

/** @} */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _RTEMS_PROFILING_H */
