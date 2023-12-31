/* SPDX-License-Identifier: BSD-2-Clause */

/**
 *  @file
 *
 *  @brief Wait at a Spinlock
 *  @ingroup POSIXAPI
 */

/*
 *  COPYRIGHT (c) 1989-2007.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  Copyright (c) 2016 embedded brains GmbH & Co. KG
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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems/posix/spinlockimpl.h>

RTEMS_STATIC_ASSERT(
#if defined(RTEMS_SMP)
  offsetof( POSIX_Spinlock_Control, Lock.next_ticket )
#else
  offsetof( POSIX_Spinlock_Control, reserved[ 0 ] )
#endif
    == offsetof( pthread_spinlock_t, _Lock._next_ticket ),
  POSIX_SPINLOCK_T_LOCK_NEXT_TICKET
);

RTEMS_STATIC_ASSERT(
#if defined(RTEMS_SMP)
  offsetof( POSIX_Spinlock_Control, Lock.now_serving )
#else
  offsetof( POSIX_Spinlock_Control, reserved[ 1 ] )
#endif
    == offsetof( pthread_spinlock_t, _Lock._now_serving ),
  POSIX_SPINLOCK_T_LOCK_NOW_SERVING
);

RTEMS_STATIC_ASSERT(
  offsetof( POSIX_Spinlock_Control, interrupt_state )
    == offsetof( pthread_spinlock_t, _interrupt_state ),
  POSIX_SPINLOCK_T_INTERRUPT_STATE
);

RTEMS_STATIC_ASSERT(
  sizeof( POSIX_Spinlock_Control ) == sizeof( pthread_spinlock_t ),
  POSIX_SPINLOCK_T_SIZE
);

int pthread_spin_lock( pthread_spinlock_t *spinlock )
{
  POSIX_Spinlock_Control *the_spinlock;
  ISR_Level               level;
#if defined(RTEMS_SMP) && defined(RTEMS_PROFILING)
  SMP_lock_Stats          unused_stats;
  SMP_lock_Stats_context  unused_context;
#endif

  the_spinlock = _POSIX_Spinlock_Get( spinlock );
  _ISR_Local_disable( level );
#if defined(RTEMS_SMP)
  _SMP_ticket_lock_Acquire(
    &the_spinlock->Lock,
    &unused_stats,
    &unused_context
  );
#endif
  the_spinlock->interrupt_state = level;
  return 0;
}

int pthread_spin_trylock( pthread_spinlock_t *spinlock )
  RTEMS_ALIAS( pthread_spin_lock );
