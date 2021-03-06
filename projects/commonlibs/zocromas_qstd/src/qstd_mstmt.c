#define R_GOOD(_r) ((_r)>=0)
/* #define RGEMSG mysql_error(mas_qstd_mysql(qstd)) */
#define RGEMSG mas_mysqlpfs_mstmt_error(mstmt)
#include "qstd_defs.h"
#include <string.h>

/* #include <mastar/wrap/mas_memory.h> */
#include <mastar/minierr/minierr.h>
#include <mastar/regerr/masregerr.h>
#include <mastar/mysqlpfs/mysqlpfs_defs.h>

/* #include <mastar/mysqlpfs/mysqlpfs_base.h> */
#include <mastar/mysqlpfs/mysqlpfs.h>
#include <mastar/mysqlpfs/mysqlpfs_query.h>
/* #include <mastar/mysqlpfs/mysqlpfs_mstmt_base.h> */
#include <mastar/mysqlpfs/mysqlpfs_mstmt.h>

#include "qstd_structs.h"

#include "qstd_mstmt.h"

int
mas_qstd_mstmt_prepare_param_longlong( mysqlpfs_mstmt_t * mstmt, int pos )
{
  rDECLBAD;
  rC( mas_mysqlpfs_mstmt_prepare_param_longlong( mstmt, pos ) );
  if ( !rGOOD )
    WARN( "%s", mas_mysqlpfs_mstmt_error( mstmt ) );

  rRET;
}

int
mas_qstd_mstmt_prepare_result_longlong( mysqlpfs_mstmt_t * mstmt, int pos )
{
  rDECLBAD;
  rC( mas_mysqlpfs_mstmt_prepare_result_longlong( mstmt, pos ) );
  if ( !rGOOD )
    WARN( "%s", mas_mysqlpfs_mstmt_error( mstmt ) );

  rRET;
}

int
mas_qstd_mstmt_prepare_param_string( mysqlpfs_mstmt_t * mstmt, int pos )
{
  rDECLBAD;
  rC( mas_mysqlpfs_mstmt_prepare_param_string( mstmt, pos, ( mysqlpfs_s_length_t ) 255 ) );
  if ( !rGOOD )
    WARN( "%s", mas_mysqlpfs_mstmt_error( mstmt ) );

  rRET;
}

int
mas_qstd_mstmt_prepare_result_string( mysqlpfs_mstmt_t * mstmt, int pos )
{
  rDECLBAD;
  rC( mas_mysqlpfs_mstmt_prepare_result_string( mstmt, pos, ( mysqlpfs_s_length_t ) 255 ) );
  if ( !rGOOD )
    WARN( "%s", mas_mysqlpfs_mstmt_error( mstmt ) );
  rRET;
}

int
mas_qstd_mstmt_set_param_longlong( mysqlpfs_mstmt_t * mstmt, int pos, unsigned long long num, unsigned is_null )
{
  rDECLBAD;
  rC( mas_mysqlpfs_mstmt_set_param_longlong( mstmt, pos, num, is_null ) );

  rRET;
}

int
mas_qstd_mstmt_set_param_string( mysqlpfs_mstmt_t * mstmt, int pos, const char *string )
{
  rDECLBAD;
  rC( mas_mysqlpfs_mstmt_set_param_string( mstmt, pos, string ) );

  rRET;
}

int
mas_qstd_mstmt_set_param_binary( mysqlpfs_mstmt_t * mstmt, int pos, const unsigned char *binary, size_t length )
{
  rDECLBAD;
  rC( mas_mysqlpfs_mstmt_set_param_binary( mstmt, pos, binary, length ) );

  rRET;
}

int
mas_qstd_mstmt_data_seek( mysqlpfs_mstmt_t * mstmt, unsigned long long offset )
{
  rDECLBAD;
  rC( mas_mysqlpfs_mstmt_data_seek( mstmt, offset ) );

  rRET;
}

int
mas_qstd_mstmt_execute_store( mysqlpfs_mstmt_t * mstmt )
{
  rDECLBAD;
  rC( mas_mysqlpfs_mstmt_execute_store( mstmt ) );
  rRET;
}

int
mas_qstd_mstmt_execute( mysqlpfs_mstmt_t * mstmt )
{
  rDECLBAD;
  rC( mas_mysqlpfs_mstmt_execute( mstmt ) );
  rRET;
}

int
mas_qstd_mstmt_fetch( mysqlpfs_mstmt_t * mstmt, int *phas_data )
{
  rDECLBAD;
  int truncated = 0;

  rC( mas_mysqlpfs_mstmt_fetch_t( mstmt, phas_data, &truncated ) );
  if ( truncated )
  {
//  MYSQL_BIND : mstmt->binds.param.bind[np].error
    /* WARN( "MYSQL_DATA_TRUNCATED" ); */
  }
  else
  {
    /* WARN( "not MYSQL_DATA_TRUNCATED" ); */
  }
  rRET;
}

int
mas_qstd_mstmt_fetch_t( mysqlpfs_mstmt_t * mstmt, int *phas_data, int *truncated )
{
  rDECLBAD;
  rC( mas_mysqlpfs_mstmt_fetch_t( mstmt, phas_data, truncated ) );
  rRET;
}

int
mas_qstd_mstmt_get_result_longlong( mysqlpfs_mstmt_t * mstmt, int pos, unsigned long long *pnum, unsigned *pis_null )
{
  return mas_mysqlpfs_mstmt_get_result_longlong( mstmt, pos, pnum, pis_null );
}

int
mas_qstd_mstmt_get_result_string_na( mysqlpfs_mstmt_t * mstmt, int pos, const char **pstring )
{
  return mas_mysqlpfs_mstmt_get_result_string_na( mstmt, pos, pstring );
}

int
mas_qstd_mstmt_bind_param( mysqlpfs_mstmt_t * mstmt )
{
  rDECLBAD;
  rC( mas_mysqlpfs_mstmt_bind_param( mstmt ) );
  if ( !rGOOD )
    WARN( "%s", mas_mysqlpfs_mstmt_error( mstmt ) );
  rRET;
}

int
mas_qstd_mstmt_bind_result( mysqlpfs_mstmt_t * mstmt )
{
  rDECLBAD;
  rC( mas_mysqlpfs_mstmt_bind_result( mstmt ) );
  if ( !rGOOD )
    WARN( "%s", mas_mysqlpfs_mstmt_error( mstmt ) );
  rRET;
}

int
mas_qstd_mstmt_free_result( mysqlpfs_mstmt_t * mstmt )
{
  return mas_mysqlpfs_mstmt_free_result( mstmt );
}

int
mas_qstd_mstmt_insert_id( mysqlpfs_mstmt_t * mstmt )
{
  rDECLBAD;
  rC( mas_mysqlpfs_mstmt_insert_id( mstmt ) );
  rRET;
}

unsigned long long
mas_qstd_mstmt_affected_rows( mysqlpfs_mstmt_t * mstmt )
{
  return mas_mysqlpfs_mstmt_affected_rows( mstmt );
}

const char *
mas_qstd_mstmt_error( mysqlpfs_mstmt_t * mstmt )
{
  return mas_mysqlpfs_mstmt_error( mstmt );
}

int
mas_qstd_mstmt_ret_code( mysqlpfs_mstmt_t * mstmt )
{
  rDECLBAD;
  rC( mas_mysqlpfs_mstmt_ret_code( mstmt ) );
  rRET;
}
