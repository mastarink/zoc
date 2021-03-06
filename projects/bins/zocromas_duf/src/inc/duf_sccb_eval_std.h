#ifndef MAS_DUF_SCCB_EVAL_STD_H
# define MAS_DUF_SCCB_EVAL_STD_H

# include "duf_sccb_types.h"                                         /* duf_scan_callbacks_t; duf_sccb_handle_t; duf_sccb_data_row_t ✗ */
# include "duf_pdi_types.h"                                          /* duf_depthinfo_t ✗ */

# include "duf_se.h"                                                 /* DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ✗ */

/* * see duf_sccb_eval.h for names explanation * */

duf_scan_callbacks_t *duf_first_sccb( void );

/* const duf_action_table_t *duf_find_atable_sccb_by_evname_std( const char *name ); */
const duf_scan_callbacks_t *duf_find_sccb_by_evname_std( const char *name );

/* const duf_scan_callbacks_t *duf_finor_load_d_sccb_by_evname_std( const char *name ); */

DR( OTHER, ev_evnamed_list_std, const char *names ) __attribute__ ( ( warn_unused_result ) );

/* int duf_ev_evnamen_std( const char *name, size_t len ) __attribute__ ( ( warn_unused_result ) ); */
/* int duf_ev_evname_std( const char *name ) __attribute__ ( ( warn_unused_result ) ); */

/* int duf_ev_pdi_evname_std_at( duf_depthinfo_t * pdi, const char *name, const char *arg ) __attribute__ ( ( warn_unused_result ) ); */
DR( OTHER, ev_pdi_evname_std, duf_depthinfo_t * pdi, const char *name ) __attribute__ ( ( warn_unused_result ) );

/* int duf_ev_pdi_evnamen_std_pt( duf_depthinfo_t * pdi, const char *name, size_t namelen, const mas_argvc_t * ptarg ) */
/*       __attribute__ ( ( warn_unused_result ) );                                                                     */
/* int duf_ev_pdi_evname_std_pt( duf_depthinfo_t * pdi, const char *name, const mas_argvc_t * ptarg ) __attribute__ ( ( warn_unused_result ) ); */

#endif
