#include <string.h>


#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance_options.h"

#include "duf_status.h"
#include "duf_status_ref.h"

#include "duf_option_find.h"
#include "duf_option_descr.h"
#include "duf_option_extended.h"
#include "duf_option_typed.h"

#include "duf_option_old.h"
/* ###################################################################### */
#include "duf_option.h"
/* ###################################################################### */

DUF_WRAPSTATIC int
duf_clarify_xcmd_full( const duf_longval_extended_t * extended, const char *optargg, duf_option_stage_t istage,
                       const duf_longval_extended_table_t * xtable, int no, duf_option_source_t source )
{
  DEBUG_STARTR( r );
  if ( extended )
  {
/* 
 * duf_parse_option_long return 
 *            =0  for other option
 *   errorcode<0  for error
 * */
    DUF_TRACE( options, +6, "xname:%s; arg:%s; istage:%d; no:%d", extended ? extended->o.name : "?", optargg, istage, no );

/* TODO : register  extended + optargg for further use */
    global_status_register_xcmd( extended, optargg, istage, no, source );
    DOR( r, duf_clarify_xcmd_typed( extended, optargg, istage, xtable, no ) );
    DUF_TRACE( options, +6, "parsed typed:`%s`   %s", extended->o.name, duf_error_name_i( r ) );


    if ( DUF_IS_ERROR_N( r, DUF_ERROR_OPTION_NOT_PARSED ) && !no )
    {
      assert( 0 );
      DUF_TRACE( explain, 1, "@old opt for %s", extended ? extended->o.name : NULL );
      DOZR( r, duf_clarify_xcmd_old( extended, optargg, istage, xtable ) );
    }
    DUF_TRACE( options, +6, "parsed CLI option:  %s  %s", duf_option_description_x_tmp( -1, extended ), duf_error_name_i( r ) );
  }
  else
    DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_NOT_PARSED );
  DUF_TRACE( options, 7, "@(%s) xname:%s; arg:%s; istage:%d; no:%d", duf_error_name_i( r ), extended ? extended->o.name : "?", optargg, istage, no );
  DEBUG_ENDR( r );
}

#ifdef MAS_WRAP_FUNC
int
DUF_WRAPPED( duf_clarify_xcmd_full ) ( const duf_longval_extended_t * extended, const char *optargg, duf_option_stage_t istage,
                                       const duf_longval_extended_table_t * xtable, int no, duf_option_source_t source )
{
  DEBUG_STARTR( r );

  DEBUG_E_LOWER( DUF_ERROR_OPTION_NOT_PARSED );
  DOR( r, duf_clarify_xcmd_full( extended, optargg, istage, xtable, no, source ) );
  DUF_TRACE( options, +5, "xname:%s; arg:%s; no:%d", extended ? extended->o.name : "?", optargg, no );
  DEBUG_E_UPPER( DUF_ERROR_OPTION_NOT_PARSED );
  DEBUG_ENDR( r );
}
#endif
/*
 * return 
 *               =0  for normal options
 * or errorcode (<0) for error
 * */
int
duf_clarify_opt( duf_option_code_t codeval, int longindex, const char *optargg, duf_option_stage_t istage, duf_option_source_t source )
{
  DEBUG_STARTR( r );
  const duf_longval_extended_t *extended = NULL;
  const duf_longval_extended_table_t *xtable = NULL;
  int no = 0;

  assert( ( int ) codeval >= 0 );
  /* short always corresponds long (in my case) - find it */
  DUF_TRACE( options, +5, "parse option longindex:%d", longindex );
  if ( longindex < 0 )
  {
    extended = duf_find_codeval_extended_std( codeval, &xtable, &r );
    /* DUF_TEST_R1( r ); */
    DUF_TRACE( options, +5, "@@%s found by codeval of option %d (%c) => [--%s] (%s)", extended ? "" : "not", codeval, codeval > ' '
               && codeval <= 'z' ? codeval : '?', extended ? extended->o.name : "?", duf_error_name_i( r ) );
  }
  else if ( !extended )
  {
    extended = duf_longindex2extended( longindex, &xtable, &no );
    /* DUF_TEST_R1( r ); */
    DUF_TRACE( options, +5, "@@found by codeval of option %d (%c) => [--%s] (%s)", codeval, codeval > ' '
               && codeval <= 'z' ? codeval : '?', extended ? extended->o.name : "?", duf_error_name_i( r ) );
  }
  DUF_TRACE( options, +5, "parse option codeval: %d (%c) longindex:%d [--%s] (%s)", codeval, codeval > ' '
             && codeval <= 'z' ? codeval : '?', longindex, extended ? extended->o.name : "?", duf_error_name_i( r ) );
  if ( DUF_NOERROR( r ) )
  {
    if ( extended )
    {
      DOR( r, DUF_WRAPPED( duf_clarify_xcmd_full ) ( extended, optargg, istage, xtable, no, source ) );
    }
    else                        /* switch is useless !... ?? */
      switch ( ( int ) codeval )
      {
      case ':':
        DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_VALUE );
        DUF_TEST_R( r );
        DUF_TRACE( options, +5, "* options r: %d", r );
        break;
      case '?':
        DUF_MAKE_ERRORM( r, DUF_ERROR_OPTION, "* options r: %d", r );
        DUF_TEST_R( r );
        DUF_TRACE( options, +5, "* options r: %d", r );
        break;
      default:
        DUF_MAKE_ERRORM( r, DUF_ERROR_OPTION, "* options r: %d; codeval:%d; longindex:%d;", r, codeval, longindex );
        DUF_TEST_R( r );
        DUF_TRACE( options, +5, "* options r: %d; codeval:%d;", r, codeval );
        break;
      }
  }
  DEBUG_ENDR( r );
}

/* 20150924.144106 */
int
duf_clarify_opt_x( duf_option_code_t codeval, int longindex, const char *optargg, duf_option_stage_t istage, duf_option_source_t source )
{
  DEBUG_STARTR( r );
  char *oa;

  oa = duf_string_options_expand( optargg, '?' );
  DOR( r, duf_clarify_opt( codeval, longindex, oa, istage, source ) ); /* => duf_clarify_xcmd_full */
  mas_free( oa );
  DEBUG_ENDR( r );
}

/* 20150924.144102 */
int
duf_clarify_argv( mas_argvc_t * ptarg, mas_cargvc_t * pcarg, int pos )
{
  DEBUG_STARTR( r );
  mas_del_argv( ptarg->argc, ptarg->argv, 0 );
  ptarg->argc = 0;
  ptarg->argv = NULL;

  ptarg->argc = mas_add_argv_argv( ptarg->argc, &ptarg->argv, pcarg->argc, pcarg->argv, pos );
  DUF_TRACE( options, 5, "@@@@@argc:%d", ptarg->argc );

  /* targ.argv becomes valid here - may init pdi etc. */
  DEBUG_ENDR( r );
}
