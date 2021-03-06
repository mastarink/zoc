#define DUF_SQL_PDI_STMT
#include <string.h>

#include <assert.h>
#include <unistd.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_maintenance.h"

#include "duf_config_ref.h"
#include "duf_levinfo_openclose.h"
#include "duf_levinfo_updown.h"
#include "duf_levinfo_ref.h"
#include "duf_sql_defs.h"
#include "duf_sql_field.h"

#include "duf_pdi.h"
#include "duf_pdi_ref.h"

#include "duf_mod_defs.h"

#include "duf_sccb_eval_std.h"


/* #include "duf_path2db.h" */
#include "duf_path2dirid.h"

#include "duf_maindb.h"


#include "sql_beginning_selected.h"
#include "sql_beginning_tables.h"


/* ########################################################################################## */
DUF_MOD_DECLARE_ALL_FUNCS( dumplet )
/* ########################################################################################## */
     static duf_sql_sequence_t final_sql = /* */
     {
       .name = "final @ ...",
       .done = 0,
       .sql = {
               NULL,
               }
     };

/* ########################################################################################## */

duf_scan_callbacks_t duf_dumplet_callbacks = {
  .title = "dumplet",
  .name = "dumplet",
  .def_opendir = 0,
  .init_scan = dumplet_init,
  .beginning_sql_seq = &sql_create_selected,

  .node_scan_before2 = dumplet_node_before2,
  .node_scan_before2_deleted = dumplet_node_before2_del,

  .node_scan_after2 = dumplet_node_after2,
  .node_scan_after2_deleted = dumplet_node_after2_del,

  .node_scan_middle2 = dumplet_node_middle2,
  .node_scan_middle2_deleted = dumplet_node_middle2_del,

  .leaf_scan_fd2 = dumplet_de_content2,
  .leaf_scan_fd2_deleted = dumplet_de_content2_del,

  .leaf_scan2 = dumplet_leaf2,
  .leaf_scan2_deleted = dumplet_leaf2_del,

  .dirent_file_scan_before2 = dumplet_de_file_before2,
  .dirent_dir_scan_before2 = dumplet_de_dir_before2,

/* TODO : exp;ain values of use_std_leaf and use_std_node TODO */
  .use_std_leaf = 1,            /* 1 : preliminary selection; 2 : direct (beginning_sql_seq=NULL recommended in many cases) */
  .use_std_node = 1,            /* 1 : preliminary selection; 2 : direct (beginning_sql_seq=NULL recommended in many cases) */
  .final_sql_seq = &final_sql,
};

/* ########################################################################################## */

static int
dumplet_init( duf_sqlite_stmt_t * pstmt_unused, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );

  DUF_TRACE( mod, 0, "dumplet_init %s", duf_levinfo_path( pdi ) );

  DEBUG_ENDR( r );
}

static int
dumplet_de_content2( duf_sqlite_stmt_t * pstmt, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );

  assert( 0 == strcmp( DUF_GET_SFIELD2( filename ), duf_levinfo_itemtruename( pdi ) ) );
  assert( duf_levinfo_opened_dh( pdi ) > 0 || duf_levinfo_item_deleted( pdi ) );
  assert( duf_levinfo_stat( pdi ) || duf_levinfo_item_deleted( pdi ) );
  DUF_TRACE( mod, 4, "dumplet %s : %s -a-", duf_levinfo_path( pdi ), duf_levinfo_itemtruename( pdi ) );

  DEBUG_ENDR( r );
}

static int
dumplet_de_content2_del( duf_sqlite_stmt_t * pstmt, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );

  assert( 0 == strcmp( DUF_GET_SFIELD2( filename ), duf_levinfo_itemtruename( pdi ) ) );
  DUF_TRACE( mod, 6, "dumplet %s : %s -a-", duf_levinfo_path( pdi ), duf_levinfo_itemtruename( pdi ) );

  DEBUG_ENDR( r );
}

static int
dumplet_leaf2( duf_sqlite_stmt_t * pstmt, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );
  const char *DUF_UNUSED rs = NULL;

  assert( !duf_levinfo_dfd( pdi ) );
  assert( 0 == strcmp( DUF_GET_SFIELD2( filename ), duf_levinfo_itemtruename( pdi ) ) );
  assert( duf_levinfo_dbstat( pdi ) );

#if 0
  {
    char *path = NULL;

    path = duf_dirid2path( duf_levinfo_dirid( pdi ), &r );
    DUF_TRACE( mod, 2, "@@dumplet       %s", path );
    DUF_TRACE( mod, 2, "@@@dumplet       %s : %s", duf_levinfo_path( pdi ), duf_levinfo_itemtruename( pdi ) );
    DUF_TRACE( mod, 2, "@@@dumplet id2p: %s", path );
    mas_free( path );
    path = NULL;
    assert( 0 );
  }
#elif 1
  duf_depthinfo_t di = {
    .pdi_name = "dumplet_pdi"
  };
  duf_ufilter_t uf = {
    .md5id.flag = 1,.md5id.min = DUF_GET_UFIELD2( md5id ),.md5id.max = DUF_GET_UFIELD2( md5id ),
    .same.flag = 1,.same.min = 2,.same.max = 0,
  };
#  if 0
  DOR( r, duf_pdi_init_from_dirid( &di, &uf, duf_levinfo_dirid( pdi ), NULL /* sql_set */ , 0 /* caninsert */ , 1 /* recursive */ ,
                                   0 /* opendir */  ) );
#  else
  if ( uf.md5id.min > 0 && DUF_GET_UFIELD2( nsame ) > 1 )
  {
    DOR( r,
         DUF_WRAPPED( duf_pdi_init ) ( &di, &uf, duf_levinfo_path_top( pdi ) /* duf_levinfo_path( pdi ) */ , NULL /* sql_set */ , 0 /* caninsert */ ,
                                       1 /* recursive */ ,
                                       0 /* opendir */  ) );
    DUF_TEST_R( r );
#  endif
    /* DOR( r, duf_levinfo_godown_dbopenat_dh( pdi, duf_levinfo_itemtruename( pdi ), 1 (* is_leaf *) , pstmt_files ) ); */
    DOR( r, duf_levinfo_godown_openat_dh( &di, duf_levinfo_itemtruename( pdi ), 1 /* is_leaf */  ) );
    DUF_TEST_R( r );
    if ( DUF_NOERROR( r ) )
    {
      DUF_TRACE( mod, 0, "@@@dumplet  %s : %s", duf_levinfo_path( pdi ), duf_levinfo_itemtruename( pdi ) );
      /* "selected" tables should be different!? */
      DOR_NOE( r, duf_evaluate_pdi_sccb_std( "tree", &di, &uf ), DUF_ERROR_NOT_IN_DB );
      DUF_CLEAR_ERROR( r, DUF_ERROR_NOT_IN_DB );
      DUF_TEST_R( r );
      assert( di.pup == &uf );
    }
    rs = duf_error_name( r );
    duf_pdi_shut( &di );
  }
#else
  /*
   * TODO
   * 1. SELECT ... FROM .... LEFT JOIN .... LEFT JOIN .... WHERE fd.md5id=DUF_GET_UFIELD2(md5id)
   * 2. dirid2path ...
   * */
#endif

  DEBUG_ENDR( r );
}

static int
dumplet_leaf2_del( duf_sqlite_stmt_t * pstmt, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );

  /* Never called (no deleted flag - didn't try to open !!) */
  DUF_TRACE( mod, 4, "dumplet %s : %s -a-", duf_levinfo_path( pdi ), duf_levinfo_itemtruename( pdi ) );

  DEBUG_ENDR( r );
}

static int
dumplet_node_before2( duf_sqlite_stmt_t * pstmt_unused, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );

  DUF_TRACE( mod, 10, "dumplet %s : %s", duf_levinfo_path( pdi ), duf_levinfo_itemshowname( pdi ) );

  DEBUG_ENDR( r );
}

static int
dumplet_node_before2_del( duf_sqlite_stmt_t * pstmt, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );

  DUF_TRACE( mod, 10, "@dumplet node before: %s : %s", duf_levinfo_path( pdi ), duf_levinfo_itemshowname( pdi ) );

  DEBUG_ENDR( r );
}

static int
dumplet_node_middle2( duf_sqlite_stmt_t * pstmt, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );

  DUF_TRACE( mod, 10, "dumplet %s : %s", duf_levinfo_path( pdi ), duf_levinfo_itemshowname( pdi ) );

  DEBUG_ENDR( r );
}

static int
dumplet_node_middle2_del( duf_sqlite_stmt_t * pstmt, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );

  DUF_TRACE( mod, 10, "@dumplet node middle %s : %s", duf_levinfo_path( pdi ), duf_levinfo_itemshowname( pdi ) );

  DEBUG_ENDR( r );
}

static int
dumplet_node_after2( duf_sqlite_stmt_t * pstmt, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );

  DUF_TRACE( mod, 10, "dumplet %s : %s", duf_levinfo_path( pdi ), duf_levinfo_itemshowname( pdi ) );

  DEBUG_ENDR( r );
}

static int
dumplet_node_after2_del( duf_sqlite_stmt_t * pstmt, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );

  DUF_TRACE( mod, 10, "@dumplet node after %s : %s", duf_levinfo_path( pdi ), duf_levinfo_itemshowname( pdi ) );

  DEBUG_ENDR( r );
}

static int
dumplet_de_dir_before2( duf_sqlite_stmt_t * pstmt_unused, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );

  DUF_TRACE( mod, 8, "dumplet de dir before: %s : %s", duf_levinfo_path( pdi ), duf_levinfo_itemshowname( pdi ) );

  DEBUG_ENDR( r );
}

static int
dumplet_de_file_before2( duf_sqlite_stmt_t * pstmt_unused, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );

  DUF_TRACE( mod, 6, "dumplet de file before: %s : %s", duf_levinfo_path( pdi ), duf_levinfo_itemshowname( pdi ) );

  DEBUG_ENDR( r );
}
