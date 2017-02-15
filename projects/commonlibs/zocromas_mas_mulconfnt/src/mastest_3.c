#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_arg_tools.h>

#include "mulconfnt_defs.h"
#include "mulconfnt_structs.h"

#include "option_tablist_base.h"

#include "tools.h"
#include "source.h"
#include "source_list_base.h"
#include "source_list.h"
#include "parse.h"
#include "mulconfnt_error.h"

#include "mastest.h"

int
test_3( int argc _uUu_, const char *argv[], int nseries, const char *series_suffix, int do_fprintf _uUu_ )
{
  const char *arg;
  char *v_string0 = NULL;
  char *v_string1 = NULL;
  char *v_string2 = NULL;
  char *v_string3 = NULL;

  const char *xargv[] = {
    argv[0],
    "something",
    "--string0=lorem-ipsum",
    "wow",
    "--string1=lorem ipsum",
    "--string2='lorem ipsum'",
    "--string3=\"lorem ipsum\"",
    "abrakadabra",
  };
#define NUM_OPTS 4
#define NUM_NOPTS ( sizeof( xargv ) / sizeof( xargv[0] ) - NUM_OPTS )

  int xargc = sizeof( xargv ) / sizeof( xargv[0] );

  config_option_t options[] = {
    {"string0", 0, MULCONF_RTYP_STRING | MULCONF_RTYP_FLAG_AUTOFREE, &v_string0,.flags = MULCONF_OPTION_NEED_EQ}
    , {"string1", 0, MULCONF_RTYP_STRING | MULCONF_RTYP_FLAG_AUTOFREE, &v_string1,.flags = MULCONF_OPTION_NEED_EQ}
    , {"string2", 0, MULCONF_RTYP_STRING | MULCONF_RTYP_FLAG_AUTOFREE, &v_string2,.flags =
       MULCONF_OPTION_NEED_EQ | MULCONF_OPTION_UNQUOTE}
    , {"string3", 0, MULCONF_RTYP_STRING | MULCONF_RTYP_FLAG_AUTOFREE, &v_string3,.flags =
       MULCONF_OPTION_NEED_EQ | MULCONF_OPTION_UNQUOTE}

    , {.name = NULL,.shortname = 0,.restype = 0,.ptr = NULL,.val = 0,.desc = NULL,.argdesc = NULL} /* */
  };
  config_option_table_list_t test_tablist = {
    .next = NULL,.count = ( sizeof( options ) / sizeof( options[0] ) ),.name = "test-table",.options = options, /* */
  };

  {
    FILE *f;
    char fname[128];

    snprintf( fname, sizeof( fname ), "mastest_%d%s.commands", nseries, series_suffix );
    f = fopen( fname, "w" );

    if ( f )
    {
      for ( int i = 1; i < xargc; i++ )
      {
        fprintf( f, "%s\n", xargv[i] );
      }
      fclose( f );
    }
  }
  {
    config_source_list_t *plist = mulconfnt_source_list_create(  );
    config_source_desc_t *osrc = mulconfnt_source_list_add_source( plist, MULCONF_SOURCE_ARGV, xargc, xargv, NULL, "=", NULL );

    mastest_next_group(  );
    mastest_exam( __LINE__, plist ? 1 : 0, "OK", "Error", "plist: %p", plist );
    mastest_exam( __LINE__, osrc ? 1 : 0, "OK", "Error", "osrc: %p", osrc );

    mulconfnt_source_lookup_all( osrc, &test_tablist );
    if ( osrc && osrc->oldtarg.argc )
    {
      FILE *f;
      char fname[128];

      snprintf( fname, sizeof( fname ), "mastest_%d%s.args", nseries, series_suffix );
      f = fopen( fname, "w" );
      if ( f )
      {
        for ( int i = 1; i < osrc->oldtarg.argc; i++ )
        {
          fprintf( f, "%s\n", osrc->oldtarg.argv[i] );
        }
        fclose( f );
      }
    }

    mastest_next_group(  );
    mastest_exam( __LINE__, !mulconfnt_error_source( osrc ), "OK", "Error", "mulconfnt_error: %d (last error: \"%s\")",
                  mulconfnt_error_source( osrc ), mulconfnt_error_source_msg( osrc ) );
    mastest_next_group(  );

    mastest_exam( __LINE__, v_string0
                  && 0 == mas_strcmp( v_string0, "lorem-ipsum" ), "OK", "Error", "string0=%s ? %s", v_string0 ? v_string0 : "<NULL>", "lorem-ipsum" );
  /* if ( v_string0 )         */
  /*   mas_free( v_string0 ); */
  /* v_string0 = NULL;        */

    mastest_exam( __LINE__, v_string1
                  && 0 == mas_strcmp( v_string1, "lorem ipsum" ), "OK", "Error", "string1=%s ? %s", v_string1 ? v_string1 : "<NULL>", "lorem ipsum" );
  /* if ( v_string1 )         */
  /*   mas_free( v_string1 ); */
  /* v_string1 = NULL;        */

    mastest_exam( __LINE__, v_string2
                  && 0 == mas_strcmp( v_string2, "lorem ipsum" ), "OK", "Error", "string1=%s ? %s", v_string2 ? v_string2 : "<NULL>", "lorem ipsum" );
  /* if ( v_string2 )         */
  /*   mas_free( v_string2 ); */
  /* v_string2 = NULL;        */

    mastest_exam( __LINE__, v_string3
                  && 0 == mas_strcmp( v_string3, "lorem ipsum" ), "OK", "Error", "string1=%s ? %s", v_string3 ? v_string3 : "<NULL>", "lorem ipsum" );
  /* if ( v_string3 )         */
  /*   mas_free( v_string3 ); */
  /* v_string3 = NULL;        */

    mastest_next_group(  );
    mastest_exam( __LINE__, mulconfnt_source_argc_no( osrc ) == NUM_NOPTS, "OK", "Error", "%d ? %d", mulconfnt_source_argc_no( osrc ), NUM_NOPTS );
    arg = mulconfnt_source_arg_no( osrc, 1 );
    mastest_exam( __LINE__, arg && 0 == mas_strcmp( "something", arg ), "OK", "Error", "'%s' ? '%s'", "something", arg );
    arg = mulconfnt_source_arg_no( osrc, 2 );
    mastest_exam( __LINE__, arg && 0 == mas_strcmp( "wow", arg ), "OK", "Error", "'%s' ? '%s'", "wow", arg );
    arg = mulconfnt_source_arg_no( osrc, 3 );
    mastest_exam( __LINE__, arg && 0 == mas_strcmp( "abrakadabra", arg ), "OK", "Error", "'%s' ? '%s'", "abrakadabra", arg );

    char **argvno = mulconfnt_source_argv_no( osrc );

    mastest_exam( __LINE__, argvno && argvno[1] && 0 == mas_strcmp( "something", argvno[1] ), "OK", "Error", "'%s' ? '%s'", "something", argvno[1] );
    mastest_exam( __LINE__, argvno && argvno[2] && 0 == mas_strcmp( "wow", argvno[2] ), "OK", "Error", "'%s' ? '%s'", "wow", argvno[2] );
    mastest_exam( __LINE__, argvno && argvno[3]
                  && 0 == mas_strcmp( "abrakadabra", argvno[3] ), "OK", "Error", "'%s' ? '%s'", "abrakadabra", argvno[3] );

    mulconfnt_source_list_delete( plist );
  }
  mulconfnt_config_option_tablist_close( &test_tablist );
  return 0;
}
