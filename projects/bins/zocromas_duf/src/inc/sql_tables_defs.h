#ifndef MAS_SQL_TABLES_DEFS_H
#  define MAS_SQL_TABLES_DEFS_H

#  include "duf_defs.h"
#  include "duf_db_defs.h"
#  include "sql_tables_global_defs.h"

#  ifndef DUF_SQL_SELECTED_TEMPORARY
#    define DUF_SQL_SELECTED_DROP
#  else
#    define DUF_SQL_SELECTED_DROP
#  endif

#  define DUF_ATTACH_COMMON_PATTERN "${DB_PATH}${DB_NAME}_$_"

#  ifndef DUF_SQL_TTABLES_TEMPORARY
#    define DUF_ATTACH_TTABLES_PATTERN DUF_ATTACH_COMMON_PATTERN
#  endif

#  ifndef DUF_SQL_SELECTED_TEMPORARY
#    define DUF_ATTACH_SELECTED_PATTERN DUF_ATTACH_COMMON_PATTERN "${PDI_NAME}_selected.db"
#  endif

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

#  define DUF_SQL_TABLES_TMP_TDB_OPTIONS       "t_tdb_options"

#  if 1

#    define DUF_SQL_TABLES_FILENAMES_FULL          DUF_DBPREF  DUF_SQL_TABLES_FILENAMES
#    define DUF_SQL_TABLES_FILEDATAS_FULL          DUF_DBPREF  DUF_SQL_TABLES_FILEDATAS
#    define DUF_SQL_TABLES_MD5_FULL                DUF_DBPREF  DUF_SQL_TABLES_MD5
#    define DUF_SQL_TABLES_SHA1_FULL               DUF_DBPREF  DUF_SQL_TABLES_SHA1
#    define DUF_SQL_TABLES_SD5_FULL                DUF_DBPREF  DUF_SQL_TABLES_SD5
#    define DUF_SQL_TABLES_CRC32_FULL              DUF_DBPREF  DUF_SQL_TABLES_CRC32
#    define DUF_SQL_TABLES_SIZES_FULL              DUF_DBPREF  DUF_SQL_TABLES_SIZES
#    define DUF_SQL_TABLES_EXIF_FULL               DUF_DBPREF  DUF_SQL_TABLES_EXIF
#    define DUF_SQL_TABLES_EXIF_MODEL_FULL         DUF_DBPREF  DUF_SQL_TABLES_EXIF_MODEL
#    define DUF_SQL_TABLES_MIME_FULL               DUF_DBPREF  DUF_SQL_TABLES_MIME
#    define DUF_SQL_TABLES_PATHS_FULL              DUF_DBPREF  DUF_SQL_TABLES_PATHS
#    define DUF_SQL_TABLES_TAGS_FULL               DUF_DBPREF  DUF_SQL_TABLES_TAGS
#    define DUF_SQL_TABLES_TAGNAMES_FULL           DUF_DBPREF  DUF_SQL_TABLES_TAGNAMES

#    ifdef DUF_SQL_TTABLES_TEMPORARY
#      define DUF_SQL_TABLES_TMP_TDB_OPTIONS_FULL     DUF_SQL_TABLES_TMP_TDB_OPTIONS
#    else
#      define DUF_SQL_TABLES_TMP_TDB_OPTIONS_FULL    DUF_DBTEMPPREF  DUF_SQL_TABLES_TMP_TDB_OPTIONS
#    endif
#  else
#    define DUF_SQL_TABLES_FILENAMES_FULL            DUF_SQL_TABLES_FILENAMES
#    define DUF_SQL_TABLES_FILEDATAS_FULL            DUF_SQL_TABLES_FILEDATAS
#    define DUF_SQL_TABLES_MD5_FULL                  DUF_SQL_TABLES_MD5
#    define DUF_SQL_TABLES_SHA1_FULL                 DUF_SQL_TABLES_SHA1
#    define DUF_SQL_TABLES_SD5_FULL                  DUF_SQL_TABLES_SD5
#    define DUF_SQL_TABLES_CRC32_FULL                DUF_SQL_TABLES_CRC32
#    define DUF_SQL_TABLES_SIZES_FULL                DUF_SQL_TABLES_SIZES
#    define DUF_SQL_TABLES_EXIF_FULL                 DUF_SQL_TABLES_EXIF
#    define DUF_SQL_TABLES_EXIF_MODEL_FULL           DUF_SQL_TABLES_EXIF_MODEL
#    define DUF_SQL_TABLES_MIME_FULL                 DUF_SQL_TABLES_MIME
#    define DUF_SQL_TABLES_PATHS_FULL                DUF_SQL_TABLES_PATHS
#    define DUF_SQL_TABLES_TAGS_FULL                 DUF_SQL_TABLES_TAGS
#    define DUF_SQL_TABLES_TAGNAMES_FULL             DUF_SQL_TABLES_TAGNAMES
#    define DUF_SQL_TABLES_TMP_PATHTOT_FILES_FULL    DUF_SQL_TABLES_TMP_PATHTOT_FILES
#    define DUF_SQL_TABLES_TMP_PATHTOT_DIRS_FULL     DUF_SQL_TABLES_TMP_PATHTOT_DIRS
#  endif

#endif
