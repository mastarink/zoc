#include <mastar/wrap/mas_std_def.h>

#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/modules/mas_modules_commands_eval.h>

#include <mastar/types/mas_control_types.h>
extern mas_control_t ctrl;

#include <mastar/msg/mas_msg_def.h>
#include <mastar/msg/mas_msg_tools.h>
#include <mastar/log/mas_log.h>

/* #include <mastar/channel/mas_channel_object.h> */
/* #include <mastar/channel/mas_channel_open.h> */
#include <mastar/channel/mas_channel_buffer.h>
#include <mastar/channel/mas_channel.h>

#include <mastar/messageio/mas_message_io.h>


#include <mastar/types/mas_message_types.h>



/*
this:
  mas_transaction_xcromas.c
related:
  mas_transaction.c
  mas_transaction_http.c

  mas_tools.c
  mas_msg_tools.c
*/

static void
do_exit_server( mas_rcontrol_t * prcontrol )
{
  if ( prcontrol )
  {
    if ( ctrl.restart )
      prcontrol->qbin = MSG_BIN_RESTART;
    else if ( ctrl.exit )
      prcontrol->qbin = MSG_BIN_QUIT;
    else
      prcontrol->qbin = MSG_BIN_DISCONNECT;
    prcontrol->keep_alive = 0;
  }
  ctrl.stop_listeners = 1;
  /* ctrl.watcher_stop = 1; */
}

static int
mas_proto_xcromas_evaluate_and_answer( mas_rcontrol_t * prcontrol, const char *question, mas_header_t * pheader )
{
  int r = -1;
  char *answer = NULL;

  prcontrol->qbin = MSG_BIN_NONE;
  answer = mas_evaluate_transaction_command( prcontrol, question );
  tMSG( "B(%d) Q(%d) SL(%d)", prcontrol->qbin, ctrl.do_exit, ctrl.stop_listeners );
  if ( ctrl.do_exit )
  {
    MAS_LOG( "qbin => %u", prcontrol->qbin );
    tMSG( "qbin => %u; ctrl.stop_listeners:%u", prcontrol->qbin, ctrl.stop_listeners );
    do_exit_server( prcontrol );
    tMSG( "qbin => %u; ctrl.stop_listeners:%u", prcontrol->qbin, ctrl.stop_listeners );
  }
  pheader->binary = prcontrol->qbin;
  HMSG( "ANSWERING" );
  /* if ( ( answer = mas_evaluate_cmd( 0 (* only_level *) , question, question (* args *) , 1 (*level *) , pheader, NULL, */
  /*                                   prcontrol, NULL ) ) )                                                              */
  if ( MAS_VALID_ANSWER( answer ) )
  {
    HMSG( "ANSWER: %lx:%s", ( unsigned long ) answer, answer );
    tMSG( "%s%ssign %s;answer is %s (%s)", pheader->binary ? "bin;" : "", pheader->new_opts ? "new opts;" : "",
          pheader->bad ? "BAD;" : "", pheader->sign == MSG_SIGNATURE ? "ok" : "bad", answer ? "+" : "-" );
    if ( pheader->bad )
    {
      r = -2;
      EMSG( "r:%d", r );
    }
    else
    {
      if ( pheader && MAS_VALID_ANSWER( answer ) && !pheader->binary )
        pheader->len = strlen( answer ) + 1;
      r = mas_channel_write_message( prcontrol->h.pchannel, answer, pheader );
      /* EMSG( "ANSWER: %d => %s", r, answer ); */
      if ( r < 0 )
      {
        P_ERR;
        EMSG( "r:%d", r );
      }
    }
    mas_free( answer );
    answer = NULL;
  }
  else
  {
    if ( answer && pheader )
      pheader->binary = MSG_BIN_UNKNOWN_COMMAND;
    r = mas_channel_write_message( prcontrol->h.pchannel, NULL, pheader );
    HMSG( "WRITTEN %d", r );
    if ( r < 0 )
    {
      P_ERR;
      EMSG( "r:%d", r );
    }
  }
  return r;
}

int
mas_proto_main( mas_rcontrol_t * prcontrol, mas_transaction_protodesc_t * proto_desc, const void *pheader_void )
{
  int r = -1;
  const mas_header_t *pheader_data = ( mas_header_t * ) pheader_void;

/* short: mas_channel_read_message */

  /* prcontrol->h.pchannel->buffer.maxread = 1028 * 4; */
  pheader_data = ( mas_header_t * ) mas_channel_buffer( prcontrol->h.pchannel, NULL );

  HMSG( "XCROMAS (%d) %lu", pheader_data ? 1 : 0, ( unsigned long ) prcontrol->h.pchannel->buffer.length );

  HMSG( "h:(%lu) got:%lu; h.len:%u", sizeof( mas_header_t ), prcontrol->h.pchannel->buffer.length, pheader_data ? pheader_data->len : 0 );

  /* if ( !pheader_data ) */
  /*   sleep( 2 );        */



  if ( prcontrol && pheader_data && pheader_data->sign == MSG_SIGNATURE )
  {
    mas_header_t header_copy;
    const char *question = NULL;

    /* if ( sizeof( mas_header_t ) + pheader_data->len < prcontrol->h.pchannel->buffer.length ) */
    /*   mas_channel_read_remainder( prcontrol->h.pchannel );                                   */


    while ( sizeof( mas_header_t ) + pheader_data->len < prcontrol->h.pchannel->buffer.length )
    {
      HMSG( "pheader_data->len:%u; buflen:%lu", pheader_data->len, prcontrol->h.pchannel->buffer.length );
      mas_channel_read_some( prcontrol->h.pchannel );
    }



    prcontrol->proto_desc = proto_desc;

    memset( &header_copy, 0, sizeof( header_copy ) );
    MAS_LOG( "xcromas session" );
    /* prcontrol->proto = MAS_TRANSACTION_PROTOCOL_XCROMAS; */
    if ( pheader_data )
    {
      header_copy = *pheader_data;
      question = ( char * ) ( pheader_data + 1 );
    }
    tMSG( "Q:%s", question && *question ? question : "-" );
    tMSG( "cl. q:%s from pid %lu", question && *question ? question : "-EMPTY-", ( unsigned long ) header_copy.pid );


    tMSG( "got msg from pid=%lu", ( unsigned long ) header_copy.pid );
    MAS_LOG( "xc (pid:%lu) Q: %s", ( unsigned long ) header_copy.pid, question && *question ? question : "-" );

    header_copy.new_opts = 0;
    if ( header_copy.sign != MSG_SIGNATURE )
    {
      EMSG( "header_copy sig : %lx", ( unsigned long ) header_copy.sign );
      header_copy.bad = 1;
    }
    {
      header_copy.done_cmd = 0;
      if ( header_copy.bad )
      {
        r = -2;
        EMSG( "r:%d", r );
      }
      else
      {
        tMSG( "q for e & a :%s", question );
        r = mas_proto_xcromas_evaluate_and_answer( prcontrol, question, &header_copy );
        tMSG( "e & a :%d", r );
      }
    }

    question = NULL;
  }
  else
  {
    r = 0;
  }
  return r;
}