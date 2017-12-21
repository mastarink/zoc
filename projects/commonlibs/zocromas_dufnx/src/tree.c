#define R_GOOD(_r) ((_r)>=0)
/* #include <stdio.h> */
#include <string.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_utils_path.h>

#include <mastar/minierr/minierr.h>
/* #include <mastar/regerr/masregerr.h> */
#include <mastar/regerr/masregerr_defs.h>

#include <mastar/mulconfnt/mulconfnt_structs.h>

#include <mastar/mulconfnt/option_interface_base.h>
#include <mastar/mulconfnt/option_interface.h>

#include <mastar/masxfs/masxfs_pathinfo_base.h>
#include <mastar/masxfs/masxfs_pathinfo.h>
#include <mastar/masxfs/masxfs_pathinfo_ref.h>

#include <mastar/levinfo/masxfs_levinfo_tools.h>
#include <mastar/levinfo/masxfs_levinfo_structs.h>
#include <mastar/levinfo/masxfs_levinfo_ref.h>

/* #include <mastar/qstd/qstd_mstmt_base.h> */

#include "structs.h"
#include "mysql.h"

#include "tree.h"

static int numline_treecb = 0;

/* of type: masxfs_scan_fun_simple_t */
static int
treecb( masxfs_levinfo_t * li _uUu_, masxfs_levinfo_flags_t flags _uUu_, void *userdata _uUu_, masxfs_depth_t reldepth _uUu_ )
{
  FILE *fil = ( FILE * ) userdata;
  static masxfs_depth_t top_depth = 0;

/* EXAM( !epath, TRUE, "%d ? %d" ); */
  size_t size = masxfs_levinfo_size_ref( li, flags );
  int fd _uUu_ = masxfs_levinfo_fd_ref( li, flags );
  masxfs_depth_t depth = masxfs_levinfo_depth_ref( li, flags );
  ino_t inode = masxfs_levinfo_inode_ref( li, flags );
  const char *ename = masxfs_levinfo_name_ref( li, flags );
  const char *epath _uUu_ = masxfs_levinfo_path_ref( li, flags );

  if ( !numline_treecb && depth )
    top_depth = depth - 1;
  const char *prefix = masxfs_levinfo_prefix_ref( li, "    ", "└── ", "│   ", "├── ", top_depth + 1, flags );

  numline_treecb++;
#if 1
/* /usr/bin/tree -U --inodes -s -a mastest | nl -ba -nrn -w4 > tree-U--inodes-s-a.tree */
  fprintf( fil, "%4d\t%s[%-10ld %10ld]  %s\n", numline_treecb, prefix ? prefix : "", inode, size, ename ? ename : "" /*, epath ? epath : "" */  );
#else
  fprintf( fil, "%4d. %s %ld fd:%d D:%ld i:%ld '%s'\n", numline_treecb, prefix ? prefix : "", size, fd, ( long ) depth, inode,
           ename ? ename : "" /*, epath ? epath : "" */  );
#endif
  return 0;
}

int
dufnx_tree( const char *real_path, masxfs_depth_t maxdepth _uUu_, FILE * fil, masxfs_levinfo_flags_t inflags, mas_dufnx_mysql_data_t * mysql )
{
  rDECLGOOD;

  masxfs_levinfo_flags_t walkflags = MASXFS_CB_RECURSIVE | MASXFS_CB_STAT | MASXFS_CB_SINGLE_CB | inflags;

  masxfs_entry_callback_t callbacks[] = {
    {treecb,.flags = /* MASXFS_CB_OFF_NAME | MASXFS_CB_PATH | */ MASXFS_CB_PREFIX | MASXFS_CB_TRAILINGSLASH | MASXFS_CB_STAT /* | MASXFS_CB_FD */ },
    {NULL}
  };
  dufnx_qstd( mysql );
  masxfs_pathinfo_t *pi = NULL;

#if 0
  pi = masxfs_pathinfo_create_setup_realpath( real_path, 128 /* depth limit */ , 0 );
  masxfs_pathinfo_open( pi, walkflags );
  WARN( "pidepth:%d", masxfs_pathinfo_pidepth( pi ) );
  for ( masxfs_depth_t i = 0; i < masxfs_pathinfo_pidepth( pi ); i++ )
  {
    masxfs_levinfo_t *li = masxfs_pathinfo_li( pi, i );

    WARN( "node_id:%ld %s", masxfs_levinfo_node_id_val( li ), masxfs_levinfo_detype2s( masxfs_levinfo_detype( li, walkflags ) ) );
  }
  masxfs_pathinfo_delete( pi, MASXFS_CB_MODE_FS | MASXFS_CB_MODE_DB );
  pi - NULL;
#endif

  pi = masxfs_pathinfo_create_setup_realpath( real_path, 128 /* depth limit */ , 0 );

/* masxfs_levinfo_flags_t xflags1 _uUu_ = MASXFS_CB_UP_ROOT; */

/* masxfs_levinfo_flags_t xflags2 _uUu_ = MASXFS_CB_FROM_ROOT | MASXFS_CB_SELF_N_UP; */
  masxfs_levinfo_flags_t xflags2 = MASXFS_CB_FROM_ROOT | MASXFS_CB_SELF;
  masxfs_entry_filter_t entry_filter = {.typeflags = MASXFS_ENTRY_REG | MASXFS_ENTRY_LNK | MASXFS_ENTRY_DIR,.maxdepth = maxdepth };

  numline_treecb = 0;
  WARN( "**<TREE>**" );
  rC( masxfs_pathinfo_scanf_cbs( pi, &entry_filter, callbacks, fil /* userdata */ , walkflags | xflags2,
                                 0 ) );
  masxfs_pathinfo_delete( pi, MASXFS_CB_MODE_FS | MASXFS_CB_MODE_DB );
  rRET;
}
