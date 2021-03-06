#ifndef MAS_DUF_OPTIMPL_DEFS_H
# define MAS_DUF_OPTIMPL_DEFS_H

/* everything OBSOLETE 20160315.134409 */
# if 0
#  define DUF_OPT_NAME_D(_cli, _n,_d)		muc_coption_cnames_tmp( _cli, -1, DUF_OPTION_VAL_ ## _n, _d )
#  define DUF_OPT_NAME(_cli,_n)  DUF_OPT_NAME_D(_cli,_n,NULL)

#  define DUF_OPT_FLAG_NAME_D(_cli, _n, _d)	muc_coption_cnames_tmp( _cli, -1, DUF_OPTION_VAL_FLAG_ ## _n,_d )
#  define DUF_OPT_FLAG_NAME(_cli,_n)  DUF_OPT_FLAG_NAME_D(_cli, _n,NULL)

#  define DUF_OPT_NAME1(_cli,_n)			muc_coption_cnames_tmp( _cli,  1, DUF_OPTION_VAL_ ## _n )

/* #  define DUF_OPT_FLAG_NAME1_D(_n,_d)	muc_coption_cnames_tmp( _cli,  1, DUF_OPTION_FLAG_ ## _n, _d ) */
/* #  define DUF_OPT_FLAG_NAME1(_n) DUF_OPT_FLAG_NAME1_D(_n,NULL) */

/* #  define DUF_OPT_NAME2_D(_n,_d)		muc_coption_cnames_tmp( _cli,  2, DUF_OPTION_VAL_ ## _n,_d ) */
/* #  define DUF_OPT_NAME2(_n) DUF_OPT_NAME2_D(_n,NULL) */

#  define DUF_OPT_FLAG_NAME2_D(_cli,_n,_d)	muc_coption_cnames_tmp( _cli,  2, DUF_OPTION_VAL_FLAG_ ## _n,_d )
#  define DUF_OPT_FLAG_NAME2(_cli,_n) DUF_OPT_FLAG_NAME2_D(_cli,_n,NULL)

/* ------------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------------- */

/* #  include "duf_config_ref.h" */
/* */
#  ifdef MAS_TRACING
#   define DUF_OPTIONG(_lo)          (MAST_TRACE(flags, 0, "FLAG %s", # _lo), DUF_CONFIGG(_lo))
#  else
#   define DUF_OPTIONG(_lo)          DUF_CONFIGG(_lo)
#  endif
#  define DUF_OPTIONG_F(_lo)        DUF_OPTIONG(_lo)
#  define DUF_OPTIONG_N(_lo)        DUF_OPTIONG(_lo)
#  define DUF_OPTIONG_A(_lo, _a)    DUF_OPTIONG(_lo)._a

#  define DUF_OPTIONA(_lo)          DUF_CONFIGA(_lo)
#  define DUF_OPTIONA_F(_lo)        DUF_OPTIONA(_lo)
#  define DUF_OPTIONA_N(_lo)        DUF_OPTIONA(_lo)
#  define DUF_OPTIONA_A(_lo, _a)    DUF_OPTIONA(_lo)._a

#  define DUF_OPTIONW(_lo)          DUF_CONFIGW(_lo)
#  define DUF_OPTIONW_F(_lo)        DUF_OPTIONW(_lo)
#  define DUF_OPTIONW_N(_lo)        DUF_OPTIONW(_lo)
#  define DUF_OPTIONW_A(_lo, _a)    DUF_OPTIONW(_lo)._a

/* */
#  define DUF_OPTIONG_FLAGG(_lo, _pref, _fls) DUF_OPTIONG_F(_pref _fls._lo)
#  define DUF_OPTIONG_FLAG(_lo, _ref, _pref) DUF_OPTIONG_FLAGG(_lo, _pref, _ref v.flag)

#  define DUF_OPTIONW_FLAGG(_lo, _pref, _fls) DUF_OPTIONW_F(_pref _fls._lo)
#  define DUF_OPTIONW_FLAG(_lo, _ref, _pref) DUF_OPTIONW_FLAGG(_lo, _pref, _ref v.flag)
/* */

#  define DUF_OPTIONG_NUM(_lo, _pref) DUF_OPTIONG_N(_pref._lo)
#  define DUF_OPTIONW_NUM(_lo, _pref) DUF_OPTIONW_N(_pref._lo)

#  define DUF_OPTIONG_RNUM(_lo, _pref) DUF_OPTIONG_N(_pref->_lo)
#  define DUF_OPTIONW_RNUM(_lo, _pref) DUF_OPTIONW_N(_pref->_lo)
/* ------------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------------- */
#  define DUF_CLIW_FLAG(_lo)     DUF_OPTIONW_FLAG(_lo, ->, pcli)
#  define DUF_OPTW_FLAG(_lo)     DUF_OPTIONW_FLAG(_lo, ., opt)
#  define DUF_ACTW_FLAG(_lo)     DUF_OPTIONW_FLAG(_lo, ., opt.act)
#  define DUF_FORMATW_FLAG(_lo)  DUF_OPTIONW_FLAG(_lo, ., opt.bformat)
#  define DUF_UW_FLAG(_lo)       DUF_OPTIONW_FLAG(_lo, ->, vars.puz) /* OBSOLETE 20160315.132838 */

#  define DUF_CLIG_FLAG(_lo)     DUF_OPTIONG_FLAG(_lo, ->, pcli)
#  define DUF_OPTG_FLAG(_lo)     DUF_OPTIONG_FLAG(_lo, ., opt)
#  define DUF_ACTG_FLAG(_lo)     DUF_OPTIONG_FLAG(_lo, ., opt.act)
#  define DUF_OUTG_FLAG(_lo)     DUF_OPTIONG_FLAG(_lo, ., opt.output)
#  define DUF_FORMATG_FLAG(_lo)  DUF_OPTIONG_FLAG(_lo, ., opt.bformat)
#  define DUF_UG_FLAG(_lo)       DUF_OPTIONG_FLAG(_lo, ->, vars.puz) /* OBSOLETE 20160315.132838 */

#  define DUF_CLIW_NUM( _lo, _n ) DUF_OPTIONW_RNUM( _lo, pcli._n )
#  define DUF_OPTW_NUM( _lo, _n ) DUF_OPTIONW_NUM( _lo, opt._n )
#  define DUF_ACTW_NUM(_lo)      DUF_OPTW_NUM( _lo, act )
#  define DUF_FORMATW_NUM(_lo)   DUF_OPTW_NUM( _lo, bformat )

#  define DUF_CLIG_NUM( _lo, _n ) DUF_OPTIONG_RNUM( _lo, pcli._n )
#  define DUF_OPTG_NUM( _lo, _n ) DUF_OPTIONG_NUM( _lo, opt._n )
#  define DUF_ACTG_NUM(_lo)      DUF_OPTG_NUM( _lo, act )
#  define DUF_FORMATG_NUM(_lo)   DUF_OPTG_NUM( _lo, bformat )

#  define DUF_FORMATW_FLAG_NUM(_lo)   DUF_OPTW_NUM( _lo, bformat.v.flag )
#  define DUF_FORMATG_FLAG_NUM(_lo)   DUF_OPTG_NUM( _lo, bformat.v.flag )

#  define DUF_UG_NUM(_lo)        DUF_OPTIONG_NUM( _lo, u )
#  define DUF_UW_NUM(_lo)        DUF_OPTIONW_NUM( _lo, u )

#  define DUF_TRACE_NUM(_lo)    DUF_OPTW_NUM( _lo, trace )
# endif

#endif

/*
vi: ft=c
*/
