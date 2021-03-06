#include <stddef.h>

#include "duf_optimpl_class.h"

#include "duf_config_structs.h"
#include "duf_config_pointers.h"                                     /* duf_get_( .* )_pointer */

#include "duf_optable_def.h"
#include "duf_optimpl_enum.h"                                        /* duf_option_code_t ✗ */

/* #include "duf_optimpl_sccb.h"                                        (* TODO : temp for duf_optimpl_O_db_open               *) */

#include "duf_mod_types.h"

/*
At duf_xtended_table.c:
  1. extern const muc_longval_extended_table_t optable_str[];
  2.  static const muc_longval_extended_table_t *_lo_extended_multi[] = {
         ...
	optable_str,
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
  .name = "str",
  .xlist =                                                           /* */
  {

   {.o = {DO_Q( "tag-file" ) /*         */ , DO_A_R /* */ , DO_V( TAG_FILE )} /*           */ , DO_CL( CONTROL ) /* */ ,
    /*      */ DO_OC( STR, vars.tag.file ) /*                                               */ , DO_H( tag file ) /*                         */ },
   {.o = {DO_Q( "tag-dir" ) /*          */ , DO_A_R /* */ , DO_V( TAG_DIR )} /*            */ , DO_CL( CONTROL ) /* */ ,
    /*      */ DO_OC( STR, vars.tag.dir ) /*                                                */ , DO_H( tag dir ) /*                          */ },

   {.o = {DO_Q( "save-directory" ) /*   */ , DO_A_R /* */ , DO_V( SAVE_DIRECTORY )} /*     */ , DO_CL( CONTROL ) /* */ ,
    /*      */ DO_OC( STR, save.path ) /*                                                   */ , DO_H( save path ) /*                        */ },

   {.o = {DO_Q( "config-dir" ) /*      */ , DO_A_R /* */ , DO_V( CONFIG_DIR )} /*          */ , DO_CL( CONTROL ) /*  */ ,
    /*      */ DO_OC( STR, conf.config_dir ), DO_AT_STAGE( PRESETUP ) /*                    */ , DO_H( config directory ) /*                 */ },
   {.o = {DO_Q( "cmds-dir" ) /*      */ , DO_A_R /* */ , DO_V( CMDS_DIR )} /*              */ , DO_CL( CONTROL ) /*  */ ,
    /*      */ DO_OC( STR, conf.cmds_dir ), DO_AT_STAGE( SETUP ) /*                         */ , DO_H( config directory ) /*                 */ },

   {.o = {.name = NULL}}
   }
};

/* vi: ft=c colorcolumn=3,44,59,60,95,96,123,145,146
*/
