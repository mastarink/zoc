#include <stddef.h>

#include "duf_optimpl_class.h"

#include "duf_config_structs.h"
#include "duf_config_pointers.h"                                     /* duf_get_( .* )_pointer */

#include "duf_optable_def.h"
#include "duf_optimpl_enum.h"                                        /* duf_option_code_t ✗ */

#include "duf_mod_types.h"

/*
At duf_xtended_table.c:
  1. extern const muc_longval_extended_table_t optable_main[];
  2.  static const muc_longval_extended_table_t *_lo_extended_multi[] = {
         ...
	optable_main,
	 ...

	NULL
      };
*/
static muc_longval_extended_table_t optable;

const duf_mod_handler_t duf_mod_handler_uni[] = {
  {"optab", &optable},
  {NULL, NULL}
};

static muc_longval_extended_table_t optable = {
  .name = "io",
  .xlist =                                                           /* */
  {

   {.o = {DO_Q( "output-level" ) /*     */ , DO_A_O /* */ , DO_V( OUTPUT_LEVEL )} /*       */ , DO_CL( PRINT ) /*   */ ,
    /*      */ DO_OC( NUM, opt.output.stream.level ) /*        */ , DO_AT_STAGE( SETUP ) /*        */ , DO_H( output level ) /*              */ },
   {.o = {DO_Q( "output-header-tty" ) /**/, DO_A_O /* */ , DO_V( OUTPUT_HEADER_TTY )} /*  */ , DO_CL( PRINT ) /*   */ ,
    /* */ DO_OC( STR, opt.output.stream.header_tty ) /*       */ , DO_AT_STAGE( SETUP ) /*         */ , DO_HQ( "output header for tty" ) /*  */ },

   {.o = {DO_Q( "output-file" ) /*      */ , DO_A_R /* */ , DO_V( OUTPUT_FILE )} /*        */ , DO_CL( PRINT ) /*   */ ,
    /*      */ DO_OC( FILE, opt.output.stream ) /*      */ , DO_AT_STAGE( SETUP ) /*        */ , DO_H( output to file ) /*                   */ },
   {.o = {DO_Q( "set-output-file" ) /*  */ , DO_A_R /* */ , DO_V( OUTPUT_FILE )} /*        */ , DO_CL( PRINT ) /*   */ ,
    /*    */ DO_OC( FILE, opt.output.stream ) /* */ , DO_SET_STAGE( FIRST, INTERACTIVE ) /* */ , DO_H( output to file ) /*                   */ },

   {.o = {DO_Q( "output-stderr" ) /*    */ , DO_A_N /* */ , DO_V( OUTPUT_STDERR )} /*      */ , DO_CL( PRINT ) /* */
    ,.call = {.value = {.u = 2}}
    , /* */ DO_OC( FILE, opt.output.stream ), DO_AT_STAGE( SETUP ) /*                       */ , DO_H( output to stderr ) /*                 */ },
   {.o = {DO_Q( "set-output-stderr" ) /* */ , DO_A_N /* */ , DO_V( OUTPUT_STDERR )} /*     */ , DO_CL( PRINT ) /* */
    ,.call = {.value = {.u = 2}}
    , /* */ DO_OC( FILE, opt.output.stream ), DO_SET_STAGE( FIRST, INTERACTIVE ) /*         */ , DO_H( output to stderr ) /*                 */ },

   {.o = {DO_Q( "output-stdout" ) /*    */ , DO_A_N /* */ , DO_V( OUTPUT_STDOUT )} /*      */ , DO_CL( PRINT ) /* */
    ,.call = {.value = {.u = 1}}
    , /* */ DO_OC( FILE, opt.output.stream ), DO_AT_STAGE( SETUP ) /*                       */ , DO_H( output to stdout ) /*                 */ },
   {.o = {DO_Q( "set-output-stdout" ) /*    */ , DO_A_N /* */ , DO_V( OUTPUT_STDOUT )} /*  */ , DO_CL( PRINT ) /* */
    ,.call = {.value = {.u = 1}}
    , /* */ DO_OC( FILE, opt.output.stream ), DO_SET_STAGE( FIRST, INTERACTIVE ) /*         */ , DO_H( output to stdout ) /*                 */ },

   {.o = {.name = NULL}
    }
   }
};

/* vi: et ft=c colorcolumn=3,44,59,60,95,96,123,145,146
*/
