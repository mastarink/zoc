#ifndef DUF_SELECTOR_H
#  define DUF_SELECTOR_H

char *duf_selector2sql( const duf_sql_set_t * sql_set, const char *selected_db );

char *duf_selector_total2sql( const duf_sql_set_t * sql_set, const char *selected_db );

char *duf_insert_selected_db( const char *sql, const char *dbname );

#endif
