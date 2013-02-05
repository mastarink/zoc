#include "mas_basic_def.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <readline/readline.h>
#include <readline/history.h>

#include <mastar/wrap/mas_memory.h>

#include "mas_common.h"

#include "modules/inc/mas_modules_commands_eval.h"
#include "modules/inc/mas_modules_commands.h"

/*
this:
  mas_cmdmod_readline.c
related:
  mas_cmdmod_get.c
  mas_cmdmod_mem.c
  mas_cmdmod_client.c
  mas_cmdmod_server.c
  mas_cmdmod_set.c
  mas_cmdmod_root.c
  mas_modules_commands.c
  mas_modules_commands_eval.c
*/


static char *
readline_cmd( STD_CMD_ARGS )
{
  char *buf;
  char *mas_readline_buffer;

  ctrl.ticker_hide = 1;
  mas_readline_buffer = readline( "\nserver> " );
  ctrl.ticker_hide = 0;
  buf = mas_strdup( mas_readline_buffer );
  mas_other_free( mas_readline_buffer );
  mas_readline_buffer = NULL;

  return buf;
}

mas_cmd_t subcmdtable[] = {
  {1, NULL, readline_cmd, NULL} /* get readline */
  ,
  {999, NULL, NULL, NULL}
};
