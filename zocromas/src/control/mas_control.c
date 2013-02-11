#include "mas_basic_def.h"

#include <stdlib.h>
#include <pthread.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_arg_tools.h>

#include <mastar/types/mas_control_types.h>
#include <mastar/types/mas_opts_types.h>
extern mas_control_t ctrl;
extern mas_options_t opts;

/* #include "mas_common.h" */

#include "mas_control.h"

/*
this:
  mas_control.c
related:
  mas_control_data.c
  mas_common.h
  mas_tools.c
  mas_opts.c
  mas_cs.c
  mas_cs.h

more:
  mas_rcontrol_object.c
*/


/*
 * */
void
mas_ctrl_init( mas_options_t * popts )
{
  /* ctrl.is_client / ctrl.is_server set at the beginning of mas_init_client / mas_init_server */
  ctrl.in_client = 0;
  ctrl.keep_listening = 0;
  if ( ctrl.is_client )
    ctrl.in_client = 1;
  else
    ctrl.keep_listening = 1;
  ctrl.messages = !popts->nomessages;
  ctrl.daemon = !popts->nodaemon;
}

void
mas_ctrl_destroy( void )
{
  mas_del_argv( ctrl.commands_num, ctrl.commands, 0 );
  ctrl.commands_num = 0;
  ctrl.commands = NULL;
  if ( ctrl.protos )
    mas_free( ctrl.protos );
  ctrl.protos_num = 0;
  ctrl.protos = NULL;
}

void
mas_ctrl_add_command( const char *s )
{
  /* const char *se;              */
  /*                              */
  /* se = mas_find_eq_value( s ); */
  ctrl.commands_num = mas_add_argv_arg( ctrl.commands_num, &ctrl.commands, s );
}
