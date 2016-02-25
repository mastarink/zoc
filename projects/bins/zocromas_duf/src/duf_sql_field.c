/* #undef MAS_TRACING */
#include <assert.h>                                                  /* assert */
#include <string.h>

#include <mastar/wrap/mas_std_def.h>

#include "duf_tracen_defs.h"                                         /* DUF_TRACE ♠ */
#include "duf_errorn_defs.h"                                         /* DUF_NOERROR; DUF_CLEAR_ERROR; DUF_E_(LOWER|UPPER); DUF_TEST_R ... ♠ */

#include "duf_start_end.h"                                           /* DUF_STARTR ; DUF_ENDR ♠ */
#include "duf_dodefs.h"                                              /* DOR ♠ */

/* #include "duf_config.h" */
#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ♠ */

#include "duf_sql_positional.h"
#include "duf_sql_prepared.h"

/* ###################################################################### */
#include "duf_sql_field.h"                                           /* __duf_sql_str_by_name2 for DUF_GET_UFIELD2 etc. ♠ */
/* ###################################################################### */
void
__duf_sql_dump_row( duf_stmnt_t * pstmt )
{
  for ( int icol = 0; icol < duf_sql_column_count( pstmt ); icol++ )
  {
    const char *t;
    const char *n MAS_UNUSED;
    const char *v;

    t = duf_sql_column_decltype( pstmt, icol );
    n = duf_sql_column_name( pstmt, icol );
    v = duf_sql_column_string( pstmt, icol );
    if ( v )
    {
      if ( t && 0 == strcasecmp( t, "integer" ) )
      {
        T( "@@@@%2d[%8s]:\t %11s = \t %lld", icol, t ? t : "-", n, duf_sql_column_long_long( pstmt, icol ) );
      }
      else if ( t && 0 == strcasecmp( t, "text" ) )
      {
        T( "%2d[%8s]:\t %11s = \t\"%s\"", icol, t ? t : "-", n, v );
      }
      else
      {
        T( "%2d[%8s]:\t %11s = \t'%s'", icol, t ? t : "-", n, v );
      }
    }
    else
    {
      T( "@@@%2d[%8s]:\t %11s = \t NULL", icol, t ? t : "-", n );
    }
  }
}

int
__duf_sql_pos_by_name2( duf_stmnt_t * pstmt, const char *name )
{
  int pos = -1;

  for ( int icol = 0; icol < duf_sql_column_count( pstmt ); icol++ )
  {
  /* T( "@field %s ? %s (%d)", name, duf_sql_column_name( pstmt, icol ), pos ); */
    if ( 0 == strcmp( duf_sql_column_name( pstmt, icol ), name ) )
    {
      pos = icol;
    /* T( "@field %s  found (%d)", name, pos ); */
      break;
    }
  }
/* T( "@field %s  pos:%d", name, pos ); */
  if ( pos < 0 )
  {
    T( "@field %s not found:", name );
    for ( int icol = 0; icol < duf_sql_column_count( pstmt ); icol++ )
    {
      T( "@@@- %s", duf_sql_column_name( pstmt, icol ) );
    }
    assert( 0 );
  }
  return pos;
}

const char *
__duf_sql_str_by_name2( duf_stmnt_t * pstmt, const char *name, int opt )
{
  const char *ptr = NULL;
  int pos = __duf_sql_pos_by_name2( pstmt, name );

  assert( opt || pos >= 0 );

  if ( pos >= 0 )
    ptr = duf_sql_column_string( pstmt, pos );

  return ptr;
}

unsigned long long
__duf_sql_ull_by_name2( duf_stmnt_t * pstmt, const char *name, int opt )
{
  unsigned long long val = 0;
  int pos = __duf_sql_pos_by_name2( pstmt, name );

  assert( opt || pos >= 0 );

  if ( pos >= 0 )
    val = duf_sql_column_long_long( pstmt, pos );

  return val;
}
