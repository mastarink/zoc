#include <mastar/wrap/mas_std_def.h>
#include <mastar/types/mas_common_defs.h>


#include <stdlib.h>

#include <mastar/wrap/mas_memory.h>


#include <mastar/fileinfo/mas_unidata.h>

#include <mastar/modules/mas_modules_commands_eval.h>
#include <mastar/modules/mas_modules_commands.h>


/*
this:
  mas_cmdmod_show.c
related:
  mas_cmdmod_get.c
  mas_cmdmod_client.c
  mas_cmdmod_mem.c
  mas_cmdmod_set.c
  mas_cmdmod_root.c
  mas_modules_commands.c
  mas_modules_commands_eval.c
*/



static mas_evaluated_t *
msg_cmd( STD_CMD_ARGS )
{
  char *result = NULL;

  result = mas_strdup( "smth" );
  return mas_evaluated_wrap_pchar( result );
}

mas_cmd_t subcmdtable[] = {
  {0, NULL, mas_evaluate_list_cmd, NULL}
  ,
  {1, "msg", msg_cmd, "show"}   /* show msg ... */
  ,
  {999, NULL, NULL, NULL}
};
