#include <stddef.h>

#include "duf_optimpl_class.h"

#include "duf_config_structs.h"
#include "duf_config_pointers.h"                                     /* duf_get_( .* )_pointer */

#include "duf_optable_def.h"
#include "duf_optimpl_enum.h"                                        /* duf_option_code_t ✗ */

#include "duf_optimpl_help.h"
#include "duf_optimpl_example.h"
#include "duf_optimpl_oclass_help.h"
#include "duf_optimpl_set_help.h"

#include "duf_mod_types.h"

/*
 * At duf_xtended_table.c:
 * 1. extern const muc_longval_extended_table_t optable_help[];
 * 2.  static const muc_longval_extended_table_t *_lo_extended_multi[] = {
 *    ...
 *     optable_help,
 *    ...
 *   NULL
 *  };
*/
static muc_longval_extended_table_t optable;

const duf_mod_handler_t duf_mod_handler_uni[] = {
  {"optab", &optable},
  {NULL, NULL}
};

static muc_longval_extended_table_t optable = {
  .name = "help",
  DO_AT_STAGE( SETUP ), DO_STG_NOT( FIRST ),
  .xlist =                                                           /* */
  {

   {.o = {DO_Q( "help-class" ) /*   */ , DO_A_R /*  */ } /*       */ , DO_CL( HELP ) /*       */ ,
    /*      */ DO_S_CALL( oclass_help_str ) /*             */ , DO_H( help on ... ) /*                      */ },
   {.o = {DO_Q( "help-class-all" ) /*   */ , DO_A_N /*  */ , DO_V( HELP_CL_ALL )} /*       */ , DO_CL( HELP ) /*       */ ,
    /*      */ DO_EIA_CALLCL( oclass_help_all, ALL ) /*             */ , DO_H( help on all ) /*                      */ },
   {.o = {DO_Q( "help-class-collect" ) /**/, DO_A_N /*  */ , DO_V( HELP_CL_COLLECT )} /*   */ , DO_CL( HELP ) /*       */ ,
    /*      */ DO_EIA_CALLCL( oclass_help, COLLECT ) /*             */ , DO_H( help on collect ) /*                  */ },
   {.o = {DO_Q( "help-class-filter" ) /* */ , DO_A_N /* */ , DO_V( HELP_CL_FILTER )} /*    */ , DO_CL( HELP ) /*       */ ,
    /*      */ DO_EIA_CALLCL( oclass_help, FILTER ) /*              */ , DO_H( help on collect ) /*                  */ },
   {.o = {DO_Q( "help-class-control" ) /**/, DO_A_N /*  */ , DO_V( HELP_CL_CONTROL )} /*   */ , DO_CL( HELP ) /*       */ ,
    /*      */ DO_EIA_CALLCL( oclass_help, CONTROL ) /*             */ , DO_H( help on control ) /*                  */ },
   DO_CMD( "help-class-db", N, HELP_CL_DB, DO_CL( HELP ), DO_EIA_CALLCL( oclass_help, DB ), DO_H( help on db control ) ),
   DO_CMD( "help-class-fs", N, HELP_CL_FS, DO_CL( HELP ), DO_EIA_CALLCL( oclass_help, FS ), DO_H( help on fs control ) ),
   {.o = {DO_Q( "help-class-debug" ) /* */ , DO_A_N /*  */ , DO_V( HELP_CL_DEBUG )} /*     */ , DO_CL( HELP ) /*       */ ,
    /*      */ DO_EIA_CALLCL( oclass_help, DEBUG ) /*               */ , DO_H( help on debug ) /*                    */ },
   {.o = {DO_Q( "help-class-help" ) /*  */ , DO_A_N /*  */ , DO_V( OCLASS_HELP )} /*        */ , DO_CL( HELP ) /*       */ ,
    /*      */ DO_EIA_CALLCL( oclass_help, HELP ) /*                */ , DO_H( help on help ) /*                     */ },
   {.o = {DO_Q( "help-class-nodesc" ) /* */ , DO_A_N /* */ , DO_V( HELP_CL_NODESC )} /*    */ , DO_CL( HELP ) /*       */ ,
    /*      */ DO_EIA_CALLCL( oclass_help, NODESC ) /*              */ , DO_H( help on nodesc ) /*                   */ },
   {.o = {DO_Q( "help-class-obsolete" ) /**/, DO_A_N /* */ , DO_V( HELP_CL_OBSOLETE )} /*  */ , DO_CL( HELP ) /*       */ ,
    /*      */ DO_EIA_CALLCL( oclass_help, OBSOLETE ) /*            */ , DO_H( help on obsolete ) /*                 */ },
   {.o = {DO_Q( "help-class-none" ) /*  */ , DO_A_N /*  */ , DO_V( HELP_CL_NONE )} /*      */ , DO_CL( HELP ) /*       */ ,
    /*      */ DO_EIA_CALLCL( oclass_help, NONE ) /*                */ , DO_H( help on none ) /*                     */ },
   {.o = {DO_Q( "help-class-print" ) /* */ , DO_A_N /*  */ , DO_V( HELP_CL_PRINT )} /*     */ , DO_CL( HELP ) /*       */ ,
    /*      */ DO_EIA_CALLCL( oclass_help, PRINT ) /*               */ , DO_H( help on print ) /*                    */ },
   {.o = {DO_Q( "help-class-reference" ), DO_A_N /*     */ , DO_V( HELP_CL_REFERENCE )} /* */ , DO_CL( HELP ) /*       */ ,
    /*      */ DO_EIA_CALLCL( oclass_help, REFERENCE ) /*           */ , DO_H( help on reference ) /*                */ },
   {.o = {DO_Q( "help-class-request" ) /**/, DO_A_N /*  */ , DO_V( HELP_CL_REQUEST )} /*   */ , DO_CL( HELP ) /*       */ ,
    /*      */ DO_EIA_CALLCL( oclass_help, REQUEST ) /*              */ , DO_H( help on request ) /*                  */ },
   {.o = {DO_Q( "help-class-scan" ) /*  */ , DO_A_N /*  */ , DO_V( HELP_CL_SCAN )} /*      */ , DO_CL( HELP ) /*       */ ,
    /*      */ DO_EIA_CALLCL( oclass_help, SCAN ) /*                 */ , DO_H( help on scan ) /*                     */ },
   {.o = {DO_Q( "help-class-system" ) /* */ , DO_A_N /*  */ , DO_V( HELP_CL_SYSTEM )} /*   */ , DO_CL( HELP ) /*       */ ,
    /*      */ DO_EIA_CALLCL( oclass_help, SYSTEM ) /*               */ , DO_H( help on system ) /*                   */ },
   {.o = {DO_Q( "help-class-trace" ) /* */ , DO_A_N /*  */ , DO_V( HELP_CL_TRACE )} /*     */ , DO_CL( HELP ) /*       */ ,
    /*      */ DO_EIA_CALLCL( oclass_help, TRACE ) /*                */ , DO_H( help on trace ) /*                    */ },
   {.o = {DO_Q( "help-class-update" ) /* */ , DO_A_N /*  */ , DO_V( HELP_CL_UPDATE )} /*   */ , DO_CL( HELP ) /*       */ ,
    /*      */ DO_EIA_CALLCL( oclass_help, UPDATE ) /*               */ , DO_H( help on update ) /*                   */ },

#if 0
   {.o = {DO_Q( "help-set" ) /*         */ , DO_A_O /*  */ , DO_V( HELP_SET )} /*          */ , DO_CL( HELP ) /*       */ ,
    /*      */ DO_S_CALL( set_help ) /*                            */ , DO_H( help on set / optfile ) /*            */ },
#elif 1
 /* see help-set :: see list-options, list-extended   */
   DO_CMD( "help-set", O, HELP_SET, DO_CL( HELP ), DO_S_CALL( set_help ), DO_H( help on set / optfile ) ),
#endif

   {.o = {DO_Q( "help" ) /*              */ , DO_A_N /*  */ , DO_V( HELP )} /*             */ , DO_CL( HELP ) /*       */ ,
    /*      */ DO_EV_CALL( help ) /*                               */ , DO_H( show short options help ) /*                             */ },
   {.o = {DO_Q( "help-examples" ) /*    */ , DO_A_N /* */ , DO_V( EXAMPLES )} /*           */ , DO_CL( HELP ) /*      */ ,
    /*      */ DO_EV_CALL( examples ) /*                                                    */ ,
    DO_H( show short options examples ) /*                         */ },

   {.o = {.name = NULL}}
   }
};

/* vi: ft=c colorcolumn=3,44,59,60,95,96,123,145,146
*/
