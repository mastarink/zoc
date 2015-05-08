#include <stdarg.h>
#include <stddef.h>
#include <string.h>
#include <time.h>
#include <assert.h>

#include <sys/types.h>
#include <sys/stat.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance.h"
#include "duf_utils.h"
#include "duf_utils_path.h"

#include "duf_config_ref.h"

#include "duf_pdi.h"
#include "duf_sccb.h"
#include "duf_levinfo.h"
#include "duf_levinfo_ref.h"

#include "duf_option_defs.h"
#include "duf_option_find.h"
#include "duf_option_descr.h"
#include "duf_option_extended.h"

/* ###################################################################### */
#include "duf_option_typed.h"
/* ###################################################################### */


static FILE *
duf_open_file_special( const char *pname, char **popenedname, int *pr )
{
  int r = 0;
  FILE *newfile = NULL;
  int overw = 0;
  const char *mode = "w";
  struct stat st;

  if ( *pname == '@' )
  {
    overw = 1;
    pname++;
  }
  else if ( *pname == '+' )
  {
    mode = "a";
    pname++;
  }
  *popenedname = mas_strdup( pname );
  if ( 0 == stat( pname, &st ) && ( ( !S_ISCHR( st.st_mode ) || !( st.st_mode & S_IWUSR ) ) && ( !overw && *mode != 'a' ) ) )
  {
    DUF_SHOW_ERROR( "can't open file %s for writing file exists %llu / %llu chr:%d\n", pname, ( unsigned long long ) st.st_dev,
                    ( unsigned long long ) st.st_rdev, S_ISCHR( st.st_mode ) );
    DOR( r, DUF_ERROR_OPTION_VALUE );
  }
  else
  {
    newfile = fopen( pname, mode );
    if ( !newfile )
    {
      DUF_SHOW_ERROR( "can't open file %s\n", pname );
      DOR( r, DUF_ERROR_OPTION_VALUE );
    }
  }
  if ( pr )
    *pr = r;
  return newfile;
}

static int
duf_set_file_special( const char *pname, char **pfilename, FILE ** pfile, FILE * defout, int handleid )
{
  DEBUG_STARTR( r );
  FILE *newout;

  newout = defout;

  if ( pfile )
  {
    *pfile = NULL;
    if ( !newout && handleid )
    {
      switch ( handleid )
      {
      case 1:
        newout = stdout;
        break;
      case 2:
        newout = stderr;
        break;
      }
    }
    if ( pfilename )
    {
      if ( *pfilename )
        mas_free( *pfilename );
      *pfilename = NULL;
      if ( pname && *pname )
        newout = duf_open_file_special( pname, pfilename, &r );
    }
    if ( !newout )
      newout = defout;
    if ( newout )
    {
      if ( *pfile && *pfile != stderr && *pfile != stdout )
        fclose( *pfile );
      *pfile = newout;
    }
  }
  DEBUG_ENDR( r );
}

int
duf_parse_option_long_typed( const duf_longval_extended_t * extended, const char *optargg, int istage, int noo )
{
  DEBUG_STARTR( r );

  if ( !extended )
    DOR( r, DUF_ERROR_OPTION );

  DUF_TRACE( options, 2, "parsing typed:`%s`;   %s", extended->o.name, duf_error_name( r ) );
  DUF_TRACE( options, 2, "r:%d; xname:%s; noo:%d", r, extended ? extended->o.name : "?", noo );
/*
 * if arg is help option
 * return class id for options to display the help
 * */

  if ( r >= 0 )
  {
    unsigned doplus = 0;
    char *byteptr = NULL;

    switch ( extended->relto )
    {
    case DUF_OFFSET_config:
      byteptr = ( ( ( char * ) duf_config ) + extended->m_offset );
      break;
    case DUF_OFFSET_depthinfo:
      byteptr = ( ( ( char * ) duf_config->pdi ) + extended->m_offset );
      break;
    case DUF_OFFSET_ufilter:
      byteptr = ( ( ( char * ) duf_config->pu ) + extended->m_offset );
      break;
    }
#define DUF_NUMOPT( _no, _rt, _typ,_dopls,_conv) \
      if (_rt>=0) \
      { \
	if ( !_no ) \
	{ \
	  int __rl = 0; \
	  _typ *p; \
	  p = ( _typ * ) byteptr; /* byteptr only valid if extended->m_hasoff == 1 */ \
	  if ( extended->m_hasoff == 1 && optargg ) /* if  extended->m_hasoff == 1, then mcfg_offset is offset */ \
	  { \
	    _typ __v; \
	    if (*optargg=='?') \
	    { DUF_PRINTF(0, "%s=%lld", extended->o.name, (unsigned long long)( *p )); } \
	    else \
	    { \
	      __v = _conv( optargg, &__rl ); \
	      if ( __rl < 0 ) \
	      { DOR(_rt, DUF_ERROR_OPTION_VALUE); } \
	      else \
	      { \
		( *p ) = __v; \
		DUF_TRACE( options, 1, "%s : number set:%llu", extended->o.name, (unsigned long long) __v ); \
	      } \
	    } \
	  } \
	  else if ( _dopls ) \
	  { \
	    ( *p )++; \
	  } \
	} \
	else \
	{ \
	  DOR(_rt, DUF_ERROR_OPTION); \
	} \
      }
#define DUF_PNUMOPT( _no, _rt, _typ ) \
      if (_rt>=0) \
      { \
	if ( !_no ) \
	{ \
	  _typ *p; \
	  p = ( _typ * ) byteptr; /* byteptr only valid if extended->m_hasoff == 1 */ \
	  if ( extended->m_hasoff == 1 ) /* if  extended->m_hasoff == 1, then mcfg_offset is offset */ \
	  { \
	    DUF_PRINTF(0, "%lld", (unsigned long long)( *p )); \
	  } \
	} \
	else \
	{ \
	  DOR(_rt, DUF_ERROR_OPTION); \
	} \
      }
#define DUF_QPNUMOPT( _no, _rt, _typ, _conv ) \
      if (_rt>=0) \
      { \
	if ( !_no ) \
	{ \
	  if ( optargg && *optargg ) \
	  { \
	    DUF_NUMOPT(_no, _rt, _typ,0,_conv); \
	    if (_rt==DUF_ERROR_OPTION_VALUE) { DUF_CLEAR_ERROR(_rt, DUF_ERROR_OPTION_VALUE); DUF_PNUMOPT(_no, _rt, _typ); } \
	  } \
	  else \
	  {  DUF_PNUMOPT(_no, _rt, _typ); } \
	} \
	else \
	{ \
	  DOR(_rt, DUF_ERROR_OPTION); \
	} \
      }
#define DUF_MINMAXOPT(_no, _rt, _typ,_conv) \
      if (_rt>=0) \
      { \
	if ( !_no ) \
	{ \
	  int __rl = 0; \
	  _typ *mm; \
	  mm= ( _typ * ) byteptr; /* byteptr only valid if extended->m_hasoff == 1 */ \
	  if ( extended->m_hasoff == 1 ) /* if  extended->m_hasoff == 1, then mcfg_offset is offset */ \
	  { \
	    if( optargg ) \
	    { \
	      _typ __v; \
	      char c=0; \
	      mm->flag = 1; \
	      if ( *optargg == '+' ) \
		c = *optargg++; \
	      else if ( *optargg == '-' ) \
		c = *optargg++; \
	      __v.min = _conv( optargg, &__rl ); \
	      if ( __rl < 0 ) \
	      { DOR(_rt, DUF_ERROR_OPTION_VALUE); } \
	      else \
	      { \
		if ( c=='+' ) \
		{ \
		  mm->min = __v.min; \
		  mm->max = 0; \
		} \
		else if ( c=='-' ) \
		{ \
		  mm->min = 0; \
		  mm->max = __v.min; \
		} \
		else \
		{ \
		  mm->min = mm->max = __v.min; \
		} \
		DUF_TRACE( options, 1, "%s: min/max set:%llu", extended->o.name, (unsigned long long) __v.min ); \
	      } \
	    } \
	    else \
	    { \
	      mm->flag = 1; \
	      mm->min = extended->call.value.u; \
	      mm->max = extended->call.value2.u; \
	      DUF_TRACE( options, 1, "%s: min/max set:%llu:%llu", extended->o.name, \
		  (unsigned long long) extended->call.value.u,  (unsigned long long) extended->call.value2.u ); \
	    } \
	  } \
	} \
	else \
	{ \
	  DOR(_rt, DUF_ERROR_OPTION); \
	} \
      }
#define DUF_MOPT(_no, _rt,_typ,_mix,_conv) \
      if(_rt>=0) \
      { \
	if ( !_no ) \
	{ \
	  int __rl = 0; \
	  _typ *mm; \
	  mm= ( _typ * ) byteptr; /* byteptr only valid if extended->m_hasoff == 1 */ \
	  if ( extended->m_hasoff == 1 && optargg ) /* if  extended->m_hasoff == 1, then mcfg_offset is offset */ \
	  { \
	    mm->flag = 1; \
	    mm->_mix = _conv( optargg, &__rl ); \
	    if ( __rl < 0 ) \
	    {  DOR(_rt, DUF_ERROR_OPTION_VALUE); } \
	    else \
	    { DUF_TRACE( options, 1, #_mix "%s: set:%llu", extended->o.name, (unsigned long long) mm->_mix ); } \
	  } \
	} \
	else \
	{ \
	  DOR(_rt, DUF_ERROR_OPTION); \
	} \
      }

#define DUF_OUTPUTFILE_A(_no, _rt, _typ, _defoptarg, _defout) \
      if(_rt>=0) \
      { \
	if( !_no ) \
	{ \
	  int __rl = 0; \
	  const char *s; \
	  _typ *mm; \
	  s=optargg ? optargg : _defoptarg; \
	  mm= ( _typ * ) byteptr; /* byteptr only valid if extended->m_hasoff == 1 */ \
	  if ( extended->m_hasoff == 1 /* && (s || extended->call.value.u */ ) /* if  extended->m_hasoff == 1, then mcfg_offset is offset */ \
	  { \
	    DOR(__rl, duf_set_file_special( s, &mm->file, &mm->out, _defout, extended->call.value.u )); \
	    if ( __rl < 0 ) \
	    {  DOR(_rt, DUF_ERROR_OPTION_VALUE); } \
	  } \
	} \
	else \
	{ \
	  DOR(_rt, DUF_ERROR_OPTION); \
	} \
      }
#define DUF_OUTPUTFILE(_no, _rt, _typ, _defout) DUF_OUTPUTFILE_A(_no, _rt, _typ, NULL, _defout)

    /* if ( ( extended->stage.min <= istage && extended->stage.max >= istage ) || extended->stage.flag ) */
    if ( istage == -1 || ( extended->stage.min <= istage && extended->stage.max >= istage ) || extended->stage.flag )
    {
      int nof;

      nof = noo;
      switch ( extended->vtype )
      {
      case DUF_OPTION_VTYPE_NONE:
        if ( r >= 0 )
          DOR( r, DUF_ERROR_OPTION_NOT_PARSED );
        /* DUF_TEST_R( r ); */
        break;
      case DUF_OPTION_VTYPE_NOOP:
        break;
      case DUF_OPTION_VTYPE_UPLUS:
        if ( noo )
          DOR( r, DUF_ERROR_OPTION_NOT_PARSED );
        if ( r >= 0 )
          doplus = 1;
      case DUF_OPTION_VTYPE_NUM:
        if ( noo )
          DOR( r, DUF_ERROR_OPTION_NOT_PARSED );
        DUF_NUMOPT( noo, r, unsigned, doplus, duf_strtol_suff );

        break;
      case DUF_OPTION_VTYPE_NL:
        if ( noo )
          DOR( r, DUF_ERROR_OPTION_NOT_PARSED );
        DUF_NUMOPT( noo, r, unsigned long, 0, duf_strtol_suff );

        break;
      case DUF_OPTION_VTYPE_NLL:
        if ( noo )
          DOR( r, DUF_ERROR_OPTION_NOT_PARSED );
        if ( r >= 0 )
        {
          DUF_NUMOPT( noo, r, unsigned long long, 0, duf_strtoll_suff );
        }
        break;
      case DUF_OPTION_VTYPE_PNUM:
        if ( noo )
          DOR( r, DUF_ERROR_OPTION_NOT_PARSED );
        if ( r >= 0 )
        {
          DUF_QPNUMOPT( noo, r, unsigned, duf_strtol_suff );
        }
        break;
      case DUF_OPTION_VTYPE_PNL:
        if ( noo )
          DOR( r, DUF_ERROR_OPTION_NOT_PARSED );
        DUF_QPNUMOPT( noo, r, unsigned long, duf_strtol_suff );

        break;
      case DUF_OPTION_VTYPE_PNLL:
        if ( noo )
          DOR( r, DUF_ERROR_OPTION_NOT_PARSED );
        DUF_QPNUMOPT( noo, r, unsigned long long, duf_strtoll_suff );

        break;
      case DUF_OPTION_VTYPE_MIN:
        if ( noo )
          DOR( r, DUF_ERROR_OPTION_NOT_PARSED );
        DUF_MOPT( noo, r, duf_limits_t, min, duf_strtol_suff );
        break;
      case DUF_OPTION_VTYPE_MAX:
        if ( noo )
          DOR( r, DUF_ERROR_OPTION_NOT_PARSED );
        DUF_MOPT( noo, r, duf_limits_t, max, duf_strtol_suff );
        break;
      case DUF_OPTION_VTYPE_MINMAX:
        if ( noo )
          DOR( r, DUF_ERROR_OPTION_NOT_PARSED );
        DUF_MINMAXOPT( noo, r, duf_limits_t, duf_strtol_suff );
        break;
      case DUF_OPTION_VTYPE_MINLL:
        if ( noo )
          DOR( r, DUF_ERROR_OPTION_NOT_PARSED );
        DUF_MOPT( noo, r, duf_limitsll_t, min, duf_strtol_suff );
        break;
      case DUF_OPTION_VTYPE_MAXLL:
        if ( noo )
          DOR( r, DUF_ERROR_OPTION_NOT_PARSED );
        DUF_MOPT( noo, r, duf_limitsll_t, max, duf_strtoll_suff );
        break;
      case DUF_OPTION_VTYPE_MINMAXLL:
        if ( noo )
          DOR( r, DUF_ERROR_OPTION_NOT_PARSED );
        DUF_MINMAXOPT( noo, r, duf_limitsll_t, duf_strtoll_suff );
        break;
        /* case DUF_OPTION_CLASS_DEBUG: */
        /* DUF_PRINTF( 0, "------------ %lu", extended->mcfg_offset ); */
        /* break; */
      case DUF_OPTION_VTYPE_NOFLAG:
        nof = !nof;
      case DUF_OPTION_VTYPE_FLAG:
        {
          if ( extended->invert )
            nof = !nof;
          if ( r >= 0 )
          {
            unsigned *pi;

            pi = ( unsigned * ) byteptr;
            if ( nof )
              ( *pi ) &= ~extended->afl.bit;
            else
              ( *pi ) |= extended->afl.bit;
            DUF_TEST_R( r );
          }
        }
        break;
      case DUF_OPTION_VTYPE_PFLAG:
        if ( noo )
          DOR( r, DUF_ERROR_OPTION_NOT_PARSED );
        if ( r >= 0 )
        {
          unsigned *pi;

          pi = ( unsigned * ) byteptr;
          DUF_PRINTF( 0, "%s is %s", extended->o.name, ( ( *pi ) & extended->afl.bit ) ? "ON" : "OFF" );
          /* DUF_TEST_R( r ); */
        }
        break;
      case DUF_OPTION_VTYPE_SFLAG:
        if ( r >= 0 )
        {
          unsigned short *pis;

          pis = ( unsigned short * ) byteptr;
          if ( noo )
            ( *pis ) &= ~extended->afl.sbit;
          else
            ( *pis ) |= extended->afl.sbit;
          DUF_TEST_R( r );
        }
        break;
      case DUF_OPTION_VTYPE_NOSFLAG:
        if ( noo )
          DOR( r, DUF_ERROR_OPTION_NOT_PARSED );
        if ( r >= 0 )
        {
          unsigned *pi;

          pi = ( unsigned * ) byteptr;
          ( *pi ) &= ~extended->afl.sbit;
          DUF_TEST_R( r );
        }
        break;
      case DUF_OPTION_VTYPE_PSFLAG:
        if ( noo )
          DOR( r, DUF_ERROR_OPTION_NOT_PARSED );
        if ( r >= 0 )
        {
          unsigned *pi;

          pi = ( unsigned * ) byteptr;
          DUF_PRINTF( 0, "(s) %s is %s", extended->o.name, ( ( *pi ) & extended->afl.sbit ) ? "ON" : "OFF" );
          /* DUF_TEST_R( r ); */
        }
        break;
      case DUF_OPTION_VTYPE_STR:
        if ( noo )
          DOR( r, DUF_ERROR_OPTION_NOT_PARSED );
        if ( r >= 0 )
        {
          char **pstr;

          pstr = ( char ** ) byteptr;
          if ( pstr && *pstr )
            mas_free( *pstr );
          *pstr = NULL;
          if ( optargg )
          {
            *pstr = mas_strdup( optargg );
            DUF_TRACE( options, 0, "string set:%s", optargg );
          }
        }
        break;
      case DUF_OPTION_VTYPE_PSTR:
        if ( r >= 0 )
        {
          char **pstr;

          pstr = ( char ** ) byteptr;
          if ( noo )
          {
            if ( pstr && *pstr )
              mas_free( *pstr );
            *pstr = NULL;
          }
          else if ( optargg && *optargg )
          {
            if ( pstr && *pstr )
              mas_free( *pstr );
            *pstr = mas_strdup( optargg );
            DUF_TRACE( options, 0, "string set:%s", optargg );
          }
          else if ( extended->m_hasoff == 1 ) /* if  extended->m_hasoff == 1, then mcfg_offset is offset */
          {
            DUF_PRINTF( 0, "%s", ( *pstr ) );
          }
        }
        break;
      case DUF_OPTION_VTYPE_PDISTR:
        if ( noo )
          DOR( r, DUF_ERROR_OPTION_NOT_PARSED );
        if ( r >= 0 )
        {
          if ( optargg && *optargg )
            DOR( r,
                 duf_pdi_reinit_anypath( *( ( duf_depthinfo_t ** ) byteptr ), optargg,
                                         NULL /* const char *node_selector2 *//* , duf_config->pu,  recursive */  ) );
          else
            DUF_PRINTF( 0, "%s", duf_levinfo_path( *( ( duf_depthinfo_t ** ) byteptr ) ) );
        }
        break;
      case DUF_OPTION_VTYPE_PDINUM:
        if ( noo )
          DOR( r, DUF_ERROR_OPTION_NOT_PARSED );
        if ( r >= 0 && extended->m_hasoff == 1 )
        {
          /* if ( optargg && *optargg )                                                                                              */
          /*   DOR( r, duf_pdi_reinit_anypath( *( ( duf_depthinfo_t ** ) byteptr ), optargg, &duf_config->u, 0 (* recursive *)  ) ); */
          /* else                                                                                                                    */
          DUF_PRINTF( 0, "%d", *( ( int * ) byteptr ) );
        }
        break;
      case DUF_OPTION_VTYPE_PDISCCB:
        if ( noo )
          DOR( r, DUF_ERROR_OPTION_NOT_PARSED );
        if ( r >= 0 )
        {
          duf_sccb_handle_t csccbh = {
            .pdi = duf_config->pdi,
            .sccb = extended->call.fdesc.hi.sccb,
          };
          duf_sccb_handle_t *sccbh = &csccbh;

#include "duf_sccb_way.h"
          DOR( r, duf_sccb_pdi( SCCBX ) );
        }
        break;
/* r = duf_make_sccb( sccb );                            */
/* r = duf_sccbh_each_path( duf_sccb_handle_t * sccbh ); */
      case DUF_OPTION_VTYPE_DATETIME:
        if ( noo )
          DOR( r, DUF_ERROR_OPTION_NOT_PARSED );
        DUF_NUMOPT( noo, r, unsigned long long, 0, duf_strtime2long );

        break;
      case DUF_OPTION_VTYPE_MINMAXDATETIME:
        if ( noo )
          DOR( r, DUF_ERROR_OPTION_NOT_PARSED );
        DUF_MINMAXOPT( noo, r, duf_limitsll_t, duf_strtime2long );
        break;
      case DUF_OPTION_VTYPE_MINDATETIME:
        if ( noo )
          DOR( r, DUF_ERROR_OPTION_NOT_PARSED );
        DUF_MOPT( noo, r, duf_limitsll_t, min, duf_strtime2long );
        break;
      case DUF_OPTION_VTYPE_MAXDATETIME:
        if ( noo )
          DOR( r, DUF_ERROR_OPTION_NOT_PARSED );
        DUF_MOPT( noo, r, duf_limitsll_t, max, duf_strtime2long );
        break;
      case DUF_OPTION_VTYPE_VIFUN:
        if ( noo )
          DOR( r, DUF_ERROR_OPTION_NOT_PARSED );
        if ( r >= 0 )
        {
          if ( extended->call.fdesc.vi.func )
            ( extended->call.fdesc.vi.func ) ( extended->call.fdesc.vi.arg );
          else
            r = DUF_ERROR_OPTION_NO_FUNC;
        }
        break;
      case DUF_OPTION_VTYPE_AFUN:
        if ( noo )
          DOR( r, DUF_ERROR_OPTION_NOT_PARSED );
        if ( r >= 0 )
        {
          assert( duf_config->cargv );
          if ( extended->call.fdesc.a.func )
            ( extended->call.fdesc.a.func ) ( duf_config->cargc, duf_config->cargv );
        }
        break;
      case DUF_OPTION_VTYPE_TNFUN:
        if ( noo )
          DOR( r, DUF_ERROR_OPTION_NOT_PARSED );
        if ( r >= 0 )
        {
          if ( extended->call.fdesc.tn.func )
            ( extended->call.fdesc.tn.func ) ( &duf_config->targc, &duf_config->targv, duf_strtol_suff( optargg, &r ) );
        }
        break;
      case DUF_OPTION_VTYPE_TSFUN:
        if ( noo )
          DOR( r, DUF_ERROR_OPTION_NOT_PARSED );
        if ( r >= 0 )
        {
          if ( extended->call.fdesc.tn.func )
            ( extended->call.fdesc.ts.func ) ( &duf_config->targc, &duf_config->targv, optargg );
        }
        break;
      case DUF_OPTION_VTYPE_FILE:
        if ( noo )
          DOR( r, DUF_ERROR_OPTION_NOT_PARSED );
        if ( r >= 0 )
        {
          DUF_OUTPUTFILE( noo, r, duf_config_output_t, stderr );
          /* {                                                                                 */
          /*   char start_time[128] = "??";                                                    */
          /*                                                                                   */
          /*   duf_strflocaltime( start_time, sizeof( start_time ), "%Y%m%d.%H:%M:%S", NULL ); */
          /*   PF( "%s", start_time );                                                         */
          /* }                                                                                 */

          /* DOR(r, DUF_ERROR_OPTION_NOT_PARSED); */
        }
        break;
      }
    }
  }
  DEBUG_ENDR( r );
}
