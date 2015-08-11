#include <string.h>

#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>
#include <errno.h>


#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

/* #include <mastar/tools/mas_arg_tools.h> */

#include "duf_maintenance.h"

#include "duf_utils.h"

#include "duf_config_ref.h"

/* ###################################################################### */
#include "duf_dh.h"
/* ###################################################################### */



int
duf_statat_dh( duf_dirhandle_t * pdhandle, const duf_dirhandle_t * pdhandleup, const char *name )
{
  int r = 0;
  int updfd = 0;

  updfd = pdhandleup ? pdhandleup->dfd : 0;
  if ( pdhandle && pdhandleup && name && pdhandleup->dfd )
  {
    r = fstatat( updfd, name, &pdhandle->st, AT_SYMLINK_NOFOLLOW | AT_NO_AUTOMOUNT );

    pdhandle->rs = r;
    if ( !pdhandle->rs )
      pdhandle->rs++;
    pdhandle->source = DUF_DH_SOURCE_FS;

    if ( r < 0 )
    {
      if ( errno == ENOENT )
      {
        DUF_SHOW_ERROR( "No such entry %s", name );
        DUF_MAKE_ERROR( r, DUF_ERROR_STATAT_ENOENT );
      }
      else
      {
        char serr[1024] = "";
        char *s;

        s = strerror_r( errno, serr, sizeof( serr ) );
        DUF_SHOW_ERROR( "(%d) errno:%d statat_dh :%s; name:'%s' ; at-dfd:%d", r, errno, s ? s : serr, name, updfd );
        DUF_MAKE_ERROR( r, DUF_ERROR_STATAT );
      }
    }
  }
  else
  {
    DUF_SHOW_ERROR( "parameter error pdhandle:%d; pdhandleup:%d; name:%d; pdhandleup->dfd:%d", pdhandle ? 1 : 0, pdhandleup ? 1 : 0,
                    name ? 1 : 0, pdhandleup && pdhandleup->dfd ? 1 : 0 );
    DUF_MAKE_ERROR( r, DUF_ERROR_STATAT );
  }
  return r;
}

int
duf_openat_dh( duf_dirhandle_t * pdhandle, const duf_dirhandle_t * pdhandleup, const char *name, int asfile )
{
  int r = 0;
  int updfd = 0;

  assert( pdhandle );
  assert( name );
  assert( *name );
  updfd = pdhandleup ? pdhandleup->dfd : 0;
  assert( pdhandle );
  assert( name );
  assert( updfd );
  if ( pdhandle && name && updfd )
  {
    const char *openname;

    if ( *name )
      openname = name;
    else
      openname = "/";
    if ( asfile )
      r = openat( updfd, name, O_NOFOLLOW | O_RDONLY );
    else
      r = openat( updfd, openname, O_DIRECTORY | O_NOFOLLOW | O_PATH | O_RDONLY );

    if ( r > 0 )
    {
      pdhandle->dfd = r;

      DUF_TRACE( explain, 5, "lowlev. opened (%d) ≪%s≫", pdhandle->dfd, name );

      /* r = fstatat( updfd, name, &pdhandle->st, AT_SYMLINK_NOFOLLOW | AT_NO_AUTOMOUNT ); */

      pdhandle->rs = r;
      if ( !pdhandle->rs )
        pdhandle->rs++;

      duf_config->nopen++;
      DUF_TRACE( fs, 5, "openated %s (%u - %u = %u) h%u", name, duf_config->nopen, duf_config->nclose,
                 duf_config->nopen - duf_config->nclose, pdhandle->dfd );
    }
    else if ( r < 0 && errno == ENOENT )
    {
      DUF_MAKE_ERROR( r, DUF_ERROR_OPENAT_ENOENT );
    }
    else if ( r < 0 )
    {
      char serr[1024] = "";
      char *s;

      s = strerror_r( errno, serr, sizeof( serr ) );
      DUF_SHOW_ERROR( "(%d) errno:%d openat_dh :%s; name:'%s' ; at-dfd:%d", r, errno, s ? s : serr, name, updfd );
      DUF_MAKE_ERROR( r, DUF_ERROR_OPENAT );
    }
  }
  else
  {
    DUF_SHOW_ERROR( "parameter error pdhandle:%d; name:%s; updfd:%d", pdhandle ? 1 : 0, name, updfd );
    DUF_MAKE_ERROR( r, DUF_ERROR_OPENAT );
  }
  return r;
}

int
duf_open_dh( duf_dirhandle_t * pdhandle, const char *path )
{
  int r = 0;

  if ( pdhandle && path )
  {
    r = open( path, O_DIRECTORY | O_NOFOLLOW | O_PATH | O_RDONLY );
    if ( r > 0 )
    {
      int rs = 0;

      pdhandle->dfd = r;

      rs = stat( path, &pdhandle->st );
      if ( rs < 0 )
        r = rs;
      pdhandle->rs = rs;
      if ( !pdhandle->rs )
        pdhandle->rs++;

      duf_config->nopen++;
      DUF_TRACE( fs, 5, "opened %s (%u - %u = %u)  h%u", path, duf_config->nopen, duf_config->nclose,
                 duf_config->nopen - duf_config->nclose, pdhandle->dfd );
      assert( pdhandle->dfd );
    }
    else if ( errno == ENOENT )
    {
      DUF_MAKE_ERROR( r, DUF_ERROR_OPEN_ENOENT );
    }
    else
    {
      char serr[512] = "";
      char *s;

      s = strerror_r( errno, serr, sizeof( serr ) );
      DUF_SHOW_ERROR( "(%d) errno:%d open_dh :%s; name:'%s'", r, errno, s ? s : serr, path );
      DUF_MAKE_ERROR( r, DUF_ERROR_OPEN );
    }
  }
  else if ( path )
  {
    r = 0;
    assert( pdhandle->dfd );
  }
  else
  {
    DUF_SHOW_ERROR( "parameter error pdhandle:%d; path:%d;", pdhandle ? 1 : 0, path ? 1 : 0 );
  }
  DUF_TRACE( fs, 5, "(%d)? opened %s", r, path );
  return r;
}

/* returns handle >0 */
int
duf_opened_dh( duf_dirhandle_t * pdhandle )
{
  int r = 0;

  if ( pdhandle )
    r = pdhandle->dfd;
  else
    DUF_MAKE_ERROR( r, DUF_ERROR_PTR );

  return r;
}

int
duf_close_dh( duf_dirhandle_t * pdhandle )
{
  int r = 0;

  assert( pdhandle );
  if ( pdhandle )
  {
    assert( pdhandle->dfd );
    if ( pdhandle->dfd )
    {
      r = close( pdhandle->dfd );
      DUF_TRACE( explain, 5, "lowlev. closed (%d)", pdhandle->dfd );
      if ( r )
      {
        {
          /* for debug only!!! */
          /* assert( pdhandle->dfd < 1000 ); */
        }

        DUF_SHOW_ERROR( "close dfd:%d", pdhandle->dfd );
        DUF_MAKE_ERROR( r, DUF_ERROR_CLOSE );
      }
      DUF_TRACE( fs, 5, "closed (%u - %u = %u)  h%u", duf_config->nopen, duf_config->nclose, duf_config->nopen - duf_config->nclose, pdhandle->dfd );
      duf_config->nclose++;
    }
    else
    {
      DUF_MAKE_ERROR( r, DUF_ERROR_NOT_OPEN );
      DUF_SHOW_ERROR( "parameter error pdhandleup->dfd:%d", pdhandle && pdhandle->dfd ? 1 : 0 );
    }

    pdhandle->dfd = 0;
  }
  else
    r = 0;
  return r;
}

int
duf_check_dh( const char *msg )
{
  int r = 0;

  DUF_TRACE( fs, 2, "%s (%u - %u = %u)", msg, duf_config->nopen, duf_config->nclose, duf_config->nopen - duf_config->nclose );
  return r;
}
