/* #undef MAS_TRACING */
#include <assert.h>
#include <string.h>

#include "duf_tracen_defs_preset.h"                                  /* MAST_TRACE_CONFIG; etc. ✗ */
/* #include "duf_errorn_defs_preset.h"                                  (* MAST_ERRORS_FILE; etc. ✗ *) */

#include <mastar/wrap/mas_memory.h>                                  /* mas_(malloc|free|strdup); etc. ▤ */
/* #include <mastar/tools/mas_utils_path.h>                             (* mas_normalize_path; mas_pathdepth etc. ♣ *) */
#include <mastar/trace/mas_trace.h>
/* #include <mastar/error/mas_error_defs_ctrl.h> */
/* #include <mastar/error/mas_error_defs_make.h> */
/* #include <mastar/error/mas_error_defs.h> */

#include "duf_config.h"                                              /* duf_get_config ✗ */
#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ✗ */

#include "duf_levinfo_ref_def.h"
#include "duf_levinfo_credel.h"                                      /* duf_levinfo_create; duf_levinfo_delete ✗ */

#include "duf_sccb_scanstage.h"                                      /* duf_scanstage_name; duf_scanstage_scanner; ✗ */

#include "duf_li_credel.h"
#include "duf_li.h"

#include "duf_dh.h"                                                  /* duf_openat_dh; duf_open_dh; duf_opened_dh; duf_close_dh; duf_statat_dh; etc. ✗ */

#include "duf_levinfo_structs.h"

#include "duf_pathinfo_structs.h"                                    /* duf_pathinfo_s; (from duf_pathinfo_types: duf_pathinfo_t ) ✗ */

#include "duf_pathinfo_ref_def.h"
/* ###################################################################### */
#include "duf_pathinfo_ref.h"
/* ###################################################################### */

/* 20150904.122230 */
duf_levinfo_t *
duf_pi_ptr_d( const duf_pathinfo_t * pi, int d )
{
  assert( pi );
/* assert( d >= 0 ); */
/* assert( pi->levinfo ); */

  return pi && ( d >= 0 && d <= ( int ) pi->maxdepth ) && pi->levinfo ? &pi->levinfo[d] : NULL;
}
/* *INDENT-OFF*  */
DUF_PATHINFO_FC_REF( duf_levinfo_t , ptr )
DUF_PATHINFO_FC_UP_REF( duf_levinfo_t , ptr )
/* *INDENT-ON*  */

/* 20150904.122234 */
const char *
duf_pi_path_d( const duf_pathinfo_t * pi, int d )
{
  char *path = NULL;
  duf_node_type_t nt;

  assert( pi );
  nt = duf_pi_node_type_d( pi, d );
  if ( d >= 0 /* && nt != DUF_NODE_NONE */  )
  {
    duf_levinfo_t *pli = NULL;

  /* QT( "@@%d d:%d - %s", pi->maxdepth, d, duf_nodetype_name( duf_pi_node_type_d( pi, d ) ) ); */
    assert( nt == DUF_NODE_LEAF || nt == DUF_NODE_NODE );
    if ( nt == DUF_NODE_LEAF )
      d--;

    pli = duf_pi_ptr_d( pi, d );
    assert( pli );

  /* QT( "@%d d:%d", pi->maxdepth, d ); */
    if ( !pli->fullpath )
      pli->fullpath = duf_li_path( duf_pi_ptr_d( pi, 0 ), d + 1 );
    path = pli->fullpath;
  }
  MAST_TRACE( path, 4, "fullpath:%s", path );
  return path;
}
/* *INDENT-OFF*  */
DUF_PATHINFO_FC( const char *, path )
DUF_PATHINFO_FC_UP( const char *, path )
DUF_PATHINFO_FC_TOP( const char *, path )
/* *INDENT-ON*  */

DUF_PATHINFO_3GET( duf_node_type_t, node_type, node_type );
/* DUF_PATHINFO_3GET_REF( void, context, context ); */

/* 20150904.122510 */
const char *
duf_pi_relpath_d( const duf_pathinfo_t * pi, int d )
{
  const char *toppath;
  const char *path;

  assert( pi );
  toppath = duf_pi_path_top( pi );
  path = duf_pi_path_d( pi, d );
  return path ? path + ( toppath ? strlen( toppath ) : 0 ) : NULL;
}
/* *INDENT-OFF*  */
DUF_PATHINFO_FC( const char *, relpath )
DUF_PATHINFO_FC_UP( const char *, relpath )
/* *INDENT-ON*  */

#if 0
/* 20150904.122152 */
const char *
duf_pi_path_q( const duf_pathinfo_t * pi, const char *q )
{
  const char *p;

  p = duf_pi_path( pi );
  return p ? p : q;
}

/* 20150904.122155 */
char *
duf_pi_path_qdup( const duf_pathinfo_t * pi, const char *q )
{
  const char *p;

  p = duf_pi_path_q( pi, q );
  return p ? mas_strdup( p ) : NULL;
}
#endif

/* *INDENT-OFF*  */
DUF_PATHINFO_4GET( int, dfd, lev_dh.dfd )
/* *INDENT-ON*  */

/* 20150904.122200 */
const char *
duf_pi_itemshowname_d( const duf_pathinfo_t * pi, int d )
{
  const char *n = NULL;

  n = duf_pi_itemtruename_d( pi, d );
/* return n ? ( *n ? n : "/" ) : n; */
  return n && !*n ? "/" : n;
}

/* *INDENT-OFF*  */
DUF_PATHINFO_FC_REF( const char, itemshowname )
DUF_PATHINFO_FC_UP_REF( const char, itemshowname )
/* *INDENT-ON*  */

/* 20150904.122206 */
const char *
duf_pi_itemshowname_q( const duf_pathinfo_t * pi, const char *q )
{
  const char *p;

  p = duf_pi_itemshowname( pi );
  return p ? p : q;
}

/* 20150904.122209 */
const char *
duf_pi_itemtruename_d( const duf_pathinfo_t * pi, int d )
{
  const char *n = NULL;

  duf_levinfo_t *pli;

  pli = duf_pi_ptr_d( pi, d );

  n = pli ? pli->itemname : NULL;
/* return n ? ( *n ? n : "/" ) : n; */
  return n;
}

/* *INDENT-OFF*  */
DUF_PATHINFO_FC_REF( const char, itemtruename )
DUF_PATHINFO_FC_UP_REF( const char, itemtruename )
/* *INDENT-ON*  */

int
duf_pi_is_leaf_d( const duf_pathinfo_t * pi, int d )
{
  assert( pi );
  return d >= 0 ? duf_pi_ptr_d( pi, d )->node_type == DUF_NODE_LEAF : DUF_NODE_NONE;
}
/* *INDENT-OFF*  */
DUF_PATHINFO_FC( int, is_leaf )
DUF_PATHINFO_FC_UP( int, is_leaf )  
/* *INDENT-ON*  */

unsigned long long
duf_pi_dirid_d( const duf_pathinfo_t * pi, int d )
{
  unsigned long long n = NULL;

  duf_levinfo_t *pli;

  pli = duf_pi_ptr_d( pi, d );

  n = pli ? pli->db.dirid : 0;
  return n;
}

/* *INDENT-OFF*  */
DUF_PATHINFO_FC(  unsigned long long, dirid )
DUF_PATHINFO_FC_UP(  unsigned long long, dirid )
/* *INDENT-ON*  */

unsigned long long
duf_pi_nodedirid_d( const duf_pathinfo_t * pi, int d )
{
  unsigned long long nodedirid = 0;

  assert( pi );
  nodedirid = duf_pi_dirid_d( pi, d - ( duf_pi_is_leaf_d( pi, d ) ? 1 : 0 ) );
  return nodedirid;
}
/* *INDENT-OFF*  */
DUF_PATHINFO_FC( unsigned long long, nodedirid )
DUF_PATHINFO_FC_UP( unsigned long long, nodedirid )  
/* *INDENT-ON*  */

#if 0
/* 20150904.122217 */
const char *
duf_pi_itemtruename_q( const duf_pathinfo_t * pi, const char *q )
{
  const char *p;

  p = duf_pi_itemtruename( pi );
  return p ? p : q;
}
#endif
/************************************************************************/
/* 20150904.122223 */
struct stat *
duf_pi_stat_d( const duf_pathinfo_t * pi, int d )
{
  struct stat *pst = NULL;

/* if ( duf_pdi_opendir( pdi ) ) */
  {
    duf_dirhandle_t *pdh;

    pdh = duf_pi_pdh_d( pi, d );

    if ( pdh && pdh->rs > 0 && pdh->source == DUF_DH_SOURCE_FS )
      pst = &pdh->st;
  }
/* TODO NOT here: assert( pst->st_dev ); 
 * (used to check presence, so no stat is OK)
 * TODO */
  return pst;
}
/* *INDENT-OFF*  */
DUF_PATHINFO_FC_REF( struct stat, stat )
DUF_PATHINFO_FC_UP_REF( struct stat, stat )
/* *INDENT-ON*  */

/* *INDENT-OFF*  */
DUF_PATHINFO_3GET_REF( duf_dirhandle_t, pdh, lev_dh )
/* *INDENT-ON*  */

DUF_PATHINFO_ST_FLD_NAME( ino, inode );
DUF_PATHINFO_ST_FLD( dev );
DUF_PATHINFO_ST_FLD( mode );
DUF_PATHINFO_ST_FLD( nlink );
DUF_PATHINFO_ST_FLD( uid );
DUF_PATHINFO_ST_FLD( gid );
DUF_PATHINFO_ST_TYP_FLD( blkcnt, blocks );
DUF_PATHINFO_ST_FLD( blksize );
DUF_PATHINFO_ST_TYP_FLD( off, size );
DUF_PATHINFO_ST_TYP_FLD_NAME( time, atime, asec );
DUF_PATHINFO_ST_TYP_FLD_NAME( time, mtime, msec );
DUF_PATHINFO_ST_TYP_FLD_NAME( time, ctime, csec );
DUF_PATHINFO_ST_TYP_FLD_NAME( long, atim.tv_nsec, ansec );
DUF_PATHINFO_ST_TYP_FLD_NAME( long, mtim.tv_nsec, mnsec );
DUF_PATHINFO_ST_TYP_FLD_NAME( long, ctim.tv_nsec, cnsec );

/************************************************************************/
void
duf_pi_set_topdepth( duf_pathinfo_t * pi )
{
  if ( pi )
    pi->topdepth = pi->depth;
}

#if 0
int
duf_pi_topdepth( const duf_pathinfo_t * pi )
{
  return pi ? pi->topdepth : 0;
}

int
duf_pi_maxdepth( const duf_pathinfo_t * pi )
{
  return pi ? pi->maxdepth : 0;
}

int
duf_pi_depth( const duf_pathinfo_t * pi )
{
  return pi ? pi->depth : 0;
}

duf_levinfo_t *
duf_pi_levinfo( const duf_pathinfo_t * pi )
{
  return pi ? pi->levinfo : NULL;
}

#else
/* *INDENT-OFF*  */
DUF_PATHINFO_GET_VAL( int, topdepth)
DUF_PATHINFO_GET_VAL( int, maxdepth)
DUF_PATHINFO_GET_VAL( int, depth)
DUF_PATHINFO_GET_VAL( duf_levinfo_t *, levinfo)
/* *INDENT-ON*  */
#endif

duf_levinfo_t *
duf_pi_li_d( const duf_pathinfo_t * pi, int d )
{
  duf_levinfo_t *li = NULL;

  if ( pi && pi->levinfo && d >= 0 && d <= ( int ) pi->maxdepth )
    li = &pi->levinfo[d];
  return li;
}
/* *INDENT-OFF*  */
DUF_PATHINFO_FC( duf_levinfo_t *, li )
DUF_PATHINFO_FC_UP( duf_levinfo_t *, li )
/* *INDENT-ON*  */

int
duf_pi_deltadepth_d( const duf_pathinfo_t * pi, int d )
{
  return pi ? d - duf_pi_topdepth( pi ) : 0;
}
/* *INDENT-OFF*  */
DUF_PATHINFO_FC( int, deltadepth )
DUF_PATHINFO_FC_UP( int, deltadepth )
/* *INDENT-ON*  */

#if 0
void
duf_pi_set_each_stmt_d( duf_pathinfo_t * pi, duf_stmnt_t * pstmt_val, int d )
{
  if ( pi && d >= 0 )
    pi->levinfo[d].each_pstmt = pstmt_val;
}

duf_stmnt_t *
duf_pi_each_stmt_d( const duf_pathinfo_t * pi, int d )
{
  return pi && d >= 0 ? pi->levinfo[d].each_pstmt : NULL;
}
#endif
/* *INDENT-OFF*  */
DUF_PATHINFO_3SET( duf_stmnt_t *, each_stmt, each_pstmt)
DUF_PATHINFO_3GET( duf_stmnt_t *, each_stmt, each_pstmt)
/* *INDENT-ON*  */

const char *
duf_pi_itemname( const duf_pathinfo_t * pi )
{
  return pi ? duf_li_itemname( duf_pi_li( pi ) /* &pi->levinfo[pi->depth] */  ) : NULL;
}
