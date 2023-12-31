/* SPDX-License-Identifier: BSD-2-Clause */

/*
 *  This test exercises the POSIX Barrier manager.
 *
 *  COPYRIGHT (c) 1989-2009.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  Copyright (c) 2017 embedded brains GmbH & Co. KG
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

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

/* #define __USE_XOPEN2K XXX already defined on GNU/Linux */
#include <pthread.h>

#include "tmacros.h"

const char rtems_test_name[] = "PSXBARRIER 1";

static void test_barrier_null( void )
{
  int eno;

  eno = pthread_barrier_init( NULL, NULL, 1 );
  rtems_test_assert( eno == EINVAL );

  eno = pthread_barrier_wait( NULL );
  rtems_test_assert( eno == EINVAL );

  eno = pthread_barrier_destroy( NULL );
  rtems_test_assert( eno == EINVAL );
}

static void test_barrier_not_initialized( void )
{
  pthread_barrier_t bar;
  int eno;

  memset( &bar, 0xff, sizeof( bar ) );

  eno = pthread_barrier_wait(& bar) ;
  rtems_test_assert( eno == EINVAL );

  eno = pthread_barrier_destroy( &bar );
  rtems_test_assert( eno == EINVAL );
}

static void test_barrier_invalid_copy( void )
{
  pthread_barrier_t bar;
  pthread_barrier_t bar2;
  int eno;

  eno = pthread_barrier_init( &bar, NULL, 1 );
  rtems_test_assert( eno == 0 );

  memcpy( &bar2, &bar, sizeof( bar2 ) );

  eno = pthread_barrier_wait( &bar2 );
  rtems_test_assert( eno == EINVAL );

  eno = pthread_barrier_destroy( &bar2 );
  rtems_test_assert( eno == EINVAL );

  eno = pthread_barrier_destroy( &bar );
  rtems_test_assert( eno == 0 );
}

#define NUMBER_THREADS 2
pthread_t ThreadIds[NUMBER_THREADS];
pthread_barrier_t Barrier;
void *BarrierThread(void *arg);

void *BarrierThread(void *arg)
{
  pthread_t id = *(pthread_t *) arg;
  int       status;

  printf( "pthread_barrier_wait( &Barrier ) for thread 0x%08" PRIxpthread_t "\n", id );
  status = pthread_barrier_wait( &Barrier );
  printf( "pthread_barrier_wait - 0x%08" PRIxpthread_t " released\n", id );
  rtems_test_assert( (status == 0) || (status == PTHREAD_BARRIER_SERIAL_THREAD) );

  return NULL;
}

/*
 *  main entry point to the test
 */

#if defined(__rtems__)
int test_main(void);

int test_main(void)
#else
int main(
  int    argc,
  char **argv
)
#endif
{
  pthread_barrier_t    *bad_barrier = NULL;
  pthread_barrier_t     barrier;
  pthread_barrierattr_t attr;
  int                   status;
  int                   p;
  int                   i;

  TEST_BEGIN();

  /*************** NULL POINTER CHECKS *****************/
  puts( "pthread_barrierattr_init( NULL ) -- EINVAL" );
  status = pthread_barrierattr_init( NULL );
  rtems_test_assert( status == EINVAL );

  puts( "pthread_barrierattr_setpshared( NULL, private ) -- EINVAL" );
  status = pthread_barrierattr_setpshared( NULL, PTHREAD_PROCESS_PRIVATE );
  rtems_test_assert( status == EINVAL );

  puts( "pthread_barrierattr_setpshared( NULL, shared ) -- EINVAL" );
  status = pthread_barrierattr_setpshared( NULL, PTHREAD_PROCESS_SHARED );
  rtems_test_assert( status == EINVAL );

  puts( "pthread_barrierattr_getpshared( NULL, &p ) -- EINVAL" );
  status = pthread_barrierattr_getpshared( NULL, &p );
  rtems_test_assert( status == EINVAL );

  puts( "pthread_barrierattr_destroy( NULL ) -- EINVAL" );
  status = pthread_barrierattr_destroy( NULL );
  rtems_test_assert( status == EINVAL );

  /*************** NOT INITIALIZED CHECKS *****************/
  /* cheat visibility */
  attr.is_initialized = 0;
  puts( "pthread_barrierattr_setpshared( &attr, shared ) -- EINVAL" );
  status = pthread_barrierattr_setpshared( &attr, PTHREAD_PROCESS_SHARED );
  rtems_test_assert( status == EINVAL );

  puts( "pthread_barrierattr_getpshared( &attr, NULL ) -- EINVAL" );
  status = pthread_barrierattr_getpshared( &attr, NULL );
  rtems_test_assert( status == EINVAL );

  puts( "pthread_barrierattr_destroy( &attr ) -- EINVAL" );
  status = pthread_barrierattr_destroy( &attr );
  rtems_test_assert( status == EINVAL );


  /*************** ACTUALLY WORK THIS TIME *****************/

  puts( "pthread_barrierattr_init( &attr ) -- OK" );
  status = pthread_barrierattr_init( &attr );
  rtems_test_assert( status == 0 );

  puts( "pthread_barrierattr_setpshared( &attr, private ) -- OK" );
  status = pthread_barrierattr_setpshared( &attr, PTHREAD_PROCESS_PRIVATE );
  rtems_test_assert( status == 0 );

  puts( "pthread_barrierattr_getpshared( &attr, &p ) -- OK" );
  status = pthread_barrierattr_getpshared( &attr, &p );
  rtems_test_assert( status == 0 );
  rtems_test_assert( p == PTHREAD_PROCESS_PRIVATE );

  puts( "pthread_barrierattr_setpshared( &attr, shared ) -- OK" );
  status = pthread_barrierattr_setpshared( &attr, PTHREAD_PROCESS_SHARED );
  rtems_test_assert( status == 0 );

  puts( "pthread_barrierattr_getpshared( &attr, &p ) -- OK" );
  status = pthread_barrierattr_getpshared( &attr, &p );
  rtems_test_assert( status == 0 );
  rtems_test_assert( p == PTHREAD_PROCESS_SHARED );

  /*************** BAD PSHARED CHECK *****************/
  puts( "pthread_barrierattr_setpshared( &attr, private ) -- EINVAL" );
  status = pthread_barrierattr_setpshared( &attr, ~PTHREAD_PROCESS_PRIVATE );
  rtems_test_assert( status == EINVAL );

  /*************** DESTROY/REUSE CHECK *****************/
  puts( "pthread_barrierattr_destroy( &attr ) -- OK" );
  status = pthread_barrierattr_destroy( &attr );
  rtems_test_assert( status == 0 );

  puts( "pthread_barrierattr_getpshared( &attr, &p ) destroyed -- EINVAL" );
  status = pthread_barrierattr_getpshared( &attr, &p );
  rtems_test_assert( status == EINVAL );

  /*************** pthread_barrier_init ERROR CHECKs *********/
  /* NULL barrier argument */
  puts( "pthread_barrier_init( NULL, NULL, 2 ) -- EINVAL" );
  status = pthread_barrier_init( NULL, NULL, 2 );
  rtems_test_assert( status == EINVAL );

  /* uninitialized attr argument */
  puts( "pthread_barrier_init( &barrier, &attr, 2 ) -- EINVAL" );
  status = pthread_barrier_init( &barrier, &attr, 2 );
  rtems_test_assert( status == EINVAL );

  /* zero count argument */
  puts( "pthread_barrierattr_init( &attr ) -- OK" );
  status = pthread_barrierattr_init( &attr );
  rtems_test_assert( status == 0 );

  puts( "pthread_barrier_init( &barrier, &attr, 0 ) -- EINVAL" );
  status = pthread_barrier_init( &barrier, &attr, 0 );
  rtems_test_assert( status == EINVAL );

  puts( "pthread_barrier_init( &barrier, &attr, 1 ) -- EINVAL" );
  attr.process_shared = -1;
  status = pthread_barrier_init( &barrier, &attr, 1 );
  rtems_test_assert( status == EINVAL );

  puts( "pthread_barrierattr_setpshared( &attr, shared ) -- OK" );
  status = pthread_barrierattr_setpshared( &attr, PTHREAD_PROCESS_SHARED );
  rtems_test_assert( status == 0 );

  puts( "pthread_barrier_init( &barrier, &attr, 1 ) -- OK" );
  status = pthread_barrier_init( &barrier, &attr, 1 );
  rtems_test_assert( status == 0 );

  puts( "pthread_barrier_destroy( &barrier ) -- OK" );
  status = pthread_barrier_destroy( &barrier );
  rtems_test_assert( status == 0 );

  puts( "pthread_barrierattr_destroy( &attr ) -- OK" );
  status = pthread_barrierattr_destroy( &attr );
  rtems_test_assert( status == 0 );

  /*************** pthread_barrier_destroy ERROR CHECKs *********/
  /* NULL barrier argument */
  puts( "pthread_barrier_destroy( NULL ) -- EINVAL" );
  status = pthread_barrier_destroy( NULL );
  rtems_test_assert( status == EINVAL );

  puts( "pthread_barrier_destroy( bad_barrier ) -- EINVAL" );
  status = pthread_barrier_destroy( bad_barrier );
  rtems_test_assert( status == EINVAL );

  /*************** pthread_barrier_wait ERROR CHECKs *********/
  /* NULL barrier argument */
  puts( "pthread_barrier_wait( NULL ) -- EINVAL" );
  status = pthread_barrier_wait( NULL );
  rtems_test_assert( status == EINVAL );

  puts( "pthread_barrier_wait( bad_barrier ) -- EINVAL" );
  status = pthread_barrier_wait( bad_barrier );
  rtems_test_assert( status == EINVAL );

  /*************** ACTUALLY CREATE ONE CHECK *****************/
  puts( "pthread_barrierattr_init( &attr ) -- OK" );
  status = pthread_barrierattr_init( &attr );
  rtems_test_assert( status == 0 );

  puts( "pthread_barrier_init( &barrier, &attr, 2 ) -- OK" );
  status = pthread_barrier_init( &barrier, &attr, 2 );
  rtems_test_assert( status == 0 );

  puts( "pthread_barrier_destroy( &barrier ) -- OK" );
  status = pthread_barrier_destroy( &barrier );
  rtems_test_assert( status == 0 );

  /*************** CREATE THREADS AND LET THEM RELEASE *****************/
  puts( "pthread_barrier_init( &Barrier, &attr, NUMBER_THREADS ) -- OK" );
  status = pthread_barrier_init( &Barrier, &attr, NUMBER_THREADS );
  rtems_test_assert( status == 0 );

  for (i=0 ; i<NUMBER_THREADS ; i++ ) {

    /* check for unable to destroy while threads waiting */
    if (i == NUMBER_THREADS - 1) {
      puts( "pthread_barrier_destroy( &Barrier ) -- EBUSY" );
      status = pthread_barrier_destroy( &Barrier );
      rtems_test_assert( status == EBUSY );
    }

    /* create a thread to block on the barrier */
    printf( "Init: pthread_create - thread %d OK\n", i+1 );
    status = pthread_create(&ThreadIds[i], NULL, BarrierThread, &ThreadIds[i]);
    rtems_test_assert( !status );

    sleep(1);
  }

  test_barrier_null();
  test_barrier_not_initialized();
  test_barrier_invalid_copy();

  /*************** END OF TEST *****************/
  TEST_END();
  exit(0);
}
