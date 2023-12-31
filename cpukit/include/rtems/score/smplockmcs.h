/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup RTEMSScoreSMPLock
 *
 * @brief This header file provides the interfaces of the
 *   @ref RTEMSScoreSMPLock related to MCS locks.
 */

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

#ifndef _RTEMS_SCORE_SMPLOCKMCS_H
#define _RTEMS_SCORE_SMPLOCKMCS_H

#include <rtems/score/cpuopts.h>

#if defined(RTEMS_SMP)

#include <rtems/score/atomic.h>
#include <rtems/score/smplockstats.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @addtogroup RTEMSScoreSMPLock
 *
 * @{
 */

/**
 * @brief SMP Mellor-Crummey and Scott (MCS) lock context.
 */
typedef struct SMP_MCS_lock_Context {
  /**
   * @brief The next context on the queue if it exists.
   */
  union {
    /**
     * @brief The next context as an atomic unsigned integer pointer value.
     */
    Atomic_Uintptr atomic;

    /**
     * @brief The next context as a normal pointer.
     *
     * Only provided for debugging purposes.
     */
    struct SMP_MCS_lock_Context *normal;
  } next;

  /**
   * @brief Indicates if the lock is owned or free in case a previous context
   * exits on the queue.
   *
   * This field is initialized to a non-zero value.  The previous lock owner
   * (which is the owner of the previous context) will set it to zero during
   * its lock release.
   */
  Atomic_Uint locked;

#if defined(RTEMS_PROFILING)
  SMP_lock_Stats_context Stats_context;

  unsigned int queue_length;
#endif
} SMP_MCS_lock_Context;

/**
 * @brief SMP Mellor-Crummey and Scott (MCS) lock control.
 */
typedef struct {
  /**
   * @brief The queue tail context.
   *
   * The lock is free, in case this field is zero, otherwise it is locked by
   * the owner of the queue head.
   */
  union {
    /**
     * @brief The queue tail context as an atomic unsigned integer pointer
     * value.
     */
    Atomic_Uintptr atomic;

    /**
     * @brief The queue tail context as a normal pointer.
     *
     * Only provided for debugging purposes.
     */
    struct SMP_MCS_lock_Context *normal;
  } queue;
} SMP_MCS_lock_Control;

/**
 * @brief SMP MCS lock control initializer for static initialization.
 */
#define SMP_MCS_LOCK_INITIALIZER { { ATOMIC_INITIALIZER_UINTPTR( 0 ) } }

/**
 * @brief Initializes the SMP MCS lock.
 *
 * Concurrent initialization leads to unpredictable results.
 *
 * @param[in, out] lock The SMP MCS lock control.
 */
static inline void _SMP_MCS_lock_Initialize( SMP_MCS_lock_Control *lock )
{
  _Atomic_Init_uintptr( &lock->queue.atomic, 0 );
}

/**
 * @brief Destroys the SMP MCS lock.
 *
 * Concurrent destruction leads to unpredictable results.
 *
 * @param[out] lock The SMP MCS lock control.
 */
static inline void _SMP_MCS_lock_Destroy( SMP_MCS_lock_Control *lock )
{
  (void) lock;
}

/**
 * @brief Acquires the SMP MCS lock.
 *
 * @param[in, out] lock The lock to acquire.
 * @param[in, out] context The lock context.
 * @param stats the SMP lock statistics.
 */
static inline void _SMP_MCS_lock_Do_acquire(
  SMP_MCS_lock_Control   *lock,
  SMP_MCS_lock_Context   *context
#if defined(RTEMS_PROFILING)
  ,
  SMP_lock_Stats         *stats
#endif
)
{
  SMP_MCS_lock_Context           *previous;
#if defined(RTEMS_PROFILING)
  SMP_lock_Stats_acquire_context  acquire_context;

  _SMP_lock_Stats_acquire_begin( &acquire_context );
  context->queue_length = 0;
#endif

  _Atomic_Store_uintptr( &context->next.atomic, 0, ATOMIC_ORDER_RELAXED );
  _Atomic_Store_uint( &context->locked, 1, ATOMIC_ORDER_RELAXED );

  previous = (SMP_MCS_lock_Context *) _Atomic_Exchange_uintptr(
    &lock->queue.atomic,
    (uintptr_t) context,
    ATOMIC_ORDER_ACQ_REL
  );

  if ( previous != NULL ) {
    unsigned int locked;

    _Atomic_Store_uintptr(
      &previous->next.atomic,
      (uintptr_t) context,
      ATOMIC_ORDER_RELAXED
    );

    do {
      locked = _Atomic_Load_uint( &context->locked, ATOMIC_ORDER_ACQUIRE );
    } while ( locked != 0 );
  }

#if defined(RTEMS_PROFILING)
  _SMP_lock_Stats_acquire_end(
    &acquire_context,
    stats,
    &context->Stats_context,
    context->queue_length
  );
#endif
}

/**
 * @brief Acquires an SMP MCS lock.
 *
 * This function will not disable interrupts.  The caller must ensure that the
 * current thread of execution is not interrupted indefinite once it obtained
 * the SMP MCS lock.
 *
 * @param lock The SMP MCS lock control.
 * @param context The SMP MCS lock context.
 * @param stats The SMP lock statistics.
 */
#if defined(RTEMS_PROFILING)
  #define _SMP_MCS_lock_Acquire( lock, context, stats ) \
    _SMP_MCS_lock_Do_acquire( lock, context, stats )
#else
  #define _SMP_MCS_lock_Acquire( lock, context, stats ) \
    _SMP_MCS_lock_Do_acquire( lock, context )
#endif

/**
 * @brief Releases an SMP MCS lock.
 *
 * @param[in, out] lock The SMP MCS lock control.
 * @param[in, out] context The SMP MCS lock context.
 */
static inline void _SMP_MCS_lock_Release(
  SMP_MCS_lock_Control *lock,
  SMP_MCS_lock_Context *context
)
{
  SMP_MCS_lock_Context *next;

  next = (SMP_MCS_lock_Context *) _Atomic_Load_uintptr(
    &context->next.atomic,
    ATOMIC_ORDER_RELAXED
  );

  if ( next == NULL ) {
    uintptr_t expected;
    bool      success;

    expected = (uintptr_t) context;
    success = _Atomic_Compare_exchange_uintptr(
      &lock->queue.atomic,
      &expected,
      0,
      ATOMIC_ORDER_RELEASE,
      ATOMIC_ORDER_RELAXED
    );

    if ( success ) {
#if defined(RTEMS_PROFILING)
      _SMP_lock_Stats_release_update( &context->Stats_context );
#endif
      /* Nobody waits. So, we are done */
      return;
    }

    do {
      next = (SMP_MCS_lock_Context *) _Atomic_Load_uintptr(
        &context->next.atomic,
        ATOMIC_ORDER_RELAXED
      );
    } while ( next == NULL );
  }

#if defined(RTEMS_PROFILING)
  next->queue_length = context->queue_length + 1;
  _SMP_lock_Stats_release_update( &context->Stats_context );
#endif

  _Atomic_Store_uint( &next->locked, 0, ATOMIC_ORDER_RELEASE );
}

/** @} */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* RTEMS_SMP */

#endif /* _RTEMS_SCORE_SMPLOCKMCS_H */
