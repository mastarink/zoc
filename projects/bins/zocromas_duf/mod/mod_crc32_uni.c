/* #define DUF_GET_FIELD_FROM_ROW */
/* #undef MAS_TRACING */
#include <assert.h>                                                  /* assert */
#include <stddef.h>                                                  /* NULL */
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <zlib.h>

#include "duf_tracen_defs_preset.h"                                  /* MAST_TRACE_CONFIG; etc. ✗ */
#include "duf_errorn_defs_preset.h"                                  /* MAST_ERRORS_FILE; etc. ✗ */

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>                                  /* mas_(malloc|free|strdup); etc. ▤ */
#include <mastar/trace/mas_trace.h>
#include <mastar/error/mas_error_defs_ctrl.h>
#include <mastar/error/mas_error_defs_make.h>
#include <mastar/error/mas_error_defs.h>

#include "duf_sccb_structs.h"

#include "duf_config.h"                                              /* duf_get_config ✗ */
#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ✗ */

#include "duf_pdi_ref.h"
#include "duf_pdi_stmt.h"                                            /* duf_pdi_find_statement_by_id; etc. ✗ */

#include "duf_levinfo_ref.h"                                         /* duf_levinfo_*; etc. ✗ */

#include "duf_sql_se_stmt_defs.h"                                    /* DUF_SQL_SE_BIND_S_OPT etc. ✗ */

#include "duf_sccb_row_field_defs.h"                                 /* DUF_*FIELD2* ✗ */
/* #include "duf_sccb_row.h"                                            (* datarow_* ✗ *) */

#include "duf_sccbh_ref.h"
#include "duf_sccbh_shortcuts.h"                                     /* H_SCCB; H_PDI; H_* ... ✗ */
#include "duf_sccbh_row.h"                                           /* duf_sccbh_row_get_*; sccbh_rows_eval ✗ */

#include "duf_sql_defs.h"                                            /* DUF_SQL_IDFIELD etc. ✗ */
#include "duf_sql_field.h"                                           /* __duf_sql_str_by_name2 for DUF_GET_QUFIELD2 etc. ✗ */

#include "duf_sql_bind.h"                                            /* duf_sql_... for DUF_SQL_BIND_... etc. ✗ */
#include "duf_sql_prepared.h"                                        /* duf_sql_prepare; duf_sql_step; duf_sql_finalize; ✗ */

#include "sql_beginning_tables.h"                                    /* DUF_SQL_TABLES... etc. ✗ */

#include "duf_pdi_structs.h"
/* ########################################################################################## */
#include "duf_mod_types.h"
/* static int duf_crc32_dirent_content2( duf_depthinfo_t * H_PDI, duf_sccb_handle_t * sccbh MAS_UNUSED ); */
static DR( MOD, crc32_dirent_content2, duf_depthinfo_t * pdi_unused, duf_sccb_handle_t * sccbh );

/* ########################################################################################## */
#define FILTER_DATA  "fd.crc32id IS NULL"

static duf_sql_sequence_t final_sql =                                /* */
{
  .name = "final-crc32",
  .done = 0,
  .sql = {
          "UPDATE " DUF_SQL_TABLES_CRC32_FULL " SET dup32cnt=(SELECT COUNT(*) " /* */
          " FROM " DUF_SQL_TABLES_CRC32_FULL " AS c32 "              /* */
          " JOIN " DUF_SQL_TABLES_FILEDATAS_FULL " AS fd ON (fd.crc32id=c32." DUF_SQL_IDFIELD ") " /* */
          " WHERE " DUF_SQL_TABLES_CRC32_FULL "." DUF_SQL_IDFIELD "=c32." DUF_SQL_IDFIELD ")" /* */
        /* " WHERE " DUF_SQL_TABLES_CRC32_FULL ".crc32sum=c32.crc32sum )" (* *) */
          ,
          NULL,
          }
};

/* ########################################################################################## */
static duf_scan_callbacks_t duf_sccb_dispatch;

const duf_mod_handler_t duf_mod_handler_uni[] = {
  {"sccb", &duf_sccb_dispatch},
  {NULL, NULL}
};

/* ########################################################################################## */
static duf_scanner_set_t scanners[] = {
  {
   .flags = DUF_SCANNER_SET_FLAG_TO_OPEN | DUF_SCANNER_SET_FLAG_DB,  /* */
   .type = DUF_NODE_LEAF,                                            /* */
   .scanstage = DUF_SCANSTAGE_DB_LEAVES,                             /* */
   .fun = F2ND( crc32_dirent_content2 ),                             /* */
   },

  {.fun = NULL}
};

static duf_scan_callbacks_t duf_sccb_dispatch = {
  .title = "collect crc32",
  .name = "crc32",
  .init_scan = NULL,
  .def_opendir = 1,

#if 0
  .leaf_scan_fd2 = F2ND( crc32_dirent_content2 ),
#else
  .scanners = scanners,
#endif

/* TODO : explain values of use_std_leaf_set_num and use_std_node_set_num TODO */
  .use_std_leaf_set_num = -1,                                        /* 1 : preliminary selection; 2 : direct (beginning_sql_seq=NULL recommended in many cases) */
  .use_std_node_set_num = 2,                                         /* 1 : preliminary selection; 2 : direct (beginning_sql_seq=NULL recommended in many cases) */
/* .std_leaf_set_name = "std-leaf-no-sel", */
  .std_leaf_set_name = "std-leaf-no-sel-fd",
  .std_node_set_name = "std-node-two",
#define DUF_FILTER
  .leaf = {                                                          /* */
           .name = "crc32-leaf",
           .type = DUF_NODE_LEAF,
           .fieldset =                                               /* */
           "#crc32",
           .fieldsets = {
                         "#basic",
                         "#plus",
                         "#crc32x",
                         NULL}
           ,
           .selector2 = "#std-ns-fd-leaf",
           .matcher = " fn.Pathid=:parentdirID "                     /* */
           ,                                                         /* */
#if 0
           .filter =                                                 /* */
           "( " FILTER_DATA " OR crc." DUF_SQL_IDFIELD " IS NULL ) " /*                          */ " AND " /* */
           "( sz.size    IS NULL OR sz.size > 0 ) " /*                                             */ " AND " /* */
           "(  :fFast    IS NULL OR sz.size IS NULL OR sz.dupzcnt IS NULL OR sz.dupzcnt > 1 ) " /* */ " AND " /* */
           " 1 "                                                     /* */
           ,                                                         /* */
#else
           .afilter = {"sz.size > 0"},
           .afilter_fresh = {FILTER_DATA " OR crc." DUF_SQL_IDFIELD " IS NULL", "sz.size  IS NULL OR sz.size > 0"},
           .afilter_fast = {"sz.size IS NULL OR sz.dupzcnt IS NULL OR sz.dupzcnt > 1"},
#endif
         /*, .group=" fd." DUF_SQL_IDFIELD */
         /* .count_aggregate = "DISTINCT fd." DUF_SQL_IDFIELD (* *) */
           }
  ,
  .node = {
           .name = "crc32-node",
           .type = DUF_NODE_NODE,
           .expand_sql = 1,                                          /* */
           .fieldset =                                               /* */
         /* "'crc32-node' AS fieldset_id, " (* *) */
           " pt." DUF_SQL_IDFIELD " AS dirid"                        /* */
           ", pt." DUF_SQL_IDFIELD " AS nameid "                     /* */
           ", pt." DUF_SQL_DIRNAMEFIELD " AS dname, pt." DUF_SQL_DIRNAMEFIELD " AS dfname,  pt.ParentId " /* */
           ", pt.size AS filesize, pt.mode AS filemode, pt.dev, pt.uid, pt.gid, pt.nlink, pt.inode, pt.rdev, pt.blksize, pt.blocks, STRFTIME( '%s', pt.mtim ) AS mtime " /* */
           ,
           .selector2 =                                              /* */
           " FROM " DUF_SQL_TABLES_PATHS_FULL " AS pt "              /* */
           ,
           .matcher = "pt.ParentId=:parentdirID AND ( :dirName IS NULL OR dname=:dirName )" /* */
           ,                                                         /* */
           },
  .final_sql_seq = &final_sql,
};

/* ########################################################################################## */
static
SRP( MOD, unsigned long long, crc32id, 0, pdistat2file_crc32id_existed, duf_depthinfo_t * pdi_unused MAS_UNUSED, duf_sccb_handle_t * sccbh MAS_UNUSED,
     unsigned long crc32sum )
{
  const char *sql = "SELECT " DUF_SQL_IDFIELD " AS crc32id FROM " DUF_SQL_TABLES_CRC32_FULL " WHERE crc32sum=:Crc32sum"
        /* " INDEXED BY " DUF_SQL_TABLES_CRC32 "_uniq WHERE  crc32sum=:Crc32sum" */
          ;

  DUF_SQL_SE_START_STMT( H_PDI, select_crc32, sql, pstmt_local );
  MAST_TRACE( select, 3, "S:%s", sql );
  DUF_SQL_SE_BIND_LL( Crc32sum, crc32sum, pstmt_local );
  DUF_SQL_SE_STEP( pstmt_local );
  if ( QISERR1_N( SQL_ROW ) )
  {
    ERRCLEAR1( SQL_ROW );
    MAST_TRACE( select, 10, "<selected>" );
  /* crc32id = duf_sql_column_long_long( pstmt_local, 0 ); */
    crc32id = DUF_GET_QUFIELD3( pstmt_local, crc32id );
  /* rpr = 0; */
  }
  else
  {
    MAST_TRACE( select, 10, "<NOT selected> (%d)", QERRIND );
  }
  DUF_SQL_SE_END_STMT( H_PDI, select_crc32, pstmt_local );           /* clears SQL_ROW / SQL_DONE */
  ERP( MOD, unsigned long long, crc32id, 0, pdistat2file_crc32id_existed, duf_depthinfo_t * pdi_unused, duf_sccb_handle_t * sccbh,
       unsigned long crc32sum );
}

static
SRP( MOD, unsigned long long, crc32id, -1, insert_crc32_uni, duf_depthinfo_t * pdi_unused MAS_UNUSED, duf_sccb_handle_t * sccbh MAS_UNUSED,
     unsigned long long crc32sum, const char *filename MAS_UNUSED, int need_id )
{
  int changes = 0;

#ifdef MAS_TRACING
  const char *real_path = duf_levinfo_path( H_PDI );
#endif

  if ( crc32sum )
  {
    static unsigned long insert_cnt = 0;

    if ( !duf_get_config_flag_disable_insert(  ) )
    {
      static const char *sql = "INSERT OR IGNORE INTO " DUF_SQL_TABLES_CRC32_FULL " (crc32sum) VALUES (:crc32sum)";

      MAST_TRACE( crc32, 10, "%0llx %s%s", crc32sum, real_path, filename );
      DUF_SQL_SE_START_STMT( H_PDI, insert_crc32, sql, pstmt_local );
      MAST_TRACE( insert, 0, "(%ld) S:%s", insert_cnt, sql );
      DUF_SQL_SE_BIND_LL( crc32sum, crc32sum, pstmt_local );
      DUF_SQL_SE_STEPC( pstmt_local );
      DUF_SQL_SE_CHANGES( H_PDI, changes, pstmt_local );
      DUF_SQL_SE_END_STMT( H_PDI, insert_crc32, pstmt_local );       /* clears SQL_ROW / SQL_DONE */
      insert_cnt++;
    }
    duf_pdi_reg_changes( H_PDI, changes );
    if ( ( QISERR1_N( SQL_CONSTRAINT ) || QNOERR ) && !changes )
    {
      if ( need_id )
        crc32id = CRP( pdistat2file_crc32id_existed, H_PDI, sccbh, crc32sum );
    }
    else if ( QNOERR /* assume SQLITE_OK */  )
    {
      if ( need_id && changes )
      {
        crc32id = duf_sql_last_insert_rowid(  );
      }
    }
  /* else                                       */
  /* {                                          */
  /*   DUF_SHOW_ERROR( "insert crc32 %d", lr ); */
  /* }                                          */
  }
  else
  {
  /* DUF_SHOW_ERROR( "Wrong data" ); */
    ERRMAKE_M( DATA, "no crc32 sum" );
  }

  ERP( MOD, unsigned long long, crc32id, -1, insert_crc32_uni, duf_depthinfo_t * pdi_unused, duf_sccb_handle_t * sccbh, unsigned long long crc32sum,
       const char *filename MAS_UNUSED, int need_id );
}

static
SR( MOD, make_crc32_uni, int fd, unsigned long long *pbytes, unsigned long long *pcrc32sum )
{
  size_t bufsz = 512 * 8;
  unsigned long crc32sum = 0;
  unsigned char *buffer;

  if ( !duf_get_config_flag_disable_calculate(  ) )
    crc32sum = crc32( 0L, Z_NULL, 0 );
/* if ( !duf_get_config_flag_disable_calculate()) */
  {
    size_t bytes = 0;

    buffer = mas_malloc( bufsz );
    if ( buffer )
    {
      int cnt = 0;
      int maxcnt = 1;

    /* lseek( fd, -bufsz * maxcnt, SEEK_END ); */
      while ( QNOERR && cnt++ < maxcnt )
      {
        int ry;

        ry = read( fd, buffer, bufsz );
        MAST_TRACE( crc32, 10, "read %d : crc32sum:%lx", ry, crc32sum );
        if ( ry < 0 )
        {
        /* DUF_ERRSYS( "read file" ); */
          MASE_ERRSYS( "read file" );
          ERRMAKE( READ );
        }

        if ( ry > 0 )
        {
          if ( pbytes )
            ( *pbytes ) += ry;
          if ( !duf_get_config_flag_disable_calculate(  ) )
          {
            crc32sum = crc32( crc32sum, buffer, ry );
            bytes += ry;
          }
        }
        MAST_TRACE( crc32, 10, "ry:%d; r:%d; crc32sum:%lx", ry, QERRIND, crc32sum );
        if ( ry <= 0 )
          break;

      }
      MAST_TRACE( crc32, 10, "last crc32sum:%lx", crc32sum );
      mas_free( buffer );
    }
    else
    {
      ERRMAKE( MEMORY );
    }
  /* QT( "@bytes:%lu", bytes ); */
  }
  if ( pcrc32sum )
    *pcrc32sum = crc32sum;

  ER( MOD, make_crc32_uni, int fd, unsigned long long *pbytes, unsigned long long *pcrc32sum );
}

static
SR( MOD, crc32_dirent_content2, duf_depthinfo_t * pdi_unused MAS_UNUSED, duf_sccb_handle_t * sccbh MAS_UNUSED )
{
  unsigned long long crc32sum = 0;
  static unsigned long content_cnt = 0;
  unsigned long long bytes = 0;

  DUF_RSFIELD2( fname );
  MAST_TRACE( crc32, 0, "+ %s", fname );

/* assert( duf_levinfo_dbstat( H_PDI ) ); */
  assert( duf_levinfo_dfd( H_PDI ) );
  assert( duf_levinfo_stat( H_PDI ) );

  if ( duf_get_config_flag_disable_calculate(  ) )
    crc32sum = ( unsigned long long ) duf_levinfo_dirid( H_PDI );
  else
    CR( make_crc32_uni, duf_levinfo_dfd( H_PDI ), &bytes, &crc32sum );

  content_cnt++;
  if ( QNOERR )
  {
    unsigned long long crc32id = 0;

    MAST_TRACE( crc32, 10, "insert %s", fname );

    crc32id = CRP( insert_crc32_uni, H_PDI, sccbh, crc32sum, fname /* for dbg message only */ , 1 /*need_id */  );
    if ( QNOERR && crc32id )
    {
      int changes = 0;

      H_PDI->cnts.dirent_content2++;
      if ( QNOERR && !duf_get_config_flag_disable_update(  ) )
      {
        DUF_RUFIELD2( filedataid );
#if 0
        DOR( r,
             duf_sql( "UPDATE " DUF_SQL_TABLES_FILEDATAS_FULL " SET crc32id='%llu' WHERE " DUF_SQL_IDFIELD "='%lld'", &changes, crc32id,
                      filedataid ) );
#else
        const char *sql = "UPDATE " DUF_SQL_TABLES_FILEDATAS_FULL " SET crc32id=:crc32Id WHERE " DUF_SQL_IDFIELD " =:dataId ";

        DUF_SQL_SE_START_STMT( H_PDI, update_crc32id, sql, pstmt_local );
        MAST_TRACE( mod, 3, "S:%s", sql );
        DUF_SQL_SE_BIND_LL( crc32Id, crc32id, pstmt_local );
        DUF_SQL_SE_BIND_LL( dataId, filedataid, pstmt_local );
        DUF_SQL_SE_STEPC( pstmt_local );
        DUF_SQL_SE_CHANGES( H_PDI, changes, pstmt_local );
        DUF_SQL_SE_END_STMT( H_PDI, update_crc32id, pstmt_local );   /* clears SQL_ROW / SQL_DONE */
#endif
      }
      duf_pdi_reg_changes( H_PDI, changes );

    }
    MAST_TRACE( crc32, 0, "(%lu) %04llx : crc32id: %llu (sz:%lu) \"%s\"", content_cnt, crc32sum, crc32id,
                duf_levinfo_stat_size( H_PDI ) /* duf_levinfo_stat( H_PDI )->st_size */ , fname );
  /* MAST_TRACE( scan, 12, "  " DUF_DEPTH_PFMT ": scan 5    * %04lx : %llu", duf_pdi_depth( H_PDI ), crc32sum, crc32id ); */
  }
  H_PDI->total_bytes += bytes;
  H_PDI->total_files++;

  ER( MOD, crc32_dirent_content2, duf_depthinfo_t * pdi_unused, duf_sccb_handle_t * sccbh );
}
