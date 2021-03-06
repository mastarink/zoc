#include <mastar/wrap/mas_std_def.h>
#include <mastar/types/mas_common_defs.h>

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_arg_tools.h>
#include <mastar/tools/mas_tools.h>


#include <mastar/types/mas_opts_types.h>


#include <mastar/msg/mas_msg_def.h>
#include <mastar/msg/mas_msg_tools.h>

#include "mas_cli_opts_init.h"
#include "mas_cli_opts_data.h"

#include "mas_opts.h"


/*
this:
  mas_opts.c
related:
  mas_opts_common.c
  mas_opts_types.h
  mas_opts_data.c
  mas_msg_tools.c
  mas_control.c

*/

int
mas_opts_delete( mas_options_t * popts )
{
  if ( popts )
  {
    mas_del_argv( popts->hostsv.c, popts->hostsv.v, 0 );
    popts->hostsv.c = 0;
    popts->hostsv.v = NULL;

    mas_del_argv( popts->protosv.c, popts->protosv.v, 0 );
    popts->protosv.c = 0;
    popts->protosv.v = NULL;

    mas_del_argv( popts->commandsv.c, popts->commandsv.v, 0 );
    popts->commandsv.c = 0;
    popts->commandsv.v = NULL;

    if ( popts->uuid )
      mas_free( popts->uuid );
    popts->uuid = NULL;

    if ( popts->save_user_opts_filename )
      mas_free( popts->save_user_opts_filename );
    popts->save_user_opts_filename = NULL;

    if ( popts->init_message )
      mas_free( popts->init_message );
    popts->init_message = NULL;

    if ( popts->msgfilename )
      mas_free( popts->msgfilename );
    popts->msgfilename = NULL;

    if ( popts->stderr_filename )
      mas_free( popts->stderr_filename );
    popts->stderr_filename = NULL;

    if ( popts->stdout_filename )
      mas_free( popts->stdout_filename );
    popts->stdout_filename = NULL;

    if ( popts->user )
      mas_free( popts->user );
    popts->user = NULL;

    if ( popts->group )
      mas_free( popts->group );
    popts->group = NULL;

    if ( popts->dir.mods )
      mas_free( popts->dir.mods );
    popts->dir.mods = NULL;

    if ( popts->dir.pids )
      mas_free( popts->dir.pids );
    popts->dir.pids = NULL;

    if ( popts->dir.proto )
      mas_free( popts->dir.proto );
    popts->dir.proto = NULL;

    if ( popts->dir.log )
      mas_free( popts->dir.log );
    popts->dir.log = NULL;

    if ( popts->dir.history )
      mas_free( popts->dir.history );
    popts->dir.history = NULL;

    if ( popts->dir.post )
      mas_free( popts->dir.post );
    popts->dir.post = NULL;

    if ( popts->dir.config )
      mas_free( popts->dir.config );
    popts->dir.config = NULL;
    {
      if ( popts->thname.parent_bunchm )
        mas_free( popts->thname.parent_bunchm );
      popts->thname.parent_bunchm = NULL;

      if ( popts->thname.daemon_bunchm )
        mas_free( popts->thname.daemon_bunchm );
      popts->thname.daemon_bunchm = NULL;

      if ( popts->thname.parent_bunchx )
        mas_free( popts->thname.parent_bunchx );
      popts->thname.parent_bunchx = NULL;

      if ( popts->thname.daemon_bunchx )
        mas_free( popts->thname.daemon_bunchx );
      popts->thname.daemon_bunchx = NULL;

      if ( popts->thname.parent_bunchi )
        mas_free( popts->thname.parent_bunchi );
      popts->thname.parent_bunchi = NULL;

      if ( popts->thname.daemon_bunchi )
        mas_free( popts->thname.daemon_bunchi );
      popts->thname.daemon_bunchi = NULL;

      if ( popts->thname.parent_masterm )
        mas_free( popts->thname.parent_masterm );
      popts->thname.parent_masterm = NULL;

      if ( popts->thname.daemon_masterm )
        mas_free( popts->thname.daemon_masterm );
      popts->thname.daemon_masterm = NULL;

      if ( popts->thname.parent_masterx )
        mas_free( popts->thname.parent_masterx );
      popts->thname.parent_masterx = NULL;

      if ( popts->thname.daemon_masterx )
        mas_free( popts->thname.daemon_masterx );
      popts->thname.daemon_masterx = NULL;

      if ( popts->thname.parent_masterth )
        mas_free( popts->thname.parent_masterth );
      popts->thname.parent_masterth = NULL;

      if ( popts->thname.daemon_masterth )
        mas_free( popts->thname.daemon_masterth );
      popts->thname.daemon_masterth = NULL;

      if ( popts->thname.parent_masterthx )
        mas_free( popts->thname.parent_masterthx );
      popts->thname.parent_masterthx = NULL;

      if ( popts->thname.daemon_masterthx )
        mas_free( popts->thname.daemon_masterthx );
      popts->thname.daemon_masterthx = NULL;

      if ( popts->thname.listenw )
        mas_free( popts->thname.listenw );
      popts->thname.listenw = NULL;

      if ( popts->thname.listenin )
        mas_free( popts->thname.listenin );
      popts->thname.listenin = NULL;

      if ( popts->thname.listent )
        mas_free( popts->thname.listent );
      popts->thname.listent = NULL;

      if ( popts->thname.listenm )
        mas_free( popts->thname.listenm );
      popts->thname.listenm = NULL;

      if ( popts->thname.listen_close )
        mas_free( popts->thname.listen_close );
      popts->thname.listen_close = NULL;

      if ( popts->thname.listen_exit )
        mas_free( popts->thname.listen_exit );
      popts->thname.listen_exit = NULL;
    }
    if ( popts->configfilename )
      mas_free( popts->configfilename );
    popts->configfilename = NULL;

    mas_cli_options_argv_destroy( popts );
    mas_cli_options_data_destroy( popts );
  }
  return 0;
}

int
mas_pre_init_default_opts( mas_options_t * popts )
{
#ifdef MAS_SERVER_STRING
  popts->hostsv.c = mas_add_argv_arg( popts->hostsv.c, &popts->hostsv.v, MAS_SERVER_STRING );
#endif
#ifdef MAS_SERVER_DEF_PROTO
  popts->protosv.c = mas_add_argv_arg( popts->protosv.c, &popts->protosv.v, MAS_SERVER_DEF_PROTO );
#endif
#ifdef MAS_BASE_DIR
#  ifdef MAS_LOG_DIR
  if ( !popts->dir.log )
  {
    popts->dir.log = mas_strdup( MAS_BASE_DIR "/" MAS_LOG_DIR );
    HMSG( "DEF.LOGDIR: %s < '%s'", popts->dir.log, MAS_BASE_DIR "/" MAS_LOG_DIR );
  }
#  endif
#endif
#ifdef MAS_PIDS_DIR
  if ( !popts->dir.pids )
  {
    popts->dir.pids = mas_strdup( MAS_BASE_DIR "/" MAS_PIDS_DIR );
    HMSG( "DEF.PIDSDIR: %s", popts->dir.pids );
  }
#endif


#define XSTR(s) STR(s)
#define STR(s) #s
#ifdef MAS_LIBDIR
#  ifdef MAS_MODS_DIR
  if ( !popts->dir.mods )
  {
    popts->dir.mods = mas_strdup( XSTR( MAS_LIBDIR ) "/" MAS_MODS_DIR );
    HMSG( "DEF.MODSDIR: %s", popts->dir.mods );
  }
#  endif
#  ifdef MAS_PROTO_DIR
  if ( !popts->dir.proto )
  {
    popts->dir.proto = mas_strdup( XSTR( MAS_LIBDIR ) "/" MAS_PROTO_DIR );
    HMSG( "DEF.PROTODIR: %s", popts->dir.proto );
  }
#  endif
#endif
  return 0;
}

__attribute__ ( ( constructor( 3001 ) ) )
     static void mas_constructor( void )
{
  /* fprintf( stderr, "******************** CONSTRUCTOr %s e%d\n", __FILE__, errno ); */
  mas_common_constructor( IL, 1 );
}
