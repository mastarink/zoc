#include <stdarg.h>
#include <string.h>
#include <getopt.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_types.h"

#include "duf_dbg.h"

/* ###################################################################### */
#include "duf_config.h"
/* ###################################################################### */

duf_config_t *duf_config = NULL;


int
duf_config_create( void )
{
  duf_dbgfunc( DBG_START, __func__, __LINE__ );
  duf_config = mas_malloc( sizeof( duf_config_t ) );
  memset( duf_config, 0, sizeof( duf_config ) );
  duf_config->u.maxdepth = 100;
  duf_config->db_dir = mas_strdup( getenv( "MSH_SHN_PROJECTS_DIR" ) );
  duf_config->db_dir = mas_strcat_x( duf_config->db_dir, "/../duf_db" );
  duf_config->db_name = mas_strdup( "duf-photo.sqlite3" );
  duf_dbgfunc( DBG_END, __func__, __LINE__ );
  return 0;
}

int
duf_config_delete( void )
{
  duf_dbgfunc( DBG_START, __func__, __LINE__ );
  mas_free( duf_config->db_dir );
  duf_config->db_dir = NULL;

  mas_free( duf_config->db_name );
  duf_config->db_name = NULL;

  mas_free( duf_config->group );
  duf_config->group = NULL;

  mas_del_argv( duf_config->targc, duf_config->targv, 0 );
  duf_config->targc = 0;
  duf_config->targv = NULL;



  mas_free( duf_config );
  duf_config = NULL;
  duf_dbgfunc( DBG_END, __func__, __LINE__ );
  return 0;
}

int
duf_config_show( void )
{
  duf_dbgfunc( DBG_START, __func__, __LINE__ );
  if ( duf_config )
  {
    fprintf( stderr, "db_dir: %s\n", duf_config->db_dir );
  }
  for ( int ia = 0; ia < duf_config->targc; ia++ )
  {
    fprintf( stderr, "targv[%d]: %s\n", ia, duf_config->targv[ia] );
  }
  duf_dbgfunc( DBG_END, __func__, __LINE__ );
  return 0;
}
