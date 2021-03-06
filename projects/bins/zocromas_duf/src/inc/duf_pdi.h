#ifndef MAS_DUF_PDI_H
# define MAS_DUF_PDI_H

# include "duf_pdi_types.h"                                          /* duf_depthinfo_t ♠ */
# include "duf_hook_types.h"                                         /* duf_sql_set_t */
# include "duf_debug_defs.h"                                         /* DUF_WRAPSTATIC; DUF_WRAPPED ...  ♠ */
# include "duf_se.h"                                                 /* DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ♠ */

/* int DUF_WRAPPED( duf_pdi_init ) ( duf_depthinfo_t * pdi, const duf_ufilter_t * pu, const char *real_path,                                              */
/*                                   const duf_sql_set_t * sql_set, int caninsert, unsigned frecursive, int fallow_dirs, unsigned flinear, int opendir ); */
int duf_pdi_init( duf_depthinfo_t * pdi, const duf_ufilter_t * pu, const char *real_path,
                  const duf_sql_set_t * sql_set, int caninsert, unsigned frecursive, int fallow_dirs, unsigned flinear, int opendir );

int duf_pdi_init_min_r( duf_depthinfo_t * pdi, const char *real_path );

int duf_pdi_init_from_dirid( duf_depthinfo_t * pdi, const duf_ufilter_t * pu, unsigned long long dirid,
                             const duf_sql_set_t * sql_set, int caninsert, unsigned frecursive, int fallow_dirs, unsigned flinear, int opendir );

int duf_pdi_shut( duf_depthinfo_t * pdi );
int duf_pdi_close( duf_depthinfo_t * pdi );

#endif
