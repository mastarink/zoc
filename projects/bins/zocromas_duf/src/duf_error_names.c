#include <mastar/sqlite/mas_sqlite_const.h>

/* ###################################################################### */
#include "duf_error_names.h"
/* ###################################################################### */

#define DUF_ERROR_NAME(ername) {.name=#ername, .code=ername}

const duf_errdesc_t _error_descriptions_table[] = {
  DUF_ERROR_NAME( MAS_SQL_ERROR ),
  DUF_ERROR_NAME( MAS_SQL_INTERNAL ),
  DUF_ERROR_NAME( MAS_SQL_PERM ),
  DUF_ERROR_NAME( MAS_SQL_ABORT ),
  DUF_ERROR_NAME( MAS_SQL_BUSY ),
  DUF_ERROR_NAME( MAS_SQL_LOCKED ),
  DUF_ERROR_NAME( MAS_SQL_NOMEM ),
  DUF_ERROR_NAME( MAS_SQL_READONLY ),
  DUF_ERROR_NAME( MAS_SQL_INTERRUPT ),
  DUF_ERROR_NAME( MAS_SQL_IOERR ),
  DUF_ERROR_NAME( MAS_SQL_CORRUPT ),
  DUF_ERROR_NAME( MAS_SQL_NOTFOUND ),
  DUF_ERROR_NAME( MAS_SQL_FULL ),
  DUF_ERROR_NAME( MAS_SQL_CANTOPEN ),
  DUF_ERROR_NAME( MAS_SQL_PROTOCOL ),
  DUF_ERROR_NAME( MAS_SQL_EMPTY ),
  DUF_ERROR_NAME( MAS_SQL_SCHEMA ),
  DUF_ERROR_NAME( MAS_SQL_TOOBIG ),
  DUF_ERROR_NAME( MAS_SQL_CONSTRAINT ),
  DUF_ERROR_NAME( MAS_SQL_MISMATCH ),
  DUF_ERROR_NAME( MAS_SQL_MISUSE ),
  DUF_ERROR_NAME( MAS_SQL_NOLFS ),
  DUF_ERROR_NAME( MAS_SQL_AUTH ),
  DUF_ERROR_NAME( MAS_SQL_FORMAT ),
  DUF_ERROR_NAME( MAS_SQL_RANGE ),
  DUF_ERROR_NAME( MAS_SQL_NOTADB ),
  DUF_ERROR_NAME( MAS_SQL_NOTICE ),
  DUF_ERROR_NAME( MAS_SQL_WARNING ),
  DUF_ERROR_NAME( MAS_SQL_ROW ),
  DUF_ERROR_NAME( MAS_SQL_DONE ),
/* extended */
  DUF_ERROR_NAME( MAS_SQL_IOERR_READ ),
  DUF_ERROR_NAME( MAS_SQL_IOERR_SHORT_READ ),
  DUF_ERROR_NAME( MAS_SQL_IOERR_WRITE ),
  DUF_ERROR_NAME( MAS_SQL_IOERR_FSYNC ),
  DUF_ERROR_NAME( MAS_SQL_IOERR_DIR_FSYNC ),
  DUF_ERROR_NAME( MAS_SQL_IOERR_TRUNCATE ),
  DUF_ERROR_NAME( MAS_SQL_IOERR_FSTAT ),
  DUF_ERROR_NAME( MAS_SQL_IOERR_UNLOCK ),
  DUF_ERROR_NAME( MAS_SQL_IOERR_RDLOCK ),
  DUF_ERROR_NAME( MAS_SQL_IOERR_DELETE ),
  DUF_ERROR_NAME( MAS_SQL_IOERR_BLOCKED ),
  DUF_ERROR_NAME( MAS_SQL_IOERR_NOMEM ),
  DUF_ERROR_NAME( MAS_SQL_IOERR_ACCESS ),
  DUF_ERROR_NAME( MAS_SQL_IOERR_CHECKRESERVEDLOCK ),
  DUF_ERROR_NAME( MAS_SQL_IOERR_LOCK ),
  DUF_ERROR_NAME( MAS_SQL_IOERR_CLOSE ),
  DUF_ERROR_NAME( MAS_SQL_IOERR_DIR_CLOSE ),
  DUF_ERROR_NAME( MAS_SQL_IOERR_SHMOPEN ),
  DUF_ERROR_NAME( MAS_SQL_IOERR_SHMSIZE ),
  DUF_ERROR_NAME( MAS_SQL_IOERR_SHMLOCK ),
  DUF_ERROR_NAME( MAS_SQL_IOERR_SHMMAP ),
  DUF_ERROR_NAME( MAS_SQL_IOERR_SEEK ),
  DUF_ERROR_NAME( MAS_SQL_IOERR_DELETE_NOENT ),
  DUF_ERROR_NAME( MAS_SQL_IOERR_MMAP ),
  DUF_ERROR_NAME( MAS_SQL_IOERR_GETTEMPPATH ),
  DUF_ERROR_NAME( MAS_SQL_IOERR_CONVPATH ),
  DUF_ERROR_NAME( MAS_SQL_LOCKED_SHAREDCACHE ),
  DUF_ERROR_NAME( MAS_SQL_BUSY_RECOVERY ),
  DUF_ERROR_NAME( MAS_SQL_BUSY_SNAPSHOT ),
  DUF_ERROR_NAME( MAS_SQL_CANTOPEN_NOTEMPDIR ),
  DUF_ERROR_NAME( MAS_SQL_CANTOPEN_ISDIR ),
  DUF_ERROR_NAME( MAS_SQL_CANTOPEN_FULLPATH ),
  DUF_ERROR_NAME( MAS_SQL_CANTOPEN_CONVPATH ),
  DUF_ERROR_NAME( MAS_SQL_CORRUPT_VTAB ),
  DUF_ERROR_NAME( MAS_SQL_READONLY_RECOVERY ),
  DUF_ERROR_NAME( MAS_SQL_READONLY_CANTLOCK ),
  DUF_ERROR_NAME( MAS_SQL_READONLY_ROLLBACK ),
  DUF_ERROR_NAME( MAS_SQL_READONLY_DBMOVED ),
  DUF_ERROR_NAME( MAS_SQL_ABORT_ROLLBACK ),
  DUF_ERROR_NAME( MAS_SQL_CONSTRAINT_CHECK ),
  DUF_ERROR_NAME( MAS_SQL_CONSTRAINT_COMMITHOOK ),
  DUF_ERROR_NAME( MAS_SQL_CONSTRAINT_FOREIGNKEY ),
  DUF_ERROR_NAME( MAS_SQL_CONSTRAINT_FUNCTION ),
  DUF_ERROR_NAME( MAS_SQL_CONSTRAINT_NOTNULL ),
  DUF_ERROR_NAME( MAS_SQL_CONSTRAINT_PRIMARYKEY ),
  DUF_ERROR_NAME( MAS_SQL_CONSTRAINT_TRIGGER ),
  DUF_ERROR_NAME( MAS_SQL_CONSTRAINT_UNIQUE ),
  DUF_ERROR_NAME( MAS_SQL_CONSTRAINT_VTAB ),
  DUF_ERROR_NAME( MAS_SQL_CONSTRAINT_ROWID ),
  DUF_ERROR_NAME( MAS_SQL_NOTICE_RECOVER_WAL ),
  DUF_ERROR_NAME( MAS_SQL_NOTICE_RECOVER_ROLLBACK ),
  DUF_ERROR_NAME( MAS_SQL_WARNING_AUTOINDEX ),
/*========================================*/
  DUF_ERROR_NAME( DUF_OK ),
  DUF_ERROR_NAME( DUF_ERROR_UNDEFINED ),
  DUF_ERROR_NAME( DUF_ERROR_UNKNOWN ),
  DUF_ERROR_NAME( DUF_ERROR_UNKNOWN_NODE ),
  DUF_ERROR_NAME( DUF_ERROR_MAIN ),
  DUF_ERROR_NAME( DUF_ERROR_NO_ACTIONS ),
  DUF_ERROR_NAME( DUF_ERROR_PTR ),
  DUF_ERROR_NAME( DUF_ERROR_DATA ),
  DUF_ERROR_NAME( DUF_ERROR_SCCB ),
  DUF_ERROR_NAME( DUF_ERROR_MD5 ),
  DUF_ERROR_NAME( DUF_ERROR_SHA1 ),
  DUF_ERROR_NAME( DUF_ERROR_CRC32 ),
  DUF_ERROR_NAME( DUF_ERROR_EXIF_END ),
  DUF_ERROR_NAME( DUF_ERROR_EXIF_NO_MODEL ),
  DUF_ERROR_NAME( DUF_ERROR_EXIF_NO_DATE ),
  DUF_ERROR_NAME( DUF_ERROR_EXIF_BROKEN_DATE ),
  DUF_ERROR_NAME( DUF_ERROR_EXIF ),
  DUF_ERROR_NAME( DUF_ERROR_NOT_OPEN ),
  DUF_ERROR_NAME( DUF_ERROR_EOF ),
  DUF_ERROR_NAME( DUF_ERROR_PATH ),
  DUF_ERROR_NAME( DUF_ERROR_OPENAT ),
  DUF_ERROR_NAME( DUF_ERROR_OPENAT_ENOENT ),
  DUF_ERROR_NAME( DUF_ERROR_OPEN ),
  DUF_ERROR_NAME( DUF_ERROR_OPEN_ENOENT ),
  DUF_ERROR_NAME( DUF_ERROR_FS_DISABLED ),
  DUF_ERROR_NAME( DUF_ERROR_READ ),
  DUF_ERROR_NAME( DUF_ERROR_CLOSE ),
  DUF_ERROR_NAME( DUF_ERROR_UNLINK ),
  DUF_ERROR_NAME( DUF_ERROR_OPTION ),
  DUF_ERROR_NAME( DUF_ERROR_OPTION_MULTIPLE ),
  DUF_ERROR_NAME( DUF_ERROR_OPTION_NOT_FOUND ),
  DUF_ERROR_NAME( DUF_ERROR_OPTION_NOT_PARSED ),
  DUF_ERROR_NAME( DUF_ERROR_OPTION_NO_FUNC ),
  DUF_ERROR_NAME( DUF_ERROR_OPTION_VALUE ),
  DUF_ERROR_NAME( DUF_ERROR_SUBOPTION ),
  DUF_ERROR_NAME( DUF_ERROR_SCANDIR ),
  DUF_ERROR_NAME( DUF_ERROR_CHECK_TABLES ),
  DUF_ERROR_NAME( DUF_ERROR_CLEAR_TABLES ),
  DUF_ERROR_NAME( DUF_ERROR_NO_FILE_SELECTOR ),
  DUF_ERROR_NAME( DUF_ERROR_DB_NO_PATH ),
  DUF_ERROR_NAME( DUF_ERROR_NO_STR_CB ),
  DUF_ERROR_NAME( DUF_ERROR_BIND_NAME ),
  DUF_ERROR_NAME( DUF_ERROR_LEVINFO_SIZE ),
  DUF_ERROR_NAME( DUF_ERROR_TOO_DEEP ),
  DUF_ERROR_NAME( DUF_ERROR_MAX_REACHED ),
  DUF_ERROR_NAME( DUF_ERROR_MAX_SEQ_REACHED ),
  DUF_ERROR_NAME( DUF_ERROR_GET_FIELD ),
  DUF_ERROR_NAME( DUF_ERROR_NOT_IN_DB ),
  DUF_ERROR_NAME( DUF_ERROR_TOTALS ),
  DUF_ERROR_NAME( DUF_ERROR_NO_FIELD ),
  DUF_ERROR_NAME( DUF_ERROR_NO_FIELD_OPTIONAL ),
  DUF_ERROR_NAME( DUF_ERROR_INSERT_MDPATH ),
  DUF_ERROR_NAME( DUF_ERROR_NOT_FOUND ),
  DUF_ERROR_NAME( DUF_ERROR_MKDIR ),
  DUF_ERROR_NAME( DUF_ERROR_STAT ),
  DUF_ERROR_NAME( DUF_ERROR_STAT_ENOENT ),
  DUF_ERROR_NAME( DUF_ERROR_STATAT ),
  DUF_ERROR_NAME( DUF_ERROR_STATAT_ENOENT ),
  DUF_ERROR_NAME( DUF_ERROR_PDI_SQL ),
  DUF_ERROR_NAME( DUF_ERROR_SQL_NO_FIELDSET ), /* */
  DUF_ERROR_NAME( DUF_ERROR_SQL_NO_TABLE ), /* */
  DUF_ERROR_NAME( DUF_ERROR_MEMORY ), /* */
  DUF_ERROR_NAME( DUF_ERROR_ARGUMENT ),
  DUF_ERROR_NAME( DUF_ERROR_FILE_EXISTS ),
  DUF_ERROR_NAME( DUF_ERROR_SCCB_NOT_FOUND ),
/*========================================*/
  DUF_ERROR_NAME( DUF_ERROR_ERROR_MAX ),
};

const duf_errdesc_t *duf_error_descriptions_table = _error_descriptions_table;
int duf_error_descriptions_table_size = sizeof( _error_descriptions_table ) / sizeof( _error_descriptions_table[0] );
