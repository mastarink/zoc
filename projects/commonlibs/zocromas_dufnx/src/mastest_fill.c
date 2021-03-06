#define R_GOOD(_r) ((_r)>=0)
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <mastar/minierr/minierr.h>
#include <mastar/regerr/masregerr_defs.h>
#include <mastar/exam/masexam.h>

#include <mastar/levinfo/masxfs_levinfo_structs.h>
#include <mastar/levinfo/masxfs_levinfo_tools.h>
#include <mastar/levinfo/masxfs_levinfo_ref_depth.h>
#include <mastar/levinfo/masxfs_levinfo_ref_inode.h>
#include <mastar/levinfo/masxfs_levinfo_ref_size.h>
#include <mastar/levinfo/masxfs_levinfo_ref_name.h>
#include <mastar/levinfo/masxfs_levinfo_ref_fd.h>
#include <mastar/levinfo/masxfs_levinfo_ref.h>
#include <mastar/levinfo/masxfs_levinfo_db.h>

#include <mastar/masxfs/masxfs_pathinfo_base.h>
#include <mastar/masxfs/masxfs_pathinfo.h>

#include <mastar/qstd/qstd_query.h>
#include <mastar/qstd/qstd_mstmt_base.h>
#include <mastar/qstd/qstd_mstmt.h>
#include <mastar/qstd/qstd_mstmt_parents.h>
#include <mastar/qstd/qstd_mstmt_names.h>
#include <mastar/qstd/qstd_mstmt_sizes.h>
#include <mastar/qstd/qstd_mstmt_datas.h>
#include <mastar/qstd/qstd_mstmt_props.h>

static int
testfillcb( masxfs_levinfo_t * li, masxfs_levinfo_flags_t flags, void *qstdv _uUu_, unsigned long serial _uUu_, masxfs_depth_t reldepth _uUu_ )
{

#if 1
  masxfs_levinfo_db_store( li, flags );
#else
  {
    mas_qstd_t *qstd = ( mas_qstd_t * ) qstdv;
    unsigned long long parent_id = masxfs_levinfo_parent_id( li, flags );
    masxfs_entry_type_t detype = masxfs_levinfo_detype( li, flags );
    unsigned long long node_id = 0;
    unsigned long long dataid = 0;
    const char *sdetype = masxfs_levinfo_detype2s( detype );
    const char *ename = masxfs_levinfo_name_ref( li, flags );

  /* unsigned long long propid _uUu_ = 0; */

  /* if ( detype == MASXFS_ENTRY_REG_NUM ) */
    {
      size_t size = masxfs_levinfo_size_ref( li, flags );
      size_t thesize _uUu_ = mas_qstd_mstmt_insget_sizes_id( qstd, size );
    }
    {
      const masxfs_stat_t *stat = masxfs_levinfo_stat_ref( li, flags );

      if ( stat )
      {
        dataid = mas_qstd_mstmt_selinsget_datas_id( qstd, stat );
      /* if ( detype == MASXFS_ENTRY_REG_NUM ) */
        mas_qstd_mstmt_selinsget_props_id( qstd, dataid, sdetype, stat );
      }
    }
  /* WARN( "D%d; %lld: '%s'", li->lidepth, parent_id, ename ); */
    {
      unsigned long long theid = 0;
      masxfs_depth_t lidepth = masxfs_levinfo_depth_ref( li, flags );

      if ( lidepth != 0 )
      {
# if 0
        theid = mas_qstd_mstmt_insget_names_id( qstd, ename, parent_id, sdetypes[detype] );
# endif
# if 1
        assert( parent_id );
        theid = mas_qstd_mstmt_selinsget_names_id( qstd, ename, parent_id, dataid, sdetype );
# endif
# if 0
        theid = mas_qstd_mstmt_insselget_names_id( qstd, ename, parent_id, dataid, sdetypes[detype] );
# endif
      }
      if ( detype == MASXFS_ENTRY_DIR_NUM )
      {
        node_id = mas_qstd_mstmt_selinsget_parents_id( qstd, theid );
      /* WARN( "(%lld) set node_id=%lld (was:%lld) for '%s'", parent_id, node_id, li->db.node_id, li->name ); */
        masxfs_levinfo_set_node_id( li, node_id );
      }
    /* masxfs_depth_t depth _uUu_ = masxfs_levinfo_depth_ref( li, flags );                                                    */
    /*                                                                                                                        */
    /* if ( !theid || 0 == strcmp( ename, "home" ) || node_id == 66 || node_id == 1 )                               */
    /*   MARK( "(T6)", " %ld. '%s' ID: %llu => %llu; node_id:%llu", ( long ) depth, ename, ( unsigned long long ) theid, */
    /*         ( unsigned long long ) parent_id, ( unsigned long long ) node_id );                                       */
    }
  }
#endif
  return 0;
}

int
testfill( const char *path )
{
  rDECLBAD;

  masxfs_entry_callback_t callbacks[] = {
    {
     .fun_simple = testfillcb,.flags =
     /* MASXFS_CB_OFF_NAME | MASXFS_CB_PATH | */ MASXFS_CB_PREFIX | MASXFS_CB_TRAILINGSLASH | MASXFS_CB_STAT /* | MASXFS_CB_FD */ }

    , {0}
  };
  WARN( "******** testfill *******" );
  {
    mas_qstd_t *qstd = mas_qstd_instance_setup( "mysql.mastar.lan", "masdufnt", "i2xV9KrTA54HRpj4e", "masdufntdb", 3306, "qstd_" );

    rC( mas_qstd_drop_tables( qstd ) );
    rC( mas_qstd_create_tables( qstd ) );
    if ( rGOOD )
    {
    /* const char *path0 = "mastest"; */
    /* const char *path0 = "/home/mastar/.mas/lib/big/misc/develop/autotools/zoc/projects/commonlibs/zocromas_xfs/mastest"; */
      masxfs_pathinfo_t *pi = masxfs_pathinfo_create_setup( path, 128 /* depth limit */ , 0 );

      {
        masxfs_levinfo_flags_t flagsfs _uUu_ = MASXFS_CB_RECURSIVE | MASXFS_CB_MODE_FS | MASXFS_CB_SINGLE_CB;
        masxfs_type_flags_t typeflags = MASXFS_ENTRY_REG | MASXFS_ENTRY_LNK | MASXFS_ENTRY_DIR;

        {
          rC( mas_qstd_start_transaction( qstd ) );
        /* TODO FIXME : limiting maxdepth here (filling db) leads to memleak when scanning db 20170320.140237 */
          WARN( "******** testfill scan *******" );
          masxfs_levinfo_flags_t xflags1 _uUu_ = MASXFS_CB_UP_ROOT;
          masxfs_levinfo_flags_t xflags2 _uUu_ = MASXFS_CB_FROM_ROOT | MASXFS_CB_SELF_N_UP;

          rC( masxfs_pathinfo_scan_cbs( pi, typeflags, &callbacks[0], qstd, flagsfs | xflags1 ) );
        /* rC( masxfs_pathinfo_scanf_cbs( pi, &entry_filter, &callback, qstd, flagsfs | xflags1 ) ); */
          WARN( "******** /testfill scan *******" );
          rC( mas_qstd_end_transaction( qstd ) );
        }

        rC( mas_qstd_update_summary( qstd ) );
      }
      masxfs_pathinfo_delete( pi, MASXFS_CB_MODE_ALL );
    }
    mas_qstd_instance_delete(  );
  }
  WARN( "******** /testfill *******" );

  rRET;
}
