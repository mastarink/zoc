/* #undef MAS_TRACING */
#include <assert.h>

#include "duf_tracen_defs_preset.h"                                  /* MAST_TRACE_CONFIG; etc. ✗ */
#include "duf_errorn_defs_preset.h"                                  /* MAST_ERRORS_FILE; etc. ✗ */

#include <mastar/trace/mas_trace.h>
#include <mastar/error/mas_error_defs_ctrl.h>
#include <mastar/error/mas_error_defs_make.h>
#include <mastar/error/mas_error_defs.h>

#include "duf_se_only.h"                                             /* Only DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ✗ */

#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ✗ */

#include "duf_sccb_row.h"                                            /* datarow_* ✗ */

#include "duf_sccbh_structs.h"                                       /* duf_sccb_handle_s (from duf_sccbh_types: duf_sccb_handle_t; duf_sccbh_fun_t; duf_rsccbh_fun_t) ✗ */
/* ###################################################################### */
#include "duf_sccbh_ref.h"
/* ###################################################################### */

void
duf_sccbh_set_pdi( duf_sccb_handle_t * sccbh, duf_depthinfo_t * pdi )
{
  if ( sccbh )
    sccbh->pdi = pdi;
}

duf_depthinfo_t *
duf_sccbh_pdi( const duf_sccb_handle_t * sccbh )
{
  return sccbh ? sccbh->pdi : NULL;
}

duf_depthinfo_t *
duf_sccbh_pdi_p( duf_sccb_handle_t * sccbh )
{
  return sccbh ? sccbh->pdi : NULL;
}

/* const duf_scan_callbacks_t *const *sccb_array; */
const duf_scan_callbacks_t *const *
duf_sccbh_sccbarray( const duf_sccb_handle_t * sccbh )
{
  return sccbh ? sccbh->sccb_array : NULL;
}

int
duf_sccbh_sccbindex( const duf_sccb_handle_t * sccbh )
{
  return sccbh ? sccbh->sccb_index : 0;
}

int *
duf_sccbh_psccbindex( duf_sccb_handle_t * sccbh )
{
  return sccbh ? &sccbh->sccb_index : NULL;
}

void
duf_sccbh_set_total_counted( duf_sccb_handle_t * sccbh, unsigned val )
{
  sccbh->total_counted = val ? 1 : 0;
}

unsigned
duf_sccbh_total_counted( const duf_sccb_handle_t * sccbh )
{
  return sccbh ? sccbh->total_counted : 0;
}

unsigned long long
duf_sccbh_total_items( const duf_sccb_handle_t * sccbh )
{
  return sccbh ? sccbh->total_items : 0;
}

unsigned long long *
duf_sccbh_ptotal_items( duf_sccb_handle_t * sccbh )
{
  return sccbh ? &sccbh->total_items : NULL;
}

const duf_scan_callbacks_t *
duf_sccbh_sccb( const duf_sccb_handle_t * sccbh )
{
  return sccbh && sccbh->sccb_array ? sccbh->sccb_array[sccbh->sccb_index] : NULL;
}

void
duf_sccbh_set_changes( duf_sccb_handle_t * sccbh, unsigned long long changes )
{
  assert( sccbh );
  if ( sccbh )
    sccbh->changes = changes;
}

unsigned long long
duf_sccbh_changes( const duf_sccb_handle_t * sccbh )
{
  return sccbh ? sccbh->changes : 0;
}

const duf_sccb_data_list_t *
duf_sccbh_rowlist( const duf_sccb_handle_t * sccbh )
{
  return sccbh ? &sccbh->rowlist : NULL;
}

duf_sccb_data_list_t *
duf_sccbh_rowlist_p( duf_sccb_handle_t * sccbh )
{
  return sccbh ? &sccbh->rowlist : NULL;
}

const duf_sccb_data_row_t *
duf_sccbh_rows( const duf_sccb_handle_t * sccbh )
{
  return CRX( datarow_list_last, CRX( sccbh_rowlist, sccbh ) ) /*    sccbh ? sccbh->rowlist.last : NULL */ ;
}

SRN( SCCBH, void, sccbh_set_current_row, duf_sccb_handle_t * sccbh, const duf_sccb_data_row_t * row )
{
  assert( sccbh );
  sccbh->current_row = row;
  ERN( SCCBH, void, sccbh_set_current_row, duf_sccb_handle_t * sccbh, const duf_sccb_data_row_t * row );
}

const duf_sccb_data_row_t *
duf_sccbh_current_row( const duf_sccb_handle_t * sccbh )
{
  return sccbh ? sccbh->current_row : NULL;
}

duf_sccbh_fun_t
duf_sccbh_progress_leaf_cb( const duf_sccb_handle_t * sccbh )
{
  return sccbh ? sccbh->progress_leaf_cb : NULL;
}

duf_sccbh_fun_t
duf_sccbh_progress_node_cb( const duf_sccb_handle_t * sccbh )
{
  return sccbh ? sccbh->progress_node_cb : NULL;
}
