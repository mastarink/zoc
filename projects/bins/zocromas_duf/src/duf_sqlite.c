#include <assert.h>
#include <string.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_types.h"

#include "duf_utils.h"
#include "duf_service.h"

#include "duf_dbg.h"

#include "duf_config.h"


/* ###################################################################### */
#include "duf_sqlite.h"
/* ###################################################################### */


/* int duf_constraint = SQLITE_CONSTRAINT; */
static sqlite3 *pDb = NULL;

int
duf_sqlite_error_code( int r3 )
{
  int rt;

  rt = (r3 == SQLITE_OK) ? 0 : ( r3 > 0 ? DUF_SQLITE_ERROR_BASE + r3 : r3 );
  return rt;
}

int
duf_sqlite_open( const char *dbpath )
{
  int r3 = 0;

/*										*/ DEBUG_START(  );
  if ( !pDb )
  {
    r3 = sqlite3_initialize(  );
    DUF_TRACE( action, 0, "DB Initialize %s (%d)", r3 == SQLITE_OK ? "OK" : "FAIL", r3 );
    if ( r3 == SQLITE_OK )
    {
      r3 = sqlite3_open( dbpath, &pDb );
      DUF_TRACE( action, 0, "DB Open %s %s (%d)", dbpath, r3 == SQLITE_OK ? "OK" : "FAIL", r3 );
    }
  }
  /*          */ DEBUG_END(  );
  return ( r3 );
}

int
duf_sqlite_close( void )
{
  int r3 = 0;

/*										*/ DEBUG_START(  );
  if ( pDb )
    r3 = sqlite3_close( pDb );
  DUF_TRACE( action, 0, "DB Close %s (%d)", r3 == SQLITE_OK ? "OK" : "FAIL", r3 );

  r3 = sqlite3_shutdown(  );
  pDb = NULL;
  DUF_TRACE( action, 0, "DB Shutdown %s (%d)", r3 == SQLITE_OK ? "OK" : "FAIL", r3 );

/*										*/ DEBUG_END(  );
  return ( r3 );
}

int
duf_sqlite_execcb( const char *sql, duf_sqexe_cb_t sqexe_cb, void *sqexe_data, int *pchanges, char **pemsg )
{
  int r3 = SQLITE_OK;
  char *emsg = ( char * ) NULL;

  if ( pemsg )
    *pemsg = NULL;
/*										*/ DEBUG_START(  );
  DUF_TRACE( sql, 2, "[%s] ", sql );
  if ( pchanges )
    *pchanges = 0;
  if ( pDb )
  {
    if ( duf_config->cli.dbg.nosqlite )
    {
      DUF_TRACE( current, 0, "SKIP %s", sql );
    }
    else
    {
      r3 = sqlite3_exec( pDb, sql, sqexe_cb, sqexe_data, &emsg );
    }
    if ( r3 == SQLITE_OK && pchanges )
      *pchanges = sqlite3_changes( pDb );
    DUF_TRACE( sql, 0, "  [%s]", sql );
    DUF_TRACE( sql, 1, "r3:%d; changes:%d", r3, pchanges ? *pchanges : -1 );
/*										*/ DEBUG_STEP(  );
  }
  if ( pemsg )
    *pemsg = emsg;
  else if ( emsg )
    sqlite3_free( emsg );
/*										*/ DEBUG_ENDR3( r3 );
  DUF_TRACE( sql, 3, "[%s] : %d", sql, r3 );
  return r3;
}

int
duf_sqlite_execcb_e( const char *sql, duf_sqexe_cb_t sqexe_cb, void *sqexe_data, int *pchanges )
{
  int r3;
  char *emsg = NULL;

  r3 = duf_sqlite_execcb( sql, sqexe_cb, sqexe_data, pchanges, &emsg );
  if ( r3 != SQLITE_OK )
    DUF_ERROR( "(%d) SQL '%s' in [%s]", r3, emsg ? emsg : "-", sql ? sql : "?" );
  if ( emsg )
    sqlite3_free( emsg );
  emsg = NULL;

  return r3;
}

int
duf_sqlite_exec( const char *sql, int *pchanges, char **pemsg )
{
  return duf_sqlite_execcb( sql, ( duf_sqexe_cb_t ) NULL, ( void * ) NULL, pchanges, pemsg );
}

int
duf_sqlite_exec_c( const char *sql, int constraint_ignore, int *pchanges )
{
  int r3;
  char *emsg = NULL;

  r3 = duf_sqlite_exec( sql, pchanges, &emsg );
  if ( r3 != SQLITE_OK && !( r3 == SQLITE_CONSTRAINT && constraint_ignore ) )
    DUF_ERROR( "(%d) SQL '%s' in [%s]", r3, emsg ? emsg : "-", sql ? sql : "?" );

  if ( emsg )
    sqlite3_free( emsg );
  emsg = NULL;
  return r3;
}

int
duf_sqlite_exec_e( const char *sql, int *pchanges )
{
  int r3;
  char *emsg = NULL;

  r3 = duf_sqlite_exec( sql, pchanges, &emsg );
  if ( r3 != SQLITE_OK )
    DUF_ERROR( "(%d) SQL '%s' in [%s]", r3, emsg ? emsg : "-", sql ? sql : "?" );
  if ( emsg )
    sqlite3_free( emsg );
  emsg = NULL;

  return r3;
}


int
duf_vsqlite_c( const char *fmt, int constraint_ignore, int *pchanges, va_list args )
{
  int r3 = 0;
  char *sql;

  DEBUG_START(  );

  DUF_TRACE( sql, 2, "[%s] ", fmt );
  sql = sqlite3_vmprintf( fmt, args );
  DUF_TRACE( sql, 2, "[%s] ", sql );
  r3 = duf_sqlite_exec_c( sql, constraint_ignore, pchanges );

  DUF_TRACE( sql, 3, "[%s] : %d", sql, r3 );
  sqlite3_free( sql );
  sql = NULL;
  DUF_TRACE( sql, 3, "r3: %d", r3 );

  DEBUG_ENDR3( r3 );
  return ( r3 );
}

int
duf_vsqlite_e( const char *fmt, int *pchanges, va_list args )
{
  int r3 = 0;
  char *sql;

  DEBUG_START(  );

  DUF_TRACE( sql, 2, "[%s] ", fmt );
  sql = sqlite3_vmprintf( fmt, args );
  DUF_TRACE( sql, 2, "[%s] ", sql );
  r3 = duf_sqlite_exec_e( sql, pchanges );

  DUF_TRACE( sql, 3, "[%s] : %d", sql, r3 );
  sqlite3_free( sql );
  sql = NULL;
  DUF_TRACE( sql, 3, "r3: %d", r3 );

  DEBUG_ENDR3( r3 );
  return ( r3 );
}


int
duf_vsqlite( const char *fmt, int *pchanges, va_list args )
{
  int r3 = 0;
  char *sql;
  char *emsg = NULL;

  DEBUG_START(  );

  DUF_TRACE( sql, 2, "[%s] ", fmt );
  sql = sqlite3_vmprintf( fmt, args );
  DUF_TRACE( sql, 2, "[%s] ", sql );
  r3 = duf_sqlite_exec( sql, pchanges, &emsg );
  if ( r3 != SQLITE_OK )
    DUF_ERROR( "(%d) SQL '%s' in [%s]", r3, emsg ? emsg : "-", sql ? sql : "?" );

  DUF_TRACE( sql, 3, "(%d) SQL '%s' [%s]", r3, emsg ? emsg : "-", sql ? sql : "?" );
  if ( emsg )
    sqlite3_free( emsg );

  sqlite3_free( sql );
  sql = NULL;
  DUF_TRACE( sql, 3, "r3: %d", r3 );

  DEBUG_ENDR3( r3 );
  return ( r3 );
}




/*
 *  sel_cb -- callback called for each row with:
 *    sel_cb_udata	     passed
 *    str_cb + str_cb_udata  passed to be called there
 *  fmt + args - sql
 * */
int
duf_sqlite_vselect( duf_sel_cb_t sel_cb, void *sel_cb_udata, duf_str_cb_t str_cb, void *str_cb_udata,
                    duf_depthinfo_t * pdi, duf_scan_callbacks_t * sccb /*, const duf_dirhandle_t * pdhu_off */ , const char *fmt,
                    va_list args )
{
  int r3 = 0;
  int row, column;
  char *sql, **presult = NULL;

  const char *const *pcresult = NULL;

  va_list qargs;

  DEBUG_START(  );
  va_copy( qargs, args );
  if ( pDb )
  {
    char *emsg = NULL;

    DUF_TRACE( sql, 2, "in for %s", fmt );

    sql = sqlite3_vmprintf( fmt, args );
    if ( !sql )
    {
      DUF_ERROR( "what happend to sql? [%s] => [%s]", fmt, sql );
    }
    r3 = sqlite3_get_table( pDb, sql, &presult, &row, &column, &emsg );
    assert( r3 != SQLITE_CORRUPT );

    DUF_TRACE( sql, 0, "  [%s]", sql );
    DUF_TRACE( sql, 1, "r3=%d;  %u rows", r3, row );
    DEBUG_STEPS( sql );
    /* if ( trace )                                            */
    /*   printf(  "(%d) trace:[%s]\x1b[K\n", r3, sql ); */
    pcresult = ( const char *const * ) presult;
    if ( r3 == SQLITE_OK )
    {
      DEBUG_STEPIS( row, "rows SQLITE_OK" );
      if ( row )
      {
        int rcb = 0;

        for ( int ir = 1; ir <= row && rcb == 0; ir++ )
        {
          va_list cargs;
          duf_record_t rrecord;

          va_copy( cargs, qargs );
/* 
 * sel_cb is of duf_sel_cb_t:
 * */

#ifdef DUF_RECORD_WITH_NROWS
          rrecord.nrow = ir - 1;
          rrecord.nrows = row;
#endif
          rrecord.ncolumns = column;
          rrecord.pnames = &pcresult[0 * column];
          rrecord.presult = &pcresult[ir * column];
          {

            rcb = ( sel_cb ) ( &rrecord, sel_cb_udata, str_cb, str_cb_udata, pdi, sccb );

            DUF_TEST_R( rcb );
            DUF_TRACE( sql, 2, "row #%u; <sel_cb(%p) = %d", ir, ( void * ) ( unsigned long long ) sel_cb, rcb );
          }
        }
        if ( rcb )
        {
          if ( rcb == DUF_ERROR_MAX_REACHED )
          {
            /* DUF_TRACE( action, 0, "Maximum reached" ); */
          }
          else
          {
            /* DUF_ERROR( "rcb:%d", rcb ); */
          }
        }
        DUF_TEST_R( rcb );
        DUF_TEST_R3( r3 );
        DUF_TRACE( sql, 4, "rcb:%d; r3:%d sel_cb:%s; str_cb:%s", rcb, r3, DUF_FUNN( sel_cb ), DUF_FUNN( str_cb ) );
        r3 = rcb ? rcb : r3;
        DUF_TEST_R3( r3 );
      }
      else
      {
        DUF_TRACE( sql, 0, "Nothing by [%s]", sql );
      }
    }
    else if ( r3 == SQLITE_CONSTRAINT )
    {
      DUF_TEST_R3( r3 );
      DUF_ERROR( "SQL : %s [%s]", emsg ? emsg : "no error", sql );
    }
    else
    {
      DUF_TEST_R3( r3 );
      DUF_ERROR( "SQL : %s [%s]", emsg ? emsg : "no error", sql );
    }
    if ( emsg )
      sqlite3_free( emsg );
    emsg = NULL;
    sqlite3_free_table( presult );
    presult = NULL;
    sqlite3_free( sql );
    sql = NULL;
  }
  DEBUG_ENDR3( r3 );
  return ( r3 );
}

unsigned long long
duf_sqlite_last_insert_rowid( void )
{
  unsigned long long li = 0;

  DEBUG_START(  );
  if ( pDb )
  {
    li = ( unsigned long long ) sqlite3_last_insert_rowid( pDb );
  }
  DEBUG_ENDULL( li );
  return ( li );
}

int
duf_sqlite_prepare( const char *sql, duf_sqlite_stmt_t ** pstmt )
{
  int r3 = 0;
  const char *tail = NULL;

  /* DUF_PRINTF( 0, "PREPARE" ); */
  DUF_TRACE( sql, 0, "  [%s]", sql );
  r3 = sqlite3_prepare_v2( pDb, sql, strlen( sql ), pstmt, &tail );
  DUF_TEST_R3( r3 );
  return r3;
}

int
duf_sqlite_step( duf_sqlite_stmt_t * stmt )
{
  int r3;

  /* DUF_PRINTF( 0, "STEP" ); */
  r3 = sqlite3_step( stmt );
  assert( r3 != SQLITE_MISUSE );
  DUF_TEST_R3( r3 );
  return r3;
}

int
duf_sqlite_finalize( duf_sqlite_stmt_t * stmt )
{
  int r3;

  /* DUF_PRINTF( 0, "FINALIZE" ); */
  r3 = sqlite3_finalize( stmt );
  DUF_TEST_R3( r3 );
  return r3;
}

int
duf_sqlite_bind_parameter_index( duf_sqlite_stmt_t * stmt, const char *name )
{
  int r3 = 0;

  r3 = sqlite3_bind_parameter_index( stmt, name );
  return r3;
}



int
duf_sqlite_bind_long_long( duf_sqlite_stmt_t * stmt, int num, long long val )
{
  int r3 = 0;
  sqlite3_int64 val64;

  /* DUF_PRINTF( 0, "BIND" ); */
  val64 = val;
  r3 = sqlite3_bind_int64( stmt, num, val64 );
  /* DUF_ERROR( "val:%lld", val64 ); */
  DUF_TEST_R3( r3 );
  return r3;
}

int
duf_sqlite_bind_int( duf_sqlite_stmt_t * stmt, int num, int val )
{
  int r3 = 0;

  r3 = sqlite3_bind_int( stmt, num, val );
  DUF_TEST_R3( r3 );
  return r3;
}

int
duf_sqlite_bind_null( duf_sqlite_stmt_t * stmt, int num )
{
  int r3 = 0;

  r3 = sqlite3_bind_null( stmt, num );
  DUF_TEST_R3( r3 );
  return r3;
}

int
duf_sqlite_bind_double( duf_sqlite_stmt_t * stmt, int num, double val )
{
  int r3 = 0;

  r3 = sqlite3_bind_double( stmt, num, val );
  DUF_TEST_R3( r3 );
  return r3;
}

int
duf_sqlite_bind_string( duf_sqlite_stmt_t * stmt, int num, const char *val )
{
  int r3 = 0;

  r3 = sqlite3_bind_text( stmt, num, val, strlen( val ), SQLITE_TRANSIENT );
  DUF_TEST_R3( r3 );
  return r3;
}

int
duf_sqlite_changes( void )
{
  int changes = 0;

  changes = sqlite3_changes( pDb );
  return changes;
}

long long
duf_sqlite_column_long_long( duf_sqlite_stmt_t * stmt, int icol )
{
  return sqlite3_column_int64( stmt, icol );
}

int
duf_sqlite_column_int( duf_sqlite_stmt_t * stmt, int icol )
{
  return sqlite3_column_int( stmt, icol );
}

const char *
duf_sqlite_column_string( duf_sqlite_stmt_t * stmt, int icol )
{
  return ( const char * ) sqlite3_column_text( stmt, icol );
}

const char *
duf_sqlite_column_name( duf_sqlite_stmt_t * stmt, int index )
{
  const char *str;

  str = sqlite3_column_name( stmt, index );
  return str;
}

int
duf_sqlite_column_count( duf_sqlite_stmt_t * stmt )
{
  int cnt;

  cnt = sqlite3_column_count( stmt );
  return cnt;
}
