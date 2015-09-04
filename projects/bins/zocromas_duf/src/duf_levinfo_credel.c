#include <string.h>
#include <assert.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_maintenance.h"

#include "duf_levinfo_updown.h"

/* ###################################################################### */
#include "duf_levinfo_credel.h"
/* ###################################################################### */




/* create level-control array, open 0 level */
int
duf_levinfo_create( duf_depthinfo_t * pdi, int pathdepth, int recursive, int opendir )
{
  DEBUG_STARTR( r );

  assert( pdi );

  if ( !pdi->pathinfo.levinfo )
  {
    size_t lsz;
    int max_rel_depth = 0;

    max_rel_depth = pdi && pdi->pu ? pdi->pu->max_rel_depth : 20;
    assert( pdi->pathinfo.depth == -1 );
    /* DUF_TRACE( temp, 0, "@@@@@@@ %u", max_rel_depth ); */
    if ( max_rel_depth /* FIXME */  )
    {
      pdi->maxdepth = max_rel_depth + ( pathdepth ? pathdepth : 20 );
      pdi->recursive = recursive ? 1 : 0;
      pdi->opendir = opendir ? 1 : 0;
      lsz = sizeof( pdi->pathinfo.levinfo[0] ) * ( pdi->maxdepth + 3 );
      pdi->pathinfo.levinfo = mas_malloc( lsz );
      assert( pdi->pathinfo.levinfo );
      memset( pdi->pathinfo.levinfo, 0, lsz );
      assert( pdi->pathinfo.depth == -1 );
    }
    else
    {
      T( "max_rel_depth:%d", max_rel_depth );
      DUF_MAKE_ERROR( r, DUF_ERROR_LEVINFO_SIZE );
    }
    /* DOR( r, duf_levinfo_open_dh( pdi, path ) ); */
  }
  else
  {
  }
  DEBUG_ENDR( r );
}

/* delete level-control array, close 0 level */
int
duf_levinfo_delete( duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );

  assert( pdi );
  assert( pdi->pathinfo.levinfo );

  if ( pdi->pathinfo.levinfo )
  {
    while ( DUF_NOERROR( r ) && pdi->pathinfo.depth >= 0 )
    {
      duf_levinfo_goup( pdi );
    }
    assert( pdi->pathinfo.depth == -1 );

    mas_free( pdi->pathinfo.levinfo );
    pdi->pathinfo.levinfo = NULL;
  }
  DEBUG_ENDR( r );
}
