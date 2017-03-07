#include "mulconfnt_defs.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <malloc.h>

#include <popt.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_arg_tools.h>
#include <mastar/exam/masexam.h>

#include "mulconfnt_structs.h"

#include "mulconfnt_error.h"

#include "source.h"
#include "source_list_base.h"
#include "source_list.h"
#include "parse.h"

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

static void constructor_main( int argc, char **argv, char **envp ) __attribute__ ( ( constructor( 2001 ) ) );
static void
constructor_main( int argc _uUu_, char **argv _uUu_, char **envp _uUu_ )
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
main( int argc, const char *argv[] )
{
  int test_popt( int argc, const char *argv[], int nseries, const char *series_suffix, int do_fprintf );
  int test_popt1( int argc, const char *argv[], int nseries, const char *series_suffix, int do_fprintf );
  int test_0( int argc, const char *argv[], int nseries, const char *series_suffix, int do_fprintf );
  int test_1( int argc, const char *argv[], int nseries, const char *series_suffix, int do_fprintf );
  int test_1s( int argc, const char *argv[], int nseries, const char *series_suffix, int do_fprintf );
  int test_1mul( int argc, const char *argv[], int nseries, const char *series_suffix, int do_fprintf );
  int test_1enf( int argc, const char *argv[], int nseries, const char *series_suffix, int do_fprintf );
  int test_1u( int argc, const char *argv[], int nseries, const char *series_suffix, int do_fprintf );
  int test_2( int argc, const char *argv[], int nseries, const char *series_suffix, int do_fprintf );
  int test_2a( int argc, const char *argv[], int nseries, const char *series_suffix, int do_fprintf );
  int test_3( int argc, const char *argv[], int nseries, const char *series_suffix, int do_fprintf );
  int test_3a( int argc, const char *argv[], int nseries, const char *series_suffix, int do_fprintf );
  int test_3q( int argc, const char *argv[], int nseries, const char *series_suffix, int do_fprintf );
  int test_4( int argc, const char *argv[], int nseries, const char *series_suffix, int do_fprintf );
  int test_5( int argc, const char *argv[], int nseries, const char *series_suffix, int do_fprintf );
  int test_6( int argc, const char *argv[], int nseries, const char *series_suffix, int do_fprintf );

  masexam_do_t funlist[] _uUu_ = {
    {0, test_popt, 0, "popt"},
    {0, test_popt1, 1, "popt"},
    {0, test_0, 0, ""},
    {1, test_1, 1, "",.do_fprintf = 0},
    {1, test_1s, 1, "s"},
    {1, test_1mul, 1, "mul",.f_print_ok = 0},
    {1, test_1u, 1, "u"},
    {1, test_1enf, 1, "enf"},
    {1, test_2, 2, ""},
    {1, test_2a, 2, "a"},
    {1, test_3, 3, ""},
    {1, test_3a, 3, "a"},
    {1, test_3q, 3, "q"},
    {1, test_4, 4, ""},
    {1, test_5, 5, ""},
    {1, test_6, 6, "",.f_print_ok = 0},
    {0}
  };
  masexam_test( argc, argv, funlist );
#define TOTAL_TESTS 349 - 1
  masexam_exam( 0, masexam_tests_count(  ) == TOTAL_TESTS, "OK", "Error", "tests_count=%d ? %d", masexam_tests_count(  ), TOTAL_TESTS );

  return 0;
}