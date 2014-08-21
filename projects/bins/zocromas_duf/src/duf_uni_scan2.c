#define DUF_SQL_PDI_STMT

#include <string.h>
#include <assert.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_trace_defs.h"
#include "duf_debug_defs.h"


#include "duf_types.h"

#include "duf_utils.h"
#include "duf_service.h"
#include "duf_config_ref.h"

#include "duf_pdi.h"
#include "duf_levinfo.h"

#include "duf_options.h"
/* #include "duf_cli_options.h" */


#include "duf_sql.h"
#include "duf_sql2.h"

#include "duf_path.h"

#include "duf_sccb.h"

#include "duf_dbg.h"

#include "duf_dir_scan2.h"

#include "duf_uni_scan.h"

/* ###################################################################### */
#include "duf_uni_scan2.h"
/* ###################################################################### */


/*
 * run   --uni-scan /home/mastar/a/down/ --max-depth=4  --max-items=70 -R --tree --print  --files
 * run   --uni-scan /home/mastar/a/down/ --max-depth=4  --max-items=70 -R --tree --print
*/


/* duf_str_cb(1?)_uni_scan_dir:
 * this is callback of type: duf_str_cb_t (second range):
 *
 * if recursive flag set, call duf_scan_dirs_by_parentid(2?) + pdi (built from str_cb_udata) with duf_str_cb(1?)_uni_scan_dir
 *       for each <dir> record by dirid (i.e. children of dirid) with corresponding args 
 * otherwise do nothing
 *
 *   i.e.
 *     1. for <current> dir call sccb->node_scan_before
 *     2. for each leaf in <current> dir call sccb->leaf_scan
 *     3. for <current> dir call sccb->node_scan_middle
 *   recursively from <current> dir (if recursive flag set):
 *     4. for each dir in <current> dir call duf_str_cb(1?)_uni_scan_dir + &di as str_cb_udata
 *     5. for <current> dir call sccb->node_scan_after
 * */

int
duf_str_cb2_uni_scan_dir( duf_sqlite_stmt_t * pstmt, duf_depthinfo_t * pdi, duf_scan_callbacks_t * sccb )
{
  int r = 0;

  DEBUG_START(  );

  assert( pdi );

  DUF_TRACE( scan, 10, "+" );
  if ( r >= 0 )
  {
    if ( DUF_U_FLAG( recursive ) && ( !pdi->u.maxreldepth || duf_pdi_reldepth( pdi ) < pdi->u.maxreldepth ) )
    {
/* duf_scan_fil_by_pi:
 * call duf_str_cb(2?)_uni_scan_dir + pdi (also) as str_cb_udata for each <dir> record by dirid (i.e. children of dirid) with corresponding args
 *
 * i.e. if recursive, call duf_scan_dirs_by_parentid + pdi (built from str_cb_udata) with duf_str_cb(1?)_uni_scan_dir
 *       for each <dir> record by dirid (i.e. children of dirid) with corresponding args 
 *         otherwise do nothing
 *
 *   i.e.
 *     1. for <current> dir call sccb->node_scan_before
 *     2. for each leaf in <current> dir call sccb->leaf_scan
 *     3. for <current> dir call sccb->node_scan_middle
 *   recursively from <current> dir (if recursive flag set):
 *     4. for each dir in <current> dir call str_cb + str_cb_udata
 *     5. for <current> dir call sccb->node_scan_after
 * */
      if ( duf_pdi_reldepth( pdi ) <= 1 )
      {
        DUF_TRACE( explain, 3, "cb2; option %s; option %s value %d; depth %d",
                   duf_option_cnames_tmp( DUF_OPTION_FLAG_RECURSIVE ), duf_option_cnames_tmp( DUF_OPTION_MAXDEPTH ), pdi->u.maxreldepth,
                   duf_pdi_reldepth( pdi ) );
      }
      else
      {
        DUF_TRACE( explain, 4, "cb2; option %s; option %s value %d; depth %d",
                   duf_option_cnames_tmp( DUF_OPTION_FLAG_RECURSIVE ), duf_option_cnames_tmp( DUF_OPTION_MAXDEPTH ), pdi->u.maxreldepth,
                   duf_pdi_reldepth( pdi ) );
      }

      DUF_OINV_OPENED( pdi-> );
      DUF_TRACE( scan, 10, "+" );
      r = duf_scan_dirs_by_parentid2( pstmt, duf_str_cb2_uni_scan_dir, pdi, sccb );
    }
    else
    {
      DUF_TRACE( explain, 1, "no %s option or depth condition by %s value %d", duf_option_cnames_tmp( DUF_OPTION_FLAG_RECURSIVE ),
                 duf_option_cnames_tmp( DUF_OPTION_MAXDEPTH ), pdi->u.maxreldepth );
    }

  }
  DUF_TEST_R( r );
  DEBUG_END(  );
  return r;
}