#include <string.h>
#include <assert.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_maintenance.h"

#include "duf_context.h"

#include "duf_sql_defs.h"
#include "duf_sql_field.h"

#include "duf_levinfo_ref.h"
#include "duf_levinfo_ref_def.h"
#include "duf_levinfo_context.h"

/* ###################################################################### */
#include "duf_levinfo.h"
/* ###################################################################### */



static void
duf_li_clear( duf_levinfo_t * pli )
{
  assert( pli );
  assert( pli->lev_dh.opened_copy || pli->lev_dh.dfd == 0 );
  if ( pli->itemname )
  {
    /* DUF_SHOW_ERROR( "CLEAR %s %p", pli->itemname, pli->itemname ); */
    mas_free( pli->itemname );
  }
  pli->itemname = NULL;

  if ( pli->fullpath )
    mas_free( pli->fullpath );
  pli->fullpath = NULL;
#if 0
  duf_levinfo_clear_context( pli );
#else
  duf_clear_context( &pli->context );
#endif
  memset( pli, 0, sizeof( *pli ) );
}

void
duf_levinfo_clear_level_d( duf_depthinfo_t * pdi, int d )
{
  assert( pdi );
  assert( pdi->pathinfo.levinfo );
  assert( d >= 0 );
  duf_li_clear( &pdi->pathinfo.levinfo[d] );
}

/* 20150831.000000 */
void
duf_levinfo_init_level_d( duf_depthinfo_t * pdi, const char *itemname, unsigned long long dirid, unsigned long long ndirs, unsigned long long nfiles,
                          int is_leaf, int d )
{
  assert( pdi );
  assert( d >= 0 );
  assert( pdi->pathinfo.levinfo );
  duf_levinfo_clear_level_d( pdi, d );
#if 0
  pdi->pathinfo.levinfo[d].lev_dh.dirid = dirid;
#endif
  /* assert( dirid == 0 ); (* ?? *) */
  pdi->pathinfo.levinfo[d].dirid = dirid;
  pdi->pathinfo.levinfo[d].numdir = ndirs;
  pdi->pathinfo.levinfo[d].numfile = nfiles;
  if ( itemname )
  {
    /* DUF_SHOW_ERROR( "BEFORE NEW LEVEL %d %s %p", d, pdi->pathinfo.levinfo[d].itemname, pdi->pathinfo.levinfo[d].itemname ); */
    assert( !pdi->pathinfo.levinfo[d].itemname );
    pdi->pathinfo.levinfo[d].itemname = mas_strdup( itemname );
    /* DUF_SHOW_ERROR( "NEW LEVEL %d %s %p", d, pdi->pathinfo.levinfo[d].itemname, pdi->pathinfo.levinfo[d].itemname ); */
  }
  pdi->pathinfo.levinfo[d].is_leaf = is_leaf ? 1 : 0;
}

/* 20150901.173353 */
void
duf_levinfo_init_level( duf_depthinfo_t * pdi, const char *itemname, unsigned long long dirid, unsigned long long ndirs, unsigned long long nfiles,
                        int is_leaf )
{
  duf_levinfo_init_level_d( pdi, itemname, dirid, ndirs, nfiles, is_leaf, pdi->pathinfo.depth );
}

int
duf_levinfo_dbinit_level_d( duf_depthinfo_t * pdi, duf_sqlite_stmt_t * pstmt, int is_leaf, int d )
{
  DEBUG_STARTR( r );

  assert( pdi );
  assert( d >= 0 );
  assert( pdi->pathinfo.levinfo );

  duf_levinfo_t *pli;

  assert( !pdi->pathinfo.levinfo[d].itemname );
  pli = &pdi->pathinfo.levinfo[d];
  /* if ( pdi->opendir ) */
  {
    duf_dirhandle_t *pdhlev = &pli->lev_dh;

    /* if ( S_ISBLK( stX.st_mode ) ) */
    /* {                             */
    /* }                             */
    /* DUF_SHOW_ERROR( "%s", pdi->pathinfo.levinfo[d].is_leaf ? "LEAF" : "NODE" ); */
    /* DUF_PRINTF( 0, "d:%d [%s]", d, pdi->pathinfo.levinfo[d].itemname ); */
    {
      /* stat */
      if ( DUF_GET_SFIELD2( dfname ) )
      {
        assert( !pli->itemname );
        pli->itemname = mas_strdup( DUF_GET_SFIELD2( dfname ) );
      }
      pli->is_leaf = is_leaf ? 1 : 0;

      pli->dirid = DUF_GET_UFIELD2( dirid );
      pli->numdir = DUF_GET_UFIELD2( ndirs );
      pli->numfile = DUF_GET_UFIELD2( nfiles );
      pli->nameid = DUF_GET_UFIELD2( nameid );
      pdhlev->st.st_dev = DUF_GET_UFIELD2( dev );
      pdhlev->st.st_ino = DUF_GET_UFIELD2( inode );
      pdhlev->st.st_mode = DUF_GET_UFIELD2( filemode );
      pdhlev->st.st_nlink = DUF_GET_UFIELD2( nlink );
      pdhlev->st.st_uid = DUF_GET_UFIELD2( uid );
      pdhlev->st.st_gid = DUF_GET_UFIELD2( gid );
      pdhlev->st.st_rdev = DUF_GET_UFIELD2( rdev );
      pdhlev->st.st_size = DUF_GET_UFIELD2( filesize );
      pdhlev->st.st_blksize = DUF_GET_UFIELD2( blksize );
      pdhlev->st.st_blocks = DUF_GET_UFIELD2( blocks );
      /* pdhlev->st.st_atim =; */
      /* pdhlev->st.st_mtim =; */
      /* pdhlev->st.st_ctim =; */
      DUF_TRACE( levinfo, 10, "(%d)? levinfo openated %s; dfd:%d", r, pdi->pathinfo.levinfo[d].itemname, pdhlev->dfd );
      pdhlev->rdb++;
      pdhlev->source = DUF_DH_SOURCE_DB;

      /* mas_free( sp ); */
    }
  }
  DEBUG_ENDR( r );
}
