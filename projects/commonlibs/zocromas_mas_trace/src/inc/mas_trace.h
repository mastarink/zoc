#ifndef MAS_MAST_TRACE_H
#  define MAS_MAST_TRACE_H

#  include <stdio.h>

/* mas_trace_mode_t */
#  include "mas_trace_defs.h"
/* #  include "mas_trace_types.h" */


int mas_vtrace( /* mas_trace_mode_t trace_mode,  mas_trace_submode_t trace_submode, */const char *name, int level, int minlevel, const char *funcid,
                int linid, double time0, char signum, unsigned flags, int nerr, FILE * out, const char *prefix, int fun_width,
                const char *fmt, va_list args );
int mas_trace( /* mas_trace_mode_t trace_mode,  mas_trace_submode_t trace_submode, */const char *name, int level, int minlevel, const char *funcid,
               int linid, double time0, char signum, unsigned flags, int nerr, FILE * out, const char *prefix, int fun_width,
               const char *fmt, ... ) __attribute__ ( ( format( __printf__, 13, 14 ) ) );


typedef enum
{
  MAST_TRACE_FLAGID_SYSTEM,
  MAST_TRACE_FLAGID_MAX,
} mas_trace_flagid_t;

#  define  MAST_TRACE_FLAG_SYSTEM  1 << MAST_TRACE_FLAGID_SYSTEM
#  define  MAST_TRACE_FLAG_MAX     1 << MAST_TRACE_FLAGID_MAX


#endif