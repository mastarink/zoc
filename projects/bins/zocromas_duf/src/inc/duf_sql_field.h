#ifndef MAS_DUF_SQL_FIELD_H
# define MAS_DUF_SQL_FIELD_H

# include "duf_sql_types.h"                                          /* duf_stmnt_t ✗ */
/* # include "duf_se.h"                                                 (* DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ✗ *) */

/* for DUF_GET_?FIELD2 */
int __duf_sql_pos_by_name2( duf_stmnt_t * psqlstmt, const char *name );
const char *__duf_sql_str_by_name2( duf_stmnt_t * psqlstmt, const char *name, int opt );
unsigned long long __duf_sql_ull_by_name2( duf_stmnt_t * psqlstmt, const char *name, int opt );

void __duf_sql_dump_row( duf_stmnt_t * psqlstmt );

#endif
