/* #undef MAS_TRACING */
#include <assert.h>

#include "duf_tracen_defs.h"                                         /* DUF_TRACE ♠ */
#include "duf_errorn_defs.h"                                         /* DUF_NOERROR; DUF_CLEAR_ERROR; DUF_E_(LOWER|UPPER); DUF_TEST_R ... ♠ */

#include "duf_start_end.h"                                           /* DUF_STARTR ; DUF_ENDR ♠ */
#include "duf_dodefs.h"                                              /* DOR ♠ */

#include "duf_config.h"
#include "duf_config_util.h"

/* ###################################################################### */
#include "duf_pdi_ref.h"
/* ###################################################################### */

duf_depthinfo_t *
duf_pdi_root( duf_depthinfo_t * pdi )
{
  return pdi->root_pdi ? pdi->root_pdi : pdi;
}

int
duf_pdi_seq( const duf_depthinfo_t * pdi )
{
  return pdi ? pdi->seq : 0;
}

int
duf_pdi_recursive( const duf_depthinfo_t * pdi )
{
  return pdi ? ( pdi->recursive ? 1 : 0 ) : 0;
}

int
duf_pdi_allow_dirs( const duf_depthinfo_t * pdi )
{
  return pdi ? ( pdi->allow_dirs ? 1 : 0 ) : 0;
}

int
duf_pdi_linear( const duf_depthinfo_t * pdi )
{
  return pdi ? ( pdi->linear ? 1 : 0 ) : 0;
}

void
duf_pdi_reg_changes( duf_depthinfo_t * pdi, int changes )
{
  if ( pdi )
    pdi->changes += changes;
}

int
duf_pdi_opendir( const duf_depthinfo_t * pdi )
{
  return pdi ? ( pdi->opendir ? 1 : 0 ) : 0;
}

int
duf_pdi_set_opendir( duf_depthinfo_t * pdi, int od )
{
  DUF_STARTR( rd );

  assert( pdi );
  rd = pdi->opendir;
  pdi->opendir = od;
  DUF_TRACE( fs, 3, "set opendir:%d", od );

  DUF_ENDR( rd );
}
