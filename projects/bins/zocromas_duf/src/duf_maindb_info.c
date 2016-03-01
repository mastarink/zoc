#include <assert.h>                                                  /* assert */

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>                                  /* mas_(malloc|free|strdup); etc. ♣ */
#include <mastar/tools/mas_arg_tools.h>                              /* mas_strcat_x; etc. ♣ */
#include <mastar/tools/mas_time.h>                                   /* mas_(|double_|xlocal|xgm|xvstrf|xvstrftime_|tstrflocal|strflocal|strfgm)time ; strtime2long; etc. ♣ */
#include <mastar/sqlite/mas_sqlite_const.h>

#include "duf_tracen_defs.h"                                         /* MAST_TRACE ♠ */
#include "duf_errorn_defs.h"                                         /* DUF_NOERROR; DUF_CLEAR_ERROR; DUF_E_(LOWER|UPPER); DUF_TEST_R ... ♠ */

#include "duf_start_end.h"                                           /* DUF_STARTR ; DUF_ENDR ♠ */
#include "duf_dodefs.h"                                              /* DOR ♠ */

#include "duf_printn_defs.h"                                         /* DUF_PRINTF etc. ♠ */

#include "duf_config.h"                                              /* duf_get_config ♠ */
#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ♠ */
#include "duf_config_ref.h"
#include "duf_config_output_util.h"

#include "duf_sql_defs.h"                                            /* DUF_SQL_IDFIELD etc. ♠ */

#include "duf_sql_prepared.h"
#include "duf_sql_positional.h"
#include "sql_tables_defs.h"
#include "duf_sql_stmt_defs.h"                                       /* DUF_SQL_BIND_S_OPT etc. ♠ */

/* ###################################################################### */
#include "duf_maindb_info.h"
/* ###################################################################### */

static unsigned long *
duf_info_from_db( int count, const char *sql )
{
  unsigned long *tuple = NULL;
  int rpr = 0;

  if ( count > 0 )
  {

    DUF_SQL_START_STMT_NOPDI( sql, rpr, pstmt );

    DUF_SQL_STEP( rpr, pstmt );
    if ( DUF_IS_ERROR_N( rpr, DUF_SQL_ROW ) )
    {
      tuple = mas_malloc( count * sizeof( unsigned long ) );

      for ( int iv = 0; iv < count; iv++ )
      {
        long long val;

        val = duf_sql_column_long_long( pstmt, iv );
        tuple[iv] = val;
      }
    }
    DUF_SQL_END_STMT_NOPDI( rpr, pstmt );
  }

  return tuple;
}

int
duf_main_db_info( void )
{
  typedef struct
  {
    int count;
    const char *sql;
    const char *title;
    const char *labels[20];
  } duf_infodata_t;

  DUF_STARTR( r );
  if ( duf_config->opt.act.v.flag.info )
  {
    duf_infodata_t infod[] = {
#if 0
      {.title = "paths",.count = 5,.labels = {"num of paths", "max num dirs", "max num files", "@min inow", "@max inow", NULL}
       ,.sql =
       "SELECT  COUNT(*), max(numdirs), max(numfiles), STRFTIME('%s',min(pt.inow)), STRFTIME('%s',max(pt.inow)) FROM " DUF_SQL_TABLES_PATHS_FULL
       " as pt LEFT JOIN pathtot_dirs AS td ON (td.Pathid=pt." DUF_SQL_IDFIELD ") LEFT JOIN pathtot_files AS tf ON (tf.Pathid=pt." DUF_SQL_IDFIELD
       ")"}
      ,
#endif
      {.title = "datas with reasonable date",.count = 5,.labels = {"#", "@min mtim", "@max mtim", "@min inow", "@max inow", NULL}
       ,.sql =                                                       /* */
       "SELECT COUNT(*), STRFTIME('%s',min(mtim)), STRFTIME('%s',max(mtim)), STRFTIME('%s',min(inow)), STRFTIME('%s',max(inow)) " /* */
       " FROM " DUF_SQL_TABLES_FILEDATAS_FULL                        /* */
       " WHERE cast(STRFTIME('%s',mtim) as integer)>320000000 and cast(STRFTIME('%s',mtim) as integer)<1600000000"}
      ,
      {.title = "zero files",.count = 1,.labels = {"#", NULL}
       ,.sql =
       "SELECT COUNT(*) FROM " DUF_SQL_TABLES_FILENAMES_FULL " AS fn JOIN filedatas AS fd ON (fn.dataid=fd." DUF_SQL_IDFIELD
       ") JOIN " DUF_SQL_TABLES_SIZES_FULL " as sz ON (sz.size=fd.size) WHERE fd.size=0"}
      ,
      {.title = "nonzero files",.count = 1,.labels = {"#", NULL}
       ,.sql =
       "SELECT COUNT(*) FROM " DUF_SQL_TABLES_FILENAMES_FULL " AS fn JOIN filedatas AS fd ON (fn.dataid=fd." DUF_SQL_IDFIELD
       ") JOIN " DUF_SQL_TABLES_SIZES_FULL " as sz ON (sz.size=fd.size) WHERE fd.size>0"}
      ,
      {.title = "names",.count = 3,.labels = {"#", "@min inow", "@max inow", NULL}
       ,.sql = "SELECT COUNT(*), STRFTIME('%s',min(inow)), STRFTIME('%s',max(inow)) FROM " DUF_SQL_TABLES_FILENAMES_FULL ""}
      ,
      {.title = "names 2",.count = 1,.labels = {"#", NULL}
       ,.sql =
       "SELECT COUNT(*) FROM " DUF_SQL_TABLES_FILENAMES_FULL " AS fn JOIN filedatas AS fd ON (fn.dataid=fd." DUF_SQL_IDFIELD ") JOIN "
       DUF_SQL_TABLES_MD5_FULL " AS md ON (md." DUF_SQL_IDFIELD "=fd.md5id)"}
      ,
      {.title = "distinct md5id",.count = 1,.labels = {"#", NULL}
       ,.sql =
       "SELECT COUNT(distinct md5id) FROM " DUF_SQL_TABLES_FILENAMES_FULL " AS fn JOIN filedatas AS fd ON (fn.dataid=fd." DUF_SQL_IDFIELD ") JOIN "
       DUF_SQL_TABLES_MD5_FULL " AS md ON (md." DUF_SQL_IDFIELD "=fd.md5id)"}
      ,
      {.title = "n/z Sizes",.count = 5,.labels = {"#", "min size", "max size", "@min inow", "@max inow", NULL}
       ,.sql =
       "SELECT COUNT(*), min(size), max(size), STRFTIME('%s',min(inow)), STRFTIME('%s',max(inow)) FROM " DUF_SQL_TABLES_SIZES_FULL " WHERE size>0"}
      ,
      {.title = "dup sets by size",.count = 1,.labels = {"#", NULL}
       ,.sql = "SELECT COUNT(*) FROM " DUF_SQL_TABLES_SIZES_FULL " WHERE dupzcnt>1"}
      ,
      {.title = "Md5's",.count = 1,.labels = {"#", "@min inow", "@max inow", NULL}
       ,.sql = "SELECT COUNT(*), STRFTIME('%s',min(md.inow)), STRFTIME('%s',max(md.inow)) FROM " DUF_SQL_TABLES_MD5_FULL " AS md"}
      ,
      {.title = "dup sets by Md5's",.count = 1,.labels = {"#", NULL}
       ,.sql = "SELECT COUNT(*) FROM " DUF_SQL_TABLES_MD5_FULL " AS md WHERE dup5cnt>1"}
      ,
      {.title = "Mime's",.count = 3,.labels = {"#", "@min inow", "@max inow", NULL}
       ,.sql = "SELECT COUNT(*), STRFTIME('%s',min(mi.inow)), STRFTIME('%s',max(mi.inow)) FROM " DUF_SQL_TABLES_MIME_FULL " AS mi"}
      ,
      {.title = "dup sets by Mime's",.count = 1,.labels = {"#", NULL}
       ,.sql = "SELECT COUNT(*) FROM " DUF_SQL_TABLES_MIME_FULL " AS mi WHERE dupmimecnt>1"}
      ,
      {.title = "exif's",.count = 3,.labels = {"#", "@min date_time", "@max date_time", "@min inow", "@max inow", NULL}
       ,.sql =
       "SELECT COUNT(*), STRFTIME('%s',min(date_time)), STRFTIME('%s',max(date_time)), STRFTIME('%s',min(inow)), STRFTIME('%s',max(inow)) FROM exif WHERE  cast(STRFTIME('%s',date_time) as integer)>0"}
      ,
    };
    MAST_TRACE( explain, 0, "-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-" );
    MAST_TRACE( explain, 0, "-=-=-=-=-=-=-=-=-    =-=-=-=-=-=-=-=-=-" );
    MAST_TRACE( explain, 0, "-=-=-=-=-=-                 -=-=-=-=-=-" );
    MAST_TRACE( explain, 0, "-=-=-=                         =-==--=-=-=-" );
    MAST_TRACE( explain, 0, "info                              info info info" );
    MAST_TRACE( explain, 0, "-=-=-=                      -=-=-=-=-=-" );
    MAST_TRACE( explain, 0, "-=-=-=-=-=-=-       -=-=-=-=-=-=-=-=-=-" );
    MAST_TRACE( explain, 0, "-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-" );
  /*
     SELECT  COUNT(*), min(pt.inow), max(pt.inow), max(numdirs), max(numfiles) FROM " DUF_SQL_TABLES_PATHS_FULL " as pt LEFT JOIN pathtot_dirs AS td ON (td.Pathid=pt." DUF_SQL_IDFIELD ") LEFT JOIN pathtot_files AS tf ON (tf.Pathid=pt." DUF_SQL_IDFIELD ") ;
     SELECT COUNT(*), datetime(min(mtim), 'unixepoch'), datetime(max(mtim), 'unixepoch'), min(inow), max(inow) FROM filedatas WHERE mtim>320000000 and mtim<1600000000
     SELECT COUNT(*) FROM " DUF_SQL_TABLES_FILENAMES_FULL " AS fn JOIN filedatas AS fd ON (fn.dataid=fd." DUF_SQL_IDFIELD ") JOIN " DUF_SQL_TABLES_SIZES_FULL " as sz ON (sz.size=fd.size) WHERE fd.size>0;
     SELECT COUNT(*) FROM " DUF_SQL_TABLES_FILENAMES_FULL " AS fn JOIN filedatas AS fd ON (fn.dataid=fd." DUF_SQL_IDFIELD ") JOIN " DUF_SQL_TABLES_SIZES_FULL " as sz ON (sz.size=fd.size) WHERE fd.size=0;
     SELECT COUNT(*), min(inow), max(inow)                                                                     FROM " DUF_SQL_TABLES_FILENAMES_FULL ";
     SELECT COUNT(*), max(size), min(size), min(inow), max(inow)                                               FROM " DUF_SQL_TABLES_SIZES_FULL "; 
     SELECT COUNT(*) FROM " DUF_SQL_TABLES_SIZES_FULL " WHERE dupzcnt>1;
     SELECT COUNT(*) FROM " DUF_SQL_TABLES_MD5_FULL " AS md;   
     SELECT COUNT(*)              FROM " DUF_SQL_TABLES_FILENAMES_FULL " AS fn JOIN filedatas AS fd ON (fn.dataid=fd." DUF_SQL_IDFIELD ") JOIN " DUF_SQL_TABLES_MD5_FULL " AS md ON (md." DUF_SQL_IDFIELD "=fd.md5id);
     SELECT COUNT(distinct md5id) FROM " DUF_SQL_TABLES_FILENAMES_FULL " AS fn JOIN filedatas AS fd ON (fn.dataid=fd." DUF_SQL_IDFIELD ") JOIN " DUF_SQL_TABLES_MD5_FULL " AS md ON (md." DUF_SQL_IDFIELD "=fd.md5id);
   */
    for ( unsigned iop = 0; iop < sizeof( infod ) / sizeof( infod[0] ); iop++ )
    {
      unsigned long *tuple;
      int nolab = 0;

    /* MAST_TRACE( explain, 0, "#%d. SQL:'%s'", iop, infod[iop].sql ); */
      tuple = duf_info_from_db( infod[iop].count, infod[iop].sql );
      DUF_PRINTF( 0, ".#%2d. [ %30s ] -- ", iop, infod[iop].title );
      for ( int iv = 0; iv < infod[iop].count; iv++ )
      {
        const char *label;

        if ( !nolab && !infod[iop].labels[iv] )
        {
          nolab = 1;
          label = NULL;
        }
        else
        {
          label = infod[iop].labels[iv];
        }
        if ( label )
        {
          time_t tv;

          tv = ( time_t ) tuple[iv];
          if ( *label == '@' )
          {
            label++;
            char buf[64];

            mas_strfgmtime( buf, sizeof( buf ), "%F %T", &tv );
          /* DUF_PRINTF( 0, ". %7s: %6lu:%15s;  ", label, tuple[iv], buf ); */
            DUF_PRINTF( 0, ". %s: %15s;  ", label, buf );
          }
          else
          {
            DUF_PRINTF( 0, ". %s: %6lu;  ", label, tuple[iv] );
          }
        }
        else
          DUF_PRINTF( 0, ". %6lu;  ", tuple[iv] );
      }
      DUF_PRINTF( 0, ";" );
      mas_free( tuple );
    }
    MAST_TRACE( explain, 0, "-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-" );
  }
  DUF_ENDR( r );
}
