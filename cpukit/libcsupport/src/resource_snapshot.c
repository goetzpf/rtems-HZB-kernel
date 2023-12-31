/* SPDX-License-Identifier: BSD-2-Clause */

/*
 * Copyright (C) 2012, 2014 embedded brains GmbH & Co. KG
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

#include <rtems/libcsupport.h>

#include <string.h>

#include <rtems/libio_.h>
#include <rtems/malloc.h>
#include <rtems/score/rbtreeimpl.h>
#include <rtems/score/protectedheap.h>
#include <rtems/score/threadimpl.h>
#include <rtems/score/wkspace.h>
#include <rtems/posix/keyimpl.h>

static const struct {
  Objects_APIs api;
  uint16_t cls;
} objects_info_table[] = {
  { OBJECTS_POSIX_API, OBJECTS_POSIX_KEYS },
  { OBJECTS_CLASSIC_API, OBJECTS_RTEMS_BARRIERS },
  { OBJECTS_CLASSIC_API, OBJECTS_RTEMS_EXTENSIONS },
  { OBJECTS_CLASSIC_API, OBJECTS_RTEMS_MESSAGE_QUEUES },
  { OBJECTS_CLASSIC_API, OBJECTS_RTEMS_PARTITIONS },
  { OBJECTS_CLASSIC_API, OBJECTS_RTEMS_PERIODS },
  { OBJECTS_CLASSIC_API, OBJECTS_RTEMS_PORTS },
  { OBJECTS_CLASSIC_API, OBJECTS_RTEMS_REGIONS },
  { OBJECTS_CLASSIC_API, OBJECTS_RTEMS_SEMAPHORES },
  { OBJECTS_CLASSIC_API, OBJECTS_RTEMS_TASKS },
  { OBJECTS_CLASSIC_API, OBJECTS_RTEMS_TIMERS },
  { OBJECTS_POSIX_API, OBJECTS_POSIX_MESSAGE_QUEUES },
  { OBJECTS_POSIX_API, OBJECTS_POSIX_SEMAPHORES },
  { OBJECTS_POSIX_API, OBJECTS_POSIX_THREADS }
  #ifdef RTEMS_POSIX_API
    ,
    { OBJECTS_POSIX_API, OBJECTS_POSIX_TIMERS }
  #endif
};

static int open_files(void)
{
  int free_count = 0;
  rtems_libio_t *iop;

  rtems_libio_lock();

  iop = rtems_libio_iop_free_head;
  while (iop != NULL) {
    ++free_count;

    iop = iop->data1;
  }

  rtems_libio_unlock();

  return (int) rtems_libio_number_iops - free_count;
}

static void get_heap_info(Heap_Control *heap, Heap_Information_block *info)
{
  _Heap_Get_information(heap, info);
  memset(&info->Stats, 0, sizeof(info->Stats));
}

static POSIX_Keys_Control *get_next_key(Objects_Id *id)
{
  return (POSIX_Keys_Control *)
    _Objects_Get_next(*id, &_POSIX_Keys_Information, id);
}

static uint32_t get_active_posix_key_value_pairs(void)
{
  uint32_t count = 0;
  Objects_Id id = OBJECTS_ID_INITIAL_INDEX;
  POSIX_Keys_Control *the_key;

  while ((the_key = get_next_key(&id)) != NULL ) {
    count += _Chain_Node_count_unprotected(&the_key->Key_value_pairs);
    _Objects_Allocator_unlock();
  }

  return count;
}

void rtems_resource_snapshot_take(rtems_resource_snapshot *snapshot)
{
  uint32_t *active;
  size_t i;

  memset(snapshot, 0, sizeof(*snapshot));

  _RTEMS_Lock_allocator();

  _Thread_Kill_zombies();

  get_heap_info(RTEMS_Malloc_Heap, &snapshot->heap_info);
  get_heap_info(&_Workspace_Area, &snapshot->workspace_info);

  active = &snapshot->active_posix_keys;

  for (i = 0; i < RTEMS_ARRAY_SIZE(objects_info_table); ++i) {
    const Objects_Information *information;

    information = _Objects_Get_information(
      objects_info_table[i].api,
      objects_info_table[i].cls
    );

    if (information != NULL) {
      active[i] = _Objects_Active_count(information);
    }
  }

  _RTEMS_Unlock_allocator();

  snapshot->active_posix_key_value_pairs = get_active_posix_key_value_pairs();
  snapshot->open_files = open_files();
}

bool rtems_resource_snapshot_equal(
  const rtems_resource_snapshot *a,
  const rtems_resource_snapshot *b
)
{
  return memcmp(a, b, sizeof(*a)) == 0;
}

bool rtems_resource_snapshot_check(const rtems_resource_snapshot *snapshot)
{
  rtems_resource_snapshot now;

  rtems_resource_snapshot_take(&now);

  return rtems_resource_snapshot_equal(&now, snapshot);
}
