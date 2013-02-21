#include <mastar/wrap/mas_std_def.h>
/* #include "mas_server_def.h" */
/* #include "mas_basic_def.h"  */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <malloc.h>

#include <pthread.h>
#include <sys/prctl.h>

#include <mastar/wrap/mas_lib.h>
#include <mastar/wrap/mas_lib_thread.h>
#include <mastar/tools/mas_tools.h>

#include <mastar/types/mas_control_types.h>
#include <mastar/types/mas_opts_types.h>
extern mas_control_t ctrl;
extern mas_options_t opts;

/* #include "mas_common.h" */
#include <mastar/msg/mas_msg_def.h>
#include <mastar/msg/mas_msg_tools.h>
#include <mastar/log/mas_log.h>

#ifdef MAS_USE_CURSES
#  include <mastar/msg/mas_curses.h>
#endif

#include <mastar/thtools/mas_thread_tools.h>

#include <mastar/listener/mas_listener_control.h>

#include "mas_ticker.h"

/*
this:
  mas_ticker.c
related:
  mas_master.c
  mas_watcher.c
more:
  mas_cmdmod_server.c
  mas_init_server.c
  mas_init_server.h
  
  mas_cmdmod_client.h
  mas_cmdmod_get.h
  mas_cmdmod_mem.h
  mas_cmdmod_set.h
  mas_cmdmod_top.h

  mas_server_main.c
  mas_server_sig.c
  mas_server_sig.h
  mas_server_tools.c
  mas_server_tools.h

  mas_log.c
*/


/* static char *tmp = NULL; */

static size_t ticker_stacksize = 0;
static void *ticker_stackaddr = NULL;

static void
mas_ticker_cleanup( void *arg )
{
/* mas_lcontrols_cleaning_transactions returns not-joined-count */
  MAS_LOG( "by the way (ticker ending): cleaning transactions" );
  mas_lcontrols_cleaning_transactions( ctrl.forget_transactions, 0 /* don't wait */  );
  FMSG( "TICKER CLEANUP" );
  MAS_LOG( "ticker cleanup" );
}

static void
mas_ticker( void )
{
#define MUL 5
  int itick = 0;
  double zinterval = 2.;
  double interval = zinterval / MUL;

  while ( !ctrl.fatal )
  {
    unsigned long cur_time = ( unsigned long ) time( NULL );

    pthread_testcancel(  );
    if ( !ctrl.stamp.start_time )
      ctrl.stamp.start_time = ( unsigned long ) time( NULL );

    if ( itick % MUL == 0 && !ctrl.ticker_hide && ( unsigned long ) time( NULL ) - ctrl.sigint_time > 5 )
    {
      /* extern char edata, end; */
      int l1 = 0, l2 = 0;
      unsigned long elapsed_time;
      char outstr[200];
      static unsigned long gone_history[61];
      static unsigned long gone_history_cnt = 0;
      static double peak = 0.;

      if ( !gone_history_cnt )
      {
        memset( gone_history, 0, sizeof( gone_history ) );
        gone_history_cnt++;
      }
      else
        for ( int ig = sizeof( gone_history ) / sizeof( gone_history[0] ) - 1; ig > 0; ig-- )
        {
          gone_history[ig] = gone_history[ig - 1];
        }
      gone_history[0] = ctrl.clients_gone;
      *outstr = 0;
      strftime( outstr, sizeof( outstr ), "%d.%m %T", mas_xlocaltime(  ) );

      elapsed_time = cur_time - ctrl.stamp.start_time;

/* for test only : */
#ifdef MAS_USE_CURSES
      if ( use_curses )
      {
        extern unsigned long memory_balance;

        MFP( "(E%3lus:m%5lu) [%s] it%d; [c0%lu:c%lu:g%lu:g2%lu:D%ld] i/p:%d; keep l:%d\r", elapsed_time,
             memory_balance, outstr, itick, ctrl.clients_came0, ctrl.clients_came, ctrl.clients_gone, ctrl.clients_gone2,
             ctrl.clients_came - ctrl.clients_gone, ctrl.in_pipe, ctrl.keep_listening );
      }
      else
#endif
      {
        extern unsigned long memory_balance;
        double a1, a2, a10, a60;

        a1 = ( ( double ) ( ( gone_history[0] - gone_history[1] ) / 1 ) ) / zinterval;
        a2 = ( ( double ) ( ( gone_history[0] - gone_history[2] ) / 2 ) ) / zinterval;
        a10 = ( ( gone_history[0] - gone_history[10] ) / 10 ) / zinterval;
        a60 = ( ( gone_history[0] - gone_history[60] ) / 60 ) / zinterval;
        if ( peak < a1 )
          peak = a1;
        if ( ctrl.ticker_mode == 1 )
        {
          MFP( "\x1b]2;+%05lus(m%5lu) [%s] %d; [%lu:%lu:%ld:%lu] "
               "(lock:%d:%d) i/p:%d;" " mxc:%lu log { %lu-%lu=%lu m%lu }\x7", elapsed_time, memory_balance, outstr, itick,
               ctrl.clients_came, ctrl.clients_gone, ctrl.clients_came - ctrl.clients_gone, ctrl.xch_cnt, l1, l2, ctrl.in_pipe,
               ctrl.maxclients, ctrl.log_q_came, ctrl.log_q_gone, ctrl.log_q_came - ctrl.log_q_gone, ctrl.log_q_mem );
        }
        else if ( ctrl.ticker_mode == 2 )
        {
          MFP( "\r\x1b[33;41;1m+%05lus\x1b[0m\x1b[37;44;1m(m%5lu\x1b[0m) [\x1b[33;42;1m%s\x1b[0m] %d; [%lu:%lu:%ld:%lu] "
               "(lock:%d:%d) i/p:%d;" " mxc:%lu log { %lu-%lu=%lu m%lu }\x1b[K", elapsed_time, memory_balance, outstr, itick,
               ctrl.clients_came, ctrl.clients_gone, ctrl.clients_came - ctrl.clients_gone, ctrl.xch_cnt, l1, l2, ctrl.in_pipe,
               ctrl.maxclients, ctrl.log_q_came, ctrl.log_q_gone, ctrl.log_q_came - ctrl.log_q_gone, ctrl.log_q_mem );
        }
        /* MFP( "(%3lus:m%5lu) [%s] %d; [%lu:%lu:%ld:%lu] " "(lock:%d:%d) i/p:%d;"                                                    */
        /*      " {%lu:%8.2f(%6.4g):%6.2f} {%6.2f:%6.2f:%6.2f} log %lu-%lu=%lu\x1b[K\r", elapsed_time, memory_balance, outstr, itick, */
        /*      ctrl.clients_came, ctrl.clients_gone, ctrl.clients_came - ctrl.clients_gone, ctrl.xch_cnt, l1, l2, ctrl.in_pipe,      */
        /*      ctrl.maxclients, ( ( double ) ctrl.clients_gone ) / ctrl.transactions_time,                                           */
        /*      ctrl.transactions_time * 1.E3 / ( ( double ) ctrl.clients_gone ), peak, a1, a10, a60, ctrl.log_q_came,                */
        /*      ctrl.log_q_gone, ctrl.log_q_came-ctrl.log_q_gone );                                                                   */
      }
    }
    /* malloc_trim( 0 ); */
    mas_nanosleep( interval );

    /* MAS_LOG( "by the way (ticker): cleaning transactions" ); */
//  mas_lcontrols_cleaning_transactions( ctrl.forget_transactions, 0 /* don't wait */  );
    itick++;
  }
}

/* naming : pthread_create argument = th */
static void *
mas_ticker_th( void *arg )
{
  int old_cancelability = 0;

  ctrl.ticker_tid = mas_gettid(  );
  if ( prctl( PR_SET_NAME, ( unsigned long ) "zoctick" ) < 0 )
  {
    P_ERR;
  }

  MAS_LOG( "ticker start" );
  pthread_setcanceltype( PTHREAD_CANCEL_ASYNCHRONOUS, &old_cancelability );
  mas_in_thread( MAS_THREAD_TICKER, NULL, NULL );

  pthread_cleanup_push( mas_ticker_cleanup, NULL );
  mas_ticker(  );
  pthread_cleanup_pop( 1 );

  MAS_LOG( "ticker stop" );
  FMSG( "TICKER STOP" );
  mas_pthread_exit( NULL );
  return ( NULL );
}

/* naming : pthread_create = start */
int
mas_ticker_start( void )
{
  int r = 0;

  MFP( "\x1b]2;starting ticker; mode:%d\x7", ctrl.ticker_mode );
  if ( !ctrl.ticker_thread )
  {
    {
      int r;

      r = pthread_attr_getstack( &ctrl.thglob.ticker_attr, &ticker_stackaddr, &ticker_stacksize );
      tMSG( "creating ticker thread stack:%lu @ %p", ticker_stacksize, ticker_stackaddr );
      HMSG( "+ TICKER mode %d", ctrl.ticker_mode );
    }

    /* if ( !tmp )                 */
    /*   tmp = mas_malloc( 4321 ); */
    MAS_LOG( "starting ticker th." );

    /* r = mas_xpthread_create( &ctrl.ticker_thread, mas_ticker_th, MAS_THREAD_TICKER, NULL ); */
    r = pthread_create( &ctrl.ticker_thread, &ctrl.thglob.ticker_attr, mas_ticker_th, NULL );
    {
      int policy, rs;
      struct sched_param sched;

      rs = pthread_getschedparam( ctrl.ticker_thread, &policy, &sched );
      MAS_LOG( "(%d) created(?) ticker thread [%lx] %d - %d (%d)", r, ctrl.ticker_thread, policy, sched.sched_priority, rs );
      /* SCHED_IDLE ... SCHED_RR */
      rs = pthread_setschedparam( ctrl.ticker_thread, SCHED_IDLE, &sched );
      /* rs = pthread_getschedparam( ctrl.ticker_thread, &policy, &sched ); */
      tMSG( "(%d) created(?) ticker thread [%lx] %d - %d (%d)", r, ctrl.ticker_thread, policy, sched.sched_priority, rs );
    }
    MAS_LOG( "(%d) created(?) ticker thread [%lx]", r, ctrl.ticker_thread );
  }
  else
  {
    MAS_LOG( "running w/o ticker th." );
  }
  return r;
}

/* naming : cancel + join = stop */
int
mas_ticker_stop( void )
{
  int r = 0;

  if ( ctrl.ticker_thread )
  {
    FMSG( "CANCEL TICKER" );
    MAS_LOG( "stopping (cancelling) ticker [%lx]", ctrl.ticker_thread );

    mas_pthread_cancel( ctrl.ticker_thread );

    mas_xpthread_join( ctrl.ticker_thread );
    /* mas_pthread_detach( ctrl.ticker_thread ); */
    mMSG( "JOINED ticker" );
    MAS_LOG( "stopped ticker" );
    ctrl.ticker_thread = ( pthread_t ) 0;
    HMSG( "- TICKER" );
    FMSG( "TICKER STOPPED" );
    MFP( "\x1b]2;stopped ticker; mode:%d\x7", ctrl.ticker_mode );
  }
  else
  {
    MAS_LOG( "ticker already cancelled" );
  }
  return r;
}