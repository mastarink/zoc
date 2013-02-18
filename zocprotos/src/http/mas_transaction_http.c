#include <mastar/wrap/mas_std_def.h>

#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_tools.h>

#include <mastar/log/mas_log.h>
#include <mastar/thtools/mas_ocontrol_tools.h>

#include <mastar/modules/mas_modules_commands_eval.h>

#include <mastar/fileinfo/mas_fileinfo.h>
#include <mastar/fileinfo/mas_fileinfo_object.h>

#include <mastar/variables/mas_variables.h>


#include "mas_http_request.h"
#include "mas_http_reply.h"
#include "mas_http_get.h"

#include "mas_transaction_http.h"


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

int
mas_proto_main( mas_rcontrol_t * prcontrol, const mas_transaction_protodesc_t *proto_desc, const void *string_void )
{
  int w = 0;
  mas_http_t *http = NULL;
  const char *string = ( const char * ) string_void;

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

  /* MAS_LOG( "http?: to create rq :(%lu) %s", strlen( string ), string ); */
  MAS_LOG( "http?: to create rq" );
  http = mas_proto_http_create_request( prcontrol );
  MAS_LOG( "http?: to parse rq" );
  if ( http )
    http = mas_proto_http_parse_request( prcontrol, proto_desc, http, string );
  MAS_LOG( "http?: parsed rq : %s", prcontrol && prcontrol->proto_desc ? prcontrol->proto_desc->name : "?" );

  MAS_LOG( "http: to make docroot" );
  if ( http )
    mas_http_make_docroot( prcontrol, http );
  if ( http )
  {
    if ( http->URI && 0 == strncmp( http->URI, "/xcromas/", 9 ) )
    {
      /* char *answer = NULL;                                                                                                  */
      /*                                                                                                                       */
      /* answer = mas_evaluate_command_slash( http->URI + 9 );                                                                 */
      /* if ( answer )                                                                                                         */
      /* {                                                                                                                     */
      /*   _mas_fileinfo_link_dataz( http->content, answer );                                                                  */
      /*   http->outdata =                                                                                                     */
      /*         mas_variable_create_text( http->outdata, MAS_THREAD_TRANSACTION, "header", "Content-Type", "text/plain", 0 ); */
      /* }                                                                                                                     */
      http->content =
            mas_fileinfo_init( http->content, http->docroot, http->URI, mas_evaluate_command_slash_plus, ( const void * ) prcontrol );
      mas_fileinfo_set_icontent_type( http->content, MAS_CONTENT_TEXT );
    }
    else
      http->content = mas_fileinfo_init( http->content, http->docroot, http->URI, mas_load_file, ( const void * ) NULL /* prcontrol */  );
  }
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
      MAS_LOG( "http: get/head :(%lu) %s", strlen( string ), string );
      http = mas_http_make_out_head_get( prcontrol, http );
      break;
    }


  if ( http )
    http = mas_http_make_etag( prcontrol, http );

  /* if ( http )                                                */
  /*   http = mas_http_make_out_std_headers( prcontrol, http ); */
  /* if ( http && http->status_code == MAS_HTTP_CODE_NONE ) */
  /* {                                                      */
  /*   if ( http->content && http->content->filesize )      */
  /*     http->status_code = MAS_HTTP_CODE_OK;              */
  /*   else                                                 */
  /*     http->status_code = MAS_HTTP_CODE_NOT_FOUND;       */
  /* }                                                      */

  if ( http )
    http = mas_http_reply( prcontrol, http );
  MAS_LOG( "WRITTEN %lu", http ? http->written : 0 );
  if ( http )
    mas_proto_http_delete_request( http );

  /* if ( w < 0 )           */
  /* {                      */
  /*   EMSG( "BAD %d", w ); */
  /* }                      */
  return http ? w : 0;
}