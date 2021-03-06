#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

#include <pthread.h>
#include <sys/prctl.h>

#include <mastar/types/mas_control_types.h>
#include <mastar/types/mas_opts_types.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/types/mas_common_defs.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/wrap/mas_lib.h>
#include <mastar/wrap/mas_lib_thread.h>
#include <mastar/tools/mas_tools.h>

#include <mastar/msg/mas_msg_def.h>
#include <mastar/msg/mas_msg_tools.h>

#include <mastar/log/mas_log.h>

#include <mastar/thtools/mas_ocontrol_tools.h>
#include <mastar/thtools/mas_thread_tools.h>

#include <mastar/transaction/mas_transaction_control.h>
#include <mastar/transaction/mas_transaction.h>

#include <mastar/listener/mas_listener_control.h>
#include <mastar/listener/mas_listeners.h>


#include "mas_server_tools.h"

#include "mas_watcher.h"

/*
this:
  mas_watcher.c
related:
  mas_master.c
  mas_ticker.c
more:
  mas_cmdmod_server.c
  mas_init_server.c
  mas_init_server.h
  mas_cmdmod_client.c
  mas_cmdmod_get.c
  mas_cmdmod_mem.c
  mas_cmdmod_set.c
  mas_cmdmod_root.c
  mas_server_main.c
  mas_server_sig.c
  mas_server_sig.h
  mas_server_tools.c
  mas_server_tools.h

*/




/* static char *tmp = NULL; */

static size_t watcher_stacksize = 0;
static void *watcher_stackaddr = NULL;

static void
mas_watcher_cleanup( void *arg )
{
/* mas_lcontrols_cleaning_transactions returns not-joined-count */
  MAS_LOG( "by the way (watcher ending): cleaning transactions" );
#if 0
  mas_lcontrols_cleaning_transactions( ctrl.forget_transactions, 0 /* don't wait */  );
#endif
  EMSG( "WATCHER CLEANUP" );
  MAS_LOG( "watcher cleanup" );
  /* mas_in_thread_end(  ); */
}

static void
mas_watcher( void )
{
  CTRL_PREPARE;
  EVAL_PREPARE;
  int rn = 0;

#define MUL 10
  int itick = 0;
  double zinterval = 2.;
  double interval = zinterval / MUL;
  int stop = 0;
  int nlisteners_ever_open = 0;

  IEVAL( rn, prctl( PR_SET_NAME, ( unsigned long ) "zocWatchLS" ) );
  while ( ( !ctrl.fatal && !stop ) || ( ctrl.clients_created > ctrl.clients_removed ) )
  {
    /* unsigned long cur_time = ( unsigned long ) time( NULL ); */
    int out;

    if ( !ctrl.stamp.start_time )
      ctrl.stamp.start_time = ( unsigned long ) time( NULL );
    ctrl.watch_cnt++;
    out = itick % MUL == 0 && !ctrl.watcher_hide;
    {
      /* extern mas_options_t g_opts; */
      extern mas_options_t *gpopts;
      unsigned nlistener = 0;
      unsigned ntransaction = 0;
      unsigned nlistener_open = 0;

      /* unsigned long elapsed_time; */
      mas_lcontrol_t *plcontrol = NULL;

      /* elapsed_time = cur_time - ctrl.stamp.start_time; */
      if ( out )
      {
        /* thMSG( "watcher %lu", elapsed_time ); */
        MAS_LOG( "WATCH ==========================================================================================" );
        MAS_LOG( "WATCH main th: %u [%lx] master th: %u [%lx]", ctrl.threads.n.main.tid, ctrl.threads.n.main.thread,
                 ctrl.threads.n.master.tid, ctrl.threads.n.master.thread );
        MAS_LOG( "WATCH ticker th: %u [%lx] watcher th: %u [%lx] logger th: %u [%lx]", ctrl.threads.n.ticker.tid,
                 ctrl.threads.n.ticker.thread, ctrl.threads.n.watcher.tid, ctrl.threads.n.watcher.thread, ctrl.threads.n.logger.tid,
                 ctrl.threads.n.logger.thread );
        MAS_LOG( "WATCH clients: {%lu - %lu = %lu};server pid:%u; tid:%5u; [%lx]", ctrl.clients_came, ctrl.clients_gone,
                 ctrl.clients_came - ctrl.clients_gone, ctrl.threads.n.main.pid, ctrl.threads.n.main.tid, ctrl.threads.n.main.thread );
      }
      nlistener = 0;
      ntransaction = 0;
      nlistener_open = 0;
      pthread_rwlock_rdlock( &ctrl.thglob.lcontrols_list_rwlock );
      MAS_LIST_FOREACH( plcontrol, ctrl.lcontrols_list, next )
      {
        struct timeval linactive_time;
        struct timeval lnow_time;

        {
          gettimeofday( &lnow_time, NULL );
          {
            linactive_time.tv_sec = lnow_time.tv_sec - plcontrol->h.activity_time.tv_sec;
            linactive_time.tv_usec = lnow_time.tv_usec - plcontrol->h.activity_time.tv_usec;
            if ( linactive_time.tv_usec < 0 )
            {
              linactive_time.tv_usec += 1000000;
              linactive_time.tv_sec -= 1;
            }
          }
        }

        if ( out )
        {
          MAS_LOG( "WATCH > %u. %20s:%-6u;tid:%5u; [%lx] {%lu - %lu = %lu} {%lu.%06lu %lu.%06lu I/A:%lu.%06lu}  [%s]", nlistener,
                   plcontrol->host, plcontrol->port, plcontrol->h.tid, plcontrol->h.thread, plcontrol->clients_came,
                   plcontrol->clients_gone, plcontrol->clients_came - plcontrol->clients_gone, plcontrol->h.activity_time.tv_sec,
                   plcontrol->h.activity_time.tv_usec, lnow_time.tv_sec, lnow_time.tv_usec, linactive_time.tv_sec,
                   linactive_time.tv_usec, plcontrol->h.pchannel ? "L" : "C" );
        }
        {
          mas_rcontrol_t *prcontrol;
          unsigned ntransaction_in_l;

          nlistener++;
          ntransaction_in_l = 0;
          pthread_rwlock_rdlock( &plcontrol->transaction_rwlock );
          MAS_LIST_FOREACH( prcontrol, plcontrol->transaction_controls_list, next )
          {
            char *sip = NULL;
            mas_channel_t *pchannel = NULL;
            struct timeval rnow_time;
            struct timeval rinactive_time;

            ntransaction_in_l++;
            ntransaction++;
            if ( plcontrol->h.pchannel )
            {
              nlisteners_ever_open++;
              nlistener_open++;
            }
            {
              gettimeofday( &rnow_time, NULL );
              {
                rinactive_time.tv_sec = rnow_time.tv_sec - prcontrol->h.activity_time.tv_sec;
                rinactive_time.tv_usec = rnow_time.tv_usec - prcontrol->h.activity_time.tv_usec;
                if ( rinactive_time.tv_usec < 0 )
                {
                  rinactive_time.tv_usec += 1000000;
                  rinactive_time.tv_sec -= 1;
                }
              }
            }
            pchannel = prcontrol ? prcontrol->h.pchannel : NULL;
            if ( out )
            {
              const char *proto_name = NULL;

              if ( pchannel )
                sip = mas_ip_string( &pchannel->serv.addr.sin_addr );

              proto_name = prcontrol->proto_desc ? prcontrol->proto_desc->name : "?";
              MAS_LOG( "WATCH  >> %u. %s %s:%u;tid:%5u; [%lx] {%lu.%06lu %lu.%06lu I/A:%lu.%06lu} #%u", ntransaction_in_l,
                       proto_name, sip ? sip : "?", pchannel ? pchannel->port : 0, prcontrol->h.tid, prcontrol->h.thread,
                       prcontrol->h.activity_time.tv_sec, prcontrol->h.activity_time.tv_usec, rnow_time.tv_sec, rnow_time.tv_usec,
                       rinactive_time.tv_sec, rinactive_time.tv_usec, prcontrol->xch_cnt );
              if ( sip )
                mas_free( sip );
            }
            if ( prcontrol->proto_desc && 0 == strcmp( prcontrol->proto_desc->name, "http" ) )
              if ( rinactive_time.tv_sec > 10 )
              {
                HMSG( "WATCHER : http tr. inactive %lus!!", rinactive_time.tv_sec );
                mas_transaction_cancel( prcontrol );
              }
          }
        }
        pthread_rwlock_unlock( &plcontrol->transaction_rwlock );
      }
      pthread_rwlock_unlock( &ctrl.thglob.lcontrols_list_rwlock );
      if ( out )
        MAS_LOG( "WATCH ==========================================================================================" );
      if ( ctrl.watcher_stop )
      {
        EMSG( "WATCHER STOP : %d %d %d", nlistener_open, ntransaction, nlisteners_ever_open );
        if ( nlistener_open == 0 && ntransaction == 0 /* && nlisteners_ever_open */  )
          stop = 1;
      }
      /* else if ( !ctrl.threads.n.main.thread || ( !ctrl.threads.n.master.thread && gpopts->make_master_thread ) */
      else if ( !ctrl.threads.n.main.thread || ( !ctrl.threads.n.master.thread && OPT_QFLAG( gpopts, make_master_thread ) )
                || !ctrl.threads.n.watcher.thread || !ctrl.threads.n.logger.thread || !ctrl.threads.n.ticker.thread )
      {
        HMSG( "WATCHER main:%d; master:%d", ctrl.threads.n.main.thread ? 1 : 0, ctrl.threads.n.master.thread ? 1 : 0 );
      }
      /* fprintf( stderr, "WATCHER %u %u %u\n", nlistener_open, ntransaction, nlisteners_ever_open ); */
    }
    if ( ctrl.stop_listeners )
    {
      ctrl.keep_listening = 0;
      mas_listeners_cancel(  );
      ctrl.watcher_stop = 1;
      /* ctrl.stop_listeners = 0; */
      EMSG( "WATCHER - STOP LISTENERS" );
    }
    /* WMSG( "WATCHER SL %d", ctrl.stop_listeners ); */

    mas_nanosleep( interval );

#if 0
    mas_lcontrols_cleaning_transactions( ctrl.forget_transactions, 0 /* don't wait */  );
#endif
    itick++;
  }
  EMSG( "WATCH LOOP END : %u %u : %lu - %lu [%d]", ctrl.fatal, stop, ctrl.clients_created, ctrl.clients_removed,
        ( !ctrl.fatal && !stop && ctrl.clients_created > ctrl.clients_removed ) );
  IEVAL( rn, prctl( PR_SET_NAME, ( unsigned long ) "zocWatchLSXit" ) );
}

/* naming : pthread_create argument = th */
static void *
mas_watcher_th( void *arg )
{
  CTRL_PREPARE;
  EVAL_PREPARE;
  int rn = 0;

  ctrl.threads.n.watcher.tid = mas_gettid(  );
  IEVAL( rn, prctl( PR_SET_NAME, ( unsigned long ) "zocWatchTh" ) );

  MAS_LOG( "watcher start" );
  mas_in_thread( MAS_THREAD_WATCHER, NULL, NULL );

  pthread_cleanup_push( mas_watcher_cleanup, NULL );
  mas_watcher(  );
  pthread_cleanup_pop( 1 );

  MAS_LOG( "watcher stop" );
  EMSG( "WATCHER STOP" );
  IEVAL( rn, prctl( PR_SET_NAME, ( unsigned long ) "zocWatchThXit" ) );
  mas_pthread_exit( NULL );
  return ( NULL );
}

/* naming : pthread_create = start */
int
mas_watcher_start( const mas_options_t * popts )
{
  CTRL_PREPARE;
  EVAL_PREPARE;
  int r = 0;

  if ( !ctrl.threads.n.watcher.thread )
  {
    {
      ( void ) /* r = */ pthread_attr_getstack( &ctrl.thglob.watcher_attr, &watcher_stackaddr, &watcher_stacksize );
      tMSG( "creating watcher thread stack:%lu @ %p", ( unsigned long ) watcher_stacksize, watcher_stackaddr );
      HMSG( "+ WATCHER" );
    }

    MAS_LOG( "starting watcher th." );

    /* r = mas_xpthread_create( &ctrl.threads.n.watcher.thread, mas_watcher_th, MAS_THREAD_WATCHER, NULL ); */
    r = pthread_create( &ctrl.threads.n.watcher.thread, &ctrl.thglob.watcher_attr, mas_watcher_th, NULL );
    MAS_LOG( "(%d) created(?) watcher thread [%lx]", r, ctrl.threads.n.watcher.thread );
  }
  else
  {
    MAS_LOG( "running w/o watcher th." );
  }
  return r;
}

/* naming : cancel + join = stop */
int
mas_watcher_stop( void )
{
  CTRL_PREPARE;
  EVAL_PREPARE;
  int r = 0;

  if ( ctrl.threads.n.watcher.thread )
  {
    MAS_LOG( "stopping (cancelling) watcher [%lx]", ctrl.threads.n.watcher.thread );

    mas_pthread_cancel( ctrl.threads.n.watcher.thread );

    mas_xpthread_join( ctrl.threads.n.watcher.thread );
    /* MAS_LOG( "stopped watcher" ); */
    ctrl.threads.n.watcher.thread = ( pthread_t ) 0;
    HMSG( "- WATCHER" );
    WMSG( "WATCHER STOPPED" );
  }
  else
  {
    MAS_LOG( "watcher already cancelled" );
  }
  return r;
}
