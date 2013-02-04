#include "mas_basic_def.h"

#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <mastar/wrap/mas_memory.h>

#include "mas_common.h"
#include "log/inc/mas_log.h"

/* #include "channel/inc/mas_channel.h" */
#include "transaction/inc/mas_transaction.h"
#include "variables/inc/mas_variables.h"

#include "mas_http_utils.h"

/*
this:
  mas_http_utils.c
more:
  mas_transaction.c
  mas_http_get.c
  mas_http_get.h
  mas_http_reply.c
  mas_http_reply.h
  mas_http_request.c
  mas_http_request.h
  mas_http_types.h
  mas_http_utils.h
  mas_transaction_http.c
  mas_transaction_http.h

*/

char *
mas_proto_http_nonblank( const char *smessage, const char **ppsn )
{
  const unsigned char *ps, *pse;

  ps = ( unsigned char * ) smessage;
  while ( *ps && *ps <= ' ' )
    ps++;
  pse = ps;
  while ( *pse && *pse > ' ' )
    pse++;
  if ( ppsn )
  {
    const unsigned char *psn = NULL;

    psn = pse;
    while ( *psn && *psn <= ' ' )
      psn++;
    *ppsn = ( char * ) psn;
  }
  return mas_strndup( ( char * ) ps, pse - ps );
}

char *
mas_proto_http_nonc( const char *smessage, const char **ppsn, char *delim )
{
  const char *ps, *pse;

  ps = smessage;
  while ( *ps && strchr( delim, *ps ) )
    ps++;
  pse = ps;
  while ( *pse && !strchr( delim, *pse ) )
    pse++;
/* rMSG("WOW '%s'", ps); */
  if ( ppsn )
  {
    const char *psn = NULL;

    psn = pse;
    while ( *psn && strchr( delim, *psn ) )
      psn++;
    *ppsn = psn;
  }
  return mas_strndup( ps, pse - ps );
}


mas_http_t *
mas_proto_http_write( mas_http_t * http, char *cbuf, size_t sz )
{
  int w = -1;

  if ( http )
    w = mas_transaction_write( http->prcontrol, cbuf, sz );
  MAS_LOG( "written http %u of %lu", w, sz );
  if ( w > 0 )
    http->written += w;
  return http;
}

mas_http_t *
mas_proto_http_vwritef( mas_http_t * http, const char *fmt, va_list args )
{
  int w = -1;

  if ( http )
    w = mas_transaction_vwritef( http->prcontrol, fmt, args );
  if ( w > 0 )
    http->written += w;
  return http;
}

mas_http_t *
mas_proto_http_writef( mas_http_t * http, const char *fmt, ... )
{
  va_list args;

  va_start( args, fmt );
  if ( http )
    http = mas_proto_http_vwritef( http, fmt, args );
  va_end( args );
  return http;
}

mas_http_t *
mas_proto_http_write_pairs( mas_http_t * http, const char *set )
{
  if ( http )
  {
    char *buf;
    int bufsize = 1024 * 6;

    buf = mas_malloc( bufsize );

    if ( buf )
    {
      int wm = -1;

      wm = mas_variables_memory_write_pairs( http->outdata, set, buf, bufsize );
      if ( wm > 0 )
      {
        strcpy( buf + wm, "\r\n" );
        wm += 2;
        MAS_LOG( "w.pairs:[%s]", buf );
        http = mas_proto_http_write( http, buf, wm );
      }
      mas_free( buf );
    }
  }
  return http;
}

mas_http_t *
mas_proto_http_write_values( mas_http_t * http, const char *set )
{
  int w = -1;

  if ( http )
    w = mas_transaction_write_values( http->outdata, set, http->prcontrol );
  if ( w > 0 )
    http->written += w;
  return http;
}
