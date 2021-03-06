/* #undef MAS_TRACING */
#define MAST_TRACE_CONFIG muc_get_cli_options_trace_config(cli)
#include <assert.h>                                                  /* assert */
#include <string.h>

#include "duf_tracen_defs_preset.h"                                  /* MAST_TRACE_CONFIG; etc. ✗ */
#include "duf_errorn_defs_preset.h"                                  /* MAST_ERRORS_FILE; etc. ✗ */

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>                                  /* mas_(malloc|free|strdup); etc. ▤ */
#include <mastar/tools/mas_arg_tools.h>                              /* mas_strcat_x; etc. ▤ */
#include <mastar/trace/mas_trace.h>
#include <mastar/error/mas_error_defs_ctrl.h>
#include <mastar/error/mas_error_defs_make.h>
#include <mastar/error/mas_error_defs.h>

#include <mastar/multiconfig/muc_option_config.h>

#include "duf_se_only.h"                                             /* Only DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ✗ */

#include "duf_evsql_begfin.h"                                        /* duf_eval_sqlsq() ✗ */
#include "duf_sql_se_stmt_defs.h"                                    /* DUF_SQL_SE_BIND_S_OPT etc. ✗ */
#include "duf_sql_bind.h"                                            /* duf_sql_... for DUF_SQL_BIND_... etc. ✗ */

#include "sql_beginning_tables.h"                                    /* DUF_SQL_TABLES... etc. ✗ */

/* ###################################################################### */
#include "duf_optmod_tmpdb.h"
/* ###################################################################### */

/* TODO test, if still working */
static
SR( OPTIONS, bind_tmp, duf_stmnt_t * pstmt_arg, const duf_ufilter_t * pu_unused MAS_UNUSED, const duf_yfilter_t * py_unused MAS_UNUSED,
    const mas_argvc_t * ttarg, const void *ptr )
{
/* DUF_STARTR( r ); */
  muc_config_cli_t *cli = NULL;

  cli = ( muc_config_cli_t * ) ptr;
  if ( ttarg && ttarg->argc >= 2 && ttarg->argv )
  {
  /* MAST_TRACE( temp, 0, "%d:0 >>>> %s", ttarg->argc, ttarg->argv[0] ); */
  /* MAST_TRACE( temp, 0, "%d:1 >>>> %s", ttarg->argc, ttarg->argv[1] ); */
  /* MAST_TRACE( temp, 0, "%d:2 >>>> %s", ttarg->argc, ttarg->argv[2] ); */
    DUF_SQL_SE_BIND_S_OPTQ( optVal, ttarg->argv[0], pstmt_arg );
    DUF_SQL_SE_BIND_S_OPTQ( optName, ttarg->argv[1], pstmt_arg );
    DUF_SQL_SE_BIND_S_OPTQ( optArg, ttarg->argv[2], pstmt_arg );
  /* MAST_TRACE( sql, 0, "@@@@bind: argv[]={'%s', '%s', '%s'}", ttarg->argv[0], ttarg->argv[1], ttarg->argv[2] ); */
  }
/* DUF_ENDR( r ); */
  ER( OPTIONS, bind_tmp, duf_stmnt_t * pstmt_arg, const duf_ufilter_t * pu_unused MAS_UNUSED, const duf_yfilter_t * py_unused MAS_UNUSED,
      const mas_argvc_t * ttarg, const void *ptr );
}

muc_SR( OPTIONS, tmpdb_add, muc_config_cli_t * cli, int oval, const char *optname, const char *optargg )
{
/* DUF_STARTR( r ); */
  static duf_sql_sequence_t tmpseq = {                               /* */
    .name = "tmpseq",
    .sql = {
          /* "DELETE FROM " DUF_SQL_TABLES_TMP_TDB_OPTIONS_FULL " WHERE oval=:optVal AND name=:optName AND arg=:optArg" (* *) , */
            "INSERT OR IGNORE INTO " DUF_SQL_TABLES_TMP_TDB_OPTIONS_FULL " (oval, name, arg) VALUES (:optVal, :optName, :optArg) " /* */ ,
            NULL}
  };
  char optval[512];
  mas_argvc_t ttarg = {.argc = 0,.argv = NULL };
  snprintf( optval, sizeof( optval ), "%d", oval );
  ttarg.argc = mas_add_argv_arg( ttarg.argc, &ttarg.argv, optval );
  ttarg.argc = mas_add_argv_arg( ttarg.argc, &ttarg.argv, optname );
  ttarg.argc = mas_add_argv_arg( ttarg.argc, &ttarg.argv, optargg );

  CR( eval_sqlsq_cb, &tmpseq, NULL /* title */ , ( duf_ufilter_t * ) NULL, ( duf_yfilter_t * ) NULL, duf_bind_tmp, &ttarg,
      NULL /* selected_db */ , cli /* ptr */  );
  mas_del_argv( ttarg.argc, ttarg.argv, 0 );
/* DUF_ENDR( r ); */
  muc_ER( OPTIONS, tmpdb_add, muc_config_cli_t * cli, int oval, const char *optname, const char *optargg );
}
