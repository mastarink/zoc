#include <mastar/wrap/mas_std_def.h>

#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>

#include <signal.h>
#include <dlfcn.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_arg_tools.h>

#include <mastar/types/mas_control_types.h>
#include <mastar/types/mas_opts_types.h>
extern mas_control_t ctrl;
extern mas_options_t opts;

#include <mastar/msg/mas_msg_def.h>
#include <mastar/msg/mas_msg_tools.h>
#include <mastar/log/mas_log.h>


#include "mas_modules_commands_eval.h"
#include "mas_modules_commands.h"

/*
this:
  mas_modules_commands.c
related:
  mas_modules_commands.h
  mas_modules_commands_eval.c
  mas_modules_load_module.c
*/



static mas_cmd_t *
mas_modules_lookup_question( const char *ownerlib, mas_cmd_t * cmdtable, const char *question, const char **pargs )
{
  const char *args = NULL;
  const char *q = NULL;
  size_t len;
  mas_cmd_t *found = NULL;

  if ( question )
  {
    int quo = 0;

    args = mas_find_next_arg( question, &q, &len, &quo );
  }
  MAS_LOG( "looking for q:'%s' args:'%s'", q, args );
  {
    mas_cmd_t *cmd = NULL;

    cmd = cmdtable;
    WMSG( "LOOKUP : '%s'", question );
    while ( cmd && !found && ( cmd->name || cmd->function || cmd->libname /* || cp->subtable */  ) )
    {
      size_t l = 0;

      /* HMSG( "TEST %s.%s for %s", ownerlib, cmd->name, question ); */
      if ( cmd->name )
        l = strlen( cmd->name );
#if 0
      if ( ( !cmd->name && !*question ) || ( cmd->name && 0 == strncmp( cmd->name, question, ( l = strlen( cmd->name ) ) ) && l == len ) )
#else
      if ( !( cmd->name || question ) || ( cmd->name && question && 0 == strncmp( cmd->name, question, l ) && l == len ) )
#endif
      {
        MAS_LOG( "located #%d: cmd '%s'", cmd->id, question );
        HMSG( "located #%d: cmd '%s'", cmd->id, question );
        found = cmd;
        if ( pargs )
          ( *pargs ) = args;
      }
      cmd++;
    }
  }
  MAS_LOG( "cmd %s : %s (%s)", question, found ? "FOUND" : "NOT found", found ? found->name : "" );
  tMSG( "cmd %s : %s (%s)", question, found ? "FOUND" : "NOT found", found ? found->name : "" );
  if ( found )
  {
    WMSG( "FOUND %s.%s", ownerlib, found->name );
  }
  else
  {
    WMSG( "NOT FOUND %s.%s", ownerlib, question );
  }
  return found;
}


char *
mas_modules_commands( STD_CMD_ARGS )
{
  char *answer = NULL;
  static mas_cmd_t def_cmd = { 1, "unknown", NULL, NULL, 1 };
  mas_cmd_t *found = NULL;

  if ( this_command )
  {
    WMSG( "LOOKUP ... from %s.%s sub", "...", this_command->name );
    found = mas_modules_lookup_question( this_command->libname, this_command->subtable, question, &args );
    MAS_LOG( "(L%u) command %s (%s) %s [%s]", level, found ? found->name : NULL, question, found ? "FOUND" : "NOT FOUND", args );
    if ( !found )
    {
      found = mas_modules_lookup_question( this_command->libname, this_command->subtable, "*", &args );
      if ( found )
        args = question;
    }
    MAS_LOG( "(L%u) command %s (%s) %s [%s]", level, found ? found->name : NULL, "*", found ? "FOUND" : "NOT FOUND", args );
    if ( !found )
    {
      found = &def_cmd;
      MAS_LOG( "setting def_cmd %p : %s", ( void * ) found, found ? found->name : NULL );
    }
  }
  else if ( !level )
  {
    MAS_LOG( "empty command; level %d", level );
    if ( prcontrol )
      prcontrol->qbin = MSG_BIN_EMPTY_COMMAND;
  }

  if ( found )
  {
    if ( found->unknown )
    {
      if ( prcontrol )
        prcontrol->qbin = MSG_BIN_UNKNOWN_COMMAND;
      EMSG( "NOT found cmd '%s'", question );
      MAS_LOG( "NOT found cmd '%s'", question );
      answer = mas_strdup( question );
    }
    else if ( found->only_level && found->only_level != level )
    {
      if ( prcontrol )
        prcontrol->qbin = MSG_BIN_UNKNOWN_COMMAND;
      EMSG( "NOT found @ level %u / %u cmd '%s'", level, found->only_level, question );
      MAS_LOG( "NOT found @ level %u / %u cmd '%s'", level, found->only_level, question );
      answer = mas_strdup( question );
    }
    else
    {
      tMSG( "evaluating %s ( %s )", question, args );
      MAS_LOG( "evaluating %s ( %s )", question, args );
      WMSG( "EVAL FOUND %s.%s", this_command->libname, found->name );
      answer = mas_evaluate_cmd( 0, this_command->subtable, found, prcontrol, question, args, level + 1 );
    }
  }
  else
  {
    if ( prcontrol )
      prcontrol->qbin = MSG_BIN_UNKNOWN_COMMAND;
    MAS_LOG( "NO question, no answer" );
    EMSG( "NOT FOUND; question:'%s'", question );
  }
  return answer;
}
