#include <stddef.h>

#include "duf_optimpl_class.h"

#include "duf_config_structs.h"
#include "duf_config_pointers.h"                                     /* duf_get_( .* )_pointer */

#include "duf_optable_def.h"
#include "duf_optimpl_enum.h"                                        /* duf_option_code_t ✗ */

#include "duf_mod_types.h"

/*
At duf_xtended_table.c:
  1. extern const muc_longval_extended_table_t optable_debug[];
  2.  static const muc_longval_extended_table_t *_lo_extended_multi[] = {
         ...
	optable_debug,
	 ...

	NULL
      };
*/

const muc_longval_extended_table_t optable_debug = {
  .name = "debug",
  .xlist =                                                           /* */
  {
#if 0
   {.o = {DO_Q( "min-dbg-lines" ) /*    */ , DO_A_R /* */ , DO_V( MIN_DBGLINE )} /*        */ , DO_CL( DEBUG ) /*   */ ,
    /*      */ DO_OC( NUM, opt.dbg.min_line ) /*                                            */ , DO_H(  ... ) /*                             */ },
   {.o = {DO_Q( "max-dbg-lines" ) /*    */ , DO_A_R /* */ , DO_V( MAX_DBGLINE )} /*        */ , DO_CL( DEBUG ) /*   */ ,
    /*      */ DO_OC( NUM, opt.dbg.max_line ) /*                                            */ , DO_H(  ... ) /*                             */ },
   {.o = {DO_Q( "debug" ) /*            */ , DO_A_O /* */ , DO_V( DEBUG )} /*              */ , DO_CL( DEBUG ) /*   */ ,
    /*      */ DO_OC( UPLUS, opt.dbg.debug ) /*                                             */ , DO_H( debug ) /*                            */ },
#endif
 /* {.o = {DO_Q( "verbose" ) (*          *) , DO_A_O (* *) , DO_V( VERBOSE )} (*            *) , DO_CL( DEBUG ) (*   *) ,                           */
 /*  (*      *) DO_OC( UPLUS, opt.dbg.verbose ), DO_SET_STAGE( BOOT, PRESETUP ) (*           *) , DO_H( verbose ) (*                          *) }, */

 /*  DO_V / DOO_V is necessary for short opt's */
   {DOO_Q( "verbose" ) /*                  */ , DOO_A_O /* */ , DOO_V( VERBOSE ) /*          */ , DOO_CL( DEBUG ) /*  */ ,
    /*   */ DOO_OC( UPLUS, opt.flow.verbose ) /* */ , DO_SET_STAGE( BOOT, PRESETUP ) /*          */ ,
    DOO_H( verbose ) /*                         */ },

   {.o = {.name = NULL}}
   }
};

/* vi: ft=c colorcolumn=3,44,59,60,95,96,123,145,146
*/
