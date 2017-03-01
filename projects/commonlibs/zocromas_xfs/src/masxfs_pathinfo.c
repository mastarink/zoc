#include "masxfs_defs.h"
#include <stdio.h>
#include <string.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/minierr/minierr.h>

#include "masxfs_structs.h"

#include "masxfs_levinfo_tools.h"
#include "masxfs_levinfo_path.h"
#include "masxfs_levinfo.h"

/* #include "masxfs_pathinfo_io.h" */

#include "masxfs_pathinfo.h"

char *
masxfs_pathinfo_pi2path( masxfs_pathinfo_t * pi )
{
  return masxfs_levinfo_lia2path( pi->levinfo, pi->pidepth, 0 );
}

int
masxfs_pathinfo_scan( masxfs_pathinfo_t * pi, masxfs_entry_callback_t * callbacks, unsigned long flags, masxfs_depth_t maxdepth )
{
  int r = 0, rc = 0;

/* r = masxfs_pathinfo_opendir( pi ); */
/* QRPI( pi, r );                     */

/* if ( r >= 0 ) */
  {
    masxfs_levinfo_t *li = masxfs_pathinfo_last_li( pi );

    if ( !( flags & MASXFS_CB_RECURSIVE ) )
      DIE( "%s", "NOREC" );
    r = masxfs_levinfo_scandirn_cb( li, callbacks, flags, pi->pidepth + maxdepth );
    QRPI( pi, r );
  /* rc = masxfs_pathinfo_closedir_all( pi ); */
    if ( r >= 0 )
      r = rc;
    QRPI( pi, r );
  }
  return r;
}

masxfs_levinfo_t *
masxfs_pathinfo_tail( masxfs_pathinfo_t * pi, masxfs_depth_t offset )
{
  return pi->levinfo + pi->pidepth - 1 - offset;
}

masxfs_levinfo_t *
masxfs_pathinfo_last_li( masxfs_pathinfo_t * pi )
{
  return masxfs_pathinfo_tail( pi, 0 );
}