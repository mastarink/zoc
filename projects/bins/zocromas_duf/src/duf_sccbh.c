#include <string.h>

#include <assert.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>


#include "duf_maintenance.h"

#include "duf_config_ref.h"

#include "duf_option_defs.h"
#include "duf_levinfo_ref.h"

#include "duf_dir_scan2.h"

#include "duf_sccb_def.h"

#include "duf_sccbh_shortcuts.h"
#include "duf_sccb.h"
/* ###################################################################### */
#include "duf_sccbh.h"
/* ###################################################################### */

int
duf_sccbh_eval_pdi( duf_sccb_handle_t * sccbh )
{
  DEBUG_STARTR( r );

  assert( PDI );
  assert( PDI->depth >= 0 );

  DUF_TRACE( explain, 0,
             "≫≫≫≫≫≫≫≫≫≫  to scan %" "s" /* DUF_ACTION_TITLE_FMT */ " ≪≪≪≪≪≪≪≪≪≪≪≪≪≪≪≪≪",
             duf_uni_scan_action_title( SCCB ) );
  DUF_SCCB( DUF_TRACE, scan, 0, "scanning: top dirID: %llu; path: %s;", duf_levinfo_dirid( PDI ), duf_levinfo_path( PDI ) );
  DUF_E_NO( DUF_ERROR_TOO_DEEP );
/* duf_scan_dirs_by_pdi_maxdepth:
 * if recursive, call duf_scan_dirs_by_parentid + pdi (built from str_cb_udata)
 *       for each <dir> record by top dirID (i.e. children of top dirID) with corresponding args
 * otherwise do nothing
 *
 * call duf_str_cb(1?)_uni_scan_dir with pdi for each dir at db by top dirID (i.e. children of top dirID)
 *
 *   i.e.
 *     1. for <current> dir call sccb->node_scan_before
 *     2. for each leaf in <current> dir call sccb->leaf_scan
 *     3. for <current> dir call sccb->node_scan_middle
 *   recursively from <current> dir (if recursive flag set):
 *     4. for each dir in <current> dir call duf_str_cb(1?)_uni_scan_dir + &di as str_cb_udata
 *     5. for <current> dir call sccb->node_scan_after
 * */

  DUF_TRACE( scan, 4, "+" );
  if ( !SCCB->disabled )
    DOR( r, duf_scan_dirs_by_pdi_wrap( ( duf_sqlite_stmt_t * ) NULL, sccbh ) );

  /* delete level-control array, close 0 level */

  /* if ( pchanges )
   *pchanges += di.changes; */


  if ( r >= 0 && DUF_ACT_FLAG( summary ) )
  {
    DUF_PRINTF( 0, "%s", duf_uni_scan_action_title( SCCB ) );

    DUF_PRINTF( 0, " summary; seq:     %llu", PDI->seq );
    DUF_PRINTF( 0, " summary; seq-leaf:%llu", PDI->seq_leaf );
    DUF_PRINTF( 0, " summary; seq-node:%llu", PDI->seq_node );
    if ( PU->max_seq )
      DUF_PRINTF( 0, " of %llu (max-seq)", PU->max_seq );
  }
  DUF_E_YES( DUF_ERROR_TOO_DEEP );
  DEBUG_ENDR( r );
}