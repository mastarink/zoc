#include <string.h>

#include <limits.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_arg_tools.h>

#include "masxfs_defs.h"
#include "masxfs_error.h"

#include "masxfs_structs.h"

#include "masxfs_levinfo_base.h"
#include "masxfs_levinfo_io.h"

#include "masxfs_levinfo_path.h"
#include "masxfs_levinfo.h"

masxfs_levinfo_t *
masxfs_levinfo_root( masxfs_levinfo_t * li )
{
  return li ? li - li->lidepth : NULL;
}

masxfs_levinfo_t *
masxfs_levinfo_li2lia( masxfs_levinfo_t * li )
{
  return masxfs_levinfo_root( li );
}

static masxfs_entry_type_t
de2entry( int d_type )
{
  masxfs_entry_type_t c = 0;

  switch ( d_type )
  {
  case DT_BLK:
    c = MASXFS_ENTRY_BLK;
    break;
  case DT_CHR:
    c = MASXFS_ENTRY_CHR;
    break;
  case DT_DIR:
    c = MASXFS_ENTRY_DIR;
    break;
  case DT_FIFO:
    c = MASXFS_ENTRY_FIFO;
    break;
  case DT_LNK:
    c = MASXFS_ENTRY_LNK;
    break;
  case DT_REG:
    c = MASXFS_ENTRY_REG;
    break;
  case DT_SOCK:
    c = MASXFS_ENTRY_SOCK;
    break;
  case DT_UNKNOWN:
    c = MASXFS_ENTRY_UNKNOWN;
    break;
  }
  return c;
}

static int
masxfs_levinfo_scanli_cb( masxfs_levinfo_t * li, masxfs_entry_callback_t * cb, int recursive )
{
  int r = 0, rc = 0;

  masxfs_levinfo_opendir( li );
  r = masxfs_levinfo_scandir_cb( li, cb, recursive );
  if ( r )
    RDIE( "R:%d", r );
  rc = masxfs_levinfo_closedir( li );
  if ( !r )
    r = rc;
  if ( r )
    RDIE( "R:%d", r );
  return r;
}

int
masxfs_levinfo_scanentry_cb( masxfs_levinfo_t * li, masxfs_entry_callback_t * cb, int recursive )
{
  int r = 0;

  if ( li && li->de && cb )
  {
    const char *name = li->de->d_name;
    masxfs_scan_fun_simple_t fun_simple = cb->fun_simple;
    int d_type = li->de->d_type;

  /* char *fpath = NULL; */

    if ( !( name[0] == '.' && ( ( name[1] == '.' && name[2] == 0 ) || name[1] == 0 ) ) )
    {
    /* TODO - Don't remove next 20170217.123704 */
#if 0
      if ( d_type == DT_UNKNOWN )
      {
        struct stat st;

        fpath = masxfs_normalize_path( path, name );
        d_type = DT_UNKNOWN;
        r = lstat( fpath, &st );
        if ( !r )
        {
          switch ( st.st_mode & S_IFMT )
          {
          case S_IFSOCK:
            d_type = DT_SOCK;
            break;
          case S_IFLNK:
            d_type = DT_LNK;
            break;
          case S_IFREG:
            d_type = DT_REG;
            break;
          case S_IFBLK:
            d_type = DT_BLK;
            break;
          case S_IFDIR:
            d_type = DT_DIR;
            break;
          case S_IFCHR:
            d_type = DT_CHR;
            break;
          case S_IFIFO:
            d_type = DT_FIFO;
            break;
          default:
            d_type = DT_UNKNOWN;
            break;
          }
        }
      }
#endif
      if ( !r )
      {
        masxfs_entry_type_bit_t entry_bit = 1 << de2entry( d_type );

        if ( ( cb->types & entry_bit ) && fun_simple )
        {
          char *path = masxfs_levinfo_li2path_up( li );

          fun_simple( path, name );
          mas_free( path );
        }
        if ( recursive && d_type == DT_DIR )
        {
          li++;
          masxfs_levinfo_init( li, name );
          r = masxfs_levinfo_scanli_cb( li, cb, recursive );
          if ( r )
            RDIE( "R:%d", r );
          masxfs_levinfo_reset( li );
          li--;
        }
      }
    /* if ( fpath )         */
    /*   mas_free( fpath ); */
    }
  }
  else
    r = -1;
  if ( r )
    RDIE( "R:%d (%d:%d:%d)", r, li ? 1 : 0, li && li->de ? 1 : 0, cb ? 1 : 0 );
  return r;
}

int
masxfs_levinfo_scanentry( masxfs_levinfo_t * li, masxfs_entry_callback_t * callbacks, int recursive )
{
  int r = 0;

  if ( li )
  {
    for ( masxfs_entry_callback_t * cb = callbacks; !r && cb && cb->fun_simple; cb++ )
    {
      r = masxfs_levinfo_scanentry_cb( li, cb, recursive );
      if ( r )
        RDIE( "R:%d", r );
    }
  }
  else
    r = -1;
  if ( r )
    RDIE( "R:%d", r );
  return r;
}

int
masxfs_levinfo_scandir_cb( masxfs_levinfo_t * li, masxfs_entry_callback_t * cb, int recursive )
{
  int r = 0;

  if ( !masxfs_levinfo_rewinddir( li ) )
  {
    while ( !r && masxfs_levinfo_readdir( li ) )
    {
      r = masxfs_levinfo_scanentry_cb( li, cb, recursive );
      if ( r )
        RDIE( "R:%d", r );
    }
  }
  else
    r = -1;
  if ( r )
    RDIE( "R:%d", r );
  return r;
}

int
masxfs_levinfo_scandir( masxfs_levinfo_t * li, masxfs_entry_callback_t * callbacks, int recursive )
{
  int r = 0;

  if ( !masxfs_levinfo_rewinddir( li ) )
  {
    while ( !r && masxfs_levinfo_readdir( li ) )
    {
      r = masxfs_levinfo_scanentry( li, callbacks, recursive );
      if ( r )
        RDIE( "R:%d", r );
    }
  }
  else
    r = -1;
  if ( r )
    RDIE( "R:%d", r );
  return r;
}
