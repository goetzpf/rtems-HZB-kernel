/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup RTEMSImplClassicMessageMP
 *
 * @brief This header file provides the implementation interfaces of the
 *   @ref RTEMSImplClassicMessageMP.
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

#ifndef _RTEMS_RTEMS_MSGMP_H
#define _RTEMS_RTEMS_MSGMP_H

#ifndef _RTEMS_RTEMS_MESSAGEIMPL_H
# error "Never use <rtems/rtems/msgmp.h> directly; include <rtems/rtems/messageimpl.h> instead."
#endif

#include <rtems/score/mpciimpl.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup RTEMSImplClassicMessageMP \
 *   Message Manager Multiprocessing (MP) Support
 *
 * @ingroup RTEMSImplClassicMessage
 *
 * @brief This group contains the implementation to support the Message Manager
 *   in multiprocessing (MP) configurations.
 *
 * @{
 */

/**
 *  The following enumerated type defines the list of
 *  remote message queue operations.
 */
typedef enum {
  MESSAGE_QUEUE_MP_ANNOUNCE_CREATE             =  0,
  MESSAGE_QUEUE_MP_ANNOUNCE_DELETE             =  1,
  MESSAGE_QUEUE_MP_EXTRACT_PROXY               =  2,
  MESSAGE_QUEUE_MP_RECEIVE_REQUEST             =  3,
  MESSAGE_QUEUE_MP_RECEIVE_RESPONSE            =  4,
  MESSAGE_QUEUE_MP_SEND_REQUEST                =  5,
  MESSAGE_QUEUE_MP_SEND_RESPONSE               =  6,
  MESSAGE_QUEUE_MP_URGENT_REQUEST              =  7,
  MESSAGE_QUEUE_MP_URGENT_RESPONSE             =  8,
  MESSAGE_QUEUE_MP_BROADCAST_REQUEST           =  9,
  MESSAGE_QUEUE_MP_BROADCAST_RESPONSE          = 10,
  MESSAGE_QUEUE_MP_FLUSH_REQUEST               = 11,
  MESSAGE_QUEUE_MP_FLUSH_RESPONSE              = 12,
  MESSAGE_QUEUE_MP_GET_NUMBER_PENDING_REQUEST  = 13,
  MESSAGE_QUEUE_MP_GET_NUMBER_PENDING_RESPONSE = 14
}   Message_queue_MP_Remote_operations;

/**
 *  The following data structure defines the packet used to perform
 *  remote message queue operations.
 */
typedef struct {
  rtems_packet_prefix                Prefix;
  Message_queue_MP_Remote_operations operation;
  rtems_name                         name;
  rtems_option                       option_set;
  Objects_Id                         proxy_id;
  uint32_t                           count;
  uint32_t                           size;
  uint32_t                           buffer[ RTEMS_ZERO_LENGTH_ARRAY ];
}   Message_queue_MP_Packet;

#define MESSAGE_QUEUE_MP_PACKET_SIZE \
  offsetof(Message_queue_MP_Packet, buffer)

static inline bool _Message_queue_MP_Is_remote( Objects_Id id )
{
  return _Objects_MP_Is_remote( id, &_Message_queue_Information );
}

/**
 *  @brief Message_queue_Core_message_queue_mp_support
 *
 *  Input parameters:
 *    the_thread - the remote thread the message was submitted to
 *    id         - id of the message queue
 *
 *  Output parameters: NONE
 */
void  _Message_queue_Core_message_queue_mp_support (
  Thread_Control *the_thread,
  rtems_id        id
);

/**
 *  @brief _Message_queue_MP_Send_process_packet
 *
 *  This routine performs a remote procedure call so that a
 *  process operation can be performed on another node.
 */
void _Message_queue_MP_Send_process_packet (
  Message_queue_MP_Remote_operations operation,
  Objects_Id                         message_queue_id,
  rtems_name                         name,
  Objects_Id                         proxy_id
);

/**
 * @brief Issues a remote rtems_message_queue_broadcast() request.
 */
rtems_status_code _Message_queue_MP_Broadcast(
  rtems_id    id,
  const void *buffer,
  size_t      size,
  uint32_t   *count
);

/**
 * @brief Issues a remote rtems_message_queue_flush() request.
 */
rtems_status_code _Message_queue_MP_Flush(
  rtems_id  id,
  uint32_t *count
);

/**
 * @brief Issues a remote rtems_message_queue_get_number_pending() request.
 */
rtems_status_code _Message_queue_MP_Get_number_pending(
  rtems_id  id,
  uint32_t *count
);

/**
 * @brief Issues a remote rtems_message_queue_receive() request.
 */
rtems_status_code _Message_queue_MP_Receive(
  rtems_id        id,
  void           *buffer,
  size_t         *size,
  rtems_option    option_set,
  rtems_interval  timeout
);

/**
 * @brief Issues a remote rtems_message_queue_send() request.
 */
rtems_status_code _Message_queue_MP_Send(
  rtems_id    id,
  const void *buffer,
  size_t      size
);

/**
 * @brief Issues a remote rtems_message_queue_urgent() request.
 */
rtems_status_code _Message_queue_MP_Urgent(
  rtems_id    id,
  const void *buffer,
  size_t      size
);

/**
 *  @brief _Message_queue_MP_Send_object_was_deleted
 *
 *  This routine is invoked indirectly by the thread queue
 *  when a proxy has been removed from the thread queue and
 *  the remote node must be informed of this.
 */
void _Message_queue_MP_Send_object_was_deleted (
  Thread_Control *the_proxy,
  Objects_Id      mp_id
);

/**@}*/

#ifdef __cplusplus
}
#endif

#endif
/* end of file */
