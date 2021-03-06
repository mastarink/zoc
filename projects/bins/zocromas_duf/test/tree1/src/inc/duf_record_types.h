#ifndef MAS_DUF_RECORD_TYPES_H
#  define MAS_DUF_RECORD_TYPES_H

struct duf_scan_callbacks_s;
struct duf_sccb_handle_s;

/* #  include "duf_sccb_way.h" */

#  include "duf_levinfo_types.h"
#  include "duf_pdi_types.h"


typedef struct
{
#  ifdef DUF_RECORD_WITH_NROWS
  int nrow;
  int nrows;
#  endif
  int ncolumns;
  const char *const *presult;
  const char *const *pnames;
}
duf_record_t;

/* this is callback of type:duf_sel_cb_t (second range; ; sel_cb) */
typedef int ( *duf_str_cb_t ) ( void *str_cb_udata, struct duf_sccb_handle_s /* duf_sccb_handle_t */  * sccbh, duf_record_t * precord );
typedef int ( *duf_sel_cb_t ) ( duf_record_t * precord, void *sel_cb_udata, duf_str_cb_t str_cb, void *str_cb_udata,
                                struct duf_sccb_handle_s /* duf_sccb_handle_t */  * sccbh );
typedef int ( *duf_sel_cb_match_t ) ( duf_record_t * precord );


#endif

/*
vi: ft=c
*/
