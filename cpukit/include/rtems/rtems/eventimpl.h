/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup RTEMSImplClassicEvent
 *
 * @brief This header file provides the implementation interfaces of
 *   the @ref RTEMSImplClassicEvent.
 */

/*  COPYRIGHT (c) 1989-2008.
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

#ifndef _RTEMS_RTEMS_EVENTIMPL_H
#define _RTEMS_RTEMS_EVENTIMPL_H

#include <rtems/rtems/eventdata.h>
#include <rtems/score/thread.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup RTEMSImplClassicEvent Event Manager
 *
 * @ingroup RTEMSImplClassic
 *
 * @brief This group contains the Event Manager implementation.
 *
 * @{
 */

/**
 * @brief Seizes a set of events.
 *
 * @param event_in is the input event set.
 *
 * @param option_set is the option set.
 *
 * @param ticks is the optional timeout in clock ticks.
 *
 * @param event_out[out] is the output event set.
 *
 * @param executing[in, out] is the executing thread.
 *
 * @param event[in, out] is the source event set.
 *
 * @param wait_class is the thread wait class of the source event set.
 *
 * @param block_state is the thread blocking state of the source event set.
 *
 * @param lock_context[in, out] is the lock context set up by _Thread_Get().
 *
 * @retval ::RTEMS_SUCCESSFUL The requested operation was successful.
 *
 * @retval ::RTEMS_UNSATISFIED The events of interest were not immediately
 *   available.
 *
 * @retval ::RTEMS_TIMEOUT The events of interest were not available within the
 *   specified timeout interval.
 */
rtems_status_code _Event_Seize(
  rtems_event_set    event_in,
  rtems_option       option_set,
  rtems_interval     ticks,
  rtems_event_set   *event_out,
  Thread_Control    *executing,
  Event_Control     *event,
  Thread_Wait_flags  wait_class,
  States_Control     block_state,
  ISR_lock_Context  *lock_context
);

/**
 * @brief Surrenders a set of events.
 *
 * @param the_thread[in, out] is the thread of the event set.
 *
 * @param event_in is the set of events to post.
 *
 * @param event[in, out] is the target event set.
 *
 * @param wait_class is the thread wait class of the target event set.
 *
 * @param lock_context[in, out] is the lock context set up by _Thread_Get().
 *
 * @retval ::RTEMS_SUCCESSFUL The requested operation was successful.
 */
rtems_status_code _Event_Surrender(
  Thread_Control    *the_thread,
  rtems_event_set    event_in,
  Event_Control     *event,
  Thread_Wait_flags  wait_class,
  ISR_lock_Context  *lock_context
);

/**
 * @brief Initializes an event control block to have no pending events.
 *
 * @param event is the event control block to initialize.
 */
static inline void _Event_Initialize( Event_Control *event )
{
  event->pending_events = 0;
}

/**
 * @brief Checks if the event set is empty.
 *
 * @param the_event_set is the event set to check.
 *
 * @return Returns true, if there are no posted events in the event set,
 *   otherwise false.
 */
static inline bool _Event_sets_Is_empty(
  rtems_event_set the_event_set
)
{
  return ( the_event_set == 0 );
}

/**
 * @brief Posts the events in the specified event set.
 *
 * @param the_new_events is the set of events to post.
 *
 * @param the_event_set[in, out] is the event set.
 */
static inline void _Event_sets_Post(
  rtems_event_set  the_new_events,
  rtems_event_set *the_event_set
)
{
  *the_event_set |= the_new_events;
}

/**
 * @brief Gets the events of the specified event condition.
 *
 * @param the_event_set is the event set.
 *
 * @param the_event_condition is the event condition defining the events of interest.
 *
 * @return Return the events of the event condition which are posted in the
 *   event set.
 */
static inline rtems_event_set _Event_sets_Get(
  rtems_event_set the_event_set,
  rtems_event_set the_event_condition
)
{
   return ( the_event_set & the_event_condition );
}

/**
 * @brief Clears a set of events from an event set.
 *
 * @param the_event_set is the event set.
 *
 * @param the_mask is the set of events to clear.
 *
 * @return Returns the event set with all event cleared specified by the event
 *   mask.
 */
static inline rtems_event_set _Event_sets_Clear(
 rtems_event_set the_event_set,
 rtems_event_set the_mask
)
{
   return ( the_event_set & ~(the_mask) );
}

/**@}*/

#ifdef __cplusplus
}
#endif

#if defined(RTEMS_MULTIPROCESSING)
#include <rtems/rtems/eventmp.h>
#endif

#endif
/* end of include file */
