#include <string.h>
#include <assert.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_maintenance.h"

#include "duf_levinfo_ref_def.h"
#include "duf_levinfo_context.h"
#include "duf_context.h"

#include "duf_pathinfo_ref.h"
/* ###################################################################### */
#include "duf_levinfo_ref.h"
/* ###################################################################### */


/************************************************************************/
duf_levinfo_t *
duf_levinfo_ptr_d( const duf_depthinfo_t * pdi, int d )
{
  assert( pdi );
  assert( pdi->inited );
#if 0
  assert( d >= 0 );
  assert( pdi->pathinfo.levinfo );
  return d >= 0 && pdi ? &pdi->pathinfo.levinfo[d] : NULL;
#else
  return ( d <= pdi->pathinfo.maxdepth ) ? duf_pi_ptr_d( &pdi->pathinfo, d ) : NULL;
#endif
}
/* *INDENT-OFF*  */
DUF_LEVINFO_FC_REF( duf_levinfo_t , ptr )
DUF_LEVINFO_FC_UP_REF( duf_levinfo_t , ptr )
/* *INDENT-ON*  */

/************************************************************************/


/* int                                                         */
/* duf_levinfo_is_leaf_d( const duf_depthinfo_t * pdi, int d ) */
/* {                                                           */
/*   return duf_levinfo_ptr_d( pdi, d )->is_leaf ? 1 : 0;      */
/* }                                                           */
/* (* *INDENT-OFF*  *)                                         */
/* DUF_LEVINFO_FC( int, is_leaf )                              */
/* DUF_LEVINFO_FC_UP( int, is_leaf )                           */
/* (* *INDENT-ON*  *)                                          */

DUF_LEVINFO_3GET_BOOL( int, is_leaf, is_leaf );

/************************************************************************/


struct stat *
duf_levinfo_stat_d( const duf_depthinfo_t * pdi, int d )
{
  struct stat *pst = NULL;

  /* if ( pdi->opendir ) */
  {
    if ( duf_levinfo_ptr_d( pdi, d )->lev_dh.rs > 0 && duf_levinfo_ptr_d( pdi, d )->lev_dh.source == DUF_DH_SOURCE_FS )
      pst = &duf_levinfo_ptr_d( pdi, d )->lev_dh.st;
  }
  /* TODO NOT here: assert( pst->st_dev ); 
   * (used to check presence, so no stat is OK)
   * TODO */
  return pst;
}
/* *INDENT-OFF*  */
DUF_LEVINFO_FC_REF( struct stat, stat )
DUF_LEVINFO_FC_UP_REF( struct stat, stat )
/* *INDENT-ON*  */


DUF_LEVINFO_ST_FLD_NAME( ino, inode );
DUF_LEVINFO_ST_FLD( dev );
DUF_LEVINFO_ST_FLD( mode );
DUF_LEVINFO_ST_FLD( nlink );
DUF_LEVINFO_ST_FLD( uid );
DUF_LEVINFO_ST_FLD( gid );
DUF_LEVINFO_ST_TYP_FLD( blkcnt, blocks );
DUF_LEVINFO_ST_FLD( blksize );
DUF_LEVINFO_ST_TYP_FLD( off, size );
DUF_LEVINFO_ST_TYP_FLD_NAME( time, atime, asec );
DUF_LEVINFO_ST_TYP_FLD_NAME( time, mtime, msec );
DUF_LEVINFO_ST_TYP_FLD_NAME( time, ctime, csec );
DUF_LEVINFO_ST_TYP_FLD_NAME( long, atim.tv_nsec, ansec );
DUF_LEVINFO_ST_TYP_FLD_NAME( long, mtim.tv_nsec, mnsec );
DUF_LEVINFO_ST_TYP_FLD_NAME( long, ctim.tv_nsec, cnsec );


struct stat *
duf_levinfo_dbstat_d( const duf_depthinfo_t * pdi, int d )
{
  struct stat *pst = NULL;

  /* if ( pdi->opendir ) */
  {
    if ( duf_levinfo_ptr_d( pdi, d )->lev_dh.rdb > 0 && duf_levinfo_ptr_d( pdi, d )->lev_dh.source == DUF_DH_SOURCE_DB )
      pst = &duf_levinfo_ptr_d( pdi, d )->lev_dh.st;
  }
  /* TODO NOT here: assert( pst->st_dev ); 
   * (used to check presence, so no stat is OK)
   * TODO */
  return pst;
}
/* *INDENT-OFF*  */
DUF_LEVINFO_FC_REF( struct stat, dbstat )
DUF_LEVINFO_FC_UP_REF( struct stat, dbstat )
/* *INDENT-ON*  */


DUF_LEVINFO_DBST_FLD_NAME( ino, inode );
DUF_LEVINFO_DBST_FLD( dev );
DUF_LEVINFO_DBST_FLD( mode );
DUF_LEVINFO_DBST_FLD( nlink );
DUF_LEVINFO_DBST_FLD( uid );
DUF_LEVINFO_DBST_FLD( gid );
DUF_LEVINFO_DBST_TYP_FLD( blkcnt, blocks );
DUF_LEVINFO_DBST_FLD( blksize );
DUF_LEVINFO_DBST_TYP_FLD( off, size );
DUF_LEVINFO_DBST_TYP_FLD_NAME( time, atime, asec );
DUF_LEVINFO_DBST_TYP_FLD_NAME( time, mtime, msec );
DUF_LEVINFO_DBST_TYP_FLD_NAME( time, ctime, csec );
DUF_LEVINFO_DBST_TYP_FLD_NAME( long, atim.tv_nsec, ansec );
DUF_LEVINFO_DBST_TYP_FLD_NAME( long, mtim.tv_nsec, mnsec );
DUF_LEVINFO_DBST_TYP_FLD_NAME( long, ctim.tv_nsec, cnsec );





/* TODO st_nlink, st_uid, st_gid, st_blksize, st_blocks, st_?tim */
/************************************************************************/

/* int                                                              */
/* duf_levinfo_item_deleted_d( const duf_depthinfo_t * pdi, int d ) */
/* {                                                                */
/*                                                                  */
/*   return duf_levinfo_ptr_d( pdi, d )->deleted ? 1 : 0;           */
/* }                                                                */
/*                                                                  */
/* (* *INDENT-OFF*  *)                                              */
/* DUF_LEVINFO_FC( int, item_deleted )                              */
/* DUF_LEVINFO_FC_UP( int, item_deleted )                           */
/* (* *INDENT-ON*  *)                                               */

/* *INDENT-OFF*  */
DUF_LEVINFO_3GET_BOOL( int, item_deleted, deleted )
/* *INDENT-ON*  */

/************************************************************************/

const char *
duf_levinfo_itemshowname_d( const duf_depthinfo_t * pdi, int d )
{
  const char *n = NULL;

  n = duf_levinfo_ptr_d( pdi, d )->itemname;
  /* return n ? ( *n ? n : "/" ) : n; */
  return n && !*n ? "/" : n;
}

/* *INDENT-OFF*  */
DUF_LEVINFO_FC_REF( const char, itemshowname )
DUF_LEVINFO_FC_UP_REF( const char, itemshowname )
/* *INDENT-ON*  */

const char *
duf_levinfo_itemshowname_q( const duf_depthinfo_t * pdi, const char *q )
{
  const char *p;

  p = duf_levinfo_itemshowname( pdi );
  return p ? p : q;
}

const char *
duf_levinfo_itemtruename_d( const duf_depthinfo_t * pdi, int d )
{
  const char *n = NULL;

  n = duf_levinfo_ptr_d( pdi, d )->itemname;
  /* return n ? ( *n ? n : "/" ) : n; */
  return n;
}

/* *INDENT-OFF*  */
DUF_LEVINFO_FC_REF( const char, itemtruename )
DUF_LEVINFO_FC_UP_REF( const char, itemtruename )
/* *INDENT-ON*  */

const char *
duf_levinfo_itemtruename_q( const duf_depthinfo_t * pdi, const char *q )
{
  const char *p;

  p = duf_levinfo_itemtruename( pdi );
  return p ? p : q;
}

/************************************************************************/

/* SET */
/* void                                                                                */
/* duf_levinfo_set_items_files_d( duf_depthinfo_t * pdi, unsigned long long n, int d ) */
/* {                                                                                   */
/*   duf_levinfo_ptr_d( pdi, d )->items.files = n;                                     */
/* }                                                                                   */
/*                                                                                     */
/* (* *INDENT-OFF*  *)                                                                 */
/* DUF_LEVINFO_FS( unsigned long long, items_files )                                   */
/* DUF_LEVINFO_FS_UP( unsigned long long, items_files )                                */
/* (* *INDENT-ON*  *)                                                                  */

/* *INDENT-OFF*  */
DUF_LEVINFO_3SET( unsigned long long, items_files, items.files )
/* *INDENT-ON*  */

/************************************************************************/
/* unsigned long long                                              */
/* duf_levinfo_items_files_d( const duf_depthinfo_t * pdi, int d ) */
/* {                                                               */
/*   return duf_levinfo_ptr_d( pdi, d )->items.files;              */
/* }                                                               */
/*                                                                 */
/* (* *INDENT-OFF*  *)                                             */
/* DUF_LEVINFO_FC( unsigned long long, items_files )               */
/* DUF_LEVINFO_FC_UP( unsigned long long, items_files )            */
/* (* *INDENT-ON*  *)                                              */

/* *INDENT-OFF*  */
DUF_LEVINFO_3GET( unsigned long long, items_files, items.files )
/* *INDENT-ON*  */

/************************************************************************/

/* void                                                                              */
/* duf_levinfo_set_dirid_d( duf_depthinfo_t * pdi, unsigned long long dirid, int d ) */
/* {                                                                                 */
/*   duf_levinfo_ptr_d( pdi, d )->dirid = dirid;                                     */
/* }                                                                                 */
/*                                                                                   */
/* (* *INDENT-OFF*  *)                                                               */
/* DUF_LEVINFO_FS( unsigned long long, dirid )                                       */
/* DUF_LEVINFO_FS_UP( unsigned long long, dirid )                                    */
/* (* *INDENT-ON*  *)                                                                */
/*                                                                                   */
/* *INDENT-OFF*  */
DUF_LEVINFO_3SET( unsigned long long, dirid, dirid )
/* *INDENT-ON*  */

/************************************************************************/

/* unsigned long long                                        */
/* duf_levinfo_dirid_d( const duf_depthinfo_t * pdi, int d ) */
/* {                                                         */
/*   return duf_levinfo_ptr_d( pdi, d )->dirid;              */
/* }                                                         */
/*                                                           */
/* (* *INDENT-OFF*  *)                                       */
/* DUF_LEVINFO_FC( unsigned long long, dirid )               */
/* DUF_LEVINFO_FC_UP( unsigned long long, dirid )            */
/* (* *INDENT-ON*  *)                                        */

/* *INDENT-OFF*  */
DUF_LEVINFO_3GET( unsigned long long, dirid, dirid )
/* *INDENT-ON*  */

/************************************************************************/

  /* *INDENT-OFF*  */
DUF_LEVINFO_3GET( unsigned long long, nameid, nameid )
/* *INDENT-ON*  */

/************************************************************************/

unsigned long long
duf_levinfo_nodedirid_d( const duf_depthinfo_t * pdi, int d )
{
  unsigned long long nodedirid = 0;

  nodedirid = duf_levinfo_dirid_d( pdi, d - ( duf_levinfo_is_leaf_d( pdi, d ) ? 1 : 0 ) );
  return nodedirid;
}
/* *INDENT-OFF*  */
DUF_LEVINFO_FC( unsigned long long, nodedirid )
DUF_LEVINFO_FC_UP( unsigned long long, nodedirid )
/* *INDENT-ON*  */

/************************************************************************/

/* int                                                        */
/* duf_levinfo_numdir_d( const duf_depthinfo_t * pdi, int d ) */
/* {                                                          */
/*   return duf_levinfo_ptr_d( pdi, d )->numdir;              */
/* }                                                          */
/*                                                            */
/* (* *INDENT-OFF*  *)                                        */
/* DUF_LEVINFO_FC( int, numdir )                              */
/* DUF_LEVINFO_FC_UP( int, numdir )                           */
/* (* *INDENT-ON*  *)                                         */

/* *INDENT-OFF*  */
DUF_LEVINFO_3GET( int, numdir, numdir )
/* *INDENT-ON*  */

/************************************************************************/

/* SET */

void
duf_levinfo_set_context_d( duf_depthinfo_t * pdi, void *ctx, int d )
{
  duf_clear_context( &duf_levinfo_ptr_d( pdi, d )->context );
  duf_set_context( &duf_levinfo_ptr_d( pdi, d )->context, ctx );
}

/* *INDENT-OFF*  */
DUF_LEVINFO_FS( void *, context )
DUF_LEVINFO_FS_UP( void *, context )
/* *INDENT-ON*  */

/************************************************************************/

void *
duf_levinfo_context_d( const duf_depthinfo_t * pdi, int d )
{
  return duf_context( &duf_levinfo_ptr_d( pdi, d )->context );
}

/* *INDENT-OFF*  */
DUF_LEVINFO_FC( void *, context )
DUF_LEVINFO_FC_UP( void *, context )
/* *INDENT-ON*  */

/************************************************************************/

/* duf_dirhandle_t *                                       */
/* duf_levinfo_pdh_d( const duf_depthinfo_t * pdi, int d ) */
/* {                                                       */
/*   return &duf_levinfo_ptr_d( pdi, d )->lev_dh;          */
/* }                                                       */
/*                                                         */
/* (* *INDENT-OFF*  *)                                     */
/* DUF_LEVINFO_FC( duf_dirhandle_t *, pdh )                */
/* DUF_LEVINFO_FC_UP( duf_dirhandle_t*, pdh )              */
/* (* *INDENT-ON*  *)                                      */

/* *INDENT-OFF*  */
DUF_LEVINFO_3GET_REF( duf_dirhandle_t, pdh, lev_dh )
/* *INDENT-ON*  */

/************************************************************************/

/* int                                                     */
/* duf_levinfo_dfd_d( const duf_depthinfo_t * pdi, int d ) */
/* {                                                       */
/*   return duf_levinfo_ptr_d( pdi, d )->lev_dh.dfd;       */
/* }                                                       */
/*                                                         */
/* (* *INDENT-OFF*  *)                                     */
/* DUF_LEVINFO_FC( int, dfd )                              */
/* DUF_LEVINFO_FC_UP( int, dfd )                           */
/* (* *INDENT-ON*  *)                                      */

/* *INDENT-OFF*  */
DUF_LEVINFO_4GET( int, dfd, lev_dh.dfd )
/* *INDENT-ON*  */



/************************************************************************/
/* *INDENT-OFF*  */
DUF_LEVINFO_3GET( int, source, lev_dh.source )
/* *INDENT-ON*  */

/* *INDENT-OFF*  */
DUF_LEVINFO_3GET( int, opened_copy, lev_dh.opened_copy )
/* *INDENT-ON*  */



/************************************************************************/

const char *
duf_levinfo_path_d( const duf_depthinfo_t * pdi, int d )
{
#if 0
  char *path = NULL;
#else
  const char *path = NULL;
#endif
  assert( pdi );
  if ( pdi->inited && d >= 0 )
  {
    assert( d >= 0 );
    assert( pdi->pathinfo.levinfo );
    assert( pdi->inited );
    if ( duf_levinfo_ptr_d( pdi, d )->is_leaf )
      d--;
#if 0
    {
      assert( d >= 0 );
      if ( duf_levinfo_ptr_d( pdi, d )->fullpath )
      {
        path = duf_levinfo_ptr_d( pdi, d )->fullpath;
      }
      else
        /* if ( pdi->path ) */
      {
        /* size_t lenp = strlen( pdi->path ); */
        size_t len = 2;
        char *p;

        for ( int i = 0; i <= d; i++ )
        {
          assert( pdi->pathinfo.levinfo[i].itemname );
          len += strlen( pdi->pathinfo.levinfo[i].itemname ) + 1;
        }
        path = mas_malloc( len );
        /* path = strcpy( path, pdi->path ); */
        p = path;

        for ( int i = 0; i <= d; i++ )
        {
          size_t l;

          if ( p == path || *( p - 1 ) != '/' )
            *p++ = '/';
          *p = 0;
          DUF_TRACE( path, 4, "path:%s", path );
          l = strlen( pdi->pathinfo.levinfo[i].itemname );
          if ( l > 0 )
          {
            strcpy( p, pdi->pathinfo.levinfo[i].itemname );
            p += l;
            *p++ = '/';
          }
          *p = 0;
        }
        assert( d >= 0 );
        duf_levinfo_ptr_d( pdi, d )->fullpath = path;
        DUF_TRACE( path, 4, "fullpath:%s", path );
      }
      /* else                                */
      /* {                                   */
      /*   DUF_SHOW_ERROR( "pdi->path not set" ); */
      /* }                                   */
    }
#else
    path = duf_pi_path_d( &pdi->pathinfo, d );
#endif
  }
#if 0
  if ( !path )
  {
    DUF_SHOW_ERROR( "no path returned" );
  }
#endif
  return path;
}

/* *INDENT-OFF*  */
DUF_LEVINFO_FC( const char *, path )
DUF_LEVINFO_FC_UP( const char *, path )
DUF_LEVINFO_FC_TOP( const char *, path )
/* *INDENT-ON*  */

const char *
duf_levinfo_relpath_d( const duf_depthinfo_t * pdi, int d )
{
  const char *toppath;
  const char *path;
  size_t len;

  toppath = duf_levinfo_path_top( pdi );
  len = strlen( toppath );
  path = duf_levinfo_path_d( pdi, d );
  return path + len;
}
/* *INDENT-OFF*  */
DUF_LEVINFO_FC( const char *, relpath )
DUF_LEVINFO_FC_UP( const char *, relpath )
/* *INDENT-ON*  */

const char *
duf_levinfo_path_q( const duf_depthinfo_t * pdi, const char *q )
{
  const char *p;

  p = duf_levinfo_path( pdi );
  return p ? p : q;
}

char *
duf_levinfo_path_qdup( const duf_depthinfo_t * pdi, const char *q )
{
  char *path = NULL;
  const char *p;

  p = duf_levinfo_path_q( pdi, q );
  if ( p )
    path = mas_strdup( p );
  return path;
}


/************************************************************************/
