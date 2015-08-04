#ifndef MAS_DUF_OPTIONS_DEF_H
#  define MAS_DUF_OPTIONS_DEF_H

#  define DUF_CONFIG_PATH_FROM_ENV "MSHDIRS_CONF_DIR"
#  define DUF_CONFIG_FILE_NAME "zocromas_duf.conf"

/* #  define DUF_DECLARE_OPTION(_name)                                         */
/* #  define DUF_DECLARE_OPTION_V(_name, _val) DUF_OPTION_VAL_ ## _name = _val */
typedef enum
{
  DUF_OPTION_VAL_NONE,

  DUF_OPTION_VAL_SMART_HELP = 'h',
  DUF_OPTION_VAL_HELP = 'H',
  DUF_OPTION_VAL_EXAMPLES = 'x',
  DUF_OPTION_VAL_TEST = 't',
  DUF_OPTION_VAL_VERBOSE = 'v',
  DUF_OPTION_VAL_DEBUG = 'g',
  DUF_OPTION_VAL_VERSION = 'V',
  DUF_OPTION_VAL_MEMUSAGE = 'M',
/* db general */
  DUF_OPTION_VAL_DB_DIRECTORY = 'Y',
  DUF_OPTION_VAL_FLAG_CREATE_TABLES = 'O',
  DUF_OPTION_VAL_FLAG_INTEGRITY = 'G',
  DUF_OPTION_VAL_FLAG_DRY_RUN = 'z',

  /* trace */
  DUF_OPTION_VAL_ACTION_TRACE = 'A',
  DUF_OPTION_VAL_SCAN_TRACE = 'S',
  DUF_OPTION_VAL_TEMP_TRACE = '$',
  DUF_OPTION_VAL_COLLECT_TRACE = 'I',
  DUF_OPTION_VAL_ANY_TRACE = 'y',
  DUF_OPTION_VAL_MATCH_TRACE = 'm',
  /* /trace */

  DUF_OPTION_VAL_FLAG_RECURSIVE = 'R',
  /* DUF_OPTION_VAL_TREE_TO_DB = 'B', */
  DUF_OPTION_VAL_FLAG_ADD_PATH = 'P',

  /* actions */
#  if 0
  DUF_OPTION_VAL_FLAG_TREE = 'T',
  DUF_OPTION_VAL_FLAG_PRINT = 'p',
#  else
  DUF_OPTION_VAL_TREE = 'T',    /* new implementation 20150518.083230 */
  DUF_OPTION_VAL_LSFILES = 'p', /* new implementation 20150518.083230 */
#  endif
  DUF_OPTION_VAL_FLAG_SUMMARY = 's',
  DUF_OPTION_VAL_FLAG_FAST = 'F',

  /* actions */
  DUF_OPTION_VAL_FLAG_COLLECT = 'i',
  DUF_OPTION_VAL_FLAG_FILES = 'f',
  DUF_OPTION_VAL_FLAG_DIRS = 'd',
  DUF_OPTION_VAL_FLAG_DIRENT = 'E',

  DUF_OPTION_VAL_FLAG_FILEDATA = 'D',
  DUF_OPTION_VAL_FLAG_FILENAMES = 'n',
  DUF_OPTION_VAL_FLAG_SD5 = '2',
  DUF_OPTION_VAL_FLAG_CRC32 = '3',
  DUF_OPTION_VAL_FLAG_MD5 = '5',
  DUF_OPTION_VAL_FLAG_MIME = 'e',
  DUF_OPTION_VAL_FLAG_EXIF = 'X',


/*==================================================*/
  DUF_OPTION_VAL_LONG = 1000,
/*==================================================*/
  DUF_OPTION_VAL_HELP_NONE,
  DUF_OPTION_VAL_HELP_ALL,

  DUF_OPTION_VAL_HELP_HELP,
  DUF_OPTION_VAL_HELP_SYSTEM,
  DUF_OPTION_VAL_HELP_CONTROL,
  DUF_OPTION_VAL_HELP_REFERENCE,
  DUF_OPTION_VAL_HELP_COLLECT,
  DUF_OPTION_VAL_HELP_FILTER,
  DUF_OPTION_VAL_HELP_SCAN,
  DUF_OPTION_VAL_HELP_UPDATE,
  DUF_OPTION_VAL_HELP_REQUEST,
  DUF_OPTION_VAL_HELP_PRINT,

  DUF_OPTION_VAL_HELP_TRACE,
  DUF_OPTION_VAL_HELP_DEBUG,
  DUF_OPTION_VAL_HELP_OBSOLETE,
  DUF_OPTION_VAL_HELP_NODESC,
  DUF_OPTION_VAL_HELP_OTHER,

  DUF_OPTION_VAL_LIST_OPTIONS,

  DUF_OPTION_VAL_SCAN_DIR_TRACE,
  DUF_OPTION_VAL_SCAN_REG_TRACE,


/* db general */
  DUF_OPTION_VAL_DB_NAME_MAIN,
  DUF_OPTION_VAL_DB_NAME_ADM,
  DUF_OPTION_VAL_DB_OPEN,

  /* DUF_OPTION_VAL_FLAG_ZERO_DB, */
  DUF_OPTION_VAL_FLAG_REMOVE_DATABASE,
  DUF_OPTION_VAL_FLAG_DROP_TABLES,
  DUF_OPTION_VAL_FLAG_VACUUM,

  DUF_OPTION_VAL_FLAG_INTERACTIVE,
  DUF_OPTION_VAL_FLAG_PROGRESS,
  DUF_OPTION_VAL_FLAG_BEGINNING_SQL,

  /* i/o */
  DUF_OPTION_VAL_OUTPUT_LEVEL,
  DUF_OPTION_VAL_OUTPUT_FILE,
  DUF_OPTION_VAL_OUTPUT_STDERR,
  DUF_OPTION_VAL_OUTPUT_STDOUT,
  DUF_OPTION_VAL_SFORMAT_DIRS_TREE,
  DUF_OPTION_VAL_SFORMAT_FILES_TREE,
  DUF_OPTION_VAL_SFORMAT_PREFIX_GEN_TREE,
  DUF_OPTION_VAL_SFORMAT_PREFIX_FILES_TREE,
  DUF_OPTION_VAL_SFORMAT_PREFIX_DIRS_TREE,
  DUF_OPTION_VAL_SFORMAT_DIRS_LIST,
  DUF_OPTION_VAL_SFORMAT_FILES_LIST,
  DUF_OPTION_VAL_SFORMAT_DIRS_GEN,
  DUF_OPTION_VAL_SFORMAT_FILES_GEN,
  DUF_OPTION_VAL_BFORMAT,
  DUF_OPTION_VAL_FLAG_USE_BINFORMAT,

  /* execution control */
  DUF_OPTION_VAL_FLAG_DISABLE_CALCULATE,
  DUF_OPTION_VAL_FLAG_DISABLE_INSERT,
  DUF_OPTION_VAL_FLAG_DISABLE_UPDATE,

  /* trace */
  DUF_OPTION_VAL_DRY_RUN_TRACE,
  DUF_OPTION_VAL_EXPLAIN_TRACE,

  DUF_OPTION_VAL_ERROR_TRACE,
  DUF_OPTION_VAL_TEMPORARY_TRACE,
  DUF_OPTION_VAL_SEQ_TRACE,
  DUF_OPTION_VAL_OPTIONS_TRACE,
  DUF_OPTION_VAL_CONFIG_TRACE,
  DUF_OPTION_VAL_CALLS_TRACE,
  DUF_OPTION_VAL_DEPTH_TRACE,
  DUF_OPTION_VAL_DELETED_TRACE,
  DUF_OPTION_VAL_DIRENT_TRACE,
  DUF_OPTION_VAL_FS_TRACE,
  DUF_OPTION_VAL_INTEGRITY_TRACE,
  DUF_OPTION_VAL_SD5_TRACE,
  DUF_OPTION_VAL_MD5_TRACE,
  DUF_OPTION_VAL_CRC32_TRACE,
  DUF_OPTION_VAL_MIME_TRACE,
  DUF_OPTION_VAL_EXIF_TRACE,
  DUF_OPTION_VAL_MDPATH_TRACE,
  DUF_OPTION_VAL_PATH_TRACE,
  DUF_OPTION_VAL_MOD_TRACE,
  DUF_OPTION_VAL_SAMPLE_TRACE,
  DUF_OPTION_VAL_SAMPUPD_TRACE,
  DUF_OPTION_VAL_SQL_TRACE,
  DUF_OPTION_VAL_SQLITE_TRACE,
  DUF_OPTION_VAL_SELECT_TRACE,
  DUF_OPTION_VAL_INSERT_TRACE,
  DUF_OPTION_VAL_UPDATE_TRACE,
  DUF_OPTION_VAL_IO_TRACE,
  DUF_OPTION_VAL_ALL_TRACE,

  DUF_OPTION_VAL_TRACE_NONEW,

  /* trace i/o */
  DUF_OPTION_VAL_TRACE_FILE,
  DUF_OPTION_VAL_TRACE_STDERR,
  DUF_OPTION_VAL_TRACE_STDOUT,

  /* debug */
  DUF_OPTION_VAL_MIN_DBGLINE,
  DUF_OPTION_VAL_MAX_DBGLINE,

  DUF_OPTION_VAL_FLAG_EXPLAIN,

  /* actions */
  DUF_OPTION_VAL_SAMPLE,
  DUF_OPTION_VAL_SAMPUPD,
  DUF_OPTION_VAL_FLAG_MDPATH,
/* /actions */

  /* DUF_OPTION_VAL_UPDATE_PATH, */
  /* DUF_OPTION_VAL_UPDATE_MD5, */
  /* DUF_OPTION_VAL_ZERO_DUPLICATES, */
  /* DUF_OPTION_VAL_UPDATE_DUPLICATES, */
  /* DUF_OPTION_VAL_UPDATE_MDPATH, */
  /* DUF_OPTION_VAL_UPDATE_MDPATH_SELECTIVE, */
  /* DUF_OPTION_VAL_UPDATE_FILEDATA, */
  /* DUF_OPTION_VAL_ZERO_FILEDATA, */
  /* DUF_OPTION_VAL_UPDATE_EXIF, */

  /* filters */
  DUF_OPTION_VAL_MINDIRFILES,
  DUF_OPTION_VAL_MAXDIRFILES,
  DUF_OPTION_VAL_GLOB_INCL_FS_FILES,
  DUF_OPTION_VAL_GLOB_EXCL_FS_FILES,

  DUF_OPTION_VAL_NAMEID,
  DUF_OPTION_VAL_MINNAMEID,
  DUF_OPTION_VAL_MAXNAMEID,

  DUF_OPTION_VAL_DIRID,
  DUF_OPTION_VAL_MINDIRID,
  DUF_OPTION_VAL_MAXDIRID,

  DUF_OPTION_VAL_MTIME,
  DUF_OPTION_VAL_MINMTIME,
  DUF_OPTION_VAL_MAXMTIME,

  DUF_OPTION_VAL_ATIME,
  DUF_OPTION_VAL_MINATIME,
  DUF_OPTION_VAL_MAXATIME,

  DUF_OPTION_VAL_CTIME,
  DUF_OPTION_VAL_MINCTIME,
  DUF_OPTION_VAL_MAXCTIME,

  DUF_OPTION_VAL_EXIFDT,
  DUF_OPTION_VAL_MINEXIFDT,
  DUF_OPTION_VAL_MAXEXIFDT,

  DUF_OPTION_VAL_INODEID,
  DUF_OPTION_VAL_MININODEID,
  DUF_OPTION_VAL_MAXINODEID,

  DUF_OPTION_VAL_SD5ID,
  DUF_OPTION_VAL_MINSD5ID,
  DUF_OPTION_VAL_MAXSD5ID,

  DUF_OPTION_VAL_MD5ID,
  DUF_OPTION_VAL_MINMD5ID,
  DUF_OPTION_VAL_MAXMD5ID,

  DUF_OPTION_VAL_CRC32ID,
  DUF_OPTION_VAL_MINCRC32ID,
  DUF_OPTION_VAL_MAXCRC32ID,

  DUF_OPTION_VAL_MIMEID,
  DUF_OPTION_VAL_MINMIMEID,
  DUF_OPTION_VAL_MAXMIMEID,

  DUF_OPTION_VAL_EXIFID,
  DUF_OPTION_VAL_MINEXIFID,
  DUF_OPTION_VAL_MAXEXIFID,

  DUF_OPTION_VAL_SIZE,
  DUF_OPTION_VAL_MINSIZE,
  DUF_OPTION_VAL_MAXSIZE,

  DUF_OPTION_VAL_SAME,
  DUF_OPTION_VAL_MINSAME,
  DUF_OPTION_VAL_MAXSAME,

  DUF_OPTION_VAL_EXIFSAME,
  DUF_OPTION_VAL_MINEXIFSAME,
  DUF_OPTION_VAL_MAXEXIFSAME,

  DUF_OPTION_VAL_MIMESAME,
  DUF_OPTION_VAL_MINMIMESAME,
  DUF_OPTION_VAL_MAXMIMESAME,

  DUF_OPTION_VAL_GLOB_DB,
  DUF_OPTION_VAL_GLOB_INCL_DB_FILES,
  DUF_OPTION_VAL_GLOB_EXCL_DB_FILES,
  DUF_OPTION_VAL_SAME_AS,
  /* DUF_OPTION_VAL_CD, */
  /* DUF_OPTION_VAL_SCCB, */
  DUF_OPTION_VAL_LIST_SCCBS,
  DUF_OPTION_VAL_EVALUATE_SCCB,

  DUF_OPTION_VAL_MAXRELDEPTH,
  DUF_OPTION_VAL_MAXSEQ,
  DUF_OPTION_VAL_MAXITEMS,
  DUF_OPTION_VAL_MAXITEMS_FILES,
  DUF_OPTION_VAL_MAXITEMS_DIRS,
  /* /filters */

  DUF_OPTION_VAL_FLAG_UNI_SCAN,
  DUF_OPTION_VAL_FLAG_DO_SCCBS,
#  if 0
  DUF_OPTION_VAL_FLAG_SHOW_SCCBS,
  DUF_OPTION_VAL_FLAG_SHOW_SCCBH,
#  endif
  DUF_OPTION_VAL_FLAG_INFO,

  DUF_OPTION_VAL_CD,
  DUF_OPTION_VAL_QUIT,
  DUF_OPTION_VAL_PWD,


  DUF_OPTION_VAL_ADD_TARG,
  DUF_OPTION_VAL_LIST_TARG,
  DUF_OPTION_VAL_CLEAR_TARG,
  /* DUF_OPTION_VAL_PRINT_PATHS, */
  /* DUF_OPTION_VAL_PRINT_DIRS, */
  /* DUF_OPTION_VAL_PRINT_FILES, */
  /* DUF_OPTION_VAL_PRINT_DUPLICATES, */
  /* DUF_OPTION_VAL_SAME_FILES, */
  /* DUF_OPTION_VAL_SAME_EXIF, */
  /* DUF_OPTION_VAL_SAME_MD5, */
  /* DUF_OPTION_VAL_GROUP, */
  /* DUF_OPTION_VAL_LIMIT, */
  /* DUF_OPTION_VAL_ADD_TO_GROUP, */
  /* DUF_OPTION_VAL_REMOVE_FROM_GROUP, */



  DUF_OPTION_VAL_ANY_OTHER,
  DUF_OPTION_VAL_MAX_LONG
} duf_option_code_t;

#endif

/*
vi: ft=c
*/
