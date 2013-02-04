#include "mas_basic_def.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <pthread.h>

#include <sys/time.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_tools.h>

#include "mas_common.h"
#include "log/inc/mas_log.h"

#include "channel/inc/mas_channel.h"
#include "channel/inc/mas_channel_object.h"
#include "variables/inc/mas_variables.h"

#include "mas_lcontrol_object.h"

/*
this:
  mas_listener_control.c
related:
  mas_listener.c
  mas_channel_object.c

  mas_channel.c
  mas_control_types.h
*/

/*
this:
  mas_lcontrol_object.c
related:
  mas_lcontrol_object.h
  mas_listener_wait_client.c
more:
  mas_listener.c
  mas_listener_control.c
  mas_listener_control.h
  mas_listener_control_list.c
  mas_listener_control_list.h
  mas_listener_control_types.h
  mas_listener.h
  mas_listeners.c
  mas_listeners.h
  mas_listener_wait_client.h
  mas_server_main.c

  mas_cmdmod_listener.c

*/


mas_lcontrol_t *
mas_lcontrol_create( void )
{
  mas_lcontrol_t *plcontrol;

  plcontrol = mas_malloc( sizeof( mas_lcontrol_t ) );
  if ( plcontrol )
  {
    memset( plcontrol, 0, sizeof( mas_lcontrol_t ) );
  }
  else
  {
    EMSG( "FATAL" );
  }
  return plcontrol;
}

int
mas_lcontrol_register( mas_lcontrol_t * plcontrol )
{
  if ( ctrl.lcontrols_list )
  {
    /* pthread_mutex_lock( &ctrl.thglob.lcontrols_list_mutex ); */
    pthread_rwlock_wrlock( &ctrl.thglob.lcontrols_list_rwlock );
    MAS_LIST_ADD( ctrl.lcontrols_list, plcontrol, next );
    plcontrol->serial = ++ctrl.listener_serial;
    MAS_LOG( "registering listener; serial:%lu", plcontrol->serial );

    pthread_rwlock_unlock( &ctrl.thglob.lcontrols_list_rwlock );
    /* pthread_mutex_unlock( &ctrl.thglob.lcontrols_list_mutex ); */

    /* thMSG( "+C l/th %p ['%s':%u]", ( void * ) plcontrol, plcontrol->host, plcontrol->port ); */
  }
  else
  {
    /* ........... */
    EMSG( "+C l/th %p ['%s':%u]", ( void * ) plcontrol, plcontrol->host, plcontrol->port );
    sleep( 10 );
  }
  return 0;
}

/* naming : create + init + register = make */
mas_lcontrol_t *
mas_lcontrol_make( const char *host, unsigned port )
{
  mas_lcontrol_t *plcontrol = NULL;

  /* thMSG( ">C l/th ['%s':%u]", host, port ); */
  plcontrol = mas_lcontrol_create(  );
  if ( plcontrol )
  {
    mas_lcontrol_init( plcontrol, host, port );
    mas_lcontrol_register( plcontrol );
    /* thMSG( ">>C l/th %p ['%s':%u]", ( void * ) plcontrol, plcontrol->host, plcontrol->port ); */
  }
  else
  {
    EMSG( "?" );
  }
  return plcontrol;
}

static int
mas_lcontrol_create_transaction_controls_list( mas_lcontrol_t * plcontrol )
{
  if ( !plcontrol->transaction_controls_list )
  {
    /* WRONG pthread_mutex_lock( &plcontrol->transaction_mutex ); */
    /* WRONG pthread_rwlock_wrlock( &plcontrol->transaction_rwlock ); */
    plcontrol->transaction_controls_list = mas_malloc( sizeof( mas_rcontrol_list_head_t ) );
    if ( plcontrol->transaction_controls_list )
    {
      memset( plcontrol->transaction_controls_list, 0, sizeof( mas_rcontrol_list_head_t ) );
      MAS_LIST_INIT( plcontrol->transaction_controls_list );
    }
    /* WRONG pthread_rwlock_unlock( &plcontrol->transaction_rwlock ); */
    /* WRONG pthread_mutex_unlock( &plcontrol->transaction_mutex ); */
  }
  return 0;
}

/* naming : set members = init */
int
mas_lcontrol_init( mas_lcontrol_t * plcontrol, const char *host, unsigned port )
{
  unsigned hport;

  plcontrol->hostlen = mas_parse_host_port( host, &hport, port );
  plcontrol->host = mas_strndup( host, plcontrol->hostlen );
  plcontrol->port = hport;
  /* thMSG( "HOST: %s; PORT:%u", plcontrol->host, plcontrol->port ); */
  plcontrol->thread = ( pthread_t ) 0;
  /* pthread_mutex_init( &plcontrol->transaction_mutex, NULL ); */
  pthread_rwlock_init( &plcontrol->transaction_rwlock, NULL );
  pthread_rwlock_init( &plcontrol->variables_rwlock, NULL );
  plcontrol->status = MAS_STATUS_NONE;

  plcontrol->pchannel = mas_channel_create(  );
  mas_channel_init( plcontrol->pchannel, CHN_SOCKET, plcontrol->host, plcontrol->hostlen, plcontrol->port );
  {
    struct timeval td;

    gettimeofday( &td, NULL );
    plcontrol->activity_time = td;
  }
  mas_lcontrol_create_transaction_controls_list( plcontrol );
  return 0;
}

/* naming : free members + free = delete */
int
mas_lcontrol_delete( mas_lcontrol_t * plcontrol )
{
  if ( ctrl.lcontrols_list && !MAS_LIST_EMPTY( ctrl.lcontrols_list ) )
  {
    thMSG( "REMOVE %d %p", __LINE__, ( void * ) plcontrol );
    /* pthread_mutex_lock( &ctrl.thglob.lcontrols_list_mutex ); */
    pthread_rwlock_wrlock( &ctrl.thglob.lcontrols_list_rwlock );

    MAS_LIST_REMOVE( ctrl.lcontrols_list, plcontrol, mas_lcontrol_s, next );

    pthread_rwlock_unlock( &ctrl.thglob.lcontrols_list_rwlock );
    /* pthread_mutex_unlock( &ctrl.thglob.lcontrols_list_mutex ); */
    if ( plcontrol->host )
      mas_free( plcontrol->host );
    plcontrol->host = NULL;
    {
      mas_variables_list_head_t *vars;

      vars = plcontrol->variables;
      plcontrol->variables = NULL;
      mas_variables_delete( vars );
    }

    if ( plcontrol->pchannel )
    {
      mas_channel_delete( plcontrol->pchannel, 0, 0 );
      plcontrol->pchannel = NULL;
    }
    if ( plcontrol->transaction_controls_list )
    {
      /* pthread_mutex_lock( &plcontrol->transaction_mutex ); */
      pthread_rwlock_wrlock( &plcontrol->transaction_rwlock );
      mas_free( plcontrol->transaction_controls_list );
      plcontrol->transaction_controls_list = NULL;
      pthread_rwlock_unlock( &plcontrol->transaction_rwlock );
      /* pthread_mutex_unlock( &plcontrol->transaction_mutex ); */
    }
    /* pthread_mutex_destroy( &plcontrol->transaction_mutex ); */
    pthread_rwlock_destroy( &plcontrol->variables_rwlock );
    pthread_rwlock_destroy( &plcontrol->transaction_rwlock );
    /* memset( &plcontrol->transaction_mutex, 0, sizeof( plcontrol->transaction_mutex ) ); */
    memset( &plcontrol->transaction_rwlock, 0, sizeof( plcontrol->transaction_rwlock ) );
    thMSG( "FREE %d %p", __LINE__, ( void * ) plcontrol );
    plcontrol->thread = ( pthread_t ) 0;
    mas_free( plcontrol );
  }
  return 0;
}
