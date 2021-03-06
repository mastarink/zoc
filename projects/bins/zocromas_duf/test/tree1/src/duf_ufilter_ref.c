#include <string.h>
#include <assert.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance.h"
#include "duf_config_ref.h"


/* ###################################################################### */
#include "duf_ufilter_ref.h"
/* ###################################################################### */



int
duf_ufilter_max_rel_depth( const duf_ufilter_t * pu )
{
  return pu ? pu->max_rel_depth : 0;
}

int
duf_ufilter_max_filter( const duf_ufilter_t * pu, int seq, const duf_items_t * pitems )
{
  DEBUG_STARTR( r );
  if ( pu->max_seq && seq >= pu->max_seq )
    DUF_MAKE_ERROR( r, DUF_ERROR_MAX_SEQ_REACHED );
  else if ( pu->maxitems.files && pitems->files >= pu->maxitems.files )
    DUF_MAKE_ERROR( r, DUF_ERROR_MAX_REACHED );
  else if ( pu->maxitems.dirs && pitems->dirs >= pu->maxitems.dirs )
    DUF_MAKE_ERROR( r, DUF_ERROR_MAX_REACHED );
  else if ( pu->maxitems.total && pitems->total >= pu->maxitems.total )
    DUF_MAKE_ERROR( r, DUF_ERROR_MAX_REACHED );
  DEBUG_ENDR( r );
}
