#define MAS_MAX_INT_1 2
#define MAS_MAX_INT_2 10
#include <mastar/wrap/mas_std_def.h>
/* #include "mas_client_def.h" */
/* #include "mas_basic_def.h"  */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <signal.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/wrap/mas_lib_thread.h>

#include <mastar/types/mas_control_types.h>
#include <mastar/types/mas_opts_types.h>
extern mas_control_t ctrl;
extern mas_options_t opts;

#include <mastar/msg/mas_msg_def.h>
#include <mastar/msg/mas_msg_tools.h>
#include <mastar/msg/mas_curses.h>

#include "mas_init_client.h"

#include "mas_client.h"


/*
this:
  mas_client_sig.c
related:
  mas_client_readline.c
  mas_client.c
  mas_client_main.c

  mas_sig.c
  mas_server_sig.c
*/



/* pthread_t *listening_threads = NULL; */

/* typedef void ( *sighandler_t ) ( int ); */
/* sighandler_t signal( int signum, sighandler_t handler ); */

void
sigint_han( int s )
{
  /* extern int rl_done; */
  static unsigned long start_time = 0;
  unsigned long cur_time = ( unsigned long ) time( NULL );

  if ( !start_time || cur_time - start_time > 5 )
  {
    start_time = ( unsigned long ) time( NULL );
    ctrl.int_cnt = 0;
  }
  if ( ctrl.stderrfile )
    fprintf( ctrl.stderrfile, "INT %d of %d; i/c:%d; i/p:%d k/l:%u\n", ctrl.int_cnt, MAS_MAX_INT_2, ctrl.in_client, ctrl.in_pipe,
             ctrl.keep_listening );
/*
  if (pthread_equal( pthread_self(  ),ctrl.pth ))
  {
    pthread_t *pt;

    pt = listening_threads;
    while ( pt && *pt )
    {
      GMSG( "INT to %lx @ %lx", ( unsigned long ) *pt, pthread_self(  ) );
      pthread_kill( *pt, SIGINT );
      pt++;
    }
  }
  */
  ctrl.int_cnt++;
  if ( ctrl.int_cnt > MAS_MAX_INT_1 )
  {
    ctrl.in_client = 0;
    ctrl.keep_listening = 0;
    ctrl.in_pipe--;
  }
  /* RL_SETSTATE( RL_STATE_DONE ); */
  if ( ctrl.int_cnt > MAS_MAX_INT_2 )
  {
    exit( 3 );
  }
  /* rl_done = 1;             */
  /* rl_reset_after_signal(); */
}

void
sigquit_han( int s )
{
  ctrl.in_client = 0;
  ctrl.keep_listening = 0;
  ctrl.in_pipe--;
#ifdef EMSG
  EMSG( "QUIT%s", "." );
#endif
}

void
sigabrt_han( int s )
{
  ctrl.in_client = 0;
  ctrl.keep_listening = 0;
  ctrl.in_pipe--;
#ifdef EMSG
  EMSG( "ABRT%s", "." );
#endif
}

void
sigterm_han( int s )
{
#ifdef EMSG
  EMSG( "TERM %d", ctrl.term_cnt );
#endif
  ctrl.term_cnt++;
  ctrl.keep_listening = 0;
  ctrl.in_client = 0;
  ctrl.in_pipe--;
}

void
sighup_han( int s )
{
#ifdef EMSG
  EMSG( "HUP" );
#endif
  ctrl.in_client = 0;
}

void
sigpipe_han( int s )
{
#ifdef EMSG
  EMSG( "PIPE" );
#endif
  /* ctrl.in_client = 1; */
  /* ctrl.in_pipe--; */
}

void
mas_atexit( void )
{
  mas_destroy_client(  );
#ifdef MAS_TRACEMEM
  extern unsigned long memory_balance;

  FMSG( "AT EXIT, memory_balance:%ld", memory_balance );
  if ( print_memlist( ctrl.stderrfile, FL ) < 0 )
    if ( print_memlist( ctrl.old_stderrfile, FL ) < 0 )
      if ( print_memlist( ctrl.msgfile, FL ) < 0 )
        print_memlist( stderr, FL );

#else
  FMSG( "AT EXIT" );
#endif
  if ( ctrl.old_stderrfile )
  {
    fclose( ctrl.old_stderrfile );
    ctrl.old_stderrfile = NULL;
  }
  FMSG( "======================================================================" );
}

__attribute__ ( ( constructor ) )
     static void master_constructor( void )
{
  if ( !ctrl.stderrfile )
    ctrl.stderrfile = stderr;
  atexit( mas_atexit );
  ctrl.is_client = 1;
  ctrl.is_server = 0;
  if ( ctrl.stderrfile )
    fprintf( ctrl.stderrfile, "******************** CONSTRUCTOR %s\n", __FILE__ );
}

__attribute__ ( ( destructor ) )
     static void master_destructor( void )
{
  if ( ctrl.stderrfile )
    fprintf( ctrl.stderrfile, "******************** DESTRUCTOR %s\n", __FILE__ );
}