#ifndef MAS_DUF_CLI_TYPES_H
#  define MAS_DUF_CLI_TYPES_H

#  define DUF_OPTIONS_SHORT "hxD:N:RvAeSIQTVCfd5iMtZBPOG"
typedef enum
{
  DUF_OPTION_NONE,
  DUF_OPTION_HELP = 'h',
  DUF_OPTION_HELP_EXAMPLES = 'x',
  DUF_OPTION_DB_DIRECTORY = 'D',
  DUF_OPTION_DB_NAME = 'N',
  DUF_OPTION_RECURSIVE = 'R',
  DUF_OPTION_VERBOSE = 'v',

  DUF_OPTION_ACTION_TRACE = 'A',
  DUF_OPTION_ERROR_TRACE = 'e',
  DUF_OPTION_SCAN_TRACE = 'S',
  DUF_OPTION_FILL_TRACE = 'I',
  DUF_OPTION_SQL_TRACE = 'Q',
  DUF_OPTION_ANY_TRACE = 'Y',
  DUF_OPTION_CURRENT_TRACE = 'C',
  DUF_OPTION_ALL_TRACE = 'T',

  DUF_OPTION_VACUUM = 'V',
  DUF_OPTION_FILES = 'f',
  DUF_OPTION_DIRS = 'd',
  DUF_OPTION_MD5 = '5',
  DUF_OPTION_FILL = 'i',
  DUF_OPTION_MEMUSAGE = 'M',
  DUF_OPTION_TOTALS = 't',
  DUF_OPTION_ZERO_DB = 'Z',
  DUF_OPTION_TREE_TO_DB = 'B',
  DUF_OPTION_ADD_PATH = 'P',
  DUF_OPTION_CREATE_TABLES = 'O',
  DUF_OPTION_INTEGRITY = 'G',

  DUF_OPTION_LONG = 1000,

  DUF_OPTION_FORMAT,

  DUF_OPTION_INTEGRITY_TRACE,
  DUF_OPTION_FS_TRACE,
  DUF_OPTION_SAMPLE_TRACE,
  DUF_OPTION_CALLS_TRACE,
  DUF_OPTION_SAMPUPD_TRACE,
  DUF_OPTION_PATH_TRACE,
  DUF_OPTION_MD5_TRACE,
  DUF_OPTION_MDPATH_TRACE,
  DUF_OPTION_TRACE_NONEW,
  DUF_OPTION_TRACE_FILE,
  DUF_OPTION_TRACE_STDERR,
  DUF_OPTION_TRACE_STDOUT,

  DUF_OPTION_DEBUG,
  DUF_OPTION_MIN_DBGLINE,
  DUF_OPTION_MAX_DBGLINE,

  DUF_OPTION_NOOPENAT,


  DUF_OPTION_REMOVE_DATABASE,
  DUF_OPTION_DROP_TABLES,
  DUF_OPTION_MDPATH,
  /* DUF_OPTION_UPDATE_PATH, */
  /* DUF_OPTION_UPDATE_MD5, */
  /* DUF_OPTION_ZERO_DUPLICATES, */
  /* DUF_OPTION_UPDATE_DUPLICATES, */
  /* DUF_OPTION_UPDATE_MDPATH, */
  /* DUF_OPTION_UPDATE_MDPATH_SELECTIVE, */
  DUF_OPTION_UPDATE_FILEDATA,
  DUF_OPTION_ZERO_FILEDATA,
  DUF_OPTION_UPDATE_EXIF,
  DUF_OPTION_MINDIRFILES,
  DUF_OPTION_MAXDIRFILES,
  DUF_OPTION_MINSIZE,
  DUF_OPTION_MAXSIZE,
  DUF_OPTION_MINSAME,
  DUF_OPTION_MAXSAME,
  DUF_OPTION_MAXDEPTH,
  DUF_OPTION_MAXSEQ,
  DUF_OPTION_MAXITEMS,
  DUF_OPTION_MAXITEMS_FILES,
  DUF_OPTION_MAXITEMS_DIRS,
  DUF_OPTION_SAMPLE,
  DUF_OPTION_SAMPUPD,
  DUF_OPTION_PRINT,
  DUF_OPTION_TREE,
  DUF_OPTION_UNI_SCAN,
  /* DUF_OPTION_PRINT_PATHS, */
  /* DUF_OPTION_PRINT_DIRS, */
  /* DUF_OPTION_PRINT_FILES, */
  /* DUF_OPTION_PRINT_DUPLICATES, */
  DUF_OPTION_SAME_FILES,
  DUF_OPTION_SAME_EXIF,
  DUF_OPTION_SAME_MD5,
  DUF_OPTION_GROUP,
  /* DUF_OPTION_LIMIT, */
  DUF_OPTION_ADD_TO_GROUP,
  DUF_OPTION_REMOVE_FROM_GROUP,
  DUF_OPTION_MAX_LONG,
} duf_option_code_t;

typedef enum
{
  DUF_TRACE_MODE_NONE,
  DUF_TRACE_MODE_error,
  DUF_TRACE_MODE_errorr,
  DUF_TRACE_MODE_any,
  DUF_TRACE_MODE_verbose,
  DUF_TRACE_MODE_action,
  DUF_TRACE_MODE_current,
  DUF_TRACE_MODE_MAX,
  DUF_TRACE_MODE_fill,
  DUF_TRACE_MODE_path,
  DUF_TRACE_MODE_scan,
  DUF_TRACE_MODE_fs,
  DUF_TRACE_MODE_sample,
  DUF_TRACE_MODE_sampupd,
  DUF_TRACE_MODE_statistics,
  DUF_TRACE_MODE_mdpath,
  DUF_TRACE_MODE_md5,
  DUF_TRACE_MODE_sql,
} duf_trace_mode_t;

typedef enum
{
  DUF_FORMAT_SEQ,
  DUF_FORMAT_DIRID,
  DUF_FORMAT_NSAME,
  DUF_FORMAT_INODE,
  DUF_FORMAT_MODE,
  DUF_FORMAT_NLINK,
  DUF_FORMAT_UID,
  DUF_FORMAT_GID,
  DUF_FORMAT_FILESIZE,
  DUF_FORMAT_MTIME,
  DUF_FORMAT_FILENAME,
  DUF_FORMAT_MD5,
  DUF_FORMAT_OFFSET,
  DUF_FORMAT_MAX,
} duf_format_index_t;
typedef struct
{
  unsigned seq:1;
  unsigned dirid:1;
  unsigned inode:1;
  unsigned mode:1;
  unsigned nlink:1;
  unsigned uid:1;
  unsigned gid:1;
  unsigned filesize:1;
  unsigned filename:1;
  unsigned md5:1;
  unsigned mtime:1;
  unsigned nsame;
  unsigned offset;
} duf_format_t;

typedef struct
{
  unsigned remove_database:1;
  unsigned drop_tables:1;
  unsigned create_tables:1;
  unsigned add_path:1;
  /* unsigned update_path:1; */
  /* unsigned update_md5:1; */
  unsigned update_duplicates:1;
  /* unsigned update_mdpath:1; */
  /* unsigned update_mdpath_selective:1; */
  unsigned update_filedata:1;
  unsigned zero_filedata:1;
  unsigned update_exif:1;
  unsigned mdpath:1;
  unsigned vacuum:1;
  unsigned md5:1;
  unsigned integrity:1;
  unsigned fill:1;
  unsigned print:1;
  unsigned tree:1;
  unsigned files:1;
  unsigned dirs:1;
  unsigned uni_scan:1;
  /* unsigned print_paths:1; */
  /* unsigned print_dirs:1; */
  /* unsigned print_files:1; */
  unsigned print_duplicates:1;
  /* (* unsigned zero_duplicates:1; *) */
  unsigned same_files:1;
  unsigned same_md5:1;
  unsigned same_exif:1;
  unsigned to_group:1;
  unsigned from_group:1;
  unsigned totals;
  unsigned sample;
  unsigned sampupd;
} duf_config_cli_actions_t;

typedef struct
{
  unsigned debug:1;
  unsigned nosqlite;
  unsigned verbose;
  unsigned long min_line;
  unsigned long max_line;
  unsigned long lines;
} duf_config_cli_debug_t;

typedef struct
{
  unsigned error;
  unsigned errorr;
  unsigned any;
  unsigned fs;
  unsigned current;
  unsigned calls;
  unsigned action;
  unsigned scan;
  unsigned sample;
  unsigned sampupd;
  unsigned md5;
  unsigned mdpath;
  unsigned integrity:1;
  unsigned fill;
  unsigned sql;
  unsigned statistics;
  unsigned nonew;
  unsigned path;
  char *file;
  FILE *out;
} duf_config_cli_trace_t;

typedef struct
{
  duf_config_cli_actions_t act;
  duf_config_cli_debug_t dbg;
  duf_config_cli_trace_t trace;
  duf_format_t format;
  /* long limit; */
  unsigned noopenat:1;
} duf_config_cli_t;

#endif