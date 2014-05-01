#ifndef MAS_DUF_SQLITE_H
#  define MAS_DUF_SQLITE_H

#  include <sqlite3.h>

int duf_sqlite_error_code( int r3 );

int duf_sqlite_open( const char *dbpath );
int duf_sqlite_close( void );


int duf_sqlite_execcb_e( const char *sql, duf_sqexe_cb_t sqexe_cb, void *sqexe_data, int *pchanges );
int duf_sqlite_execcb( const char *sql, duf_sqexe_cb_t sqexe_cb, void *sqexe_data, int *pchanges, char **pemsg );
int duf_sqlite_exec( const char *sql, int *pchanges, char **pemsg );
int duf_sqlite_exec_c( const char *sql, int constraint_ignore, int *pchanges );
int duf_sqlite_exec_e( const char *sql, int *pchanges );

int duf_vsqlite_c( const char *sqlfmt, int constraint_ignore, int *pchanges, va_list args );
int duf_vsqlite_e( const char *fmt, int *pchanges, va_list args );

int duf_sqlite_vselect( duf_sel_cb_t sel_cb, void *sel_cb_udata, duf_str_cb_t str_cb, void *str_cb_udata,
                        duf_depthinfo_t * pdi, duf_scan_callbacks_t * sccb  , const char *sqlfmt,
                        va_list args );

unsigned long long duf_sqlite_last_insert_rowid( void );

/* As for sqlite 3.8.4.3 */
/* #define SQLITE_OK           0   (* Successful result *)                          */
/* (* beginning-of-error-codes *)                                                   */
/* #define SQLITE_ERROR        1   (* SQL error or missing database *)              */
/* #define SQLITE_INTERNAL     2   (* Internal logic error in SQLite *)             */
/* #define SQLITE_PERM         3   (* Access permission denied *)                   */
/* #define SQLITE_ABORT        4   (* Callback routine requested an abort *)        */
/* #define SQLITE_BUSY         5   (* The database file is locked *)                */
/* #define SQLITE_LOCKED       6   (* A table in the database is locked *)          */
/* #define SQLITE_NOMEM        7   (* A malloc() failed *)                          */
/* #define SQLITE_READONLY     8   (* Attempt to write a readonly database *)       */
/* #define SQLITE_INTERRUPT    9   (* Operation terminated by sqlite3_interrupt()*) */
/* #define SQLITE_IOERR       10   (* Some kind of disk I/O error occurred *)       */
/* #define SQLITE_CORRUPT     11   (* The database disk image is malformed *)       */
/* #define SQLITE_NOTFOUND    12   (* Unknown opcode in sqlite3_file_control() *)   */
/* #define SQLITE_FULL        13   (* Insertion failed because database is full *)  */
/* #define SQLITE_CANTOPEN    14   (* Unable to open the database file *)           */
/* #define SQLITE_PROTOCOL    15   (* Database lock protocol error *)               */
/* #define SQLITE_EMPTY       16   (* Database is empty *)                          */
/* #define SQLITE_SCHEMA      17   (* The database schema changed *)                */
/* #define SQLITE_TOOBIG      18   (* String or BLOB exceeds size limit *)          */
/* #define SQLITE_CONSTRAINT  19   (* Abort due to constraint violation *)          */
/* #define SQLITE_MISMATCH    20   (* Data type mismatch *)                         */
/* #define SQLITE_MISUSE      21   (* Library used incorrectly *)                   */
/* #define SQLITE_NOLFS       22   (* Uses OS features not supported on host *)     */
/* #define SQLITE_AUTH        23   (* Authorization denied *)                       */
/* #define SQLITE_FORMAT      24   (* Auxiliary database format error *)            */
/* #define SQLITE_RANGE       25   (* 2nd parameter to sqlite3_bind out of range *) */
/* #define SQLITE_NOTADB      26   (* File opened that is not a database file *)    */
/* #define SQLITE_NOTICE      27   (* Notifications from sqlite3_log() *)           */
/* #define SQLITE_WARNING     28   (* Warnings from sqlite3_log() *)                */
/* #define SQLITE_ROW         100  (* sqlite3_step() has another row ready *)       */
/* #define SQLITE_DONE        101  (* sqlite3_step() has finished executing *)      */
#  define DUF_SQLITE_CODE_CHAR(arr,name) arr[name]=#name


#endif
