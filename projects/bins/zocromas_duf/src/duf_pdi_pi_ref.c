/* #undef MAS_TRACING */
#include <assert.h>

#include "duf_tracen_defs_preset.h"                                  /* MAST_TRACE_CONFIG; etc. ✗ */

#include <mastar/trace/mas_trace.h>

#include "duf_pathinfo_ref.h"

#include "duf_pdi_structs.h"
/* ###################################################################### */
#include "duf_pdi_pi_ref.h"
/* ###################################################################### */
/* 20151114.124354 */
int
duf_pdi_depth( const duf_depthinfo_t * pdi )
{
#if 0
  return pdi ? pdi->pathinfo.depth : 0;
#else
  return pdi ? duf_pi_depth( &pdi->pathinfo ) : 0;
#endif
}

/* 20151114.124356 */
int
duf_pdi_reldepth_d( const duf_depthinfo_t * pdi, int d )
{
  return pdi ? duf_pi_deltadepth_d( &pdi->pathinfo, d ) : 0;
}

/* 20151114.124357 */
/* pdi->pathinfo.depth - pdi->pathinfo.topdepth */
int
duf_pdi_reldepth( const duf_depthinfo_t * pdi )
{
  return pdi ? duf_pi_deltadepth( &pdi->pathinfo ) : 0;
}

/* 20151114.124358 */
int
duf_pdi_topdepth( const duf_depthinfo_t * pdi )
{
#if 0
  return pdi ? pdi->pathinfo.topdepth : 0;
#else
  return pdi ? duf_pi_topdepth( &pdi->pathinfo ) : 0;
#endif
}

/* 20151114.124400 */
void
duf_pdi_set_topdepth( duf_depthinfo_t * pdi )
{
#if 0
  if ( pdi )
    pdi->pathinfo.topdepth = pdi->pathinfo.depth;
#else
  duf_pi_set_topdepth( &pdi->pathinfo );
#endif
}

/* 20151114.124402 */
int
duf_pdi_maxdepth( const duf_depthinfo_t * pdi )
{
#if 0
  return pdi ? pdi->pathinfo.maxdepth : 0;
#else
  return pdi ? duf_pi_maxdepth( &pdi->pathinfo ) : 0;
#endif
}

/* 20151114.124404 */
duf_levinfo_t *
duf_pdi_levinfo( const duf_depthinfo_t * pdi )
{
  return pdi ? duf_pi_levinfo( &pdi->pathinfo ) : NULL;
}

const duf_pathinfo_t *
duf_pdi_pathinfo( const duf_depthinfo_t * pdi )
{
  return pdi ? &pdi->pathinfo : NULL;
}

duf_pathinfo_t *
duf_pdi_pathinfo_p( duf_depthinfo_t * pdi )
{
  return pdi ? &pdi->pathinfo : NULL;
}
