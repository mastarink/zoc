#include <stddef.h>

#include "duf_optimpl_class.h"

#include "duf_config_structs.h"
#include "duf_config_pointers.h"                                     /* duf_get_( .* )_pointer */

#include "duf_optable_def.h"
#include "duf_optimpl_enum.h"                                        /* duf_option_code_t ✗ */

#include "duf_mod_types.h"

/*
At duf_xtended_table.c:
  1. extern const muc_longval_extended_table_t optable_sflag[];
  2.  static const muc_longval_extended_table_t *_lo_extended_multi[] = {
         ...
	optable_sflag,
	 ...

	NULL
      };
*/

const muc_longval_extended_table_t optable_sflag = {
  .name = "sflag",
  .xlist =                                                           /* */
  {
   {.o = {DO_Q( "disable-calculate" ) /* */ , DO_A_N, DO_VF( DISABLE_CALCULATE )} /*       */ , DO_CL( COLLECT ) /*    */ ,
    /*      */ DO_OC( BSFLAG, opt.disable ) /*           */ , DO_BFL( disable, calculate ) /* */ , DO_H(  .... ) /*                            */ },
   {.o = {DO_Q( "disable-insert" ) /*   */ , DO_A_N, DO_VF( DISABLE_INSERT )} /*           */ , DO_CL( COLLECT ) /*    */ ,
    /*      */ DO_OC( BSFLAG, opt.disable ) /*           */ , DO_BFL( disable, insert ) /*    */ , DO_H(  .... ) /*                            */ },
   {.o = {DO_Q( "disable-update" ) /*   */ , DO_A_N, DO_VF( DISABLE_UPDATE )} /*           */ , DO_CL( COLLECT ) /*    */ ,
    /*      */ DO_OC( BSFLAG, opt.disable ) /*           */ , DO_BFL( disable, update ) /*    */ , DO_H(  .... ) /*                            */ },
   {.o = {DO_Q( "disable-fs" ) /*       */ , DO_A_N /* */ , DO_VF( DISABLE_FS )} /*        */ , DO_CL( FS ) /* */ , DO_STAGE_SPLS /* */ ,
    /*      */ DO_OC( BSFLAG, opt.disable ) /*           */ , DO_BFL( disable, fs ) /*        */ , DO_H( disable_fs ) /*                       */ },

   {.o = {DO_Q( "disable-memusage" ) /*       */ , DO_A_N /* */ , DO_VF( DISABLE_MEMUSAGE )} /*  */ , DO_CL( CONTROL ) /* */ , DO_STAGE_SPLS /* */ ,
    /*      */ DO_OC( BSFLAG, opt.disable ) /*           */ , DO_BFL( disable, memusage ) /*  */ , DO_H( disable_memusage ) /*                 */ },

   {.o = {.name = NULL}}
   }
};

/* vi: ft=c colorcolumn=3,44,59,60,95,96,123,145,146
*/
