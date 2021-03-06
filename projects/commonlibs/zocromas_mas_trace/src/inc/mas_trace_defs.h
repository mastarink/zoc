#ifndef MAS_MAST_TRACE_DEFS_H
#  define MAS_MAST_TRACE_DEFS_H

#  include "mas_print_defs.h"
#  include "mas_trace_defs_base.h"
#  include "mas_trace_defs_what.h"
#  include "mas_trace_defs_sys.h"


#  ifdef MAS_TRACING
#    define MAST_TRACE_C( _tcfg, ... )				MAST_TRACE_WHAT_C( _tcfg,			MAST_TRACE_AT_TCFG_,	__VA_ARGS__ )
#    define MAST_TRACE( ... )					(/*fprintf(stderr, "[%p]",MAST_TRACE_CONFIG),*/MAST_TRACE_C(      MAST_TRACE_CONFIG, 			__VA_ARGS__ ))
#  else
#    define MAST_TRACE( ... )
#  endif

#  define MAST_TRACE_WP_C( _tcfg, _prefix, ... )			MAST_TRACE_WHAT_WP_C(        _tcfg, _prefix,	MAST_TRACE_AT_TCFG_,	__VA_ARGS__ )
#  define MAST_OTRACE_WP_C( _out, _tcfg, _prefix, ... )		MAST_OTRACE_WHAT_WP_C( _out, _tcfg, _prefix,	MAST_TRACE_AT_TCFG_,	__VA_ARGS__ )
#  define MAST_TRACE_WP( ... )					MAST_TRACE_WP_C(	    MAST_TRACE_CONFIG, 			__VA_ARGS__ )
#  define MAST_OTRACE_WP(_out, ... )				MAST_OTRACE_WP_C( _out,	    MAST_TRACE_CONFIG, 			__VA_ARGS__ )

#  define MAST_DEBUG(_lev, ...)  if ( MAST_IF_DEBUGN( _lev ) ) {  __VA_ARGS__ ; }

/* #  define TR(_r) if (MAST_IS_ERROR(_r)) {T("@@@@@@(i:%d:c:%d) %s", _r, duf_error_code_i(_r), duf_error_name_i(_r));} */


# define QT(_fmt, ...) MAST_TRACE(temp,0,"@"_fmt,__VA_ARGS__)
# define QTT( ...) MAST_TRACE(temp,0,__VA_ARGS__)
# define QTR if (QISERR) {QT("@@@@@@(i:%d:c:%d) %s", QERRIND, QERRCODE, QERRNAME);}

#endif

/*
vi: ft=c
vi: colorcolumn=65,105,121
*/
