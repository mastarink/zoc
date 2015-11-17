#include <string.h>

#include <mastar/tools/mas_utils_path.h> /* mas_pathdepth */

#include "duf_maintenance.h"

#include "duf_levinfo_ref_def.h"
#include "duf_levinfo_context.h"
#include "duf_levinfo_credel.h"

#include "duf_li_credel.h"
#include "duf_li.h"


#include "duf_dh.h"

#include "duf_pathinfo_ref.h"
#include "duf_pathinfo.h"

#include "duf_pathinfo_ref_def.h"
/* ###################################################################### */
#include "duf_pathinfo_credel.h"
/* ###################################################################### */

void
duf_pi_levinfo_delete( duf_pathinfo_t * pi )
{
  if ( pi->levinfo )
  {
    duf_li_clear_n( pi->levinfo, pi->maxdepth );
    mas_free( pi->levinfo );
    pi->levinfo = NULL;
  }
}

void
duf_pi_init_level_d( duf_pathinfo_t * pi, const char *itemname, unsigned long long dirid, duf_node_type_t node_type, int d )
{
  duf_levinfo_t *li;

  li = duf_pi_ptr_d( pi, d );
  duf_li_init( li, itemname, dirid, node_type );
}

int
duf_pi_levinfo_create( duf_pathinfo_t * pi, size_t maxdepth )
{
  DEBUG_STARTR( r );
  duf_levinfo_t *pli = NULL;

  pli = duf_li_create( maxdepth );
  DOR( r, duf_pi_levinfo_set( pi, pli, maxdepth ) );

  DEBUG_ENDR( r );
}

void
duf_pi_copy( duf_pathinfo_t * pidst, const duf_pathinfo_t * pisrc, int no_li )
{
  assert( pidst );
  assert( pisrc );
  memcpy( pidst, pisrc, sizeof( duf_pathinfo_t ) );
  pidst->levinfo = ( no_li ) ? NULL : duf_li_clone( pisrc->levinfo, pisrc->maxdepth );
}

int
duf_pi_shut( duf_pathinfo_t * pi )
{
  DEBUG_STARTR( r );

  pi->depth = pi->topdepth = 0;
  pi->maxdepth = 0;
  assert( !pi->maxdepth );
  assert( !pi->topdepth );
  assert( !pi->depth );
  assert( !pi->levinfo );

  DEBUG_ENDR( r );
}


