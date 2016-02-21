#undef MAS_TRACING
/* #include <stddef.h> */

#include "duf_maintenance_z.h"
#include "duf_maintenance_tracen.h"
#include "duf_maintenance_errors.h"

#include "duf_config_util.h"

#include "duf_option_config.h"
#include "duf_option_clarify.h"
#include "duf_option_cfind.h"

#include "duf_option_stage.h"
#include "duf_option_source.h"

#include "duf_option_clarify_batch.h"

#include "duf_option_clarify_new.h"

/* ###################################################################### */
#include "duf_options_cli.h"
/* ###################################################################### */

DUF_UNUSED static char *
duf_clrfy_cli_opts_msgs( duf_option_code_t codeval, int optindd, int optoptt )
{
  const char *arg;
  static const char *msg = "Invalid option";
  char buffer[2048] = "";

#if 0
  arg = duf_cli_options_config( duf_get_config_cli(  ) )->carg.argv[optindd];
#else
  arg = duf_cli_options_get_cargvn( duf_get_config_cli(  ), optindd );
#endif
  if ( duf_verbose && duf_verbose(  ) > 0 )
    snprintf( buffer, sizeof( buffer ), "%s '%s' arg[%d]=\"%s\" [%u/%c/%c]", msg, arg, optindd, arg, codeval, codeval, optoptt );
  else
    snprintf( buffer, sizeof( buffer ), " %s '%s'", msg, arg );
  return mas_strdup( buffer );
}

#if 0
static void
duf_clrfy_cli_opts_msg( duf_option_code_t codeval, int optindd, int optoptt, const char *shorts_unused DUF_UNUSED )
{
/* const char *arg; */
/* static const char *msg = "Invalid option"; */

/* arg = DUF_CONFIGG( cli.carg.argv )[optindd]; */
# if 0
  if ( optoptt && codeval > ' ' && codeval <= 'z' )
  {
    if ( !duf_verbose || duf_verbose(  ) == 0 )
      _DUF_SHOW_ERROR( "@@@@@@@@@@@ V %s '-%c'      ", msg, optoptt );
    else
    {
      if ( codeval == '?' )
        _DUF_SHOW_ERROR( "@@@@@@@@@@@ A %s '-%c' arg[%d]=\"%s\"", msg, optoptt, optindd, arg );
      else
        _DUF_SHOW_ERROR( "@@@@@@@@@@@ B %s '-%c' arg[%d]=\"%s\" [%u/%c]", msg, optoptt, optindd, arg, codeval, codeval );
    }
  }
  else
# endif
  {
# if 0
    if ( duf_verbose && duf_verbose(  ) > 0 )
      _DUF_SHOW_ERROR( "@@@@@@@@@@@ %s '%s' arg[%d]=\"%s\" [%u/%c/%c]", msg, arg, optindd, arg, codeval, codeval, optoptt );
    else
      _DUF_SHOW_ERROR( "@@@@@@@@@@@ %s '%s'", msg, arg );
# else
    char *s;

    s = duf_clrfy_cli_opts_msgs( codeval, optindd, optoptt );
    _DUF_SHOW_ERROR( "@@@@@@@@@@@%s", s );
    mas_free( s );
# endif
# if 0
    for ( int i = 0; i < DUF_CONFIGG( cli.carg.argc ); i++ )
    {
      _DUF_SHOW_ERROR( "@@@ (%d) %c%d '%s'", optindd, i == optindd ? '*' : ' ', i, DUF_CONFIGG( cli.carg.argv )[i] );
    }
# endif
  }
}
#endif

static
SR( OPTIONS, lcoption_parse, int longindex, duf_option_code_t codeval, duf_option_stage_t istage, duf_option_adata_t * paod )
{

  DUF_TRACE( options, 40, "@@@@@@getopt_long: cv:%-4d =>  ('%c') '%s' li:%d; oi:%d; oo:%d; oe:%d; stage:%s", codeval, codeval > ' '
             && codeval <= 'z' ? codeval : '?', longindex >= 0 ? duf_cli_options_get_longopts_table( duf_get_config_cli(  ) )[longindex].name : "?",
             longindex, optind, optopt, opterr, duf_optstage_name( istage ) );
  {
    int optoptt = 0;

    optoptt = 0;
    if ( codeval == '?' )
    {
      char *msg = NULL;

      optoptt = optopt;

      msg = duf_clrfy_cli_opts_msgs( codeval, optind - 1, optoptt );

      DUF_TRACE( options, +140, "@@@@@@@%d:%s", optind - 1, duf_cli_options_get_carg( duf_get_config_cli(  ) )->argv[optind - 1] );
      if ( istage != DUF_OPTION_STAGE_BOOT )
        DUF_MAKE_ERRORM( QERRIND, DUF_ERROR_OPTION_NOT_FOUND, msg );
      mas_free( msg );

    /* DUF_CLEAR_ERROR( QERRIND, DUF_ERROR_OPTION_NOT_FOUND ); */
    }

#if 0                                                                /* 1:old clarify; 0:new clarify; see also duf_option_clarify_batch.c.... ; 20160115.170518 */
    CR( lcoption_clarify, longindex, codeval, optarg, istage, DUF_OPTION_SOURCE( CLI ) );
#else
    if ( QNOERR )
    {
      mas_cargvc_t *carg;
      const char *longoptname = NULL;
      const char *qarg = NULL;

      carg = duf_cli_options_get_carg( duf_get_config_cli(  ) );
      if ( optind > 0 )
        qarg = carg->argv[optind - 1];
    /* longoptname = duf_coption_find_name_at_std( codeval, QPERRIND ); */
      longoptname = duf_lcoption_find_name_at_std( codeval, &longindex, QPERRIND );
      assert( ( longindex > 0 && longoptname ) || ( longindex < 0 && !longoptname ) );
      if ( longindex < 0 /* && codeval == '?' */  && !longoptname && qarg && qarg[0] == '-' && qarg[1] == '-' && qarg[2] != '-' )
        longoptname = qarg + 2;

      if ( longoptname )
        CR( soption_xclarify_na_new_at_stdx_default, longoptname, optarg, 0 /* value_separator */ , istage, DUF_OPTION_SOURCE( CLI ),
            paod );
# if 0
      else if ( codeval == '?' )
      {
        SERR( OPTION_NOT_PARSED );
        assert( 0 );
      }
# endif
      else                                                           /* ERROR */
      {
        SERR( OPTION_NOT_PARSED );
      /* assert( 0 ); */
      }
    }
#endif
  }
  ER( OPTIONS, lcoption_parse, int longindex, duf_option_code_t codeval, duf_option_stage_t istage, duf_option_adata_t * paod );
}

static
SR( OPTIONS, cli_parse_targ, int optindd, duf_option_stage_t istage )
{
  int numxargv = 0;
  mas_cargvc_t *carg;

  carg = duf_cli_options_get_carg( duf_get_config_cli(  ) );
  numxargv = carg->argc - optindd;
  DUF_TRACE( options, 40, "@cli opts: stage:%s; carg->argc:%d; numxargv:%d; optindd:%d;", duf_optstage_name( istage ), carg->argc, numxargv,
             optindd );

/* 20160129.121211 DUF_OPTION_STAGE_SETUP Changed to  DUF_OPTION_STAGE_BOOT */
  if ( QNOERR && istage == DUF_OPTION_STAGE_BOOT && /* optind < carg.argc && */ numxargv > 0 )
  {
  /* to do once, at stage SETUP (?), for next stages */
    DUF_TRACE( options, +150, "(for targ) carg.argv[%d]=\"%s\"", optindd, carg->argv[optindd] );
    CR( argv_clarify, duf_cli_options_get_targ( duf_get_config_cli(  ) ), carg, optindd );

    if ( QNOERR )
    /* DUF_CONFIGWN( cli.targ_offset, duf_reorder_argvc_at_sign( DUF_CONFIGA( cli.targ ) ) ); */
      duf_cli_options_reset_targ_offset( duf_get_config_cli(  ) );
  }
  else
  {
    DUF_TRACE( options, +150, "(no targ) optind:%d; optindd:%d; numxargv:%d", optind, optindd, numxargv );
  }
  ER( OPTIONS, cli_parse_targ, int optindd, duf_option_stage_t istage );
}

/* 20160113.124316 */
static
SR( OPTIONS, cli_parse, const char *shorts, duf_option_stage_t istage, duf_option_adata_t * paod )
{
/* DUF_STARTR( r ); */

  mas_cargvc_t *carg;

/* int optoptt = 0; */
  int optindd = 0, optindp DUF_UNUSED = 0;

  DUF_E_LOWER( DUF_ERROR_OPTION_NOT_FOUND );
  optopt = 0;
  opterr = 0;
  optind = 1;
  optindp = optind;

  carg = duf_cli_options_get_carg( duf_get_config_cli(  ) );

  DUF_TRACE( options, 40, "@to clarify cli opts: stage:%s; carg->argc:%d", duf_optstage_name( istage ), carg->argc );

  {
    duf_option_code_t codeval;
    duf_option_t *longtable;

    longtable = duf_cli_options_get_longopts_table( duf_get_config_cli(  ) );

  /* optind
   *   inside loop means next arg to process in non-permuted list
   *   outside loop means first not processed arg in permuted list
   * */
    do
    {
      int longindex;

      optopt = 0, longindex = -1;

      codeval = getopt_long( carg->argc, carg->argv, shorts, longtable, &longindex );
      if ( codeval >= 0 )
      {
        optindd = optind;
      /* codeval >=0 && codeval!='?' && longindex<0 means short => in this case longindex=...  */
#if 1
        CR( lcoption_parse, longindex, codeval, istage, paod );
/* TODO */
#else
        const duf_longval_extended_t *extended = NULL;

        extended = duf_noption_xfind_no_at_stdx( name, arg ? 1 : 0, 1 /* soft */ , pno, pcnt, QPERRIND );
        CR( xoption_parse, extended, istage );
#endif
        optindp = optind;
      }
    }
    while ( QNOERR && codeval >= 0 );
  }
  CR( cli_parse_targ, optindd, istage );
/* DUF_ENDR_UPPER( r, DUF_ERROR_OPTION_NOT_FOUND ); */
  DUF_E_UPPER( DUF_ERROR_OPTION_NOT_FOUND );
/* DUF_ENDR( r ); */
  ER( OPTIONS, cli_parse, const char *shorts, duf_option_stage_t istage, duf_option_adata_t * paod );
}

SR( OPTIONS, source_cli_parse, duf_option_stage_t istage, duf_int_void_func_t cb_do_interactive DUF_UNUSED,
    duf_cpchar_void_func_t cb_prompt_interactive DUF_UNUSED, duf_option_adata_t * paod, duf_option_source_code_t sourcecode DUF_UNUSED )
{
/* DUF_STARTR( r ); */

  DUF_TRACE( optsource, 0, "@   source:%s", duf_optsourcecode_name( sourcecode ) );
  DUF_TRACE( options, 20, "@@@@cli options; stage:%s", duf_optstage_name( istage ) );

  CR( cli_parse, duf_cli_options_get_shorts( duf_get_config_cli(  ) ), istage, paod );

  if ( istage < DUF_OPTION_STAGE_SETUP )
    DUF_CLEAR_ERROR( QERRIND, DUF_ERROR_OPTION_NOT_FOUND );

  DUF_TRACE( options, 20, "@@@@cli options done; stage:%s (%d:%s)", duf_optstage_name( istage ), QERRIND, QERRNAME );
/* DUF_ENDR( r ); */

  ER( OPTIONS, source_cli_parse, duf_option_stage_t istage, duf_int_void_func_t cb_do_interactive DUF_UNUSED,
      duf_cpchar_void_func_t cb_prompt_interactive DUF_UNUSED, duf_option_adata_t * paod, duf_option_source_code_t sourcecode DUF_UNUSED );
}
