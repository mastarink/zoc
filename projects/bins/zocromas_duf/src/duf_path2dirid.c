#include <assert.h>                                                  /* assert */
#include <string.h>

#include "duf_tracen_defs_preset.h"                                  /* MAST_TRACE_CONFIG; etc. ✗ */
#include "duf_errorn_defs_preset.h"                                  /* MAST_ERRORS_FILE; etc. ✗ */

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>                                  /* mas_(malloc|free|strdup); etc. ▤ */
#include <mastar/tools/mas_arg_tools.h>                              /* mas_strcat_x; etc. ▤ */
#include <mastar/tools/mas_utils_path.h>                             /* mas_normalize_path; mas_pathdepth; mas_realpath etc. ▤ */
#include <mastar/trace/mas_trace.h>
#include <mastar/error/mas_error_defs_ctrl.h>
#include <mastar/error/mas_error_defs_make.h>
#include <mastar/error/mas_error_defs.h>

#include "duf_config.h"                                              /* duf_get_config ✗ */
#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ✗ */

#include "duf_levinfo_ref.h"                                         /* duf_levinfo_*; etc. ✗ */

#include "duf_pdi.h"                                                 /* duf_pdi_init; duf_pdi_shut; duf_pdi_close ✗ */
#include "duf_pdi_stmt.h"                                            /* duf_pdi_find_statement_by_id; etc. ✗ */

#include "duf_sccb_row_field_defs.h"                                 /* DUF_*FIELD2* ✗ */

#include "duf_sql_defs.h"                                            /* DUF_SQL_IDFIELD etc. ✗ */
#include "duf_sql_se_stmt_defs.h"                                    /* DUF_SQL_SE_BIND_S_OPT etc. ✗ */

#include "duf_evsql_selector_new.h"                                  /* duf_selector2sql_new; duf_selector2sql_2new; duf_expand_sql; ✗ */

#include "duf_sql_bind.h"                                            /* duf_sql_... for DUF_SQL_BIND_... etc. ✗ */
#include "duf_sql_prepared.h"                                        /* duf_sql_prepare; duf_sql_step; duf_sql_finalize; ✗ */
#include "duf_sql_field.h"                                           /* __duf_sql_str_by_name2 for DUF_GET_QUFIELD2 etc. ✗ */

#include "sql_beginning_tables.h"                                    /* DUF_SQL_TABLES... etc. ✗ */

#include "duf_pdi_structs.h"

/* ###################################################################### */
#include "duf_path2dirid.h"                                          /* duf_dirid2path; etc. ✗ */
/* ###################################################################### */

/* 20150901.121302
 * use temporary inited pdi
 * to get dirid for path
 * */
SRP( OTHER, unsigned long long, dirid, 0, path2dirid, const char *path )
{
  char *real_path;

  real_path = mas_realpath( path /* , &rpr */  );
  if ( !real_path )
    ERRMAKE( PATH );

  if ( QNOERR )
  {
    duf_depthinfo_t di = {.pdi_name = "path2dirid" };

    CR( pdi_init_min_r, &di, real_path );
    if ( QNOERR )
      dirid = duf_levinfo_dirid( &di );
  /* xchanges = di.changes; --- needless!? */
    duf_pdi_close( &di );
  }
  mas_free( real_path );
  ERP( OTHER, unsigned long long, dirid, 0, path2dirid, const char *path );
}

static
SRP( PDI, char *, name, NULL, dirid2name_existed_i, duf_depthinfo_t * pdi, const char *sqlv, unsigned long long dirid, unsigned long long *pparentid )
{
  DUF_SQL_SE_START_STMT( pdi, dirid2name_existed, sqlv, pstmt_local );
  {
    DUF_SQL_SE_BIND_LL( dirID, dirid, pstmt_local );
    DUF_SQL_SE_STEP( pstmt_local );
    if ( QISERR1_N( SQL_ROW ) )
    {
    /* rpr = 0; */
      MAST_TRACE( select, 0, "<selected> %s", sqlv );

      name = mas_strdup( DUF_GET_QSFIELD3( pstmt_local, name ) );
      if ( pparentid )
        *pparentid = DUF_GET_QUFIELD3( pstmt_local, parentid );
    }
    else
    {
      MAST_TRACE( select, 10, "<NOT selected> (%d)", QERRIND );
    }
  }
  DUF_SQL_SE_END_STMT( pdi, dirid2name_existed, pstmt_local );
  ERP( PDI, char *, name, NULL, dirid2name_existed_i, duf_depthinfo_t * pdi, const char *sqlv, unsigned long long dirid,
       unsigned long long *pparentid );
}

SRP( OTHER, char *, name, NULL, dirid2name_existed, duf_depthinfo_t * pdi, unsigned long long dirid, unsigned long long *pparentid )
{
  char *sqlv = NULL;

  duf_sql_set_t def_node_set = {
    .fieldset = "pt.parentid AS parentid, pt." DUF_SQL_DIRNAMEFIELD " AS name " /* */
            ,
    .selector2 = " FROM " DUF_SQL_TABLES_PATHS_FULL " AS pt "        /* */
    " WHERE pt." DUF_SQL_IDFIELD "=:dirID"
  };

  assert( pdi );

  {
#if 0
    sqlv = duf_selector2sql( &def_node_set, pdi->pdi_name, pr );
#else
    sqlv = duf_selector2sql_new( &def_node_set, 0 /* orderid */ , pdi->pdi_name, 0 /*total */ , QPERRIND );
#endif
    if ( sqlv )
    {
      name = duf_dirid2name_existed_i( pdi, sqlv, dirid, pparentid, QPERRIND );

      mas_free( sqlv );
    }
  }
  ERP( OTHER, char *, name, NULL, dirid2name_existed, duf_depthinfo_t * pdi, unsigned long long dirid, unsigned long long *pparentid );
}

SRP( OTHER, char *, path, NULL, dirid2path, unsigned long long dirid )
{
  int done = 0;
  int depth = 0;

  duf_depthinfo_t di = {.pdi_name = "dirid2path" };
#if 0
  DOR( rpr, DUF_WRAPPED( duf_pdi_init ) ( &di, NULL /* pu */ , NULL /* real_path */ , NULL /* sql_set */ , 0 /* caninsert */ , 1 /* recursive */ ,
                                          0 /* linear */ ,
                                          0 /* opendir */  ) );
#else
  CR( pdi_init_min_r, &di, NULL /* real_path */  );
#endif
  do
  {
    char *name = NULL;

    name = duf_dirid2name_existed( &di, dirid, &dirid, QPERRIND );
    if ( name )
    {
      char *opath;

      opath = path;
      path = mas_normalize_path_plus( *name ? name : "/", opath, NULL );
      mas_free( opath );
    /* T( "(%llu) '%s' => path:%s", dirid, name, path ); */
      depth++;
    }
    else
      done = 1;
    mas_free( name );
  } while ( !done );
  duf_pdi_close( &di );

  ERP( OTHER, char *, path, NULL, dirid2path, unsigned long long dirid );
}
