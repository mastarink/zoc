/* #define DUF_GET_FIELD_FROM_ROW */
/* #undef MAS_TRACING */
#include <assert.h>                                                  /* assert */
#include <stddef.h>                                                  /* NULL */
#include <string.h>

#include "duf_tracen_defs_preset.h"                                  /* MAST_TRACE_CONFIG; etc. ✗ */
#include "duf_errorn_defs_preset.h"                                  /* MAST_ERRORS_FILE; etc. ✗ */

#include <mastar/wrap/mas_std_def.h>
#include <mastar/trace/mas_trace.h>
#include <mastar/error/mas_error_defs_ctrl.h>
#include <mastar/error/mas_error_defs_make.h>
#include <mastar/error/mas_error_defs.h>

#include "duf_sccb_types.h"                                          /* duf_scan_callbacks_t; duf_sccb_handle_t; duf_sccb_data_row_t ✗ */
#include "duf_sccb_structs.h"

#include "sql_beginning_types.h"                                     /* duf_sql_sequence_t */

#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ✗ */

#include "duf_levinfo_ref.h"                                         /* duf_levinfo_*; etc. ✗ */

#include "duf_sccb_row_field_defs.h"                                 /* DUF_*FIELD2* ✗ */

#include "duf_sccbh_ref.h"
#include "duf_sccbh_shortcuts.h"                                     /* H_SCCB; H_PDI; H_* ... ✗ */

#include "duf_path2db.h"                                             /* duf_real_path2db; etc. ✗ */

#include "sql_beginning_selected.h"
#include "sql_beginning_tables.h"                                    /* DUF_SQL_TABLES... etc. ✗ */

/* ########################################################################################## */
#include "duf_mod_types.h"
/* static int duf_register_pdidirectory(  duf_depthinfo_t * H_PDI, duf_sccb_handle_t * sccbh MAS_UNUSED ); */
static DR( MOD, register_pdidirectory, duf_depthinfo_t * pdi_unused, duf_sccb_handle_t * sccbh );

/* ########################################################################################## */

static duf_sql_sequence_t final_sql =                                /* */
{
  .name = "final-directories",
  .done = 0,
  .sql = {
          "DELETE FROM path_pairs"                                   /* */
          ,
          "INSERT OR IGNORE INTO path_pairs (samefiles, Pathid1, Pathid2) SELECT COUNT(*), fna.Pathid AS Pathid1, fnb.Pathid  AS Pathid2" /* */
          " FROM " DUF_SQL_TABLES_FILENAMES_FULL " AS fna"           /* */
          "   JOIN " DUF_SQL_TABLES_FILEDATAS_FULL " AS fda ON (fna.dataid=fda." DUF_SQL_IDFIELD ")" /* */
          "   JOIN " DUF_SQL_TABLES_MD5_FULL " AS mda ON (fda.md5id=mda." DUF_SQL_IDFIELD ")" /* */
          "   JOIN " DUF_SQL_TABLES_FILEDATAS_FULL " AS fdb ON (fdb.md5id=mda." DUF_SQL_IDFIELD ")" /* */
          "   JOIN " DUF_SQL_TABLES_FILENAMES_FULL " AS fnb ON (fdb." DUF_SQL_IDFIELD "=fnb.dataid)" /* */
          " WHERE Pathid1 < Pathid2 AND fna." DUF_SQL_FILENAMEFIELD "=fnb." DUF_SQL_FILENAMEFIELD "" /* */
          " GROUP BY Pathid1, Pathid2"                               /* */
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
   .type = DUF_NODE_NODE,                                            /* */
   .scanstage = DUF_SCANSTAGE_FS_ITEMS,                              /* */
   .fun = F2ND( register_pdidirectory ),                             /* */
   },

  {.fun = NULL}
};

static duf_scan_callbacks_t duf_sccb_dispatch = {
  .title = "directories",
  .name = "dirs",
  .init_scan = NULL,
  .def_opendir = 1,

#if 0
  .dirent_dir_scan_before2 = F2ND( register_pdidirectory ),
#elif 1
  .scanners = scanners,
#endif
  .no_count = 1,
  .no_progress = 1,
  .count_nodes = 1,
#if 0
# if 0
  .beginning_sql_seq = &sql_create_selected,
# else
  .beginning_sql_seq = &sql_update_selected,
# endif
#endif
/* TODO : explain values of use_std_leaf_set_num and use_std_node_set_num TODO */
  .use_std_leaf_set_num = 1,                                         /* 1 : preliminary selection; 2 : direct (beginning_sql_seq=NULL recommended in many cases) */
  .use_std_node_set_num = 2,                                         /* 1 : preliminary selection; 2 : direct (beginning_sql_seq=NULL recommended in many cases) */
  .std_leaf_set_name = "std-leaf-one",
  .std_node_set_name = "std-node-two",
/* filename for debug only */
  .leaf = {                                                          /* */
           .name = "dirs-leaf",
           .type = DUF_NODE_LEAF,
           .fieldset =                                               /* Never used!? */
           NULL},
  .node = {                                                          /* */
           .name = "dirs-node",
           .type = DUF_NODE_NODE,
           .expand_sql = 1,                                          /* */
           .fieldset =                                               /* */
         /* "'dirs-node' AS fieldset_id, " (* *) */
           " pt." DUF_SQL_IDFIELD " AS dirid "                       /* */
           ", pt." DUF_SQL_IDFIELD " AS nameid "                     /* */
           ", pt." DUF_SQL_DIRNAMEFIELD " AS dname, pt." DUF_SQL_DIRNAMEFIELD " AS dfname,  pt.parentid " /* */
           ", pt.size AS filesize, pt.mode AS filemode, pt.dev, pt.uid, pt.gid, pt.nlink, pt.inode, pt.rdev, pt.blksize, pt.blocks, STRFTIME( '%s', pt.mtim ) AS mtime " /* */
           ,
           .selector2 =                                              /* */
           " FROM " DUF_SQL_TABLES_PATHS_FULL " AS pt "              /* */
           ,
           .matcher = "pt.parentid = :parentdirID  AND ( :dirName IS NULL OR dname=:dirName ) " /* */
           ,
           .filter = NULL                                            /* */
           },
  .final_sql_seq = &final_sql
};

/* ########################################################################################## */

/* make sure dir name in db */

SR( MOD, register_pdidirectory, duf_depthinfo_t * pdi_unused MAS_UNUSED, duf_sccb_handle_t * sccbh MAS_UNUSED )
{
#if 0
  assert( 0 == strcmp( fname_unused, duf_levinfo_itemname( H_PDI ) ) );
  {
    struct stat *st = duf_levinfo_stat( H_PDI );

    assert( st->st_dev == pst_dir_unused->st_dev );
    assert( st->st_ino == pst_dir_unused->st_ino );
    assert( st->st_mode == pst_dir_unused->st_mode );
    assert( st->st_nlink == pst_dir_unused->st_nlink );
    assert( st->st_uid == pst_dir_unused->st_uid );
    assert( st->st_gid == pst_dir_unused->st_gid );
    assert( st->st_rdev == pst_dir_unused->st_rdev );
    assert( st->st_size == pst_dir_unused->st_size );
    assert( st->st_blksize == pst_dir_unused->st_blksize );
    assert( st->st_blocks == pst_dir_unused->st_blocks );
  /* assert( st->st_atim == pst_dir_unused->st_atim ); */
  /* assert( st->st_mtim == pst_dir_unused->st_mtim ); */
  /* assert( st->st_ctim == pst_dir_unused->st_ctim ); */
    assert( 0 == memcmp( st, pst_dir_unused, sizeof( struct stat ) ) );
    assert( pst_dir_unused == st );
  }
#endif

/* fname === */
  MAST_TRACE( mod, 0, "@ scan entry dir 2 by %s", duf_levinfo_itemshowname( H_PDI ) );

  CR( levinfo_stat2dirid, H_PDI, 1 /* caninsert */ ,
      &duf_sccb_dispatch.node /*, 0 need_id - no error (1=error) if there is no record */  );

  ER( MOD, register_pdidirectory, duf_depthinfo_t * pdi_unused, duf_sccb_handle_t * sccbh MAS_UNUSED );
}
