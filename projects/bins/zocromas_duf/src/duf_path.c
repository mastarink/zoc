#define DUF_SQL_PDI_STMT

#include <string.h>

#include <sys/types.h>
#include <unistd.h>

#include <fcntl.h>              /* Definition of AT_* constants */
#include <sys/stat.h>

#include <assert.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_types.h"

#include "duf_utils.h"
#include "duf_config.h"

#include "duf_pdi.h"
#include "duf_levinfo.h"

#include "duf_sql_field.h"

#include "duf_sql.h"
#include "duf_sql2.h"

#include "duf_dbg.h"

/* ###################################################################### */
#include "duf_path.h"
/* ###################################################################### */


char *
duf_join_path( const char *path, const char *fname )
{
  char *fpath = NULL;

  DEBUG_START(  );
  if ( fname )
  {
    if ( path )
      fpath = mas_strdup( path );
    if ( *fname != '/' && !( fpath && *fpath == '/' && fpath[1] == 0 ) )
      fpath = mas_strcat_x( fpath, "/" );
    fpath = mas_strcat_x( fpath, fname );
  }
  DEBUG_ENDS( fpath );
  return fpath;
}

#ifdef DUF_COMPILE_EXPIRED
typedef struct
{
  char *name;
  unsigned long long parentid;
} name_parid_udata_t;

/* will be static! */
/* 
 * this is callback of type:duf_sel_cb_t (second range; ; sel_cb)
 * */
int
duf_sel_cb_name_parid( duf_record_t * precord, void *sel_cb_udata, duf_str_cb_t str_cb_unused, void *str_cb_udata_unused,
                       duf_depthinfo_t * pdi, duf_scan_callbacks_t * sccb_unused )
{
  int r = 0;
  name_parid_udata_t *pud;

  DEBUG_START(  );
  pud = ( name_parid_udata_t * ) sel_cb_udata;
  if ( precord->nrow == 0 )
  {
    DUF_UFIELD( parentid );
    DUF_SFIELD( dirname );
    pud->parentid = parentid;
    /* pud->parentid = duf_sql_ull_by_name( "parentid", precord, 0 ); */


    pud->name = mas_strdup( dirname );


    /* pud->parentid = strtoll( precord->presult[0], NULL, 10 ); */
    /* pud->name = mas_strdup( precord->presult[1] );            */
  }
  /* fprintf( stderr, "pud a:%d nrow:%d\n", precord->presult ? 1 : 0, nrow ); */
  /* fprintf( stderr, "pud b: %s :: %s\n", precord->presult[0], precord->presult[1] ); */
  DEBUG_END(  );
  return r;
}

char *
duf_pathid_to_path_dh( unsigned long long dirid, duf_dirhandle_t * pdh_pathid, const duf_depthinfo_t * pdi, int *pr )
{
  char *path = NULL;
  int r = 0;
  int openat = !duf_config->cli.noopenat && pdh_pathid;

  int test_o = duf_config->nopen;
  int test_c = duf_config->nclose;

  DEBUG_START(  );
  duf_check_dh( "Before" );


  if ( dirid == 0 )
  {
    duf_dirhandle_t dh = {.dfd = 0 };
    path = NULL;
    if ( openat )
    {
      dh.dirid = dirid;
      r = duf_open_dh( &dh, "/" );
      DUF_TEST_R( r );
      if ( r >= 0 )
        *pdh_pathid = dh;
      r = 0;
    }
  }
  else
  {
    name_parid_udata_t pathdef = {.name = NULL,.parentid = 0 };

/* get parentid for dirid */
    r = duf_sql_select( duf_sel_cb_name_parid, &pathdef, STR_CB_DEF, STR_CB_UDATA_DEF, ( duf_depthinfo_t * ) NULL,
                        ( duf_scan_callbacks_t * ) NULL /*  sccb */ ,
                        "SELECT parentid, dirname " " FROM duf_paths WHERE id=%llu", dirid );
    DUF_TEST_R( r );
    if ( r >= 0 )
    {
      if ( pathdef.name )
      {
        if ( pathdef.parentid >= 0 )
        {
          char *parent = NULL;
          duf_dirhandle_t dhu = {.dfd = 0 };
          duf_dirhandle_t dh = {.dfd = 0 };
          duf_dirhandle_t *pdhu_pathid = NULL;

          if ( openat )
            pdhu_pathid = &dhu;

          parent = duf_pathid_to_path_dh( pathdef.parentid, pdhu_pathid, pdi, &r ); /* open!! */
          DUF_TEST_R( r );
          path = duf_join_path( parent, pathdef.name );
          if ( r >= 0 && openat )
          {
            r = duf_openat_dh( &dh, pdhu_pathid, pathdef.name, 0 /* asfile */  );
            DUF_TEST_R( r );
            if ( r >= 0 )
              *pdh_pathid = dh;
            r = 0;
          }
          if ( openat && pdhu_pathid && pdhu_pathid->dfd )
            duf_close_dh( pdhu_pathid );
          if ( parent )
            mas_free( parent );
        }
      }
    }
    if ( pathdef.name )
      mas_free( pathdef.name );
    DEBUG_ENDS( path );
  }
  if ( pr )
    *pr = r;
  {
    int dopen = ( duf_config->nopen - duf_config->nclose ) - ( test_o - test_c );

    assert( dopen == 1 || !openat );
  }
  duf_check_dh( "After" );
  return path;
}

char *
duf_pathid_to_path_s( unsigned long long dirid, const duf_depthinfo_t * pdi, int *pr )
{
  int r = 0;
  char *s = NULL;

  s = duf_pathid_to_path_dh( dirid, ( duf_dirhandle_t * ) NULL, pdi, &r );
  DUF_TEST_R( r );
  if ( pr )
    *pr = r;
  return s;
}

#endif


static char *
duf_pathid_to_path2_in( duf_sqlite_stmt_t * pstmt, unsigned long long dirid, const duf_depthinfo_t * pdi, int *pr )
{
  char *path = NULL;
  int r = 0;

  DEBUG_START(  );
  if ( dirid > 0 )
  {
/* get parentid for dirid */

    if ( pstmt )
    {
      duf_sql_reset( pstmt );
      if ( r >= 0 )
        r = duf_sql_bind_long_long( pstmt, ":dirid", dirid );
      if ( r >= 0 )
        r = duf_sql_step( pstmt );
      if ( r == DUF_SQL_ROW )
      {
        char *tail;

        DUF_UFIELD2( parentid );
        DUF_SFIELD2( dirname );
        tail = mas_strdup( dirname );
        if ( parentid >= 0 )
        {
          char *parent = NULL;

          parent = duf_pathid_to_path2_in( pstmt, parentid, pdi, &r ); /* open!! */
          DUF_TEST_R( r );
          path = duf_join_path( parent, tail );
          /* DUF_ERROR( "%llu (%s): %s @ %llu => %s", dirid, parent, tail, parentid, path ); */

          if ( tail )
            mas_free( tail );
          if ( parent )
            mas_free( parent );
        }
      }
      DUF_TEST_R( r );
      DEBUG_ENDS( path );
    }
  }
  if ( pr )
    *pr = r;
  return path;
}

char *
duf_pathid_to_path2( unsigned long long dirid, const duf_depthinfo_t * pdi, int *pr )
{
  char *path = NULL;
  int r = 0;
  duf_sqlite_stmt_t *pstmt = NULL;

  {
    const char *sql = "SELECT parentid, dirname FROM duf_paths WHERE id=:dirid";

    if ( r >= 0 && sql )
      r = duf_sql_prepare( sql, &pstmt );
  }
  path = duf_pathid_to_path2_in( pstmt, dirid, pdi, &r );
  path = mas_strcat_x( path, "/" );

  {
    int rf = duf_sql_finalize( pstmt );

    pstmt = NULL;
    DUF_TEST_R( rf );
    DUF_TRACE( action, 0, "FINALIZE %s;", rf < 0 ? "FAIL" : "" );

    if ( r >= 0 || r == DUF_SQL_DONE )
      r = rf;
    DUF_TEST_R( r );
  }
  if ( pr )
    *pr = r;
  return path;
}

/* insert path into db; return id */
unsigned long long
duf_insert_path_uni2( duf_depthinfo_t * pdi, const char *dename, int ifadd, duf_levinfo_t * pli, dev_t dev_id, ino_t dir_ino,
                      unsigned long long parentid, int need_id, int *pr )
{
  unsigned long long dirid = 0;
  int r = 0;

  DEBUG_START(  );
  /* unsigned char c1 = ( unsigned char ) ( dename ? *dename : 0 ); */

  if ( dename /* && dev_id && dir_ino */  )
  {
    int changes = 0;

    if ( ifadd && !duf_config->cli.disable.insert )
    {
      static const char *sql =
            "INSERT OR IGNORE INTO duf_paths ( dev, inode, dirname, parentid) VALUES (:dev, :inode, :dirname, :parentid )";
      DUF_SQL_START_STMT( pdi, select_path, sql, r, pstmt );
      DUF_SQL_BIND_LL( dev, dev_id, r, pstmt );
      DUF_SQL_BIND_LL( inode, dir_ino, r, pstmt );
      DUF_SQL_BIND_S( dirname, dename, r, pstmt );
      DUF_SQL_BIND_LL( parentid, parentid, r, pstmt );
      DUF_SQL_STEP( r, pstmt );
      DUF_SQL_CHANGES( changes, r, pstmt );
      DUF_SQL_END_STMT( r, pstmt );
    }
    /* sql = NULL; */
    DUF_TRACE( current, 0, "<changes> : %d", changes );
    /* if ( r == DUF_SQL_CONSTRAINT ) */
    if ( need_id )
    {
      if ( ( r == DUF_SQL_CONSTRAINT || !r ) && !changes )
      {
        /* duf_sqlite_stmt_t *pstmt = NULL; */
        const char *sql = "SELECT duf_paths.id AS dirid, duf_paths.dirname " /*      */
              ", tf.numfiles AS nfiles, td.numdirs AS ndirs " /*      */
              " FROM duf_paths LEFT JOIN duf_pathtot_dirs AS td ON (td.pathid=duf_paths.id) " /*      */
              " LEFT JOIN duf_pathtot_files AS tf ON (tf.pathid=duf_paths.id) " /*      */
              " WHERE duf_paths.parentid=:dirid AND dirname=:dirname";

        DUF_SQL_START_STMT( pdi, select_path, sql, r, pstmt );
        DUF_SQL_BIND_LL( dirid, parentid, r, pstmt );
        DUF_SQL_BIND_S( dirname, dename, r, pstmt );
        DUF_SQL_STEP( r, pstmt );

        if ( r == DUF_SQL_ROW )
        {
          DUF_TRACE( current, 0, "<selected>" );
          dirid = duf_sql_column_long_long( pstmt, 0 );
          if ( need_id && !dirid )
          {
            DUF_ERROR( "no dirid by parentid=%llu and dename='%s'", parentid, dename );
          }
          assert( !need_id || dirid );
          if ( pli )
          {
            pli->dirid = dirid;
            pli->itemname = mas_strdup( duf_sql_column_string( pstmt, 1 ) );
            pli->numfile = duf_sql_column_long_long( pstmt, 2 );
            pli->numdir = duf_sql_column_long_long( pstmt, 3 );
          }
          r = 0;
        }
        else
        {
          DUF_TEST_R( r );
          DUF_TRACE( current, 0, "<NOT selected> (%d)", r );
        }
        DUF_SQL_END_STMT( r, pstmt );
        DUF_TEST_R( r );
        DUF_TRACE( collect, 1, "sometime inserted (SQLITE_OK) dirid=%llu:'%s'", dirid, dename );
      }
      else if ( !r /* assume SQLITE_OK */  )
      {
        dirid = duf_sql_last_insert_rowid(  );
        if ( need_id && !dirid )
          DUF_ERROR( "no dirid by parentid=%llu and dename='%s'", parentid, dename );
        assert( !need_id || dirid );
        DUF_TRACE( collect, 1, "inserted (SQLITE_OK) dirid=%llu:'%s'", dirid, dename );
      }
      DUF_TEST_R( r );
    }
  }
  else
  {
    DUF_ERROR( "Wrong data dename(%s) OR dev_id(%lu) OR dir_ino(%lu)", dename, dev_id, dir_ino );
    r = DUF_ERROR_DATA;
  }
  if ( pr )
    *pr = r;
  if ( need_id && !dirid )
  {
    DUF_ERROR( "no dirid by parentid=%llu and dename='%s'", parentid, dename );
  }
  DUF_TEST_R( r );
  /* assert( !need_id || dirid ); */
  DEBUG_ENDULL( dirid );
  return dirid;
}

unsigned long long
duf_real_path_to_pathid2( duf_depthinfo_t * pdi, const char *rpath, int ifadd, int need_id, int *pr )
{
  unsigned long long parentid = 0;
  char *real_path = mas_strdup( rpath );
  int r = 0;

  {
    int upfd = 0;
    char *dir = real_path;

    DUF_TRACE( path, 0, "%s PATHID for %s", ifadd ? "ADD" : "GET", real_path );
    while ( r >= 0 && dir && *dir )
    {
      char *edir = dir;
      const char *insdir;
      struct stat st_dir;

      DUF_TRACE( path, 0, "prepare [%s] under %llu", dir, parentid );
      while ( edir && *edir && *edir != '/' )
        edir++;
      if ( edir && *edir == '/' )
        *edir++ = 0;
      DUF_TRACE( path, 0, "prepared [%s] up to [%s] fd:%d", dir, edir, upfd );

      insdir = dir;
      if ( pdi )
      {
        if ( r >= 0 )
          r = duf_levinfo_down( pdi, 0, insdir, 0 /* ndirs */ , 0 /* nfiles */ , 0 /* is_leaf */  );
        if ( r >= 0 )
          r = duf_levinfo_openat_dh( pdi );
      }
      if ( pdi )
        upfd = duf_levinfo_dfd( pdi );
      else
      {
        const char *opendir;
        int fd = 0;

        if ( *insdir )
          opendir = insdir;
        else
          opendir = "/";
        fd = upfd;
        r = fstatat( fd, opendir, &st_dir, AT_SYMLINK_NOFOLLOW | AT_NO_AUTOMOUNT );
        upfd = openat( fd, opendir, O_DIRECTORY | O_NOFOLLOW | O_PATH | O_RDONLY );
        if ( fd )
          close( fd );
        fd = 0;
      }
      if ( r )
      {
        DUF_ERROR( "No such entry %s\n", real_path );
        break;
      }
      else
      {
        DUF_TRACE( path, 0, "to insert [%s]", insdir ? insdir : "/" );
        parentid = duf_insert_path_uni2( pdi, insdir, ifadd, duf_levinfo( pdi ), st_dir.st_dev, st_dir.st_ino, parentid, 1 /*need_id */ ,
                                         &r );
        /* assert( parentid ); */

        if ( pdi )
          duf_levinfo_set_dirid( pdi, parentid );
        DUF_TRACE( path, 0, "inserted [%s] AS %llu", insdir, parentid );
        DUF_TRACE( path, 0, "#%llu : added real path %s", parentid, real_path );
      }
      dir = edir;
      DUF_TRACE( path, 0, "next [%s] under %llu", dir, parentid );
    }
    if ( !pdi && upfd )
      close( upfd );
    upfd = 0;
  }
  duf_pdi_set_topdepth( pdi );
  mas_free( real_path );
  if ( r >= 0 && !parentid )
    r = DUF_ERROR_NOT_IN_DB;
  if ( pr )
    *pr = r;

  return parentid;
}

unsigned long long
duf_insert_path_uni( duf_depthinfo_t * pdi, const char *dename, dev_t dev_id, ino_t dir_ino, unsigned long long parentid, int need_id,
                     int *pr )
{
  return duf_insert_path_uni2( pdi, dename, 1, ( duf_levinfo_t * ) NULL, dev_id, dir_ino, parentid, need_id, pr );
}
