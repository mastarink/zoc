/* #undef MAS_TRACING */
#include <assert.h>                                                  /* assert */
#include <stddef.h>                                                  /* NULL */

#include "duf_tracen_defs_preset.h"

#include <mastar/wrap/mas_std_def.h>
#include <mastar/trace/mas_trace.h>

#include "duf_tracen_defs.h"                                         /* MAST_TRACE ♠ */
#include "duf_errorn_defs.h"                                         /* DUF_NOERROR; DUF_CLEAR_ERROR; DUF_E_(LOWER|UPPER); DUF_TEST_R ... ♠ */

#include "duf_start_end.h"                                           /* DUF_STARTR ; DUF_ENDR ♠ */
#include "duf_dodefs.h"                                              /* DOR ♠ */

#include "duf_config.h"                                              /* duf_get_config ♠ */
#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ♠ */
#include "duf_config_ref.h"
#include "duf_config_defs.h"                                         /* DUF_CONF... ♠ */

#include "duf_levinfo_ref.h"

#include "duf_pdi_ref.h"
#include "duf_pdi_stmt.h"

#include "duf_sql_defs.h"                                            /* DUF_SQL_IDFIELD etc. ♠ */
#include "duf_sql_stmt_defs.h"                                       /* DUF_SQL_BIND_S_OPT etc. ♠ */
#include "duf_sql_prepared.h"
#include "duf_sql_bind.h"                                            /* duf_sql_... for DUF_SQL_BIND_... etc. ♠ */
#include "duf_sql_field.h"                                           /* __duf_sql_str_by_name2 for DUF_GET_UFIELD2 etc. ♠ */
#include "duf_sql_prepared.h"

#include "sql_beginning_tables.h"                                    /* DUF_SQL_TABLES... etc. ♠ */

/* ###################################################################### */
#include "duf_filedata.h"
/* ###################################################################### */

unsigned long long
duf_pdistat2file_dataid_existed( duf_depthinfo_t * pdi, int *pr )
{
  int rpr = 0;
  unsigned long long dataid = 0;
  const char *sql =
          "SELECT " DUF_SQL_IDFIELD " AS dataid FROM " DUF_SQL_TABLES_FILEDATAS_FULL
          " INDEXED BY " DUF_SQL_TABLES_FILEDATAS "_uniq WHERE dev=:Dev AND inode=:iNode";

  DUF_START(  );

  DUF_SQL_START_STMT( pdi, select_filedata, sql, rpr, pstmt );
  MAST_TRACE( select, 3, "S:%s", sql );
  DUF_SQL_BIND_LL( Dev, ( unsigned long long ) duf_levinfo_stat_dev( pdi ), rpr, pstmt );
  DUF_SQL_BIND_LL( iNode, ( unsigned long long ) duf_levinfo_stat_inode( pdi ), rpr, pstmt );
  DUF_SQL_STEP( rpr, pstmt );
  if ( DUF_IS_ERROR_N( rpr, DUF_SQL_ROW ) )
  {
    MAST_TRACE( select, 10, "<selected>" );
#if 0
    dataid = duf_sql_column_long_long( pstmt, 0 );
#else
    dataid = DUF_GET_UFIELD2( dataid );
#endif
    assert( dataid > 0 );
  /* rpr = 0; */
  }
  else
  {
  /* DUF_TEST_R( rpr ); */
    MAST_TRACE( select, 0, "@<NOT selected> (%d)", rpr );
    assert( 0 );
  }
/* DUF_TEST_R( rpr ); */
  DUF_SQL_END_STMT( pdi, select_filedata, rpr, pstmt );
  if ( pr )
    *pr = rpr;
  assert( dataid > 0 );
  DUF_ENDULL( dataid );
  return dataid;
}

unsigned long long
duf_pdistat2file_dataid( duf_depthinfo_t * pdi, int need_id, int *pr )
{
  int rpr = 0;
  unsigned long long dataid = 0;

  DUF_START(  );

  {
    int changes = 0;

    if ( !DUF_CONFIGG( opt.disable.flag.insert ) )
    {
      const char *sql = "INSERT OR IGNORE INTO " DUF_SQL_TABLES_FILEDATAS_FULL /* */
              " (dev,   inode,  size,  mode,  nlink,  uid,  gid,  blksize,  blocks,  atim,  atimn,  mtim,  mtimn,  ctim,  ctimn) " /* */
              " VALUES "                                             /* */
              " (:Dev, :iNode, :Size, :Mode, :nLink, :UID, :GID, :blkSize, :Blocks, " /* */
              "datetime(:aTim, 'unixepoch'), :aTimn, " "datetime(:mTim, 'unixepoch'), :mTimn, " "datetime(:cTim, 'unixepoch'), :cTimn) " /* */ ;

      DUF_SQL_START_STMT( pdi, insert_filedata, sql, rpr, pstmt );
      MAST_TRACE( insert, 0, "S:%s", sql );

      DUF_SQL_BIND_LL( Dev, ( unsigned long long ) duf_levinfo_stat_dev( pdi ), rpr, pstmt );
      DUF_SQL_BIND_LL( iNode, ( unsigned long long ) duf_levinfo_stat_inode( pdi ), rpr, pstmt );
      DUF_SQL_BIND_LL( Mode, ( unsigned long long ) duf_levinfo_stat_mode( pdi ), rpr, pstmt );
      DUF_SQL_BIND_LL( Size, ( unsigned long long ) duf_levinfo_stat_size( pdi ), rpr, pstmt );
      DUF_SQL_BIND_LL( nLink, ( unsigned long long ) duf_levinfo_stat_nlink( pdi ), rpr, pstmt );
      DUF_SQL_BIND_LL( UID, ( unsigned long long ) duf_levinfo_stat_uid( pdi ), rpr, pstmt );
      DUF_SQL_BIND_LL( GID, ( unsigned long long ) duf_levinfo_stat_gid( pdi ), rpr, pstmt );
      DUF_SQL_BIND_LL( blkSize, ( unsigned long long ) duf_levinfo_stat_blksize( pdi ), rpr, pstmt );
      DUF_SQL_BIND_LL( Blocks, ( unsigned long long ) duf_levinfo_stat_blocks( pdi ), rpr, pstmt );

      DUF_SQL_BIND_LL( aTim, ( unsigned long long ) duf_levinfo_stat_asec( pdi ), rpr, pstmt );
      DUF_SQL_BIND_LL( mTim, ( unsigned long long ) duf_levinfo_stat_msec( pdi ), rpr, pstmt );
      DUF_SQL_BIND_LL( cTim, ( unsigned long long ) duf_levinfo_stat_csec( pdi ), rpr, pstmt );

      DUF_SQL_BIND_LL( aTimn, ( unsigned long long ) duf_levinfo_stat_ansec( pdi ), rpr, pstmt );
      DUF_SQL_BIND_LL( mTimn, ( unsigned long long ) duf_levinfo_stat_mnsec( pdi ), rpr, pstmt );
      DUF_SQL_BIND_LL( cTimn, ( unsigned long long ) duf_levinfo_stat_cnsec( pdi ), rpr, pstmt );

      DUF_SQL_STEPC( rpr, pstmt );
      DUF_SQL_CHANGES( changes, rpr, pstmt );
      DUF_SQL_END_STMT( pdi, insert_filedata, rpr, pstmt );
    }
    MAST_TRACE( select, 2, "<changes> : %d", changes );
    if ( need_id )
    {
      if ( ( DUF_IS_ERROR_N( rpr, DUF_SQL_CONSTRAINT ) || DUF_NOERROR( rpr ) ) && !changes )
      {
        dataid = duf_pdistat2file_dataid_existed( pdi, pr );
      }
      else if ( DUF_NOERROR( rpr ) /* assume SQLITE_OK */  && changes )
      {
        dataid = duf_sql_last_insert_rowid(  );
        if ( !dataid )
        {
          if ( DUF_NOERROR( rpr ) )
            DUF_MAKE_ERRORM( rpr, DUF_ERROR_NOT_IN_DB, "(2) no dataid by parentid=%llu", duf_levinfo_dirid_up( pdi ) );
        }
        else
        {
        }
        assert( dataid );
        MAST_TRACE( collect, 1, "inserted (SQLITE_OK) dataid=%llu", dataid );
      }
      DUF_TEST_R( rpr );
    }
  }
/* else                                   */
/* {                                      */
/*   DUF_SHOW_ERROR( "Wrong data" );      */
/*   DUF_MAKE_ERROR( rpr, DUF_ERROR_DATA ); */
/* }                                      */
  if ( pr )
    *pr = rpr;
  assert( !need_id || dataid );
  DUF_ENDULL( dataid );
  return dataid;
}

int
duf_pdistat2file( duf_depthinfo_t * pdi )
{
  DUF_STARTR( r );

  ( void ) /* dataid= */ duf_pdistat2file_dataid( pdi, 0 /*need_id */ , &r );
  DUF_ENDR( r );
}
