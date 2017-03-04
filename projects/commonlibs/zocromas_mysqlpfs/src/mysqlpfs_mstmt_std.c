#define RGEMSG mysql_error(&pfs->mysql)
#include "mysqlpfs_defs.h"
#include <string.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/minierr/minierr.h>
#include <mastar/regerr/masregerr.h>

#include "qstd_structs.h"

#include "mysqlpfs_base.h"
#include "mysqlpfs_mstmt_base.h"
#include "mysqlpfs_mstmt.h"

#include "mysqlpfs_mstmt_std.h"

void
mas_qstd_init( mas_qstd_t * qstd, const char *host, const char *user, const char *passwd, const char *db, int port )
{
  if ( qstd )
  {
    qstd->pfs = mysqlpfs_create_setup( host, user, passwd, db, port );
    qstd->std_mstmts = mas_qstd_mstmt_create_array(  );
  }
}

mas_qstd_t *
mas_qstd_create( void )
{
  return mas_calloc( 1, sizeof( mas_qstd_t ) );
}

mas_qstd_t *
mas_qstd_create_setup( const char *host, const char *user, const char *passwd, const char *db, int port )
{
  mas_qstd_t *qstd = mas_qstd_create(  );

  mas_qstd_init( qstd, host, user, passwd, db, port );
  return qstd;
}

void
mas_qstd_reset( mas_qstd_t * qstd )
{
  if ( qstd )
  {
    mas_qstd_mstmt_delete_array( qstd->std_mstmts );
    mysqlpfs_delete( qstd->pfs );
  }
}

void
mas_qstd_delete( mas_qstd_t * qstd )
{
  mas_qstd_reset( qstd );
  if ( qstd )
    mas_free( qstd );
}

/**********************************************************************************/

mysqlpfs_mstmt_t **
mas_qstd_mstmt_create_array( void )
{
  return mas_calloc( STD_MSTMT_MAX, sizeof( mysqlpfs_mstmt_t ** ) );
}

void
mas_qstd_mstmt_reset_array( mysqlpfs_mstmt_t ** mstmts )
{
  if ( mstmts )
    for ( int nst = 0; nst < STD_MSTMT_MAX; nst++ )
    {
      if ( mstmts[nst] )
        mas_mysqlpfs_mstmt_delete( mstmts[nst] );
      mstmts[nst] = NULL;
    }
}

void
mas_qstd_mstmt_delete_array( mysqlpfs_mstmt_t ** mstmts )
{
  mas_qstd_mstmt_reset_array( mstmts );
  mas_free( mstmts );
}

mysqlpfs_mstmt_t *
mas_qstd_mstmt_init( mas_qstd_t * qstd, mysqlpfs_std_id_t stdid )
{
  rSET( 0 );
  mysqlpfs_mstmt_t *mstmt = NULL;

  if ( qstd && stdid >= 0 && stdid < STD_MSTMT_MAX )
  {
    mysqlpfs_mstmt_t **mstmts = qstd->std_mstmts;

    mstmt = mstmts[stdid];
    if ( mstmt )
    {
      mas_mysqlpfs_mstmt_delete( mstmt );
      mstmt = mstmts[stdid] = NULL;
    }
    switch ( stdid )
    {
    case STD_MSTMT_INSERT_NAMES:
      {
        char *insop = "INSERT INTO filenames(name,parent_id,detype) VALUES (?,?,?)";

        mysqlpfs_t *pfs = qstd->pfs;

        mstmt = mas_mysqlpfs_mstmt_create_setup( pfs, 3, 0, insop );

        rC( mas_mysqlpfs_mstmt_prepare_param_string( mstmt, 0, 255 ) );
        rC( mas_mysqlpfs_mstmt_prepare_param_longlong( mstmt, 1 ) );
        rC( mas_mysqlpfs_mstmt_prepare_param_string( mstmt, 2, 255 ) );
        rC( mas_mysqlpfs_mstmt_bind_param( mstmt ) );
      }
      break;
    case STD_MSTMT_SELECT_NAMES_ID:
      {
        char *selop = "SELECT id FROM filenames WHERE name=? AND parent_id<=>?";

        mysqlpfs_t *pfs = qstd->pfs;

        mstmt = mas_mysqlpfs_mstmt_create_setup( pfs, 2, 1, selop );

        rC( mas_mysqlpfs_mstmt_prepare_param_string( mstmt, 0, 255 ) );
        rC( mas_mysqlpfs_mstmt_prepare_param_longlong( mstmt, 1 ) );
        rC( mas_mysqlpfs_mstmt_bind_param( mstmt ) );
        rC( mas_mysqlpfs_mstmt_prepare_result_longlong( mstmt, 0 ) );
        rC( mas_mysqlpfs_mstmt_bind_result( mstmt ) );
      }
    case STD_MSTMT_MAX:
      break;
    }
    mstmts[stdid] = mstmt;
  }
  return mstmt;
}

mysqlpfs_mstmt_t *
mas_qstd_mstmt_get( mas_qstd_t * qstd, mysqlpfs_std_id_t stdid )
{
  mysqlpfs_mstmt_t *mstmt = NULL;

  if ( qstd && stdid >= 0 && stdid < STD_MSTMT_MAX )
  {
  /* mysqlpfs_mstmt_t **mstmts = pfs->std_mstmts; */
    mysqlpfs_mstmt_t **mstmts = qstd->std_mstmts;

    mstmt = mstmts[stdid];
    if ( !mstmt )
    {
      mstmt = mas_qstd_mstmt_init( qstd, stdid );
      QRGP( mstmt );
    }
    QRGP( mstmt );
  }
  else
  {
    WARN( "qstd:%p; stdid:%d; STD_MSTMT_MAX:%d;", qstd, stdid, STD_MSTMT_MAX );
    QRG( -1 );
  }

  return mstmt;
}

mysqlpfs_s_ulonglong_t
mas_qstd_mstmt_selget_names_id( mas_qstd_t * qstd, const char *name, mysqlpfs_s_ulonglong_t parent_id )
{
  rSET( 0 );
  mysqlpfs_s_ulonglong_t num = 0;

  QRGP( name );
  {
    mysqlpfs_mstmt_t *mstmt_s = mas_qstd_mstmt_get( qstd, STD_MSTMT_SELECT_NAMES_ID );

    QRGP( mstmt_s );

    rC( mas_mysqlpfs_mstmt_set_param_string( mstmt_s, 0, name ) );
    rC( mas_mysqlpfs_mstmt_set_param_longlong( mstmt_s, 1, parent_id, parent_id ? FALSE : TRUE ) );
    rC( mas_mysqlpfs_mstmt_execute_store( mstmt_s ) );

    rC( mas_mysqlpfs_mstmt_fetch( mstmt_s ) );

    if ( r != MYSQL_NO_DATA )
    {
      mysqlpfs_s_bool_t is_null = 0;

      rC( mas_mysqlpfs_mstmt_get_result_longlong( mstmt_s, 0, &num, &is_null ) );
    }

    mas_mysqlpfs_mstmt_free_result( mstmt_s );
  }
  return num;
}

mysqlpfs_s_ulonglong_t
mas_qstd_mstmt_insget_names_id( mas_qstd_t * qstd, const char *name, mysqlpfs_s_ulonglong_t parent_id, const char *sdetype )
{
  rSET( 0 );
  QRGP( qstd );
  QRGP( name );
  mysqlpfs_s_ulonglong_t theid = 0;

  {
    mysqlpfs_mstmt_t *mstmt = mas_qstd_mstmt_get( qstd, STD_MSTMT_INSERT_NAMES );

    QRGP( mstmt );
    rC( mas_mysqlpfs_mstmt_set_param_string( mstmt, 0, name ) );
    rC( mas_mysqlpfs_mstmt_set_param_longlong( mstmt, 1, parent_id, parent_id ? FALSE : TRUE ) );
    rC( mas_mysqlpfs_mstmt_set_param_string( mstmt, 2, sdetype ) );

    rC( mas_mysqlpfs_mstmt_execute( mstmt ) );
    if ( !r && mas_mysqlpfs_mstmt_affected_rows( mstmt ) == 1 )
      theid = mas_mysqlpfs_mstmt_insert_id( mstmt );
  }
  QRG( theid > 0 ? 0 : -1 );
  return theid;
}

mysqlpfs_s_ulonglong_t
mas_qstd_mstmt_selinsget_names_id( mas_qstd_t * qstd, const char *name, mysqlpfs_s_ulonglong_t parent_id, const char *sdetype )
{
/* rSET( 0 ); */
  QRGP( qstd );
  QRGP( name );

  mysqlpfs_s_ulonglong_t theid = mas_qstd_mstmt_selget_names_id( qstd, name, parent_id );

/* WARN( "SELINS %lld", theid ); */
  if ( !theid )
    theid = mas_qstd_mstmt_insget_names_id( qstd, name, parent_id, sdetype );

  QRG( theid > 0 ? 0 : -1 );
  return theid;
}

mysqlpfs_s_ulonglong_t
mas_qstd_mstmt_insselget_names_id( mas_qstd_t * qstd, const char *name, mysqlpfs_s_ulonglong_t parent_id, const char *sdetype )
{
/* rSET( 0 ); */
  QRGP( qstd );
  QRGP( name );
  mysqlpfs_s_ulonglong_t theid = mas_qstd_mstmt_insget_names_id( qstd, name, parent_id, sdetype );

  if ( !theid )
    theid = mas_qstd_mstmt_selget_names_id( qstd, name, parent_id );

  QRG( theid > 0 ? 0 : -1 );
  return theid;
}
