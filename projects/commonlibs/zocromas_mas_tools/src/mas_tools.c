/* tools */
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>

#include <errno.h>
#include <fcntl.h>

/*                                       */
/* #include <mastar/msg/mas_msg_def.h>   */
/* #include <mastar/msg/mas_msg_tools.h> */
/*                                       */
/*                                       */


#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_lib.h>
#include <mastar/wrap/mas_memory.h>

/* #include "mas_arg_tools.h" */

/* #include <mastar/types/mas_opts_types.h> */



/* ########################################################################################## */
#include "mas_tools.h"
/* ########################################################################################## */


/*
this:
  mas_tools.c
related:
  mas_tools_def.h
  mas_msg_tools.c
  mas_thread_tools.c
  mas_memory.c
  mas_cs.c
  mas_cs.h
  mas_control.c
  mas_opts.c

*/


size_t
mas_xvsnprintf( char *str, size_t size, const char *format, va_list args )
{
  size_t r = 0;

  r = vsnprintf( str, size, format, args );
  return r;
}


char *
mas_dump1( const char *data, size_t len, size_t perline )
{
  char *buffer = NULL;
  char *pbuf;
  ssize_t dsz;

  if ( !perline )
    perline = 16;

  if ( data )
  {
    size_t lines;

    lines = len / perline + 1;
    dsz = lines * ( perline + 1 ) * 5 + 1;
    pbuf = buffer = mas_malloc( dsz );
    for ( size_t icg = 0; icg < len; icg += perline )
    {
      /* size_t llen;                                           */
      /*                                                        */
      /* llen = ( len - icg >= perline ? perline : len - icg ); */
      for ( size_t ic = 0; ic < perline; ic++ )
      {
        char b;

        if ( icg + ic >= len )
        {
          snprintf( pbuf, dsz + buffer - pbuf, "   " );
        }
        else
        {
          b = data[icg + ic];
          snprintf( pbuf, dsz + buffer - pbuf, " %02x", b );
        }
        pbuf += 3;
      }
      snprintf( pbuf, dsz + buffer - pbuf, " : " );
      pbuf += 3;
      for ( size_t ic = 0; ic < perline; ic++ )
      {
        char b;

        if ( icg + ic >= len )
        {
          b = 0;
          snprintf( pbuf, dsz + buffer - pbuf, "  " );
        }
        else
        {
          b = data[icg + ic];
          snprintf( pbuf, dsz + buffer - pbuf, " %c", b >= ' ' && b <= 'z' ? b : '.' );
        }
        pbuf += 2;
      }
      if ( icg + perline < len )
        snprintf( pbuf, dsz + buffer - pbuf, "\n" );
      pbuf += 1;
    }
  }
  return buffer;
}

char *
mas_dump2( const char *data, size_t len, size_t perline )
{
  char *buffer = NULL;
  char *pbuf;
  ssize_t dsz;

  if ( !perline )
    perline = 16;

  if ( data )
  {
    size_t lines;

    lines = len / perline + 1;
    dsz = lines * ( perline + 1 ) * 6 + 1;
    pbuf = buffer = mas_malloc( dsz );
    for ( size_t icg = 0; icg < len; icg += perline )
    {
      /* size_t llen;                                           */
      /*                                                        */
      /* llen = ( len - icg >= perline ? perline : len - icg ); */
      for ( size_t ic = 0; ic < perline; ic++ )
      {
        char b;

        if ( icg + ic >= len )
        {
          snprintf( pbuf, dsz + buffer - pbuf, "   " );
        }
        else
        {
          b = data[icg + ic];
          snprintf( pbuf, dsz + buffer - pbuf, " %02x", b );
        }
        pbuf += 3;
      }
      snprintf( pbuf, dsz + buffer - pbuf, "\n" );
      pbuf += 1;
      for ( size_t ic = 0; ic < perline; ic++ )
      {
        char b;

        if ( icg + ic >= len )
        {
          b = 0;
          snprintf( pbuf, dsz + buffer - pbuf, "   " );
        }
        else
        {
          b = data[icg + ic];
          snprintf( pbuf, dsz + buffer - pbuf, "  %c", b >= ' ' && b <= 'z' ? b : '.' );
        }
        pbuf += 3;
      }
      if ( icg + perline < len )
        snprintf( pbuf, dsz + buffer - pbuf, "\n" );
      pbuf += 1;
    }
  }
  return buffer;
}

char *
mas_escape( char *str )
{
  size_t len;
  char *ns = NULL;
  char *ip = NULL;
  char *op = NULL;

  len = strlen( str ) + 1;
  ns = mas_malloc( len );
  ip = str;
  op = ns;
  /* assert( op >= ns ); */
  while ( str && *str && ( ( size_t ) ( op - ns ) ) < len )
  {
    if ( *ip == '\\' )
    {
      ip++;
      if ( *ip == 'x' )
      {
        unsigned c;

        ip++;
        sscanf( ip, "%2x", &c );
        *op++ = ( char ) c;
        ip += 2;
      }
      else if ( *ip == 'd' )
      {
        unsigned c;

        ip++;
        sscanf( ip, "%2u", &c );
        *op++ = ( char ) c;
        ip += 2;
      }
      else if ( *ip == 'n' )
      {
        *op++ = '\n';
        ip++;
      }
      else if ( *ip == 'r' )
      {
        *op++ = '\r';
        ip++;
      }
      else if ( *ip == 't' )
      {
        *op++ = '\t';
        ip++;
      }
    }
    else
    {
      *op++ = *ip++;
    }
  }
  return ns;
}

char *
mas_ip_string( void *sin_addr )
{
  char ip[128];
  const char *pip;

  pip = mas_inet_ntop( AF_INET, sin_addr, ip, sizeof( ip ) );
  return pip ? mas_strdup( pip ) : NULL;
}

size_t
mas_parse_host_port( const char *host, unsigned *phport, unsigned default_port )
{
  const char *semi = NULL;
  size_t len = 0;
  unsigned hport = 0;

  semi = strchr( host, ':' );
  if ( semi )
  {
    len = semi - host;
    sscanf( semi + 1, "%u", &hport );
  }
  else
  {
    len = strlen( host );
  }
  if ( !hport )
    hport = default_port;
  /* if ( !hport )                */
  /*   hport = opts.default_port; */
  if ( phport )
    *phport = hport;
  /* ? hport : ( port ? port : opts.default_port ) */
  return len;
}

ssize_t
mas_load_stat( int fd, size_t * ptruesize, ino_t * ptrueinode, time_t * ptruefiletime )
{
  struct stat st;
  size_t size = 0;
  int r = -1;

  if ( fd > 0 )
  {
    r = fstat( fd, &st );
    if ( r >= 0 )
    {
      if ( S_ISREG( st.st_mode ) )
      {
        size = st.st_size;
        if ( ptruesize )
          *ptruesize = size;
      }
      if ( ptrueinode )
        *ptrueinode = st.st_ino;
      if ( ptruefiletime )
        *ptruefiletime = st.st_mtime;
    }
  }
  if ( r < 0 )
    size = -1;
  return size;
}

char *
mas_load_file( FILE * file, size_t size, size_t * ptruesize, ino_t * ptrueinode, time_t * ptruefiletime, const void *arg
               __attribute__ ( ( unused ) ) )
{
  char *filedata = NULL;

  /* rMSG( "TO load %s : %s", filepath, file ? "OPENED" : "NOT opened" ); */
  if ( file )
  {
    ssize_t r;

    if ( !size )
    {
      size = mas_load_stat( fileno( file ), ptruesize, ptrueinode, ptruefiletime );
      /* struct stat st;                   */
      /*                                   */
      /* r = fstat( fileno( file ), &st ); */
      /* if ( r >= 0 )                     */
      /* {                                 */
      /*   if ( S_ISREG( st.st_mode ) )    */
      /*     size = st.st_size;            */
      /*   if ( ptruesize )                */
      /*     *ptruesize = size;            */
      /*   if ( ptrueinode )               */
      /*     *ptrueinode = st.st_ino;      */
      /*   if ( ptruefiletime )            */
      /*     *ptruefiletime = st.st_mtime; */
      /* }                                 */
    }
    if ( size > 0 )
    {
      filedata = mas_malloc( size );
      r = fread( filedata, 1, size, file );
      if ( r >= 0 && ptruesize )
        *ptruesize = r;
    }
  }
  else
  {
    /* MAS_LOGERR( errno, "file %s not loaded", filepath ); */
    /* errno = 0; */
  }
  return filedata;
}

char *
mas_load_fd( int fd, size_t size, size_t * ptruesize, ino_t * ptrueinode, time_t * ptruefiletime, const void *arg __attribute__ ( ( unused ) ) )
{
  char *filedata = NULL;

  if ( fd )
  {
    ssize_t r;

    if ( !size )
    {
      size = mas_load_stat( fd, ptruesize, ptrueinode, ptruefiletime );
      /* struct stat st;                        */
      /*                                        */
      /* r = fstat( fd, &st );                  */
      /* if ( r >= 0 && S_ISREG( st.st_mode ) ) */
      /*   size = st.st_size;                   */
    }
    if ( size > 0 )
    {
      filedata = mas_malloc( size );
      r = read( fd, filedata, size );
      if ( r >= 0 && ptruesize )
        *ptruesize = r;
    }
  }
  else
  {
    /* MAS_LOGERR( errno, "file %s not loaded", filepath ); */
    /* errno = 0; */
  }
  return filedata;
}

char *
mas_load_filename_file( const char *filepath, size_t size, size_t * ptruesize, ino_t * ptrueinode, time_t * ptruefiletime, const void *arg )
{
  char *filedata = NULL;
  FILE *file = NULL;

  file = fopen( filepath, "r" );
  /* rMSG( "TO load %s : %s", filepath, file ? "OPENED" : "NOT opened" ); */
  if ( file )
  {
    filedata = mas_load_file( file, size, ptruesize, ptrueinode, ptruefiletime, arg );
    fclose( file );
  }
  else
  {
    /* MAS_LOGERR( errno, "file %s not loaded", filepath ); */
    /* errno = 0; */
  }
  return filedata;
}

char *
mas_load_filename_fd( const char *filepath, size_t size, size_t * ptruesize, ino_t * ptrueinode, time_t * ptruefiletime, const void *arg )
{
  char *filedata = NULL;
  int fd;

  fd = open( filepath, O_RDONLY );
  /* rMSG( "TO load %s : %s", filepath, fd > 0 ? "OPENED" : "NOT opened" ); */
  if ( fd )
  {
    filedata = mas_load_fd( fd, size, ptruesize, ptrueinode, ptruefiletime, arg );
    close( fd );
  }
  else
  {
    /* MAS_LOGERR( errno, "file %s not loaded", filepath ); */
    /* errno = 0; */
  }
  return filedata;
}

void
mas_common_constructor( const char *fname, int lin __attribute__ ( ( unused ) ), int lev )
{
  fprintf( stderr, "\n%s CONstructor %s e%d @", lev == 0 ? "--------------------" : "********************", fname, errno );
}

void
mas_common_destructor( const char *fname, int lin __attribute__ ( ( unused ) ), int lev )
{
  fprintf( stderr, "\n%s DEstructor %s e%d @", lev == 0 ? "--------------------" : "********************", fname, errno );
}
