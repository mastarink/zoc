#define DUF_SQL_PDI_STMT

#include <string.h>
#include <errno.h>
#include <unistd.h>

#include <zlib.h>

#include <assert.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>


#include "duf_maintenance.h"


#include "duf_config_ref.h"

#include "duf_pdi.h"
#include "duf_pdi_ref.h"
#include "duf_pdi_stmt.h"

#include "duf_levinfo_ref.h"


#include "duf_sql_defs.h"
#include "duf_sql_field.h"
#include "duf_sql_field2.h"

#include "duf_sql.h"
#include "duf_sql1.h"
#include "duf_sql2.h"

/* #include "duf_dbg.h" */

/* #include "sql_beginning_selected.h" */
#include "sql_beginning_tables.h"


/* ########################################################################################## */
static int crc32_dirent_content2( duf_sqlite_stmt_t * pstmt, duf_depthinfo_t * pdi );

/* ########################################################################################## */
static duf_sql_sequence_t final_sql = {.done = 0,
  .name = "final @ ...",
  .sql = {
          "UPDATE " DUF_SQL_TABLES_CRC32_FULL " SET dup32cnt=(SELECT COUNT(*) " /* */
          " FROM " DUF_SQL_TABLES_CRC32_FULL " AS c32 " /* */
          " JOIN " DUF_SQL_TABLES_FILEDATAS_FULL " AS fd ON (fd.crc32id=c32." DUF_SQL_IDNAME ") " /* */
          " WHERE " DUF_SQL_TABLES_CRC32_FULL "." DUF_SQL_IDNAME "=c32." DUF_SQL_IDNAME ")" /* */
          /* " WHERE " DUF_SQL_TABLES_CRC32_FULL ".crc32sum=c32.crc32sum )" (* *) */
          ,
          NULL,
          }
};

/* ########################################################################################## */

duf_scan_callbacks_t duf_collect_openat_crc32_callbacks = {
  .title = "collect crc32",
  .name = "crc32",
  .init_scan = NULL,
  .def_opendir = 1,





  .leaf_scan_fd2 = crc32_dirent_content2,

/* TODO : exp;ain values of use_std_leaf and use_std_node TODO */
  .use_std_leaf = 0,            /* 1 : preliminary selection; 2 : direct (beginning_sql_seq=NULL recommended in many cases) */
  .use_std_node = 0,            /* 1 : preliminary selection; 2 : direct (beginning_sql_seq=NULL recommended in many cases) */
  .leaf = {                     /* */
           .name = "crc32 leaf",
           .type = DUF_NODE_LEAF,
           .fieldset =          /* */
           /* "'crc32-leaf' AS fieldset_id, " (* *) */
           "fn.Pathid AS dirid " /* */
           ", 0 as ndirs, 0 as nfiles" /* */
           " , fd." DUF_SQL_IDNAME " AS filedataid, fd.inode AS inode " /* */
           " , fn.name AS filename, fn.name AS dfname, fd.size AS filesize " /* */
           " , fd.dev, fd.uid, fd.gid, fd.nlink, strftime('%s',fd.mtim) AS mtime, fd.rdev, fd.blksize, fd.blocks " /* */
           " , crc.dup32cnt AS nsame " /* */
           " , fn." DUF_SQL_IDNAME " AS filenameid " /* */
           " , fn." DUF_SQL_IDNAME " AS nameid " /* */
           " , fd.mode AS filemode, crc.crc32sum " /* */
           " , fd.crc32id AS crc32id" /* */
           " , fd.md5id AS md5id" /* */
           ,
           .selector2 =         /* */
           /* "SELECT %s " */
           " FROM " DUF_SQL_TABLES_FILENAMES_FULL " AS fn " /* */
           " LEFT JOIN " DUF_SQL_TABLES_FILEDATAS_FULL " AS fd ON (fn.dataid=fd." DUF_SQL_IDNAME ") " /* */
           " LEFT JOIN " DUF_SQL_TABLES_SIZES_FULL " as sz ON (sz.size=fd.size)" /* */
           " LEFT JOIN " DUF_SQL_TABLES_CRC32_FULL " AS crc ON (crc." DUF_SQL_IDNAME "=fd.crc32id)" /* */
           ,
           .matcher = " fn.Pathid=:parentdirID " /* */
           ,                    /* */
           .filter =            /* */
           " ( fd.crc32id IS NULL OR crc." DUF_SQL_IDNAME " IS NULL ) AND " /* */
           " sz.size > 0                                              AND " /* */
           "(  :fFast IS NULL OR sz.size IS NULL OR sz.dupzcnt > 1 )  AND " /* */
           " 1 "                /* */
           /*, .group=" fd." DUF_SQL_IDNAME */
           ,                    /* */
           .count_aggregate = "distinct fd." DUF_SQL_IDNAME}
  ,
  .node = {
           .name = "crc32 node",
           .type = DUF_NODE_NODE,
   .expand_sql = 1,        /* */
           .fieldset =          /* */
           /* "'crc32-node' AS fieldset_id, " (* *) */
           " pt." DUF_SQL_IDNAME " AS dirid" /* */
           ", pt." DUF_SQL_IDNAME " AS nameid " /* */
           ", pt.dirname, pt.dirname AS dfname,  pt.ParentId " /* */
           ", tf.numfiles AS nfiles, td.numdirs AS ndirs, tf.maxsize AS maxsize, tf.minsize AS minsize" /* */
           ", pt.size AS filesize, pt.mode AS filemode, pt.dev, pt.uid, pt.gid, pt.nlink, pt.inode, pt.rdev, pt.blksize, pt.blocks, STRFTIME( '%s', pt.mtim ) AS mtime " /* */
           ,
           .selector2 =         /* */
           /* "SELECT     pt." DUF_SQL_IDNAME " AS dirid, pt.dirname, pt.dirname AS dfname,  pt.ParentId "                  */
           /* ", tf.numfiles AS nfiles, td.numdirs AS ndirs, tf.maxsize AS maxsize, tf.minsize AS minsize " */
           " FROM " DUF_SQL_TABLES_PATHS_FULL " AS pt " /* */
           " LEFT JOIN " DUF_SQL_TABLES_TMP_PATHTOT_DIRS_FULL "  AS td ON (td.Pathid=pt." DUF_SQL_IDNAME ") " /* */
           " LEFT JOIN " DUF_SQL_TABLES_TMP_PATHTOT_FILES_FULL " AS tf ON (tf.Pathid=pt." DUF_SQL_IDNAME ") " /* */
#if 0
           " LEFT JOIN " DUF_DBPREF "pathtot_dirs AS td ON (td.Pathid=pt." DUF_SQL_IDNAME ") " /* */
           " LEFT JOIN " DUF_DBPREF "pathtot_files AS tf ON (tf.Pathid=pt." DUF_SQL_IDNAME ") " /* */
#endif
           ,
           .matcher = "pt.ParentId=:parentdirID AND ( :dirName IS NULL OR dirname=:dirName )" /* */
           ,                    /* */
           .filter = NULL       /* */
#if 0
           ,
           .selector_total2 =   /* */
           " /* c3 */ FROM " DUF_SQL_TABLES_PATHS_FULL " AS p " /* */
#endif
           },
  .final_sql_seq = &final_sql,
};

/* ########################################################################################## */

static unsigned long long
duf_insert_crc32_uni( duf_depthinfo_t * pdi, unsigned long crc32sum, const char *filename, int need_id, int *pr )
{
  unsigned long long crc32id = -1;
  int lr = 0;
  int changes = 0;

#ifdef MAS_TRACING
  const char *real_path = duf_levinfo_path( pdi );
#endif

  DEBUG_START(  );
  if ( crc32sum )
  {
    static unsigned long insert_cnt = 0;

    if ( !DUF_CONFIGG( cli.disable.flag.insert ) )
    {
#if 1
      static const char *sql = "INSERT OR IGNORE INTO " DUF_SQL_TABLES_CRC32_FULL " (crc32sum) VALUES (:crc32sum)";

      DUF_TRACE( crc32, 10, "%0lx %s%s", crc32sum, real_path, filename );
      DUF_SQL_START_STMT( pdi, insert_crc32, sql, lr, pstmt );
      DUF_TRACE( insert, 0, "(%ld) S:%s", insert_cnt, sql );
      DUF_SQL_BIND_LL( crc32sum, crc32sum, lr, pstmt );
      DUF_SQL_STEP( lr, pstmt );
      DUF_SQL_CHANGES( changes, lr, pstmt );
      DUF_SQL_END_STMT( insert_crc32, lr, pstmt );
      insert_cnt++;
#else

#endif
    }
    duf_pdi_reg_changes( pdi, changes );
    if ( ( lr == DUF_SQL_CONSTRAINT || !lr ) && !changes )
    {
      if ( need_id )
      {
        duf_scan_callbacks_t sccb = {
          .leaf.type = DUF_NODE_LEAF,
          .leaf.fieldset = "crc32id"
        };
        duf_sccb_handle_t csccbh = {.sccb = &sccb };
        lr = duf_sql_select( duf_sel_cb_field_by_sccb, &crc32id, STR_CB_DEF, STR_CB_UDATA_DEF, /* */
                             &csccbh, /* */
                             "SELECT " DUF_SQL_IDNAME " AS crc32id FROM " DUF_SQL_TABLES_CRC32_FULL " WHERE crc32sum='%lld'", crc32sum );
      }
    }
    else if ( !lr /* assume SQLITE_OK */  )
    {
      if ( need_id && changes )
      {
        crc32id = duf_sql_last_insert_rowid(  );
      }
    }
    else
    {
      DUF_SHOW_ERROR( "insert crc32 %d", lr );
    }
  }
  else
  {
    DUF_SHOW_ERROR( "Wrong data" );
    DUF_MAKE_ERROR( lr, DUF_ERROR_DATA );
    DUF_TEST_R( lr );
  }

  if ( pr )
    *pr = lr;

  DEBUG_ENDULL( crc32id );
  return crc32id;
}

static int
duf_make_crc32_uni( int fd, unsigned long *pcrc32sum )
{
  DEBUG_STARTR( r );
  size_t bufsz = 512 * 8;
  unsigned long crc32sum = 0;
  unsigned char *buffer;

  if ( !DUF_CONFIGG( cli.disable.flag.calculate ) )
    crc32sum = crc32( 0L, Z_NULL, 0 );
  /* if ( !DUF_CONFIGG(cli.disable.flag.calculate )) */
  {
    buffer = mas_malloc( bufsz );
    if ( buffer )
    {
      int cnt = 0;
      int maxcnt = 1;

      /* lseek( fd, -bufsz * maxcnt, SEEK_END ); */
      while ( DUF_NOERROR( r ) && cnt++ < maxcnt )
      {
        int rr;

        rr = read( fd, buffer, bufsz );
        DUF_TRACE( crc32, 10, "read %d : crc32sum:%lx", rr, crc32sum );
        if ( rr < 0 )
        {
          DUF_ERRSYS( "read file" );
          DUF_MAKE_ERROR( r, DUF_ERROR_READ );
        }
        DUF_TEST_R( r );
        if ( rr > 0 && !DUF_CONFIGG( cli.disable.flag.calculate ) )
          crc32sum = crc32( crc32sum, buffer, rr );
        DUF_TRACE( crc32, 10, "rr:%d; r:%d; crc32sum:%lx", rr, r, crc32sum );
        if ( rr <= 0 )
          break;

      }
      DUF_TRACE( crc32, 10, "last crc32sum:%lx", crc32sum );
      mas_free( buffer );
    }
    else
    {
      DUF_MAKE_ERROR( r, DUF_ERROR_MEMORY );
    }
  }
  if ( pcrc32sum )
    *pcrc32sum = crc32sum;
  DEBUG_ENDR( r );
}


static int
crc32_dirent_content2( duf_sqlite_stmt_t * pstmt, /* const struct stat *pst_file_needless, */ duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );
  unsigned long crc32sum = 0;
  static unsigned long content_cnt = 0;

  DUF_UFIELD2( filedataid );
  DUF_SFIELD2( filename );
  DUF_TRACE( crc32, 0, "+ %s", filename );

  /* assert( duf_levinfo_dbstat( pdi ) ); */
  assert( duf_levinfo_dfd( pdi ) );
  assert( duf_levinfo_stat( pdi ) );

  if ( DUF_CONFIGG( cli.disable.flag.calculate ) )
    crc32sum = duf_levinfo_dirid( pdi );
  else
    DOR( r, duf_make_crc32_uni( duf_levinfo_dfd( pdi ), &crc32sum ) );

  content_cnt++;
  if ( DUF_NOERROR( r ) )
  {
    unsigned long long crc32id = 0;

    DUF_TRACE( crc32, 10, "insert %s", filename );


    crc32id = duf_insert_crc32_uni( pdi, crc32sum, filename /* for dbg message only */ , 1 /*need_id */ , &r );
    if ( DUF_NOERROR( r ) && crc32id )
    {
      int changes = 0;

      pdi->cnts.dirent_content2++;
      if ( DUF_NOERROR( r ) && !DUF_CONFIGG( cli.disable.flag.update ) )
        DOR( r, duf_sql( "UPDATE " DUF_SQL_TABLES_FILEDATAS_FULL " SET crc32id=%llu WHERE " DUF_SQL_IDNAME "=%lld", &changes, crc32id, filedataid ) );
      duf_pdi_reg_changes( pdi, changes );
      DUF_TEST_R( r );
    }
    DUF_TRACE( crc32, 0, "(%lu) %04lx : crc32id: %llu (sz:%lu) \"%s\"", content_cnt, crc32sum, crc32id, duf_levinfo_stat( pdi )->st_size, filename );
    /* DUF_TRACE( scan, 12, "  " DUF_DEPTH_PFMT ": scan 5    * %04lx : %llu", duf_pdi_depth( pdi ), crc32sum, crc32id ); */
  }
  DEBUG_ENDR( r );
}
