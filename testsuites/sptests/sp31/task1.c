/* SPDX-License-Identifier: BSD-2-Clause */

/*
 *  COPYRIGHT (c) 1989-2011.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  Copyright (c) 2009 embedded brains GmbH & Co. KG
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

#include "system.h"

#include <rtems/rtems/timerimpl.h>

rtems_timer_service_routine Should_not_fire_TSR(
  rtems_id  ignored_id,
  void     *ignored_address
);

volatile int TSR_fired;

rtems_timer_service_routine Should_not_fire_TSR(
  rtems_id  ignored_id,
  void     *ignored_address
)
{
  TSR_fired = 1;
}

static rtems_timer_service_routine Do_nothing(
  rtems_id  ignored_id,
  void     *ignored_address
)
{
  /* Do nothing */
}

rtems_task Task_1(
  rtems_task_argument argument
)
{
  rtems_id                 tmid;
  rtems_id                 tmid2;
  rtems_time_of_day        time;
  rtems_status_code        status;
  rtems_timer_information  info;

/* Get id */

  puts( "TA1 - rtems_timer_ident - identing timer 1" );
  status = rtems_timer_ident( Timer_name[ 1 ], &tmid );
  directive_failed( status, "rtems_timer_ident" );
  printf( "TA1 - timer 1 has id (0x%" PRIxrtems_id ")\n", tmid );

  puts( "TA1 - rtems_timer_ident - identing timer 2" );
  status = rtems_timer_ident( Timer_name[ 2 ], &tmid2 );
  directive_failed( status, "rtems_timer_ident" );
  printf( "TA1 - timer 2 has id (0x%" PRIxrtems_id ")\n", tmid2 );

/* make sure insertion does not unintentionally fire a timer per PR147 */

  TSR_fired = 0;

  puts( "TA1 - rtems_timer_server_fire_after - 1 second" );
  status = rtems_timer_server_fire_after(
    tmid, rtems_clock_get_ticks_per_second(), Should_not_fire_TSR, NULL );
  directive_failed( status, "rtems_timer_server_fire_after" );

  puts( "TA1 - rtems_task_wake_after - 1/2 second" );
  status = rtems_task_wake_after( rtems_clock_get_ticks_per_second() / 2 );
  directive_failed( status, "rtems_timer_server_fire_after" );

  directive_failed( status, "rtems_timer_server_fire_after" );
  puts( "TA1 - rtems_timer_server_fire_after - timer 2 in 1/2 second" );
  status = rtems_timer_server_fire_after(
    tmid2, rtems_clock_get_ticks_per_second() / 2, Should_not_fire_TSR, NULL );
  directive_failed( status, "rtems_timer_server_fire_after" );

  if ( TSR_fired ) {
    puts( "TA1 - TSR fired and should not have!" );
    rtems_test_exit(1);
  }

  puts( "TA1 - rtems_timer_cancel - timer 1" );
  status = rtems_timer_cancel( tmid );
  directive_failed( status, "rtems_timer_cancel" );

  puts( "TA1 - rtems_timer_cancel - timer 2" );
  status = rtems_timer_cancel( tmid2 );
  directive_failed( status, "rtems_timer_cancel" );


/* now check that rescheduling an active timer works OK. */
  puts( "TA1 - rtems_timer_server_fire_after - timer 1 in 30 seconds" );
  status = rtems_timer_server_fire_after(
    tmid, 30 * rtems_clock_get_ticks_per_second(), Delayed_resume, NULL );
  directive_failed( status, "rtems_timer_server_fire_after" );

  puts( "TA1 - rtems_timer_server_fire_after - timer 2 in 60 seconds" );
  status = rtems_timer_server_fire_after(
    tmid2, 60 * rtems_clock_get_ticks_per_second(), Delayed_resume, NULL );
  directive_failed( status, "rtems_timer_server_fire_after" );

  status = rtems_timer_get_information( tmid, &info );
  printf(
    "Timer 1 scheduled for %" PRIdWatchdog_Interval " ticks since boot\n",
    info.start_time + info.initial
  );

  puts( "TA1 - rtems_task_wake_after - 1 second" );
  status = rtems_task_wake_after( 1 * rtems_clock_get_ticks_per_second() );
  directive_failed( status, "rtems_timer_wake_after" );

  puts( "TA1 - rtems_timer_server_fire_after - timer 2 in 60 seconds" );
  status = rtems_timer_server_fire_after(
    tmid2, 60 * rtems_clock_get_ticks_per_second(), Delayed_resume, NULL );
  directive_failed( status, "rtems_timer_server_fire_after" );

  status = rtems_timer_get_information( tmid, &info );
  directive_failed( status, "rtems_timer_get_information" );
  printf(
    "Timer 1 scheduled for %" PRIdWatchdog_Interval " ticks since boot\n",
    info.start_time + info.initial
  );

  puts( "TA1 - rtems_task_wake_after - 1 second" );
  status = rtems_task_wake_after( 1 * rtems_clock_get_ticks_per_second() );
  directive_failed( status, "rtems_timer_wake_after" );

  puts( "TA1 - rtems_timer_server_fire_after - timer 2 in 60 seconds" );
  status = rtems_timer_server_fire_after(
    tmid2, 60 * rtems_clock_get_ticks_per_second(), Delayed_resume, NULL );
  directive_failed( status, "rtems_timer_server_fire_after" );

  status = rtems_timer_get_information( tmid, &info );
  directive_failed( status, "rtems_timer_get_information" );
  printf(
    "Timer 1 scheduled for %" PRIdWatchdog_Interval " ticks since boot\n",
     info.start_time + info.initial
  );

  puts( "TA1 - rtems_timer_cancel - timer 1" );
  status = rtems_timer_cancel( tmid );
  directive_failed( status, "rtems_timer_cancel" );

  puts( "TA1 - rtems_timer_cancel - timer 2" );
  status = rtems_timer_cancel( tmid2 );
  directive_failed( status, "rtems_timer_cancel" );

/* after which is allowed to fire */

  Print_time();

  puts( "TA1 - rtems_timer_server_fire_after - timer 1 in 3 seconds" );
  status = rtems_timer_server_fire_after(
    tmid,
    3 * rtems_clock_get_ticks_per_second(),
    Delayed_resume,
    NULL
  );
  directive_failed( status, "rtems_timer_server_fire_after" );

  puts( "TA1 - rtems_task_suspend( RTEMS_SELF )" );
  status = rtems_task_suspend( RTEMS_SELF );
  directive_failed( status, "rtems_task_suspend" );

  Print_time();

/* after which is reset and allowed to fire */

  puts( "TA1 - rtems_timer_server_fire_after - timer 1 in 3 seconds" );
  status = rtems_timer_server_fire_after(
    tmid,
    3 * rtems_clock_get_ticks_per_second(),
    Delayed_resume,
    NULL
  );
  directive_failed( status, "rtems_timer_server_fire_after" );

  puts( "TA1 - rtems_task_wake_after - 1 second" );
  status = rtems_task_wake_after( 1 * rtems_clock_get_ticks_per_second() );
  directive_failed( status, "rtems_task_wake_after" );

  Print_time();

  puts( "TA1 - rtems_timer_reset - timer 1" );
  status = rtems_timer_reset( tmid );
  directive_failed( status, "rtems_timer_reset" );

  puts( "TA1 - rtems_task_suspend( RTEMS_SELF )" );
  status = rtems_task_suspend( RTEMS_SELF );
  directive_failed( status, "rtems_task_suspend" );

  Print_time();

  /*
   *  Reset the time since we do not know how long the user waited
   *  before pressing <cr> at the pause.  This insures that the
   *  actual output matches the screen.
   */

  build_time( &time, 12, 31, 1988, 9, 0, 7, 0 );

  status = rtems_clock_set( &time );
  directive_failed( status, "rtems_clock_set" );

/* after which is canceled */

  puts( "TA1 - rtems_timer_server_fire_after - timer 1 in 3 seconds" );
  status = rtems_timer_server_fire_after(
    tmid,
    3 * rtems_clock_get_ticks_per_second(),
    Delayed_resume,
    NULL
  );
  directive_failed( status, "rtems_timer_server_fire_after" );

  puts( "TA1 - rtems_timer_cancel - timer 1" );
  status = rtems_timer_cancel( tmid );
  directive_failed( status, "rtems_timer_cancel" );

/* when which is allowed to fire */

  Print_time();

  status = rtems_clock_get_tod( &time );
  directive_failed( status, "rtems_clock_get_tod" );

  time.second += 3;

  puts( "TA1 - rtems_timer_server_fire_when - timer 1 in 3 seconds" );
  status = rtems_timer_server_fire_when( tmid, &time, Delayed_resume, NULL );
  directive_failed( status, "rtems_timer_server_fire_when" );

  puts( "TA1 - rtems_task_suspend( RTEMS_SELF )" );
  status = rtems_task_suspend( RTEMS_SELF );
  directive_failed( status, "rtems_task_suspend" );

  Print_time();

/* when which is canceled */

  status = rtems_clock_get_tod( &time );
  directive_failed( status, "rtems_clock_get_tod" );

  time.second += 3;

  puts( "TA1 - rtems_timer_server_fire_when - timer 1 in 3 seconds" );
  status = rtems_timer_server_fire_when( tmid, &time, Delayed_resume, NULL );
  directive_failed( status, "rtems_timer_server_fire_when" );

  puts( "TA1 - rtems_task_wake_after - 1 second" );
  status = rtems_task_wake_after( 1 * rtems_clock_get_ticks_per_second() );
  directive_failed( status, "rtems_task_wake_after" );

  Print_time();

  puts( "TA1 - rtems_timer_cancel - timer 1" );
  status = rtems_timer_cancel( tmid );
  directive_failed( status, "rtems_timer_cancel" );

/* TOD timer insert with non empty TOD timer chain */

  status = rtems_clock_get_tod( &time );
  directive_failed( status, "rtems_clock_get_tod" );

  time.second += 3;

  puts( "TA1 - rtems_timer_server_fire_when - timer 1 in 3 seconds" );
  status = rtems_timer_server_fire_when( tmid, &time, Do_nothing, NULL );
  directive_failed( status, "rtems_timer_server_fire_when" );

  puts( "TA1 - rtems_timer_server_fire_when - timer 2 in 3 seconds" );
  status = rtems_timer_server_fire_when( tmid2, &time, Do_nothing, NULL );
  directive_failed( status, "rtems_timer_server_fire_when" );

  puts( "TA1 - rtems_task_wake_after - 1 second" );
  status = rtems_task_wake_after( 1 * rtems_clock_get_ticks_per_second() );
  directive_failed( status, "rtems_task_wake_after" );

  puts( "TA1 - rtems_timer_server_fire_when - timer 2 in 3 seconds" );
  status = rtems_timer_server_fire_when( tmid2, &time, Do_nothing, NULL );
  directive_failed( status, "rtems_timer_server_fire_when" );

  puts( "TA1 - rtems_timer_cancel - timer 1" );
  status = rtems_timer_cancel( tmid );
  directive_failed( status, "rtems_timer_cancel" );

  puts( "TA1 - rtems_timer_cancel - timer 2" );
  status = rtems_timer_cancel( tmid2 );
  directive_failed( status, "rtems_timer_cancel" );

/* TOD chain processing with time wrap */

  time.second = 30;

  status = rtems_clock_set( &time );
  directive_failed( status, "rtems_clock_set" );

  time.second = 31;

  puts( "TA1 - rtems_timer_server_fire_when - timer 1 in 1 seconds" );
  status = rtems_timer_server_fire_when( tmid, &time, Do_nothing, NULL );
  directive_failed( status, "rtems_timer_server_fire_when" );

  time.second = 29;

  status = rtems_clock_set( &time );
  directive_failed( status, "rtems_clock_set" );

  puts( "TA1 - rtems_timer_server_fire_after - timer 2 in 1 tick" );
  status = rtems_timer_server_fire_after( tmid2, 1, Do_nothing, NULL );
  directive_failed( status, "rtems_timer_server_fire_after" );

  puts( "TA1 - rtems_task_wake_after - 1 tick" );
  status = rtems_task_wake_after( 1 );
  directive_failed( status, "rtems_task_wake_after" );

  puts( "TA1 - rtems_timer_cancel - timer 1" );
  status = rtems_timer_cancel( tmid );
  directive_failed( status, "rtems_timer_cancel" );

/* delete */
  puts( "TA1 - rtems_task_wake_after - YIELD (only task at priority)" );
  status = rtems_task_wake_after( RTEMS_YIELD_PROCESSOR );
  directive_failed( status, "rtems_task_wake_after" );

  puts( "TA1 - timer_deleting - timer 1" );
  status = rtems_timer_delete( tmid );
  directive_failed( status, "rtems_timer_delete" );

  TEST_END();
  rtems_test_exit( 0 );
}
