#define DUF_SQL_PDI_STMT

#include <string.h>
#include <assert.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_arg_tools.h>


#include "duf_maintenance.h"

#include "duf_config_ref.h"
#include "duf_match.h"
#include "duf_utils.h"

#include "duf_levinfo.h"
#include "duf_pdi.h"
#include "duf_sccb.h"

#include "duf_sql_defs.h"
#include "duf_sql_field.h"
#include "duf_sql.h"

/* #include "duf_sql1.h" */
#include "duf_sql2.h"



/* ###################################################################### */
#include "duf_item_match2.h"
/* ###################################################################### */

int
duf_match_leaf2( duf_sqlite_stmt_t * pstmt )
{
  int r;

  DUF_SFIELD2( filename );
  DUF_UFIELD2( filesize );
  DUF_UFIELD2( md5id );
  DUF_UFIELD2OPT( nsame );


  r = ( duf_filename_match( &duf_config->u.glob, filename ) ) && ( duf_lim_matchll( duf_config->u.size, filesize ) )
        && ( duf_lim_match( duf_config->u.same, nsame ) ) && ( duf_lim_match( duf_config->u.md5id, md5id ) );

  /* DUF_PRINTF( 0, "@@@@@@@@@@ %llu -- %llu -- %llu == %d / %d", duf_config->u.size.min, filesize, duf_config->u.size.max, */
  /*             dufOFF_lim_matchll( duf_config->u.size, filesize ), r );                                                      */
  if ( !r )
    DUF_TRACE( match, 0, "NOT MATCH %s (mode 2)", filename );
  return r;
}