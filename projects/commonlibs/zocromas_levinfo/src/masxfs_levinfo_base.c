#define R_GOOD(_r) ((_r)>=0)
#include "masxfs_levinfo_defs.h"
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/minierr/minierr.h>

#include "masxfs_levinfo_structs.h"

#include "masxfs_levinfo_io.h"
#include "masxfs_levinfo_io_dir.h"

#include "masxfs_levinfo_tools.h"
#include "masxfs_levinfo_digest.h"

#include "masxfs_levinfo_base.h"

masxfs_levinfo_t *
masxfs_levinfo_create_array( masxfs_depth_t sz )
{
  masxfs_levinfo_t *li = mas_calloc( sz, sizeof( masxfs_levinfo_t ) ); /* calloc: The memory is set to zero */

  return li;
}

masxfs_levinfo_t *
masxfs_levinfo_create_array_setup( masxfs_depth_t sz )
{
  masxfs_levinfo_t *lia = masxfs_levinfo_create_array( sz );

  return lia;
}

masxfs_levinfo_t *
masxfs_levinfo_create( void )
{
  return masxfs_levinfo_create_array( 0 );
}

void
masxfs_levinfo_n_init( masxfs_levinfo_t * li, masxfs_depth_t lidepth, const char *name, size_t len, masxfs_entry_type_t detype,
                       unsigned long long node_id, masxfs_stat_t * stat, masxfs_xstatc_t * xstatc )
{
  if ( li && name )
  {
    char *newname = mas_strndup( name, len );

    if ( li->name )
      mas_free( li->name );
    li->name = newname;
    li->detype = detype;
  /* li->deinode = d_inode; (* TODO take from destat OR needless!!! *) */
    li->lidepth = lidepth;
    li->db.node_id = node_id;
    if ( li->db.stat )
      mas_free( li->db.stat );
    li->db.stat = NULL;
    if ( stat )
    {
      li->db.stat = mas_calloc( 1, sizeof( masxfs_stat_t ) );
      *( li->db.stat ) = *stat;                                      /* memcpy */
    }
    if ( li->db.xstat )
    {
      if ( li->db.xstat->hex_digest )
        mas_free( li->db.xstat->hex_digest );
      mas_free( li->db.xstat );
      li->db.xstat = NULL;
    }
  /* WARN( "xstatc:%p", xstatc ); */
    if ( xstatc )
    {
      li->db.xstat = mas_malloc( sizeof( masxfs_xstat_t ) );
      li->db.xstat->dg = xstatc->dg;
      li->db.xstat->id = xstatc->id;
    /* li->db.xstat->nsamesize = xstatc->x.nsamesize;     */
    /* li->db.xstat->nsamedigest = xstatc->x.nsamedigest; */
    /* li->db.xstat->nameid = xstatc->x.nameid;           */
      li->db.xstat->hex_digest = mas_strdup( xstatc->chex_digest );
    }
  }
  else
    QRLI( li, -1 );
}

void
masxfs_levinfo_init( masxfs_levinfo_t * li, masxfs_depth_t lidepth, const char *name, masxfs_entry_type_t detype /*, ino_t d_inode */ ,
                     masxfs_stat_t * stat, masxfs_xstatc_t * xstat, unsigned long long node_id )
{
  masxfs_levinfo_n_init( li, lidepth, name, name ? strlen( name ) : 0, detype /*, d_inode */ , node_id, stat, xstat );
}

/* flags: only MASXFS_CB_MODE_FS or/and MASXFS_CB_MODE_DB used */
void
masxfs_levinfo_reset( masxfs_levinfo_t * li, masxfs_levinfo_flags_t flags )
{
  if ( li )
  {
    masxfs_levinfo_closedir( li, flags /* | MASXFS_CB_MODE_FS | MASXFS_CB_MODE_DB */  ); /* flags: only MASXFS_CB_MODE_FS or/and MASXFS_CB_MODE_DB used */
    masxfs_levinfo_close( li, flags /* | MASXFS_CB_MODE_FS | MASXFS_CB_MODE_DB */  ); /* flags: only MASXFS_CB_MODE_FS or/and MASXFS_CB_MODE_DB used */
    li->fd = 0;
    if ( li->name )
      mas_free( li->name );
    li->name = NULL;
    {
      if ( flags & MASXFS_CB_MODE_FS )
      {
        if ( li->fs.stat )
          mas_free( li->fs.stat );
        li->fs.stat = NULL;
      }
      if ( flags & MASXFS_CB_MODE_DB )
      {
        if ( li->db.stat )
          mas_free( li->db.stat );
        li->db.stat = NULL;
        if ( li->db.xstat )
        {
          if ( li->db.xstat->hex_digest )
            mas_free( li->db.xstat->hex_digest );
          mas_free( li->db.xstat );
          li->db.xstat = NULL;
        }
      }
    }
    if ( li->path )
      mas_free( li->path );
    li->path = NULL;
    if ( li->prefix )
      mas_free( li->prefix );
    li->prefix = NULL;

    masxfs_digests_delete( li->digests );

    li->digests = NULL;
    memset( li, 0, sizeof( masxfs_levinfo_t ) );
  }
}

void
masxfs_levinfo_reset_lia( masxfs_levinfo_t * lia, masxfs_depth_t sz, masxfs_levinfo_flags_t flags )
{
  if ( lia )
    for ( masxfs_depth_t il = 0; il < sz && ( lia + il )->name; il++ )
    {
      masxfs_levinfo_reset( lia + il, flags );                       /* flags: only MASXFS_CB_MODE_FS or/and MASXFS_CB_MODE_DB used */
    }
}

void
masxfs_levinfo_delete_lia( masxfs_levinfo_t * li, masxfs_depth_t sz, masxfs_levinfo_flags_t flags )
{
  if ( li )
  {
    masxfs_levinfo_reset_lia( li, sz, flags );
    mas_free( li );
  }
}

void
masxfs_levinfo_delete_lia_tail( masxfs_levinfo_t * li, masxfs_levinfo_flags_t flags )
{
  if ( li )
  {
    masxfs_levinfo_t *li0 = li - li->lidepth; /*depth + 1 = size */

    masxfs_levinfo_delete_lia( li0, li->lidepth + 1, flags );
  }
}

int
masxfs_levinfo_init_valid( masxfs_levinfo_t * li, masxfs_depth_t lidepth, masxfs_entry_filter_t * entry_pfilter, const char *dename,
                           masxfs_entry_type_t detype, unsigned long long node_id, masxfs_stat_t * stat, masxfs_xstatc_t * xstatc )
{
  int validx = 0;

  if ( li )
  {
    assert( !li->db.stat );
    masxfs_levinfo_init( li, lidepth, dename, detype, stat, xstatc, node_id );
    assert( li->db.stat );
    {
      validx = masxfs_levinfo_stat_valid( li, entry_pfilter, MASXFS_CB_MODE_DB )
              && masxfs_levinfo_xstat_valid( li, entry_pfilter, MASXFS_CB_MODE_DB );
      if ( !validx )
      {
        masxfs_levinfo_reset( li, MASXFS_CB_MODE_DB );               /* flags: only MASXFS_CB_MODE_FS or/and MASXFS_CB_MODE_DB used */
      }
    }
  }
  return validx;
}
