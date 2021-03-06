#define R_GOOD(_r) ((_r)>=0)
#include "mulconfnt_defs.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <malloc.h>

/* #include <popt.h> */

#include <mastar/wrap/mas_memory.h>
/* #include <mastar/tools/mas_arg_tools.h> */
#include <mastar/minierr/minierr.h>
#include <mastar/exam/masexam.h>

#include "mulconfnt_structs.h"
#include "source_defaults.h"

/* #include "mulconfnt_error.h" */

#include "source.h"
#include "source_list_base.h"
#include "source_list.h"
/* #include "parse.h" */

#include "mastest.h"

/*
 * See also
 *
 * dev-libs/argtable
 * dev-libs/shhopt
 * dev-libs/popt
 *
 * */

#if 0
extern void *__libc_malloc( size_t size );

int malloc_hook_active = 0;
void *
my_malloc_hook( size_t size, void *caller _uUu_ )
{
  void *result;

// deactivate hooks for logging
  malloc_hook_active = 0;

  result = malloc( size );

// do logging
//  [ ...]
  fprintf( stderr, "Wow\n" );
// reactivate hooks
  malloc_hook_active = 1;

  return result;
}

void *
malloc( size_t size )
{
  void *caller _uUu_ = __builtin_return_address( 0 );

  if ( malloc_hook_active )
    return my_malloc_hook( size, caller );
  return __libc_malloc( size );
}
#endif
void
mastest_print_allocated( const char *msg, int line, const char *func )
{
  struct mallinfo mi;

  mi = mallinfo(  );
  fprintf( stderr, "\n\x1b[0;1;44;35m%s %d bytes at %d:%s\x1b[0m\n", msg, mi.uordblks, line, func );
}

static void constructor_main(  ) __attribute__ ( ( constructor( 2001 ) ) );
static void
constructor_main(  )
{
  mastest_print_allocated( "Allocated", __LINE__, __func__ );
  mallopt( M_CHECK_ACTION, 1 );
/* configure my zocromas_mas_wrap library (malloc/free wrapper) not to print memory usage map; may be enabled later */
#ifdef MAS_TRACEMEM
  {
    extern int mas_mem_disable_print_usage __attribute__ ( ( weak ) );

    if ( &mas_mem_disable_print_usage )
    {
      mas_mem_disable_print_usage = 0;
    }
  }

#endif
  fprintf( stderr, "START\n" );
}

/* getopt_long */
int
tests( int argc, const char *argv[] )
{
  int test_popt( int argc, const char *argv[], int nseries, const char *series_suffix, int variant );
  int test_popt1( int argc, const char *argv[], int nseries, const char *series_suffix, int variant );
  int test_0( int argc, const char *argv[], int nseries, const char *series_suffix, int variant );
  int test_1( int argc, const char *argv[], int nseries, const char *series_suffix, int variant );
  int test_1s( int argc, const char *argv[], int nseries, const char *series_suffix, int variant );
  int test_1mul( int argc, const char *argv[], int nseries, const char *series_suffix, int variant );
  int test_1enf( int argc, const char *argv[], int nseries, const char *series_suffix, int variant );
  int test_1u( int argc, const char *argv[], int nseries, const char *series_suffix, int variant );
  int test_2( int argc, const char *argv[], int nseries, const char *series_suffix, int variant );
  int test_2a( int argc, const char *argv[], int nseries, const char *series_suffix, int variant );
  int test_3( int argc, const char *argv[], int nseries, const char *series_suffix, int variant );
  int test_3a( int argc, const char *argv[], int nseries, const char *series_suffix, int variant );
  int test_3q( int argc, const char *argv[], int nseries, const char *series_suffix, int variant );
  int test_4( int argc, const char *argv[], int nseries, const char *series_suffix, int variant );
  int test_5( int argc, const char *argv[], int nseries, const char *series_suffix, int variant );
  int test_6( int argc, const char *argv[], int nseries, const char *series_suffix, int variant );
  int test_7( int argc, const char *argv[], int nseries, const char *series_suffix, int variant );
  int test_static( int argc, const char *argv[], int nseries, const char *series_suffix, int variant );
  int test_static2( int argc, const char *argv[], int nseries, const char *series_suffix, int variant );
  int test_manual_0( int argc, const char *argv[], int nseries, const char *series_suffix, int variant );
  int test_manual( int argc, const char *argv[], int nseries, const char *series_suffix, int variant );
  int test_manual_1( int argc, const char *argv[], int nseries, const char *series_suffix, int variant );

  masexam_do_t funlist[] _uUu_ = {
    {0, test_popt, 0, "popt",.stdin_text = NULL},
    {0, test_popt1, 1, "popt"},
    {FALSE, test_0, 0, ""},

    {TRUE, test_1, 1, "",.variant = 0},
    {TRUE, test_1s, 1, "s"},
    {TRUE, test_1mul, 1, "mul",.f_print_ok = 0},
    {TRUE, test_1u, 1, "u"},
    {TRUE, test_1enf, 1, "enf"},
    {TRUE, test_2, 2, ""},
    {TRUE, test_2a, 2, "a"},
    {TRUE, test_3, 3, ""},
    {TRUE, test_3a, 3, "a"},
    {TRUE, test_3q, 3, "q"},
    {TRUE, test_4, 4, ""},
    {TRUE, test_5, 5, ""},
    {TRUE, test_6, 6, "",.f_print_ok = 0},
    {TRUE, test_7, 7, "",.stdin_text = "and1b:and2b"},               /* !! */
    {TRUE, test_static, 8, "static"},
    {TRUE, test_static2, 8, "static2"},
    {FALSE, test_manual_0, 9, "manual_0"},
    {TRUE, test_manual, 9, "manual"},
    {FALSE, test_manual_1, 10, "manual_1"},
    {0}
  };
  WARN( "### FDs:%d", mastest_fds(  ) );
  masexam_test( argc, argv, funlist );
  WARN( "### FDs:%d", mastest_fds(  ) );

  return 0;
}

int
mastest_fds( void )
{
  return masexam_fds(  );
}

int
main( int argc, const char *argv[] )
{
  int repeat = 3;

  WARN( "### FDs:%d", mastest_fds(  ) );

  for ( int ir = 0; ir < repeat; ir++ )
    tests( argc, argv );

#define TOTAL_TESTS (  387 +63 +63 - 1 + 17) *repeat
/* EXAMX( masexam_tests_count(  ) == TOTAL_TESTS, "tests_count=%d ? %d", masexam_tests_count(  ), TOTAL_TESTS ); */
  EXAM( masexam_tests_count(  ), TOTAL_TESTS, "tests_count=%d ? %d" );

  masregerr_print_simple_all_default( NULL, NULL, 0 );
  masregerrs_delete_default( NULL );
#if 0
  {
    int filedes[2];                                                  /* pipe */
    int file_rd = fileno( stdin );                                   /* filedescriptor = 0 */
    int file_wr = fileno( stdout );                                  /* filedescriptor = 1 */
    int pipe_rd;
    int pipe_wr;

    int saved[2] _uUu_ = { dup( file_rd ), dup( file_wr ) };

    pipe( filedes );

    pipe_wr = filedes[1];
    pipe_rd = filedes[0];
  /* */

    if ( fork(  ) )
    {
      int saved _uUu_ = dup( file_rd );
      char ch;

    /* child */
    /* stdin replaced by pipe output */
      dup2( pipe_rd, file_rd );

      close( pipe_wr );                                              /* close part of pipe unneeded */

      while ( ( ch = fgetc( stdin ) ) > 0 )
      {
      /* fprintf(stderr, "c=%c\n",ch); */
        fprintf( stdout, "%d : (%c)", fileno( stdout ), ch );
      }

      close( pipe_rd );
    /* close(file_rd); */
    /* close(file_wr); */
      dup2( saved, file_rd );
    }
    else
    {
      int saved _uUu_ = dup( file_wr );

    /* parent */
    /* stdout replaced by pipe input */
      dup2( pipe_wr, file_wr );

      close( pipe_rd );                                              /* close part of pipe unneeded */

      fprintf( stdout, "%s", "A Hello world\n" );
      close( pipe_wr );
    /* close(file_rd); */
    /* close(file_wr); */
      fflush( stdout );
      dup2( saved, file_wr );
    }
  }
#endif
}
