/* #undef MAS_TRACING */
#include <assert.h>                                                  /* assert */
#include <stddef.h>                                                  /* NULL */
#include <string.h>
#include <unistd.h>

#include "duf_tracen_defs_preset.h"                                  /* MAST_TRACE_CONFIG; etc. ♠ */
#include "duf_errorn_defs_preset.h"

#include <mastar/wrap/mas_std_def.h>
#include <mastar/trace/mas_trace.h>
#include <mastar/error/mas_error_defs_ctrl.h>
#include <mastar/error/mas_error_defs_make.h>
#include <mastar/error/mas_error_defs.h>

#include "duf_sccb_types.h"                                          /* duf_scan_callbacks_t ♠ */

/*  #include "duf_tracen_defs.h"  (*  T; TT; TR ♠  *) */
/*  #include "duf_errorn_defs.h"  (*  DUF_NOERROR; DUF_CLEAR_ERROR; DUF_E_(LOWER|UPPER); DUF_TEST_R ... ♠  *) */

/*  #include "duf_start_end.h"  (*  DUF_STARTR ; DUF_ENDR ♠  *) */
/*  #include "duf_dodefs.h"  (*  DOR ♠  *) */

#include "duf_config.h"                                              /* duf_get_config ♠ */
#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ♠ */
/* #include "duf_config_ref.h" */
#include "duf_config_defs.h"                                         /* DUF_CONF... ♠ */

#include "duf_levinfo_openclose.h"
#include "duf_levinfo_stat.h"
#include "duf_levinfo_ref.h"                                         /* duf_levinfo_*; etc. ♠ */

#include "duf_sql_defs.h"                                            /* DUF_SQL_IDFIELD etc. ♠ */
#include "duf_sql_field.h"                                           /* __duf_sql_str_by_name2 for DUF_GET_UFIELD2 etc. ♠ */

/* #include "duf_option_defs.h" */

/* #include "duf_dbg.h" */
#include "duf_mod_defs.h"

#include "sql_beginning_selected.h"
#include "sql_beginning_tables.h"                                    /* DUF_SQL_TABLES... etc. ♠ */

#include "duf_tags.h"

/* ########################################################################################## */
/* DUF_MOD_DECLARE_ALL_FUNCS( tagit ) */
static int duf_tagit_init( duf_stmnt_t * pstmt_unused MAS_UNUSED, duf_depthinfo_t * pdi );
static int duf_tagit_leaf2( duf_stmnt_t * pstmt, duf_depthinfo_t * pdi );
static int duf_tagit_node_before2( duf_stmnt_t * pstmt_unused MAS_UNUSED, duf_depthinfo_t * pdi );
static int duf_tagit_node_middle2( duf_stmnt_t * pstmt_unused MAS_UNUSED, duf_depthinfo_t * pdi );
static int duf_tagit_node_after2( duf_stmnt_t * pstmt_unused MAS_UNUSED, duf_depthinfo_t * pdi );

/* ########################################################################################## */

/* ########################################################################################## */

duf_scan_callbacks_t duf_tagit_callbacks = {
  .title = "tag it",
  .name = "tagit",
  .def_opendir = 0,
  .init_scan = duf_tagit_init,
#if 0
  .beginning_sql_seq = &sql_create_selected,
#elif 0
  .beginning_sql_seq = &sql_update_selected,
#endif

  .node_scan_before2 = duf_tagit_node_before2,
/* .node_scan_before2_deleted = duf_tagit_node_before2_del, */

  .node_scan_after2 = duf_tagit_node_after2,
/* .node_scan_after2_deleted = duf_tagit_node_after2_del, */

  .node_scan_middle2 = duf_tagit_node_middle2,
/* .node_scan_middle2_deleted = duf_tagit_node_middle2_del, */

/* .leaf_scan_fd2 = duf_tagit_de_content2, */
/* .leaf_scan_fd2_deleted = duf_tagit_de_content2_del, */

  .leaf_scan2 = duf_tagit_leaf2,
/* .leaf_scan2_deleted = duf_tagit_leaf2_del, */

/* .dirent_file_scan_before2 = duf_tagit_de_file_before2, */
/* .dirent_dir_scan_before2 = duf_tagit_de_dir_before2, */

/* TODO : explain values of use_std_leaf_set_num and use_std_node_set_num TODO */
  .use_std_leaf_set_num = 2,                                         /* 1 : preliminary selection; 2 : direct (beginning_sql_seq=NULL recommended in many cases) */
  .use_std_node_set_num = 2,                                         /* 1 : preliminary selection; 2 : direct (beginning_sql_seq=NULL recommended in many cases) */
  .std_leaf_set_name = "std-leaf-no-sel",
  .std_node_set_name = "std-node-two",
};

/* ########################################################################################## */

static 
SR(MOD,tagit_init, duf_stmnt_t * pstmt_unused MAS_UNUSED, duf_depthinfo_t * pdi MAS_UNUSED )
{
/*   DUF_STARTR( r ) */;

  MAST_TRACE( mod, 0, "tagit_init %s", duf_levinfo_path( pdi ) );

/*  DUF_ENDR( r );*/
ER(MOD,tagit_init, duf_stmnt_t * pstmt_unused , duf_depthinfo_t * pdi  );
}

static 
SR(MOD,tagit_leaf2, duf_stmnt_t * pstmt MAS_UNUSED, duf_depthinfo_t * pdi )
{
/*   DUF_STARTR( r ) */;
  if ( DUF_CONFIGG( vars.tag.file ) )
  {
#ifdef MAS_TRACING
    assert( !duf_levinfo_dfd( pdi ) );
  /* filename from db same as duf_levinfo_itemname( pdi ) */
    assert( 0 == strcmp( DUF_GET_SFIELD2( fname ), duf_levinfo_itemtruename( pdi ) ) );
    assert( duf_levinfo_dbstat( pdi ) );
#endif

    duf_add_tag( pdi, "filename", duf_levinfo_nameid( pdi ), DUF_CONFIGG( vars.tag.file ) ? DUF_CONFIGG( vars.tag.file ) : "NONE", QPERRIND );

    MAST_TRACE( mod, 2, "@@tagit %s", duf_levinfo_path( pdi ) );
  }
/*  DUF_ENDR( r );*/
ER(MOD,tagit_leaf2, duf_stmnt_t * pstmt , duf_depthinfo_t * pdi );
}

static 
SR(MOD,tagit_node_before2, duf_stmnt_t * pstmt_unused MAS_UNUSED, duf_depthinfo_t * pdi MAS_UNUSED )
{
/*   DUF_STARTR( r ) */;
  if ( DUF_CONFIGG( vars.tag.dir ) )
  {
    MAST_TRACE( mod, 1, "(%s:%s)tagit %s : %s", DUF_CONFIGG( vars.tag.dir ), DUF_CONFIGG( vars.tag.file ), duf_levinfo_path( pdi ),
                duf_levinfo_itemshowname( pdi ) );

#if 0
# ifdef MAS_TRACING
    DUF_SFIELD2( fname );
# endif
    MAST_TRACE( mod, 1, "tagit %s : %s", duf_levinfo_path( pdi ), filename );
#endif
  }
/*  DUF_ENDR( r );*/
ER(MOD,tagit_node_before2, duf_stmnt_t * pstmt_unused , duf_depthinfo_t * pdi  );
}

static 
SR(MOD,tagit_node_middle2, duf_stmnt_t * pstmt_unused MAS_UNUSED, duf_depthinfo_t * pdi MAS_UNUSED )
{
/*   DUF_STARTR( r ) */;

  if ( DUF_CONFIGG( vars.tag.dir ) )
  {
    MAST_TRACE( mod, 1, "(%s:%s)tagit %s : %s", DUF_CONFIGG( vars.tag.dir ), DUF_CONFIGG( vars.tag.file ), duf_levinfo_path( pdi ),
                duf_levinfo_itemshowname( pdi ) );

#if 0
# ifdef MAS_TRACING
    DUF_SFIELD2( fname );
# endif
    MAST_TRACE( mod, 1, "tagit node middle: %s : %s", duf_levinfo_path( pdi ), filename );
#endif
  }
/*  DUF_ENDR( r );*/
ER(MOD,tagit_node_middle2, duf_stmnt_t * pstmt_unused , duf_depthinfo_t * pdi  );
}

static 
SR(MOD,tagit_node_after2, duf_stmnt_t * pstmt_unused MAS_UNUSED, duf_depthinfo_t * pdi MAS_UNUSED )
{
/*   DUF_STARTR( r ) */;

  if ( DUF_CONFIGG( vars.tag.dir ) )
  {
    MAST_TRACE( mod, 1, "(%s:%s)tagit %s : %s", DUF_CONFIGG( vars.tag.dir ), DUF_CONFIGG( vars.tag.file ), duf_levinfo_path( pdi ),
                duf_levinfo_itemshowname( pdi ) );

#if 0
# ifdef MAS_TRACING
    DUF_SFIELD2( fname );
# endif
    MAST_TRACE( mod, 1, "tagit node after: %s : %s", duf_levinfo_path( pdi ), filename );
#endif
  }
/*  DUF_ENDR( r );*/
ER(MOD,tagit_node_after2, duf_stmnt_t * pstmt_unused , duf_depthinfo_t * pdi  );
}
