#ifndef MASREGERR_DEFS_H
# define MASREGERR_DEFS_H
# ifndef MAS_PACKAGE
#  define MAS_PACKAGE PACKAGE
# endif
# ifndef MAS_PACKAGE_NAME
#  define MAS_PACKAGE_NAME PACKAGE_NAME
# endif

# include <stddef.h>
# include <errno.h>

# define _uUu_ __attribute__(( unused ))

# ifndef TRUE
#  define TRUE (1)
# endif
# ifndef FALSE
#  define FALSE (0)
# endif

# ifndef RGEMSG
#  define RGEMSG strerror(errno)
# endif
/* # define MAS_FLF __func__,__LINE__,__FILE__ */
# define MAS_FLF __func__,__LINE__,__FILE__
# define MAS_FLFFF __func__,__LINE__,__FILE__,__FUNCTION__,__PRETTY_FUNCTION__

# define MASREGERR_DEFAULT_SIZE 256

# if 0
#  define rDECL(_v) int r_cnt _uUu_=0,r=(_v)
#  define rC(_x) if (!r_cnt || !r) { r=(_x); r_cnt++; }
#  define rCODE r
#  define rRET return r
# else
/* #  ifndef R_GOOD            */
/* #   define R_GOOD(_r) (!_r) */
/* #  endif                    */
#  define R_BAD(_r) (!R_GOOD((_r)))
#  define rCODE ret_code.r
#  define rCNT ret_code.cnt
#  define rDECL(_v) struct {int cnt;int r;} ret_code={0}; rCODE=(_v)
#  define rDECLGOOD rDECL(0)
#  define rDECLBAD rDECL(-1)
#  define rGOOD (R_GOOD(rCODE))
#  define rGOOD_OR1 (!rCNT || rGOOD)
#  define rBAD (R_BAD(rCODE))
#  if 1
#   define rSETBAD (rCODE=-1)
#   define rRET return (R_GOOD(rCODE)?rCODE:-1)
#  else
#   define rSETBAD (rCODE=-1,ADIE("SET ERROR:%d", rCODE))
#   define rRET if (rBAD) ADIE("ERROR:%d", rCODE);return (R_GOOD(rCODE)?rCODE:-1)
#  endif
#  define rSETGOOD rCODE=0
/* #  define rC(_x) if (!ret_code.cnt || !ret_code.r) { ret_code.r=(_x); ret_code.cnt++; } */
#  define rC(_x)  (rGOOD_OR1 ? ( rCNT++,(rCODE=(_x)),rGOOD) : rGOOD)

/*                                       1(regerrs),      2,3,4,5,6      7(package)    6(perrno)    7(sys)        8(fmt),...*/
#  define RGEX(_sys,  ...) masregerr_reg(NULL,            MAS_FLFFF, MAS_PACKAGE_NAME, &errno,      errno?_sys:0, __VA_ARGS__ )

#  define RGE        RGEX(0, NULL, NULL)
#  define RGES RGESM(RGEMSG)

/* #  define RGEM(...)  RGEX(0, __VA_ARGS__, RGEMSG) */
#  define RGEM(...)  RGEX(0, __VA_ARGS__)
/* #  define RGESM(...) RGEX(1, "SYS: %s {%s}", __VA_ARGS__, RGEMSG) */
#  define RGESM(...) RGEX(1, "SYS: %s", __VA_ARGS__)

#  define RGER(_r)		( ( R_BAD((_r)) ) ? RGE : 0 )
#  define RGERM(_r, ...)	( ( R_BAD((_r)) ) ? RGEM(__VA_ARGS__) : 0 )

/* #  define RGESR(_r)             { if ( R_BAD((_r)) ) RGES; } */
#  define RGESR(_r)		( ( R_BAD((_r)) ) ? RGES : 0 )
/* #  define RGESRM(_r, ...)       { if ( R_BAD((_r)) ) RGESM(__VA_ARGS__); } */
#  define RGESRM(_r, ...)	( R_BAD((_r)) ? RGESM(__VA_ARGS__) : 0 )

# endif
/* #define rCALL if (!r) r= */

#endif
