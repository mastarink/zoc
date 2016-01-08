#include <string.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance_options.h"

#include "duf_utils.h"          /* duf_strtol_suff duf_strtoll_suff ... */

#include "duf_option_defs.h"
#include "duf_option_find.h"
#include "duf_option_tmpdb.h"
#include "duf_options_config.h"

/* #include "duf_option_names.h" */

#include "duf_option_stage.h"
#include "duf_option_source.h"

/* ###################################################################### */
#include "duf_option_typed_defs.h"
#include "duf_option_typed.h"
/* ###################################################################### */


static FILE *
duf_open_file_special( const char *pname, char **popenedname, int overwrite, int *pr )
{
  int rpr = 0;
  FILE *newfile = NULL;
  int overw = 0;
  const char *mode = "w";
  struct stat st;
  int skipchar = 0;
  int ry;

  DUF_TRACE( io, 0, "to open %s", pname );
  if ( ( skipchar = ( *pname == '@' ) ) || overwrite )
    overw = 1;
  else if ( ( skipchar = ( *pname == '+' ) ) )
    mode = "a";
  if ( skipchar )
    pname++;
  *popenedname = mas_strdup( pname );
  ry = stat( pname, &st );
  if ( ry >= 0 && ( ( !S_ISCHR( st.st_mode ) || !( st.st_mode & S_IWUSR ) ) && ( !overw && *mode != 'a' ) ) )
  {
    /* DUF_SHOW_ERROR( "can't open file %s for writing file exists %llu / %llu chr:%d\n", pname, ( unsigned long long ) st.st_dev, */
    /*                 ( unsigned long long ) st.st_rdev, S_ISCHR( st.st_mode ) );                                                 */
    DUF_MAKE_ERRORM( rpr, DUF_ERROR_FILE_EXISTS, "can't open file %s for writing file exists %llu / %llu chr:%d", pname,
                     ( unsigned long long ) st.st_dev, ( unsigned long long ) st.st_rdev, S_ISCHR( st.st_mode ) );
  }
  else
  {
    newfile = fopen( pname, mode );
    if ( !newfile )
    {
      /* DUF_SHOW_ERROR( "can't open file %s\n", pname ); */
      DUF_MAKE_ERRORM( rpr, DUF_ERROR_OPEN, "can't open file %s", pname );
    }
  }
  if ( pr )
    *pr = rpr;
  return newfile;
}

/*  20150924.120214 for DUF_OUTPUTFILE DUF_OUTPUTFILE_A */
static int
duf_set_file_special( const char *pname, int overwrite, char **pfilename, FILE ** pfile, FILE * defout, int handleid )
{
  DEBUG_STARTR( r );
  FILE *newout;

  newout = defout;
  DUF_TRACE( io, 0, "to set file special %s", pname );
  if ( pfile )
  {
    *pfile = NULL;

    DUF_TRACE( io, 0, "handleid %d", handleid );
    if ( handleid )
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
    DUF_TRACE( io, 0, "pname %s", pname );
    if ( pfilename )
    {
      if ( *pfilename )
        mas_free( *pfilename );
      *pfilename = NULL;
    }
    if ( pname && *pname )
      newout = duf_open_file_special( pname, pfilename, overwrite, &r );
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
duf_clarify_xcmd_typed( const duf_longval_extended_t * extended, const char *optargg, duf_option_stage_t istage,
                        const duf_longval_extended_table_t * xtable, int noo, duf_option_source_t source )
{
  DEBUG_STARTR( r );
  duf_option_data_t od DUF_UNUSED;

  od.stage = istage;
  od.source = source;
  od.optarg = optargg;
  od.xtable = xtable;
  od.noo = noo;
  od.extended = extended;
  if ( DUF_OPTION_CHECK_STAGE( istage, extended, xtable ) ) /* duf_check_stage */
  {
    if ( extended->calltype )
    {
      DOR( r, duf_clarify_xcmd_typed_call( extended, optargg, istage, xtable, noo, source ) );
      if ( DUF_IS_ERROR_N( r, DUF_ERROR_OPTION_NOT_CLARIFIED ) )
      {
        DUF_CLEAR_ERROR( r, DUF_ERROR_OPTION_NOT_CLARIFIED );
        DOR( r, duf_clarify_xcmd_typed_gen( extended, optargg, istage, xtable, noo, source ) );
      }
    }
    else
    {
      DOR( r, duf_clarify_xcmd_typed_gen( extended, optargg, istage, xtable, noo, source ) );
    }
  }
  else
  {
    DUF_TRACE( options, 60, "@--%s='%s'; `noo`:%d : NOT for this stage; istage:%s", extended ? extended->o.name : "?", optargg ? optargg : "", noo,
               duf_optstage_name( istage ) );
    /* DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_NOT_FOUND ); */
  }
  DEBUG_ENDR( r );
}

static void *
duf_clarify_xcmd_typed_byteptr( const duf_longval_extended_t * extended )
{
  void *byteptr = NULL;

  DUF_TRACE( options, 60, "to switch by extended->relto=%d", extended->relto );
  switch ( extended->relto )
  {
  case DUF_OFFSET_none:
    assert( extended->m_offset == 0 );
    break;
  case DUF_OFFSET_config:
    DUF_TRACE( options, 60, "relto=%d", extended->relto );
#if 0
    byteptr = ( ( ( char * ) duf_config ) + extended->m_offset );
#else
    byteptr = duf_get_config_offset( extended->m_offset );
    /* assert( byteptr == ( ( ( char * ) duf_config ) + extended->m_offset ) ); */
#endif
    break;
#if 1
    /* case DUF_OFFSET_depthinfo:                                                  */
    /*   DUF_TRACE( options, 60, "relto=%d", extended->relto );                    */
    /*   byteptr = ( ( ( char * ) DUF_CONFIGG( scn.pdi ) ) + extended->m_offset ); */
    /*   break;                                                                    */
  case DUF_OFFSET_ufilter:
    DUF_TRACE( options, 60, "relto=%d", extended->relto );
    /* byteptr = ( ( ( char * ) DUF_CONFIGG( scn.puz ) ) + extended->m_offset ); */
    byteptr = duf_get_config_puz_offset( extended->m_offset );
    break;
#endif
  }
  return byteptr;
}

int
duf_clarify_xcmd_typed_gen( const duf_longval_extended_t * extended, const char *optargg, duf_option_stage_t istage,
                            const duf_longval_extended_table_t * xtable DUF_UNUSED, int noo, duf_option_source_t source )
{
  DEBUG_STARTR( r );

  /* if ( !extended )                         */
  /*   DUF_MAKE_ERROR( r, DUF_ERROR_OPTION ); */

  DUF_TRACE( options, 60, "to clarify typed:'%s'; `noo`:%d; stage:%s; source:%s", extended ? extended->o.name : "?", noo,
             duf_optstage_name( istage ), duf_optsource_name( source ) );
/*
 * if arg is help option
 * return class id for options to display the help
 * */

  if ( DUF_NOERROR( r ) )
  {
    unsigned doplus = 0;
    void *byteptr = NULL;

    byteptr = duf_clarify_xcmd_typed_byteptr( extended );
    DUF_TRACE( options, 60, "to check stage; istage:%s", duf_optstage_name( istage ) );
    /* if ( DUF_OPTION_CHECK_STAGE( istage, extended, xtable ) ) (* duf_check_stage *) *//* moved upper */
    {
      int nof;

      nof = noo;
      DUF_TRACE( options, 60, "stage OK; switching by extended->vtype=%d", extended->vtype );
      switch ( extended->vtype )
      {
      case DUF_OPTION_VTYPE_NONE:
        DUF_TRACE( options, 70, "vtype NONE" );
#if 0
        if ( DUF_NOERROR( r ) )
          DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_NOT_PARSED ); // Why was ???
        /* DUF_TEST_R( r ); */
#endif
        break;
      case DUF_OPTION_VTYPE_NOOP:
        DUF_TRACE( options, 70, "vtype NOOP" );
        break;
      case DUF_OPTION_VTYPE_UPLUS: /* stage (?) not SETUP *//* --trace-...; --debug; --verbose */
        DUF_TRACE( options, 70, "vtype UPLUS" );
        if ( noo )
          DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_NOT_PARSED );
        if ( DUF_NOERROR( r ) )
          doplus = 1;
      case DUF_OPTION_VTYPE_NUM: /* stage SETUP *//* --max-...; --min-...; --output-level; --use-format; etc. (misc) */
        DUF_TRACE( options, 70, "vtype NUM --%s='%s'", extended->o.name, optargg ? optargg : "" );
        if ( noo )
          DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_NOT_PARSED );
        DUF_NUMOPT( noo, r, unsigned, doplus, duf_strtol_suff );


        DUF_TRACE( options, 70, "(%d) vtype NUM --%s='%s'", r, extended->o.name, optargg ? optargg : "" );
        break;
      case DUF_OPTION_VTYPE_NUML: /* stage SETUP *//* not used ?! */
        DUF_TRACE( options, 70, "vtype NL" );
        if ( noo )
          DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_NOT_PARSED );
        DUF_NUMOPT( noo, r, unsigned long, 0, duf_strtol_suff );


        break;
      case DUF_OPTION_VTYPE_NUMLL: /* stage SETUP *//* not used ?! */
        DUF_TRACE( options, 70, "vtype NLL" );
        if ( noo )
          DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_NOT_PARSED );
        DUF_NUMOPT( noo, r, unsigned long long, 0, duf_strtoll_suff );


        break;
      case DUF_OPTION_VTYPE_MIN: /* stage SETUP */
        DUF_TRACE( options, 70, "vtype MIN" );
        if ( noo )
          DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_NOT_PARSED );
        DUF_MOPT( noo, r, duf_limits_t, min, duf_strtol_suff );
        break;
      case DUF_OPTION_VTYPE_MAX: /* stage SETUP */
        DUF_TRACE( options, 70, "vtype MAX" );
        if ( noo )
          DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_NOT_PARSED );
        DUF_MOPT( noo, r, duf_limits_t, max, duf_strtol_suff );
        break;
      case DUF_OPTION_VTYPE_MINMAX: /* stage SETUP */
        DUF_TRACE( options, 70, "vtype MINMAX" );
        if ( noo )
          DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_NOT_PARSED );
        DUF_MINMAXOPT( noo, r, duf_limits_t, duf_strtol_suff );
        break;

      case DUF_OPTION_VTYPE_MINLL: /* stage SETUP */
        DUF_TRACE( options, 70, "vtype MINLL" );
        if ( noo )
          DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_NOT_PARSED );
        DUF_MOPT( noo, r, duf_limitsll_t, min, duf_strtol_suff );
        break;
      case DUF_OPTION_VTYPE_MAXLL: /* stage SETUP */
        DUF_TRACE( options, 70, "vtype MAXLL" );
        if ( noo )
          DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_NOT_PARSED );
        DUF_MOPT( noo, r, duf_limitsll_t, max, duf_strtoll_suff );
        break;
      case DUF_OPTION_VTYPE_MINMAXLL: /* stage SETUP */
        DUF_TRACE( options, 70, "vtype MINMAXLL" );
        if ( noo )
          DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_NOT_PARSED );
        DUF_MINMAXOPT( noo, r, duf_limitsll_t, duf_strtoll_suff );
        break;

      case DUF_OPTION_VTYPE_NOFLAG: /* stage SETUP */
        DUF_TRACE( options, 70, "vtype NOFLAG" );
        nof = !nof;
      case DUF_OPTION_VTYPE_FLAG: /* stage SETUP *//*  unsigned set of flags */
        DUF_TRACE( options, 70, "vtype %s %x", QSTR( DUF_OPTION_VTYPE_FLAG ) + 17, extended->afl.bit );
        {
          if ( extended->invert )
            nof = !nof;
          if ( DUF_NOERROR( r ) )
          {
            unsigned *pi;

            pi = ( unsigned * ) byteptr;
            if ( nof )
              ( *pi ) &= ~extended->afl.bit;
            else
              ( *pi ) |= extended->afl.bit;
            /* duf_config->opt.act.v.flag.info = 1; */
            DUF_TRACE( options, +140, "@@@@@@[%d] %p %p :%s: %x %x (%x)", nof, byteptr, pi, extended->o.name, *pi,
                       extended->afl.bit, ( *pi | extended->afl.bit ) );
            DUF_TEST_R( r );
          }
        }
        break;
#if 0
      case DUF_OPTION_VTYPE_PFLAG:
        DUF_TRACE( options, 70, "vtype PFLAG" );
        if ( noo )
          DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_NOT_PARSED );
        if ( DUF_NOERROR( r ) )
        {
          unsigned *pi;

          pi = ( unsigned * ) byteptr;
          DUF_PRINTF( 0, "%s is %s", extended->o.name, ( ( *pi ) & extended->afl.bit ) ? "ON" : "OFF" );
          /* DUF_TEST_R( r ); */
        }
        break;
#endif
      case DUF_OPTION_VTYPE_NOSFLAG: /* stage SETUP */
        DUF_TRACE( options, 70, "vtype NOFLAG" );
        nof = !nof;
      case DUF_OPTION_VTYPE_SFLAG: /* stage SETUP *//*  unsigned short set of flags */
        DUF_TRACE( options, 70, "vtype SFLAG" );
        if ( DUF_NOERROR( r ) )
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
#if 0
      case DUF_OPTION_VTYPE_PSFLAG:
        DUF_TRACE( options, 70, "vtype PSFLAG" );
        if ( noo )
          DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_NOT_PARSED );
        if ( DUF_NOERROR( r ) )
        {
          unsigned *pi;

          pi = ( unsigned * ) byteptr;
          DUF_PRINTF( 0, "(s) %s is %s", extended->o.name, ( ( *pi ) & extended->afl.sbit ) ? "ON" : "OFF" );
          /* DUF_TEST_R( r ); */
        }
        break;
#endif
      case DUF_OPTION_VTYPE_XCHR: /* stage SETUP */
        DUF_TRACE( options, 70, "vtype XCHR for %s='%s'", extended->o.name, optargg ? optargg : "" );
        if ( noo )
          DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_NOT_PARSED );
        if ( DUF_NOERROR( r ) )
        {
          char *pchr;

          pchr = ( char * ) byteptr;
          if ( optargg )
          {
            unsigned cc = 0;

            sscanf( optargg, "%x", &cc );
            *pchr = ( char ) cc;
            DUF_TRACE( options, +150, "char set(%x):'%c' @%p", cc, cc, pchr );
          }
        }
        break;
      case DUF_OPTION_VTYPE_STR: /* stage SETUP */
        /* FIXME DUF_OPTION_VTYPE_PSTR vs. DUF_OPTION_VTYPE_STR */
        DUF_TRACE( options, 70, "vtype STR for %s='%s'", extended->o.name, optargg ? optargg : "" );
        if ( noo )
          DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_NOT_PARSED );
        if ( DUF_NOERROR( r ) )
        {
          char **pstr;

          pstr = ( char ** ) byteptr;
          if ( pstr && *pstr )
            mas_free( *pstr );
          *pstr = NULL;
          if ( optargg )
          {
            *pstr = mas_strdup( optargg );
            DUF_TRACE( options, +140, "string set:%s @%p", optargg, pstr );
          }
        }
        break;
      case DUF_OPTION_VTYPE_CSTR: /* stage SETUP */
        DUF_TRACE( options, 70, "vtype %s for %s='%s'", QSTR( DUF_OPTION_VTYPE_CSTR ) + 17, extended->o.name, optargg ? optargg : "" );
        if ( noo )
          DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_NOT_PARSED );
        if ( DUF_NOERROR( r ) )
        {
          duf_config_string_t *pcs;

          pcs = ( duf_config_string_t * ) byteptr;
          if ( pcs && pcs->value )
            mas_free( pcs->value );
          pcs->value = NULL;
          if ( optargg )
          {
            pcs->value = mas_strdup( optargg );
            DUF_TRACE( options, +140, "string set:%s @%p", optargg, pcs->value );
          }
        }
        break;
#if 0
      case DUF_OPTION_VTYPE_PSTR:
        /* FIXME DUF_OPTION_VTYPE_PSTR vs. DUF_OPTION_VTYPE_STR */
        DUF_TRACE( options, 70, "vtype PSTR" );


        if ( DUF_NOERROR( r ) )
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
            DUF_TRACE( options, +150, "pstring set:%s", optargg );
          }
          else if ( extended->m_hasoff == 1 ) /* if  extended->m_hasoff == 1, then mcfg_offset is offset */
          {
            DUF_PRINTF( 0, "%s", ( *pstr ) );
          }
        }
        break;
#endif
      case DUF_OPTION_VTYPE_ARGV: /* stage SETUP */
        /* case DUF_OPTION_VTYPE_PAA: */
        DUF_TRACE( options, 70, "vtype PAA" );

        if ( DUF_NOERROR( r ) )
        {
          mas_argvc_t *parg;

          parg = ( mas_argvc_t * ) byteptr;
          if ( noo )
          {
            if ( parg )
              mas_argv_delete( parg->argc, parg->argv );
          }
          else if ( optargg && *optargg )
          {
            parg->argc = mas_add_argv_arg( parg->argc, &parg->argv, optargg );
          }
          else if ( extended->m_hasoff == 1 ) /* if  extended->m_hasoff == 1, then mcfg_offset is offset */
          {
            char *str = mas_argv_string( parg->argc, parg->argv, 0 );

            DUF_PRINTF( 0, "%s", ( str ) );
            mas_free( str );
          }
        }
        break;
      case DUF_OPTION_VTYPE_TDB: /* stage SETUP */
        DUF_TRACE( options, 70, "vtype TDB" );

        if ( DUF_NOERROR( r ) )
        {
          if ( noo )
          {
#if 0
            duf_tmpdb_delete( extended->o.name, optargg );
#endif
          }
          else if ( optargg && *optargg )
            DOR( r, duf_tmpdb_add( extended->o.val, extended->o.name, optargg ) );
        }
        break;
        /*
         * DATETIME, MINMAXDATETIME, MINDATETIME, MAXDATETIME
         * */
      case DUF_OPTION_VTYPE_DATETIME: /* stage SETUP */
        DUF_TRACE( options, 70, "vtype DATETIME" );
        if ( noo )
          DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_NOT_PARSED );
        DUF_NUMOPT( noo, r, unsigned long long, 0, duf_strtime2long );

        break;
      case DUF_OPTION_VTYPE_MINMAXDATETIME: /* stage SETUP */
        DUF_TRACE( options, 70, "vtype MINMAXDATETIME" );
        if ( noo )
          DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_NOT_PARSED );
        DUF_MINMAXOPT( noo, r, duf_limitsll_t, duf_strtime2long );
        break;
      case DUF_OPTION_VTYPE_MINDATETIME: /* stage SETUP */
        DUF_TRACE( options, 70, "vtype MINDATETIME" );
        if ( noo )
          DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_NOT_PARSED );
        DUF_MOPT( noo, r, duf_limitsll_t, min, duf_strtime2long );
        break;
      case DUF_OPTION_VTYPE_MAXDATETIME: /* stage SETUP */
        DUF_TRACE( options, 70, "vtype MAXDATETIME" );
        if ( noo )
          DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_NOT_PARSED );
        DUF_MOPT( noo, r, duf_limitsll_t, max, duf_strtime2long );
        break;
#if 0
        /*
         * _CALL
         * */
      case DUF_OPTION_VTYPE_GEN_CALL:
	assert(0);
        DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_NOT_CLARIFIED );
        break;
      case DUF_OPTION_VTYPE_EV_CALL: /* stage Not? SETUP *//* call with nothing (VV:void-void) */
	assert(0);
        DUF_TRACE( options, 70, "vtype EV_CALL" );
        if ( noo )
          DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_NOT_PARSED );
#if 0
        if ( DUF_NOERROR( r ) )
        {
          if ( extended->call.fdesc.ev.func )
            DUF_MAKE_ERROR( r, ( extended->call.fdesc.ev.func ) (  ) );
          else
            DUF_MAKE_ERROR( r, DUF_ERROR_NO_FUNC );
        }
#else
        IF_DORF( r, extended->call.fdesc.ev.func ); /* fixed no arg */
#endif
        break;
      case DUF_OPTION_VTYPE_EIA_CALL: /* stage Not? SETUP *//* call with numeric (int) arg from table (EIA:void-int-arg) */
	assert(0);
        DUF_TRACE( options, 70, "vtype EIA_CALL" );
        if ( noo )
          DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_NOT_PARSED );
#if 0
        if ( DUF_NOERROR( r ) )
        {
          if ( extended->call.fdesc.eia.func )
            DOR( r, ( extended->call.fdesc.eia.func ) ( extended->call.fdesc.eia.arg ) );
          else
            DUF_MAKE_ERROR( r, DUF_ERROR_NO_FUNC );
        }
#else
        IF_DORF( r, extended->call.fdesc.eia.func, extended->call.fdesc.eia.arg ); /* fixed int arg */
#endif
        break;
      case DUF_OPTION_VTYPE_SAS_CALL: /* stage Not? SETUP *//* call with constant string arg from table (SAS:void-string-sarg) */
	assert(0);
        DUF_TRACE( options, 70, "vtype SAS_CALL" );
        if ( noo )
          DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_NOT_PARSED );
#if 0
        if ( DUF_NOERROR( r ) )
        {
          if ( extended->call.fdesc.sa.func )
            DOR( r, ( extended->call.fdesc.sa.func ) ( extended->call.fdesc.sa.arg ) );
          else
            DUF_MAKE_ERROR( r, DUF_ERROR_NO_FUNC );
        }
#else
        IF_DORF( r, extended->call.fdesc.sas.func, extended->call.fdesc.sas.arg ); /* fixed string arg */
#endif
        break;
      case DUF_OPTION_VTYPE_SAN_CALL: /* stage Not? SETUP *//* call with optarg and constant num arg from table (SAN:void-string-sarg) */
	assert(0);
        DUF_TRACE( options, 70, "vtype SAN_CALL" );
        if ( noo )
          DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_NOT_PARSED );
#if 0
        if ( DUF_NOERROR( r ) )
        {
          if ( extended->call.fdesc.sa.func )
            DOR( r, ( extended->call.fdesc.sa.func ) ( extended->call.fdesc.sa.arg ) );
          else
            DUF_MAKE_ERROR( r, DUF_ERROR_NO_FUNC );
        }
#else
        IF_DORF( r, extended->call.fdesc.san.func, optargg, extended->call.fdesc.san.arg ); /* optarg and fixed num arg */
#endif
        break;
      case DUF_OPTION_VTYPE_A_CALL: /* stage Not? SETUP *//* call with carg[cv] (A:argv) */
	assert(0);
        DUF_TRACE( options, 70, "vtype A_CALL" );
        if ( noo )
          DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_NOT_PARSED );
        /* assert( DUF_CONFIGG( cli.carg.argv ) ); */
        assert( duf_cli_options_get_carg(  ) && duf_cli_options_get_carg(  )->argv );
#if 0
        if ( DUF_NOERROR( r ) )
        {
          if ( extended->call.fdesc.a.func )
            DOR( r, ( extended->call.fdesc.a.func ) ( DUF_CONFIGG( cli.carg.argc ), DUF_CONFIGG( cli.carg.argv ) ) );
          else
            DUF_MAKE_ERROR( r, DUF_ERROR_NO_FUNC );
        }
#else
        /* IF_DORF( r, extended->call.fdesc.a.func, DUF_CONFIGG( cli.carg.argc ), DUF_CONFIGG( cli.carg.argv ) ); */
        IF_DORF( r, extended->call.fdesc.a.func, duf_cli_options_get_carg(  )->argc, duf_cli_options_get_carg(  )->argv ); /* argc and argv */
#endif
        break;
      case DUF_OPTION_VTYPE_N_CALL: /* stage Not? SETUP *//* call with numeric optarg (N:numeric) */
	assert(0);
        DUF_TRACE( options, 70, "vtype N_CALL" );
        if ( noo )
          DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_NOT_PARSED );
#if 0
        if ( DUF_NOERROR( r ) )
        {
          if ( extended->call.fdesc.n.func )
            DOR( r, ( ( extended->call.fdesc.n.func ) ( duf_strtol_suff( optargg, &r ) ) ) );
          else
            DUF_MAKE_ERROR( r, DUF_ERROR_NO_FUNC );
        }
#else
        IF_DORF( r, extended->call.fdesc.n.func, duf_strtol_suff( optargg, &r ) );
#endif
        break;
      case DUF_OPTION_VTYPE_S_CALL: /* stage Not? SETUP *//* call with string optarg (S: string) */
	assert(0);
        DUF_TRACE( options, 70, "vtype S_CALL" );
        if ( noo )
          DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_NOT_PARSED );
#if 0
        if ( DUF_NOERROR( r ) )
        {
          if ( extended->call.fdesc.s.func )
            DOR( r, ( ( extended->call.fdesc.s.func ) ( optargg ) ) );
          else
            DUF_MAKE_ERROR( r, DUF_ERROR_NO_FUNC );
        }
#else
        IF_DORF( r, extended->call.fdesc.s.func, optargg ); /* optarg */
#endif
        break;
      case DUF_OPTION_VTYPE_AA_CALL: /* stage Not? SETUP *//* call with carg (AA:argv-argv) */
	assert(0);
        DUF_TRACE( options, 70, "vtype AA_CALL" );
        if ( noo )
          DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_NOT_PARSED );
        /* assert( DUF_CONFIGG( cli.carg.argc ) ); */
        /* assert( DUF_CONFIGG( cli.carg.argv ) ); */
        assert( duf_cli_options_get_carg(  ) && duf_cli_options_get_cargc(  ) && duf_cli_options_get_cargv(  ) );
#if 0
        if ( DUF_NOERROR( r ) )
        {
          if ( extended->call.fdesc.aa.func )
            DOR( r, ( extended->call.fdesc.aa.func ) ( DUF_CONFIGG( cli.carg ) ) );
          else
            DUF_MAKE_ERROR( r, DUF_ERROR_NO_FUNC );
        }
#else
        /* IF_DORF( r, extended->call.fdesc.aa.func, DUF_CONFIGG( cli.carg ) ); */
        IF_DORF( r, extended->call.fdesc.aa.func, duf_cli_options_get_carg(  ) ); /* arg[cv] as mas_cargvc_t */
#endif
        break;
      case DUF_OPTION_VTYPE_TN1_CALL: /* stage Not? SETUP *//* call with targ + numeric optarg (TN: targ and numeric) */
	assert(0);
        DUF_TRACE( options, 70, "vtype TN1_CALL" );
        if ( noo )
          DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_NOT_PARSED );
        IF_DORF( r, extended->call.fdesc.tn1.func, duf_cli_options_get_targ(  ), duf_strtol_suff( optargg, &r ) ); /* targ as mas_argvc_t and numeric optarg */
        break;
#if 0
      case DUF_OPTION_VTYPE_TN2_CALL: /* stage Not? SETUP *//* call with targ[cv] + numeric optarg (TN: targ and numeric) */
	assert(0);
        DUF_TRACE( options, 70, "vtype TN2_CALL" );
        if ( noo )
          DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_NOT_PARSED );
        assert( &duf_cli_options_get_targ(  )->argc == duf_cli_options_get_ptargc(  ) );
        assert( &duf_cli_options_get_targ(  )->argv == duf_cli_options_get_ptargv(  ) );
        IF_DORF( r, extended->call.fdesc.tn2.func, duf_cli_options_get_ptargc(  ), duf_cli_options_get_ptargv(  ), duf_strtol_suff( optargg, &r ) );
        break;
#endif
      case DUF_OPTION_VTYPE_TS1_CALL: /* stage Not? SETUP *//* call with targ[cv] + string optarg (TS: targ and string) */
	assert(0);
        DUF_TRACE( options, 70, "vtype TS1_CALL" );
        if ( noo )
          DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_NOT_PARSED );
        assert( &duf_cli_options_get_targ(  )->argc == duf_cli_options_get_ptargc(  ) );
        assert( &duf_cli_options_get_targ(  )->argv == duf_cli_options_get_ptargv(  ) );
        IF_DORF( r, extended->call.fdesc.ts1.func, duf_cli_options_get_targ(  ), optargg ); /* targ as mas_argvc_t and optarg */
        break;
      case DUF_OPTION_VTYPE_TS2_CALL: /* stage Not? SETUP *//* call with targ[cv] + string optarg (TS: targ and string) */
	assert(0);
        DUF_TRACE( options, 70, "vtype TS_CALL" );
        if ( noo )
          DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_NOT_PARSED );
        assert( &duf_cli_options_get_targ(  )->argc == duf_cli_options_get_ptargc(  ) );
        assert( &duf_cli_options_get_targ(  )->argv == duf_cli_options_get_ptargv(  ) );
        IF_DORF( r, extended->call.fdesc.ts2.func, duf_cli_options_get_ptargc(  ), duf_cli_options_get_ptargv(  ), optargg ); /* targc, targv and optarg */
        break;
#endif
        /* 
         * FILE
         * */
      case DUF_OPTION_VTYPE_FILE:
        DUF_TRACE( options, 70, "vtype FILE" );
        if ( noo )
          DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_NOT_PARSED );
        if ( DUF_NOERROR( r ) )
        {
          DUF_TRACE( io, 0, "DUF_OUTPUTFILE (%s)", extended->o.name );
          DUF_OUTPUTFILE( noo, r, duf_config_output_t, stderr );
          /* {                                                                                 */
          /*   char start_time[128] = "??";                                                    */
          /*                                                                                   */
          /*   duf_strflocaltime( start_time, sizeof( start_time ), "%Y%m%d.%H:%M:%S", NULL ); */
          /*   PF( "%s", start_time );                                                         */
          /* }                                                                                 */

          /* DUF_MAKE_ERROR(r, DUF_ERROR_OPTION_NOT_PARSED); */
        }
        break;
      }
      DUF_TRACE( options, +150, "@@@@@(%s)         this (%2d:%2d:%2d) stage; vtype=%2d; xname:%-20s; arg:'%s'; no:%d", mas_error_name_i( r ),
                 istage, extended->stage.min, extended->stage.max, extended->vtype, extended ? extended->o.name : "?", optargg ? optargg : "", noo );
    }
#if 0
    else
    {
      DUF_TRACE( options, 60, "@--%s='%s'; `noo`:%d : NOT for this stage; istage:%s", extended ? extended->o.name : "?", optargg ? optargg : "", noo,
                 duf_optstage_name( istage ) );
      /* DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_NOT_FOUND ); */
    }
#endif
  }
  DEBUG_ENDR( r );
  /* DEBUG_ENDR_YES( r, DUF_ERROR_OPTION_NOT_FOUND ); */
}

int
duf_clarify_xcmd_typed_call( const duf_longval_extended_t * extended, const char *optargg DUF_UNUSED, duf_option_stage_t istage DUF_UNUSED,
                             const duf_longval_extended_table_t * xtable DUF_UNUSED, int noo DUF_UNUSED, duf_option_source_t source DUF_UNUSED )
{
  DEBUG_STARTR( r );
  switch ( extended->calltype )
  {
  case DUF_OPTION_CALL_TYPE_NONE:
    DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_NOT_CLARIFIED );
    break;
  case DUF_OPTION_CALL_TYPE_EIA:
    if ( noo )
      DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_NOT_PARSED );
#if 0
    if ( DUF_NOERROR( r ) )
    {
      if ( extended->call.fdesc.eia.func )
        DOR( r, ( extended->call.fdesc.eia.func ) ( extended->call.fdesc.eia.arg ) );
      else
        DUF_MAKE_ERROR( r, DUF_ERROR_NO_FUNC );
    }
#else
    IF_DORF( r, extended->call.fdesc.eia.func, extended->call.fdesc.eia.arg ); /* fixed int arg */
#endif
    /* DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_NOT_CLARIFIED ); */
    break;
  case DUF_OPTION_CALL_TYPE_EV:
    if ( noo )
      DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_NOT_PARSED );
#if 0
    if ( DUF_NOERROR( r ) )
    {
      if ( extended->call.fdesc.ev.func )
        DUF_MAKE_ERROR( r, ( extended->call.fdesc.ev.func ) (  ) );
      else
        DUF_MAKE_ERROR( r, DUF_ERROR_NO_FUNC );
    }
#else
    IF_DORF( r, extended->call.fdesc.ev.func ); /* fixed no arg */
#endif
    /* DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_NOT_CLARIFIED ); */
    break;
  case DUF_OPTION_CALL_TYPE_A:
    if ( noo )
      DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_NOT_PARSED );
    /* assert( DUF_CONFIGG( cli.carg.argv ) ); */
    assert( duf_cli_options_get_carg(  ) && duf_cli_options_get_carg(  )->argv );
#if 0
    if ( DUF_NOERROR( r ) )
    {
      if ( extended->call.fdesc.a.func )
        DOR( r, ( extended->call.fdesc.a.func ) ( DUF_CONFIGG( cli.carg.argc ), DUF_CONFIGG( cli.carg.argv ) ) );
      else
        DUF_MAKE_ERROR( r, DUF_ERROR_NO_FUNC );
    }
#else
    /* IF_DORF( r, extended->call.fdesc.a.func, DUF_CONFIGG( cli.carg.argc ), DUF_CONFIGG( cli.carg.argv ) ); */
    IF_DORF( r, extended->call.fdesc.a.func, duf_cli_options_get_carg(  )->argc, duf_cli_options_get_carg(  )->argv ); /* argc and argv */
#endif
    /* DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_NOT_CLARIFIED ); */
    break;
  case DUF_OPTION_CALL_TYPE_AA:
    if ( noo )
      DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_NOT_PARSED );
    /* assert( DUF_CONFIGG( cli.carg.argc ) ); */
    /* assert( DUF_CONFIGG( cli.carg.argv ) ); */
    assert( duf_cli_options_get_carg(  ) && duf_cli_options_get_cargc(  ) && duf_cli_options_get_cargv(  ) );
#if 0
    if ( DUF_NOERROR( r ) )
    {
      if ( extended->call.fdesc.aa.func )
        DOR( r, ( extended->call.fdesc.aa.func ) ( DUF_CONFIGG( cli.carg ) ) );
      else
        DUF_MAKE_ERROR( r, DUF_ERROR_NO_FUNC );
    }
#else
    /* IF_DORF( r, extended->call.fdesc.aa.func, DUF_CONFIGG( cli.carg ) ); */
    IF_DORF( r, extended->call.fdesc.aa.func, duf_cli_options_get_carg(  ) ); /* arg[cv] as mas_cargvc_t */
#endif
    /* DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_NOT_CLARIFIED ); */
    break;
  case DUF_OPTION_CALL_TYPE_N:
    if ( noo )
      DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_NOT_PARSED );
#if 0
    if ( DUF_NOERROR( r ) )
    {
      if ( extended->call.fdesc.n.func )
        DOR( r, ( ( extended->call.fdesc.n.func ) ( duf_strtol_suff( optargg, &r ) ) ) );
      else
        DUF_MAKE_ERROR( r, DUF_ERROR_NO_FUNC );
    }
#else
    IF_DORF( r, extended->call.fdesc.n.func, duf_strtol_suff( optargg, &r ) );
#endif
    /* DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_NOT_CLARIFIED ); */
    break;
  case DUF_OPTION_CALL_TYPE_S:
    if ( noo )
      DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_NOT_PARSED );
#if 0
    if ( DUF_NOERROR( r ) )
    {
      if ( extended->call.fdesc.s.func )
        DOR( r, ( ( extended->call.fdesc.s.func ) ( optargg ) ) );
      else
        DUF_MAKE_ERROR( r, DUF_ERROR_NO_FUNC );
    }
#else
    IF_DORF( r, extended->call.fdesc.s.func, optargg ); /* optarg */
#endif
    /* DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_NOT_CLARIFIED ); */
    break;
  case DUF_OPTION_CALL_TYPE_SAS:
    if ( noo )
      DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_NOT_PARSED );
#if 0
    if ( DUF_NOERROR( r ) )
    {
      if ( extended->call.fdesc.sa.func )
        DOR( r, ( extended->call.fdesc.sa.func ) ( extended->call.fdesc.sa.arg ) );
      else
        DUF_MAKE_ERROR( r, DUF_ERROR_NO_FUNC );
    }
#else
    IF_DORF( r, extended->call.fdesc.sas.func, extended->call.fdesc.sas.arg ); /* fixed string arg */
#endif
    /* DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_NOT_CLARIFIED ); */
    break;
  case DUF_OPTION_CALL_TYPE_SAN:
    if ( noo )
      DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_NOT_PARSED );
#if 0
    if ( DUF_NOERROR( r ) )
    {
      if ( extended->call.fdesc.sa.func )
        DOR( r, ( extended->call.fdesc.sa.func ) ( extended->call.fdesc.sa.arg ) );
      else
        DUF_MAKE_ERROR( r, DUF_ERROR_NO_FUNC );
    }
#else
    IF_DORF( r, extended->call.fdesc.san.func, optargg, extended->call.fdesc.san.arg ); /* optarg and fixed num arg */
#endif
    /* DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_NOT_CLARIFIED ); */
    break;
  case DUF_OPTION_CALL_TYPE_TN1:
    if ( noo )
      DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_NOT_PARSED );
    IF_DORF( r, extended->call.fdesc.tn1.func, duf_cli_options_get_targ(  ), duf_strtol_suff( optargg, &r ) ); /* targ as mas_argvc_t and numeric optarg */
    /* DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_NOT_CLARIFIED ); */
    break;
#if 0
  case DUF_OPTION_CALL_TYPE_TN2:
    if ( noo )
      DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_NOT_PARSED );
    assert( &duf_cli_options_get_targ(  )->argc == duf_cli_options_get_ptargc(  ) );
    assert( &duf_cli_options_get_targ(  )->argv == duf_cli_options_get_ptargv(  ) );
    IF_DORF( r, extended->call.fdesc.tn2.func, duf_cli_options_get_ptargc(  ), duf_cli_options_get_ptargv(  ), duf_strtol_suff( optargg, &r ) );
    DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_NOT_CLARIFIED );
    break;
#endif
  case DUF_OPTION_CALL_TYPE_TS1:
    if ( noo )
      DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_NOT_PARSED );
    assert( &duf_cli_options_get_targ(  )->argc == duf_cli_options_get_ptargc(  ) );
    assert( &duf_cli_options_get_targ(  )->argv == duf_cli_options_get_ptargv(  ) );
    IF_DORF( r, extended->call.fdesc.ts1.func, duf_cli_options_get_targ(  ), optargg ); /* targ as mas_argvc_t and optarg */
    /* DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_NOT_CLARIFIED ); */
    break;
  case DUF_OPTION_CALL_TYPE_TS2:
    if ( noo )
      DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_NOT_PARSED );
    assert( &duf_cli_options_get_targ(  )->argc == duf_cli_options_get_ptargc(  ) );
    assert( &duf_cli_options_get_targ(  )->argv == duf_cli_options_get_ptargv(  ) );
    IF_DORF( r, extended->call.fdesc.ts2.func, duf_cli_options_get_ptargc(  ), duf_cli_options_get_ptargv(  ), optargg ); /* targc, targv and optarg */
    /* DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_NOT_CLARIFIED ); */
    break;
  }
  DEBUG_ENDR( r );
}
