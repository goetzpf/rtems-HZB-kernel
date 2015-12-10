/**
 * @file
 *
 * @brief Initialize Extension Manager
 *
 * @ingroup ClassicUserExtensions
 */

/*
 *  Extension Manager
 *
 *  COPYRIGHT (c) 1989-2008.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.org/license/LICENSE.
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems/extensionimpl.h>
#include <rtems/config.h>
#include <rtems/sysinit.h>

Objects_Information _Extension_Information;

static void _Extension_Manager_initialization(void)
{
  _Objects_Initialize_information(
    &_Extension_Information,
    OBJECTS_CLASSIC_API,                 /* object API */
    OBJECTS_RTEMS_EXTENSIONS,
    rtems_configuration_get_maximum_extensions(),
    sizeof( Extension_Control ),
    false,                     /* true if the name is a string */
    RTEMS_MAXIMUM_NAME_LENGTH  /* maximum length of an object name */
#if defined(RTEMS_MULTIPROCESSING)
    ,
    false,                     /* true if this is a global object class */
    NULL                       /* Proxy extraction support callout */
#endif
  );
}

RTEMS_SYSINIT_ITEM(
  _Extension_Manager_initialization,
  RTEMS_SYSINIT_USER_EXTENSIONS,
  RTEMS_SYSINIT_ORDER_MIDDLE
);
