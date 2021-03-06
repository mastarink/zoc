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

static unsigned long open_serial = 0;

/* 20150820.142734 */
static int
_duf_statat_dh( duf_dirhandle_t * pdhandle, const duf_dirhandle_t * pdhandleup, const char *path )
{
  DEBUG_STARTR( r );

  if ( pdhandle && path /* && pdhandleup && pdhandleup->dfd */  )
  {
    int ry = 0;
    int updfd = 0;

    updfd = pdhandleup ? pdhandleup->dfd : 0;

    /* fstatat: If pathname is absolute, then dirfd is ignored. */
    ry = fstatat( updfd, path, &pdhandle->st, AT_SYMLINK_NOFOLLOW | AT_NO_AUTOMOUNT );
    DUF_TRACE( fs, 5, "lowlev. fstatated (%d) ≪%s≫", ry, path );

    if ( ry < 0 )
    {
      if ( errno == ENOENT )
      {
        DUF_SHOW_ERROR( "No such entry %s", path );
        DUF_MAKE_ERROR( r, DUF_ERROR_STATAT_ENOENT );
      }
      else
      {
        char serr[1024] = "";
        char *s;

        s = strerror_r( errno, serr, sizeof( serr ) );
        DUF_SHOW_ERROR( "(%d) errno:%d statat_dh :%s; path:'%s' ; at-dfd:%d", ry, errno, s ? s : serr, path, updfd );
        DUF_MAKE_ERROR( r, DUF_ERROR_STATAT );
      }
    }
    pdhandle->rs = ry;
    if ( !pdhandle->rs )
      pdhandle->rs++;
    if ( DUF_NOERROR( r ) )
      pdhandle->source = DUF_DH_SOURCE_FS;
  }
  else
  {
    DUF_SHOW_ERROR( "parameter error pdhandle:%d; pdhandleup:%d; path:%d; pdhandleup->dfd:%d", pdhandle ? 1 : 0, pdhandleup ? 1 : 0,
                    path ? 1 : 0, pdhandleup && pdhandleup->dfd ? 1 : 0 );
    DUF_MAKE_ERROR( r, DUF_ERROR_STATAT );
  }
  DEBUG_ENDR( r );
}

/* 20150820.142729 */
int
duf_statat_dh( duf_dirhandle_t * pdhandle, const duf_dirhandle_t * pdhandleup, const char *name )
{
  DEBUG_STARTR( r );

  if ( DUF_CONFIGG( cli.disable.flag.fs ) )
  {
    DUF_MAKE_ERROR( r, DUF_ERROR_FS_DISABLED );
  }
  else
  {
    DOR( r, _duf_statat_dh( pdhandle, pdhandleup, name ) );
  }
  DEBUG_ENDR( r );
}

/* 20150820.142723 */
static int
_duf_stat_dh( duf_dirhandle_t * pdhandle, const char *path )
{
  DEBUG_STARTR( r );

  if ( pdhandle && path )
  {
    int ry = 0;





    ry = stat( path, &pdhandle->st );
    DUF_TRACE( fs, 5, "lowlev. stated (%d) ≪%s≫", r, path );

    if ( ry < 0 )
    {
      if ( errno == ENOENT )
      {
        DUF_SHOW_ERROR( "No such entry %s", path );
        DUF_MAKE_ERROR( r, DUF_ERROR_STATAT_ENOENT );
      }
      else
      {
        char serr[1024] = "";
        char *s;

        s = strerror_r( errno, serr, sizeof( serr ) );
        DUF_SHOW_ERROR( "(%d) errno:%d statat_dh :%s; path:'%s'", ry, errno, s ? s : serr, path );
        DUF_MAKE_ERROR( r, DUF_ERROR_STAT );
      }
    }

    pdhandle->rs = ry;
    if ( !pdhandle->rs )
      pdhandle->rs++;
    if ( DUF_NOERROR( r ) )
      pdhandle->source = DUF_DH_SOURCE_FS;
  }
  else if ( !path )
  {
    DUF_SHOW_ERROR( "parameter error pdhandle:%d; path:%d;", pdhandle ? 1 : 0, path ? 1 : 0 );
    DUF_MAKE_ERROR( r, DUF_ERROR_STAT );
  }
  DUF_TRACE( fs, 5, "(%d)? stated %s", r, path );
  DEBUG_ENDR( r );
}

/* 20150820.142714 */
int
duf_stat_dh( duf_dirhandle_t * pdhandle, const char *path )
{
  DEBUG_STARTR( r );

  if ( !DUF_CONFIGG( cli.disable.flag.fs ) )
  {
    DOR( r, _duf_stat_dh( pdhandle, path ) );
  }
  DEBUG_ENDR( r );
}

/* 20150820.142710 */
static int
_duf_openat_dh( duf_dirhandle_t * pdhandle, const duf_dirhandle_t * pdhandleup, const char *name, int asfile )
{
  DEBUG_STARTR( r );
  int updfd = 0;

#ifdef DUF_TMP_ASSERT0
  assert( 0 );
#endif
  assert( pdhandle );
  assert( name );
  assert( *name );
  updfd = pdhandleup ? pdhandleup->dfd : 0;
  assert( pdhandle );
  assert( name );
  assert( updfd );
  if ( pdhandle && name && updfd )
  {
    int ry = 0;
    const char *openname;


    if ( *name )
      openname = name;
    else
      openname = "/";
    if ( asfile )
      ry = openat( updfd, name, O_NOFOLLOW | O_RDONLY );
    else
      ry = openat( updfd, openname, O_DIRECTORY | O_NOFOLLOW | O_PATH | O_RDONLY );
    DUF_TRACE( fs, 5, "lowlev. openated (%d) ≪%s≫", ry, name );

    if ( ry > 0 )
    {
      pdhandle->dfd = ry;
      pdhandle->serial = ++open_serial;
      DUF_TRACE( fs, 0, "@@@@@openat #%lu. %p : %d", pdhandle->serial, pdhandle, pdhandle->dfd );

      DUF_TRACE( explain, 5, "lowlev. opened (%d) ≪%s≫", pdhandle->dfd, name );

      /* ry = fstatat( updfd, name, &pdhandle->st, AT_SYMLINK_NOFOLLOW | AT_NO_AUTOMOUNT ); */

      pdhandle->rs = ry;
      if ( !pdhandle->rs )
        pdhandle->rs++;

      DUF_CONFIGW( nopen )++;
      DUF_TRACE( fs, 5, "openated %s (%u - %u = %u) h%u", name, DUF_CONFIGG( nopen ), DUF_CONFIGG( nclose ),
                 DUF_CONFIGG( nopen ) - DUF_CONFIGG( nclose ), pdhandle->dfd );
    }
    else if ( ry < 0 && errno == ENOENT )
    {
      DUF_MAKE_ERROR( r, DUF_ERROR_OPENAT_ENOENT );
    }
    else if ( ry < 0 )
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
  DEBUG_ENDR( r );
}


/* 20150820.142704 */
int
duf_openat_dh( duf_dirhandle_t * pdhandle, const duf_dirhandle_t * pdhandleup, const char *name, int asfile )
{
  DEBUG_STARTR( r );

  if ( DUF_CONFIGG( cli.disable.flag.fs ) )
  {
    DUF_MAKE_ERROR( r, DUF_ERROR_FS_DISABLED );
  }
  else
  {
    DOR( r, _duf_openat_dh( pdhandle, pdhandleup, name, asfile ) );
  }
  DEBUG_ENDR( r );
}

/* 20150820.142701 */
static int
_duf_open_dh( duf_dirhandle_t * pdhandle, const char *path )
{
  DEBUG_STARTR( r );

  /* assert( 0 ); */
  if ( pdhandle && path )
  {
    int ry = 0;

    ry = open( path, O_DIRECTORY | O_NOFOLLOW | O_PATH | O_RDONLY );
    DUF_TRACE( fs, 2, "lowlev. opened (%d) ≪%s≫", r, path );
    if ( ry >= 0 )
    {
      pdhandle->dfd = ry;
      pdhandle->serial = ++open_serial;
      DUF_TRACE( fs, 0, "@@@@@open #%lu. %p : %d", pdhandle->serial, pdhandle, pdhandle->dfd );
      DOR( r, duf_stat_dh( pdhandle, path ) );
    }
    else
    {
      if ( errno == ENOENT )
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
  }
  else if ( !path )
  {
    DUF_SHOW_ERROR( "parameter error pdhandle:%d; path:%d;", pdhandle ? 1 : 0, path ? 1 : 0 );
    DUF_MAKE_ERROR( r, DUF_ERROR_OPENAT );
  }
  DUF_TRACE( fs, 5, "(%d)? opened %s", r, path );
  DEBUG_ENDR( r );
}

/* 20150820.142754 */
int
duf_open_dh( duf_dirhandle_t * pdhandle, const char *path )
{
  DEBUG_STARTR( r );

  if ( DUF_CONFIGG( cli.disable.flag.fs ) )
  {
    DUF_MAKE_ERROR( r, DUF_ERROR_FS_DISABLED );
  }
  else
  {
    DOR( r, _duf_open_dh( pdhandle, path ) );
  }
  DEBUG_ENDR( r );
}

/* returns handle >0 */
static int
_duf_opened_dh( duf_dirhandle_t * pdhandle )
{
  DEBUG_STARTR( r );

  if ( pdhandle )
    r = pdhandle->dfd;
  else
    DUF_MAKE_ERROR( r, DUF_ERROR_PTR );

  DEBUG_ENDR( r );
}

int
duf_opened_dh( duf_dirhandle_t * pdhandle )
{
  DEBUG_STARTR( r );

  if ( DUF_CONFIGG( cli.disable.flag.fs ) )
  {
    DUF_MAKE_ERROR( r, DUF_ERROR_FS_DISABLED );
  }
  else
  {
    DOR( r, _duf_opened_dh( pdhandle ) );
  }
  DEBUG_ENDR( r );
}

int
_duf_close_dh( duf_dirhandle_t * pdhandle )
{
  DEBUG_STARTR( r );

  assert( pdhandle );
  if ( pdhandle )
  {
    assert( pdhandle->dfd );
    if ( pdhandle->dfd )
    {
      int ry = 0;

      ry = close( pdhandle->dfd );
      DUF_TRACE( fs, 0, "@@@@ #%lu. close %p : %d", pdhandle->serial, pdhandle, pdhandle->dfd );
      DUF_TRACE( explain, 5, "lowlev. closed (%d)", pdhandle->dfd );
      if ( ry )
      {
        {
          /* for debug only!!! */
          /* assert( pdhandle->dfd < 1000 ); */
        }
        {
          char serr[1024] = "";
          char *s;

          s = strerror_r( errno, serr, sizeof( serr ) );
          DUF_SHOW_ERROR( "(%d) errno:%d close :%s;  dfd:%d", ry, errno, s ? s : serr, pdhandle->dfd );
          DUF_MAKE_ERROR( r, DUF_ERROR_CLOSE );
          assert( 0 );
        }

        DUF_TEST_R( r );
      }
      DUF_TRACE( fs, 5, "closed (%u - %u = %u)  h%u", DUF_CONFIGG( nopen ), DUF_CONFIGG( nclose ), DUF_CONFIGG( nopen ) - DUF_CONFIGG( nclose ),
                 pdhandle->dfd );
      DUF_CONFIGW( nclose )++;
    }
    else
    {
      DUF_MAKE_ERROR( r, DUF_ERROR_NOT_OPEN );
      DUF_SHOW_ERROR( "parameter error pdhandleup->dfd:%d", pdhandle && pdhandle->dfd ? 1 : 0 );
    }

    DUF_TRACE( fs, 0, "@@@@ #%lu. closed %p : %d", pdhandle->serial, pdhandle, pdhandle->dfd );
    pdhandle->dfd = 0;
  }
  DEBUG_ENDR( r );
}

int
duf_close_dh( duf_dirhandle_t * pdhandle )
{
  DEBUG_STARTR( r );

  if ( DUF_CONFIGG( cli.disable.flag.fs ) )
  {
    DUF_MAKE_ERROR( r, DUF_ERROR_FS_DISABLED );
  }
  else
  {
    DOR( r, _duf_close_dh( pdhandle ) );
  }
  DEBUG_ENDR( r );
}

int
_duf_check_dh( const char *msg )
{
  DEBUG_STARTR( r );

  DUF_TRACE( fs, 2, "%s (%u - %u = %u)", msg, DUF_CONFIGG( nopen ), DUF_CONFIGG( nclose ), DUF_CONFIGG( nopen ) - DUF_CONFIGG( nclose ) );
  DEBUG_ENDR( r );
}

int
duf_check_dh( const char *msg )
{
  DEBUG_STARTR( r );

  /* if ( !DUF_CONFIGG(cli.disable.flag.fs ) ) */
  {
    DOR( r, _duf_check_dh( msg ) );
  }
  DEBUG_ENDR( r );
}
