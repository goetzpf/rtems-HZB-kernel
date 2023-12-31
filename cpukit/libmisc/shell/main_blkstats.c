/*
 * Copyright (c) 2012 embedded brains GmbH & Co. KG
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rtems.org/license/LICENSE.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems/blkdev.h>
#include <rtems/printer.h>
#include <rtems/shellconfig.h>

#include <string.h>

static bool is_reset_option(const char *opt)
{
  return strcmp(opt, "-r") == 0 || strcmp(opt, "--reset") == 0;
}

static int rtems_shell_main_blkstats(int argc, char **argv)
{
  bool ok = false;
  bool reset = false;
  const char *device;
  rtems_printer printer;

  if (argc == 2) {
    ok = true;
    device = argv [1];
  } else if (argc == 3 && is_reset_option(argv [1])) {
    ok = true;
    reset = true;
    device = argv [2];
  }

  rtems_print_printer_printf(&printer);

  if (ok) {
    rtems_blkstats(&printer, device, reset);
  } else {
    rtems_printf(&printer, "usage: %s\n", rtems_shell_BLKSTATS_Command.usage);
  }

  return 0;
}

rtems_shell_cmd_t rtems_shell_BLKSTATS_Command = {
  .name = "blkstats",
  .usage = "blkstats [-r|--reset] PATH_TO_DEVICE",
  .topic = "files",
  .command = rtems_shell_main_blkstats
};
