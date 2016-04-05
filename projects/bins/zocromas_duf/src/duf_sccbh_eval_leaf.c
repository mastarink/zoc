/* #undef MAS_TRACING */
#include <assert.h>
#include <string.h>
#include <stddef.h>

#include "duf_tracen_defs_preset.h"                                  /* MAST_TRACE_CONFIG; etc. ✗ */
#include "duf_errorn_defs_preset.h"                                  /* MAST_ERRORS_FILE; etc. ✗ */

#include <mastar/trace/mas_trace.h>
#include <mastar/error/mas_error_defs_ctrl.h>
#include <mastar/error/mas_error_defs_make.h>
#include <mastar/error/mas_error_defs.h>

#include "duf_se_only.h"                                             /* Only DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ✗ */

#include "duf_config.h"                                              /* duf_get_config ✗ */
#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ✗ */

#include "duf_levinfo_ref.h"                                         /* duf_levinfo_*; etc. ✗ */
#include "duf_levinfo_openclose.h"
#include "duf_levinfo_stat.h"

#include "duf_sccb.h"
#include "duf_sccb_structs.h"
#include "duf_sccb_scanstage.h"                                      /* duf_scanstage_name; duf_scanstage_scanner; ✗ */
#include "duf_sccb_row.h"                                            /* datarow_*; duf_sccbh_row_get_*; sccbh_rows_eval ✗ */

#include "duf_sccbh_shortcuts.h"                                     /* H_SCCB; H_PDI; H_* ... ✗ */

#include "duf_sccbh_eval.h"
/* ###################################################################### */
#include "duf_sccbh_eval_leaf.h"                                     /* duf_sccbh_eval_db_leaf_str_cb; duf_sccbh_eval_db_leaf_fd_str_cb; ✗ */
/* ###################################################################### */

SR( SCCBH, sccbh_eval_db_leaf_qfd_str_cb, duf_sccb_handle_t * sccbh, duf_stmnt_t * pstmt_arg, duf_scanstage_t scanstage )
{
#if 0
  H_PDI->items.total++;
  H_PDI->items.files++;
#endif
  if ( scanstage == DUF_SCANSTAGE_DB_LEAVES_FD )
  {
    ERRLOWER( FS_DISABLED );
    CR( levinfo_if_openat_dh, H_PDI );
    {
      ERRLOWER( STATAT_ENOENT );
      CR( levinfo_if_statat_dh, H_PDI );
      ERRUPPER( STATAT_ENOENT );
    }
    ERRUPPER( FS_DISABLED );
/* DOR_LOWERE( r, CRX(levinfo_if_openat_dh, H_PDI ), DUF_ERROR_FS_DISABLED );                          */
/* DOR_LOWERE( r, CRX(levinfo_if_statat_dh, H_PDI ), DUF_ERROR_FS_DISABLED, DUF_ERROR_STATAT_ENOENT ); */
#if 1
    if ( CRX( levinfo_deleted, H_PDI ) )
    {
      ERRCLEAR( OPENAT_ENOENT );
      ERRCLEAR( STATAT_ENOENT );
    }
#endif
  }

  for ( H_SCCBI = 0; H_SCCB; H_SCCBI++ )
  {
    duf_scanner_fun_t scanner = NULL;

    scanner = CRX( sccb_scanstage_scanner, H_SCCB, scanstage, CRX( levinfo_deleted, H_PDI ), DUF_NODE_LEAF );
    {
      ERRLOWER( FS_DISABLED );

      CR( sccbh_call_scanner, sccbh, pstmt_arg, scanstage, scanner, DUF_NODE_LEAF );

      assert( sccbh->assert__current_node_type == DUF_NODE_LEAF );
      ERRUPPER( FS_DISABLED );
    }
  }
  H_SCCBI = 0;
  ER( SCCBH, sccbh_eval_db_leaf_qfd_str_cb, duf_sccb_handle_t * sccbh, duf_stmnt_t * pstmt_arg, duf_scanstage_t scanstage );
}

/* 20151027.144543 */
/*
 * this is callback of type: duf_str_cb_t (first range; str_cb)
 *
 * duf_str_cb_scan_file_fd is just a wrapper for sccb->leaf_scan_fd
 *
 *  - pstmt_arg - for 1 leaf data, obtained from db
 *  - pdi
 *  - sccb
 * */
SR( SCCBH, sccbh_eval_db_leaf_fd_str_cb, duf_sccb_handle_t * sccbh, duf_stmnt_t * pstmt_arg, duf_scanstage_t scanstage )
{
#if 1
# if 0
  H_PDI->items.total++;
  H_PDI->items.files++;
# endif
  {
    ERRLOWER( FS_DISABLED );
    CR( levinfo_if_openat_dh, H_PDI );
    {
      ERRLOWER( STATAT_ENOENT );
      CR( levinfo_if_statat_dh, H_PDI );
      ERRUPPER( STATAT_ENOENT );
    }
    ERRUPPER( FS_DISABLED );
  }
/* DOR_LOWERE( r, CRX(levinfo_if_openat_dh, H_PDI ), DUF_ERROR_FS_DISABLED );                          */
/* DOR_LOWERE( r, CRX(levinfo_if_statat_dh, H_PDI ), DUF_ERROR_FS_DISABLED, DUF_ERROR_STATAT_ENOENT ); */
# if 1
  if ( CRX( levinfo_deleted, H_PDI ) )
  {
    ERRCLEAR( OPENAT_ENOENT );
    ERRCLEAR( STATAT_ENOENT );
  }
# endif
  for ( H_SCCBI = 0; H_SCCB; H_SCCBI++ )
  {
    duf_scanner_fun_t scanner = NULL;

    scanner = CRX( sccb_scanstage_scanner, H_SCCB, scanstage, CRX( levinfo_deleted, H_PDI ), DUF_NODE_LEAF );
    {
      ERRLOWER( FS_DISABLED );

      CR( sccbh_call_scanner, sccbh, pstmt_arg, scanstage, scanner, DUF_NODE_LEAF );

      assert( sccbh->assert__current_node_type == DUF_NODE_LEAF );
      ERRUPPER( FS_DISABLED );
    }
  }
  H_SCCBI = 0;
#else
  CR( sccbh_eval_db_leaf_qfd_str_cb, sccbh, pstmt_arg, scanstage );
#endif
  ER( SCCBH, sccbh_eval_db_leaf_fd_str_cb, duf_sccb_handle_t * sccbh, duf_stmnt_t * pstmt_arg, duf_scanstage_t scanstage );
}

/*20150820.085324
 * this is callback of type: duf_str_cb_t (first range; str_cb)
 *
 * duf_str_cb_leaf_scan is just a wrapper for sccb->leaf_scan
 *
 *  - pstmt_arg - for 1 leaf data, obtained from db
 *  - pdi
 *  - sccb
 * */
SR( SCCBH, sccbh_eval_db_leaf_str_cb, duf_sccb_handle_t * sccbh, duf_stmnt_t * pstmt_arg, duf_scanstage_t scanstage )
{
#if 1
# if 0
  H_PDI->items.total++;
  H_PDI->items.files++;
# endif
  for ( H_SCCBI = 0; H_SCCB; H_SCCBI++ )
  {
    duf_scanner_fun_t scanner = NULL;

    scanner = CRX( sccb_scanstage_scanner, H_SCCB, scanstage, CRX( levinfo_deleted, H_PDI ), DUF_NODE_LEAF );
    {
      ERRLOWER( FS_DISABLED );

      CR( sccbh_call_scanner, sccbh, pstmt_arg, scanstage, scanner, DUF_NODE_LEAF );

      ERRUPPER( FS_DISABLED );
    }
  }
  H_SCCBI = 0;
#else
  CR( sccbh_eval_db_leaf_qfd_str_cb, sccbh, pstmt_arg, scanstage );
#endif
  ER( SCCBH, sccbh_eval_db_leaf_str_cb, duf_sccb_handle_t * sccbh, duf_stmnt_t * pstmt_arg, duf_scanstage_t scanstage );
}
