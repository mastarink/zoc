/* #define DUF_GET_FIELD_FROM_ROW */
/* #undef MAS_TRACING */
#include <assert.h>                                                  /* assert */
#include <stddef.h>                                                  /* NULL */
#include <fcntl.h>                                                   /* Definition of AT_* constants */

#include "duf_tracen_defs_preset.h"                                  /* MAST_TRACE_CONFIG; etc. ✗ */
#include "duf_errorn_defs_preset.h"                                  /* MAST_ERRORS_FILE; etc. ✗ */

#include <mastar/wrap/mas_std_def.h>
#include <mastar/tools/mas_arg_tools.h>                              /* mas_strcat_x; etc. ▤ */
#include <mastar/trace/mas_trace.h>
#include <mastar/error/mas_error_defs_ctrl.h>
#include <mastar/error/mas_error_defs_make.h>
#include <mastar/error/mas_error_defs.h>

#include "duf_sccb_structs.h"

#include "duf_sccbh_ref.h"
#include "duf_sccbh_shortcuts.h"                                     /* H_SCCB; H_PDI; H_* ... ✗ */

#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ✗ */

#include "duf_levinfo_ref.h"                                         /* duf_levinfo_*; etc. ✗ */

#include "duf_sccb_row_field_defs.h"                                 /* DUF_*FIELD2* ✗ */

#include "duf_sql_defs.h"                                            /* DUF_SQL_IDFIELD etc. ✗ */

#include "duf_filedata.h"

#include "sql_beginning_tables.h"                                    /* DUF_SQL_TABLES... etc. ✗ */

/* ########################################################################################## */
#include "duf_mod_types.h"
static DR( MOD, register_pdifiledata, duf_depthinfo_t * pdi_unused, duf_sccb_handle_t * sccbh );

/* ########################################################################################## */

static duf_sql_sequence_t final_sql =                                /* */
{
  .name = "final-filedata",
  .done = 0,
  .beginend = 0,

  .sql = {
#if 0
          "UPDATE " DUF_SQL_TABLES_MD5_FULL " SET dup5cnt=(SELECT COUNT(*) " /* */
          " FROM " DUF_SQL_TABLES_FILEDATAS_FULL " AS fd "           /* */
          " JOIN " DUF_SQL_TABLES_MD5_FULL " AS md ON (fd.md5id=md." DUF_SQL_IDFIELD ") " /* */
          " WHERE " DUF_SQL_TABLES_MD5_FULL "." DUF_SQL_IDFIELD "=md." DUF_SQL_IDFIELD ")" /* */
        /* " WHERE " DUF_SQL_TABLES_MD5_FULL ".md5sum1=md.md5sum1 AND " DUF_SQL_TABLES_MD5_FULL ".md5sum2=md.md5sum2)" */
          ,
          "UPDATE " DUF_SQL_TABLES_EXIF_FULL " SET dupexifcnt=(SELECT COUNT(*) " /* */
          " FROM " DUF_SQL_TABLES_FILEDATAS_FULL " AS fd "           /* */
          " JOIN " DUF_SQL_TABLES_EXIF_FULL " AS x ON (fd.exifid=x." DUF_SQL_IDFIELD ") " /* */
          " WHERE " DUF_SQL_TABLES_EXIF_FULL "." DUF_SQL_IDFIELD "=x." DUF_SQL_IDFIELD " AND fixed IS NULL ) WHERE fixed IS NULL" /* */
          ,
#endif
        /* XXX Needless XXX "DELETE FROM " DUF_SQL_TABLES_SIZES_FULL (* *) , */
#if 0
          "INSERT OR IGNORE INTO " DUF_SQL_TABLES_SIZES_FULL " (size, dupzcnt) " /* */
        /*     */ "SELECT size, COUNT(*) "
        /*     */ " FROM " DUF_SQL_TABLES_FILEDATAS_FULL " AS fd GROUP BY fd.size"
#else
          "INSERT OR IGNORE INTO " DUF_SQL_TABLES_SIZES_FULL " (size) SELECT size FROM " DUF_SQL_TABLES_FILEDATAS_FULL " AS fd" /* */ ,
          "UPDATE " DUF_SQL_TABLES_SIZES_FULL " SET dupzcnt=( "      /* */
        /*   */ " SELECT COUNT(*) "
        /*     */ " FROM " DUF_SQL_TABLES_SIZES_FULL " AS sz "
        /*     */ " JOIN " DUF_SQL_TABLES_FILEDATAS_FULL " AS fd ON ( fd.size=sz.size ) "
        /*     */ " WHERE " DUF_SQL_TABLES_SIZES_FULL ".size=sz.size "
          " )"                                                       /* */
#endif
        /* */
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
   .flags = DUF_SCANNER_SET_FLAG_DIRENT,                             /* */
   .type = DUF_NODE_LEAF,                                            /* */
   .scanstage = DUF_SCANSTAGE_FS_ITEMS,                              /* */
   .fun = F2ND( register_pdifiledata ),                              /* */
   },

  {.fun = NULL}
};

static duf_scan_callbacks_t duf_sccb_dispatch = {
  .title = "file data",
  .name = "filedata",
  .init_scan = NULL,
  .def_opendir = 1,

/* 20160406.163421 */
#if 0
  .dirent_file_scan_before2 = F2ND( register_pdifiledata ),
#else
  .scanners = scanners,
#endif
/* TODO : explain values of use_std_leaf_set_num and use_std_node_set_num TODO */
  .use_std_leaf_set_num = 2,                                         /* 1 : preliminary selection; 2 : direct (beginning_sql_seq=NULL recommended in many cases) */
  .use_std_node_set_num = 2,                                         /* 1 : preliminary selection; 2 : direct (beginning_sql_seq=NULL recommended in many cases) */
  .std_leaf_set_name = "std-leaf-no-sel",
  .std_node_set_name = "std-node-two",
  .count_nodes = 1,
  .leaf = {                                                          /* */
           .name = "fd-leaf",
           .type = DUF_NODE_LEAF,
           .fieldset =                                               /* Never used!? */
           NULL},
  .node = {                                                          /* */
           .name = "fd-node",
           .type = DUF_NODE_NODE,
           .expand_sql = 1,                                          /* */
#if 1
           .fieldset =                                               /* */
           "'filedata-node' AS fieldset_id, "                        /* */
           "  pt." DUF_SQL_IDFIELD " AS dirid"                       /* */
           ", pt." DUF_SQL_IDFIELD " AS nameid "                     /* */
           ", pt." DUF_SQL_DIRNAMEFIELD " AS dname, pt." DUF_SQL_DIRNAMEFIELD " AS dfname,  pt.parentid " /* */
           ", pt.size AS filesize, pt.mode AS filemode, pt.dev, pt.uid, pt.gid, pt.nlink, pt.inode, pt.rdev, pt.blksize, pt.blocks, STRFTIME( '%s', pt.mtim ) AS mtime " /* */
#endif
           ,
           .selector2 =                                              /* */
           " FROM " DUF_SQL_TABLES_PATHS_FULL " AS pt "              /* */
           ,
           .matcher = "pt.parentid = :parentdirID"                   /* */
         /*     " AND ( :dirName IS NULL OR dname=:dirName ) "    *//* */
           ,
           .filter = NULL                                            /* */
           },
  .final_sql_seq = &final_sql
};

/* ########################################################################################## */

static
SR( MOD, register_pdifiledata, duf_depthinfo_t * pdi_unused MAS_UNUSED, duf_sccb_handle_t * sccbh MAS_UNUSED )
{
  CR( pdistat2file, H_PDI, sccbh );
  ER( MOD, register_pdifiledata, duf_depthinfo_t * pdi_unused, duf_sccb_handle_t * sccbh MAS_UNUSED );
}
