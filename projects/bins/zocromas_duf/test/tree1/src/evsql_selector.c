/* 20150819.134056 */
#define DUF_SQL_PDI_STMT

#include <string.h>
#include <assert.h>
#include <unistd.h>

#include <sys/types.h>
#include <unistd.h>


#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_config_ref.h"

#include "duf_maintenance.h"

#include "duf_sql.h"

#include "duf_hook_types.h"
#include "duf_scan_types.h"

#include "std_fieldsets.h"
#include "std_selectors.h"
/* ###################################################################### */
#include "evsql_selector.h"
/* ###################################################################### */

static const char *
duf_find_std_fieldset( const char *fieldset_name, duf_node_type_t type )
{
  const char *set = NULL;
  duf_fieldset_t *fs;

  fs = all_fieldsets;
  while ( fs && fs->name )
  {
    if ( type == fs->type && 0 == strcmp( fieldset_name, fs->name ) )
    {
      set = fs->set;
      break;
    }
    fs++;
  }
  return set;
}

static const char *
duf_find_std_selector( const char *selector_name, duf_node_type_t type )
{
  const char *selector = NULL;
  duf_selector_t *sl;

  sl = all_selectors;
  while ( sl && sl->name )
  {
    if ( type == sl->type && 0 == strcmp( selector_name, sl->name ) )
    {
      selector = sl->selector2;
      break;
    }
    sl++;
  }
  return selector;
}


static const char *
duf_unref_fieldset( const char *fieldset, duf_node_type_t type, int *pr )
{
  if ( fieldset )
  {
    if ( *fieldset == '#' )
    {
      const char *fsn;

      fsn = fieldset + 1;
      fieldset = duf_find_std_fieldset( fsn, type );
      if ( !fieldset && pr )
        DUF_MAKE_ERROR( *pr, DUF_ERROR_PTR );
    }
  }
  return fieldset;
}

static const char *
duf_unref_selector( const char *selector, duf_node_type_t type, int *pr )
{
  if ( selector )
  {
    if ( *selector == '#' )
    {
      const char *sln;

      sln = selector + 1;
      selector = duf_find_std_selector( sln, type );
      if ( !selector && pr )
        DUF_MAKE_ERROR( *pr, DUF_ERROR_PTR );
    }
  }
  return selector;
}


static const char *
duf_xsdb_getvar( const char *name, const char *arg )
{
  char *str = NULL;
  static char buf[1024];

  if ( 0 == strcmp( name, "PDI_NAME" ) )
    str = ( char * ) arg;
  else if ( 0 == strcmp( name, "DB_PATH" ) )
    str = ( char * ) duf_config->db.dir;
  else if ( 0 == strcmp( name, "PID" ) )
  {
    str = ( char * ) buf;
    snprintf( buf, sizeof( buf ), "%u", getpid(  ) );
  }
  else if ( 0 == strcmp( name, "DB_NAME" ) )
  {
    str = duf_config->db.main.name;
    T( "@@@@@@name:%s : %s", name, str );
  }
  DUF_TRACE( temp, 10, "@@%s :: %s => %s", name, arg, str );
  return str;
}

char *
duf_expand_sql( const char *sql, const char *dbname )
{
  char *nsql;

  nsql = mas_expand_string_cb_arg( sql, duf_xsdb_getvar, dbname );
  /* DUF_TRACE( temp, 0, "@@@SQL:%s => %s", sql, nsql ); */
  return nsql;
}

/* 20150819.133515 */
char *
duf_selector2sql( const duf_sql_set_t * sql_set, const char *selected_db, int *pr )
{
#define DUF_SELECTOR selector2
  char *sql = NULL;

  if ( sql_set->fieldset && sql_set->DUF_SELECTOR )
  {
    /* if ( 0 == strncmp( sql_set->DUF_SELECTOR, "SELECT", 6 ) )             */
    /* {                                                                  */
    /*   char *sql3;                                                      */
    /*                                                                    */
    /*   assert( 0 );                                                     */
    /*   sql3 = duf_sql_mprintf( sql_set->DUF_SELECTOR, sql_set->fieldset ); */
    /*   sql = mas_strdup( sql3 );                                        */
    /*   mas_free( sql3 );                                                */
    /* }                                                                  */
    /* else                                                               */
    {
      int has_where = 0;
      const char *selector = NULL;
      const char *fieldset = NULL;

      fieldset = duf_unref_fieldset( sql_set->fieldset, sql_set->type, pr );
      selector = duf_unref_selector( sql_set->DUF_SELECTOR, sql_set->type, pr );
      if ( selector && fieldset )
      {
        sql = mas_strdup( "SELECT " );
        sql = mas_strcat_x( sql, fieldset );
        sql = mas_strcat_x( sql, " " );

        if ( sql_set->expand_sql )
        {
          char *tsql;

          tsql = duf_expand_sql( selector, selected_db );
          sql = mas_strcat_x( sql, tsql );
          mas_free( tsql );
        }
        else
        {
          sql = mas_strcat_x( sql, selector );
        }
#if 1
        if ( sql_set->filter )
        {
          if ( has_where )
            sql = mas_strcat_x( sql, " aND " );
          else
            sql = mas_strcat_x( sql, " wHERE " );
          has_where = 1;
          sql = mas_strcat_x( sql, sql_set->filter );
        }
#endif
#if 1
        if ( sql_set->matcher )
        {
          if ( has_where )
            sql = mas_strcat_x( sql, " AND " );
          else
            sql = mas_strcat_x( sql, " WHERE " );
          has_where = 1;
          sql = mas_strcat_x( sql, sql_set->matcher );
        }
#endif
      }
    }
  }
  else
    DUF_SHOW_ERROR( "Bad arg" );
  return sql;
#undef  DUF_SELECTOR
}

/* 20150819.133525 */
char *
duf_selector_total2sql( const duf_sql_set_t * sql_set, const char *selected_db, int *pr )
{
/* #define DUF_SELECTOR selector_total2 */
#define DUF_SELECTOR selector2
  char *sql = NULL;

  assert( sql_set );
  assert( sql_set->DUF_SELECTOR );
  if ( sql_set->DUF_SELECTOR )
  {
    /* if ( 0 == strncmp( sql_set->DUF_SELECTOR, "SELECT", 6 ) )             */
    /* {                                                                           */
    /*   char *sql3;                                                               */
    /*                                                                             */
    /*   assert( 0 );                                                              */
    /*   sql3 = duf_sql_mprintf( sql_set->DUF_SELECTOR, sql_set->fieldset ); */
    /*   sql = mas_strdup( sql3 );                                                 */
    /*   mas_free( sql3 );                                                         */
    /* }                                                                           */
    /* else                                                                        */
    {
      int has_where = 0;
      int has_group = 0;
      int has_order = 0;
      const char *selector = NULL;

      selector = duf_unref_selector( sql_set->DUF_SELECTOR, sql_set->type, pr );
      if ( selector )
      {
        sql = mas_strdup( "SELECT " );
        sql = mas_strcat_x( sql, "COUNT(" );
        sql = mas_strcat_x( sql, sql_set->count_aggregate ? sql_set->count_aggregate : "*" );
        sql = mas_strcat_x( sql, ") AS nf" );
        sql = mas_strcat_x( sql, " " );

        if ( sql_set->expand_sql )
        {
          char *tsql;

          tsql = duf_expand_sql( selector, selected_db );
          sql = mas_strcat_x( sql, tsql );
          mas_free( tsql );
        }
        else
        {
          sql = mas_strcat_x( sql, selector );
	  T("SQL:%s", sql);
        }


#if 1
        if ( sql_set->filter )
        {
          if ( has_where )
            sql = mas_strcat_x( sql, " AND " );
          else
            sql = mas_strcat_x( sql, " WHERE " );
          has_where = 1;
          sql = mas_strcat_x( sql, sql_set->filter );
        }
#endif
#if 0
        if ( sql_set->matcher )
        {
          if ( has_where )
            sql = mas_strcat_x( sql, " AnD " );
          else
            sql = mas_strcat_x( sql, " WhERE " );
          has_where = 1;
          sql = mas_strcat_x( sql, sql_set->matcher );
        }
#endif
#if 1
        if ( sql_set->group )
        {
          if ( has_group )
            sql = mas_strcat_x( sql, "," );
          else
            sql = mas_strcat_x( sql, " GROUP BY " );
          has_group = 1;
          sql = mas_strcat_x( sql, sql_set->order );
        }
#endif
#if 1
        if ( sql_set->order )
        {
          if ( has_order )
            sql = mas_strcat_x( sql, "," );
          else
            sql = mas_strcat_x( sql, " ORDER BY " );
          has_order = 1;
          sql = mas_strcat_x( sql, sql_set->order );
        }
#endif
      }

    }
    DUF_TRACE( select, 0, "TOTAL: %s", sql );
  }
  else
  {
    DUF_SHOW_ERROR( "Bad arg" );
  }
  return sql;
#undef  DUF_SELECTOR
}
