/* SPDX-License-Identifier: BSD-2-Clause */

/*
 * Copyright (c) 2013 embedded brains GmbH & Co. KG
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

#include "tmacros.h"

#include <stdio.h>
#include <inttypes.h>

const char rtems_test_name[] = "SMPSWITCHEXTENSION 1";

#define CPU_COUNT 2

#define TOGGLER_COUNT 2

#define EXTENSION_COUNT 3

#define PRIO_STOP 2

#define PRIO_SWITCH 3

#define PRIO_HIGH 4

#define PRIO_NORMAL 5

#define PRIO_LOW 6

typedef struct {
  uint32_t toggles;
  uint32_t unused_space_for_cache_line_alignment[7];
} test_toggler_counters;

typedef struct {
  test_toggler_counters counters[TOGGLER_COUNT];
  rtems_id toggler_ids[TOGGLER_COUNT];
  rtems_id extension_ids[EXTENSION_COUNT];
  uint32_t extension_switches;
  uint32_t context_switches[EXTENSION_COUNT];
} test_context;

CPU_STRUCTURE_ALIGNMENT static test_context ctx_instance;

static void switch_0(Thread_Control *executing, Thread_Control *heir)
{
  test_context *ctx = &ctx_instance;

  (void) executing;
  (void) heir;

  ++ctx->context_switches[0];
}

static void switch_1(Thread_Control *executing, Thread_Control *heir)
{
  test_context *ctx = &ctx_instance;

  (void) executing;
  (void) heir;

  ++ctx->context_switches[1];
}

static void switch_2(Thread_Control *executing, Thread_Control *heir)
{
  test_context *ctx = &ctx_instance;

  (void) executing;
  (void) heir;

  ++ctx->context_switches[2];
}

static const rtems_extensions_table extensions[EXTENSION_COUNT] = {
  { .thread_switch = switch_0 },
  { .thread_switch = switch_1 },
  { .thread_switch = switch_2 }
};

static void toggler(rtems_task_argument self)
{
  test_context *ctx = &ctx_instance;
  test_toggler_counters *counters = &ctx->counters[self];

  while (true) {
    rtems_status_code sc;

    ++counters->toggles;

    sc = rtems_task_wake_after(RTEMS_YIELD_PROCESSOR);
    rtems_test_assert(sc == RTEMS_SUCCESSFUL);
  }
}

static void switcher(rtems_task_argument self)
{
  test_context *ctx = &ctx_instance;

  while (true) {
    size_t ext_index;
    rtems_status_code sc;

    ++ctx->extension_switches;

    for (ext_index = 0; ext_index < EXTENSION_COUNT; ++ext_index) {
      sc = rtems_extension_create(
        rtems_build_name('E', 'X', 'T', (char) ('0' + ext_index)),
        &extensions[ext_index],
        &ctx->extension_ids[ext_index]
      );
      rtems_test_assert(sc == RTEMS_SUCCESSFUL);
    }

    for (ext_index = 0; ext_index < EXTENSION_COUNT; ++ext_index) {
      sc = rtems_extension_delete(ctx->extension_ids[ext_index]);
      rtems_test_assert(sc == RTEMS_SUCCESSFUL);
    }
  }
}

static void stopper(rtems_task_argument arg)
{
  (void) arg;

  while (true) {
    /* Do nothing */
  }
}

static void test(void)
{
  test_context *ctx = &ctx_instance;
  rtems_status_code sc;
  rtems_task_argument toggler_index;
  rtems_id stopper_id;
  rtems_id switcher_id;
  size_t ext_index;

  sc = rtems_task_create(
    rtems_build_name('S', 'T', 'O', 'P'),
    PRIO_STOP,
    RTEMS_MINIMUM_STACK_SIZE,
    RTEMS_DEFAULT_MODES,
    RTEMS_DEFAULT_ATTRIBUTES,
    &stopper_id
  );
  rtems_test_assert(sc == RTEMS_SUCCESSFUL);

  sc = rtems_task_create(
    rtems_build_name('E', 'X', 'S', 'W'),
    PRIO_SWITCH,
    RTEMS_MINIMUM_STACK_SIZE,
    RTEMS_DEFAULT_MODES,
    RTEMS_DEFAULT_ATTRIBUTES,
    &switcher_id
  );
  rtems_test_assert(sc == RTEMS_SUCCESSFUL);

  for (toggler_index = 0; toggler_index < TOGGLER_COUNT; ++toggler_index) {
    sc = rtems_task_create(
      rtems_build_name('T', 'O', 'G', (char) ('0' + toggler_index)),
      PRIO_NORMAL,
      RTEMS_MINIMUM_STACK_SIZE,
      RTEMS_DEFAULT_MODES,
      RTEMS_DEFAULT_ATTRIBUTES,
      &ctx->toggler_ids[toggler_index]
    );
    rtems_test_assert(sc == RTEMS_SUCCESSFUL);
  }

  for (toggler_index = 0; toggler_index < TOGGLER_COUNT; ++toggler_index) {
    sc = rtems_task_start(ctx->toggler_ids[toggler_index], toggler, toggler_index);
    rtems_test_assert(sc == RTEMS_SUCCESSFUL);
  }

  sc = rtems_task_start(switcher_id, switcher, 0);
  rtems_test_assert(sc == RTEMS_SUCCESSFUL);

  sc = rtems_task_wake_after(10 * rtems_clock_get_ticks_per_second());
  rtems_test_assert(sc == RTEMS_SUCCESSFUL);

  sc = rtems_task_start(stopper_id, stopper, 0);
  rtems_test_assert(sc == RTEMS_SUCCESSFUL);

  for (toggler_index = 0; toggler_index < TOGGLER_COUNT; ++toggler_index) {
    test_toggler_counters *counters = &ctx->counters[toggler_index];

    printf(
      "toggler %" PRIuPTR "\n"
        "\ttoggles %" PRIu32 "\n",
      toggler_index,
      counters->toggles
    );
  }

  for (ext_index = 0; ext_index < EXTENSION_COUNT; ++ext_index) {
    printf(
      "extension %" PRIuPTR "\n"
        "\tcontext switches %" PRIu32 "\n",
      ext_index,
      ctx->context_switches[ext_index]
    );
  }

  printf(
    "extension switches %" PRIu32 "\n",
    ctx->extension_switches
  );
}

static void Init(rtems_task_argument arg)
{
  TEST_BEGIN();

  if (rtems_scheduler_get_processor_maximum() >= 2) {
    test();
  }

  TEST_END();
  rtems_test_exit(0);
}

#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_SIMPLE_CONSOLE_DRIVER

#define CONFIGURE_MAXIMUM_PROCESSORS CPU_COUNT

#define CONFIGURE_MAXIMUM_TASKS (3 + TOGGLER_COUNT)

#define CONFIGURE_MAXIMUM_USER_EXTENSIONS EXTENSION_COUNT

#define CONFIGURE_INITIAL_EXTENSIONS RTEMS_TEST_INITIAL_EXTENSION

#define CONFIGURE_RTEMS_INIT_TASKS_TABLE

#define CONFIGURE_INIT

#include <rtems/confdefs.h>
