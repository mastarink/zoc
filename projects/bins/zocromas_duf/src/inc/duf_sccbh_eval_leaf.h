#ifndef MAS_DUF_SCCBH_EVAL_LEAF_H
# define MAS_DUF_SCCBH_EVAL_LEAF_H

# include "duf_se.h"                                                 /* DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ✗ */

DR( SCCBH, sccbh_eval_db_leaf_str_cb_new, duf_sccb_handle_t * sccbh, /* duf_stmnt_t * pstmt_x, */ duf_scanstage_t scanstage );

DR( SCCBH, sccbh_eval_db_leaf_fd_str_cb, duf_sccb_handle_t * sccbh, /* duf_stmnt_t * pstmt_x, */ duf_scanstage_t scanstage );

DR( SCCBH, sccbh_eval_db_leaf_str_cb, duf_sccb_handle_t * sccbh, /* duf_stmnt_t * pstmt_x, */ duf_scanstage_t scanstage );

/* DR( SCCBH, sccbh_eval_db_leaf_qfd_pack_new, duf_sccb_handle_t * sccbh, duf_scanstage_t scanstage ); */

DRX( SCCBH, int, np, 0, sccbh_eval_new_pack, duf_sccb_handle_t * sccbh );

#endif
