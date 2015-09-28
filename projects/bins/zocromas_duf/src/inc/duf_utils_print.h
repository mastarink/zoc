#ifndef MAS_DUF_UTILS_PRINT_H
#  define MAS_DUF_UTILS_PRINT_H

#  include "duf_print_defs.h"

const char *duf_color_s( int is_atty, const char *s );
const char *duf_fcolor_s( FILE * out, const char *s );

int duf_vsncolor_s( int is_atty, char *buf, size_t size, const char *fmt, va_list args );
int duf_sncolor_s( int is_atty, char *buf, size_t size, const char *fmt, ... );
int duf_vprint_color_s( FILE * out, const char *fmt, va_list args );
int duf_print_color_s( FILE * out, const char *fmt, ... );



int duf_vprintf( int level, int noeol, int minlevel, int ifexit, const char *funcid, int linid, FILE * out, const char *fmt, va_list args );
int duf_printf( int level, int noeol, int minlevel, int ifexit, const char *funcid, int linid, FILE * out, const char *fmt, ... )
      __attribute__ ( ( format( __printf__, 8, 9 ) ) );

int duf_puts( int level, int minlevel, const char *funcid, int linid, FILE * out, const char *str );
int duf_writes( int level, int minlevel, const char *funcid, int linid, FILE * out, const char *str );



#endif
