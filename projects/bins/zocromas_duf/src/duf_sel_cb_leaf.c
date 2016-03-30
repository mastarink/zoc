#include <assert.h>                                                  /* assert */
#include <string.h>
#include <unistd.h>

#include "duf_tracen_defs_preset.h"                                  /* MAST_TRACE_CONFIG; etc. ✗ */
#include "duf_errorn_defs_preset.h"                                  /* MAST_ERRORS_FILE; etc. ✗ */

#include <mastar/tools/mas_arg_tools.h>                              /* mas_strcat_x; etc. ▤ */
#include <mastar/trace/mas_trace.h>
#include <mastar/error/mas_error_defs_ctrl.h>
#include <mastar/error/mas_error_defs_make.h>
#include <mastar/error/mas_error_defs.h>

#include "duf_se_only.h"                                             /* Only DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ✗ */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ✗ */

#include "duf_levinfo.h"                                             /* duf_levinfo_calc_depth; duf_levinfo_clear_level_d; etc. ✗ */
#include "duf_levinfo_ref.h"                                         /* duf_levinfo_*; etc. ✗ */
#include "duf_levinfo_updown.h"                                      /* duf_levinfo_goup */

#include "duf_pdi_pi_ref.h"                                          /* duf_pdi_depth */
#include "duf_pdi_ref.h"

#include "duf_sccb_def.h"                                            /* DUF_SCCB_PDI */
#include "duf_sccb.h"                                                /* duf_uni_scan_action_title, contained at DUF_SCCB_PDI */

#include "duf_sccbh_eval_leaf.h"                                     /* duf_sccbh_eval_db_leaf_fd_str_cb and duf_sccbh_eval_db_leaf_str_cb */

#include "duf_sccbh_shortcuts.h"                                     /* H_PDI etc. */
#include "duf_sccbh_pstmt.h"

#include "duf_sccb_structs.h"
#include "duf_sccb_scanstage.h"                                      /* duf_nodetype_name; duf_scanstage_name; duf_scanstage_scanner; ✗ */

#include "duf_levinfo_credel.h"                                      /* duf_levinfo_create; duf_levinfo_delete ✗ */
#include "duf_li_credel.h"

/* #include "duf_sccb_row.h" */

/* ###################################################################### */
#include "duf_sel_cb_leaf.h"
/* ###################################################################### */

/* 20151027.114003 */
/* DUF_WRAPSTATIC */
static
SR( SCCBH, sel_cb2_leaf_at, duf_sccb_handle_t * sccbh, duf_stmnt_t * pstmt, duf_str_cb2_t str_cb2, duf_scanstage_t scanstage )
{
/* DUF_STARTR( r ); */
  if ( str_cb2 )
  {
    MAST_TRACE( explain, 20, "=> str cb2" );
    DUF_SCCB_PDI( MAST_TRACE, scan, 10 + CRX( pdi_reldepth, H_PDI ), H_PDI, " >>> 5. leaf str cb2" );

    assert( str_cb2 == F2ND( sccbh_eval_db_leaf_fd_str_cb ) || str_cb2 == F2ND( sccbh_eval_db_leaf_str_cb ) );
    assert( CRX( pdi_depth, H_PDI ) == CRX( levinfo_calc_depth, H_PDI ) );

    if ( !( CRX( levinfo_dirid, H_PDI ) == CRX( levinfo_dirid_up, H_PDI ) && CRX( pdi_depth, H_PDI ) == CRX( levinfo_calc_depth, H_PDI ) ) )
    {
      duf_levinfo_t *pli = NULL;

    /* assert( duf_levinfo_closed( H_PDI ) ); */

      pli = CRP( nameid2li, CRX( levinfo_nameid, H_PDI ), CRX( pdi_maxdepth, H_PDI ) );
      if ( QNOERR )
        CRX( levinfo_set_li, H_PDI, pli, CRX( pdi_maxdepth, H_PDI ) );
      else
        CRX( li_delete_array, pli, CRX( pdi_maxdepth, H_PDI ) );
    }
    assert( CRX( pdi_depth, H_PDI ) > 0 );
    assert( CRX( pdi_levinfo, H_PDI ) );
    assert( CRX( levinfo_dirid, H_PDI ) == CRX( levinfo_dirid_up, H_PDI ) );
    assert( CRX( pdi_depth, H_PDI ) == CRX( levinfo_calc_depth, H_PDI ) );
    {
      assert( str_cb2 == F2ND( sccbh_eval_db_leaf_fd_str_cb ) || str_cb2 == F2ND( sccbh_eval_db_leaf_str_cb ) );
      CRV( ( str_cb2 ), sccbh, pstmt, scanstage );
    }

    if ( QNOERR )
    {
#if 0
      H_PDI->seq++;
      H_PDI->seq_leaf++;
#else
      CRX( pdi_seq_leaf_plus, H_PDI );
#endif
      MAST_TRACE( seq, 0, "seq:%llu; seq_leaf:%llu", CRX( pdi_seq, H_PDI ) /* H_PDI->seq */ , CRX( pdi_seq_leaf, H_PDI ) /* H_PDI->seq_leaf */  );
    }
    if ( sccbh->progress_leaf_cb )
      ( sccbh->progress_leaf_cb ) ( sccbh );
  }
/* DUF_ENDR( r ); */
  ER( SCCBH, sel_cb2_leaf_at, duf_sccb_handle_t * sccbh, duf_stmnt_t * pstmt, duf_str_cb2_t str_cb2, duf_scanstage_t scanstage );
}

/* duf_sel_cb_leaves:
 * this is callback of type: duf_sel_cb_t (first range): 
 *
 * called with precord
 * str_cb + str_cb_udata to be called for precord with correspondig args
 *
 * known str_cb for duf_sel_cb_leaf:
 *   duf_str_cb_leaf_scan;   duf_str_cb_leaf_scan is just a wrapper for sccb->leaf_scan
 *   duf_str_cb_scan_file_fd;  duf_str_cb_scan_file_fd is just a wrapper for sccb->leaf_scan_fd ; str_cb_udata_unused
 *   duf_str_cb_uni_scan_dir (in theory ?!)
 *   ...
 * */
/* 20150820.085847 */
SR( SCCBH, sel_cb2_leaf, duf_sccb_handle_t * sccbh, duf_stmnt_t * pstmt, duf_str_cb2_t str_cb2, duf_scanstage_t scanstage )
{
/* DUF_STARTR( r ); */
  assert( H_PDI );
  assert( CRX( pdi_depth, H_PDI ) >= 0 );
  assert( CRX( pdi_depth, H_PDI ) == CRX( levinfo_calc_depth, H_PDI ) );
/* data from db at pstmt */
  MAST_TRACE( scan, 10, "  " DUF_DEPTH_PFMT ": =====> scan leaf2", CRX( pdi_depth, H_PDI ) );
  MAST_TRACE( explain, 40, "@ sel cb2 leaf" );
  assert( str_cb2 == F2ND( sccbh_eval_db_leaf_str_cb ) || str_cb2 == F2ND( sccbh_eval_db_leaf_fd_str_cb ) || str_cb2 == NULL );
  MAST_TRACE( scan, 9, "LEAF %s", CRX( levinfo_path, H_PDI ) );
  {
  /*@ 1. go down + dbopenat */
    CR( sccbh_pstmt_godown_dbopenat_dh, sccbh, pstmt, DUF_NODE_LEAF /* node_type */  );
    MAST_TRACE( scan, 9, "(%s) LEAF down %s", QERRNAME, CRX( levinfo_path, H_PDI ) );
    assert( CRX( pdi_depth, H_PDI ) >= 0 );
    CR( sel_cb2_leaf_at, sccbh, pstmt, str_cb2, scanstage );
    assert( CRX( pdi_depth, H_PDI ) == CRX( levinfo_calc_depth, H_PDI ) );
    CR( levinfo_goup, H_PDI );
  }
  MAST_TRACE( scan, 9, "/LEAF %s", CRX( levinfo_path, H_PDI ) );
/* DUF_ENDR( r ); */
  ER( SCCBH, sel_cb2_leaf, duf_sccb_handle_t * sccbh, duf_stmnt_t * pstmt, duf_str_cb2_t str_cb2, duf_scanstage_t scanstage );
}
