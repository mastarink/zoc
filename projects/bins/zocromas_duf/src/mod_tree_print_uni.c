#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_maintenance.h"
#include "duf_hook_types.h"
#include "duf_fileinfo_types.h"

#include "duf_service.h"
#include "duf_config_ref.h"

#include "duf_pdi.h"
#include "duf_levinfo.h"
#include "duf_levinfo_ref.h"

#include "duf_sql_defs.h"
#include "duf_sql_field.h"


/* ###################################################################### */
/* #include "duf_tree_print_uni.h" */
/* ###################################################################### */


/* ━ │ ┃ ┄ ┅ ┆ ┇ ┈ ┉ ┊ ┋ ┌ ┍ ┎ ┏ ┐ ┑ ┒ ┓ └ ┕ ┖ ┗ ┘ ┙                                 */
/* ┚ ┛ ├ ┝ ┞ ┟ ┠ ┡ ┢ ┣ ┤ ┥ ┦ ┧ ┨ ┩ ┪ ┫ ┬ ┭ ┮ ┯ ┰ ┱ ┲                                 */
/* ┳ ┴ ┵ ┶ ┷ ┸ ┹ ┺ ┻ ┼ ┽ ┾ ┿ ╀ ╁ ╂ ╃ ╄ ╅ ╆ ╇ ╈ ╉ ╊ ╋                                 */
/* ╌ ╍ ╎ ╏ ═ ║ ╒ ╓ ╔ ╕ ╖ ╗ ╘ ╙ ╚ ╛ ╜ ╝ ╞ ╟ ╠ ╡ ╢ ╣ ╤ ╥ ╦ ╧ ╨ ╩ ╪ ╫ ╬ */
/* ╭ ╮ ╯ ╰ ╱ ╲ ╳ ╴ ╵ ╶ ╷ ╸ ╹ ╺ ╻ ╼ ╽ ╾ ╿                                                         */
/* ₴ ☑ ♒ */
/* ⬅⬇⬆➡🔜  🆘  🐰 */
/* ╔═╦══╤╗ ┏━┳━━┯┓ ┌─┰──┬┐ ╲    ╲   ╱    ╱   ╭─────╮
 * ║ ║  │║ ┃ ┃  │┃ │ ┃  ││  ╲    ╲ ╱    ╱    │     │       ╷
 * ╠═╬══╪╣ ┣━╋━━┿┫ ┝━╋━━┿┥   ╲    ╳    ╱     │     │      ╶┼╴
 * ╟─╫──┼╢ ┠─╂──┼┨ ├─╂──┼┤    ╲  ╱ ╲  ╱      ╰─────╯       ╵
 * ╚═╩══╧╝ ┗━┻━━┷┛ └─┸──┴┘     ╲╱   ╲╱              
 * */
static int
duf_sql_print_tree_prefix_uni( duf_depthinfo_t * pdi /*, int is_file */  )
{
  DEBUG_STARTR( r );

  {
    int d0 = 0;
    int max = duf_pdi_depth( pdi );

    d0 = duf_pdi_topdepth( pdi );
    for ( int d = d0; d <= max; d++ )
    {
      int du = d - 1;
      unsigned flags = 0;
      long ndu = duf_levinfo_numdir_d( pdi, du );
      char nduc = ndu > 0 ? '+' : ( ndu < 0 ? '-' : 'o' );
      int leaf = duf_levinfo_is_leaf_d( pdi, d );
      char leafc = leaf ? 'L' : 'D';

      /* int eod = duf_levinfo_eod_d( pdi, d ); */
      /* char eodc = eod ? '.' : '~';           */

      /* char is_filec = is_file ? 'F' : '-'; */

      if ( ndu > 0 && d > d0 )
        flags |= 0x1;
      if ( ndu < 0 && d > d0 )
        flags |= 0x2;
      if ( leaf )
        flags |= 0x4;
      /* if ( eod )      */
      /*   flags |= 0x8; */
      if ( d == max )
        flags |= 0x10;

      /* if ( d >= duf_pdi_maxdepth( pdi ) ) */
      if ( !duf_pdi_is_good_depth_d( pdi, leaf ? 1 : 0, d ) )
        flags |= 0x20;

      /* if ( is_file )   */
      /*   flags |= 0x40; */
      DUF_DEBUG( 1,             /* */
                 DUF_PRINTF( 0, ".[L%-2d", d ); /* */
                 DUF_PRINTF( 0, ".M%-2d", duf_pdi_maxdepth( pdi ) );
                 /* DUF_PRINTF( 0, ".rd%d", duf_pdi_reldepth( pdi ) ); */
                 DUF_PRINTF( 0, ".@%-3ld", ndu ); /* */
                 DUF_PRINTF( 0, ".%c%c", nduc, leafc ); /* */
                 DUF_PRINTF( 0, ".0x%02x]", flags ); );
      {
        /* DUF_PRINTF( 0, ".%05ld", ndu ); */
        /* if ( duf_levinfo_is_leaf_d( pdi, d ) ) */
        /*   DUF_PRINTF( 0, ".[  ◇ ]" );        */
        /* else                                   */
        switch ( flags )
        {
        case 0x14:
        case 0x34:
        case 0x35:
          DUF_PRINTF( 0, ".  → " );
          break;
        case 0x15:
          DUF_PRINTF( 0, ".│ → " );
          break;
        case 0x10:
        case 0x30:
          DUF_PRINTF( 0, ".└─── " );
          break;
        case 0x11:
        case 0x31:
          DUF_PRINTF( 0, ".├─── " );
          break;
        case 0x01:
        case 0x21:
          DUF_PRINTF( 0, ".│    " );
          break;
        case 0x12:
          /* DUF_PRINTF( 0, ".┣━━━ " ); */
          break;
        case 0x28:
        case 0x8:
          DUF_PRINTF( 0, ".     " );
          break;
        case 0x20:
        case 0x00:
          DUF_PRINTF( 0, ".     " );
          break;
        case 0x02:
          /* DUF_PRINTF( 0, ".┃    " ); */
          break;
        default:
          DUF_PRINTF( 0, ". [x%02x]", flags );
          break;
        }
      }
    }
  }
  DEBUG_ENDR( r );
}

/* callback of  duf_scan_hook_file_t */
/* __attribute__ ( ( unused ) ) */
/* static int                                                                                                 */
/* tree_scan_leaf( duf_depthinfo_t * pdi, duf_record_t * precord (*, const duf_dirhandle_t * pdh_unused *)  ) */
/* {                                                                                                          */
/*   DUF_SFIELD( filename );                                                                                  */
/*   (* const char *filename = duf_sql_str_by_name( "filename", precord, 0 ); *)                              */
/*                                                                                                            */
/*   DEBUG_STARTR( r );                                                                                       */
/*                                                                                                            */
/*   {                                                                                                        */
/*     duf_fileinfo_t fi = { 0 };                                                                             */
/*     duf_format_t format = {.filename = 0,.seq = 1 };                                                       */
/*     (* fi.st.st_mode = ( mode_t ) filemode; *)                                                             */
/*     (* fi.st.st_ino = ( ino_t ) inode; *)                                                                  */
/*     (* fi.st.st_mtim.tv_sec = mtim; *)                                                                     */
/*     (* fi.st.st_mtim.tv_nsec = 0; *)                                                                       */
/*     (* fi.st.st_uid = ( uid_t ) uid; *)                                                                    */
/*     (* fi.st.st_gid = ( gid_t ) gid; *)                                                                    */
/*     (* fi.st.st_nlink = ( nlink_t ) nlink; *)                                                              */
/*     (* fi.st.st_size = ( off_t ) filesize; *)                                                              */
/*     fi.name = filename;                                                                                    */
/*     (* fi.md5sum1 = md5sum1; *)                                                                            */
/*     (* fi.md5sum2 = md5sum2; *)                                                                            */
/*     duf_print_file_info( pdi, &fi, &format, ( duf_pdi_cb_t ) NULL, ( duf_pdi_cb_t ) NULL );                */
/*   }                                                                                                        */
/*                                                                                                            */
/*   r = duf_sql_print_tree_prefix_uni( pdi );                                                                */
/*                                                                                                            */
/*   (* SQL at duf_scan_files_by_dirid *)                                                                     */
/*                                                                                                            */
/*   DUF_PRINTF( 0, "%s", filename );                                                                         */
/*                                                                                                            */
/*   DEBUG_ENDR( r );                                                                                         */
/* }                                                                                                          */

static int
tree_scan_leaf2( duf_sqlite_stmt_t * pstmt, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );

  DUF_SFIELD2( filename );
  DUF_UFIELD2( filesize );
  DUF_UFIELD2( filemode );
  DUF_UFIELD2( md5id );
  DUF_UFIELD2( dataid );
  DUF_UFIELD2( md5sum1 );
  DUF_UFIELD2( md5sum2 );
  DUF_UFIELD2( mtime );
  DUF_UFIELD2( uid );
  DUF_UFIELD2( gid );
  DUF_UFIELD2( nlink );
  DUF_UFIELD2( inode );
  DUF_UFIELD2( exifid );
  DUF_UFIELD2( exifdt );
  DUF_UFIELD2( filenameid );
  DUF_UFIELD2( mimeid );
  DUF_SFIELD2( mime );
  DUF_UFIELD2( nsame );
  /* DUF_SFIELD( mtimef ); */
  /* DUF_SFIELD( dowmtime ); */
  /* DUF_SFIELD( monthmtime ); */

  {
    duf_fileinfo_t fi = { 0 };

    duf_format_combo_t format = {.v.flag = {
                                            .filename = 1,
                                            .short_filename = 1,
                                            .depth = 1,
                                            .seq = 1,
                                            .dirid_space = 1,
                                            .exifid = 1,
                                            .exifdt = 1,
                                            .mime = 1,
                                            .mimeid = 0,
                                            .nfiles_space = 1,
                                            .ndirs_space = 1,
                                            .inode = 0,
                                            .mode = 1,
                                            .nlink = 1,
                                            .user = 1,
                                            .group = 1,
                                            .filesize = 1,
                                            .md5 = 1,
                                            .md5id = 1,
                                            .mtime = 1,
                                            .dataid = 0,
                                            .prefix = 1,
                                            .suffix = 1,
                                            },
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
    fi.exifid = exifid;
    fi.exifdt = exifdt;
    fi.nameid = filenameid;
    fi.mime = mime;
    fi.mimeid = mimeid;
    fi.md5id = md5id;
    fi.dataid = dataid;
    fi.md5sum1 = md5sum1;
    fi.md5sum2 = md5sum2;


    if ( duf_print_file_info( pdi, &fi, &format, duf_sql_print_tree_prefix_uni, ( duf_pdi_cb_t ) NULL ) > 0 )
      DUF_PUTSL( 0 );
    else
      DUF_PUTS( 0, "????????????" );
  }

  /* r = duf_sql_print_tree_prefix_uni( pdi ); */

  /* SQL at duf_scan_files_by_dirid */

  /* DUF_PRINTF( 0, "%s", filename ); */

  DEBUG_ENDR( r );
}

/*
 * this is callback of type: duf_scan_hook_dir_t 
 * */
/* __attribute__ ( ( unused ) ) */
/* static int                                                                                                                */
/* tree_scan_node_before( unsigned long long pathid_unused, (* const duf_dirhandle_t * pdh_unused, *) duf_depthinfo_t * pdi, */
/*                        duf_record_t * precord )                                                                           */
/* {                                                                                                                         */
/*   (* DUF_SFIELD( dirname ); *)                                                                                            */
/*                                                                                                                           */
/*   DEBUG_STARTR( r );                                                                                                        */
/*                                                                                                                           */
/*   {                                                                                                                       */
/*     if ( duf_config->cli.format.seq )                                                                                     */
/*       DUF_PRINTF( 0, ".%-8llu ", pdi->seq );                                                                              */
/*     if ( duf_config->cli.format.dirid )                                                                                   */
/*       DUF_PRINTF( 0, ".[%8llu] ", pdi->levinfo[pdi->depth].dirid );                                                       */
/*     r = duf_sql_print_tree_prefix_uni( pdi );                                                                             */
/*     {                                                                                                                     */
/*       (* optimizing makes puts, segfault by NULL, therefore DUF_PRINTF(0, "%s\n", dirname  ); is not good *)              */
/*       (* DUF_PRINTF( 0, "<<<%s>>>", dirname ? dirname : "-=No dirname=-" ); *)                                            */
/*       DUF_PRINTF( 0, "%s", duf_levinfo_itemname( pdi ) );                                                                 */
/*     }                                                                                                                     */
/*   }                                                                                                                       */
/*                                                                                                                           */
/*   DEBUG_ENDR( r );                                                                                                        */
/* }                                                                                                                         */

static int
tree_scan_node_before2( duf_sqlite_stmt_t * pstmt_unused, /* unsigned long long pathid_unused, */ duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );

  {
    /* if ( duf_config->cli.format.seq )                               */
    /*   DUF_PRINTF( 0, ".%-8llu ", pdi->seq );                        */
    /* if ( duf_config->cli.format.dirid )                             */
    /*   DUF_PRINTF( 0, ".[%8llu] ", pdi->levinfo[pdi->depth].dirid ); */

    /* r = duf_sql_print_tree_prefix_uni( pdi ); */
    {
      /* optimizing makes puts, segfault by NULL, therefore DUF_PRINTF(0, "%s\n", dirname  ); is not good */
      /* DUF_PRINTF( 0, "<<<%s>>>", dirname ? dirname : "-=No dirname=-" ); */

      {
        duf_fileinfo_t fi = { 0 };

        duf_format_combo_t format = {.v.flag = {
                                                .filename = 1,
                                                .short_filename = 1,
                                                .depth = 1,
                                                .seq = 1,
                                                .dirid = 1,
                                                .exifid = 0,
                                                .exifdt = 0,
                                                .mimeid = 0,
                                                .nfiles = 1,
                                                .ndirs = 1,
                                                .inode = 0,
                                                .mode = 0,
                                                .nlink = 0,
                                                .user = 0,
                                                .group = 0,
                                                .filesize = 0,
                                                .md5 = 0,
                                                .md5id = 0,
                                                .mtime = 0,
                                                .prefix = 1,
                                                .suffix = 1,
                                                },
        .nsame = 0,
        };
        /* fi.nsame = nsame; */
        /* fi.st.st_mode = ( mode_t ) filemode; */
        /* fi.st.st_ino = ( ino_t ) inode;      */
        /* fi.st.st_mtim.tv_sec = mtime;        */
        /* fi.st.st_mtim.tv_nsec = 0;           */
        /* fi.st.st_uid = ( uid_t ) uid;        */
        /* fi.st.st_gid = ( gid_t ) gid;        */
        /* fi.st.st_nlink = ( nlink_t ) nlink;  */
        /* fi.st.st_size = ( off_t ) filesize; */
        fi.name = duf_levinfo_itemname( pdi );
        /* fi.md5id = md5id; */
        /* fi.md5sum1 = md5sum1; */
        /* fi.md5sum2 = md5sum2; */
        if ( duf_print_file_info( pdi, &fi, &format, duf_sql_print_tree_prefix_uni, ( duf_pdi_cb_t ) NULL ) > 0 )
          DUF_PUTSL( 0 );
        else
          DUF_PUTS( 0, "????????????" );
      }


      /* DUF_PRINTF( 0, "%s", duf_levinfo_itemname( pdi ) ); */
    }
  }

  DEBUG_ENDR( r );
}

#define DUF_SELECTED_NAME  "selected_filenames"
#define DUF_SELECTED_PATHS "selected_paths"
#define DUF_SELECTED_PATHTOT_FILES "selected_pathtot_files"
#define DUF_SELECTED_PATHTOT_DIRS "selected_pathtot_dirs"

#if 0
#  define DUF_SELECTED_NAME_FULL  DUF_DBPREF DUF_SELECTED_NAME
#  define DUF_SELECTED_PATHS_FULL DUF_DBPREF DUF_SELECTED_PATHS
#  define DUF_SELECTED_PATHTOT_FILES_FULL DUF_DBPREF DUF_SELECTED_PATHTOT_FILES
#  define DUF_SELECTED_PATHTOT_DIRS_FULL DUF_DBPREF DUF_SELECTED_PATHTOT_DIRS
#else
#  define DUF_SELECTED_NAME_FULL  DUF_SELECTED_NAME
#  define DUF_SELECTED_PATHS_FULL DUF_SELECTED_PATHS
#  define DUF_SELECTED_PATHTOT_FILES_FULL DUF_SELECTED_PATHTOT_FILES
#  define DUF_SELECTED_PATHTOT_DIRS_FULL DUF_SELECTED_PATHTOT_DIRS
#endif

static const char *beginning_sql[] = {
//"DELETE FROM " DUF_SELECTED_NAME /* */
//      ,
//"INSERT INTO " DUF_SELECTED_NAME /* */
//      " SELECT fn." DUF_SQL_IDNAME ",NULL,NULL" /* */
//      "   FROM filenames AS fn LEFT " /* */
//      "        JOIN " DUF_DBPREF "filedatas AS fd ON (fn.dataid=fd." DUF_SQL_IDNAME ") " /* */
//      "   LEFT JOIN " DUF_DBPREF "md5  AS md ON (md." DUF_SQL_IDNAME "=fd.md5id) " /* */
//      "   LEFT JOIN " DUF_DBPREF "exif  AS x ON (x." DUF_SQL_IDNAME "=fd.exifid) " /* */
//      "      WHERE "          /* */
//      "  ( (:minSize     IS NULL OR      fd.size>=:minSize)   AND (:maxSize   IS NULL OR fd.size<=:maxSize      ))  AND" /* */
//      "  ( (:minSame     IS NULL OR   md.dup5cnt>=:minSame)   AND (:maxSame   IS NULL OR md.dup5cnt<=:maxSame   ))  AND" /* */
//      "  ( (:minInode    IS NULL OR     fd.inode>=:minInode)  AND (:maxInode  IS NULL OR fd.inode<=:maxInode    ))  AND" /* */
//      "  ( (:minMd5ID    IS NULL OR     fd.md5id>=:minMd5ID)  AND (:maxMd5ID    IS NULL OR fd.md5id<=:maxMd5ID  ))  AND" /* */
//      "  ( (:minSd2ID    IS NULL OR     fd.sd5id>=:minSd2ID)  AND (:maxSd2ID    IS NULL OR fd.sd5id<=:maxSd2ID  ))  AND" /* */
//      "  ( (:minMimeID   IS NULL OR    fd.mimeid>=:minMimeID) AND (:maxMimeID IS NULL OR fd.mimeid<=:maxMimeID  ))  AND" /* */
//      "  ( (:minExifID   IS NULL OR    fd.exifid>=:minExifID) AND (:maxExifID IS NULL OR fd.exifid<=:maxExifID  ))  AND" /* */
//      "  ( (:minMTime    IS NULL OR      fd.mtim>=datetime(:minMTime,'unixepoch')) AND " /* */
//      "    (:maxMTime    IS NULL OR      fd.mtim<=datetime(:maxMTime,'unixepoch') )) AND" /* */
//      "  ( (:minExifDT   IS NULL OR  x.date_time>=datetime(:minExifDT,'unixepoch')) AND" /* */
//      "    (:maxExifDT   IS NULL OR  x.date_time<=datetime(:maxExifDT,'unixepoch'))  ) AND" /* */
//      "  ( (:minNameID   IS NULL OR fn." DUF_SQL_IDNAME ">=:minNameID) AND (:maxNameID IS NULL OR fn." DUF_SQL_IDNAME "<=:maxNameID )) AND" /* */
//      "  ( (:minDirID    IS NULL OR    fn.Pathid>=:minDirID)  AND (:maxDirID IS NULL OR fn.Pathid<=:maxDirID )) AND" /* */
//      "  ( (:minExifSame IS NULL OR x.dupexifcnt>=:minExifSame)  AND (:maxExifSame   IS NULL OR md.dup5cnt<=:maxExifSame   )) AND " /* */
//      "  ( :GName        IS NULL OR fn.name GLOB :GName ) " /* */
//      " AND "                 /* */
//      "  ( ( :GSameAs    IS NULL OR :GSamePathID IS NULL ) " /* */
//      " OR md." DUF_SQL_IDNAME "=(SELECT fdb.md5id FROM filenames AS fnb " /* */
//      "   JOIN " DUF_DBPREF "filedatas AS fdb ON (fnb.dataid=fdb." DUF_SQL_IDNAME ") " /* */
//      "          WHERE fnb.name GLOB :GSameAs AND fnb.Pathid=:GSamePathID ) ) " /* */
//      , 
//"DROP TABLE IF EXISTS " DUF_SELECTED_NAME_FULL /* */ ,
  "CREATE TEMPORARY  TABLE  " DUF_SELECTED_NAME_FULL /* */
        " AS "                  /* */
        " SELECT fn." DUF_SQL_IDNAME
        /* ",NULL AS last_updated,NULL AS inow" */
        "   FROM filenames AS fn LEFT " /* */
        "        JOIN " DUF_DBPREF "filedatas AS fd ON (fn.dataid=fd." DUF_SQL_IDNAME ") " /* */
        "   LEFT JOIN " DUF_DBPREF "md5  AS md ON (md." DUF_SQL_IDNAME "=fd.md5id) " /* */
        "   LEFT JOIN " DUF_DBPREF "exif  AS x ON (x." DUF_SQL_IDNAME "=fd.exifid) " /* */
        "      WHERE "          /* */
        "  ( (:minSize     IS NULL OR      fd.size>=:minSize)   AND (:maxSize   IS NULL OR fd.size<=:maxSize      ))  AND" /* */
        "  ( (:minSame     IS NULL OR   md.dup5cnt>=:minSame)   AND (:maxSame   IS NULL OR md.dup5cnt<=:maxSame   ))  AND" /* */
        "  ( (:minInode    IS NULL OR     fd.inode>=:minInode)  AND (:maxInode  IS NULL OR fd.inode<=:maxInode    ))  AND" /* */
        "  ( (:minMd5ID    IS NULL OR     fd.md5id>=:minMd5ID)  AND (:maxMd5ID    IS NULL OR fd.md5id<=:maxMd5ID  ))  AND" /* */
        "  ( (:minSd2ID    IS NULL OR     fd.sd5id>=:minSd2ID)  AND (:maxSd2ID    IS NULL OR fd.sd5id<=:maxSd2ID  ))  AND" /* */
        "  ( (:minMimeID   IS NULL OR    fd.mimeid>=:minMimeID) AND (:maxMimeID IS NULL OR fd.mimeid<=:maxMimeID  ))  AND" /* */
        "  ( (:minExifID   IS NULL OR    fd.exifid>=:minExifID) AND (:maxExifID IS NULL OR fd.exifid<=:maxExifID  ))  AND" /* */
        "  ( (:minMTime    IS NULL OR      fd.mtim>=datetime(:minMTime,'unixepoch')) AND " /* */
        "    (:maxMTime    IS NULL OR      fd.mtim<=datetime(:maxMTime,'unixepoch') )) AND" /* */
        "  ( (:minExifDT   IS NULL OR  x.date_time>=datetime(:minExifDT,'unixepoch')) AND" /* */
        "    (:maxExifDT   IS NULL OR  x.date_time<=datetime(:maxExifDT,'unixepoch'))  ) AND" /* */
        "  ( (:minNameID   IS NULL OR fn." DUF_SQL_IDNAME ">=:minNameID) AND (:maxNameID IS NULL OR fn." DUF_SQL_IDNAME "<=:maxNameID )) AND" /* */
        "  ( (:minDirID    IS NULL OR    fn.Pathid>=:minDirID)  AND (:maxDirID IS NULL OR fn.Pathid<=:maxDirID )) AND" /* */
        "  ( (:minExifSame IS NULL OR x.dupexifcnt>=:minExifSame)  AND (:maxExifSame   IS NULL OR md.dup5cnt<=:maxExifSame   )) AND " /* */
        "  ( :GName        IS NULL OR fn.name GLOB :GName ) " /* */
        " AND "                 /* */
        "  ( ( :GSameAs    IS NULL OR :GSamePathID IS NULL ) " /* */
        " OR md." DUF_SQL_IDNAME "=(SELECT fdb.md5id FROM filenames AS fnb " /* */
        "   JOIN " DUF_DBPREF "filedatas AS fdb ON (fnb.dataid=fdb." DUF_SQL_IDNAME ") " /* */
        "          WHERE fnb.name GLOB :GSameAs AND fnb.Pathid=:GSamePathID ) ) " /* */
        ,
//"DROP TABLE IF EXISTS " DUF_SELECTED_PATHS_FULL /* */ ,
  "CREATE TEMPORARY TABLE  " DUF_SELECTED_PATHS_FULL " AS " /* */
        " WITH RECURSIVE parents_cte(fid, did, parentid, fname) AS " /* */
        "   ( SELECT t.rowid as fid, fn.dataid AS did, p.rowid as parentid, fn.name as fname " /* */
        "      FROM " DUF_SELECTED_NAME_FULL " AS t LEFT JOIN filenames AS fn ON (t.rowid=fn.rowid) " /* */
        "         LEFT JOIN paths AS p ON (p.rowid=fn.Pathid) " /* */
        " UNION " " SELECT fid, did, pp.parentid as parentid, fname " " FROM parents_cte " /* */
        " LEFT JOIN paths as pp ON( pp.rowid = parents_cte.parentid ) " /* */
        " ) " " SELECT fid, did, parentid, fname from parents_cte WHERE parentid is not null GROUP BY parentid ",
  " CREATE TEMPORARY TABLE " DUF_SELECTED_PATHTOT_FILES_FULL /* */
        " AS "                  /* */
        " SELECT fn.Pathid AS Pathid, COUNT(*) AS numfiles, min( size ) AS minsize, max( size ) AS maxsize " /* */
        " FROM " DUF_DBPREF " filenames AS fn " /* */
        " LEFT JOIN " DUF_DBPREF " filedatas AS fd ON( fn.dataid = fd." DUF_SQL_IDNAME " ) " /* */
        " GROUP BY fn.Pathid " /* */ ,
  " CREATE TEMPORARY TABLE " DUF_SELECTED_PATHTOT_DIRS_FULL /* */
        " AS "                  /* */
        " SELECT parents." DUF_SQL_IDNAME " AS Pathid, COUNT( * )AS numdirs " /* */
        " FROM "                /* */
#if 0
        DUF_DBPREF " paths "    /* */
#else
        DUF_SELECTED_PATHS_FULL " AS pts " " LEFT JOIN " DUF_DBPREF " paths ON( pts.parentid = paths.rowid ) " /* */
#endif
        " JOIN " DUF_DBPREF " paths AS parents ON( parents." DUF_SQL_IDNAME " = paths.parentid ) " /* */
        " GROUP BY parents." DUF_SQL_IDNAME " " /* */
};

duf_scan_callbacks_t duf_print_tree_callbacks = {
  .title = __FILE__ ".tree ",
  .init_scan = NULL,
  .beginning_sql_argv = beginning_sql,
  /* .node_scan_before = tree_scan_node_before, */
  .node_scan_before2 = tree_scan_node_before2,
  /* .leaf_scan = tree_scan_leaf, */
  .leaf_scan2 = tree_scan_leaf2,
  .leaf = {.fieldset =          /* */
           " fn.Pathid AS dirid " /* */
           ", fn.name AS filename, fd.size AS filesize, fd.exifid as exifid, fd.mimeid as mimeid " /* */
           ", fd.size AS filesize " /* */
           ", uid, gid, nlink, inode " /* */
           ", STRFTIME( '%s', mtim ) AS mtime " /* */
           ", fd." DUF_SQL_IDNAME " AS dataid " /* */
           ", fd.mode AS filemode " /* */
           ", md.md5sum1, md.md5sum2 " /* */
           ", fn." DUF_SQL_IDNAME " AS filenameid " /* */
           ", md.dup5cnt AS nsame " /* */
           ", mi.mime AS mime " /* */
           ", STRFTIME( '%s', x.date_time ) AS exifdt " /* */
           /* ", md." DUF_SQL_IDNAME " AS md5id " (* *) */
           ", fd.md5id AS md5id " /* */
           ,
           /* .selector = " SELECT % s FROM " DUF_DBPREF " filenames AS fn " (* *)                           */
           /*       " LEFT JOIN " DUF_DBPREF " filedatas AS fd ON( fn.dataid = fd." DUF_SQL_IDNAME " ) " (* *)                     */
           /*       " LEFT JOIN " DUF_DBPREF " md5 AS md ON( md." DUF_SQL_IDNAME " = fd.md5id ) " (* *)                             */
           /*       " WHERE "            (* *)                                                              */
           /*       (* " fd.size >= %llu AND fd.size < %llu "            *)                          */
           /*       (* " AND( md.dup5cnt IS NULL OR ( md.dup5cnt >= %llu AND md.dup5cnt < %llu ) ) AND " *) */
           /*       " fn.Pathid = '%llu' "    (* *)                                                              */
           /*       ,                                                                                          */
           .selector2 =         /* */
           /* " SELECT % s " */
           " FROM " DUF_SELECTED_NAME_FULL " AS fns " /* */
           " JOIN " DUF_DBPREF " filenames AS fn ON( fns." DUF_SQL_IDNAME " = fn." DUF_SQL_IDNAME " ) " /* */
           " LEFT JOIN " DUF_DBPREF " filedatas AS fd ON( fn.dataid = fd." DUF_SQL_IDNAME " ) " /* */
           " LEFT JOIN " DUF_DBPREF " md5 AS md ON( md." DUF_SQL_IDNAME " = fd.md5id ) " /* */
           " LEFT JOIN " DUF_DBPREF " mime AS mi ON( mi." DUF_SQL_IDNAME " = fd.mimeid ) " /* */
           " LEFT JOIN " DUF_DBPREF " exif AS x ON( x." DUF_SQL_IDNAME " = fd.exifid ) " /* */
           " LEFT JOIN " DUF_DBPREF " exif_model AS xm ON( x.modelid = xm." DUF_SQL_IDNAME " ) " /* */
           " WHERE "            /* */
           " fn.Pathid =:parentdirID " /* */
           ,
           .selector_total2 =   /* */
           " FROM " DUF_DBPREF " filenames AS fn " /* */
           " LEFT JOIN " DUF_DBPREF " filedatas AS fd ON( fn.dataid = fd." DUF_SQL_IDNAME " ) " /* */
           " LEFT JOIN " DUF_DBPREF " md5 AS md ON( md." DUF_SQL_IDNAME " = fd.md5id ) " /* */
//         " FROM "  DUF_SELECTED_NAME_FULL " AS fns " /* */
//         " JOIN " DUF_DBPREF " filenames AS fn ON( fns." DUF_SQL_IDNAME " = fn." DUF_SQL_IDNAME " ) " /* */
//         " LEFT JOIN " DUF_DBPREF " filedatas AS fd ON( fn.dataid = fd." DUF_SQL_IDNAME " ) " /* */
//         " LEFT JOIN " DUF_DBPREF " md5 AS md ON( md." DUF_SQL_IDNAME " = fd.md5id ) " /* */
           },
  .node = {.fieldset = " pt." DUF_SQL_IDNAME " AS dirid, pt.dirname, pt.dirname AS dfname, pt.parentid " /* */
           ", tf.numfiles AS nfiles, td.numdirs AS ndirs, tf.maxsize AS maxsize, tf.minsize AS minsize " /* */
           ,
/*   .selector = " SELECT pt." DUF_SQL_IDNAME " AS dirid, pt.dirname " (* *)                                                                    */
/*         ", pt.dirname AS dfname, pt.parentid " (* *)                                                                                */
/*         ", tf.numfiles AS nfiles, td.numdirs AS ndirs, tf.maxsize AS maxsize, tf.minsize AS minsize " (* *)                          */
/* (*      ", ( SELECT count( * )FROM " DUF_DBPREF " paths AS sp WHERE sp.parentid = pt." DUF_SQL_IDNAME " ) AS ndirs "                                       */
/*            ", ( SELECT count( * )FROM "  DUF_SELECTED_NAME_FULL " AS sfn "                                                                 */
/*            " JOIN " DUF_DBPREF " filedatas AS sfd ON( sfn.dataid = sfd." DUF_SQL_IDNAME " ) "                                                   */
/*            " JOIN " DUF_DBPREF " md5 AS smd ON( sfd.md5id = smd." DUF_SQL_IDNAME " ) "                                                          */
/*            " WHERE "                                                                                                      */
/*            " sfn.Pathid = pt." DUF_SQL_IDNAME " "                                                                                     */
/*            " AND sfd.size >= %llu AND sfd.size < %llu "                                                                 */
/*            " AND( smd.dup5cnt IS NULL OR( smd.dup5cnt >= %llu AND smd.dup5cnt < %llu ) ) "                                */
/*            " ) AS nfiles "                                                                                                           */
/*            ", ( SELECT min( sfd.size ) FROM " DUF_DBPREF " filedatas AS sfd JOIN "  DUF_SELECTED_NAME_FULL " AS sfn ON( sfn.dataid = sfd." DUF_SQL_IDNAME " ) " */
/*            " WHERE sfn.Pathid = pt." DUF_SQL_IDNAME " ) AS minsize "                                                                          */
/*            ", ( SELECT max( sfd.size ) FROM " DUF_DBPREF " filedatas AS sfd JOIN "  DUF_SELECTED_NAME_FULL " AS sfn ON( sfn.dataid = sfd." DUF_SQL_IDNAME " ) " */
/*            " WHERE sfn.Pathid = pt." DUF_SQL_IDNAME " ) AS maxsize " *)                                                                       */
/*         " FROM " DUF_DBPREF " paths AS pt " (* *)                                                                                     */
/*         " LEFT JOIN " DUF_DBPREF " pathtot_dirs AS td ON( td.Pathid = pt." DUF_SQL_IDNAME " ) " (* *)                                                    */
/*         " LEFT JOIN " DUF_DBPREF " pathtot_files AS tf ON( tf.Pathid = pt." DUF_SQL_IDNAME " ) " (* *)                                                   */
/*         " WHERE pt.parentid = '%llu' " (* *)                                                                                           */
/*         ,                                                                                                                            */
           .selector2 =         /* */
           /* " SELECT pt." DUF_SQL_IDNAME " AS dirid, pt.dirname "                                                           */
           /* ", pt.dirname AS dfname, pt.parentid "                                                       */
           /* ", tf.numfiles AS nfiles, td.numdirs AS ndirs, tf.maxsize AS maxsize, tf.minsize AS minsize " */
           " FROM "
#if 0
           " paths AS pt " " LEFT JOIN " DUF_DBPREF " pathtot_dirs AS td ON( td.Pathid = pt." DUF_SQL_IDNAME " ) " /* */
           " LEFT JOIN " DUF_DBPREF " pathtot_files AS tf ON( tf.Pathid = pt." DUF_SQL_IDNAME " ) " /* */
#else
           DUF_SELECTED_PATHS_FULL " AS pts " /* */
           " LEFT JOIN " DUF_DBPREF " paths AS pt ON( pts.parentid = pt.rowid ) " /* */
           " LEFT JOIN " DUF_SELECTED_PATHTOT_DIRS_FULL "  AS td ON ( td.Pathid = pt." DUF_SQL_IDNAME " ) " /* */
           " LEFT JOIN " DUF_SELECTED_PATHTOT_FILES_FULL "  AS tf ON ( tf.Pathid = pt." DUF_SQL_IDNAME " ) " /* */
#endif
           " WHERE pt.ParentId =:parentdirID AND ( :dirName IS NULL OR dirname=:dirName ) " /* */
           }

  /* , .final_sql_argv = final_sql, */
};
