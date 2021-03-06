#define R_GOOD(_r) (!(_r))
#define RGEMSG mysql_error(mas_mysqlpfs_mysql(pfs))
/* #define RGEMSG mysql_error(&pfs->mysql) */
#include "mysqlpfs_defs.h"
#include <malloc.h>
#include <stdio.h>
#include <string.h>

#include <limits.h>
#include <stdlib.h>

#include <mysql.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/minierr/minierr.h>
#include <mastar/regerr/masregerr.h>

#include <mastar/tools/mas_arg_tools.h>

#include "mysqlpfs_structs.h"
#include "mysqlpfs.h"

#include "mysqlpfs_query.h"

int
mas_mysqlpfs_query( mysqlpfs_t * pfs, const char *sqlop )
{
  rDECLBAD;

  QRGP( pfs );
  QRGP( sqlop );
  if ( pfs && sqlop && *sqlop )
  {
    {
      char *sqlopx = mas_expand_string_cb_arg( sqlop, mas_mysqlpfs_expand_sqlop, pfs->table_prefix );

      if ( 0 == mysql_query( &pfs->mysql, sqlopx ) )
        rSETGOOD;
      mas_free( sqlopx );
    }
    QRGS( rCODE );
  }
  rRET;
}
