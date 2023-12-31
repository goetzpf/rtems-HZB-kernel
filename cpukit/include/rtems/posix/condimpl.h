/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * This include file contains the static inline implementation of the private 
 * inlined routines for POSIX condition variables.
 */

/*
 *  COPYRIGHT (c) 1989-2013.
 *  On-Line Applications Research Corporation (OAR).
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

#ifndef _RTEMS_POSIX_CONDIMPL_H
#define _RTEMS_POSIX_CONDIMPL_H

#include <errno.h>
#include <pthread.h>

#include <rtems/score/percpu.h>
#include <rtems/score/threadqimpl.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  unsigned long flags;
  Thread_queue_Syslock_queue Queue;
  pthread_mutex_t *mutex;
} POSIX_Condition_variables_Control;

#define POSIX_CONDITION_VARIABLES_CLOCK_MONOTONIC 0x1UL

#define POSIX_CONDITION_VARIABLES_FLAGS_MASK 0x1UL

#define POSIX_CONDITION_VARIABLES_MAGIC 0x18dfb1feUL

/**
 *  Constant to indicate condition variable does not currently have
 *  a mutex assigned to it.
 */
#define POSIX_CONDITION_VARIABLES_NO_MUTEX NULL

#define POSIX_CONDITION_VARIABLES_TQ_OPERATIONS &_Thread_queue_Operations_FIFO

#define POSIX_CONDITION_VARIABLE_OF_THREAD_QUEUE_QUEUE( queue ) \
  RTEMS_CONTAINER_OF( \
    queue, POSIX_Condition_variables_Control, Queue.Queue )

/**
 *  The default condition variable attributes structure.
 */
extern const pthread_condattr_t _POSIX_Condition_variables_Default_attributes;

static inline POSIX_Condition_variables_Control *_POSIX_Condition_variables_Get(
  pthread_cond_t *cond
)
{
  return (POSIX_Condition_variables_Control *) cond;
}

static inline void _POSIX_Condition_variables_Initialize(
  POSIX_Condition_variables_Control *the_cond,
  const pthread_condattr_t          *the_attr
)
{
  unsigned long flags;

  _Thread_queue_Queue_initialize( &the_cond->Queue.Queue, NULL );
  the_cond->mutex = POSIX_CONDITION_VARIABLES_NO_MUTEX;

  flags = (uintptr_t) the_cond ^ POSIX_CONDITION_VARIABLES_MAGIC;
  flags &= ~POSIX_CONDITION_VARIABLES_FLAGS_MASK;

  if ( the_attr->clock == CLOCK_MONOTONIC ) {
    flags |= POSIX_CONDITION_VARIABLES_CLOCK_MONOTONIC;
  }

  the_cond->flags = flags;
}

static inline void _POSIX_Condition_variables_Destroy(
  POSIX_Condition_variables_Control *the_cond
)
{
  the_cond->flags = ~the_cond->flags;
}

static inline clockid_t _POSIX_Condition_variables_Get_clock(
  unsigned long flags
)
{
  if ( ( flags & POSIX_CONDITION_VARIABLES_CLOCK_MONOTONIC ) != 0 ) {
    return CLOCK_MONOTONIC;
  }

  return CLOCK_REALTIME;
}

static inline Thread_Control *_POSIX_Condition_variables_Acquire(
  POSIX_Condition_variables_Control *the_cond,
  Thread_queue_Context              *queue_context
)
{
  ISR_Level       level;
  Thread_Control *executing;

  _Thread_queue_Context_ISR_disable( queue_context, level );
  _Thread_queue_Context_set_ISR_level( queue_context, level );
  executing = _Thread_Executing;
  _Thread_queue_Queue_acquire_critical(
    &the_cond->Queue.Queue,
    &executing->Potpourri_stats,
    &queue_context->Lock_context.Lock_context
  );

  return executing;
}

static inline void _POSIX_Condition_variables_Release(
  POSIX_Condition_variables_Control *the_cond,
  Thread_queue_Context              *queue_context
)
{
  _Thread_queue_Queue_release(
    &the_cond->Queue.Queue,
    &queue_context->Lock_context.Lock_context
  );
}

/**
 * @brief Implements wake up version of the "signal" operation.
 * 
 * A support routine which implements guts of the broadcast and single task
 * wake up version of the "signal" operation.
 */
int _POSIX_Condition_variables_Signal_support(
  pthread_cond_t            *cond,
  bool                       is_broadcast
);

/**
 * @brief POSIX condition variables wait support.
 *
 * A support routine which implements guts of the blocking, non-blocking, and
 * timed wait version of condition variable wait routines.
 */
int _POSIX_Condition_variables_Wait_support(
  pthread_cond_t            *cond,
  pthread_mutex_t           *mutex,
  const struct timespec     *abstime
);

bool _POSIX_Condition_variables_Auto_initialization(
  POSIX_Condition_variables_Control *the_cond
);

#define POSIX_CONDITION_VARIABLES_VALIDATE_OBJECT( the_cond, flags ) \
  do { \
    if ( ( the_cond ) == NULL ) { \
      return EINVAL; \
    } \
    flags = ( the_cond )->flags; \
    if ( \
      ( ( (uintptr_t) ( the_cond ) ^ POSIX_CONDITION_VARIABLES_MAGIC ) \
          & ~POSIX_CONDITION_VARIABLES_FLAGS_MASK ) \
        != ( flags & ~POSIX_CONDITION_VARIABLES_FLAGS_MASK ) \
    ) { \
      if ( !_POSIX_Condition_variables_Auto_initialization( the_cond ) ) { \
        return EINVAL; \
      } \
    } \
  } while ( 0 )

#ifdef __cplusplus
}
#endif

#endif
/*  end of include file */
