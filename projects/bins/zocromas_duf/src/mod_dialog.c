#include <string.h>

#include <unistd.h>

#include "duf_maintenance.h"

#include "duf_config_ref.h"

#include "duf_levinfo_openclose.h"
#include "duf_levinfo_stat.h"
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
DUF_MOD_DECLARE_ALL_FUNCS( dialog )
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

duf_scan_callbacks_t duf_dialog_callbacks = {
  .title = "dialog",
  .name = "dialog",
  .def_opendir = 0,
  .init_scan = dialog_init,
#if 0
  .beginning_sql_seq = &sql_create_selected,
#else
  .beginning_sql_seq = &sql_update_selected,
#endif

  .node_scan_before2 = dialog_node_before2,
  .node_scan_before2_deleted = dialog_node_before2_del,

  .node_scan_after2 = dialog_node_after2,
  .node_scan_after2_deleted = dialog_node_after2_del,

  .node_scan_middle2 = dialog_node_middle2,
  .node_scan_middle2_deleted = dialog_node_middle2_del,

  .leaf_scan_fd2 = dialog_de_content2,
  .leaf_scan_fd2_deleted = dialog_de_content2_del,

  .leaf_scan2 = dialog_leaf2,
  .leaf_scan2_deleted = dialog_leaf2_del,

  .dirent_file_scan_before2 = dialog_de_file_before2,
  .dirent_dir_scan_before2 = dialog_de_dir_before2,

/* TODO : exp;ain values of use_std_leaf and use_std_node TODO */
  .use_std_leaf = 1,            /* 1 : preliminary selection; 2 : direct (beginning_sql_seq=NULL recommended in many cases) */
  .use_std_node = 1,            /* 1 : preliminary selection; 2 : direct (beginning_sql_seq=NULL recommended in many cases) */
  .final_sql_seq = &final_sql,
};

/* ########################################################################################## */

static int
dialog_init( duf_stmnt_t * pstmt_unused, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );

  DUF_TRACE( mod, 0, "dialog_init %s", duf_levinfo_path( pdi ) );

  DEBUG_ENDR( r );
}

static int
dialog_de_content2( duf_stmnt_t * pstmt, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );

  assert( 0 == strcmp( DUF_GET_SFIELD2( filename ), duf_levinfo_itemtruename( pdi ) ) );
  assert( duf_levinfo_opened_dh( pdi ) > 0 || duf_levinfo_if_deleted( pdi ) );
  assert( duf_levinfo_stat( pdi ) || duf_levinfo_if_deleted( pdi ) );
  DUF_TRACE( mod, 4, "dialog %s : %s -a-", duf_levinfo_path( pdi ), duf_levinfo_itemtruename( pdi ) );

  DEBUG_ENDR( r );
}

static int
dialog_de_content2_del( duf_stmnt_t * pstmt, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );

  assert( 0 == strcmp( DUF_GET_SFIELD2( filename ), duf_levinfo_itemtruename( pdi ) ) );
  DUF_TRACE( mod, 6, "dialog %s : %s -a-", duf_levinfo_path( pdi ), duf_levinfo_itemtruename( pdi ) );

  DEBUG_ENDR( r );
}

static int
dialog_leaf2( duf_stmnt_t * pstmt, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );

  assert( !duf_levinfo_dfd( pdi ) );
  assert( 0 == strcmp( DUF_GET_SFIELD2( filename ), duf_levinfo_itemtruename( pdi ) ) );
  assert( duf_levinfo_dbstat( pdi ) );

  duf_depthinfo_t di = {
    .pdi_name = "dialog_pdi"
  };
  duf_ufilter_t uf = {
    .use_format = 4,
    .md5id.flag = 1,.md5id.min = DUF_GET_UFIELD2( md5id ),.md5id.max = DUF_GET_UFIELD2( md5id ),
    .sha1id.flag = 1,.sha1id.min = DUF_GET_UFIELD2( sha1id ),.sha1id.max = DUF_GET_UFIELD2( sha1id ),
    .same.md5.flag = 1,.same.md5.min = 2,.same.md5.max = 0,
  };
  if ( uf.md5id.min > 0 && uf.sha1id.min > 0 && DUF_GET_UFIELD2( nsame_md5 ) > 1 && DUF_GET_UFIELD2( nsame_sha1 ) > 1 )
  {
    DOR( r, DUF_WRAPPED( duf_pdi_init ) ( &di, &uf, duf_levinfo_path_top( pdi ) /* duf_levinfo_path( pdi ) */ , NULL /* sql_set */ ,
                                          0 /* caninsert */ ,
                                          1 /* recursive */ ,
                                          0 /* opendir */  ) );
    assert( di.pup == &uf );
    DUF_TEST_R( r );
    /* DOR( r, duf_levinfo_godown_dbopenat_dh( pdi, duf_levinfo_itemtruename( pdi ), 1 (* is_leaf *) , pstmt_files ) ); */
    DOR( r, duf_levinfo_godown_openat_dh( &di, duf_levinfo_itemtruename( pdi ), 1 /* is_leaf */  ) );
    DUF_TEST_R( r );
    if ( DUF_NOERROR( r ) )
    {
      DUF_TRACE( mod, 0, "@@@dialog  %s : %s", duf_levinfo_path( pdi ), duf_levinfo_itemtruename( pdi ) );
      /* "selected" tables should be different!? */
      DOR( r, duf_ev_pdi_evname_std( &di, "listing" /*, &uf */  ) );
      /* DOR_NOE( r, duf_ev_pdi_evname_std( &di, "tree" (*, &uf *)  ), DUF_ERROR_NOT_IN_DB ); */


      /* DUF_PRINTF( 0, "@@@@@@ %s%s", duf_levinfo_relpath( pdi ), duf_levinfo_itemtruename( pdi ) ); */
      {
        DOR( r, duf_find_sccb_by_evname_std( "listing" )->sccb->leaf_scan2( pstmt, pdi ) );
      }
/* TODO: call .leaf_scan2 from "listing" */


      DUF_CLEAR_ERROR( r, DUF_ERROR_NOT_IN_DB );
      DUF_TEST_R( r );
      assert( di.pup == &uf );
    }
    duf_pdi_shut( &di );
  }
  DEBUG_ENDR( r );
}

static int
dialog_leaf2_del( duf_stmnt_t * pstmt, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );

  /* Never called (no deleted flag - didn't try to open !!) */
  DUF_TRACE( mod, 4, "dialog %s : %s -a-", duf_levinfo_path( pdi ), duf_levinfo_itemtruename( pdi ) );

  DEBUG_ENDR( r );
}

static int
dialog_node_before2( duf_stmnt_t * pstmt_unused, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );

  DUF_TRACE( mod, 10, "dialog %s : %s", duf_levinfo_path( pdi ), duf_levinfo_itemshowname( pdi ) );

  DEBUG_ENDR( r );
}

static int
dialog_node_before2_del( duf_stmnt_t * pstmt, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );

  DUF_TRACE( mod, 10, "@dialog node before: %s : %s", duf_levinfo_path( pdi ), duf_levinfo_itemshowname( pdi ) );

  DEBUG_ENDR( r );
}

static int
dialog_node_middle2( duf_stmnt_t * pstmt, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );

  DUF_TRACE( mod, 10, "dialog %s : %s", duf_levinfo_path( pdi ), duf_levinfo_itemshowname( pdi ) );

  DEBUG_ENDR( r );
}

static int
dialog_node_middle2_del( duf_stmnt_t * pstmt, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );

  DUF_TRACE( mod, 10, "@dialog node middle %s : %s", duf_levinfo_path( pdi ), duf_levinfo_itemshowname( pdi ) );

  DEBUG_ENDR( r );
}

static int
dialog_node_after2( duf_stmnt_t * pstmt, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );

  DUF_TRACE( mod, 10, "dialog %s : %s", duf_levinfo_path( pdi ), duf_levinfo_itemshowname( pdi ) );

  DEBUG_ENDR( r );
}

static int
dialog_node_after2_del( duf_stmnt_t * pstmt, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );

  DUF_TRACE( mod, 10, "@dialog node after %s : %s", duf_levinfo_path( pdi ), duf_levinfo_itemshowname( pdi ) );

  DEBUG_ENDR( r );
}

static int
dialog_de_dir_before2( duf_stmnt_t * pstmt_unused, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );

  DUF_TRACE( mod, 8, "dialog de dir before: %s : %s", duf_levinfo_path( pdi ), duf_levinfo_itemshowname( pdi ) );

  DEBUG_ENDR( r );
}

static int
dialog_de_file_before2( duf_stmnt_t * pstmt_unused, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );

  DUF_TRACE( mod, 6, "dialog de file before: %s : %s", duf_levinfo_path( pdi ), duf_levinfo_itemshowname( pdi ) );

  DEBUG_ENDR( r );
}
