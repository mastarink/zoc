/* #undef MAS_TRACING */
#include <assert.h>
#include <string.h>

#include "duf_tracen_defs_preset.h"                                  /* MAST_TRACE_CONFIG; etc. ✗ */
#include "duf_errorn_defs_preset.h"                                  /* MAST_ERRORS_FILE; etc. ✗ */

#include <mastar/trace/mas_trace.h>
#include <mastar/error/mas_error_defs_ctrl.h>
#include <mastar/error/mas_error_defs.h>

#include <mastar/wrap/mas_memory.h>                                  /* mas_(malloc|free|strdup); etc. ▤ */

#include "duf_se_only.h"                                             /* Only DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ✗ */

#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ✗ */

#include "duf_levinfo.h"                                             /* duf_levinfo_calc_depth; duf_levinfo_clear_level_d; etc. ✗ */
#include "duf_levinfo_ref.h"                                         /* duf_levinfo_*; etc. ✗ */
#include "duf_levinfo_credel.h"                                      /* duf_levinfo_create; duf_levinfo_delete ✗ */
#include "duf_context.h"
#include "duf_path2dirid.h"                                          /* duf_dirid2path; etc. ✗ */

#include "duf_pdi_filters.h"                                         /* duf_pdi_pu; etc. ✗ */
#include "duf_pdi_ref.h"
#include "duf_pdi_stmt.h"                                            /* duf_pdi_find_statement_by_id; etc. ✗ */

#include "duf_pathinfo_credel.h"                                     /* duf_pi_shut; duf_pi_copy; duf_pi_levinfo_create; duf_pi_levinfo_delete etc. ✗ */
#include "duf_pathinfo.h"                                            /* duf_pi_clear*; duf_pi_levinfo_set; duf_pi_set_max_rel_depth; etc. ✗ */

#include "duf_ufilter_structs.h"
#include "duf_ufilter_ref.h"

#include "duf_maindb.h"                                              /* duf_main_db; duf_main_db_open; duf_main_db_close ✗ */

#include "duf_path2db.h"                                             /* duf_real_path2db; etc. ✗ */

#include "duf_pdi_attach.h"

#include "duf_pdi_structs.h"
/* ###################################################################### */
#include "duf_pdi.h"                                                 /* duf_pdi_init; duf_pdi_shut; duf_pdi_close ✗ */
/* ###################################################################### */

/* 20150904.085609 */
/* DUF_WRAPSTATIC */
SR( PDI, pdi_init, duf_depthinfo_t * pdi, const duf_ufilter_t * pu, const char *real_path, const duf_sql_set_t * sql_set,
    int caninsert, unsigned frecursive, int fallow_dirs, unsigned flinear, int opendir )
{
  assert( pdi );
  assert( !sql_set || sql_set->type == DUF_NODE_NODE );              /* 20160214.162255 */

  MAST_TRACE( pdi, 5, "@@@frecursive:%d; real_path:%s", frecursive, real_path );
  MAST_TRACE( pdi, 7, "@@[%p] sql_beginning_done:%d", pdi, duf_pdi_root( pdi )->sql_beginning_done );
  if ( !pdi->inited )
  {
    assert( pdi->pdi_name );
    assert( duf_levinfo_closed( pdi ) );
  /* assert( real_path ); */
    pdi->inited = 1;
    pdi->pup = pu;
    MAST_TRACE( pdi, 3, "@@@(frecursive:%d/%d) real_path:%s", frecursive, duf_pdi_recursive( pdi ), real_path );

    duf_pi_set_max_rel_depth( &pdi->pathinfo, real_path, duf_ufilter_max_rel_depth( duf_pdi_pu( pdi ) ) );

    pdi->recursive = frecursive ? 1 : 0;
    pdi->allow_dirs = fallow_dirs ? 1 : 0;
    pdi->linear = flinear ? 1 : 0;
    pdi->opendir = opendir ? 1 : 0;

    CR( levinfo_create, pdi );                                       /* depth = -1 */

    MAST_TRACE( pdi, 0, "@@@@(frecursive:%d/%d) real_path:%s", frecursive, duf_pdi_recursive( pdi ), real_path );
    assert( QISERR || pdi->pathinfo.levinfo );

#ifdef DUF_ATTACH_SELECTED_PATTERN
# ifdef DUF_SQL_SELECTED_TEMPORARY
#  error "Wrong DUF_ATTACH_SELECTED_PATTERN / DUF_SQL_SELECTED_TEMPORARY : add include sql_tables_defs.h"
# endif
    if ( pdi->pdi_name )
    {
      CR( pdi_attach_selected, pdi );
    }
#else
# ifndef DUF_SQL_SELECTED_TEMPORARY
#  error Wrong "DUF_ATTACH_SELECTED_PATTERN / DUF_SQL_SELECTED_TEMPORARY : add include sql_tables_defs.h"
# endif
#endif
  /* assert( pdi->pathinfo.depth == -1 ); */
    if ( real_path )
    {
    /* QT( "real_path:%p:%s", real_path, real_path ); */
      assert( duf_levinfo_closed( pdi ) );
      CR( real_path2db, pdi, caninsert, real_path, sql_set );
    /* assert( !duf_levinfo_closed( pdi ) ); */
      if ( !pdi->pyp )
      {
        pdi->pyp = mas_malloc( sizeof( *pdi->pyp ) );
        pdi->pyp_created = 1;
        memset( pdi->pyp, 0, sizeof( *pdi->pyp ) );
        pdi->pyp->topdirid = duf_levinfo_dirid( pdi );
      /* QT( "(ci:%d) topdirid:%llu for '%s' - '%s'", caninsert, pdi->pyp->topdirid, real_path, duf_levinfo_path( pdi ) ); */
      }
    }
    MAST_TRACE( pdi, 5, "@@@(frecursive:%d/%d) real_path:%s", frecursive, duf_pdi_recursive( pdi ), real_path );
  }
/* QT( "@%p closed:%d", pdi, duf_levinfo_closed( pdi ) ); */
  MAST_TRACE( pdi, 7, "@@[%p] sql_beginning_done:%d", pdi, duf_pdi_root( pdi )->sql_beginning_done );
  ER( PDI, pdi_init, duf_depthinfo_t * pdi, const duf_ufilter_t * pu, const char *real_path, const duf_sql_set_t * sql_set,
      int caninsert, unsigned frecursive, int fallow_dirs, unsigned flinear, int opendir );
}

/* 20151113.105524 */
/* min, recursive */
SR( PDI, pdi_init_min_r, duf_depthinfo_t * pdi, const char *real_path )
{

  CR( pdi_init, pdi, ( duf_ufilter_t * ) NULL, real_path, NULL /* sql_set */ , 0 /* caninsert */ , 1 /* recursive */ ,
      1 /* allow_dirs */ , 0 /* linear */ , 0 /* opendir */  );
  ER( PDI, pdi_init_min_r, duf_depthinfo_t * pdi, const char *real_path );
}

/* 20150904.085510 */
SR( PDI, pdi_init_from_dirid, duf_depthinfo_t * pdi, const duf_ufilter_t * pu, unsigned long long dirid,
    const duf_sql_set_t * sql_set, int caninsert, unsigned frecursive, int fallow_dirs, unsigned flinear, int opendir )
{
  char *path = NULL;

  path = duf_dirid2path( dirid, QPERRIND );
  MAST_TRACE( pdi, 0, "@@@dirid:%llu; frecursive:%d; real_path:%s", dirid, frecursive, path );
  CR( pdi_init, pdi, pu, path, sql_set, caninsert, frecursive, fallow_dirs, flinear, opendir );
  MAST_TRACE( pdi, 8, "%d: %s / %s", QERRIND, path, duf_levinfo_path( pdi ) );
  mas_free( path );
  ER( PDI, pdi_init_from_dirid, duf_depthinfo_t * pdi, const duf_ufilter_t * pu, unsigned long long dirid,
      const duf_sql_set_t * sql_set, int caninsert, unsigned frecursive, int fallow_dirs, unsigned flinear, int opendir );
}

SR( PDI, pdi_shut, duf_depthinfo_t * pdi )
{
  assert( pdi );
  if ( pdi->inited )
  {
    assert( pdi->pathinfo.depth == duf_levinfo_calc_depth( pdi ) );

    duf_clear_context( &pdi->context );
    CR( levinfo_delete, pdi );

    for ( int i = 0; i < pdi->num_idstatements; QERRIND = 0, i++ )
      CR( pdi_finalize_idstmt, pdi, i );

    mas_free( pdi->idstatements );
    pdi->idstatements = NULL;

    pdi->num_idstatements = 0;
    pdi->inited = 0;
    pdi->opendir = 0;
#if 0
    pdi->pathinfo.depth = pdi->pathinfo.topdepth = 0;
    pdi->pathinfo.maxdepth = 0;
    assert( !pdi->pathinfo.maxdepth );
    assert( !pdi->pathinfo.topdepth );
    assert( !pdi->pathinfo.depth );
    assert( !pdi->pathinfo.levinfo );
#else
    CR( pi_shut, &pdi->pathinfo );
#endif
    assert( duf_levinfo_closed( pdi ) );
    pdi->changes = 0;
    pdi->pup = NULL;
    if ( pdi->pyp_created )
    {
      assert( pdi->pyp );
      mas_free( pdi->pyp );
    }
    pdi->pyp_created = 0;
    pdi->pyp = NULL;
    pdi->items.dirs = pdi->items.files = pdi->items.total = 0;
    pdi->total_bytes = 0;
    pdi->total_files = 0;
  /* QT( "@pdi->total_bytes:%llu", pdi->total_bytes ); */
    pdi->seqq.gen = pdi->seqq.row = pdi->seqq.node = pdi->seqq.leaf = 0;
    assert( !pdi->inited );
    assert( !pdi->opendir );
    assert( !pdi->items.files );
    assert( !pdi->items.dirs );
    assert( !pdi->items.total );
    assert( !pdi->changes );
    assert( !pdi->seqq.node );
    assert( !pdi->seqq.leaf );
    assert( !pdi->seqq.gen );
    assert( !pdi->context.ptr );
    assert( !pdi->context.destructor );
    assert( !pdi->num_idstatements );
    assert( !pdi->idstatements );
    assert( !pdi->pup );
    assert( !pdi->pyp );
  }
  ER( PDI, pdi_shut, duf_depthinfo_t * pdi );
}

/* 20150904.085731 */
SR( PDI, pdi_close, duf_depthinfo_t * pdi )
{

  assert( pdi->pathinfo.depth == duf_levinfo_calc_depth( pdi ) );

  CR( pdi_shut, pdi );
  if ( pdi->pdi_name /* && pdi->db_attached_selected */  && !pdi->attached_copy )
  {
    CR( main_db_close, pdi );
    assert( !pdi->next );
    if ( pdi->db_attached_selected )
    {
      assert( 0 == strcmp( pdi->db_attached_selected, pdi->pdi_name ) );
#ifdef DUF_ATTACH_SELECTED_PATTERN
# ifdef DUF_SQL_SELECTED_TEMPORARY
#  error "Wrong DUF_ATTACH_SELECTED_PATTERN / DUF_SQL_SELECTED_TEMPORARY : add include sql_tables_defs.h"
# endif

# if 0
      DOR( r, duf_pdi_detach_selected( pdi ) );
      {
        int ry DUF_UNUSED = 0;
        char *selected_db_file;

        selected_db_file = duf_expand_sql( DUF_ATTACH_SELECTED_PATTERN, pdi->db_attached_selected );
        ry = unlink( selected_db_file );
        {
          char serr[1024] = "";
          char *s;

          s = strerror_r( errno, serr, sizeof( serr ) );
          _DUF_SHOW_ERROR( "(%d) errno:%d unlink :%s;", ry, errno, s ? s : serr );
        }
        mas_free( selected_db_file );
      }
# endif
#else
# ifndef DUF_SQL_SELECTED_TEMPORARY
#  error Wrong "DUF_ATTACH_SELECTED_PATTERN / DUF_SQL_SELECTED_TEMPORARY : add include sql_tables_defs.h"
# endif
#endif
      mas_free( pdi->db_attached_selected );
      pdi->db_attached_selected = NULL;
    }
  }
  assert( !pdi->next );
  ER( PDI, pdi_close, duf_depthinfo_t * pdi );
}
