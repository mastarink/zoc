#ifndef MAS_DUF_ERRORS_H
#  define MAS_DUF_ERRORS_H

#  include <stdarg.h>

#  include "duf_error_types.h"

void duf_force_ereport( int count );

duf_error_code_t duf_make_error( duf_error_code_t err, const char *funcid, int linid );
long duf_made_errors( duf_error_code_t err );
duf_error_code_t duf_vclear_error( duf_error_code_t r, va_list args );
duf_error_code_t duf_clear_error( duf_error_code_t r, ... );

int duf_errindex( duf_error_code_t rtest );
const char *duf_error_name( duf_error_code_t c );

void duf_set_mereport( int once, int enable, int abs, ... );
void duf_vset_mereport( int once, int enable, int abs, va_list args );
void duf_set_ereport( int once, int enable, int abs, duf_error_code_t rtest );
int duf_get_ereport_n( duf_error_code_t rtest );
int duf_get_ereport( duf_error_code_t rtest );

int duf_ecount( duf_error_code_t rtest );


void duf_set_emax_count( int maxcount, duf_error_code_t rtest );
void duf_vset_emax_count( int maxcount, va_list args );
void duf_set_memax_count( int maxcount, ... );

#endif
