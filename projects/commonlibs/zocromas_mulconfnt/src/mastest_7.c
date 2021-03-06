#define R_GOOD(_r) ((_r)>=0)
#include "mulconfnt_defs.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <popt.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_arg_tools.h>
#include <mastar/exam/masexam.h>
#include <mastar/minierr/minierr.h>

#include "mulconfnt_structs.h"

#include "global.h"

#include "global.h"
#include "option_tablist_base.h"

#include "source.h"
#include "source_list_base.h"
#include "source_list.h"
/* #include "parse.h" */

#include "option_interface_base.h"
#include "option_interface.h"

#include "mastest.h"

int
test_7( _uUu_ int argc, const char *argv[], int nseries _uUu_, const char *series_suffix _uUu_, int variant _uUu_ )
{
  rDECLGOOD;
  int ifds _uUu_ = mastest_fds(  );

  const char *xargv[] = {
    argv[0],
    "something",
    "-oO",                                                           /* --or2 --or1 */
    "--wrong",
    "-n -N",                                                         /* --nand2 --nand1 */
    "--and1", "--and2",                                              /* -Aa */
    "--and1a --and2a",
    "-Z",
    "wow",
    "--and1--and2",
    "--",
    "--nand3",
    "--this-is-not-an-option1=345",
    "--this-is-not-an-option2",
    "--",
    "abrakadabra",
  };
  int xargc = sizeof( xargv ) / sizeof( xargv[0] );

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

  unsigned long app_uflags_or _uUu_ = 0x0L;
  unsigned long app_uflags_and _uUu_ = ~0x0L;
  unsigned long app_uflags_and_a _uUu_ = ~0x0L;
  unsigned long app_uflags_and_b _uUu_ = ~0x0L;
  unsigned long app_uflags_nand _uUu_ = ~0x0L;

  mucs_option_static_t soptions[] = {
    {"or1", 'O',.restype = MUCS_RTYP_ULONG | MUCS_RTYP_BW_OR,.cust_ptr = &app_uflags_or,.def_nvalue.v_ulong = 0x101,.val = 0, "app_uflags_or",.argdesc = "value",.flags = MUCS_FLAG_NO_VALUE | MUCS_FLAG_USE_DEF_NVALUE}, /* */
    {"or2", 'o',.restype = MUCS_RTYP_ULONG | MUCS_RTYP_BW_OR,.cust_ptr = &app_uflags_or,.def_nvalue.v_ulong = 0x404,.val = 0, "app_uflags_or",.argdesc = "value",.flags = MUCS_FLAG_NO_VALUE | MUCS_FLAG_USE_DEF_NVALUE}, /* */
    {"and1", 'A',.restype = MUCS_RTYP_ULONG | MUCS_RTYP_BW_AND,.cust_ptr = &app_uflags_and,.def_nvalue.v_ulong = 0x101,.val = 0, "app_uflags_and",.argdesc = "value",.flags = MUCS_FLAG_NO_VALUE | MUCS_FLAG_USE_DEF_NVALUE}, /* */
    {"and2", 'a',.restype = MUCS_RTYP_ULONG | MUCS_RTYP_BW_AND,.cust_ptr = &app_uflags_and,.def_nvalue.v_ulong = 0x104,.val = 0, "app_uflags_and",.argdesc = "value",.flags = MUCS_FLAG_NO_VALUE | MUCS_FLAG_USE_DEF_NVALUE}, /* */
    {"and1a", '1',.restype = MUCS_RTYP_ULONG | MUCS_RTYP_BW_AND,.cust_ptr = &app_uflags_and_a,.def_nvalue.v_ulong = 0x101,.val = 0, "app_uflags_and_a",.argdesc = "value",.flags = MUCS_FLAG_NO_VALUE | MUCS_FLAG_USE_DEF_NVALUE}, /* */
    {"and2a", '2',.restype = MUCS_RTYP_ULONG | MUCS_RTYP_BW_AND,.cust_ptr = &app_uflags_and_a,.def_nvalue.v_ulong = 0x104,.val = 0, "app_uflags_and_a",.argdesc = "value",.flags = MUCS_FLAG_NO_VALUE | MUCS_FLAG_USE_DEF_NVALUE}, /* */
    {"and1b", '\0',.restype = MUCS_RTYP_ULONG | MUCS_RTYP_BW_AND,.cust_ptr = &app_uflags_and_b,.def_nvalue.v_ulong = 0x701,.val = 0, "app_uflags_and_b",.argdesc = "value",.flags = MUCS_FLAG_NO_VALUE | MUCS_FLAG_USE_DEF_NVALUE}, /* */
    {"and2b", '\0',.restype = MUCS_RTYP_ULONG | MUCS_RTYP_BW_AND,.cust_ptr = &app_uflags_and_b,.def_nvalue.v_ulong = 0x104,.val = 0, "app_uflags_and_b",.argdesc = "value",.flags = MUCS_FLAG_NO_VALUE | MUCS_FLAG_USE_DEF_NVALUE}, /* */
    {"nand1", 'N',.restype = MUCS_RTYP_ULONG | MUCS_RTYP_BW_AND | MUCS_RTYP_BW_NOT,.cust_ptr = &app_uflags_nand,.def_nvalue.v_ulong = 0x101,.val = 0, "app_uflags_nand",.argdesc = "value",.flags = MUCS_FLAG_NO_VALUE | MUCS_FLAG_USE_DEF_NVALUE}, /* */
    {"nand2", 'n',.restype = MUCS_RTYP_ULONG | MUCS_RTYP_BW_AND | MUCS_RTYP_BW_NOT,.cust_ptr = &app_uflags_nand,.def_nvalue.v_ulong = 0x404,.val = 0, "app_uflags_nand",.argdesc = "value",.flags = MUCS_FLAG_NO_VALUE | MUCS_FLAG_USE_DEF_NVALUE}, /* */
    {"nand3", '3',.restype = MUCS_RTYP_ULONG | MUCS_RTYP_BW_AND | MUCS_RTYP_BW_NOT,.cust_ptr = &app_uflags_nand,.def_nvalue.v_ulong = 0xfff,.val = 0, "app_uflags_nand",.argdesc = "value",.flags = MUCS_FLAG_NO_VALUE | MUCS_FLAG_USE_DEF_NVALUE}, /* */
    {.name = "",.shortn = '$',.flags = MUCS_FLAG_LASTOPT},           /* */
    {.name = NULL,.shortn = 0,.restype = 0,.cust_ptr = NULL,.def_string_value = NULL,.val = 0,.desc = NULL,.argdesc = NULL} /* */
  };
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
  EXAM( ( masregerrs_count_all_default( NULL, FALSE ) ), ( unsigned ) 0, "ERRORS: %d ? %d" );
  EXAM( app_uflags_or, 0x0UL, "app_uflags_or=%lx ? %lx" );
  EXAM( app_uflags_nand, 0xffffffffffffffffL, "app_uflags_nand=%lx ? %lx" );
  mucs_set_global_flag( MUCS_FLAG_CONTINUE_ON_UNRECOGNIZED );
  mucs_option_interface_t *interface = mucs_config_soption_interface_create_setup( "table-7", soptions, TRUE, 0 /* more_flags */  );

#if 0
  mucs_option_interface_add_source( interface, MUCS_SOURCE_LIBCONFIG, NULL /* name */ , 0, NULL, 0 /* min_pass */ , 0 /* npasses */  );
  mucs_option_interface_add_source( interface, MUCS_SOURCE_CONFIG, MULCONFNT_ETC_CONFIG /* name */ , 0, MULCONFNT_ETC_CONFIG, 0 /* min_pass */ ,
                                    0 /* npasses */  );
  mucs_option_interface_add_source( interface, MUCS_SOURCE_ENV, NULL /* name */ , 0, "MAS_TEST7_ENV", 0 /* min_pass */ , 0 /* npasses */  );
#endif
  mucs_option_interface_add_source( interface, MUCS_SOURCE_STDIN, NULL /* name */ , 0, NULL, 1 /* min_pass */ , 0 /* npasses */  );
  mucs_option_interface_add_source( interface, MUCS_SOURCE_ARGV, NULL /* name */ , xargc, xargv, 0 /* min_pass */ , 0 /* npasses */  );
  EXAM( ( masregerrs_count_all_default( NULL, FALSE ) ), ( unsigned ) 0, "ERRORS: %d ? %d" );

  WARN( "### FDs:%d", mastest_fds(  ) );
  rC( mucs_option_interface_lookup_all( interface, NULL ) );
  WARN( "### FDs:%d", mastest_fds(  ) );
  EXAM( ( masregerrs_count_all_default( NULL, FALSE ) ), ( unsigned ) 3, "ERRORS: %d ? %d" );
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
  WARN( "### FDs:%d", mastest_fds(  ) );

  EXAM( app_uflags_or, 0x505UL, "app_uflags_or=%lx ? %lx" );
  EXAM( app_uflags_and, 0x100UL, "app_uflags_and=%lx ? %lx" );
  EXAM( app_uflags_and_a, 0x100UL, "app_uflags_and_a=%lx ? %lx" );
  EXAM( app_uflags_and_b, ~0x0UL, "app_uflags_and_b=%lx ? %lx" );    /* stdin skipped at pass 0, NOTHING from .stdin_text = "and1b:and2b"  at  mastest_manual.c */
  EXAM( app_uflags_nand, ~0x505UL, "app_uflags_nand=%lx ? %lx" );
  EXAM( app_uflags_nand, 0xfffffffffffffafaUL, "app_uflags_nand=%lx ? %lx" );

  app_uflags_or = 0UL;
  app_uflags_and = ~0UL;
  app_uflags_and_a = ~0UL;
  app_uflags_and_b = ~0UL;
  app_uflags_nand = ~0UL;

  EXAM( app_uflags_or, 0x0UL, "app_uflags_or=%lx ? %lx" );
  EXAM( app_uflags_and, ~0x0UL, "app_uflags_and=%lx ? %lx" );
  EXAM( app_uflags_and_a, ~0x0UL, "app_uflags_and_a=%lx ? %lx" );
  EXAM( app_uflags_and_b, ~0x0UL, "app_uflags_and_b=%lx ? %lx" );    /* from .stdin_text = "and1b:and2b"  at  mastest_manual.c */
  EXAM( app_uflags_nand, ~0x0UL, "app_uflags_nand=%lx ? %lx" );
#if 1
  rC( mucs_option_interface_lookup_all( interface, NULL ) );
  EXAM( ( masregerrs_count_all_default( NULL, FALSE ) ), ( unsigned ) 6, "ERRORS: %d ? %d" );

  EXAM( app_uflags_or, 0x505UL, "app_uflags_or=%lx ? %lx" );
  EXAM( app_uflags_and, 0x100UL, "app_uflags_and=%lx ? %lx" );
  EXAM( app_uflags_and_a, 0x100UL, "app_uflags_and_a=%lx ? %lx" );
  EXAM( app_uflags_and_b, 0x100UL, "app_uflags_and_b=%lx ? %lx" );   /* from .stdin_text = "and1b:and2b"  at  mastest_manual.c */
  EXAM( app_uflags_nand, ~0x505UL, "app_uflags_nand=%lx ? %lx" );
  EXAM( app_uflags_nand, 0xfffffffffffffafaUL, "app_uflags_nand=%lx ? %lx" );
#endif
  WARN( "### FDs:%d", mastest_fds(  ) );
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
  mucs_config_option_interface_delete( interface );
  interface = NULL;
  mucs_clear_global_flags(  );
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
  EXAM( ( masregerrs_count_all_default( NULL, FALSE ) ), ( unsigned ) 6, "ERRORS: %d ? %d" );
  WARN( "### FDs:%d", mastest_fds(  ) );
  masregerr_print_simple_all_default( NULL, NULL, 0 );
  masregerrs_delete_default( NULL );
  WARN( "### FDs:%d", mastest_fds(  ) );
  EXAM( ( masregerrs_count_all_default( NULL, FALSE ) ), ( unsigned ) 0, "ERRORS: %d ? %d" );
/* WARN( "app_uflags_and_b:%lx", app_uflags_and_b ); */

  EXAM( mastest_fds(  ), ifds, "ifds=%d ? %d" );
  rRET;
}
