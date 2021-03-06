#ifndef MAS_DUF_SCCB_EVAL_H
# define MAS_DUF_SCCB_EVAL_H

# include "duf_sccb_types.h"                                         /* duf_scan_callbacks_t; duf_sccb_handle_t; duf_sccb_data_row_t ✗ */

/* fragment of func.name means:
 *
 * _ev_ : evaluate one of so-called "sccb's"
 * _(eval|evaluate)_ : evaluate something else
 *
 * sccb:	          (                        duf_scan_callbacks_t * sccb ...
 * pdi_sccb:	          ( duf_depthinfo_t * pdi, duf_scan_callbacks_t * sccb ...
 * 
 * evnamen:	          (                            const char *name, size_t namelen, const duf_action_table_t * table ...
 * evnamen_std:	          (                            const char *name, size_t namelen; with standard action table ...
 *
 * evname:	          (                        z/t const char *name,                 const duf_action_table_t * table ...
 * evname_std:	          (                        z/t const char *name; with standard action table ...
 *
 * pdi_evnamen:	          ( duf_depthinfo_t * pdi,     const char *name, size_t namelen, const duf_action_table_t * table ...
 * pdi_evnamen_std:	  ( duf_depthinfo_t * pdi,     const char *name, size_t namelen; with standard action table ...
 * pdi_evnamen_std_pt:	  ( duf_depthinfo_t * pdi,     const char *name, size_t namelen, mas_argvc_t * ptarg ; with standard action table ...
 *
 * pdi_evname:	          ( duf_depthinfo_t * pdi, z/t const char *name,                 const duf_action_table_t * table ...
 * pdi_evname_pt:         ( duf_depthinfo_t * pdi, z/t const char *name,                 const duf_action_table_t * table, mas_argvc_t * ptarg ...
 * pdi_evname_at:         ( duf_depthinfo_t * pdi, z/t const char *name,                 const duf_action_table_t * table, const char *arg ...
 * pdi_evname_std:	  ( duf_depthinfo_t * pdi, z/t const char *name; with standard action table ...
 * pdi_evname_std_pt:	  ( duf_depthinfo_t * pdi, z/t const char *name, mas_argvc_t * ptarg; with standard action table ...
 * pdi_evname_std_at:     ( duf_depthinfo_t * pdi, z/t const char *name, const char *arg; with standard action table ...
 *
 * evnamed_list:          (                        const char *names, const duf_action_table_t * table ...
 * evnamed_list_std:      (                        const char *names; with standard action table       ...
 *
 * pdi_evnamed_list:      ( duf_depthinfo_t * pdi, const char *names, const duf_action_table_t * table ...
 *
 * */

# include "duf_se.h"                                                 /* DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ✗ */

DR( OTHER, ev_evnamed_list, const char *names, duf_scan_callbacks_t * first );

/* int duf_ev_evnamen( const char *name, size_t len, duf_scan_callbacks_t * first ) __attribute__ ( ( warn_unused_result ) ); */

/* int duf_ev_evname( const char *name, duf_scan_callbacks_t * first ) __attribute__ ( ( warn_unused_result ) ); */

/* int duf_ev_sccb_array( const duf_scan_callbacks_t ** sccb_sequence, int sccb_num, int *pcnt ) __attribute__ ( ( warn_unused_result ) ); */

#endif
