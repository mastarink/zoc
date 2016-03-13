/* #undef MAS_TRACING */
#include <assert.h>                                                  /* assert */
#include <stddef.h>                                                  /* NULL */
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <openssl/sha.h>

#include "duf_tracen_defs_preset.h"                                  /* MAST_TRACE_CONFIG; etc. ♠ */
#include "duf_errorn_defs_preset.h"

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>                                  /* mas_(malloc|free|strdup); etc. ♣ */
#include <mastar/trace/mas_trace.h>
#include <mastar/error/mas_error_defs_ctrl.h>
#include <mastar/error/mas_error_defs_make.h>
#include <mastar/error/mas_error_defs.h>

/*  #include "duf_tracen_defs.h"  (*  T; TT; TR ♠  *) */
/*  #include "duf_errorn_defs.h"  (*  DUF_NOERROR; DUF_CLEAR_ERROR; DUF_E_(LOWER|UPPER); DUF_TEST_R ... ♠  *) */

/*  #include "duf_start_end.h"  (*  DUF_STARTR ; DUF_ENDR ♠  *) */
/*  #include "duf_dodefs.h"  (*  DOR ♠  *) */

#include "duf_sccb_types.h"                                          /* duf_scan_callbacks_t ♠ */

#include "duf_config.h"                                              /* duf_get_config ♠ */
#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ♠ */
/* #include "duf_config_ref.h" */
#include "duf_config_defs.h"                                         /* DUF_CONF... ♠ */
#include "duf_pdi_ref.h"
#include "duf_pdi_stmt.h"
#include "duf_levinfo_ref.h"
/* #include "duf_sql_stmt_defs.h"                                       (* DUF_SQL_BIND_S_OPT etc. ♠ *) */
#include "duf_sql_se_stmt_defs.h"                                       /* DUF_SQL_BIND_S_OPT etc. ♠ */
#include "duf_sql_defs.h"                                            /* DUF_SQL_IDFIELD etc. ♠ */
#include "duf_sql_field.h"                                           /* __duf_sql_str_by_name2 for DUF_GET_UFIELD2 etc. ♠ */
#include "duf_sql_bind.h"                                            /* duf_sql_... for DUF_SQL_BIND_... etc. ♠ */
#include "duf_sql_prepared.h"                                        /* duf_sql_(prepare|step|finalize) ♠ */
/* #include "duf_dbg.h" */
#include "sql_beginning_tables.h"                                    /* DUF_SQL_TABLES... etc. ♠ */
/* ########################################################################################## */
static int duf_sha1_dirent_content2( duf_stmnt_t * pstmt, duf_depthinfo_t * pdi );

#define ASHA1_DELTA 4

/* ########################################################################################## */
#define FILTER_DATA "fd.sha1id IS NULL"

static duf_sql_sequence_t final_sql =                                /* */
{
  .name = "final-sha1",
  .done = 0,
  .sql = {
          "UPDATE " DUF_SQL_TABLES_SHA1_FULL " SET dupsha1cnt=(SELECT COUNT(*) " /* */
          " FROM " DUF_SQL_TABLES_SHA1_FULL " AS sh "                /* */
          " JOIN " DUF_SQL_TABLES_FILEDATAS_FULL " AS fd ON (fd.sha1id=sh." DUF_SQL_IDFIELD ") " /* */
          " WHERE " DUF_SQL_TABLES_SHA1_FULL "." DUF_SQL_IDFIELD "=sh." DUF_SQL_IDFIELD ")" /* */
          ,
          NULL,
          }
};

/* ########################################################################################## */

duf_scan_callbacks_t duf_sha1_callbacks = {
  .title = "collect sha1",
  .name = "sha1",
  .init_scan = NULL,
  .def_opendir = 1,
/* .dirent_dir_scan_before = NULL, */
/* .dirent_file_scan_before = NULL, */
/* .node_scan_before = collect_openat_sha1_node_before, */
/*  .leaf_scan =  collect_openat_sha1_leaf, */
/* .leaf_scan_fd = duf_dirent_sha1_content, */
  .leaf_scan_fd2 = duf_sha1_dirent_content2,

/* TODO : explain values of use_std_leaf_set_num and use_std_node_set_num TODO */
  .use_std_leaf_set_num = 2,                                         /* 1 : preliminary selection; 2 : direct (beginning_sql_seq=NULL recommended in many cases) */
  .use_std_node_set_num = 2,                                         /* 1 : preliminary selection; 2 : direct (beginning_sql_seq=NULL recommended in many cases) */
/* .std_leaf_set_name = "std-leaf-no-sel", */
  .std_leaf_set_name = "std-leaf-no-sel-fd",
  .std_node_set_name = "std-node-two",
  .leaf = {
           .name = "sha1-leaf",
           .type = DUF_NODE_LEAF,
           .fieldset =                                               /* */
           "#sha1",
           .fieldsets = {
                         "#basic",
                         "#plus",
                         "#sha1x",
                         NULL}
           ,
           .selector2 =                                              /* */
           "#sha1-leaf",
           .matcher = " fn.Pathid=:parentdirID "                     /* */
           ,                                                         /* */
#if 0
           .filter =                                                 /* */
           "( " FILTER_DATA " OR sh." DUF_SQL_IDFIELD " IS NULL ) " /*                                          */ " AND " /* */
           "( sz.size   IS NULL OR sz.size > 0 ) " /*                                                            */ " AND " /* */
           "(  :fFast   IS NULL OR sz.size IS NULL OR sz.dupzcnt > 1 ) " /*                                      */ " AND " /* */
           "(  :fFast   IS NULL OR sd." DUF_SQL_IDFIELD " IS NULL OR sd.dup2cnt IS NULL OR sd.dup2cnt > 1 ) " /*  */ " AND " /* */
           "(  :fFast   IS NULL OR md." DUF_SQL_IDFIELD " IS NULL OR md.dup5cnt IS NULL OR md.dup5cnt > 1 ) " /*  */ " AND " /* */
           " 1 "                                                     /* */
           ,
#else
           .afilter_fresh = {FILTER_DATA " OR sh." DUF_SQL_IDFIELD " IS NULL", "sz.size  IS NULL OR sz.size > 0"},
           .afilter_fast = {
                            "sz.size IS NULL OR sz.dupzcnt IS NULL OR sz.dupzcnt > 1",
                            "sd." DUF_SQL_IDFIELD " IS NULL OR sd.dup2cnt IS NULL OR sd.dup2cnt > 1",
                            "md." DUF_SQL_IDFIELD " IS NULL OR md.dup5cnt IS NULL OR md.dup5cnt > 1"},
#endif
         /* .count_aggregate = "DISTINCT fd." DUF_SQL_IDFIELD (* *) */
           }
  ,                                                                  /* */
  .node = {                                                          /* */
           .name = "sha1-node",
           .type = DUF_NODE_NODE,
           .expand_sql = 1,                                          /* */
           .fieldset =                                               /* */
         /* "'sha1-node' AS fieldset_id, " (* *) */
           " pt." DUF_SQL_IDFIELD " AS dirid"                        /* */
           ", pt." DUF_SQL_IDFIELD " AS nameid "                     /* */
           ", pt." DUF_SQL_DIRNAMEFIELD " AS dname, pt." DUF_SQL_DIRNAMEFIELD " AS dfname,  pt.ParentId " /* */
#ifndef MAS_DUF_DEFS_H
# error use #include "duf_defs.h"
#elif defined( DUF_DO_NUMS )
           ", tf.numfiles AS nfiles, td.numdirs AS ndirs, tf.maxsize AS maxsize, tf.minsize AS minsize" /* */
#endif
#ifndef MAS_DUF_DEFS_H
# error use #include "duf_defs.h"
#elif defined( DUF_DO_RNUMS )
           ", " DUF_SQL_RNUMDIRS( pt ) " AS rndirs "                 /* */
           ", (" DUF_SQL__RNUMFILES( pt ) " WHERE " FILTER_DATA ") AS rnfiles " /* */
#endif
           ", pt.size AS filesize, pt.mode AS filemode, pt.dev, pt.uid, pt.gid, pt.nlink, pt.inode, pt.rdev, pt.blksize, pt.blocks, STRFTIME( '%s', pt.mtim ) AS mtime " /* */
           ,
           .selector2 =                                              /* */
           " FROM " DUF_SQL_TABLES_PATHS_FULL " AS pt "              /* */
#ifndef MAS_DUF_DEFS_H
# error use #include "duf_defs.h"
#elif defined( DUF_DO_NUMS )
           " LEFT JOIN " DUF_SQL_TABLES_PSEUDO_PATHTOT_DIRS_FULL "  AS td ON (td.Pathid=pt." DUF_SQL_IDFIELD ") " /* */
           " LEFT JOIN " DUF_SQL_TABLES_PSEUDO_PATHTOT_FILES_FULL " AS tf ON (tf.Pathid=pt." DUF_SQL_IDFIELD ") " /* */
#endif
           ,
           .matcher = " pt.ParentId=:parentdirID AND ( :dirName IS NULL OR dname=:dirName )" /* */
           ,
#ifndef MAS_DUF_DEFS_H
# error use #include "duf_defs.h"
#elif defined( DUF_DO_NUMS )
           .filter = " rnfiles > 0 "                                 /* */
#endif
           },
  .final_sql_seq = &final_sql,
};

/* ########################################################################################## */
SRP(MOD,unsigned long long, sha1id,0,pdistat2file_sha1id_existed, duf_depthinfo_t * pdi, unsigned long sha1sum1, unsigned long sha1sum2, unsigned long sha1sum3 )
{
  /* int rpr = 0; */
  /* unsigned long long sha1id = 0; */
  const char *sql =
          "SELECT " DUF_SQL_IDFIELD " AS sha1id FROM " DUF_SQL_TABLES_SHA1_FULL
          " WHERE sha1sum1=:sha1Sum1 AND sha1sum2=:sha1Sum2 AND sha1sum3=:sha1Sum3"
        /* " INDEXED BY " DUF_SQL_TABLES_SD5 "_uniq WHERE  sha1sum1=:sha1Sum1 AND sha1sum2=:sha1Sum2 AND sha1sum3=:sha1Sum3 */
          ;

  /* DUF_START(  ); */

  DUF_SQL_SE_START_STMT( pdi, select_sha1, sql,  pstmt );
  MAST_TRACE( select, 3, "S:%s", sql );
  DUF_SQL_SE_BIND_LL( sha1Sum1, sha1sum1,  pstmt );
  DUF_SQL_SE_BIND_LL( sha1Sum2, sha1sum2,  pstmt );
  DUF_SQL_SE_BIND_LL( sha1Sum3, sha1sum3,  pstmt );
  DUF_SQL_SE_STEP(  pstmt );
  if ( QISERR1_N(SQL_ROW ) )
  {
    MAST_TRACE( select, 10, "<selected>" );
  /* sha1id = duf_sql_column_long_long( pstmt, 0 ); */
    sha1id = DUF_GET_UFIELD2( sha1id );
  /* rpr = 0; */
  }
  else
  {
  /* DUF_TEST_R( rpr ); */
    MAST_TRACE( select, 10, "<NOT selected> (%d)", QERRIND );
  }
/* DUF_TEST_R( rpr ); */
  DUF_SQL_SE_END_STMT( pdi, select_sha1,  pstmt );
  /* if ( pr ) */
    /* *pr = rpr; */
  /* DUF_ENDULL( sha1id ); */
ERP(MOD,unsigned long long, sha1id,0,pdistat2file_sha1id_existed, duf_depthinfo_t * pdi, unsigned long sha1sum1, unsigned long sha1sum2, unsigned long sha1sum3 );
}

SRP(MOD,unsigned long long,sha1id,-1,insert_sha1_uni, duf_depthinfo_t * pdi, unsigned long long *sha1, const char *msg MAS_UNUSED, int need_id )
{
  /* unsigned long long sha1id = -1; */
  /* int lr = 0; */
  int changes = 0;

#ifdef MAS_TRACING
  const char *real_path = duf_levinfo_path( pdi );
#endif

  /* DUF_START(  ); */

  assert( sizeof( unsigned long long ) == 8 );
  assert( SHA_DIGEST_LENGTH == 2 * sizeof( unsigned long long ) + ASHA1_DELTA );
  assert( SHA_DIGEST_LENGTH == 2 * 64 / 8 + ASHA1_DELTA );
  if ( sha1 && sha1[2] && sha1[1] && sha1[0] )
  {
    if ( !DUF_CONFIGG( opt.disable.flag.insert ) )
    {
      static const char *sql =
              "INSERT OR IGNORE INTO " DUF_SQL_TABLES_SHA1_FULL " ( sha1sum1, sha1sum2, sha1sum3 ) VALUES ( :sha1sum1, :sha1sum2, :sha1sum3 )";

      MAST_TRACE( sha1, 0, "%08llx%016llx%016llx %s%s", sha1[2], sha1[1], sha1[0], real_path, msg );
      DUF_SQL_SE_START_STMT( pdi, insert_sha1, sql,  pstmt );
      MAST_TRACE( insert, 0, "S:%s", sql );
      DUF_SQL_SE_BIND_LL( sha1sum1, sha1[2],  pstmt );
      DUF_SQL_SE_BIND_LL( sha1sum2, sha1[1],  pstmt );
      DUF_SQL_SE_BIND_LL( sha1sum3, sha1[0],  pstmt );
      DUF_SQL_SE_STEPC(  pstmt );
      DUF_SQL_SE_CHANGES( changes,  pstmt );
      DUF_SQL_SE_END_STMT( pdi, insert_sha1,  pstmt );
    }
    duf_pdi_reg_changes( pdi, changes );
    if ( ( QISERR1_N(SQL_CONSTRAINT ) || QNOERR ) && !changes )
    {
      if ( need_id )
        sha1id = duf_pdistat2file_sha1id_existed( pdi, sha1[2], sha1[1], sha1[0], QPERRIND );
    }
    else if ( QNOERR /* assume SQLITE_OK */  )
    {
      if ( need_id && changes )
      {
        sha1id = duf_sql_last_insert_rowid(  );
      }
    }
  /* else                                      */
  /* {                                         */
  /*   DUF_SHOW_ERROR( "insert sha1 %d", lr ); */
  /* }                                         */
  }
  else
  {
  /* DUF_SHOW_ERROR( "Wrong data" ); */
    ERRMAKE(DATA );
  /* DUF_TEST_R( lr ); */
  }

  /* if ( pr ) */
    /* *pr = lr; */

  /* DUF_ENDULL( sha1id ); */
  /* return sha1id; */
ERP(MOD,unsigned long long,sha1id,-1,insert_sha1_uni, duf_depthinfo_t * pdi, unsigned long long *sha1, const char *msg MAS_UNUSED, int need_id );
}

static 
SR(MOD,make_sha1_uni, int fd, unsigned long long *pbytes, unsigned char *pmd )
{
/*   DUF_STARTR( r ) */;
  size_t bufsz = 1024 * 1024 * 1;
  SHA_CTX ctx;

  memset( &ctx, 0, sizeof( ctx ) );
  MAST_TRACE( sha1, 0, "make" );
  {
    char *buffer;

    buffer = mas_malloc( bufsz );
    if ( buffer )
    {
      if ( !DUF_CONFIGG( opt.disable.flag.calculate ) && ( SHA1_Init( &ctx ) != 1 ) )
        ERRMAKE(SHA1 );
/* DUF_TEST_R( r ); */
      {
        int maxcnt = 0;
        int cnt = 0;

        while ( QNOERR && ( maxcnt == 0 || cnt++ < maxcnt ) )
        {
          int ry;

          MAST_TRACE( sha1, 10, "read fd:%u", fd );
          ry = read( fd, buffer, bufsz );
        /* TODO: if (ry>0)  sscbh->bytes+=ry */
          MAST_TRACE( sha1, 10, "read ry:%u", ry );
          if ( ry < 0 )
          {
            /* DUF_ERRSYS( "read file" ); */
            MASE_ERRSYS( "read file" );

            ERRMAKE(READ );
/* DUF_TEST_R( r ); */
            break;
          }
          if ( ry > 0 )
          {
            if ( !DUF_CONFIGG( opt.disable.flag.calculate ) )
            {
              if ( pbytes )
                ( *pbytes ) += ry;
              if ( SHA1_Update( &ctx, buffer, ry ) != 1 )
                ERRMAKE(SHA1 );
            }
          }
          if ( ry <= 0 )
            break;
/* DUF_TEST_R( r ); */
        }
      }
      mas_free( buffer );
    }
    else
    {
      ERRMAKE(MEMORY );
    }
  }
  if ( !DUF_CONFIGG( opt.disable.flag.calculate ) && SHA1_Final( pmd, &ctx ) != 1 )
    ERRMAKE(SHA1 );
/*  DUF_ENDR( r );*/
ER(MOD,make_sha1_uni, int fd, unsigned long long *pbytes, unsigned char *pmd );
}

static 
SR(MOD,make_sha1r_uni, duf_depthinfo_t * pdi, unsigned char *pmdr )
{
/*   DUF_STARTR( r ) */;

  unsigned char asha1[SHA_DIGEST_LENGTH];
  unsigned long long bytes = 0;
  int fd;

  memset( asha1, 0, sizeof( asha1 ) );
  fd = duf_levinfo_dfd( pdi );
CR(make_sha1_uni, fd, &bytes, asha1 ) ;
/* reverse */
  for ( unsigned i = 0; i < sizeof( asha1 ) / sizeof( asha1[0] ); i++ )
    pmdr[i] = asha1[sizeof( asha1 ) / sizeof( asha1[0] ) - i - 1];

  pdi->total_bytes += bytes;
  pdi->total_files++;

/*  DUF_ENDR( r );*/
ER(MOD,make_sha1r_uni, duf_depthinfo_t * pdi, unsigned char *pmdr );
}

static 
SR(MOD,sha1_dirent_content2, duf_stmnt_t * pstmt, duf_depthinfo_t * pdi )
{
/*   DUF_STARTR( r ) */;
  unsigned char asha1r[SHA_DIGEST_LENGTH + ASHA1_DELTA];

  DUF_SFIELD2( fname );
  MAST_TRACE( sha1, 0, "+ %s", fname );
  memset( asha1r, 0, sizeof( asha1r ) );
  if ( !DUF_CONFIGG( opt.disable.flag.calculate ) )
CR(make_sha1r_uni, pdi, asha1r ) ;

  assert( sizeof( asha1r ) == 20 + ASHA1_DELTA );
  if ( QNOERR )
  {
    unsigned long long sha1id = 0;
    unsigned long long *pmd = ( unsigned long long * ) &asha1r;

    MAST_TRACE( sha1, 0, "insert %s", fname );

    sha1id = duf_insert_sha1_uni( pdi, pmd, fname /* for dbg message only */ , 1 /*need_id */ , QPERRIND );
    if ( sha1id )
    {
      int changes = 0;

      pdi->cnts.dirent_content2++;
      if ( !DUF_CONFIGG( opt.disable.flag.update ) )
      {
        DUF_UFIELD2( filedataid );
#if 0
        DOR( r,
             duf_sql( "UPDATE " DUF_SQL_TABLES_FILEDATAS_FULL " SET sha1id='%llu' WHERE " DUF_SQL_IDFIELD "='%lld'", &changes, sha1id, filedataid ) );
#else
        const char *sql = "UPDATE " DUF_SQL_TABLES_FILEDATAS_FULL " SET sha1id=:sha1Id WHERE " DUF_SQL_IDFIELD " =:dataId ";

        DUF_SQL_SE_START_STMT( pdi, update_sha1id, sql,  pstmt );
        MAST_TRACE( mod, 3, "S:%s", sql );
        DUF_SQL_SE_BIND_LL( sha1Id, sha1id,  pstmt );
        DUF_SQL_SE_BIND_LL( dataId, filedataid,  pstmt );
        DUF_SQL_SE_STEPC(  pstmt );
        DUF_SQL_SE_CHANGES( changes,  pstmt );
        DUF_SQL_SE_END_STMT( pdi, update_sha1id,  pstmt );
#endif
      }
      duf_pdi_reg_changes( pdi, changes );
/* DUF_TEST_R( r ); */
    }
    MAST_TRACE( sha1, 4, "%02x:%2x:%2x:%2x:%2x:%2x", asha1r[SHA_DIGEST_LENGTH - 0], asha1r[SHA_DIGEST_LENGTH - 1], asha1r[SHA_DIGEST_LENGTH - 2],
                asha1r[SHA_DIGEST_LENGTH - 3], asha1r[SHA_DIGEST_LENGTH - 4], asha1r[SHA_DIGEST_LENGTH - 5] );
    MAST_TRACE( sha1, 0, "%08llx%016llx%016llx : sha1id: %llu", pmd[2], pmd[1], pmd[0], sha1id );
  /* MAST_TRACE( scan, 12, "  " DUF_DEPTH_PFMT ": scan 5    * %016llx%016llx : %llu", duf_pdi_depth( pdi ), pmd[1], pmd[0], sha1id ); */
  }
/*  DUF_ENDR( r );*/
ER(MOD,sha1_dirent_content2, duf_stmnt_t * pstmt, duf_depthinfo_t * pdi );
}
