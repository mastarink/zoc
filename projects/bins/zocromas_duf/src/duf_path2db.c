#include <assert.h>                                                  /* assert */
#include <unistd.h>
#include <string.h>

#include "duf_tracen_defs_preset.h"                                  /* MAST_TRACE_CONFIG; etc. ✗ */
#include "duf_errorn_defs_preset.h"                                  /* MAST_ERRORS_FILE; etc. ✗ */

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>                                  /* mas_(malloc|free|strdup); etc. ▤ */
#include <mastar/trace/mas_trace.h>
#include <mastar/error/mas_error_defs_ctrl.h>
#include <mastar/error/mas_error_defs_make.h>
#include <mastar/error/mas_error_defs.h>

#include "duf_config.h"                                              /* duf_get_config ✗ */
#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ✗ */

#include "duf_levinfo_ref.h"                                         /* duf_levinfo_*; etc. ✗ */
#include "duf_levinfo_updown.h"
/* #include "duf_levinfo_openclose.h" */
#include "duf_levinfo_stat.h"
#include "duf_li.h"
#include "duf_sccbh_pstmt.h"

#include "duf_pdi_ref.h"
#include "duf_pdi_pi_ref.h"                                          /* duf_pdi_levinfo; duf_pdi_*depth; ✗ */
#include "duf_pdi_stmt.h"                                            /* duf_pdi_find_statement_by_id; etc. ✗ */

#include "duf_sccb_row_field_defs.h"                                 /* DUF_*FIELD2* ✗ */

#include "duf_sql_defs.h"                                            /* DUF_SQL_IDFIELD etc. ✗ */
#include "duf_sql_se_stmt_defs.h"                                    /* DUF_SQL_SE_BIND_S_OPT etc. ✗ */

#include "duf_evsql_selector_new.h"                                  /* duf_selector2sql_new; duf_selector2sql_2new; duf_expand_sql; ✗ */

#include "duf_sql_bind.h"                                            /* duf_sql_... for DUF_SQL_BIND_... etc. ✗ */
#include "duf_sql_prepared.h"                                        /* duf_sql_prepare; duf_sql_step; duf_sql_finalize; ✗ */
#include "duf_sql_field.h"                                           /* __duf_sql_str_by_name2 for DUF_GET_QUFIELD2 etc. ✗ */

#include "sql_beginning_tables.h"                                    /* DUF_SQL_TABLES... etc. ✗ */

#include "duf_pdi_structs.h"

/* ###################################################################### */
#include "duf_path2db.h"                                             /* duf_real_path2db; etc. ✗ */
/* ###################################################################### */

#ifndef MAS_SQL_TABLES_GLOBAL_DEFS_H
# error use #include "sql_tables_global_defs.h"
#endif

/* 20151026.111303 */
/* insert dir info to db by pdi and possbly fs */
static
SR( PDI, levinfo_stat_insert2db, duf_depthinfo_t * pdi, int *pchanges )
{

  int changes = 0;

  static const char *sql = "INSERT OR IGNORE INTO " DUF_SQL_TABLES_PATHS_FULL /* */
          " ( dev, rdev, inode,  mode,  size,  nlink,  uid,  gid,  blksize,  blocks, " /* */
          "           atim,                atimn,           mtim,                mtimn,           ctim,                ctimn, " /* */
          DUF_SQL_DIRNAMEFIELD ", parentid ) "                       /* */
          "  VALUES "                                                /* */
          " (:Dev, :rDev, :iNode, :Mode, :Size, :nLink, :UID, :GID, :blkSize, :Blocks, " /* */
          " datetime(:aTim, 'unixepoch'), :aTimn, datetime(:mTim, 'unixepoch'), :mTimn, datetime(:cTim, 'unixepoch'), :cTimn, " /* */
          " :dirName, :parentdirID )";

  DUF_SQL_SE_START_STMT( pdi, insert_path_table, sql, pstmt_local );

#if 0
  if ( !duf_levinfo_stat_dev( pdi ) )
    DOR( r, duf_levinfo_statat_dh( pdi ) );
#else
  ERRLOWER( STATAT_ENOENT );
/* DOR_LOWERE( r, duf_levinfo_if_statat_dh( pdi ), DUF_ERROR_STATAT_ENOENT ); */
  CR( levinfo_if_statat_dh, pdi );
  ERRUPPER( STATAT_ENOENT );

#endif
  assert( QISERR || duf_levinfo_stat_dev( pdi ) );

  MAST_TRACE( insert, 0, "S:%s (%lu,%lu,'%s',%llu)", sql, duf_levinfo_stat_dev( pdi ), duf_levinfo_stat_inode( pdi ),
              duf_levinfo_itemshowname( pdi ), duf_levinfo_dirid_up( pdi ) );
/* DUF_SHOW_ERROR( "insert_path_index:%d", insert_path_index ); */
  DUF_SQL_SE_BIND_S( dirName, /*  */ duf_levinfo_itemtruename( pdi ), /*  */ pstmt_local );

  DUF_SQL_SE_BIND_LL( Dev, /*                          */ duf_levinfo_stat_dev( pdi ), /*      */ pstmt_local );
  DUF_SQL_SE_BIND_LL( rDev, /*                         */ duf_levinfo_stat_rdev( pdi ), /*     */ pstmt_local );
  DUF_SQL_SE_BIND_LL( iNode, /*                        */ duf_levinfo_stat_inode( pdi ), /*    */ pstmt_local );
  DUF_SQL_SE_BIND_LL( Mode, ( unsigned long long ) /*  */ duf_levinfo_stat_mode( pdi ), /*     */ pstmt_local );
  DUF_SQL_SE_BIND_LL( Size, /*                         */ duf_levinfo_stat_size( pdi ), /*     */ pstmt_local );
  DUF_SQL_SE_BIND_LL( nLink, ( unsigned long long ) /* */ duf_levinfo_stat_nlink( pdi ), /*    */ pstmt_local );
  DUF_SQL_SE_BIND_LL( UID, /*                          */ duf_levinfo_stat_uid( pdi ), /*      */ pstmt_local );
  DUF_SQL_SE_BIND_LL( GID, /*                          */ duf_levinfo_stat_gid( pdi ), /*      */ pstmt_local );
  DUF_SQL_SE_BIND_LL( blkSize, /*                      */ duf_levinfo_stat_blksize( pdi ), /*  */ pstmt_local );
  DUF_SQL_SE_BIND_LL( Blocks, /*                       */ duf_levinfo_stat_blocks( pdi ), /*   */ pstmt_local );

  DUF_SQL_SE_BIND_LL( aTim, ( unsigned long long ) duf_levinfo_stat_asec( pdi ), pstmt_local );
  DUF_SQL_SE_BIND_LL( mTim, ( unsigned long long ) duf_levinfo_stat_msec( pdi ), pstmt_local );
  DUF_SQL_SE_BIND_LL( cTim, ( unsigned long long ) duf_levinfo_stat_csec( pdi ), pstmt_local );

  DUF_SQL_SE_BIND_LL( aTimn, ( unsigned long long ) duf_levinfo_stat_ansec( pdi ), pstmt_local );
  DUF_SQL_SE_BIND_LL( mTimn, ( unsigned long long ) duf_levinfo_stat_mnsec( pdi ), pstmt_local );
  DUF_SQL_SE_BIND_LL( cTimn, ( unsigned long long ) duf_levinfo_stat_cnsec( pdi ), pstmt_local );

/* DUF_SQL_BIND_LL( parentdirID, parentid, r, pstmt_local ); */
  DUF_SQL_SE_BIND_LL( parentdirID, duf_levinfo_dirid_up( pdi ), pstmt_local );
/* NO: duf_bind_ufilter_uni( pstmt_selector ); */

  DUF_SQL_SE_STEP( pstmt_local );
  DUF_SQL_SE_CHANGES( pdi, changes, pstmt_local );
  DUF_SQL_SE_END_STMT( pdi, insert_path_table, pstmt_local );
  if ( pchanges )
    *pchanges = changes;

  ER( PDI, levinfo_stat_insert2db, duf_depthinfo_t * pdi, int *pchanges );
}

/* 20151026.110612 */
/* at levinfo current level: set dirid,numdir,numfile */
static
SR( PDI, set_dirid_and_nums, duf_depthinfo_t * pdi, unsigned long long dirid )
{

  duf_levinfo_t *pli;

  pli = duf_levinfo_ptr( pdi );
  assert( pli );
  duf_levinfo_set_dirid( pdi, dirid );
  assert( dirid == duf_levinfo_dirid( pdi ) );

  ER( PDI, set_dirid_and_nums, duf_depthinfo_t * pdi, unsigned long long dirid );
}

/* 20151026.110729 */
/* at levinfo current level: set dirid,numdir,numfile
 * WRAPper for duf_set_dirid_and_nums */
static
SR( PDI, set_dirid_and_nums_from_pstmt, duf_depthinfo_t * pdi, duf_stmnt_t * pstmt_arg )
{

  CR( set_dirid_and_nums, pdi, DUF_GET_QUFIELD3( pstmt_arg, dirid ) ); /* at levinfo current level: set dirid,numdir,numfile */
  assert( DUF_GET_QUFIELD3( pstmt_arg, dirid ) == duf_levinfo_dirid( pdi ) );

  ER( PDI, set_dirid_and_nums_from_pstmt, duf_depthinfo_t * pdi, duf_stmnt_t * pstmt_arg );
}

/* 20151026.111126 */
/* at levinfo current level: set dirid,numdir,numfile by pdi and sql; */
static
SR( PDI, set_dirid_and_nums_from_sql, duf_depthinfo_t * pdi, const char *sqlv )
{

  assert( pdi->pdi_name );
  DUF_SQL_SE_START_STMT( pdi, select_path, sqlv, pstmt_local );
  MAST_TRACE( path, 2, "(%d:%s) dirid: %llu for '%s' at %llu; '%s'", QERRIND, QERRNAME, duf_levinfo_dirid( pdi ),
              duf_levinfo_itemtruename( pdi ), duf_levinfo_dirid_up( pdi ), sqlv );
  {
    const char *truedirname;

    truedirname = duf_levinfo_itemtruename( pdi );
    DUF_SQL_SE_BIND_LL( parentdirID, duf_levinfo_dirid_up( pdi ), pstmt_local );
  /* NO: duf_bind_ufilter_uni( pstmt_selector ); */
    DUF_SQL_SE_BIND_S( dirName, truedirname, pstmt_local );
    DUF_SQL_SE_STEP( pstmt_local );
  /* assert( DUF_IS_ERROR_N( r, DUF_SQL_ROW ) || DUF_IS_ERROR_N( r, DUF_SQL_DONE ) ); */
    if ( QISERR1_N( SQL_ROW ) )
    {
      ERRCLEAR1( SQL_ROW );

      MAST_TRACE( select, 0, "<selected> %s", sqlv );

      MAST_TRACE( select, 1, "S:%s (%llu,'%s') ~ '%s'; dirid:%llu", sqlv, duf_levinfo_dirid_up( pdi ), truedirname,
                  duf_levinfo_itemshowname( pdi ), DUF_GET_QUFIELD3( pstmt_local, dirid ) );
      if ( !DUF_GET_QUFIELD3( pstmt_local, dirid ) )
      {
      /* DUF_SHOW_ERROR( "(1) no dirid by parentid=%llu and " DUF_SQL_DIRNAMEFIELD "='%s'", duf_levinfo_dirid_up( pdi ), duf_levinfo_itemshowname( pdi ) ); */
        if ( QNOERR )
          ERRMAKE_M( NOT_IN_DB, "(1) no dirid by parentid=%llu AND " DUF_SQL_DIRNAMEFIELD "='%s'", duf_levinfo_dirid_up( pdi ),
                     duf_levinfo_itemshowname( pdi ) );
      }
      else
      {
        MAST_TRACE( explain, 20, "   ≪%s≫ in db as %llu @ %llu", duf_levinfo_itemshowname( pdi ), DUF_GET_QUFIELD3( pstmt_local, dirid ),
                    duf_levinfo_dirid_up( pdi ) );
      }
      assert( DUF_GET_QUFIELD3( pstmt_local, dirid ) );
      CR( set_dirid_and_nums_from_pstmt, pdi, pstmt_local );         /* at levinfo current level: set dirid,numdir,numfile; WRAPper for duf_set_dirid_and_nums */
      assert( DUF_GET_QUFIELD3( pstmt_local, dirid ) == duf_levinfo_dirid( pdi ) );
    }
    else if ( QISERR1_N( SQL_DONE ) )
    {
    /* DUF_CLEAR_ERROR( r, DUF_SQL_DONE ); */
      MAST_TRACE( select, 1, "S:%s (%llu,'%s') [%d] ~ '%s'; No dirid", sqlv, duf_levinfo_dirid_up( pdi ), truedirname, *truedirname,
                  duf_levinfo_itemshowname( pdi ) );
      MAST_TRACE( select, 10, "<NOT selected> (%d)", QERRIND );
      assert( DUF_GET_QUFIELD3( pstmt_local, dirid ) == duf_levinfo_dirid( pdi ) );
    }
    else
    {
    /* assert( DUF_GET_QUFIELD3( pstmt_local, dirid ) == duf_levinfo_dirid( pdi ) ); */
    }
  }
  assert( QISERR || ( DUF_GET_QUFIELD3( pstmt_local, dirid ) == duf_levinfo_dirid( pdi ) ) );
  DUF_SQL_SE_END_STMT( pdi, select_path, pstmt_local );

  ER( PDI, set_dirid_and_nums_from_sql, duf_depthinfo_t * pdi, const char *sqlv );
}

/* 20151026.111923 */
/* at levinfo current level: set dirid,numdir,numfile by pdi and sql set (duf_sql_set_t); */
static
SR( PDI, set_dirid_and_nums_from_sql_set, duf_depthinfo_t * pdi, const duf_sql_set_t * sql_set )
{
  unsigned long long dirid = 0;

  char *sqlv = NULL;

  duf_sql_set_t def_node_set = {
    .fieldset = "pt." DUF_SQL_IDFIELD " AS dirid "                   /* */
            ", pt." DUF_SQL_DIRNAMEFIELD " AS dname "                /*      */
            ,
    .selector2 = " FROM " DUF_SQL_TABLES_PATHS_FULL " AS pt "        /* */
            " WHERE " DUF_DBPREF "pt.ParentId=:parentdirID AND (:dirName IS NULL OR dname=:dirName)" /* */
  };

  assert( pdi );
  assert( pdi->pdi_name );
  assert( !sql_set || sql_set->type == DUF_NODE_NODE );              /* 20160214.162255 */
#if 0
  sqlv = duf_selector2sql( sql_set ? sql_set : &def_node_set, pdi->pdi_name, &r );
#else
  sqlv = duf_selector2sql_new( sql_set ? sql_set : &def_node_set, 0 /* orderid */ , pdi->pdi_name, 0 /*total */ , QPERRIND );
#endif
  CR( set_dirid_and_nums_from_sql, pdi, sqlv );                      /* at levinfo current level: set dirid,numdir,numfile by pdi and sql; */
  MAST_TRACE( path, 2, "(%d:%s) dirid: %llu for '%s' at %llu", QERRIND, QERRNAME, duf_levinfo_dirid( pdi ), duf_levinfo_itemtruename( pdi ),
              duf_levinfo_dirid_up( pdi ) );
  if ( QNOERR )
    dirid = duf_levinfo_dirid( pdi );
  mas_free( sqlv );
  MAST_TRACE( collect, 1, "sometime inserted (SQLITE_OK) dirid=%llu:'%s'", dirid, duf_levinfo_itemshowname( pdi ) );
  assert( dirid == duf_levinfo_dirid( pdi ) );
/* assert( !need_id || dirid ); */

  ER( PDI, set_dirid_and_nums_from_sql_set, duf_depthinfo_t * pdi, const duf_sql_set_t * sql_set );
}

/* TODO remake to pdiitem2dirid?? */
/* at levinfo current level: set dirid,numdir,numfile */
static
SR( PDI, levinfo_stat2dirid_i, duf_depthinfo_t * pdi, int caninsert, const duf_sql_set_t * sql_set, int need_id )
{

  assert( pdi );
  assert( pdi->pdi_name );
  assert( !sql_set || sql_set->type == DUF_NODE_NODE );              /* 20160214.162255 */

  MAST_TRACE( path, 10, "@@@@@@@@@@@ %llu/%llu; caninsert:%d; pdi:%d", duf_levinfo_dirid( pdi ), duf_levinfo_dirid_up( pdi ), caninsert,
              pdi ? 1 : 0 );
  MAST_TRACE( path, 2, "@inserting [%s] caninsert:%d", duf_levinfo_itemshowname( pdi ), caninsert );
  {
    int changes = 0;

    CR( set_dirid_and_nums_from_sql_set, pdi, sql_set );             /* at levinfo current level:
                                                                        set dirid,numdir,numfile by pdi and sql set (duf_sql_set_t); */
    MAST_TRACE( path, 2, "(%d:%s) dirid before insert: %llu for '%s' at %llu", QERRIND, QERRNAME, duf_levinfo_dirid( pdi ),
                duf_levinfo_itemtruename( pdi ), duf_levinfo_dirid_up( pdi ) );

    if ( duf_levinfo_dirid( pdi ) <= 0 && caninsert && ! /* DUF_CONFIGG( opt.disable.flag.insert ) */ duf_get_config_flag_disable_insert(  ) )
    {
      CR( levinfo_stat_insert2db, pdi, &changes );                   /* insert dir info to db by pdi and possbly fs */
    }
    MAST_TRACE( select, 10, "<changes> : %d : %s", changes, duf_levinfo_itemshowname( pdi ) );
    if ( duf_levinfo_dirid( pdi ) <= 0 && need_id )
    {
      if ( changes )
      {
        if ( QNOERR )
        {
          CR( set_dirid_and_nums, pdi, duf_sql_last_insert_rowid(  ) ); /* at levinfo current level:
                                                                           set dirid from last inserted record (numdir=0,numfile=0) */
          if ( duf_levinfo_dirid_up( pdi ) )
          {
            MAST_TRACE( explain, 10, "   ≪%s≫ in db as %llu @ %llu", duf_levinfo_itemshowname( pdi ), duf_levinfo_dirid_up( pdi ),
                        duf_levinfo_dirid_up( pdi ) );
          }
          if ( !duf_levinfo_dirid( pdi ) )
          {
          /* DUF_SHOW_ERROR( "(2) no dirid by parentid=%llu AND " DUF_SQL_DIRNAMEFIELD "='%s'", duf_levinfo_dirid_up( pdi ), duf_levinfo_itemshowname( pdi ) ); */
            ERRMAKE_M( NOT_IN_DB, "(2) no dirid by parentid=%llu AND " DUF_SQL_DIRNAMEFIELD "='%s'", duf_levinfo_dirid_up( pdi ),
                       duf_levinfo_itemshowname( pdi ) );
          }
          MAST_TRACE( collect, 1, "inserted (SQLITE_OK) dirid=%llu:'%s'", duf_levinfo_dirid( pdi ), duf_levinfo_itemshowname( pdi ) );
        }
      }
      else
      {
        if ( QISERR1_N( SQL_CONSTRAINT ) )
        {
          if ( caninsert )
          {
            assert( 0 );                                             /*  */
            CR( set_dirid_and_nums_from_sql_set, pdi, sql_set );     /* at levinfo current level:
                                                                        set dirid,numdir,numfile by pdi and sql set (duf_sql_set_t); */

          }
          else
          {
            ERRMAKE( NOT_IN_DB );
          }
        }
        else if ( QNOERR )
        {
          if ( caninsert )
          {
            assert( 0 );                                             /*  */
            CR( set_dirid_and_nums_from_sql_set, pdi, sql_set );     /* at levinfo current level:
                                                                        set dirid,numdir,numfile by pdi and sql set (duf_sql_set_t); */

          }
          else
          {
            ERRMAKE( NOT_IN_DB );
          }
        }
      /* assert( 0 );            (*  *) */
      }

    }
  }
  if ( need_id && !duf_levinfo_dirid( pdi ) )
  {
    if ( QNOERR )
      ERRMAKE( NOT_IN_DB );
#if 0
    _DUF_SHOW_ERROR( "@@@@@@@@(%s) no dirid by parentid=%llu and " DUF_SQL_DIRNAMEFIELD "='%s'", mas_error_name( r ), duf_levinfo_dirid_up( pdi ),
                     duf_levinfo_itemshowname( pdi ) );
#endif
  }

  ER( PDI, levinfo_stat2dirid_i, duf_depthinfo_t * pdi, int caninsert, const duf_sql_set_t * sql_set, int need_id );
}

/* 20151026.112249 */
/* at levinfo current level: set dirid,numdir,numfile; WRAPper for duf_levinfo_stat2dirid_i */
SR( PDI, levinfo_stat2dirid, duf_depthinfo_t * pdi, int caninsert, const duf_sql_set_t * sql_set )
{

  CR( levinfo_stat2dirid_i, pdi, caninsert, sql_set, 1 );            /* at levinfo current level: set dirid,numdir,numfile */

  ER( PDI, levinfo_stat2dirid, duf_depthinfo_t * pdi, int caninsert, const duf_sql_set_t * sql_set );
}

/* 20151026.112425 */
/* at levinfo down level: set dirid,numdir,numfile; make level current */
static
SR( PDI, levinfo_down_stat2dirid, duf_depthinfo_t * pdi, const char *directory_name, int caninsert, const duf_sql_set_t * sql_set )
{

  int up_d = 0;

  assert( pdi );
  assert( !sql_set || sql_set->type == DUF_NODE_NODE );              /* 20160214.162255 */

  up_d = duf_pdi_depth( pdi );

  CR( levinfo_godown_openat_dh, pdi, directory_name, DUF_NODE_NODE /* node_type */  ); /* check depth; 
                                                                                          resets levinfo  (currenl level);
                                                                                          may change levinfo (for upper level);
                                                                                          does not really open dh?!;
                                                                                          XXX equal XXX to duf_levinfo_godown 
                                                                                          - except DUF_ERROR_TOO_DEEP ? XXX; */

  assert( QISERR || up_d + 1 == duf_pdi_depth( pdi ) );
  CR( levinfo_stat2dirid, pdi, caninsert, sql_set /*, 1 -- need_id   - error (0= not) if there is no record */  ); /* at levinfo current level:
                                                                                                                      set dirid,numdir,numfile;
                                                                                                                      WRAPper for duf_levinfo_stat2dirid_i */

  ER( PDI, levinfo_down_stat2dirid, duf_depthinfo_t * pdi, const char *directory_name, int caninsert, const duf_sql_set_t * sql_set );
}

/* 20151026.113322 */
/* parse real_path to components and store/check each to db,
 *   setting each level info to levinfo; note: sets depth + n */
static
SR( PDI, real_path2db_i, duf_depthinfo_t * pdi, char *real_path, int caninsert, const duf_sql_set_t * sql_set /* const char *node_selector2 */  )
{

  int od = 0;

/* unsigned long long parentid = 0; */
  assert( pdi );
  assert( pdi->pdi_name );
  assert( duf_levinfo_dirid( pdi ) == 0 );
  assert( !sql_set || sql_set->type == DUF_NODE_NODE );              /* 20160214.162255 */

  MAST_TRACE( fs, 2, "set opendir to 1 (one)" );
  od = duf_pdi_set_opendir( pdi, 1 );                                /* save open status to restore */
  {
    char *path;

    path = real_path;

    MAST_TRACE( path, 6, "@@@@@@%s PATHID for       [%40s]", caninsert ? "ADD" : "GET", real_path );
    while ( QNOERR && path && *path )
    {
      char *nextdir = path;

      nextdir = strchr( path, '/' );
      if ( nextdir )
        *nextdir++ = 0;

      MAST_TRACE( path, 6, "@@      +prepared path:[%s]\tup to [%s]", path, nextdir );
      {
        int up_d = 0;

        up_d = duf_pdi_depth( pdi );
      /* down to next */
        CR( levinfo_down_stat2dirid, pdi, path, caninsert, sql_set ); /* at levinfo down level: set dirid,numdir,numfile; make level current */
        assert( QISERR || up_d + 1 == duf_pdi_depth( pdi ) );
      }
      path = nextdir;
    }
  }
  MAST_TRACE( fs, 2, "set (restore) opendir to %d", od );
  duf_pdi_set_opendir( pdi, od );                                    /* restore saved open status */
  assert( QISERR || !caninsert || !real_path || duf_levinfo_dirid( pdi ) > 0 );
  assert( QISERR || !caninsert || !real_path || duf_levinfo_path( pdi ) );

  ER( PDI, real_path2db_i, duf_depthinfo_t * pdi, char *real_path, int caninsert, const duf_sql_set_t * sql_set );
}

/* parse real_path to components and store/check each to db, setting each level info to levinfo */
SR( PDI, real_path2db, duf_depthinfo_t * pdi, int caninsert, const char *rpath, const duf_sql_set_t * sql_set )
{

  char *real_path;

  assert( pdi );
  assert( pdi->pdi_name );
/* assert( pdi->depth == -1 ); */
  assert( !sql_set || sql_set->type == DUF_NODE_NODE );              /* 20160214.162255 */

  real_path = mas_strdup( rpath );

  assert( real_path != rpath );                                      /* MEMORY ERROR !! FIXME  FIXME  FIXME  FIXME  FIXME  FIXME */

  MAST_TRACE( path, 6, "@@@%s PATHID for       [%40s]", caninsert ? "ADD" : "GET", real_path );
  {
    MAST_TRACE( explain, 10, "real_path: ≪%s≫", real_path );
  /* duf_real_path2db_i
   *  need a copy of real_path to modify during parse
   *
   *   note: sets depth + n
   * */
    /* QT( "@   %s =>", rpath ); */
    /* QT( "@=> %s", real_path ); */
    CR( real_path2db_i, pdi, real_path, caninsert, sql_set );        /* parse real_path to components and store/check each to db,
                                                                        setting each level info to levinfo;
                                                                        -XXX- sets depth + n -XXX- */
    duf_pdi_set_topdepth( pdi );
  }
  mas_free( real_path );

  ER( PDI, real_path2db, duf_depthinfo_t * pdi, int caninsert, const char *rpath, const duf_sql_set_t * sql_set );
}
