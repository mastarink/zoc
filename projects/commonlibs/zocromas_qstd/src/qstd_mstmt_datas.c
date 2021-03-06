#define R_GOOD(_r) ((_r)>=0)
/* #define RGEMSG mysql_error(mas_qstd_mysql(qstd)) */
#define RGEMSG mas_qstd_mysql_error(qstd)
#include "qstd_defs.h"
/* #include <string.h> */
/* #include <sys/types.h> */
/* #include <sys/stat.h> */
/* #include <unistd.h> */

/* #include <mastar/wrap/mas_memory.h> */
#include <mastar/minierr/minierr.h>
#include <mastar/regerr/masregerr.h>

/* #include <mastar/masxfs/masxfs_types.h> */

/* #include <mastar/mysqlpfs/mysqlpfs_base.h> */
/* #include <mastar/mysqlpfs/mysqlpfs_mstmt_base.h> */
#include <mastar/mysqlpfs/mysqlpfs_mstmt.h>
#include <mastar/mysqlpfs/mysqlpfs_defs.h>

#include "qstd_structs.h"
#include "qstd_enums.h"
/* #include "qstd_mstmt.h" */
#include "qstd_mstmt_base.h"
#include "qstd_mstmt.h"

#include "qstd_mstmt_datas.h"

unsigned long long
mas_qstd_mstmt_selget_datas_id( mas_qstd_t * qstd, const mas_qstd_stat_t * stat )
{
  rDECLBAD;
  unsigned long long theid = 0;

  {
    int np = 0;
    int nr = 0;
    int has_data = 0;
    mysqlpfs_mstmt_t *mstmt_s = mas_qstd_mstmt_get( qstd, STD_MSTMT_SELECT_DATAS_ID );

    QRGP( mstmt_s );
    rC( mas_qstd_ret_code( qstd ) );
    /* rC( mas_qstd_mstmt_ret_code( mstmt_s ) ); */

    rC( mas_qstd_mstmt_set_param_longlong( mstmt_s, np++, stat->st_dev, FALSE ) );
    rC( mas_qstd_mstmt_set_param_longlong( mstmt_s, np++, stat->st_ino, FALSE ) );
    rC( mas_qstd_mstmt_execute_store( mstmt_s ) );

    rC( mas_qstd_mstmt_fetch( mstmt_s, &has_data ) );

    if ( has_data )
    {
      unsigned is_null = 0;

      rC( mas_qstd_mstmt_get_result_longlong( mstmt_s, nr++, &theid, &is_null ) );
      assert( nr == STD_MSTMT_SELECT_DATAS_NRESULTS );
    }

    mas_qstd_mstmt_free_result( mstmt_s );
  }
  qstd->ret_code = rCODE;
  return theid;
}

unsigned long long
mas_qstd_mstmt_insget_datas_id( mas_qstd_t * qstd, const mas_qstd_stat_t * stat )
{
  rDECLBAD;
  QRGP( qstd );
  unsigned long long theid = 0;

  {
    int np = 0;
    mysqlpfs_mstmt_t *mstmt = mas_qstd_mstmt_get( qstd, STD_MSTMT_INSERT_DATAS );

    QRGP( mstmt );
    rC( mas_qstd_ret_code( qstd ) );
    /* rC( mas_qstd_mstmt_ret_code( mstmt ) ); */
    rC( mas_qstd_mstmt_set_param_longlong( mstmt, np++, stat->st_dev, FALSE ) );
    rC( mas_qstd_mstmt_set_param_longlong( mstmt, np++, stat->st_ino, FALSE ) );
    rC( mas_qstd_mstmt_set_param_longlong( mstmt, np++, stat->st_nlink, stat->st_nlink ? FALSE : TRUE ) );

    rC( mas_qstd_mstmt_execute( mstmt ) );
    if ( rGOOD && mas_qstd_mstmt_affected_rows( mstmt ) == 1 )
    {
      theid = mas_qstd_mstmt_insert_id( mstmt );
      rC( mas_qstd_mstmt_ret_code( mstmt ) );
    }
  }
/* QRG( theid >= 0 ? 0 : -1 ); */
  qstd->ret_code = rCODE;
  return theid;
}

unsigned long long
mas_qstd_mstmt_selinsget_datas_id( mas_qstd_t * qstd, const mas_qstd_stat_t * stat )
{
  rDECLBAD;
  QRGP( qstd );

  unsigned long long theid = mas_qstd_mstmt_selget_datas_id( qstd, stat );

  rC( mas_qstd_ret_code( qstd ) );

  if ( rGOOD && !theid )
  {
    theid = mas_qstd_mstmt_insget_datas_id( qstd, stat );
    rC( mas_qstd_ret_code( qstd ) );
  }

/* QRG( theid >= 0 ? 0 : -1 ); */
  qstd->ret_code = rCODE;
  return theid;
}

unsigned long long
mas_qstd_mstmt_insselget_datas_id( mas_qstd_t * qstd, const mas_qstd_stat_t * stat )
{
  rDECLBAD;
  QRGP( qstd );
  unsigned long long theid = mas_qstd_mstmt_insget_datas_id( qstd, stat );

  rC( mas_qstd_ret_code( qstd ) );

  if ( rGOOD && !theid )
  {
    theid = mas_qstd_mstmt_selget_datas_id( qstd, stat );
    rC( mas_qstd_ret_code( qstd ) );
  }

/* QRG( theid >= 0 ? 0 : -1 ); */
  qstd->ret_code = rCODE;
  return theid;
}
