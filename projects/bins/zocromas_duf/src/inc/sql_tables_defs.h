#ifndef SQL_TABLES_DEFS_H
#  define SQL_TABLES_DEFS_H




#  define DUF_SQL_SELECTED_TEMPORARY "TEMPORARY"
#  define DUF_SQL_TTABLES_TEMPORARY "TEMPORARY"
/* #  define DUF_SQL_TABLES_TEMPORARY "TEMPORARY" */

#ifndef DUF_SQL_SELECTED_TEMPORARY
#  define DUF_ATTACH_SELECTED_PATTERN "${DB_PATH}${DB_NAME}_$_${PDI_NAME}.db"
#endif

#  ifdef DUF_SQL_SELECTED_TEMPORARY
#    define DUF_SQL_SELECTED_TEMPORARY_STRING DUF_SQL_SELECTED_TEMPORARY
#  else
#    define DUF_SQL_SELECTED_TEMPORARY_STRING
#  endif

/* #  ifdef DUF_SQL_TABLES_TEMPORARY                                    */
/* #    define DUF_SQL_TABLES_TEMPORARY_STRING DUF_SQL_TABLES_TEMPORARY */
/* #  else                                                              */
/* #    define DUF_SQL_TABLES_TEMPORARY_STRING                          */
/* #  endif                                                             */

#  ifdef DUF_SQL_TTABLES_TEMPORARY
#    define DUF_SQL_TTABLES_TEMPORARY_STRING DUF_SQL_TTABLES_TEMPORARY
#  else
#    define DUF_SQL_TTABLES_TEMPORARY_STRING
#  endif

#  define DUF_SQL_TABLES_TMP_TDB_OPTIONS "t_tdb_options"
#  define DUF_SQL_TABLES_TMP_PATHTOT_FILES "t_common_pathtot_files"
#  define DUF_SQL_TABLES_TMP_PATHTOT_DIRS "t_common_pathtot_dirs"

#  define DUF_SQL_TABLES_FILENAMES  "filenames"
#  define DUF_SQL_TABLES_FILEDATAS  "filedatas"
#  define DUF_SQL_TABLES_MD5  "md5"
#  define DUF_SQL_TABLES_SD5  "sd5"
#  define DUF_SQL_TABLES_CRC32  "crc32"
#  define DUF_SQL_TABLES_SIZES  "sizes"
#  define DUF_SQL_TABLES_EXIF  "exif"
#  define DUF_SQL_TABLES_EXIF_MODEL  "exif_model"
#  define DUF_SQL_TABLES_MIME  "mime"
#  define DUF_SQL_TABLES_PATHS "paths"

#  if 1

#    define DUF_SQL_TABLES_FILENAMES_FULL          DUF_DBPREF  DUF_SQL_TABLES_FILENAMES
#    define DUF_SQL_TABLES_FILEDATAS_FULL          DUF_DBPREF  DUF_SQL_TABLES_FILEDATAS
#    define DUF_SQL_TABLES_MD5_FULL                DUF_DBPREF  DUF_SQL_TABLES_MD5
#    define DUF_SQL_TABLES_SD5_FULL                DUF_DBPREF  DUF_SQL_TABLES_SD5
#    define DUF_SQL_TABLES_CRC32_FULL              DUF_DBPREF  DUF_SQL_TABLES_CRC32
#    define DUF_SQL_TABLES_SIZES_FULL              DUF_DBPREF  DUF_SQL_TABLES_SIZES
#    define DUF_SQL_TABLES_EXIF_FULL               DUF_DBPREF  DUF_SQL_TABLES_EXIF
#    define DUF_SQL_TABLES_EXIF_MODEL_FULL         DUF_DBPREF  DUF_SQL_TABLES_EXIF_MODEL
#    define DUF_SQL_TABLES_MIME_FULL               DUF_DBPREF  DUF_SQL_TABLES_MIME
#    define DUF_SQL_TABLES_PATHS_FULL              DUF_DBPREF  DUF_SQL_TABLES_PATHS

#    ifdef DUF_SQL_TTABLES_TEMPORARY
#      define DUF_SQL_TABLES_TMP_TDB_OPTIONS_FULL     DUF_SQL_TABLES_TMP_TDB_OPTIONS
#      define DUF_SQL_TABLES_TMP_PATHTOT_FILES_FULL   DUF_SQL_TABLES_TMP_PATHTOT_FILES
#      define DUF_SQL_TABLES_TMP_PATHTOT_DIRS_FULL    DUF_SQL_TABLES_TMP_PATHTOT_DIRS
#    else
#      define DUF_SQL_TABLES_TMP_TDB_OPTIONS_FULL    DUF_DBTEMPPREF  DUF_SQL_TABLES_TMP_TDB_OPTIONS
#      define DUF_SQL_TABLES_TMP_PATHTOT_FILES_FULL  DUF_DBTEMPPREF  DUF_SQL_TABLES_TMP_PATHTOT_FILES
#      define DUF_SQL_TABLES_TMP_PATHTOT_DIRS_FULL   DUF_DBTEMPPREF  DUF_SQL_TABLES_TMP_PATHTOT_DIRS
#    endif
#  else
#    define DUF_SQL_TABLES_FILENAMES_FULL            DUF_SQL_TABLES_FILENAMES
#    define DUF_SQL_TABLES_FILEDATAS_FULL            DUF_SQL_TABLES_FILEDATAS
#    define DUF_SQL_TABLES_MD5_FULL                  DUF_SQL_TABLES_MD5
#    define DUF_SQL_TABLES_SD5_FULL                  DUF_SQL_TABLES_SD5
#    define DUF_SQL_TABLES_CRC32_FULL                DUF_SQL_TABLES_CRC32
#    define DUF_SQL_TABLES_SIZES_FULL                DUF_SQL_TABLES_SIZES
#    define DUF_SQL_TABLES_EXIF_FULL                 DUF_SQL_TABLES_EXIF
#    define DUF_SQL_TABLES_EXIF_MODEL_FULL           DUF_SQL_TABLES_EXIF_MODEL
#    define DUF_SQL_TABLES_MIME_FULL                 DUF_SQL_TABLES_MIME
#    define DUF_SQL_TABLES_PATHS_FULL                DUF_SQL_TABLES_PATHS
#    define DUF_SQL_TABLES_TMP_PATHTOT_FILES_FULL    DUF_SQL_TABLES_TMP_PATHTOT_FILES
#    define DUF_SQL_TABLES_TMP_PATHTOT_DIRS_FULL     DUF_SQL_TABLES_TMP_PATHTOT_DIRS
#  endif

#endif
