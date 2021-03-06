#ifndef MAS_ERROR_DEFS_MAKE_H
# define MAS_ERROR_DEFS_MAKE_H

# include "mas_error_regfind.h"                                      /* mas_register_error_c */

# define MASE_MAKE_ERRORFLM(_rval, _code, _func, _line, ...)         ( _rval = mas_register_error_c( _code, _func, _line, __VA_ARGS__ ) )
# define MASE_MAKE_ERRORM(_rval, _code, ...)  			MASE_MAKE_ERRORFLM( _rval, _code, __func__, __LINE__, __VA_ARGS__ )
# define MASE_MAKE_ERROR(_rval, _code )       			MASE_MAKE_ERRORM(   _rval, _code, NULL )
# define MASE_MAKE_ERRORFL(_rval, _code, _func, _line )         MASE_MAKE_ERRORFLM( _rval, _code, _func, _line, NULL )
# define MASE_MAKE_ERRORMFL(_rval, _code, _func, _line, ... )	MASE_MAKE_ERRORFLM( _rval, _code, _func, _line, __VA_ARGS__)

#endif

/*
vi: ft=c
*/
