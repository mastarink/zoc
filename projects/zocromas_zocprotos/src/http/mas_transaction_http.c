#include <mastar/wrap/mas_std_def.h>
#include <mastar/types/mas_common_defs.h>


#include <string.h>
#include <stdlib.h>
#include <unistd.h>

/* #include <mastar/wrap/mas_memory.h> */

#include <mastar/msg/mas_msg_def.h>
#include <mastar/msg/mas_msg_tools.h>

#include <mastar/log/mas_log.h>

#include <mastar/modules/mas_modules_commands_eval.h>

#ifdef MAS_HTTP_USE_FILEINFO
#  include <mastar/fileinfo/mas_fileinfo.h>
#  include <mastar/fileinfo/mas_fileinfo_object.h>
#elif defined( MAS_HTTP_USE_AUTOOBJECT )
#  include <mastar/autoobject/mas_autoobject_object.h>
#  include <mastar/autoobject/mas_autoobject.h>
#endif

#include <mastar/listener/mas_listener_control.h>

#include "mas_http_request.h"
#include "mas_http_reply.h"
#include "mas_http_get.h"



/*
this:
  mas_transaction_http.c
related:
  mas_transaction.c
  mas_transaction_xcromas.c
more:
  mas_transaction_control.c
  mas_transaction_control.h
  mas_transaction_control_types.h
  mas_transaction.h
  mas_transaction_http.h
  mas_transaction_xcromas.h

*/



/*
 * 1. HTTP/1.protocolnum 200 OK\r\n
 * 2. Server:
 * 3. Date
 * 4.
  void header(char *code,char *message)
  {
    if (protocolnum == 1)
      out_puts("HTTP/1.0 ");
    else
      out_puts("HTTP/1.1 ");
    out_puts(code);
    out_puts(message);
    out_puts("\r\nServer: publicfile\r\nDate:");
    if (!httpdate(&nowstr,&now)) _exit(21);
    out_put(nowstr.s,nowstr.len);
    out_puts("\r\n");
  }
  if (protocolnum > 0) {
    tai_now(&now);
    header(code,message);
    out_puts("Content-Length: ");
    out_put(strnum,fmt_ulong(strnum,str_len(message) + 28));
    out_puts("\r\n");
    if (protocolnum == 2)
      out_puts("Connection: close\r\n");
    out_puts("Content-Type: text/html\r\n\r\n");
  }
  if (flagbody) {
    out_puts("<html><body>");
    out_puts(message);
    out_puts("</body></html>\r\n");
  }
  out_flush();
  if (protocolnum >= 2) {
    shutdown(1,1);
    sleep(1);
  }
*/



/* static mas_variables_list_head_t *proto_variables = NULL; */



#if 0
__attribute__ ( ( constructor ) )
     static void http_constructor( void )
{
  HMSG( "CONSTRUCTOR proto http" );
  ( void ) _mas_opts_restore_relative( mas_options_t * popts, "proto/http.conf", NULL /*popts */ , opt_table,
                                       sizeof( opt_table ) / sizeof( opt_table[0] ), NULL, NULL, NULL, NULL /* arg */  );
}

__attribute__ ( ( destructor ) )
     static void http_destructor( void )
{
  HMSG( "DESTRUCTOR proto http" );
  if ( proto_variables )
    mas_variables_delete( proto_variables );
}
#endif

/*
      OPTIONS
      GET
      HEAD
      POST
      PUT
      DELETE
      TRACE
      PROPFIND
      PROPPATCH
      MKCOL
      COPY
      MOVE
      LOCK
      UNLOCK
      ACL
      REPORT
      VERSION-CONTROL
      CHECKIN
      CHECKOUT
      UNCHECKOUT
      SEARCH
      MKWORKSPACE
      UPDATE
      LABEL
      MERG
      BASELINE-CONTROL
      MKACTIVITY
*/

static int
mas_proto_make( mas_rcontrol_t * prcontrol, mas_http_t * http )
{
  int r = -1;

  prcontrol->h.substatus = MAS_SUBSTATUS_MAKE;
  prcontrol->h.subpoint = __LINE__;
  MAS_LOG( "http: to make docroot" );
  if ( http )
    mas_http_make_docroot( prcontrol, http );
  prcontrol->h.subpoint = __LINE__;
  if ( http )
  {
    HMSG( "HTTP make URL %s", http->URI );
    if ( http->URI )
    {
#ifdef MAS_HTTP_USE_FILEINFO
      if ( 0 == strncmp( http->URI, "/xcromas/", 9 ) )
      {
        prcontrol->h.subpoint = __LINE__;
        HMSG( "HTTP make /xcromas" );
        MAS_LOG( "HTTP make /xcromas" );
        http->reply_content =
              mas_fileinfo_init( http->reply_content, http->docroot, http->URI, mas_evaluate_command_slash_plus,
                                 ( const void * ) prcontrol );
        MAS_LOG( "HTTP 1 make /xcromas" );
        mas_fileinfo_set_icontent_type( http->reply_content, MAS_CONTENT_TEXT );
        MAS_LOG( "HTTP / make /xcromas" );
      }
      else
#endif
      {
        prcontrol->h.subpoint = __LINE__;
        MAS_LOG( "HTTP make at docroot" );

        /* TODO : sendfile ; replace fileinfo  with autoobject lib */
#ifdef MAS_HTTP_USE_FILEINFO
        http->reply_content =
              mas_fileinfo_init( http->reply_content, http->docroot, http->URI, mas_load_filename_at_fd,
                                 ( const void * ) NULL /* prcontrol */  );
#elif defined( MAS_HTTP_USE_AUTOOBJECT )
        /* mas_autoobject_set_icontent_type( http->reply_content, MAS_CONTENT_TEXT ); */
        http->reply_content = mas_lcontrol_get_autoobject( prcontrol->plcontrol, http->docroot, http->URI );
#endif
        MAS_LOG( "HTTP / make at docroot" );
      }
    }
  }
  prcontrol->h.subpoint = __LINE__;
  MAS_LOG( "http: protocol-specific" );
  if ( http )
    switch ( http->imethod )
    {
    case MAS_HTTP_METHOD_BAD:
    case MAS_HTTP_METHOD_NONE:
      /* Not HTTP */
      mas_proto_http_delete_request( http );
      http = NULL;
      break;
    case MAS_HTTP_METHOD_PUT:
    case MAS_HTTP_METHOD_UNKNOWN:
      http->status_code = MAS_HTTP_CODE_NOT_IMPLEMENTED;
      http = mas_http_make_out_header( http, "Title", "%d %s", http->status_code, mas_http_status_code_message( prcontrol, http ) );
      http = mas_http_make_out_header_simple( http, "Allow", "GET,HEAD,OPTIONS" );
      http = mas_http_make_data_auto( prcontrol, http );
      /* http = mas_http_make_body_simple( prcontrol, http ); */
      break;
    case MAS_HTTP_METHOD_OPTIONS:
      http->status_code = MAS_HTTP_CODE_OK;
      http = mas_http_make_out_header_simple( http, "Allow", "GET,HEAD,OPTIONS" );
      break;
    case MAS_HTTP_METHOD_GET:
    case MAS_HTTP_METHOD_HEAD:
    case MAS_HTTP_METHOD_POST:
      MAS_LOG( "http: get/head/post" );
      break;
    }
  prcontrol->h.subpoint = __LINE__;



  /* if ( http )                                                */
  /*   http = mas_http_make_out_std_headers( prcontrol, http ); */
  /* if ( http && http->status_code == MAS_HTTP_CODE_NONE ) */
  /* {                                                      */
  /*   if ( http->reply_content && http->reply_content->filesize )      */
  /*     http->status_code = MAS_HTTP_CODE_OK;              */
  /*   else                                                 */
  /*     http->status_code = MAS_HTTP_CODE_NOT_FOUND;       */
  /* }                                                      */

  if ( http )
    http = mas_http_reply( prcontrol, http );
  prcontrol->h.substatus = MAS_SUBSTATUS_MAKE;
  prcontrol->h.subpoint = __LINE__;
  HMSG( "WRITTEN %lu", http ? http->written : 0 );
  MAS_LOG( "WRITTEN %lu", http ? http->written : 0 );
  r = http ? 1 : 0;
  if ( http )
    mas_proto_http_delete_request( http );
  prcontrol->h.subpoint = __LINE__;
  prcontrol->h.subresult1 = http->written_header;
  prcontrol->h.subresult2 = http->written_body;
  return r;
}

int
mas_proto_main( mas_rcontrol_t * prcontrol, const void *place_holder )
{
  int r = 0;
  mas_http_t *http = NULL;

  /* const char *string = ( const char * ) place_holder; */

//  GET / HTTP/1.1
//  Host: mastarink.net:5002
//  User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:10.0.11) Gecko/20100101 Firefox/10.0.11
//  Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8
//  Accept-Language: uk,ru;q=0.8,en-us;q=0.5,en;q=0.3
//  Accept-Encoding: gzip, deflate
//  DNT: 1
//  Connection: keep-alive
//  Referer: http://mastarink.net:5003/
//  If-None-Match: 26555b-fd-4aaaaa685c746
//  
/*
 * The Do Not Track (DNT) header
*/
  HMSG( "HTTP main" );
  /* MAS_LOG( "http?: to create rq :(%lu) %s", strlen( string ), string ); */
  MAS_LOG( "http?: to create rq" );
  http = mas_proto_http_create_request( prcontrol );
  prcontrol->h.substatus = MAS_SUBSTATUS_INIT;
  MAS_LOG( "http?: to parse rq" );
  if ( http )
    http = mas_proto_http_parse_request( prcontrol, http );
  MAS_LOG( "http?: parsed rq : %s", prcontrol->proto_desc ? prcontrol->proto_desc->name : "?" );

  if ( http )
    r = mas_proto_make( prcontrol, http );
  HMSG( "HTTP r:%d", r );
  prcontrol->h.substatus = MAS_SUBSTATUS_END;
  return r;
}
