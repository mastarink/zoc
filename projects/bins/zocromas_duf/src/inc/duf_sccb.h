#ifndef MAS_DUF_SCCB_H
#  define MAS_DUF_SCCB_H

/* #  include "duf_sccb_way.h" */

#  include "duf_hook_types.h"
const char *_duf_uni_scan_action_title( const duf_scan_callbacks_t * sccb );
const char *duf_uni_scan_action_title( const duf_scan_callbacks_t * sccb );

int duf_sccb_pdi( duf_sccb_handle_t * sccbh );

#endif
