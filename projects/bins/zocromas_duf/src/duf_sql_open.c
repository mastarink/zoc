#include <mastar/sqlite/mas_sqlite.h>
#include <mastar/sqlite/mas_sqlite_const.h>

#include "duf_maintenance.h"

#include "duf_config_ref.h"

#include "duf_sql_error.h"
#include "duf_sql_defs.h"

/* ###################################################################### */
#include "duf_sql_open.h"
/* ###################################################################### */

int
duf_sql_open( const char *dbpath )
{
  DEBUG_STARTR( r );

  DUF_TRACE( explain, 0, "open database if fpath set; fpath:%s", DUF_CONFIGG( db.main.fpath ) );
  DOR_SQLITE( r, mas_sqlite_open( dbpath ) );

  DUF_TRACE( sql, 1, "open database; dbpath:%s : %d", dbpath, r );
  DUF_TRACE( explain, 0, "opened (?%d) database", r );
  DEBUG_ENDR( r );
}

int
duf_sql_close( void )
{
  DEBUG_STARTR( r );
  DOR_SQLITE( r, mas_sqlite_close(  ) );
  DEBUG_ENDR( r );
}