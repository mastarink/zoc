/* #include <stdarg.h> */
#include <string.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>

#include <mastar/tools/mas_tools.h>
#include <mastar/tools/mas_arg_tools.h>
#include <mastar/tools/mas_utils_path.h>

#include "duf_maintenance.h"

#include "duf_config_wref.h"

#include "duf_dbg.h"
#include "duf_pdi_credel.h"

#include "duf_ufilter.h"

/* #include "duf_option_extended.h" */
#include "duf_option_names.h"

/* #include "duf_config_ref.h" */
/* ###################################################################### */
#include "duf_config_util.h"
/* ###################################################################### */

extern duf_config_t *duf_config /* __attribute__( ( weak ) ) */ ;

duf_config_t *
duf_get_config( void )
{
  assert( duf_config );
  return duf_config;
}

void *
duf_get_config_offset( unsigned long off )
{
  assert( duf_config );
  return duf_config ? ( ( ( char * ) duf_config ) + off ) : NULL;
}

void *
duf_get_config_puz_offset( unsigned long off )
{
  assert( duf_config );
  return duf_config && duf_config->scn.puz ? ( ( ( char * ) duf_config->scn.puz ) + off ) : NULL;
}

duf_config_cli_t *
duf_get_config_cli( void )
{
  assert( duf_config );
  return duf_config ? &duf_config->cli : NULL;
}

duf_config_opt_t *
duf_get_config_opt( void )
{
  return duf_config ? &duf_config->opt : NULL;
}

int
duf_verbose( void )
{
  return duf_config ? duf_config->opt.dbg.verbose : 0;
}

int
duf_output_level( void )
{
  return duf_config ? duf_config->opt.output.level : 0;
}

FILE *
duf_output_file_c( const duf_config_t * cfg )
{
  return cfg && cfg->opt.output.out ? cfg->opt.output.out : stdout;
}

FILE *
duf_output_file( void )
{
  return duf_output_file_c( duf_config );
}

FILE *
duf_trace_file_c( const duf_config_t * cfg )
{
  return cfg && cfg->MAST_TRACE_AT_CFG.output.out ? cfg->MAST_TRACE_AT_CFG.output.out : stdout;
}

FILE *
duf_trace_file( void )
{
  return duf_trace_file_c( duf_config );
}

int
duf_trace_force_color_c( const duf_config_t * cfg )
{
  /* fprintf(stderr, "@@@@@@@@@@@@@@FC:%d\n", cfg ? cfg->opt.act.v.flag.force_color : 0 ); */
  return cfg ? cfg->opt.act.v.flag.force_color : 0;
  /* return DUF_ACTG_FLAG( force_color ); */
}

int
duf_trace_force_color( void )
{
  return duf_trace_force_color_c( duf_config );
}

int
duf_trace_nocolor_c( const duf_config_t * cfg )
{
  return cfg ? cfg->opt.act.v.flag.nocolor : 0;
  /* return DUF_ACTG_FLAG( nocolor ); */
}

int
duf_trace_nocolor( void )
{
  return duf_trace_nocolor_c( duf_config );
}

char
duf_option_delimiter( void )
{
  return duf_config ? duf_config->opt.option_delimiter : ':';
}

static const char *
duf_string_options_at_string_xsdb_getvar( const char *name, const char *arg DUF_UNUSED )
{
  static char buf[256];
  size_t len;
  size_t llen;
  const char *label = "TIME(";
  char *pbuf = buf;

  llen = strlen( label );
  *buf = 0;
  len = strlen( name );
  if ( len > llen && 0 == strncmp( name, "TIME(", llen ) && name[len - 1] == ')' )
  {
    /* strftime */
    char *fmt;

    fmt = mas_strndup( name + llen, len - llen - 1 );
    mas_tstrflocaltime( buf, sizeof( buf ), fmt, time( NULL ) );
    mas_free( fmt );
    pbuf = buf;
  }
  else if ( *name == '+' )
  {
    if ( duf_config && ( 0 == strcmp( name + 1, "db_name" ) || 0 == strcmp( name + 1, "dbname" ) || 0 == strcmp( name + 1, "db-name" ) ) )
      pbuf = DUF_CONFIGGSP( db.main.name );
  }
  /* T( "@@@@@@var %s => '%s'", name, pbuf ); */
  return pbuf;
}

static char *
_duf_string_options_expand( const char *s, char protected_prefix, int *pexpandable_later )
{
  char *xs = NULL;

  DUF_TRACE( explain, 0, "s: \"%s\"", s );
  if ( s )
  {
    if ( protected_prefix && *s == protected_prefix )
    {
      xs = mas_strdup( s );
      if ( pexpandable_later )
        *pexpandable_later = 1; /* expand later */
    }
    else
    {
      xs = mas_expand_string_cb_arg( s, duf_string_options_at_string_xsdb_getvar, NULL );
      {
        char *xs1;

        xs1 = mas_expand_string( xs );
        mas_free( xs );
        xs = xs1;
      }
    }
  }
  DUF_TRACE( explain, 0, "xs: \"%s\"", xs );
  return xs;
}

char *
duf_string_options_expand( const char *s, char protected_prefix )
{
  return _duf_string_options_expand( s, protected_prefix, NULL );
}

char *
duf_config_string_expanded( duf_config_string_t * cs )
{
  char *p = cs->value;
  char *x = NULL;

  if ( p )
  {
    if ( *p == '?' )
      p++;
    x = duf_string_options_expand( p, 0 );
    mas_free( cs->expanded );
    cs->expanded = x;
  }
  return cs->expanded;
}

char *
duf_config_db_path_add_subdir( const char *dir, int *pr )
{
  int rpr = 0;
  char *path = NULL;

  if ( DUF_CONFIGGSP( db.subdir ) )
  {
    if ( strchr( DUF_CONFIGGSP( db.subdir ), '/' ) )
    {
      DUF_MAKE_ERROR( rpr, DUF_ERROR_MKDIR );
    }
    else
    {
      int ry;

      path = mas_concat_path( dir, DUF_CONFIGGSP( db.subdir ) );
      {
        struct stat st;

        ry = stat( path, &st );
        if ( ry < 0 )
        {
          if ( errno == ENOENT )
          {
            ry = mkdir( path, S_IRWXU );

            if ( ry < 0 )
            {
              char serr[1024] = "";
              char *s;

              s = strerror_r( errno, serr, sizeof( serr ) );
              DUF_MAKE_ERRORM( rpr, DUF_ERROR_MKDIR, "(ry:%d) errno:%d mkdir :%s; path:'%s'", ry, errno, s ? s : serr, path );
              /* DUF_SHOW_ERROR( "(ry:%d) errno:%d mkdir :%s; path:'%s'", ry, errno, s ? s : serr, path ); */
            }
          }
        }
        else if ( !S_ISDIR( st.st_mode ) )
        {
          DUF_MAKE_ERROR( rpr, DUF_ERROR_STAT );
        }
      }
    }
  }
  if ( pr )
    *pr = rpr;
  return path;
}
