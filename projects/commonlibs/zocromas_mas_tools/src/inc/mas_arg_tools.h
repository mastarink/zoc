#ifndef MAS_ARG_TOOLS_H
# define MAS_ARG_TOOLS_H

# include <sys/types.h>

# define mas_strcat_x(...)  mas_strcat_xt (__VA_ARGS__)
# define mas_strncat_x(...)  mas_strncat_xt (__VA_ARGS__)

typedef const char *( *mas_arg_get_cb_arg_t ) ( const char *name, const char *arg );

size_t mas_wordlen( const char *s );
int mas_strcmp( const char *s1, const char *s2 );
int mas_strncmp( const char *s1, const char *s2, size_t len );
char *mas_chomp( char *s );
size_t mas_chomplen( const char *s );
const char *mas_strtolower( char *s );
char *mas_expand_string_cb_arg_alloc( const char *str, mas_arg_get_cb_arg_t cb, const char *arg );
char *mas_expand_string_cb_arg( const char *str, mas_arg_get_cb_arg_t cb, const char *arg );
char *mas_expand_string( const char *str );
int mas_strcmp2( const char *s1, const char *s2 );
int mas_strcmpv( const char *s1, const char *s2, char **vval );
const char *mas_find_eq_value( const char *s );

const char *mas_skip_space_nz( const char *args );
const char *mas_skip_space_nz_d( const char *args, const char *delims );
const char *mas_skip_space( const char *args );
const char *mas_skip_space_d( const char *args, const char *delims );

const char *mas_skip_argo_d( const char *args, const char **pthis, size_t * plen, int *quot __attribute__ ( ( unused ) ), const char *delims );
const char *mas_skip_argo( const char *args, const char **pthis, size_t * plen, int *quot );
const char *mas_skip_arg_d( const char *args, const char **pthis, size_t * plen, int *quot, const char *delims );
const char *mas_skip_arg( const char *args, const char **pthis, size_t * plen, int *quot );

/* int __mas_make_argv( const char *args, int argc, char ***pargv, int from_a ); */

int mas_add_argv_args( int targc, char ***ptargv, const char *args, int ia_offset );
int mas_add_argv_args_d( int targc, char ***ptargv, const char *args, int ia_offset, const char *delims );
int mas_add_argv_argv( int targc, char ***ptargv, int argc, char *const *argv, int ia_offset );
int mas_add_argv_arg( int targc, char ***ptargv, const char *arg );
int mas_add_argv_arg_nodup( int targc, char ***ptargv, char *arg );
int mas_add_argv_cmdline( int targc, char ***ptargv, const char *cmdline, size_t len );

int mas_del_argv( int argc, char **argv, int from_a );
int mas_argv_delete( int targc, char **targv );
int mas_argv_clone( char ***ptargv, int targc, char **targv );

char *mas_argv_join( int targc, char *const *targv, int from_a, char delim );
char *mas_argv_string( int targc, char *const *targv, int from_a );

const char *mas_find_next_argo( const char *args, const char **pthis, size_t * plen, int *quot );
const char *mas_find_next_arg( const char *args, const char **pthis, size_t * plen, int *quot );
const char *mas_find_next_argo_d( const char *args, const char **pthis, size_t * plen, int *quot, const char *delims );
const char *mas_find_next_arg_d( const char *args, const char **pthis, size_t * plen, int *quot, const char *delims );

char *mas_strcat_xt( char *s1, const char *s2 );
char *mas_strncat_xt( char *s1, const char *s2, size_t maxs2 );

#endif
