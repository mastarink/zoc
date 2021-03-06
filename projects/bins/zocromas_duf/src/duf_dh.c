/* #undef MAS_TRACING */
#include <assert.h>
#include <string.h>

#include "duf_tracen_defs_preset.h"                                  /* MAST_TRACE_CONFIG; etc. ✗ */
#include "duf_errorn_defs_preset.h"                                  /* MAST_ERRORS_FILE; etc. ✗ */

#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

/* #include <mastar/wrap/mas_std_def.h> */
#include <mastar/trace/mas_trace.h>
#include <mastar/error/mas_error_defs_ctrl.h>
#include <mastar/error/mas_error_defs_make.h>
#include <mastar/error/mas_error_defs.h>

#include "duf_se_only.h"                                             /* Only DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ✗ */

#include "duf_status_ref.h"

#include "duf_config.h"                                              /* duf_get_config ✗ */

#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ✗ */
#include "duf_config_defs.h"                                         /* DUF_CONF... ✗ */

#include "duf_levinfo_dirhandle_structs.h"

/* ###################################################################### */
#include "duf_dh.h"                                                  /* duf_openat_dh; duf_open_dh; duf_opened_dh; duf_close_dh; duf_statat_dh; etc. ✗ */
/* ###################################################################### */

static unsigned long open_serial = 0;

/* 20150820.142734 */
static
SR( OTHER, statat_dh_i, duf_dirhandle_t * pdhandle, const duf_dirhandle_t * pdhandleup, const char *path )
{
/* DUF_STARTR( r ); */

  if ( pdhandle && path /* && pdhandleup && pdhandleup->dfd */  )
  {
    int ry = 0;
    int updfd = 0;

    updfd = pdhandleup ? pdhandleup->dfd : 0;

  /* fstatat: If pathname is absolute, then dirfd is ignored. */
    ry = fstatat( updfd, path, &pdhandle->st, AT_SYMLINK_NOFOLLOW | AT_NO_AUTOMOUNT );
    MAST_TRACE( fs, 5, "lowlev. fstatated (%d) ≪%s≫", ry, path );

    if ( ry < 0 )
    {
      if ( errno == ENOENT )
      {
      /* DUF_SHOW_ERROR( "No such entry %s", path ); */
        ERRMAKE_M( STATAT_ENOENT, "No such entry %s", path );
      }
      else
      {
        char serr[1024] = "";
        char *s;

        s = strerror_r( errno, serr, sizeof( serr ) );
      /* DUF_SHOW_ERROR( "(%d) errno:%d statat_dh :%s; path:'%s' ; at-dfd:%d", ry, errno, s ? s : serr, path, updfd ); */
        ERRMAKE_M( STATAT, "(%d) errno:%d statat_dh :%s; path:'%s' ; at-dfd:%d", ry, errno, s ? s : serr, path, updfd );
      }
    }
    pdhandle->rs = ry;
    if ( !pdhandle->rs )
      pdhandle->rs++;
    if ( QNOERR )
      pdhandle->source = DUF_DH_SOURCE_FS;
  }
  else
  {
  /* DUF_SHOW_ERROR( "parameter error pdhandle:%d; pdhandleup:%d; path:%d; pdhandleup->dfd:%d", pdhandle ? 1 : 0, pdhandleup ? 1 : 0, */
  /*                 path ? 1 : 0, pdhandleup && pdhandleup->dfd ? 1 : 0 );                                                           */
    ERRMAKE_M( STATAT, "parameter error pdhandle:%d; pdhandleup:%d; path:%d; pdhandleup->dfd:%d", pdhandle ? 1 : 0,
               pdhandleup ? 1 : 0, path ? 1 : 0, pdhandleup && pdhandleup->dfd ? 1 : 0 );
  }
/* DUF_ENDR( r ); */
  ER( OTHER, statat_dh_i, duf_dirhandle_t * pdhandle, const duf_dirhandle_t * pdhandleup, const char *path );
}

/* 20150820.142729 */
SR( OTHER, statat_dh, duf_dirhandle_t * pdhandle, const duf_dirhandle_t * pdhandleup, const char *name )
{
/* DUF_STARTR( r ); */

  if ( DUF_CONFIGG( opt.disable.flag.fs ) )
  {
    ERRMAKE( FS_DISABLED );
  }
  else
  {
    CR( statat_dh_i, pdhandle, pdhandleup, name );
  }
/* DUF_ENDR( r ); */
  ER( OTHER, statat_dh, duf_dirhandle_t * pdhandle, const duf_dirhandle_t * pdhandleup, const char *name );
}

/* 20150820.142723 */
static
SR( OTHER, stat_dh_i, duf_dirhandle_t * pdhandle, const char *path )
{
/* DUF_STARTR( r ); */

  if ( pdhandle && path )
  {
    int ry = 0;

    ry = stat( path, &pdhandle->st );
    MAST_TRACE( fs, 5, "lowlev. stated (%d) ≪%s≫", QERRIND, path );

    if ( ry < 0 )
    {
      if ( errno == ENOENT )
      {
      /* DUF_SHOW_ERROR( "No such entry %s", path ); */
        ERRMAKE_M( STATAT_ENOENT, "No such entry %s", path );
      }
      else
      {
        char serr[1024] = "";
        char *s;

        s = strerror_r( errno, serr, sizeof( serr ) );
      /* DUF_SHOW_ERROR( "(%d) errno:%d statat_dh :%s; path:'%s'", ry, errno, s ? s : serr, path ); */
        ERRMAKE_M( STAT, "(%d) errno:%d statat_dh :%s; path:'%s'", ry, errno, s ? s : serr, path );
      }
    }

    pdhandle->rs = ry;
    if ( !pdhandle->rs )
      pdhandle->rs++;
    if ( QNOERR )
      pdhandle->source = DUF_DH_SOURCE_FS;
  }
  else if ( !path )
  {
  /* DUF_SHOW_ERROR( "parameter error pdhandle:%d; path:%d;", pdhandle ? 1 : 0, path ? 1 : 0 ); */
    ERRMAKE_M( STAT, "parameter error pdhandle:%d; path:%d;", pdhandle ? 1 : 0, path ? 1 : 0 );
  }
  MAST_TRACE( fs, 5, "(%d)? stated %s", QERRIND, path );
/* DUF_ENDR( r ); */
  ER( OTHER, stat_dh_i, duf_dirhandle_t * pdhandle, const char *path );
}

/* 20150820.142714 */
SR( OTHER, stat_dh, duf_dirhandle_t * pdhandle, const char *path )
{
/* DUF_STARTR( r ); */

  if ( !DUF_CONFIGG( opt.disable.flag.fs ) )
  {
    CR( stat_dh_i, pdhandle, path );
  }
/* DUF_ENDR( r ); */
  ER( OTHER, stat_dh, duf_dirhandle_t * pdhandle, const char *path );
}

/* 20150820.142710 */
static
SR( OTHER, openat_dh_i, duf_dirhandle_t * pdhandle, const duf_dirhandle_t * pdhandleup, const char *name, int asfile )
{
/* DUF_STARTR( r ); */
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
    MAST_TRACE( fs, 5, "lowlev. openated (%d) ≪%s≫", ry, name );

    if ( ry > 0 )
    {
      pdhandle->dfd = ry;
      pdhandle->serial = ++open_serial;
      MAST_TRACE( fs, 5, "@@@@@openat #%lu. %p : %d", pdhandle->serial, pdhandle, pdhandle->dfd );

      MAST_TRACE( explain, 50, "lowlev. opened (%d) ≪%s≫", pdhandle->dfd, name );

    /* ry = fstatat( updfd, name, &pdhandle->st, AT_SYMLINK_NOFOLLOW | AT_NO_AUTOMOUNT ); */

      pdhandle->rs = ry;
      if ( !pdhandle->rs )
        pdhandle->rs++;

      global_status.dh.nopen++;
    /* DUF_CONFIGW( dh.nopen )++; */
      MAST_TRACE( fs, 5, "openated %s (%u - %u = %u) h%u", name, global_status.dh.nopen, global_status.dh.nclose,
                  global_status.dh.nopen - global_status.dh.nclose, pdhandle->dfd );
    }
    else if ( ry < 0 && errno == ENOENT )
    {
      ERRMAKE_M( OPENAT_ENOENT, "No such entry %s", name );
    }
    else if ( ry < 0 )
    {
      char serr[1024] = "";
      char *s;

      s = strerror_r( errno, serr, sizeof( serr ) );
    /* DUF_SHOW_ERROR( "(%d) errno:%d openat_dh :%s; name:'%s' ; at-dfd:%d", r, errno, s ? s : serr, name, updfd ); */
      ERRMAKE_M( OPENAT, "(%d) errno:%d openat_dh :%s; name:'%s' ; at-dfd:%d", QERRIND, errno, s ? s : serr, name, updfd );
    }
  }
  else
  {
  /* DUF_SHOW_ERROR( "parameter error pdhandle:%d; name:%s; updfd:%d", pdhandle ? 1 : 0, name, updfd ); */
    ERRMAKE_M( OPENAT, "parameter error pdhandle:%d; name:%s; updfd:%d", pdhandle ? 1 : 0, name, updfd );
  }
/* DUF_ENDR( r ); */
  ER( OTHER, openat_dh_i, duf_dirhandle_t * pdhandle, const duf_dirhandle_t * pdhandleup, const char *name, int asfile );
}

/* 20150820.142704 */
SR( OTHER, openat_dh, duf_dirhandle_t * pdhandle, const duf_dirhandle_t * pdhandleup, const char *name, int asfile )
{
/* DUF_STARTR( r ); */

  if ( DUF_CONFIGG( opt.disable.flag.fs ) )
  {
    ERRMAKE( FS_DISABLED );
  }
  else
  {
    CR( openat_dh_i, pdhandle, pdhandleup, name, asfile );
  }
/* DUF_ENDR( r ); */
  ER( OTHER, openat_dh, duf_dirhandle_t * pdhandle, const duf_dirhandle_t * pdhandleup, const char *name, int asfile );
}

/* 20150820.142701 */
static
SR( OTHER, open_dh_i, duf_dirhandle_t * pdhandle, const char *path )
{
/* DUF_STARTR( r ); */

/* assert( 0 ); */
  if ( pdhandle && path )
  {
    int ry = 0;

    ry = open( path, O_DIRECTORY | O_NOFOLLOW | O_PATH | O_RDONLY );
    MAST_TRACE( fs, 2, "lowlev. opened (%d) ≪%s≫", QERRIND, path );
    if ( ry >= 0 )
    {
      pdhandle->dfd = ry;
      pdhandle->serial = ++open_serial;
      MAST_TRACE( fs, 5, "@@@@@open #%lu. %p : %d", pdhandle->serial, pdhandle, pdhandle->dfd );
      CR( stat_dh, pdhandle, path );
    }
    else
    {
      if ( errno == ENOENT )
      {
        ERRMAKE_M( OPEN_ENOENT, "No such entry %s", path );
      }
      else
      {
        char serr[512] = "";
        char *s;

        s = strerror_r( errno, serr, sizeof( serr ) );
      /* DUF_SHOW_ERROR( "(%d) errno:%d open_dh :%s; name:'%s'", r, errno, s ? s : serr, path ); */
        ERRMAKE_M( OPEN, "(%d) errno:%d open_dh :%s; name:'%s'", QERRIND, errno, s ? s : serr, path );
      }
    }
  }
  else if ( !path )
  {
  /* DUF_SHOW_ERROR( "parameter error pdhandle:%d; path:%d;", pdhandle ? 1 : 0, path ? 1 : 0 ); */
    ERRMAKE_M( OPENAT, "parameter error pdhandle:%d; path:%d;", pdhandle ? 1 : 0, path ? 1 : 0 );
  }
  MAST_TRACE( fs, 5, "(%d)? opened %s", QERRIND, path );
/* DUF_ENDR( r ); */
  ER( OTHER, open_dh_i, duf_dirhandle_t * pdhandle, const char *path );
}

/* 20150820.142754 */
SR( OTHER, open_dh, duf_dirhandle_t * pdhandle, const char *path )
{
/* DUF_STARTR( r ); */

  if ( DUF_CONFIGG( opt.disable.flag.fs ) )
  {
    ERRMAKE( FS_DISABLED );
  }
  else
  {
    CR( open_dh_i, pdhandle, path );
  }
/* DUF_ENDR( r ); */
ER( OTHER, open_dh, duf_dirhandle_t * pdhandle, const char *path )}

/* returns handle >0 */
static
SR( OTHER, opened_dh_i, duf_dirhandle_t * pdhandle )
{
/* DUF_STARTR( r ); */

  if ( pdhandle )
    QERRIND = pdhandle->dfd;
  else
    ERRMAKE( PTR );

/* DUF_ENDR( r ); */
  ER( OTHER, opened_dh_i, duf_dirhandle_t * pdhandle );
}

SR( OTHER, opened_dh, duf_dirhandle_t * pdhandle )
{
/* DUF_STARTR( r ); */

  if ( DUF_CONFIGG( opt.disable.flag.fs ) )
  {
    ERRMAKE( FS_DISABLED );
  }
  else
  {
    CR( opened_dh_i, pdhandle );
  }
/* DUF_ENDR( r ); */
  ER( OTHER, opened_dh, duf_dirhandle_t * pdhandle );
}

static
SR( OTHER, close_dh_i, duf_dirhandle_t * pdhandle )
{
/* DUF_STARTR( r ); */

  assert( pdhandle );
  if ( pdhandle )
  {
    assert( pdhandle->dfd );
    if ( pdhandle->dfd )
    {
      int ry = 0;

      ry = close( pdhandle->dfd );
      MAST_TRACE( fs, 5, "@@@@ #%lu. close %p : %d", pdhandle->serial, pdhandle, pdhandle->dfd );
      MAST_TRACE( explain, 50, "lowlev. closed (%d)", pdhandle->dfd );
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
        /* DUF_SHOW_ERROR( "(%d) errno:%d close :%s;  dfd:%d", ry, errno, s ? s : serr, pdhandle->dfd ); */
          ERRMAKE_M( CLOSE, "(%d) errno:%d close :%s;  dfd:%d", ry, errno, s ? s : serr, pdhandle->dfd );
          assert( 0 );
        }

      /* DUF_TEST_R( r ); */
      }
      MAST_TRACE( fs, 5, "closed (%u - %u = %u)  h%u", global_status.dh.nopen, global_status.dh.nclose,
                  global_status.dh.nopen - global_status.dh.nclose, pdhandle->dfd );
      global_status.dh.nclose++;
    }
    else
    {
    /* DUF_SHOW_ERROR( "parameter error pdhandleup->dfd:%d", pdhandle && pdhandle->dfd ? 1 : 0 ); */
      ERRMAKE_M( NOT_OPEN, "parameter error pdhandleup->dfd:%d", pdhandle && pdhandle->dfd ? 1 : 0 );
    }

    MAST_TRACE( fs, 5, "@@@@ #%lu. closed %p : %d", pdhandle->serial, pdhandle, pdhandle->dfd );
    pdhandle->dfd = 0;
  }
/* DUF_ENDR( r ); */
  ER( OTHER, close_dh_i, duf_dirhandle_t * pdhandle );
}

SR( OTHER, close_dh, duf_dirhandle_t * pdhandle )
{
/* DUF_STARTR( r ); */

  if ( DUF_CONFIGG( opt.disable.flag.fs ) )
  {
    ERRMAKE( FS_DISABLED );
  }
  else
  {
    CR( close_dh_i, pdhandle );
  }
/* DUF_ENDR( r ); */
  ER( OTHER, close_dh, duf_dirhandle_t * pdhandle );
}

static
SR( OTHER, check_dh_i, const char *msg )
{
/* DUF_STARTR( r ); */

  MAST_TRACE( fs, 2, "%s (%u - %u = %u)", msg, global_status.dh.nopen, global_status.dh.nclose, global_status.dh.nopen - global_status.dh.nclose );
/* DUF_ENDR( r ); */
  ER( OTHER, check_dh_i, const char *msg );
}

SR( OTHER, check_dh, const char *msg )
{
/* DUF_STARTR( r ); */

/* if ( !DUF_CONFIGG(opt.disable.flag.fs ) ) */
  {
    CR( check_dh_i, msg );
  }
/* DUF_ENDR( r ); */
  ER( OTHER, check_dh, const char *msg );
}
