/* #define DUF_GET_FIELD_FROM_ROW */
/* #undef MAS_TRACING */
#include <assert.h>                                                  /* assert */
#include <stddef.h>                                                  /* NULL */
#include <string.h>

#include <unistd.h>

#include "duf_tracen_defs_preset.h"                                  /* MAST_TRACE_CONFIG; etc. ✗ */
#include "duf_errorn_defs_preset.h"                                  /* MAST_ERRORS_FILE; etc. ✗ */

#include <mastar/wrap/mas_std_def.h>
#include <mastar/trace/mas_trace.h>
#include <mastar/error/mas_error_defs_ctrl.h>
#include <mastar/error/mas_error_defs_make.h>
#include <mastar/error/mas_error_defs.h>

#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ✗ */

#include "duf_pdi.h"                                                 /* duf_pdi_init; duf_pdi_shut; duf_pdi_close ✗ */
#include "duf_pdi_ref.h"
#include "duf_pdi_structs.h"

#include "duf_levinfo_openclose.h"
#include "duf_levinfo_stat.h"
#include "duf_levinfo_updown.h"
#include "duf_levinfo_ref.h"                                         /* duf_levinfo_*; etc. ✗ */

#include "duf_sccb_structs.h"
#include "duf_sccb_eval_std.h"
#include "duf_sccb_row_field_defs.h"                                 /* DUF_*FIELD2* ✗ */
/* #include "duf_sccb_row.h"                                            (* datarow_* ✗ *) */

#include "duf_sccbh_ref.h"
#include "duf_sccbh_shortcuts.h"                                     /* H_SCCB; H_PDI; H_* ... ✗ */
#include "duf_sccbh_row.h"                                           /* duf_sccbh_row_get_*; sccbh_rows_eval ✗ */

/* #include "duf_path2dirid.h"                                          (* duf_dirid2path; etc. ✗ *) */
/* #include "duf_maindb.h"                                              (* duf_main_db; duf_main_db_open; duf_main_db_close ✗ *) */
#include "duf_ufilter_structs.h"

#include "sql_beginning_selected.h"
#include "sql_beginning_tables.h"                                    /* DUF_SQL_TABLES... etc. ✗ */

#include "duf_mod_defs.h"
/* ########################################################################################## */
#include "duf_mod_types.h"
DUF_MOD_DECLARE_ALL_FUNCS( duf_dialog )
/* ########################################################################################## */
/* ########################################################################################## */
     static duf_scan_callbacks_t duf_sccb_dispatch;

     const duf_mod_handler_t duf_mod_handler_uni[] = {
       {"sccb", &duf_sccb_dispatch},
       {NULL, NULL}
     };

/* ########################################################################################## */
static duf_scan_callbacks_t duf_sccb_dispatch = {
  .title = "dialog",
  .name = "dialog",
  .def_opendir = 0,
  .init_scan = F2ND( dialog_init ),
#if 0
  .beginning_sql_seq = &sql_create_selected,
#else
  .beginning_sql_seq = &sql_update_selected,
#endif

  .node_scan_before2 = F2ND( dialog_node_before2 ),
  .node_scan_before2_deleted = F2ND( dialog_node_before2_del ),

  .node_scan_after2 = F2ND( dialog_node_after2 ),
  .node_scan_after2_deleted = F2ND( dialog_node_after2_del ),

  .node_scan_middle2 = F2ND( dialog_node_middle2 ),
  .node_scan_middle2_deleted = F2ND( dialog_node_middle2_del ),

  .leaf_scan_fd2 = F2ND( dialog_de_content2 ),
  .leaf_scan_fd2_deleted = F2ND( dialog_de_content2_del ),

  .leaf_scan2 = F2ND( dialog_leaf2 ),
  .leaf_scan2_deleted = F2ND( dialog_leaf2_del ),

  .dirent_file_scan_before2 = F2ND( dialog_de_file_before2 ),
  .dirent_dir_scan_before2 = F2ND( dialog_de_dir_before2 ),

/* TODO : explain values of use_std_leaf_set_num and use_std_node_set_num TODO */
  .use_std_leaf_set_num = 1,                                         /* 1 : preliminary selection; 2 : direct (beginning_sql_seq=NULL recommended in many cases) */
  .use_std_node_set_num = 1,                                         /* 1 : preliminary selection; 2 : direct (beginning_sql_seq=NULL recommended in many cases) */
  .std_leaf_set_name = "std-leaf-one",
  .std_node_set_name = "std-node-one",
};

/* ########################################################################################## */

SR( MOD, dialog_init, duf_depthinfo_t * pdi_unused MAS_UNUSED, duf_sccb_handle_t * sccbh MAS_UNUSED )
{
  MAST_TRACE( mod, 0, "dialog_init %s", duf_levinfo_path( H_PDI ) );

  ER( MOD, dialog_init, duf_depthinfo_t * pdi_unused, duf_sccb_handle_t * sccbh MAS_UNUSED );
}

SR( MOD, dialog_de_content2, duf_depthinfo_t * pdi_unused MAS_UNUSED, duf_sccb_handle_t * sccbh MAS_UNUSED )
{
  assert( 0 == strcmp( DUF_GET_RSFIELD2( fname ), duf_levinfo_itemtruename( H_PDI ) ) );
  assert( duf_levinfo_opened_dh( H_PDI ) > 0 || duf_levinfo_if_deleted( H_PDI ) );
  assert( duf_levinfo_stat( H_PDI ) || duf_levinfo_if_deleted( H_PDI ) );
  MAST_TRACE( mod, 4, "dialog %s : %s -a-", duf_levinfo_path( H_PDI ), duf_levinfo_itemtruename( H_PDI ) );

  ER( MOD, dialog_de_content2, duf_depthinfo_t * pdi_unused, duf_sccb_handle_t * sccbh MAS_UNUSED );
}

SR( MOD, dialog_de_content2_del, duf_depthinfo_t * pdi_unused MAS_UNUSED, duf_sccb_handle_t * sccbh MAS_UNUSED )
{
  assert( 0 == strcmp( DUF_GET_RSFIELD2( fname ), duf_levinfo_itemtruename( H_PDI ) ) );
  MAST_TRACE( mod, 6, "dialog %s : %s -a-", duf_levinfo_path( H_PDI ), duf_levinfo_itemtruename( H_PDI ) );

  ER( MOD, dialog_de_content2_del, duf_depthinfo_t * pdi_unused, duf_sccb_handle_t * sccbh MAS_UNUSED );
}

SR( MOD, dialog_leaf2, /* duf_stmnt_t * pstmt_arg_spc, */ duf_depthinfo_t * pdi_unused MAS_UNUSED, duf_sccb_handle_t * sccbh MAS_UNUSED )
{
  assert( !duf_levinfo_dfd( H_PDI ) );
  assert( 0 == strcmp( DUF_GET_RSFIELD2( fname ), duf_levinfo_itemtruename( H_PDI ) ) );
  assert( duf_levinfo_dbstat( H_PDI ) || duf_levinfo_stat( H_PDI ) );
  duf_depthinfo_t di = {
    .pdi_name = "dialog_pdi"
  };
  duf_ufilter_t uf = {
    .use_format = 5,

    .md5id.flag = 1,.md5id.min = DUF_GET_RUFIELD2( md5id ),.md5id.max = DUF_GET_RUFIELD2( md5id ),
    .same.md5.flag = 1,.same.md5.min = 2,.same.md5.max = 0,

    .sha1id.flag = 1,.sha1id.min = DUF_GET_RUFIELD2( sha1id ),.sha1id.max = DUF_GET_RUFIELD2( sha1id ),
    .same.sha1.flag = 1,.same.sha1.min = 2,.same.sha1.max = 0,
  };
  if ( uf.md5id.min > 0 && uf.sha1id.min > 0 && DUF_GET_RUFIELD2( nsame_md5 ) > 1 && DUF_GET_RUFIELD2( nsame_sha1 ) > 1 )
  {
  /* QT("@@@@@@top:%s", duf_levinfo_path_top( H_PDI )); */
    CR( pdi_init, &di, &uf, duf_levinfo_path_top( H_PDI ) /* duf_levinfo_path( H_PDI ) */ , NULL /* sql_set */ ,
        0 /* caninsert */ ,
        1 /* recursive */ ,
        1 /* allow_dirs */ ,
        0 /* linear */ ,
        0 /* opendir */  );
    assert( di.pup == &uf );
    QT( "@dialog 1 next:%p", di.next );

  /* DOR( r, duf_levinfo_godown_dbopenat_dh( H_PDI, duf_levinfo_itemtruename( H_PDI ), DUF_NODE_LEAF (* node_type *) , pstmt_files ) ); */

#if 0
  /* ??? */
    CR( levinfo_godown_openat_dh, &di, duf_levinfo_itemtruename( H_PDI ), DUF_NODE_LEAF /* node_type */  );
  /* /??? */
#endif

    if ( QNOERR )
    {
      MAST_TRACE( mod, 0, "@@@dialog  %s : %s", duf_levinfo_path( H_PDI ), duf_levinfo_itemtruename( H_PDI ) );
    /* "selected" tables should be different!? */
      CR( ev_pdi_evname_std, &di, "listing" /*, &uf */  );
      MAST_TRACE( mod, 0, "@@@@dialog  %s : %s", duf_levinfo_path( H_PDI ), duf_levinfo_itemtruename( H_PDI ) );
    /* DOR_NOE( r, duf_ev_pdi_evname_std( &di, "tree" (*, &uf *)  ), DUF_ERROR_NOT_IN_DB ); */

    /* DUF_PRINTF( 0, "@@@@@@ %s%s", duf_levinfo_relpath( H_PDI ), duf_levinfo_itemtruename( H_PDI ) ); */
      {
#if 0
        CR( find_atable_sccb_by_evname_std, "listing" )->sccb->leaf_scan2( pstmt_arg_spc, H_PDI );
#else
        duf_scanner_fun_t scanner = duf_find_sccb_by_evname_std( "listing" )->leaf_scan2;

        CRV( scanner, /* pstmt_arg_spc, */ H_PDI, sccbh );
#endif
        MAST_TRACE( mod, 0, "@@@@dialog  %s : %s", duf_levinfo_path( H_PDI ), duf_levinfo_itemtruename( H_PDI ) );
      }
/* TODO: call .leaf_scan2 from "listing" */

      ERRCLEAR( NOT_IN_DB );

      assert( di.pup == &uf );
    }
    QT( "@dialog 2 next:%p", di.next );
#if 0
    duf_pdi_shut( &di );
#else
    duf_pdi_close( &di );
#endif
    QT( "@dialog 3 next:%p", di.next );
    assert( !di.next );
  }

  ER( MOD, dialog_leaf2, /* duf_stmnt_t * pstmt_arg_spc, */ duf_depthinfo_t * pdi_unused, duf_sccb_handle_t * sccbh MAS_UNUSED );
}

SR( MOD, dialog_leaf2_del, duf_depthinfo_t * pdi_unused MAS_UNUSED, duf_sccb_handle_t * sccbh MAS_UNUSED )
{
/* Never called (no deleted flag - didn't try to open !!) */
  MAST_TRACE( mod, 4, "dialog %s : %s -a-", duf_levinfo_path( H_PDI ), duf_levinfo_itemtruename( H_PDI ) );

  ER( MOD, dialog_leaf2_del, duf_depthinfo_t * pdi_unused, duf_sccb_handle_t * sccbh MAS_UNUSED );
}

SR( MOD, dialog_node_before2, duf_depthinfo_t * pdi_unused MAS_UNUSED, duf_sccb_handle_t * sccbh MAS_UNUSED )
{
  MAST_TRACE( mod, 10, "dialog %s : %s", duf_levinfo_path( H_PDI ), duf_levinfo_itemshowname( H_PDI ) );

  ER( MOD, dialog_node_before2, duf_depthinfo_t * pdi_unused, duf_sccb_handle_t * sccbh MAS_UNUSED );
}

SR( MOD, dialog_node_before2_del, duf_depthinfo_t * pdi_unused MAS_UNUSED, duf_sccb_handle_t * sccbh MAS_UNUSED )
{
  MAST_TRACE( mod, 10, "@dialog node before: %s : %s", duf_levinfo_path( H_PDI ), duf_levinfo_itemshowname( H_PDI ) );

  ER( MOD, dialog_node_before2_del, duf_depthinfo_t * pdi_unused, duf_sccb_handle_t * sccbh MAS_UNUSED );
}

SR( MOD, dialog_node_middle2, duf_depthinfo_t * pdi_unused MAS_UNUSED, duf_sccb_handle_t * sccbh MAS_UNUSED )
{
  MAST_TRACE( mod, 10, "dialog %s : %s", duf_levinfo_path( H_PDI ), duf_levinfo_itemshowname( H_PDI ) );

  ER( MOD, dialog_node_middle2, duf_depthinfo_t * pdi_unused, duf_sccb_handle_t * sccbh MAS_UNUSED );
}

SR( MOD, dialog_node_middle2_del, duf_depthinfo_t * pdi_unused MAS_UNUSED, duf_sccb_handle_t * sccbh MAS_UNUSED )
{
  MAST_TRACE( mod, 10, "@dialog node middle %s : %s", duf_levinfo_path( H_PDI ), duf_levinfo_itemshowname( H_PDI ) );

  ER( MOD, dialog_node_middle2_del, duf_depthinfo_t * pdi_unused, duf_sccb_handle_t * sccbh MAS_UNUSED );
}

SR( MOD, dialog_node_after2, duf_depthinfo_t * pdi_unused MAS_UNUSED, duf_sccb_handle_t * sccbh MAS_UNUSED )
{
  MAST_TRACE( mod, 10, "dialog %s : %s", duf_levinfo_path( H_PDI ), duf_levinfo_itemshowname( H_PDI ) );

  ER( MOD, dialog_node_after2, duf_depthinfo_t * pdi_unused, duf_sccb_handle_t * sccbh MAS_UNUSED );
}

SR( MOD, dialog_node_after2_del, duf_depthinfo_t * pdi_unused MAS_UNUSED, duf_sccb_handle_t * sccbh MAS_UNUSED )
{
  MAST_TRACE( mod, 10, "@dialog node after %s : %s", duf_levinfo_path( H_PDI ), duf_levinfo_itemshowname( H_PDI ) );

  ER( MOD, dialog_node_after2_del, duf_depthinfo_t * pdi_unused, duf_sccb_handle_t * sccbh MAS_UNUSED );
}

SR( MOD, dialog_de_dir_before2, duf_depthinfo_t * pdi_unused MAS_UNUSED, duf_sccb_handle_t * sccbh MAS_UNUSED )
{
  MAST_TRACE( mod, 8, "dialog de dir before: %s : %s", duf_levinfo_path( H_PDI ), duf_levinfo_itemshowname( H_PDI ) );

  ER( MOD, dialog_de_dir_before2, duf_depthinfo_t * pdi_unused, duf_sccb_handle_t * sccbh MAS_UNUSED );
}

SR( MOD, dialog_de_file_before2, duf_depthinfo_t * pdi_unused MAS_UNUSED, duf_sccb_handle_t * sccbh MAS_UNUSED )
{
  MAST_TRACE( mod, 6, "dialog de file before: %s : %s", duf_levinfo_path( H_PDI ), duf_levinfo_itemshowname( H_PDI ) );

  ER( MOD, dialog_de_file_before2, duf_depthinfo_t * pdi_unused, duf_sccb_handle_t * sccbh MAS_UNUSED );
}
