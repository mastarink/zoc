#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_maintenance.h"
#include "duf_hook_types.h"
#include "duf_fileinfo_types.h"

#include "duf_service.h"
#include "duf_config_ref.h"

#include "duf_pdi.h"
#include "duf_levinfo.h"

#include "duf_sql_defs.h"
#include "duf_sql_field.h"


/* ###################################################################### */
/* #include "duf_dir_print_uni.h" */
/* ###################################################################### */




/* callback of  duf_scan_hook_file_t */
/* static int                                                                                                           */
/* scan_leaf( duf_depthinfo_t * pdi, duf_record_t * precord (*, const duf_dirhandle_t * pdh_unused *)  )                */
/* {                                                                                                                    */
/*   int r = 0;                                                                                                         */
/*                                                                                                                      */
/*   DEBUG_START(  );                                                                                                   */
/*                                                                                                                      */
/*   (* SQL at duf_scan_files_by_dirid *)                                                                               */
/*   DUF_SFIELD( filename );                                                                                            */
/*   (* const char *filename = duf_sql_str_by_name( "filename", precord, 0 ); *)                                        */
/*   DUF_UFIELD( filesize );                                                                                            */
/*   (* unsigned long long filesize = duf_sql_ull_by_name( "filesize", precord, 0 ); *)                                 */
/*   DUF_UFIELD( filemode );                                                                                            */
/*   (* unsigned long long filemode = duf_sql_ull_by_name( "filemode", precord, 0 ); *)                                 */
/*   DUF_UFIELD( md5sum1 );                                                                                             */
/*   (* unsigned long long md5a = duf_sql_ull_by_name( "md5sum1", precord, 0 ); *)                                      */
/*   DUF_UFIELD( md5sum2 );                                                                                             */
/*   (* unsigned long long md5b = duf_sql_ull_by_name( "md5sum2", precord, 0 ); *)                                      */
/*   DUF_UFIELD( mtime );                                                                                               */
/*   DUF_UFIELD( uid );                                                                                                 */
/*   DUF_UFIELD( gid );                                                                                                 */
/*   DUF_UFIELD( nlink );                                                                                               */
/*   DUF_UFIELD( inode );                                                                                               */
/*   DUF_UFIELD( nsame );                                                                                               */
/*   (* DUF_SFIELD( mtimef ); *)                                                                                        */
/*   (* DUF_SFIELD( dowmtime ); *)                                                                                      */
/*   (* DUF_SFIELD( monthmtime ); *)                                                                                    */
/*                                                                                                                      */
/*   (* if (nsame<2) return r; *)                                                                                       */
/*                                                                                                                      */
/*   (* DUF_PRINTF(0, ".> %s\n", duf_sql_str_by_name( "filename", precord ) ); *)                                       */
/*   (* DUF_PRINTF(0, ".-rw-------  1 mastar mastar-firefox 106580068 Jan 27 2014 12:35:27 sample_video_hd.zip\n" ); *) */
/*   (* duf_sql_print_tree_prefix_uni( pdi, 1 ); *)                                                                     */
/*   {                                                                                                                  */
/*     duf_fileinfo_t fi;                                                                                               */
/*                                                                                                                      */
/*     duf_format_t format = {                                                                                          */
/*       .filename = 1,                                                                                                 */
/*       .seq = 1,                                                                                                      */
/*       .dirid = 1,                                                                                                    */
/*       .inode = 1,                                                                                                    */
/*       .mode = 1,                                                                                                     */
/*       .nlink = 1,                                                                                                    */
/*       .uid = 1,                                                                                                      */
/*       .gid = 1,                                                                                                      */
/*       .filesize = 1,                                                                                                 */
/*       .md5 = 1,                                                                                                      */
/*       .mtime = 1,                                                                                                    */
/*       .nsame = 1,                                                                                                    */
/*     };                                                                                                               */
/*     fi.nsame = nsame;                                                                                                */
/*     fi.st.st_mode = ( mode_t ) filemode;                                                                             */
/*     fi.st.st_ino = ( ino_t ) inode;                                                                                  */
/*     fi.st.st_mtim.tv_sec = mtime;                                                                                    */
/*     fi.st.st_mtim.tv_nsec = 0;                                                                                       */
/*     fi.st.st_uid = ( uid_t ) uid;                                                                                    */
/*     fi.st.st_gid = ( gid_t ) gid;                                                                                    */
/*     fi.st.st_nlink = ( nlink_t ) nlink;                                                                              */
/*     fi.st.st_size = ( off_t ) filesize;                                                                              */
/*     fi.name = filename;                                                                                              */
/*     fi.md5sum1 = md5sum1;                                                                                            */
/*     fi.md5sum2 = md5sum2;                                                                                            */
/*     duf_print_file_info( pdi, &fi, &format, ( duf_pdi_cb_t ) NULL, ( duf_pdi_cb_t ) NULL );                          */
/*   }                                                                                                                  */
/*   DUF_PUTSL( 0 );                                                                                                    */
/*                                                                                                                      */
/*   DEBUG_ENDR( r );                                                                                                   */
/*   return r;                                                                                                          */
/* }                                                                                                                    */

static int
scan_leaf2( duf_sqlite_stmt_t * pstmt, duf_depthinfo_t * pdi )
{
  int r = 0;

  DEBUG_START(  );

  /* SQL at duf_scan_files_by_dirid */
  DUF_UFIELD2( dirid );
  DUF_SFIELD2( filename );
  /* const char *filename = duf_sql_str_by_name( "filename", precord, 0 ); */
  DUF_UFIELD2( filesize );
  /* unsigned long long filesize = duf_sql_ull_by_name( "filesize", precord, 0 ); */
  DUF_UFIELD2( filemode );
  /* unsigned long long filemode = duf_sql_ull_by_name( "filemode", precord, 0 ); */
  DUF_UFIELD2( md5id );
  DUF_UFIELD2( dataid );
  DUF_UFIELD2( md5sum1 );
  /* unsigned long long md5a = duf_sql_ull_by_name( "md5sum1", precord, 0 ); */
  DUF_UFIELD2( md5sum2 );
  /* unsigned long long md5b = duf_sql_ull_by_name( "md5sum2", precord, 0 ); */
  DUF_UFIELD2( mtime );
  DUF_UFIELD2( uid );
  DUF_UFIELD2( gid );
  DUF_UFIELD2( nlink );
  DUF_UFIELD2( inode );
  DUF_UFIELD2( exifid );
  DUF_UFIELD2( filenameid );
  DUF_UFIELD2( mimeid );
  DUF_SFIELD2( mime );
  DUF_UFIELD2( nsame );
  /* DUF_SFIELD( mtimef ); */
  /* DUF_SFIELD( dowmtime ); */
  /* DUF_SFIELD( monthmtime ); */

  /* if (nsame<2) return r; */

  /* DUF_PRINTF(0, ".> %s\n", duf_sql_str_by_name( "filename", precord ) ); */
  /* DUF_PRINTF(0, ".-rw-------  1 mastar mastar-firefox 106580068 Jan 27 2014 12:35:27 sample_video_hd.zip\n" ); */
  /* duf_sql_print_tree_prefix_uni( pdi, 1 ); */
  {
    duf_fileinfo_t fi = { 0 };

    duf_format_combo_t format = {.v.flag = {
                                            .filename = 1,
                                            .seq = 1,
                                            .seq_leaf = 1,
                                            .dirid = 1,
                                            .exifid = 1,
                                            .nameid = 1,
                                            .mime = 1,
                                            .mimeid = 1,
                                            .inode = 1,
                                            .mode = 1,
                                            .nlink = 1,
                                            .user = 1,
                                            .group = 1,
                                            .filesize = 1,
                                            .md5 = 1,
                                            .md5id = 1,
                                            .mtime = 1,
                                            .dataid = 1,
                                            },
    .nsame = 1,
    };
    fi.nsame = nsame;
    fi.dirid = dirid;
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
    fi.nameid = filenameid;
    fi.mime = mime;
    fi.mimeid = mimeid;
    fi.md5id = md5id;
    fi.dataid = dataid;
    fi.md5sum1 = md5sum1;
    fi.md5sum2 = md5sum2;


    duf_print_file_info( pdi, &fi, &format, ( duf_pdi_cb_t ) NULL, ( duf_pdi_cb_t ) NULL );
  }
  DUF_PUTSL( 0 );

  DEBUG_ENDR( r );
  return r;
}

/* 
 * this is callback of type: duf_scan_hook_dir_t
 * */
/* static int                                                                                                               */
/* scan_node_before( unsigned long long pathid_unused, duf_depthinfo_t * pdi, duf_record_t * precord )                      */
/* {                                                                                                                        */
/*   int r = 0;                                                                                                             */
/*                                                                                                                          */
/*   DEBUG_START(  );                                                                                                       */
/*                                                                                                                          */
/*   {                                                                                                                      */
/*     const char *real_path = NULL;                                                                                        */
/*                                                                                                                          */
/*     if ( !real_path )                                                                                                    */
/*       real_path = duf_levinfo_path( pdi );                                                                               */
/*                                                                                                                          */
/*     DUF_UFIELD( nfiles );                                                                                                */
/*     DUF_UFIELD( minsize );                                                                                               */
/*     DUF_UFIELD( maxsize );                                                                                               */
/*                                                                                                                          */
/*     if ( nfiles )                                                                                                        */
/*     {                                                                                                                    */
/*       (* optimizing makes puts, segfault by NULL, therefore DUF_PRINTF(0, ".%s\n", path  ); is not good *)               */
/*       if ( duf_config->cli.format.seq )                                                                                  */
/*         DUF_PRINTF( 0, ".d%7llu ", pdi->seq );                                                                           */
/*       if ( duf_config->cli.format.dirid )                                                                                */
/*         DUF_PRINTF( 0, ".[%8llu] ", pdi->levinfo[pdi->depth].dirid );                                                    */
/*       DUF_PRINTF( 0, ". (%5llu) %llu-%llu %s\n", nfiles, minsize, maxsize, real_path ? real_path : "-=No real_path=-" ); */
/*     }                                                                                                                    */
/*     (* mas_free( path ); *)                                                                                              */
/*   }                                                                                                                      */
/*                                                                                                                          */
/*   DEBUG_ENDR( r );                                                                                                       */
/*                                                                                                                          */
/*   return r;                                                                                                              */
/* }                                                                                                                        */

static int
scan_node_before2( duf_sqlite_stmt_t * pstmt, unsigned long long pathid_unused, duf_depthinfo_t * pdi )
{
  int r = 0;

  DEBUG_START(  );

  /* DUF_UFIELD2( nfiles ); */
  /* DUF_UFIELD2( minsize ); */
  /* DUF_UFIELD2( maxsize ); */
  /* {                                                                                                                      */
  /*   const char *real_path = NULL;                                                                                        */
  /*                                                                                                                        */
  /*   if ( !real_path )                                                                                                    */
  /*     real_path = duf_levinfo_path( pdi );                                                                               */
  /*                                                                                                                        */
  /*   if ( nfiles )                                                                                                        */
  /*   {                                                                                                                    */
  /*     (* optimizing makes puts, segfault by NULL, therefore DUF_PRINTF(0, ".%s\n", path  ); is not good *)               */
  /*     if ( duf_config->cli.format.seq )                                                                                  */
  /*       DUF_PRINTF( 0, ".d%7llu ", pdi->seq );                                                                           */
  /*     if ( duf_config->cli.format.dirid )                                                                                */
  /*       DUF_PRINTF( 0, ".[%8llu] ", pdi->levinfo[pdi->depth].dirid );                                                    */
  /*     DUF_PRINTF( 0, ". (%5llu) %llu-%llu %s\n", nfiles, minsize, maxsize, real_path ? real_path : "-=No real_path=-" ); */
  /*   }                                                                                                                    */
  /*   (* mas_free( path ); *)                                                                                              */
  /* }                                                                                                                      */
  if ( duf_levinfo_items_files( pdi ) > 0 )
  {
    duf_fileinfo_t fi = { 0 };

    duf_format_combo_t format = {.v.flag = {
                                            .filename = 0,
                                            .realpath = 1,
                                            .seq = 1,
                                            .seq_node = 1,
                                            .dirid = 1,
                                            .exifid = 0,
                                            .mimeid = 0,
                                            .inode = 0,
                                            .mode = 0,
                                            .nlink = 0,
                                            .user = 0,
                                            .group = 0,
                                            .filesize = 0,
                                            .md5 = 0,
                                            .md5id = 0,
                                            .dataid = 0,
                                            .mtime = 0,
                                            .prefix = 1,
                                            .suffix = 1,
                                            }
    ,
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
    /* fi.name = duf_levinfo_itemname( pdi ); */
    /* fi.md5id = md5id; */
    /* fi.md5sum1 = md5sum1; */
    /* fi.md5sum2 = md5sum2; */
    DUF_DEBUG( 0, PF( "at module : %llx (%d) :: %llx", format.v.bit, format.v.flag.seq, duf_config->cli.format.v.bit ) );
    DUF_PUTSL( 0 );
    if ( duf_print_file_info( pdi, &fi, &format, ( duf_pdi_cb_t ) NULL, ( duf_pdi_cb_t ) NULL ) > 0 )
      DUF_PUTSL( 0 );
    else
      DUF_PUTS( 0, "????????????" );
  }

  DEBUG_ENDR( r );

  return r;
}

#define DUF_SELECTED_NAME "fnselected"
static const char *beginning_sql[] = {
  "DELETE FROM " DUF_SELECTED_NAME /* */
        ,
  "INSERT INTO " DUF_SELECTED_NAME /* */
        " SELECT fn." DUF_SQL_IDNAME ",NULL,NULL" /* */
        "   FROM filenames AS fn LEFT " /* */
        "        JOIN " DUF_DBPREF "filedatas AS fd ON (fn.dataid=fd." DUF_SQL_IDNAME ") " /* */
        "   LEFT JOIN " DUF_DBPREF "md5  AS md ON (md." DUF_SQL_IDNAME "=fd.md5id) " /* */
        "   LEFT JOIN " DUF_DBPREF "exif  AS x ON (x." DUF_SQL_IDNAME "=fd.exifid) " /* */
        "      WHERE "          /* */
        "  ( (:minSize     IS NULL OR fd.size>=:minSize)     AND (:maxSize   IS NULL OR fd.size<=:maxSize      ))  AND" /* */
        "  ( (:minSame     IS NULL OR md.dup5cnt>=:minSame)  AND (:maxSame   IS NULL OR md.dup5cnt<=:maxSame   ))  AND" /* */
        "  ( (:minInode    IS NULL OR fd.inode>=:minInode)   AND (:maxInode  IS NULL OR fd.inode<=:maxInode    ))  AND" /* */
        "  ( (:min5ID      IS NULL OR fd.md5id>=:min5ID)     AND (:max5ID    IS NULL OR fd.md5id<=:max5ID      ))  AND" /* */
        "  ( (:min2ID      IS NULL OR fd.sd5id>=:min2ID)     AND (:max2ID    IS NULL OR fd.sd5id<=:max2ID      ))  AND" /* */
        "  ( (:minMimeID   IS NULL OR fd.mimeid>=:minMimeID) AND (:maxMimeID IS NULL OR fd.mimeid<=:maxMimeID  ))  AND" /* */
        "  ( (:minExifID   IS NULL OR fd.exifid>=:minExifID) AND (:maxExifID IS NULL OR fd.exifid<=:maxExifID  ))  AND" /* */
        "  ( (:minMTime    IS NULL OR fd.mtim>=datetime(:minMTime,'unixepoch')) AND " /* */
        "                 (:maxMTime IS NULL OR fd.mtim<=datetime(:maxMTime,'unixepoch') )) AND" /* */
        "  ( (:minExifDT   IS NULL OR x.date_time>=datetime(:minExifDT,'unixepoch')) AND" /* */
        "                 (:maxExifDT IS NULL OR fd.mtim<=datetime(:maxExifDT,'unixepoch') )) AND" /* */
        "  ( (:minNameID   IS NULL OR fn." DUF_SQL_IDNAME ">=:minNameID) AND (:maxNameID IS NULL OR fn." DUF_SQL_IDNAME "<=:maxNameID )) AND" /* */
        "  ( (:minDirID    IS NULL OR fn.Pathid>=:minDirID)  AND (:maxDirID IS NULL OR fn.Pathid<=:maxDirID )) AND" /* */
        "  ( (:minExifSame IS NULL OR x.dupexifcnt>=:minExifSame)  AND (:maxExifSame   IS NULL OR md.dup5cnt<=:maxExifSame   )) AND " /* */
        "  ( :GName        IS NULL OR fn.name GLOB :GName ) " /* */
        " AND "                 /* */
        "  ( ( :GSameAs    IS NULL OR :GSamePathID IS NULL ) " /* */
        " OR md." DUF_SQL_IDNAME "=(SELECT fdb.md5id FROM filenames AS fnb " /* */
        "                              JOIN " DUF_DBPREF "filedatas AS fdb ON (fnb.dataid=fdb." DUF_SQL_IDNAME ") " /* */
        "                                      WHERE fnb.name GLOB :GSameAs AND fnb.Pathid=:GSamePathID ) ) " /* */
        ,
};

duf_scan_callbacks_t duf_print_dir_callbacks = {
  .title = __FILE__ ".dir",
  .init_scan = NULL,
  .beginning_sql_argv = beginning_sql,
  /* .node_scan_before = scan_node_before, */
  .node_scan_before2 = scan_node_before2,
  /* .leaf_scan = scan_leaf, */
  .leaf_scan2 = scan_leaf2,
  .leaf = {.fieldset =          /* */
           "fn.Pathid AS dirid " /* */
           ", fn.name AS filename, fd.size AS filesize, fd.exifid as exifid, fd.mimeid as mimeid" /* */
           ", fd.size AS filesize " /* */
           ", uid, gid, nlink, inode, strftime('%s',mtim) AS mtime " /* */
           ", fd." DUF_SQL_IDNAME " AS dataid " /* */
           ", fd.mode AS filemode " /* */
           ", md.md5sum1, md.md5sum2 " /* */
           ", fn." DUF_SQL_IDNAME " AS filenameid" /* */
           ", md.dup5cnt AS nsame" /* */
           ", mi.mime AS mime"  /* */
           /* ", md." DUF_SQL_IDNAME " AS md5id" (* *) */
           ", fd.md5id AS md5id" /* */
           ,
           /* " , DATETIME(mtim, 'unixepoch') AS mtimef " */
           /* ", strftime('%Y-%m-%d %H:%M:%S',mtim,'unixepoch') AS mtimef " */
           /* ", case cast (strftime('%w', mtim,'unixepoch') AS integer) "                                                                   */
           /* " when 0 then 'Sun' when 1 then 'Mon' when 2 then 'Tue' when 3 then 'Wed' "                                                    */
           /* " when 4 then 'Thu' when 5 then 'Fri' else 'Sat' end AS dowmtime, " "case cast (strftime('%m', mtim,'unixepoch') AS integer) " */
           /* " when 1 then 'Jan' when 2 then 'Feb' when 3 then 'Mar' when 4 then 'Apr' when 5 then 'May' when 6 then "                      */
           /* " 'Jun' when 7 then 'Jul' when 8 then 'Aug' when 9 then 'Sep' when 10 then 'Oct' when 11 then 'Nov' when 12 then 'Dec' "       */
           /* " else 'Wow' end AS monthmtime "                                                                                               */
           /* .leaf_selector =              (* *)                                                              */
           /*       "SELECT %s "            (* *)                                                              */
           /*       " FROM " DUF_DBPREF DUF_SELECTED_NAME " AS fn " (* *)                                               */
           /*       " LEFT JOIN " DUF_DBPREF "filedatas AS fd ON (fn.dataid=fd." DUF_SQL_IDNAME ") " (* *)                     */
           /*       " LEFT JOIN " DUF_DBPREF "md5  AS md ON (md." DUF_SQL_IDNAME "=fd.md5id)" (* *)                            */
           /*       "    WHERE "            (* *)                                                              */
           /*       (* "           fd.size >= %llu AND fd.size < %llu "            *)                          */
           /*       (* "       AND (md.dup5cnt IS NULL OR (md.dup5cnt >= %llu AND md.dup5cnt < %llu)) AND " *) */
           /*       " fn.Pathid='%llu' ",                                                                      */
           .selector2 =         /* */
           /* "SELECT %s " */
           " FROM " DUF_DBPREF DUF_SELECTED_NAME " AS fns " /* */
           " JOIN " DUF_DBPREF "filenames AS fn ON (fns." DUF_SQL_IDNAME "=fn." DUF_SQL_IDNAME ")" /* */
           " LEFT JOIN " DUF_DBPREF "filedatas AS fd ON (fn.dataid=fd." DUF_SQL_IDNAME ") " /* */
           " LEFT JOIN " DUF_DBPREF "md5 AS md ON (md." DUF_SQL_IDNAME "=fd.md5id)" /* */
           " LEFT JOIN " DUF_DBPREF "mime  AS mi ON (mi." DUF_SQL_IDNAME "=fd.mimeid) " /* */
           " LEFT JOIN " DUF_DBPREF "exif  AS x ON (x." DUF_SQL_IDNAME "=fd.exifid) " /* */
           " LEFT JOIN " DUF_DBPREF "exif_model  AS xm ON (x.modelid=xm." DUF_SQL_IDNAME ") " /* */
           "    WHERE "         /* */
           " fn.Pathid=:dirID " /* */
           " ORDER BY fn." DUF_SQL_IDNAME " " /* */
	   ,
           .selector_total2 =   /* */
           " FROM " DUF_DBPREF DUF_SELECTED_NAME " AS fns " /* */
           " JOIN " DUF_DBPREF "filenames AS fn ON (fns." DUF_SQL_IDNAME "=fn." DUF_SQL_IDNAME ")" /* */
           " LEFT JOIN " DUF_DBPREF "filedatas AS fd ON (fn.dataid=fd." DUF_SQL_IDNAME ") " /* */
           " LEFT JOIN " DUF_DBPREF "md5 AS md ON (md." DUF_SQL_IDNAME "=fd.md5id)" /* */
           },
  .node = {.fieldset = "pt." DUF_SQL_IDNAME " AS dirid, pt.dirname, pt.dirname AS dfname,  pt.parentid " /* */
           ", tf.numfiles AS nfiles, td.numdirs AS ndirs, tf.maxsize AS maxsize, tf.minsize AS minsize" /* */
           ,
/* .node_selector = "SELECT     pt." DUF_SQL_IDNAME " AS dirid, pt.dirname, pt.dirname AS dfname,  pt.parentid " (* *)                */
/*       ", tf.numfiles AS nfiles, td.numdirs AS ndirs, tf.maxsize AS maxsize, tf.minsize AS minsize " (* *)                          */
/*       (* " ,(SELECT count(*) FROM " DUF_DBPREF "paths AS subpaths WHERE subpaths.parentid=pt." DUF_SQL_IDNAME ") AS ndirs "    *)  */
/*       (* " ,(SELECT count(*) FROM " DUF_DBPREF DUF_SELECTED_NAME " AS sfn "                                                    *)  */
/*       (* "          JOIN " DUF_DBPREF "filedatas AS sfd ON (sfn.dataid=sfd." DUF_SQL_IDNAME ") "                               *)  */
/*       (* "          JOIN " DUF_DBPREF "md5 AS smd ON (sfd.md5id=smd." DUF_SQL_IDNAME ") "                                      *)  */
/*       (* "          WHERE sfn.Pathid=pt." DUF_SQL_IDNAME " "                                                            *)         */
/*       (* "              AND   sfd.size >= %llu AND sfd.size < %llu "                                           *)                  */
/*       (* "              AND (smd.dup5cnt IS NULL OR (smd.dup5cnt >= %llu AND smd.dup5cnt < %llu)) "               *)               */
/*       (* " ) AS nfiles "                                                                                       *)                  */
/* (* " ,(SELECT min(sfd.size) FROM " DUF_DBPREF "filedatas AS sfd JOIN " DUF_DBPREF DUF_SELECTED_NAME " AS sfn ON (sfn.dataid=sfd." *) */
/* (* DUF_SQL_IDNAME ") "                                                                                                            *) */
/*       (* "           WHERE sfn.Pathid=pt." DUF_SQL_IDNAME ") AS minsize "                                               *)                            */
/* (* " ,(SELECT max(sfd.size) FROM " DUF_DBPREF "filedatas AS sfd JOIN " DUF_DBPREF DUF_SELECTED_NAME " AS sfn ON (sfn.dataid=sfd." *) */
/* (* DUF_SQL_IDNAME ") "                                                                                                            *) */
/*       (* "           WHERE sfn.Pathid=pt." DUF_SQL_IDNAME ") AS maxsize "                                               *)         */
/*       " FROM      " DUF_DBPREF "paths AS pt " (* *)                                                                                */
/*       " LEFT JOIN " DUF_DBPREF "pathtot_dirs  AS td ON (td.Pathid=pt." DUF_SQL_IDNAME ") " (* *)                                   */
/*       " LEFT JOIN " DUF_DBPREF "pathtot_files AS tf ON (tf.Pathid=pt." DUF_SQL_IDNAME ") " (* *)                                   */
/*       " WHERE pt.parentid='%llu' ",                                                                                                */
           .selector2 =         /* */
           /* "SELECT     pt." DUF_SQL_IDNAME " AS dirid, pt.dirname, pt.dirname AS dfname,  pt.parentid "                  */
           /* ", tf.numfiles AS nfiles, td.numdirs AS ndirs, tf.maxsize AS maxsize, tf.minsize AS minsize " */
           " FROM      " DUF_DBPREF "paths AS pt " /* */
           " LEFT JOIN " DUF_DBPREF "pathtot_dirs AS td ON (td.Pathid=pt." DUF_SQL_IDNAME ") " /* */
           " LEFT JOIN " DUF_DBPREF "pathtot_files AS tf ON (tf.Pathid=pt." DUF_SQL_IDNAME ") " /* */
           " WHERE pt.parentid=:dirID " /* */
           ,
           }
  /* .final_sql_argv = final_sql, */
};
