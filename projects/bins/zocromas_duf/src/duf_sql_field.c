/* #undef MAS_TRACING */
#include <assert.h>                                                  /* assert */
#include <string.h>

#include "duf_tracen_defs_preset.h"                                  /* MAST_TRACE_CONFIG; etc. ✗ */
/* #include "duf_errorn_defs_preset.h"                                  (* MAST_ERRORS_FILE; etc. ✗ *) */

#include <mastar/wrap/mas_std_def.h>
#include <mastar/trace/mas_trace.h>

#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ✗ */

#include "duf_sql_positional.h"                                      /* duf_sql_column_long_long etc. ✗ */
#include "duf_sql_prepared.h"                                        /* duf_sql_(prepare|step|finalize) ✗ */

/* ###################################################################### */
#include "duf_sql_field.h"                                           /* __duf_sql_str_by_name2 for DUF_GET_UFIELD2 etc. ✗ */
/* ###################################################################### */
void
__duf_sql_dump_row( duf_stmnt_t * psqlstmt )
{
  for ( int icol = 0; icol < duf_sql_column_count( psqlstmt ); icol++ )
  {
    const char *t;
    const char *n MAS_UNUSED;
    const char *v;

    t = duf_sql_column_decltype( psqlstmt, icol );
    n = duf_sql_column_name( psqlstmt, icol );
    v = duf_sql_column_string( psqlstmt, icol );
    if ( v )
    {
      if ( t && 0 == strcasecmp( t, "integer" ) )
      {
        QT( "@@@@%2d[%8s]:\t %11s = \t %lld", icol, t ? t : "-", n, duf_sql_column_long_long( psqlstmt, icol ) );
      }
      else if ( t && 0 == strcasecmp( t, "text" ) )
      {
        QT( "%2d[%8s]:\t %11s = \t\"%s\"", icol, t ? t : "-", n, v );
      }
      else
      {
        QT( "%2d[%8s]:\t %11s = \t'%s'", icol, t ? t : "-", n, v );
      }
    }
    else
    {
      QT( "@@@%2d[%8s]:\t %11s = \t NULL", icol, t ? t : "-", n );
    }
  }
}

int
__duf_sql_pos_by_name2( duf_stmnt_t * psqlstmt, const char *name )
{
  int pos = -1;

  for ( int icol = 0; icol < duf_sql_column_count( psqlstmt ); icol++ )
  {
  /* QT( "@field %s ? %s (%d)", name, duf_sql_column_name( psqlstmt, icol ), pos ); */
    if ( 0 == strcmp( duf_sql_column_name( psqlstmt, icol ), name ) )
    {
      pos = icol;
    /* QT( "@field %s  found (%d)", name, pos ); */
      break;
    }
  }
/* QT( "@field %s  pos:%d", name, pos ); */
  if ( pos < 0 )
  {
    QT( "@field %s not found:", name );
    for ( int icol = 0; icol < duf_sql_column_count( psqlstmt ); icol++ )
    {
      QT( "@@@- %s", duf_sql_column_name( psqlstmt, icol ) );
    }
  }
  return pos;
}

const char *
__duf_sql_str_by_name2( duf_stmnt_t * psqlstmt, const char *name, int opt )
{
  const char *ptr = NULL;
  int pos = __duf_sql_pos_by_name2( psqlstmt, name );

  assert( opt || pos >= 0 );

  if ( pos >= 0 )
    ptr = duf_sql_column_string( psqlstmt, pos );

  return ptr;
}

unsigned long long
__duf_sql_ull_by_name2( duf_stmnt_t * psqlstmt, const char *name, int opt )
{
  unsigned long long val = 0;
  int pos = __duf_sql_pos_by_name2( psqlstmt, name );

  assert( opt || pos >= 0 );

  if ( pos >= 0 )
    val = duf_sql_column_long_long( psqlstmt, pos );

  return val;
}
