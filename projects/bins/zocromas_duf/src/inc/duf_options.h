#ifndef MAS_DUF_OPTIONS_H
#  define MAS_DUF_OPTIONS_H

#  include "duf_option_types.h"
#  include "duf_options_def.h"

int duf_show_options( const char *a0 );

char *duf_restore_options( const char *a0 );

int duf_cli_option_by_string( const char *string );

int duf_env_options( int argc, char *argv[] );
int duf_infile_options( int argc, char *argv[] );

const duf_longval_extended_t *duf_find_longval_extended( duf_option_code_t code );
const char *duf_find_longval_help( duf_option_code_t code );

char *duf_cli_option_shorts( void );

const char *duf_option_cnames_tmp( duf_option_code_t code );
char *duf_option_names_d( duf_option_code_t code, const char *delim );

char *duf_option_names( duf_option_code_t code );
char *duf_option_names_d( duf_option_code_t code, const char *delim );


char *duf_option_description( duf_option_code_t code, const duf_longval_extended_t * extended );
char *duf_option_description_d( duf_option_code_t code, const duf_longval_extended_t * extended, const char *delimh, const char *delim );

void duf_option_smart_help( duf_option_class_t oclass );

#  define DUF_OPTION(lo)  \
    duf_config->lo
#  define DUF_OPTION_F(lo)  \
    DUF_OPTION(lo)
#  define DUF_OPTION_N(lo)  \
    DUF_OPTION(lo)
#  define DUF_OPTION_A(lo, a)  \
    DUF_OPTION(lo).a


#  define DUF_OPTION_RESTORETV(ocode, ptr, typ, up, lo, pref, value) \
  if ( ocode==DUF_OPTION_ ## up && value ) \
  { \
    _duf_restore_option_ ## typ(ptr, ocode, value ); \
  }

#  define DUF_OPTION_RESTOREV_B(ocode, ptr,  up, lo, pref, value) \
  DUF_OPTION_RESTORETV(ocode, ptr, b, up, lo, pref, value)

#  define DUF_OPTION_RESTORET( ocode, ptr, typ, up, lo, pref) \
  DUF_OPTION_RESTORETV( ocode, ptr, typ, up, lo, pref, DUF_OPTION(pref.lo))



/* # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # */
#  define DUF_OPTION_RESTORE_FLAGG(ocode, ptr, up, lo, pref, fls) \
    DUF_OPTION_RESTORET( ocode, ptr, b, FLAG_ ## up, lo, pref fls)
#  define DUF_OPTION_RESTORE_FLAG(ocode, ptr, up, lo, pref) \
    DUF_OPTION_RESTORE_FLAGG(ocode, ptr, up, lo, pref, .v.flag)

/*  ACQUIRE  */
#  define DUF_OPTION_ACQUIRE_FSET(lo)  \
   DUF_OPTION_F(lo) = 1

#  define DUF_OPTION_ACQUIRE_FLAGG(lo, pref, fls ) DUF_OPTION_ACQUIRE_FSET(pref fls.lo)
#  define DUF_OPTION_ACQUIRE_FLAG(lo, pref ) DUF_OPTION_ACQUIRE_FLAGG(lo, pref, .v.flag)

#  define DUF_OPTION_FLAGG(lo, pref, fls) DUF_OPTION_F(pref fls.lo)
#  define DUF_OPTION_FLAG(lo, pref) DUF_OPTION_FLAGG(lo, pref, .v.flag)

#  define DUF_OPTION_CASE_ACQUIRE_FLAGG(up, lo, pref, fls) \
    case DUF_OPTION_FLAG_##up: \
       DUF_OPTION_ACQUIRE_FLAGG( lo, pref, fls ); \
    break
#  define DUF_OPTION_CASE_ACQUIRE_FLAG(up, lo, pref) DUF_OPTION_CASE_ACQUIRE_FLAGG(up, lo, pref, .v.flag)


#  define DUF_OPTION_CASE_ACQUIRE_ACT_FLAG(up, lo) DUF_OPTION_CASE_ACQUIRE_FLAG(up, lo, cli.act)
#  define DUF_OPTION_CASE_ACQUIRE_U_FLAG(up, lo) DUF_OPTION_CASE_ACQUIRE_FLAG(up, lo, u)

#  define DUF_ACT_FLAG(lo) DUF_OPTION_FLAG(lo, cli.act)
#  define DUF_U_FLAG(lo) DUF_OPTION_FLAG(lo, u)

/* # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # */
#  define DUF_OPTION_ACQUIRE_NSET(lo, val)  \
   DUF_OPTION_N(lo) = val

#  define DUF_OPTION_ACQUIRE_NUM(lo)  \
      if ( optarg && *optarg ) \
        DUF_OPTION_ACQUIRE_NSET(lo, duf_strtol( optarg, &r) )

#  define DUF_OPTION_NUM(lo, pref) DUF_OPTION_N(pref.lo)


#  define DUF_OPTION_ACQUIRE_NUM_PLUS(lo)  \
      if ( optarg && *optarg ) \
        DUF_OPTION_ACQUIRE_NSET(lo, duf_strtol( optarg, &r) ); \
      else \
        DUF_OPTION_N(lo)++

#  define DUF_OPTION_RESTORE_NUM( ocode, ptr, up, lo, pref) \
    DUF_OPTION_RESTORET( ocode, ptr, i, up, lo, pref)

#  define  DUF_OPTION_CASE_ACQUIRE_NUM( up, lo, pref ) \
    case DUF_OPTION_ ## up: \
      DUF_OPTION_ACQUIRE_NUM( pref.lo ); \
    break

#  define  DUF_OPTION_CASE_ACQUIRE_NUM_PLUS( up, lo, pref ) \
    case DUF_OPTION_ ## up: \
      DUF_OPTION_ACQUIRE_NUM_PLUS( pref.lo ); \
    break

#  define DUF_OPTION_CASE_ACQUIRE_ACT_NUM(up, lo) DUF_OPTION_CASE_ACQUIRE_NUM(up, lo, cli.act)
#  define DUF_OPTION_CASE_ACQUIRE_ACT_NUM_PLUS(up, lo) DUF_OPTION_CASE_ACQUIRE_NUM_PLUS(up, lo, cli.act)
#  define DUF_OPTION_CASE_ACQUIRE_U_NUM(up, lo) DUF_OPTION_CASE_ACQUIRE_NUM(up, lo, u)
#  define DUF_OPTION_CASE_ACQUIRE_U_NUM_PLUS(up, lo) DUF_OPTION_CASE_ACQUIRE_NUM_PLUS(up, lo, u)

#  define DUF_ACT_NUM(lo) DUF_OPTION_NUM(lo, cli.act)
#  define DUF_U_NUM(lo) DUF_OPTION_NUM(lo, u)

/* # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # */
#  define DUF_OPTION_ACQUIRE_TRACE(lo)  \
    DUF_OPTION_ACQUIRE_NUM_PLUS(lo)

#  define DUF_OPTION_RESTORE_TRACE(ocode, ptr, up, lo) \
  DUF_OPTION_RESTORE_NUM(ocode, ptr,  up ## _TRACE, lo, cli.trace)

#  define DUF_OPTION_CASE_ACQUIRE_TRACE(up, lo) \
    case DUF_OPTION_## up ##_TRACE: \
      DUF_OPTION_ACQUIRE_TRACE( cli.trace.lo ); \
    break

/* # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # */
#  define DUF_OPTION_ACQUIRE_STR(lo)  \
      if ( optarg ) \
      { \
        mas_free( DUF_OPTION_N(lo) ); \
        DUF_OPTION_N(lo) = mas_strdup( optarg ); \
      }
#  define DUF_OPTION_RESTORE_STR( ocode, ptr, up, lo, pref) \
    DUF_OPTION_RESTORET( ocode, ptr, s, up, lo, pref)

#  define  DUF_OPTION_CASE_ACQUIRE_STR( up, lo, pref ) \
    case DUF_OPTION_ ## up: \
      DUF_OPTION_ACQUIRE_STR( pref.lo ); \
    break


/* # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # */


#  define DUF_OPTION_ACQUIRE_ARG(lo)  \
      if ( optarg ) DUF_OPTION_A(lo,argc)=mas_add_argv_arg(DUF_OPTION_A(lo,argc), &DUF_OPTION_A(lo,argv), optarg)

#  define  DUF_OPTION_CASE_ACQUIRE_ARG( up, lo, pref ) \
    case DUF_OPTION_ ## up: \
      DUF_OPTION_ACQUIRE_ARG( pref.lo ); \
    break

#  define DUF_OPTION_CASE_ACQUIRE_U_ARG(up, lo) DUF_OPTION_CASE_ACQUIRE_ARG(up, lo, u)


#endif