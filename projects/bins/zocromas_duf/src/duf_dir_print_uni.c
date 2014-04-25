#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* #include <unistd.h> */
#include <sys/stat.h>
#include <time.h>

#include <assert.h>
#include <openssl/md5.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_types.h"

#include "duf_utils.h"
#include "duf_service.h"
#include "duf_config.h"

/* #include "duf_sql_def.h" */
/* #include "duf_sql.h" */
#include "duf_sql_field.h"

#include "duf_path.h"

#include "duf_sql.h"

#include "duf_dbg.h"

/* ###################################################################### */
/* #include "duf_dir_print_uni.h" */
/* ###################################################################### */




/* callback of  duf_scan_callback_file_t */
static int
scan_leaf( duf_depthinfo_t * pdi, duf_record_t * precord /*, const duf_dirhandle_t * pdh_notused */  )
{
  int r = 0;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );

  /* SQL at duf_scan_files_by_dirid */
  DUF_SFIELD( filename );
  /* const char *filename = duf_sql_str_by_name( "filename", precord, 0 ); */
  DUF_UFIELD( filesize );
  /* unsigned long long filesize = duf_sql_ull_by_name( "filesize", precord, 0 ); */
  DUF_UFIELD( filemode );
  /* unsigned long long filemode = duf_sql_ull_by_name( "filemode", precord, 0 ); */
  DUF_UFIELD( md5sum1 );
  /* unsigned long long md5a = duf_sql_ull_by_name( "md5sum1", precord, 0 ); */
  DUF_UFIELD( md5sum2 );
  /* unsigned long long md5b = duf_sql_ull_by_name( "md5sum2", precord, 0 ); */
  DUF_UFIELD( mtime );
  DUF_UFIELD( uid );
  DUF_UFIELD( gid );
  DUF_UFIELD( nlink );
  DUF_UFIELD( inode );
  DUF_UFIELD( nsame );
  /* DUF_SFIELD( mtimef ); */
  /* DUF_SFIELD( dowmtime ); */
  /* DUF_SFIELD( monthmtime ); */

  /* if (nsame<2) return r; */

  /* printf( "> %s\n", duf_sql_str_by_name( "filename", precord ) ); */
  /* printf( "-rw-------  1 mastar mastar-firefox 106580068 Jan 27 2014 12:35:27 sample_video_hd.zip\n" ); */
  /* duf_sql_print_tree_prefix_uni( pdi, 1 ); */
  {
    duf_fileinfo_t fi;

    duf_format_t format = {
      .filename = 1,
      .seq = 1,
      .dirid = 1,
      .inode = 1,
      .mode = 1,
      .nlink = 1,
      .uid = 1,
      .gid = 1,
      .filesize = 1,
      .md5 = 1,
      .mtime = 1,
      .nsame = 1,
    };
    fi.nsame = nsame;
    fi.st.st_mode = ( mode_t ) filemode;
    fi.st.st_ino = ( ino_t ) inode;
    fi.st.st_mtim.tv_sec = mtime;
    fi.st.st_mtim.tv_nsec = 0;
    fi.st.st_uid = ( uid_t ) uid;
    fi.st.st_gid = ( gid_t ) gid;
    fi.st.st_nlink = ( nlink_t ) nlink;
    fi.st.st_size = ( off_t ) filesize;
    fi.name = filename;
    fi.md5sum1 = md5sum1;
    fi.md5sum2 = md5sum2;
    duf_print_file_info( stdout, pdi, &fi, &format );
  }
  printf( "\n" );

  duf_dbgfunc( DBG_END, __func__, __LINE__ );
  return r;
}

static int
scan_node_before( unsigned long long pathid, /* const duf_dirhandle_t * pdh_notused, */ duf_depthinfo_t * pdi,
                                    duf_record_t * precord )
{
  int r = 0;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );

  {
    char *path = duf_pathid_to_path_s( pathid, pdi, &r );

    DUF_UFIELD( nfiles );
    DUF_UFIELD( minsize );
    DUF_UFIELD( maxsize );

    if ( nfiles )
    {
      /* optimizing makes puts, segfault by NULL, therefore printf( "%s\n", path  ); is not good */
      if ( duf_config->cli.format.seq )
        printf( "d%7llu ", pdi->seq );
      if ( duf_config->cli.format.dirid )
        printf( "[%8llu] ", pdi->levinfo[pdi->depth].dirid );
      printf( " (%5llu) %llu-%llu %s\n", nfiles, minsize, maxsize, path ? path : "-=No path=-" );
    }
    mas_free( path );
  }

  duf_dbgfunc( DBG_END, __func__, __LINE__ );

  return r;
}

duf_scan_callbacks_t duf_print_dir_callbacks = {
  .title = __FILE__ ".dir",
  .init_scan = NULL,
  .node_scan_before = scan_node_before,
  .leaf_scan = scan_leaf,
  .fieldset =
        "duf_filenames.pathid as dirid "
        " , duf_filenames.name as filename, duf_filedatas.size as filesize " ", duf_filedatas.size as filesize "
        " , uid, gid, nlink, inode, mtim as mtime " " , dupcnt as nsame"
        " , duf_filedatas.mode as filemode " " , duf_filenames.id as filenameid" " , md.md5sum1, md.md5sum2 ",
  /* " , datetime(mtim, 'unixepoch') as mtimef " */
  /* ", strftime('%Y-%m-%d %H:%M:%S',mtim,'unixepoch') as mtimef " */
  /* ", case cast (strftime('%w', mtim,'unixepoch') as integer) "                                                                   */
  /* " when 0 then 'Sun' when 1 then 'Mon' when 2 then 'Tue' when 3 then 'Wed' "                                                    */
  /* " when 4 then 'Thu' when 5 then 'Fri' else 'Sat' end as dowmtime, " "case cast (strftime('%m', mtim,'unixepoch') as integer) " */
  /* " when 1 then 'Jan' when 2 then 'Feb' when 3 then 'Mar' when 4 then 'Apr' when 5 then 'May' when 6 then "                      */
  /* " 'Jun' when 7 then 'Jul' when 8 then 'Aug' when 9 then 'Sep' when 10 then 'Oct' when 11 then 'Nov' when 12 then 'Dec' "       */
  /* " else 'Wow' end as monthmtime "                                                                                               */
  .leaf_selector =
        "SELECT %s FROM duf_filenames "
        " JOIN duf_filedatas on (duf_filenames.dataid=duf_filedatas.id) "
        " LEFT JOIN duf_md5 as md on (md.id=duf_filedatas.md5id)" "    WHERE "
        /* "           duf_filedatas.size >= %llu AND duf_filedatas.size < %llu "            */
        /* "       AND (md.dupcnt IS NULL OR (md.dupcnt >= %llu AND md.dupcnt < %llu)) AND " */
        " duf_filenames.pathid='%llu' ",
  .node_selector =
        "SELECT duf_paths.id as dirid, duf_paths.dirname, duf_paths.dirname as dfname,  duf_paths.parentid "
        ", tf.numfiles AS nfiles, td.numdirs AS ndirs, tf.maxsize AS maxsize, tf.minsize AS minsize "
        /* " ,(SELECT count(*) FROM duf_paths as subpaths WHERE subpaths.parentid=duf_paths.id) as ndirs "       */
        /* " ,(SELECT count(*) FROM duf_filenames as sfn "                                                       */
        /* "          JOIN duf_filedatas as sfd ON (sfn.dataid=sfd.id) "                                         */
        /* "          JOIN duf_md5 as smd ON (sfd.md5id=smd.id) "                                                */
        /* "          WHERE sfn.pathid=duf_paths.id "                                                            */
        /* "              AND   sfd.size >= %llu AND sfd.size < %llu "                                           */
        /* "              AND (smd.dupcnt IS NULL OR (smd.dupcnt >= %llu AND smd.dupcnt < %llu)) "               */
        /* " ) as nfiles "                                                                                       */
        /* " ,(SELECT min(sfd.size) FROM duf_filedatas as sfd JOIN duf_filenames as sfn ON (sfn.dataid=sfd.id) " */
        /* "           WHERE sfn.pathid=duf_paths.id) as minsize "                                               */
        /* " ,(SELECT max(sfd.size) FROM duf_filedatas as sfd JOIN duf_filenames as sfn ON (sfn.dataid=sfd.id) " */
        /* "           WHERE sfn.pathid=duf_paths.id) as maxsize "                                               */
        " FROM duf_paths "
        " LEFT JOIN duf_pathtot_dirs AS td ON (td.pathid=duf_paths.id) "
        " LEFT JOIN duf_pathtot_files AS tf ON (tf.pathid=duf_paths.id)                                    "
        " WHERE duf_paths.parentid='%llu' ",
  /* .final_sql_argv = final_sql, */
};