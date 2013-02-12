#include "mas_basic_def.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_arg_tools.h>

#include <mastar/types/mas_control_types.h>
#include <mastar/types/mas_opts_types.h>
extern mas_control_t ctrl;
extern mas_options_t opts;

/* #include "mas_common.h" */
#include <mastar/msg/mas_msg_def.h>
#include <mastar/msg/mas_msg_tools.h>
#include <mastar/log/mas_log.h>

#include "init/inc/mas_opts.h"
#include "control/inc/mas_control.h"

#include "mas_cli_options.h"


/*
this:
  mas_cli_options.c
related:
  mas_cli_options.h
  mas_opts_types.h
  mas_opts_data.c
  mas_msg_tools.c
  mas_control.c

*/


/* int                                                                                                                        */
/* mas_getopt_long( int argc, char *const argv[], const char *optstring, const struct option *longopts, int *longindex )      */
/* {                                                                                                                          */
/*   return getopt_long( argc, argv, optstring, longopts, longindex );                                                        */
/* }                                                                                                                          */
/*                                                                                                                            */
/* int                                                                                                                        */
/* mas_getopt_long_only( int argc, char *const argv[], const char *optstring, const struct option *longopts, int *longindex ) */
/* {                                                                                                                          */
/*   return getopt_long_only( argc, argv, optstring, longopts, longindex );                                                   */
/* }                                                                                                                          */

typedef enum mas_cli_opts_e
{
  MAS_CLI_OPT_NONE,
  MAS_CLI_OPT_TEST = 't',
  MAS_CLI_OPT_HELP = 'h',
  MAS_CLI_OPT_DAEMON = 'd',
  MAS_CLI_OPT_HOST = 'H',
  MAS_CLI_OPT_PORT = 'P',
  MAS_CLI_OPT_MODSDIR = 'M',
  MAS_CLI_OPT_LOGDIR = 'L',
  MAS_CLI_OPT_PLUS = 10000,
  MAS_CLI_OPT_PROTODIR,
  MAS_CLI_OPT_PROTO,
  MAS_CLI_OPT_NODAEMON,
  MAS_CLI_OPT_MSG,
  MAS_CLI_OPT_NOMSG,
  MAS_CLI_OPT_NOHOSTS,
  MAS_CLI_OPT_CLOSE_STD,
  MAS_CLI_OPT_NOCLOSE_STD,
  MAS_CLI_OPT_MESSAGES,
  MAS_CLI_OPT_NOMESSAGES,
  MAS_CLI_OPT_NOLOGGER,
  MAS_CLI_OPT_LOGGER,
  MAS_CLI_OPT_NOLOG,
  MAS_CLI_OPT_LOG,
  MAS_CLI_OPT_NOTICKER,
  MAS_CLI_OPT_TICKER,
  MAS_CLI_OPT_NOWATCHER,
  MAS_CLI_OPT_WATCHER,
  MAS_CLI_OPT_NOMASTER,
  MAS_CLI_OPT_MASTER,
  MAS_CLI_OPT_NOMASTER_THREAD,
  MAS_CLI_OPT_MASTER_THREAD,
  MAS_CLI_OPT_LISTEN,
  MAS_CLI_OPT_NOLISTEN,
  MAS_CLI_OPT_LISTENER,
  MAS_CLI_OPT_NOLISTENER,
  MAS_CLI_OPT_EXITSLEEP,
  MAS_CLI_OPT_LISTENER_SINGLE,
  MAS_CLI_OPT_TRANSACTION_SINGLE,
  MAS_CLI_OPT_COMMAND,
  MAS_CLI_OPT_MSGTO,
} mas_cli_opts_t;

static char cli_enabled_options[] = "hH:P:L:t";

static struct option cli_longopts[] = {
  {"test", no_argument, NULL, MAS_CLI_OPT_TEST},
  {"help", no_argument, NULL, MAS_CLI_OPT_HELP},
  {"exitsleep", optional_argument, NULL, MAS_CLI_OPT_EXITSLEEP},
  {"daemon", no_argument, NULL, MAS_CLI_OPT_DAEMON},
  {"command", required_argument, NULL, MAS_CLI_OPT_COMMAND},
  {"message-to", required_argument, NULL, MAS_CLI_OPT_MSGTO},
  {"listener-single", no_argument, NULL, MAS_CLI_OPT_LISTENER_SINGLE},
  {"transaction-single", no_argument, NULL, MAS_CLI_OPT_TRANSACTION_SINGLE},
  {"messages", no_argument, NULL, MAS_CLI_OPT_MESSAGES},
  {"nomessages", no_argument, NULL, MAS_CLI_OPT_NOMESSAGES},
  {"close-std", no_argument, NULL, MAS_CLI_OPT_CLOSE_STD},
  {"noclose-std", no_argument, NULL, MAS_CLI_OPT_NOCLOSE_STD},
  {"nologger", no_argument, NULL, MAS_CLI_OPT_NOLOGGER},
  {"logger", no_argument, NULL, MAS_CLI_OPT_LOGGER},
  {"nolog", no_argument, NULL, MAS_CLI_OPT_NOLOG},
  {"log", no_argument, NULL, MAS_CLI_OPT_LOG},
  {"modsdir", required_argument, NULL, MAS_CLI_OPT_MODSDIR},
  {"protodir", required_argument, NULL, MAS_CLI_OPT_PROTODIR},
  {"logdir", required_argument, NULL, MAS_CLI_OPT_LOGDIR},
  {"nowatcher", no_argument, NULL, MAS_CLI_OPT_NOWATCHER},
  {"watcher", no_argument, NULL, MAS_CLI_OPT_WATCHER},
  {"noticker", no_argument, NULL, MAS_CLI_OPT_NOTICKER},
  {"ticker", no_argument, NULL, MAS_CLI_OPT_TICKER},
  {"nowatcher", no_argument, NULL, MAS_CLI_OPT_NOTICKER},
  {"watcher", no_argument, NULL, MAS_CLI_OPT_TICKER},
  {"nomaster", required_argument, NULL, MAS_CLI_OPT_NOMASTER},
  {"master", no_argument, NULL, MAS_CLI_OPT_MASTER},
  {"nomthread", no_argument, NULL, MAS_CLI_OPT_NOMASTER_THREAD},
  {"mthread", no_argument, NULL, MAS_CLI_OPT_MASTER_THREAD},
  {"nolistener", required_argument, NULL, MAS_CLI_OPT_NOLISTENER},
  {"listener", no_argument, NULL, MAS_CLI_OPT_LISTENER},
  {"nolisten", required_argument, NULL, MAS_CLI_OPT_NOLISTEN},
  {"listen", no_argument, NULL, MAS_CLI_OPT_LISTEN},
  {"nodaemon", no_argument, NULL, MAS_CLI_OPT_NODAEMON},
  {"proto", required_argument, NULL, MAS_CLI_OPT_PROTO},
  {"host", required_argument, NULL, MAS_CLI_OPT_HOST},
  {"nohosts", no_argument, NULL, MAS_CLI_OPT_NOHOSTS},
  {"port", required_argument, NULL, MAS_CLI_OPT_PORT},
  {"nomsg", no_argument, NULL, MAS_CLI_OPT_NOMSG},
  {"msg", required_argument, NULL, MAS_CLI_OPT_MSG},
};

long
mas_cli_optval( const char *arg, long def, int *pr )
{
  long val = 0;

  if ( arg && *arg )
  {
    val = strtol( arg, NULL, 10 );
    if ( ( errno == ERANGE && ( val == LONG_MAX || val == LONG_MIN ) ) || ( errno != 0 && val == 0 ) )
    {
      val = 0;
      *pr = -1;
    }
    else
      *pr = 0;
  }
  else if ( pr )
  {
    val = def;
    *pr = 1;
  }

  return val;
}

int
mas_cli_make_option( int opt, const char *m_optarg )
{
  int r = 0;

  /* HMSG( "CLI M/O %d", opt ); */
  switch ( opt )
  {
  case MAS_CLI_OPT_TEST:
    HMSG( "Test" );
    break;
  case MAS_CLI_OPT_HELP:
    HMSG( "Help" );
    break;
  case MAS_CLI_OPT_COMMAND:
    HMSG( "COMMAND %s", optarg );
    mas_ctrl_add_command( optarg );
    break;
  case MAS_CLI_OPT_MSGTO:
    HMSG( "MSG>%s", optarg );
    if ( opts.msgfilename )
      mas_free( opts.msgfilename );
    opts.msgfilename = NULL;
    if ( optarg && *optarg )
      opts.msgfilename = mas_strdup( optarg );
    break;
  case MAS_CLI_OPT_HOST:
    opts.hosts_num = mas_add_argv_arg( opts.hosts_num, &opts.hosts, optarg );
    mMSG( "HOST %d: %s [%p]", opts.hosts_num, optarg, ( void * ) opts.hosts );
    break;
  case MAS_CLI_OPT_PROTO:
    opts.protos_num = mas_add_argv_arg( opts.protos_num, &opts.protos, optarg );
    mMSG( "PROTO %d: %s [%p]", opts.protos_num, optarg, ( void * ) opts.protos );
    break;
  case MAS_CLI_OPT_PORT:
    {
      opts.default_port = 0;
      sscanf( optarg, "%u", &opts.default_port );
      HMSG( "PORT: %u", opts.default_port );
    }
    break;
  case MAS_CLI_OPT_MODSDIR:
    if ( opts.modsdir )
      mas_free( opts.modsdir );
    opts.modsdir = NULL;
    if ( optarg && *optarg )
      opts.modsdir = mas_strdup( optarg );
    break;
  case MAS_CLI_OPT_PROTODIR:
    if ( opts.protodir )
      mas_free( opts.protodir );
    opts.protodir = NULL;
    if ( optarg && *optarg )
      opts.protodir = mas_strdup( optarg );
    break;
  case MAS_CLI_OPT_LOGDIR:
    if ( opts.logdir )
      mas_free( opts.logdir );
    opts.logdir = NULL;
    if ( optarg && *optarg )
      opts.logdir = mas_strdup( optarg );
    break;
  case MAS_CLI_OPT_LISTENER_SINGLE:
    opts.listener_single = 1;
    break;
  case MAS_CLI_OPT_TRANSACTION_SINGLE:
    opts.transaction_single = 1;
    break;
  case MAS_CLI_OPT_NODAEMON:
    opts.nodaemon = 1;
    break;
  case MAS_CLI_OPT_DAEMON:
    opts.nodaemon = 0;
    break;
  case MAS_CLI_OPT_NOLOGGER:
    opts.nologger = 1;
    break;
  case MAS_CLI_OPT_LOGGER:
    opts.nologger = 0;
    break;
  case MAS_CLI_OPT_NOLOG:
    opts.nolog = 1;
    break;
  case MAS_CLI_OPT_LOG:
    opts.nolog = 0;
    break;
  case MAS_CLI_OPT_NOTICKER:
    opts.noticker = 1;
    break;
  case MAS_CLI_OPT_TICKER:
    opts.noticker = 0;
    break;
  case MAS_CLI_OPT_NOWATCHER:
    opts.nowatcher = 1;
    break;
  case MAS_CLI_OPT_WATCHER:
    opts.nowatcher = 0;
    break;
  case MAS_CLI_OPT_NOLISTENER:
    opts.nolistener = mas_cli_optval( optarg, 30, &r );
    break;
  case MAS_CLI_OPT_LISTENER:
    opts.nolistener = 0;
    break;
  case MAS_CLI_OPT_NOLISTEN:
    opts.nolisten = mas_cli_optval( optarg, 30, &r );
    break;
  case MAS_CLI_OPT_LISTEN:
    opts.nolisten = 0;
    break;
  case MAS_CLI_OPT_NOMASTER:
    opts.nomaster = mas_cli_optval( optarg, 30, &r );
    break;
  case MAS_CLI_OPT_MASTER_THREAD:
    opts.make_master_thread = 1;
    break;
  case MAS_CLI_OPT_NOMASTER_THREAD:
    opts.make_master_thread = 0;
    break;
  case MAS_CLI_OPT_MASTER:
    opts.nomaster = 0;
    break;
  case MAS_CLI_OPT_NOCLOSE_STD:
    opts.noclose_std = 1;
    break;
  case MAS_CLI_OPT_CLOSE_STD:
    opts.noclose_std = 0;
    break;
  case MAS_CLI_OPT_NOMESSAGES:
    opts.nomessages = 1;
    break;
  case MAS_CLI_OPT_MESSAGES:
    opts.nomessages = 0;
    break;
  case MAS_CLI_OPT_NOMSG:
    HMSG( "flags: %lo", opts.f.word );
    opts.f.word = 0;
    HMSG( "flags: %lo", opts.f.word );
    opts.f.bit.msg_tr = 0;
    HMSG( "flags: %lo", opts.f.word );
    break;
  case MAS_CLI_OPT_MSG:
    if ( 0 == strcmp( "mem", optarg ) )
      opts.f.bit.msg_mem = 1;
    break;
  case MAS_CLI_OPT_NOHOSTS:
    opts.hosts_num = mas_del_argv( opts.hosts_num, opts.hosts, 0 );
    opts.hosts = NULL;
    break;
  case MAS_CLI_OPT_EXITSLEEP:
    opts.exitsleep = ( unsigned ) ( optarg && *optarg ? strtol( optarg, NULL, 10 ) : 30 );
    break;

  default:                     /* '?' */
    /* fprintf( stderr, "Usage: %s [-t nsecs] [-n] name\n", argv[0] ); */

    /* ctrl.in_client = 0; */
    ctrl.fatal = 1;
    ctrl.keep_listening = 0;
    break;
  }
  /* HMSG( "getopt_long:%d Usage: %s [--help -h]", argv[0] ); */
  return r;
}

int
mas_cli_options( int argc, char *const argv[] )
{
  int r = 0;
  int opt;
  int indx = 0;
  int afterlast = 0;

  HMSG( "CLI" );
  optind = 1;
  while ( ( opt = getopt_long( argc, argv, cli_enabled_options, cli_longopts, &indx ) ) != -1 && !ctrl.fatal )
  {
    r = mas_cli_make_option( opt, optarg );
    HMSG( "CLI %d", r );
    afterlast = optind;
    /* MSG( "cli option made (%d) ctrl.fatal:%u {%d:%d}", opt, ctrl.fatal, ctrl.argv_nonoptind, optind ); */
  }
  HMSG( "/CLI %d", optind );
  MAS_LOG( "cli options made" );
  return afterlast;
}
