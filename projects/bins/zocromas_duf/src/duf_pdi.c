/* #undef MAS_TRACING */
#include <assert.h>
#include <string.h>

#include <mastar/wrap/mas_memory.h>                                  /* mas_(malloc|free|strdup); etc. ♣ */

#include "duf_tracen_defs.h"                                         /* DUF_TRACE ♠ */
#include "duf_errorn_defs.h"                                         /* DUF_NOERROR; DUF_CLEAR_ERROR; DUF_E_(LOWER|UPPER); DUF_TEST_R ... ♠ */

#include "duf_start_end.h"                                           /* DUF_STARTR ; DUF_ENDR ♠ */
#include "duf_dodefs.h"                                              /* DOR ♠ */

#include "duf_debug_defs.h"                                          /* DUF_WRAPSTATIC; DUF_WRAPPED ...  ♠ */

/* #include "duf_option_defs.h" */

/* #include "duf_config.h"                                              (* duf_get_config ♠ *) */
#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ♠ */
/* #include "duf_config_defs.h" */

#include "duf_levinfo.h"
#include "duf_levinfo_ref.h"
#include "duf_levinfo_credel.h"
#include "duf_context.h"
#include "duf_path2dirid.h"

#include "duf_pdi_filters.h"
#include "duf_pdi_ref.h"
#include "duf_pdi_pi_ref.h"
#include "duf_pdi_stmt.h"

#include "duf_pathinfo_credel.h"
#include "duf_pathinfo.h"

#include "duf_ufilter_ref.h"

/* #include "sql_tables_defs.h" */

#include "duf_maindb.h"                                              /* duf_main_db; duf_main_db_open; duf_main_db_close ♠ */

#include "duf_path2db.h"

#include "duf_pdi_attach.h"

/* ###################################################################### */
#include "duf_pdi.h"
/* ###################################################################### */

/* 20150904.085609 */
DUF_WRAPSTATIC int
duf_pdi_init( duf_depthinfo_t * pdi, const duf_ufilter_t * pu, const char *real_path, const duf_sql_set_t * sql_set,
              int caninsert, unsigned frecursive, int fallow_dirs, unsigned flinear, int opendir )
{
  DUF_STARTR( r );

  assert( pdi );
  assert( !sql_set || sql_set->type == DUF_NODE_NODE );              /* 20160214.162255 */

  DUF_TRACE( pdi, 5, "@@@frecursive:%d; real_path:%s", frecursive, real_path );
  DUF_TRACE( pdi, 7, "@@[%p] sql_beginning_done:%d", pdi, duf_pdi_root( pdi )->sql_beginning_done );
  if ( !pdi->inited )
  {
    assert( pdi->pdi_name );
  /* assert( real_path ); */
    pdi->inited = 1;
    pdi->pup = pu;
    DUF_TRACE( pdi, 3, "@@@(frecursive:%d/%d) real_path:%s", frecursive, duf_pdi_recursive( pdi ), real_path );
#if 0
    pdi->pathinfo.depth = -1;
    if ( real_path )
    {
      DOR( r, duf_pathdepth( real_path ) );
      if ( DUF_NOERROR( r ) )
        pdi->pathinfo.topdepth = r;
    }
    assert( pdi->pathinfo.depth == -1 );
    {
      int max_rel_depth = 0;

      max_rel_depth = pdi && pdi->pup ? pdi->pup->max_rel_depth : 20;
      assert( pdi->pathinfo.depth == -1 );
    /* DUF_TRACE( temp, 0, "@@@@@@@ %u", max_rel_depth ); */
      assert( max_rel_depth /* FIXME */  );
      {
        pdi->pathinfo.maxdepth = max_rel_depth + ( pdi->pathinfo.topdepth ? pdi->pathinfo.topdepth : 20 );
      }
    }
#else

# if 0
    duf_pi_set_max_rel_depth( &pdi->pathinfo, real_path, pdi->pup ? pdi->pup->max_rel_depth : 0 );
# else
    duf_pi_set_max_rel_depth( &pdi->pathinfo, real_path, duf_ufilter_max_rel_depth( duf_pdi_pu( pdi ) ) );
# endif

#endif
    pdi->recursive = frecursive ? 1 : 0;
    pdi->allow_dirs = fallow_dirs ? 1 : 0;
    pdi->linear = flinear ? 1 : 0;
    pdi->opendir = opendir ? 1 : 0;

    DOR( r, duf_levinfo_create( pdi ) );                             /* depth = -1 */

    DUF_TRACE( pdi, 0, "@@@@(frecursive:%d/%d) real_path:%s", frecursive, duf_pdi_recursive( pdi ), real_path );
    assert( r < 0 || pdi->pathinfo.levinfo );

#ifdef DUF_ATTACH_SELECTED_PATTERN
# ifdef DUF_SQL_SELECTED_TEMPORARY
#  error "Wrong DUF_ATTACH_SELECTED_PATTERN / DUF_SQL_SELECTED_TEMPORARY : add include sql_tables_defs.h"
# endif
    if ( pdi->pdi_name )
    {
      DOR( r, duf_pdi_attach_selected( pdi ) );
    }
#else
# ifndef DUF_SQL_SELECTED_TEMPORARY
#  error Wrong "DUF_ATTACH_SELECTED_PATTERN / DUF_SQL_SELECTED_TEMPORARY : add include sql_tables_defs.h"
# endif
#endif
  /* assert( pdi->pathinfo.depth == -1 ); */
    if ( real_path )
    {
    /* T( "real_path:%p:%s", real_path, real_path ); */
      DOR( r, duf_real_path2db( pdi, caninsert, real_path, sql_set ) );
      if ( !pdi->pyp )
      {
        pdi->pyp = mas_malloc( sizeof( *pdi->pyp ) );
        pdi->pyp_created = 1;
        memset( pdi->pyp, 0, sizeof( *pdi->pyp ) );
        pdi->pyp->topdirid = duf_levinfo_dirid( pdi );
      /* T( "(ci:%d) topdirid:%llu for '%s' - '%s'", caninsert, pdi->pyp->topdirid, real_path, duf_levinfo_path( pdi ) ); */
      }
    }
    DUF_TRACE( pdi, 5, "@@@(frecursive:%d/%d) real_path:%s", frecursive, duf_pdi_recursive( pdi ), real_path );
  }
  DUF_TRACE( pdi, 7, "@@[%p] sql_beginning_done:%d", pdi, duf_pdi_root( pdi )->sql_beginning_done );
  DUF_ENDR( r );
}

#ifdef MAS_WRAP_FUNC
/* 20150904.085443 */
int
DUF_WRAPPED( duf_pdi_init ) ( duf_depthinfo_t * pdi, const duf_ufilter_t * pu, const char *real_path,
                              const duf_sql_set_t * sql_set, int caninsert, unsigned frecursive, int fallow_dirs, unsigned flinear, int opendir )
{
  DUF_STARTR( r );

  DUF_TRACE( pdi, 3, "@@@frecursive:%d; real_path:%s", frecursive, real_path );
/* T( "real_path:%p:%s", real_path, real_path ); */
  DOR( r, duf_pdi_init( pdi, pu, real_path, sql_set, caninsert, frecursive, fallow_dirs, flinear, opendir ) );

  if ( DUF_NOERROR( r ) )
  {
    DUF_TRACE( explain, 1, "converted to real_path: %s", real_path );
    DUF_TRACE( explain, 0, "added path: %s", real_path );
  /* DUF_TRACE( path, 10, "diridpid: %llu", duf_levinfo_dirid( pdi ) ); */
  }
/* TODO */ assert( duf_pdi_levinfo( pdi ) );
  DUF_ENDR( r );
}
#endif

/* 20151113.105524 */
/* min, recursive */
int
duf_pdi_init_min_r( duf_depthinfo_t * pdi, const char *real_path )
{
  DUF_STARTR( r );

  DOR( r, DUF_WRAPPED( duf_pdi_init ) ( pdi, ( duf_ufilter_t * ) NULL, real_path, NULL /* sql_set */ , 0 /* caninsert */ , 1 /* recursive */ ,
                                        1 /* allow_dirs */ , 0 /* linear */ , 0 /* opendir */  ) );
  DUF_ENDR( r );
}

/* 20150904.085510 */
int
duf_pdi_init_from_dirid( duf_depthinfo_t * pdi, const duf_ufilter_t * pu, unsigned long long dirid,
                         const duf_sql_set_t * sql_set, int caninsert, unsigned frecursive, int fallow_dirs, unsigned flinear, int opendir )
{
  DUF_STARTR( r );
  char *path = NULL;

  path = duf_dirid2path( dirid, &r );
  DUF_TRACE( pdi, 0, "@@@dirid:%llu; frecursive:%d; real_path:%s", dirid, frecursive, path );
  DOR( r, duf_pdi_init( pdi, pu, path, sql_set, caninsert, frecursive, fallow_dirs, flinear, opendir ) );
  DUF_TRACE( pdi, 8, "%d: %s / %s", r, path, duf_levinfo_path( pdi ) );
  mas_free( path );
  DUF_ENDR( r );
}

int
duf_pdi_shut( duf_depthinfo_t * pdi )
{
  DUF_STARTR( r );

  assert( pdi );
  if ( pdi->inited )
  {
    assert( pdi->pathinfo.depth == duf_levinfo_calc_depth( pdi ) );

    duf_clear_context( &pdi->context );
    DOR( r, duf_levinfo_delete( pdi ) );

    for ( int i = 0; i < pdi->num_idstatements; r = 0, i++ )
      DOR( r, duf_pdi_finalize_idstmt( pdi, i ) );

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
    DOR( r, duf_pi_shut( &pdi->pathinfo ) );
#endif
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
  /* T( "@pdi->total_bytes:%llu", pdi->total_bytes ); */
    pdi->seq = pdi->seq_row = pdi->seq_node = pdi->seq_leaf = 0;
    assert( !pdi->inited );
    assert( !pdi->opendir );
    assert( !pdi->items.files );
    assert( !pdi->items.dirs );
    assert( !pdi->items.total );
    assert( !pdi->changes );
    assert( !pdi->seq_node );
    assert( !pdi->seq_leaf );
    assert( !pdi->seq );
    assert( !pdi->context.ptr );
    assert( !pdi->context.destructor );
    assert( !pdi->num_idstatements );
    assert( !pdi->idstatements );
    assert( !pdi->pup );
    assert( !pdi->pyp );
  }
  DUF_ENDR( r );
}

/* 20150904.085731 */
int
duf_pdi_close( duf_depthinfo_t * pdi )
{
  DUF_STARTR( r );

  assert( pdi->pathinfo.depth == duf_levinfo_calc_depth( pdi ) );

  DOR( r, duf_pdi_shut( pdi ) );
  if ( pdi->pdi_name /* && pdi->db_attached_selected */  && !pdi->attached_copy )
  {
    DOR( r, duf_main_db_close( pdi, r ) );
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
# endif
# if 0
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
  DUF_ENDR( r );
}
