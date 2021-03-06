/* #undef MAS_TRACING */
#include <assert.h>

#include "duf_tracen_defs_preset.h"                                  /* MAST_TRACE_CONFIG; etc. ✗ */
#include "duf_errorn_defs_preset.h"                                  /* MAST_ERRORS_FILE; etc. ✗ */

#include <mastar/trace/mas_trace.h>
#include <mastar/error/mas_error_defs_ctrl.h>
#include <mastar/error/mas_error_defs_make.h>
#include <mastar/error/mas_error_defs.h>

#include <mastar/multiconfig/muc_option_config.h>

#include "duf_se_only.h"                                             /* Only DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ✗ */

#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ✗ */

#include "duf_sccb.h"

#include "duf_pdi_global.h"                                          /* duf_pdi_global() ✗ */
#include "duf_pdi_sccb_eval.h"
/* ###################################################################### */
#include "duf_sccb_eval.h"
/* ###################################################################### */

#if 0
static int
duf_ev_sccb( const duf_scan_callbacks_t * sccb )
{
  DUF_STARTR( r );
  assert( duf_pdi_global_name(  ) );
  MAST_TRACE( sccb, 0, "evaluate name %s [%s]", sccb->name, duf_pdi_global_name(  ) );

/* T( "sccb:%d; dirid:%llu", sccb ? 1 : 0, duf_levinfo_dirid( duf_pdi_global() ) ); */
  DOR( r, duf_ev_pdi_sccb( duf_pdi_global(  ), sccb, DUF_CONFIGA( pcli->targ ), DUF_ACTG_FLAG( summary ) ) );
  DUF_ENDR( r );
}

int
duf_ev_evnamen( const char *name, size_t len, duf_scan_callbacks_t * first )
{
  DUF_STARTR( r );
  assert( duf_pdi_global_name(  ) );
  DOR( r, duf_ev_pdi_evnamen( duf_pdi_global(  ), name, len, first, DUF_CONFIGA( pcli->targ ), DUF_ACTG_FLAG( summary ) ) );
  DUF_ENDR( r );
}

int
duf_ev_evname( const char *name, duf_scan_callbacks_t * first )
{
  DUF_STARTR( r );
  assert( duf_pdi_global_name(  ) );
  MAST_TRACE( sccb, 0, "evaluate name %s [%s]", name, duf_pdi_global_name(  ) );
  DOR( r, duf_ev_pdi_evname( duf_pdi_global(  ), name, first, DUF_CONFIGA( pcli->targ ), DUF_ACTG_FLAG( summary ) ) );
  DUF_ENDR( r );
}
#endif
SR( OTHER, ev_evnamed_list, const char *names, duf_scan_callbacks_t * first )
{
/* DUF_STARTR( r ); */
  assert( duf_pdi_global_name(  ) );
/* assert( duf_pdi_global()->pyp ); */
  MAST_TRACE( sccb, 0, "evaluate sccb list '%s' [%s]", names, duf_pdi_global_name(  ) );
/* QT( "names:%s; dirid:%llu", names, duf_levinfo_dirid( duf_pdi_global() ) ); */
  CR( ev_pdi_evnamed_list, duf_pdi_global(  ), names, first, /* DUF_CONFIGA( pcli->targ ) */ muc_cli_options_get_carg( duf_get_config_cli(  ) ),
      duf_get_config_flag_act_summary(  ) );
/* DUF_ENDR( r ); */
  ER( OTHER, ev_evnamed_list, const char *names, duf_scan_callbacks_t * first );
}

#if 0
static int
duf_show_sccb_sequence( duf_scan_callbacks_t ** psccbs, int sccb_num )
{
  DUF_STARTR( r );
  DUF_PRINTF( 0, "SCCBS" );
  for ( int astep = 0; DUF_NOERROR( r ) && astep < sccb_num; astep++ )
  {
    if ( psccbs[astep] )
    {
      DUF_PRINTF( 0, "%d. %s", astep, psccbs[astep]->title );
    }
  }
  DUF_ENDR( r );
}
#endif
/*
 * for each sccb from sequence/list (with given size - # of items) make/evaluate the task
 *    by calling duf_ev_sccb
 * */
#if 0
int
duf_ev_sccb_array( const duf_scan_callbacks_t ** psccbs, int sccb_num, int *pcnt )
{
  DUF_STARTR( r );

  int cnt = 0;

  assert( duf_pdi_global_name(  ) );
  MAST_TRACE( path, 0, "@levinfo_path: %s", duf_levinfo_path( duf_pdi_global(  ) ) );

  for ( int astep = 0; DUF_NOERROR( r ) && astep < sccb_num; astep++ )
  {
    if ( psccbs[astep] )
    {
      MAST_TRACE( action, 2, "%" DUF_ACTION_TITLE_FMT ": astep %d", duf_uni_scan_action_title( psccbs[astep] ), astep );

    /* duf_ev_sccb:
     * do everything needed from sccb, i.e. :
     * - call init  from psccbs[astep]
     * - do beginning sql set  from psccbs[astep]
     * - via duf_sccb_each_targv:
     *     - evaluate psccbs[astep] for each string  from DUF_CONFIGG(pcli->targ)[cv] as path
     *     - store number of changes to *pchanges
     * - do final sql set from psccbs[astep]
       * XXX  XXX */ DOR( r, duf_ev_sccb( psccbs[astep] /* sccb */  ) );
      cnt++;
    }
  }
  if ( pcnt )
    *pcnt += cnt;
  DUF_ENDR( r );
}
#endif
