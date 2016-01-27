#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>

#include <mastar/tools/mas_arg_tools.h>


#include "duf_maintenance.h"

#include "duf_config_util.h"
#include "duf_config_ref.h"
#include "duf_config_defs.h"
#include "duf_config.h"
#include "duf_status_ref.h"

#include "duf_sys_wrap.h"

#include "duf_option_defs.h"
#include "duf_option_names.h"
#include "duf_option_stage.h"
#include "duf_option_config.h"

#include "duf_options.h"

#include "duf_levinfo_ref.h"

#include "duf_sql_open.h"

#include "duf_pdi.h"
#include "duf_pdi_ref.h"
#include "duf_pdi_reinit.h"

#include "duf_maindb_info.h"

#include "evsql_begfin.h"
#include "evsql_selector.h"

#include "sql_tables_defs.h"
#include "sql_beginning_common.h"
#include "sql_beginning_tables.h"
#include "sql_beginning_vacuum.h"
#include "sql_beginning_create.h"


/* ###################################################################### */
#include "duf_maindb.h"
/* ###################################################################### */

#if 0
static int
duf_main_db_attach_selected( const char *name )
{
  DEBUG_STARTR( r );
  static const char *sql = "ATTACH DATABASE '${DB_PATH}${SELECTED_DB}' AS duf${SELECTED_DB}";
  char *worksql;

  worksql = duf_expand_selected_db( sql, name );
  DORF( r, duf_main_db_open );
  DUF_TRACE( temp, 0, "@@@@@attach selected database %s", worksql );

  DUF_TRACE( explain, 0, "attach selected database %s", worksql );
  DUF_SQL_START_STMT_NOPDI( worksql, r, pstmt );
  DUF_SQL_STEP( r, pstmt );
  DUF_SQL_END_STMT_NOPDI( r, pstmt );
  DUF_TRACE( temp, 0, "@@@@attached selected database %s", worksql );
  mas_free( worksql );
  DEBUG_ENDR( r );
}
#endif

static int
duf_main_db_locate( void )
{
  DEBUG_STARTR( r );

  DOR( r, duf_config_make_db_paths(  ) );

  DEBUG_ENDR( r );
}

static int
duf_main_db_remove_files( void )
{
  DEBUG_STARTR( r );
  if ( DUF_CONFIGG( db.main.fpath ) )
    DORF( r, duf_unlink, DUF_CONFIGG( db.main.fpath ) );
  DEBUG_ENDR( r );
}

static int
duf_main_db_optionally_remove_files( void )
{
  DEBUG_STARTR( r );
  if ( DUF_ACTG_FLAG( remove_database ) )
  {
    DUF_TRACE( explain, 0, "     option %s, removing database", DUF_OPT_FLAG_NAME( REMOVE_DATABASE ) );
    DORF( r, duf_main_db_remove_files );
  }
  else
  {
    DUF_TRACE( explain, 1, "no %s option, not removing database", DUF_OPT_FLAG_NAME( REMOVE_DATABASE ) );
  }
  DEBUG_ENDR( r );
}

static int
duf_main_db_create_tables( void )
{
  DEBUG_STARTR( r );
  DUF_TRACE( explain, 0, "     option %s : to check / create db tables", DUF_OPT_FLAG_NAME( CREATE_TABLES ) );
  /* DOR( r, duf_check_tables(  ) ); */
  if ( DUF_OPTG_FLAG( dry_run ) )
    DUF_PRINTF( 0, "DRY %s : action '%s'", DUF_OPT_FLAG_NAME( DRY_RUN ), DUF_OPT_FLAG_NAME2( CREATE_TABLES ) );
  else
  {
    DORF( r, duf_eval_sqlsq, &sql_beginning_create_one, 0, ( const char * ) NULL /* title */ , ( duf_ufilter_t * ) NULL, ( duf_yfilter_t * ) NULL,
          ( const char * ) NULL /* selected.db */  );
    DORF( r, duf_eval_sqlsq, &sql_beginning_create_two, 0, ( const char * ) NULL /* title */ , ( duf_ufilter_t * ) NULL, ( duf_yfilter_t * ) NULL,
          ( const char * ) NULL /* selected.db */  );
    DORF( r, duf_eval_sqlsq, &sql_beginning_create_three, 0, ( const char * ) NULL /* title */ , ( duf_ufilter_t * ) NULL, ( duf_yfilter_t * ) NULL,
          ( const char * ) NULL /* selected.db */  );
    DORF( r, duf_eval_sqlsq, &sql_beginning_create_four, 0, ( const char * ) NULL /* title */ , ( duf_ufilter_t * ) NULL, ( duf_yfilter_t * ) NULL,
          ( const char * ) NULL /* selected.db */  );
  }
  DEBUG_ENDR( r );
}

static int
duf_main_db_pre_action( void )
{
  DEBUG_STARTR( r );
/* --drop-tables								*/ DEBUG_STEP(  );
  if ( DUF_NOERROR( r ) && DUF_ACTG_FLAG( drop_tables ) )
  {
    DUF_TRACE( explain, 0, "drop (zero) tables: option %s", DUF_OPT_FLAG_NAME( DROP_TABLES ) );
    if ( DUF_OPTG_FLAG( dry_run ) )
      DUF_PRINTF( 0, "DRY %s : action '%s'", DUF_OPT_FLAG_NAME( DRY_RUN ), DUF_OPT_FLAG_NAME2( DROP_TABLES ) );
    else
      DORF( r, duf_eval_sqlsq, &sql_beginning_drop, 0, NULL /* title */ , ( duf_ufilter_t * ) NULL, ( duf_yfilter_t * ) NULL,
            NULL /* selected.db */  );
    global_status.actions_done++;
  }
  else
  {
    DUF_TRACE( explain, 1, "no %s option, not dropping tables", DUF_OPT_FLAG_NAME( DROP_TABLES ) );
  }

  if ( DUF_NOERROR( r ) && DUF_ACTG_FLAG( clean_tables ) )
  {
    DUF_TRACE( explain, 0, "clean (zero) tables: option %s", DUF_OPT_FLAG_NAME( CLEAN_TABLES ) );
    if ( DUF_OPTG_FLAG( dry_run ) )
      DUF_PRINTF( 0, "DRY %s : action '%s'", DUF_OPT_FLAG_NAME( DRY_RUN ), DUF_OPT_FLAG_NAME2( CLEAN_TABLES ) );
    else
      DORF( r, duf_eval_sqlsq, &sql_beginning_clean, 0, NULL /* title */ , ( duf_ufilter_t * ) NULL, ( duf_yfilter_t * ) NULL,
            NULL /* selected.db */  );
    global_status.actions_done++;
  }
  else
  {
    DUF_TRACE( explain, 1, "no %s option, not dropping tables", DUF_OPT_FLAG_NAME( DROP_TABLES ) );
  }

/* --create-tables								*/ DEBUG_STEP(  );
  if ( DUF_NOERROR( r ) && DUF_ACTG_FLAG( create_tables ) /* && DUF_ACTG_FLAG( create_database ) */  )
  {
    DOR( r, duf_main_db_create_tables(  ) );
    global_status.actions_done++;
  }
  else
  {
    DUF_TRACE( explain, 1, "no %s option", DUF_OPT_FLAG_NAME( CREATE_TABLES ) );
  }

  if ( DUF_NOERROR( r ) && DUF_ACTG_FLAG( vacuum ) )
  {
    /* static const char *sql = "VACUUM"; */

    /* DUF_TRACE( explain, 0, "[ %s ]  option %s", sql, DUF_OPT_FLAG_NAME( VACUUM ) ); */
    if ( DUF_OPTG_FLAG( dry_run ) )
      DUF_PRINTF( 0, "DRY %s : action '%s'", DUF_OPT_FLAG_NAME( DRY_RUN ), DUF_OPT_FLAG_NAME2( VACUUM ) );
    else
      DORF( r, duf_eval_sqlsq, &sql_beginning_vacuum, 0, NULL /* title */ , ( duf_ufilter_t * ) NULL, ( duf_yfilter_t * ) NULL,
            NULL /* selected.db */  );
    global_status.actions_done++;
  }
  else
  {
    DUF_TRACE( explain, 1, "no %s option", DUF_OPT_FLAG_NAME( VACUUM ) );
  }


  DORF( r, duf_eval_sqlsq, &sql_beginning_tables, 0, NULL /* title */ , ( duf_ufilter_t * ) NULL, ( duf_yfilter_t * ) NULL, NULL /* selected.db */  );

  DEBUG_ENDR( r );
}




#ifdef DUF_ATTACH_TTABLES_PATTERN
#  ifdef DUF_SQL_TTABLES_TEMPORARY
#    error "Wrong DUF_ATTACH_SELECTED_PATTERN / DUF_SQL_SELECTED_TEMPORARY : add include sql_tables_defs.h"
#  endif
#else
#  ifndef DUF_SQL_TTABLES_TEMPORARY
#    error "Wrong DUF_ATTACH_SELECTED_PATTERN / DUF_SQL_SELECTED_TEMPORARY : add include sql_tables_defs.h"
#  endif
#endif




static int
duf_main_db_tune( void )
{
  DEBUG_STARTR( r );
#ifdef MAS_SPLIT_DB
  if ( DUF_CONFIGG( db.adm.fpath ) )
  {
    static const char *sql = "ATTACH DATABASE '" DUF_ATTACH_COMMON_PATTERN "adm.db' AS " DUF_DBADMALIAS;

    DOR( r, duf_eval_sql_one( sql, ( duf_ufilter_t * ) NULL, ( duf_yfilter_t * ) NULL, DUF_DBADMALIAS, NULL /* &changes */  ) );
    DUF_TRACE( db, 0, "(%d) %s", r, sql );
  }

#  ifndef DUF_SQL_TTABLES_TEMPORARY
  if ( DUF_CONFIGG( db.tempo.fpath ) )
  {
    static const char *sql = "ATTACH DATABASE '" DUF_ATTACH_TTABLES_PATTERN "temp.db' AS " DUF_DBTEMPALIAS;

    DOR( r, duf_eval_sql_one( sql, ( duf_ufilter_t * ) NULL, ( duf_yfilter_t * ) NULL, DUF_DBTEMPALIAS, NULL /* &changes */  ) );
    DUF_TRACE( db, 0, "(%d) %s", r, sql );
  }
#  endif


#endif

/* TODO : part to only after possible tables creation */
  DOR( r, duf_eval_sqlsq( &sql_beginning_common, 0, NULL /* title */ , ( duf_ufilter_t * ) NULL, ( duf_yfilter_t * ) NULL, NULL /* selected.db */  ) ); /* PRAGMAs etc. */
  DEBUG_ENDR( r );
}

int
duf_main_db_open( duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );

  assert( pdi );
  assert( !pdi->attached_copy );
  /* global_status.db_opened_name */

  DUF_TRACE( db, 5, "@@@@global_status.db_attached_selected:%s", global_status.db_attached_selected );
  DUF_TRACE( db, 5, "@@@@global_status.db_opened_name:%s => CFG->db.main.name:%s", global_status.db_opened_name, DUF_CONFIGGSP( db.main.name ) );

  /* DUF_TRACE( temp, 0, "db:%s : %s", DUF_CONFIGGS(db.main.name), global_status.db_opened_name ); */
  if ( global_status.db_opened_name && 0 != strcmp( DUF_CONFIGGSP( db.main.name ), global_status.db_opened_name ) )
  {
    DUF_TRACE( db, 0, "@@@@autoclose db %s => %s", global_status.db_opened_name, DUF_CONFIGGSP( db.main.name ) );
    DUF_TRACE( db, 0, "@@@@global_status.db_attached_selected:%s", global_status.db_attached_selected );
    DOR( r, duf_main_db_close( pdi, 0 ) );
    DUF_TRACE( db, 0, "@@@@global_status.db_attached_selected:%s", global_status.db_attached_selected );
  }
  DUF_TRACE( db, 5, "@@@@global_status.db_attached_selected:%s", global_status.db_attached_selected );
#if 0
  if ( !DUF_CONFIGG( db.opened ) )
#else
  if ( !global_status.db_opened_name )
#endif
  {
    assert( !global_status.db_attached_selected );
    DORF( r, duf_main_db_locate );
    DORF( r, duf_main_db_optionally_remove_files );

    DUF_TRACE( db, 0, "@@@@@to open db %s [%s]", DUF_CONFIGGSP( db.main.name ), DUF_CONFIGGS( db.main.fpath ) );
    if ( DUF_CONFIGGS( db.main.fpath ) )
    {
      if ( !DUF_ACTG_FLAG( create_database ) )
      {
        struct stat st;
        int ry = 0;

        ry = stat( DUF_CONFIGGS( db.main.fpath ), &st );
        if ( ry < 0 )
          DUF_MAKE_ERRORM( r, DUF_ERROR_STAT, DUF_CONFIGGS( db.main.fpath ) );
      }
      DORF( r, duf_sql_open, DUF_CONFIGGS( db.main.fpath ) );
    }
    else
    {
      DUF_MAKE_ERROR( r, DUF_ERROR_PTR );
    }
#if 0
    DUF_CONFIGWN( db.opened, ( DUF_NOERROR( r ) ) );
#endif
    if ( DUF_NOERROR( r ) )
      global_status.db_opened_name = mas_strdup( DUF_CONFIGGSP( db.main.name ) );
    DORF( r, duf_main_db_tune );
    DORF( r, duf_main_db_pre_action );
    DUF_TRACE( pdi, 0, "DUF_CONFIGG( scn.pdi ) %s", duf_levinfo_path( DUF_CONFIGG( scn.pdi ) ) );
#if 0
    // removed 20151026.200517 - BAD here
    DOR( r, duf_pdi_reinit_min( DUF_CONFIGG( scn.pdi ) ) );
#endif
    DUF_CLEAR_ERROR( r, DUF_ERROR_NOT_IN_DB );
    /* if ( DUF_NOERROR( r ) ) */
    /*   r++; (* ???? *)       */
  }
  {
    duf_depthinfo_t *pdis;

    pdis = global_status.pdilist;
    while ( pdis && pdis != pdi )
      pdis = pdis->next;
    if ( !pdis )
    {
      assert( !pdi->next );
      pdi->next = global_status.pdilist;
      assert( !pdi->attached_copy );
      global_status.pdilist = pdi;
      DUF_TRACE( temp, 20, "@  link (%-14p=>%-14p) pdi:%-14p => %-14p", global_status.pdilist, global_status.pdilist->next, pdi, pdi->next );
    }
  }
  DEBUG_ENDR( r );
}

int
duf_main_db_close( duf_depthinfo_t * pdi DUF_UNUSED, int ra )
{
  DEBUG_STARTR( r );
  assert( pdi );
  assert( !pdi->attached_copy );
  r = ra;
#if 0
  if ( DUF_CONFIGG( db.opened ) )
#else
  if ( global_status.db_opened_name )
#endif
  {
    int rt = 0;
    duf_depthinfo_t *pdis = NULL;

    if ( pdi->next )
    {
      duf_depthinfo_t *prev_pdis = NULL;

      pdis = global_status.pdilist;
      while ( pdis && pdis != pdi )
      {
        prev_pdis = pdis;
        pdis = pdis->next;
      }
      if ( pdis )
      {
        if ( prev_pdis )
          prev_pdis->next = pdis->next;
        else
          global_status.pdilist = pdis->next;
        DUF_TRACE( temp, 20, "@unlink (%-14p=>%-14p) pdi:%-14p => %-14p", global_status.pdilist, global_status.pdilist->next, pdi, pdi->next );
        pdis->next = NULL;
      }
    }
    if ( pdis && !global_status.pdilist ) /* close only if opened for this pdi */
    {
      DUF_TRACE( db, 0, "@@@@closing db %s", global_status.db_opened_name );
#if 0
      {
#  ifdef MAS_SPLIT_DB
        if ( DUF_CONFIGG( db.adm.fpath ) )
        {
          static const char *sql = "DETACH DATABASE 'adm'";

          DUF_TRACE( explain, 0, "detach adm database %s", DUF_CONFIGG( db.adm.fpath ) );
          DUF_SQL_START_STMT_NOPDI( sql, r, pstmt );
          DUF_SQL_STEP( r, pstmt );
          DUF_SQL_END_STMT_NOPDI( r, pstmt );
        }
        if ( DUF_CONFIGG( db.tempo.fpath ) )
        {
          static const char *sql = "DETACH DATABASE 'tempo'";

          DUF_TRACE( explain, 0, "detach tempo database %s -- %s", DUF_CONFIGG( db.tempo.fpath ), sql );
          DUF_SQL_START_STMT_NOPDI( sql, r, pstmt );
          DUF_SQL_STEP( r, pstmt );
          DUF_SQL_END_STMT_NOPDI( r, pstmt );
        }
#  endif
      }
#endif
      /* don't DOR it directly! call allways! */
      DORF( rt, duf_sql_close );
      T( "@duf_sql_close: rt:%d", rt );
      if ( r == 0 && rt < 0 )
        DOR( r, rt );
#if 0
      DUF_CONFIGWN( db.opened, !( DUF_NOERROR( r ) ) );
#else
      mas_free( global_status.db_opened_name );
      global_status.db_opened_name = NULL;
#endif
      mas_free( global_status.db_attached_selected );
      global_status.db_attached_selected = NULL;
    }
  }
  DEBUG_ENDR( r );
}

SR( TOP, main_db, int argc DUF_UNUSED, char **argv DUF_UNUSED )
{
  if ( DUF_ACTG_FLAG( info ) )
    CR( main_db_info );
  
  CR( main_db_close, DUF_CONFIGG( scn.pdi ), QERRIND ); /* [@] */
  ER( TOP, main_db, int argc DUF_UNUSED, char **argv DUF_UNUSED );
}
