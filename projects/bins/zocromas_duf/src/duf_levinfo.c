#include <string.h>

#include "duf_maintenance.h"

#include "duf_li_credel.h"

#include "duf_pathinfo.h"
#include "duf_pathinfo_depth.h"
/* #include "duf_pathinfo_credel.h" */


/* ###################################################################### */
#include "duf_levinfo.h"
/* ###################################################################### */

void
duf_levinfo_clear_level_d( duf_depthinfo_t * pdi, int d )
{
  assert( pdi );
  assert( pdi->pathinfo.levinfo );
  if ( d >= 0 )
#if 0
    duf_li_clear( &pdi->pathinfo.levinfo[d] );
#else
    duf_pi_clear_d( &pdi->pathinfo, d );
#endif
}

int
duf_levinfo_calc_depth( duf_depthinfo_t * pdi )
{
  int d;

  d = duf_pi_calc_depth( &pdi->pathinfo );
  return d;
}
