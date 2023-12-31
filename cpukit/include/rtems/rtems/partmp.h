/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup RTEMSImplClassicPartitionMP
 *
 * @brief This header file provides the implementation interfaces of the
 *   @ref RTEMSImplClassicPartitionMP.
 */

/* COPYRIGHT (c) 1989-2013.
 * On-Line Applications Research Corporation (OAR).
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

#ifndef _RTEMS_RTEMS_PARTMP_H
#define _RTEMS_RTEMS_PARTMP_H

#ifndef _RTEMS_RTEMS_PARTIMPL_H
# error "Never use <rtems/rtems/partmp.h> directly; include <rtems/rtems/partimpl.h> instead."
#endif

#include <rtems/score/mpciimpl.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup RTEMSImplClassicPartitionMP \
 *   Partition Manager Multiprocessing (MP) Support
 *
 * @ingroup RTEMSImplClassicPartition
 *
 * @brief This group contains the implementation to support the Partition Manager
 *   in multiprocessing (MP) configurations.
 *
 * @{
 */

/**
 *  The following enumerated type defines the list of
 *  remote partition operations.
 */
typedef enum {
  PARTITION_MP_ANNOUNCE_CREATE        =  0,
  PARTITION_MP_ANNOUNCE_DELETE        =  1,
  PARTITION_MP_EXTRACT_PROXY          =  2,
  PARTITION_MP_GET_BUFFER_REQUEST     =  3,
  PARTITION_MP_GET_BUFFER_RESPONSE    =  4,
  PARTITION_MP_RETURN_BUFFER_REQUEST  =  5,
  PARTITION_MP_RETURN_BUFFER_RESPONSE =  6
}   Partition_MP_Remote_operations;

/**
 *  The following data structure defines the packet used to perform
 *  remote partition operations.
 */
typedef struct {
  rtems_packet_prefix             Prefix;
  Partition_MP_Remote_operations  operation;
  rtems_name                      name;
  void                           *buffer;
  Objects_Id                      proxy_id;
}   Partition_MP_Packet;

static inline bool _Partition_MP_Is_remote( Objects_Id id )
{
  return _Objects_MP_Is_remote( id, &_Partition_Information );
}

/**
 *  @brief Partition_MP_Send_process_packet
 *
 *  Multiprocessing Support for the Partition Manager
 *
 *  This routine performs a remote procedure call so that a
 *  process operation can be performed on another node.
 */
void _Partition_MP_Send_process_packet (
  Partition_MP_Remote_operations operation,
  Objects_Id                     partition_id,
  rtems_name                     name,
  Objects_Id                     proxy_id
);

/**
 * @brief Issues a remote rtems_partition_get_buffer() request.
 */
rtems_status_code _Partition_MP_Get_buffer(
  rtems_id   id,
  void     **buffer
);

/**
 * @brief Issues a remote rtems_partition_return_buffer() request.
 */
rtems_status_code _Partition_MP_Return_buffer(
  rtems_id  id,
  void     *buffer
);

/*
 *  @brief Partition_MP_Send_object_was_deleted
 *
 *  This routine is invoked indirectly by the thread queue
 *  when a proxy has been removed from the thread queue and
 *  the remote node must be informed of this.
 *
 *  This routine is not needed by the Partition since a partition
 *  cannot be deleted when buffers are in use.
 */

#ifdef __cplusplus
}
#endif

/**@}*/

#endif
/* end of file */
