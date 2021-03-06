#ifndef MAS_TOOLS_H
# define MAS_TOOLS_H

# include <stdio.h>
# include <stdarg.h>
# include <stdio.h>
# include <time.h>
# include <sys/types.h>

size_t mas_xvsnprintf( char *str, size_t size, const char *format, va_list args );

char *mas_ip_string( void *sin_addr );

size_t mas_parse_host_port( const char *host, unsigned *phport, unsigned default_port );

char *mas_load_filename_file( const char *tail, size_t size, size_t * ptruesize, ino_t * ptrueinode, time_t * ptruefiletime, const void *arg );
char *mas_load_filename_fd( const char *filepath, size_t size, size_t * ptruesize, ino_t * ptrueinode, time_t * ptruefiletime, const void *arg );
char *mas_load_file( FILE * file, size_t size, size_t * ptruesize, ino_t * ptrueinode, time_t * ptruefiletime, const void *arg );
char *mas_load_fd( int fd, size_t size, size_t * ptruesize, ino_t * ptrueinode, time_t * ptruefiletime, const void *arg );

char *mas_dump1( const char *data, size_t len, size_t perline );
char *mas_dump2( const char *data, size_t len, size_t perline );
char *mas_escape( char *str );

void mas_common_constructor( const char *fname, int lin, int lev );
void mas_common_destructor( const char *fname, int lin, int lev );

#endif
