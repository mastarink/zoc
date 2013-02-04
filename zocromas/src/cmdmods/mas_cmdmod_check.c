#include "mas_basic_def.h"

#include <stdio.h>
#include <unistd.h>

#include <mastar/wrap/mas_memory.h>

#include "mas_common.h"

#include "variables/inc/mas_thread_variables.h"

#include "modules/inc/mas_modules_commands_eval.h"
#include "modules/inc/mas_modules_commands.h"


/*
this:
  mas_cmdmod_check.c
related:
  mas_cmdmod_top.c
  mas_cmdmod_get.c
  mas_cmdmod_client.c
  mas_cmdmod_mem.c
  mas_cmdmod_set.c
  mas_modules_commands.c
  mas_modules_commands_eval.c
*/




char *
version_cmd( STD_CMD_ARGS )
{
  return NULL;
}

char *
args_cmd( STD_CMD_ARGS )
{
  char *result = NULL;
  mas_variable_t *var;

  var = mas_thread_variables_find( "client", "args" );
  if ( var )
  {
    /* cMSG( "CHECK ARGS: %s ::: %s", args, var->value ); */
    result = mas_strdup( var->value );
  }
  else if ( args )
  {
    /* cMSG( "CHECK ARGS: %s", args ); */
    mas_thread_variable_set_text( MAS_THREAD_TRANSACTION, "client", "args", args );
    result = mas_strdup( args );
  }
  return result;
}

char *
uuid_cmd( STD_CMD_ARGS )
{
  char *uuid = NULL;

  /* if ( prcontrol ) */
  {
    /* mas_lcontrol_t *plcontrol;       */
    /*                                   */
    /* plcontrol = prcontrol->plcontrol; */

    if ( prcontrol )
    {
      if ( !prcontrol->uuid && args )
      {
        prcontrol->uuid = mas_strdup( args );
      }
      if ( prcontrol->uuid )
        uuid = mas_strdup( prcontrol->uuid );
    }
    /* if ( plcontrol )                                                              */
    /* {                                                                             */
    /*   if ( args )                                                                 */
    /*   {                                                                           */
    /*     cMSG( "CHECK UUID: %s", args );                                           */
    /*     mas_lcontrol_variable_set_text( plcontrol, MAS_THREAD_LISTENER, uuid,  ); */
    /*   }                                                                           */
    /*   else                                                                        */
    /*   {                                                                           */
    /*     var = mas_lcontrol_variables_find( plcontrol, "client", uuid );           */
    /*     if ( var )                                                                */
    /*     {                                                                         */
    /*       cMSG( "CHECK UUID: %s ::: %s", args, var->value );                      */
    /*     }                                                                         */
    /*   }                                                                           */
    /* }                                                                             */
  }
  return uuid;
}


mas_cmd_t subcmdtable[] = {
  {0, NULL, list_commands_cmd, NULL}
  ,
  {1, "version", version_cmd, NULL}
  ,
  {2, "args", args_cmd, NULL}
  ,
  {3, "uuid", uuid_cmd, NULL}
  ,
  {999, NULL, NULL, NULL}
};
