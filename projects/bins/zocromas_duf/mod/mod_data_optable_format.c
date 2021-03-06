#include <stddef.h>

#include "duf_optimpl_class.h"

#include "duf_config_structs.h"
#include "duf_config_pointers.h"                                     /* duf_get_( .* )_pointer */

#include "duf_optable_def.h"
#include "duf_optimpl_enum.h"                                        /* duf_option_code_t ✗ */

#include "duf_mod_types.h"

#include "duf_ufilter_structs.h"

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
  .name = "format",
//DO_AT_STAGE( SETUP ),
  .xlist =                                                           /* */
  {
   {.o = {DO_Q( "format-name" ) /*              */ , DO_A_R /* */ } /*                                      */ , DO_CL( FILTER ) /*  */ ,
    /*      */ DO_OU( STR, format_names.gen ) /*                                                             */ , DO_H( format name ) /*     */ },
   {.o = {DO_Q( "list-format-name" ) /*         */ , DO_A_R /* */ } /*                                      */ , DO_CL( FILTER ) /*  */ ,
    /*      */ DO_OU( STR, format_names.list ) /*                                                            */ , DO_H( format name ) /*     */ },
   {.o = {DO_Q( "tree-format-name" ) /*         */ , DO_A_R /* */ } /*                                      */ , DO_CL( FILTER ) /*  */ ,
    /*      */ DO_OU( STR, format_names.tree ) /*                                                            */ , DO_H( format name ) /*     */ },
   {.o = {DO_Q( "pack-format-name" ) /*         */ , DO_A_R /* */ } /*                                      */ , DO_CL( FILTER ) /*  */ ,
    /*      */ DO_OU( STR, format_names.pack ) /*                                                            */ , DO_H( format name ) /*     */ },
   {.o = {DO_Q( "dirs-format-name" ) /*         */ , DO_A_R /* */ } /*                                      */ , DO_CL( FILTER ) /*  */ ,
    /*      */ DO_OU( STR, format_names.dirs ) /*                                                            */ , DO_H( format name ) /*     */ },
   {.o = {DO_Q( "tree-prefix-format-name" ) /*  */ , DO_A_R /* */ } /*                                      */ , DO_CL( FILTER ) /*  */ ,
    /*      */ DO_OU( STR, format_names.tree_prefix ) /*                                                     */ , DO_H( format name ) /*     */ },

 /* {.o = {DO_Q( "bformat" ) (*                  *) , DO_A_R (* *) , DO_V( BFORMAT )} (*                     *) , DO_CL( PRINT ) (*   *)              */
 /*  (*                                                                                                       *) , DO_H( print w / bformat ) (* *) }, */
#if 0
   {.o = {DO_Q( "format-dirs-tree" ) /*         */ , DO_A_R /* */ , DO_V( SFORMAT_DIRS_TREE )} /*           */ , DO_CL( PRINT ) /* */ ,
    /*      */ DO_OC( STR, opt.output.sformat.dirs_tree ) /*                                                 */ , DO_H( format ) /*          */ },
   {.o = {DO_Q( "format-files-tree" ) /*        */ , DO_A_R /* */ , DO_V( SFORMAT_FILES_TREE )} /*          */ , DO_CL( PRINT ) /* */ ,
    /*      */ DO_OC( STR, opt.output.sformat.files_tree ) /*                                                */ , DO_H( format ) /*          */ },

   {.o = {DO_Q( "format-prefix-gen-tree" ) /*   */ , DO_A_R /* */ , DO_V( SFORMAT_PREFIX_GEN_TREE )} /*     */ , DO_CL( PRINT ) /* */ ,
    /*      */ DO_OC( STR, opt.output.sformat.prefix_gen_tree ) /*                                           */ , DO_H( format ) /*          */ },
   {.o = {DO_Q( "format-prefix-files-tree" ) /* */ , DO_A_R /* */ , DO_V( SFORMAT_PREFIX_FILES_TREE )} /*   */ , DO_CL( PRINT ) /* */ ,
    /*      */ DO_OC( STR, opt.output.sformat.prefix_files_tree ) /*                                         */ , DO_H( format ) /*          */ },
   {.o = {DO_Q( "format-prefix-dirs-tree" ) /*  */ , DO_A_R /* */ , DO_V( SFORMAT_PREFIX_DIRS_TREE )} /*    */ , DO_CL( PRINT ) /* */ ,
    /*      */ DO_OC( STR, opt.output.sformat.prefix_dirs_tree ) /*                                          */ , DO_H( format ) /*          */ },
#endif
   {.o = {DO_Q( "format-dirs-list" ) /*         */ , DO_A_R /* */ , DO_V( SFORMAT_DIRS_LIST )} /*           */ , DO_CL( PRINT ) /* */ ,
    /*      */ DO_OC( STR, opt.output.sformat.dirs_list ) /*                                                 */ , DO_H( format ) /*          */ },
   {.o = {DO_Q( "format-files-list" ) /*        */ , DO_A_R /* */ , DO_V( SFORMAT_FILES_LIST )} /*          */ , DO_CL( PRINT ) /* */ ,
    /*      */ DO_OC( STR, opt.output.sformat.files_list ) /*                                                */ , DO_H( format ) /*          */ },

   {.o = {DO_Q( "format-list-files" ) /*        */ , DO_A_R /* */ , DO_V( ASFORMAT_LIST_FILES )} /*         */ , DO_CL( PRINT ) /* */ ,
    /*      */ DO_OC( ARGV, opt.output.as_formats.list.files ) /*                                            */ , DO_H( format ) /*          */ },
   {.o = {DO_Q( "format-list-dirs" ) /*         */ , DO_A_R /* */ , DO_V( ASFORMAT_LIST_DIRS )} /*          */ , DO_CL( PRINT ) /* */ ,
    /*      */ DO_OC( ARGV, opt.output.as_formats.list.dirs ) /*                                             */ , DO_H( format ) /*          */ },

   {.o = {DO_Q( "format-pack-files" ) /*        */ , DO_A_R /* */ , DO_V( ASFORMAT_LIST_FILES )} /*         */ , DO_CL( PRINT ) /* */ ,
    /*      */ DO_OC( ARGV, opt.output.as_formats.pack.files ) /*                                            */ , DO_H( format ) /*          */ },
   {.o = {DO_Q( "format-pack-dirs" ) /*         */ , DO_A_R /* */ , DO_V( ASFORMAT_LIST_DIRS )} /*          */ , DO_CL( PRINT ) /* */ ,
    /*      */ DO_OC( ARGV, opt.output.as_formats.pack.dirs ) /*                                             */ , DO_H( format ) /*          */ },

   {.o = {DO_Q( "format-dirs-gen" ) /*              */ , DO_A_R /* */ , DO_V( SFORMAT_DIRS_GEN )} /*        */ , DO_CL( PRINT ) /* */ ,
    /*      */ DO_OC( STR, opt.output.sformat.dirs_gen ) /*                                                  */ , DO_H( format ) /*          */ },
   {.o = {DO_Q( "format-files-gen" ) /*             */ , DO_A_R /* */ , DO_V( SFORMAT_FILES_GEN )} /*       */ , DO_CL( PRINT ) /* */ ,
    /*      */ DO_OC( STR, opt.output.sformat.files_gen ) /*                                                 */ , DO_H( format ) /*          */ },

   {.o = {DO_Q( "format-tree-files" ) /*        */ , DO_A_R /* */ , DO_V( ASFORMAT_TREE_FILES )} /*         */ , DO_CL( PRINT ) /* */ ,
    /*      */ DO_OC( ARGV, opt.output.as_formats.tree.files ) /*                                            */ , DO_H( format ) /*          */ },
   {.o = {DO_Q( "format-tree-dirs" ) /*         */ , DO_A_R /* */ , DO_V( ASFORMAT_TREE_DIRS )} /*          */ , DO_CL( PRINT ) /* */ ,
    /*      */ DO_OC( ARGV, opt.output.as_formats.tree.dirs ) /*                                             */ , DO_H( format ) /*          */ },
   {.o = {DO_Q( "format-gen-files" ) /*         */ , DO_A_R /* */ , DO_V( ASFORMAT_GEN_FILES )} /*          */ , DO_CL( PRINT ) /* */ ,
    /*      */ DO_OC( ARGV, opt.output.as_formats.gen.files ) /*                                             */ , DO_H( format ) /*          */ },
   {.o = {DO_Q( "format-gen-dirs" ) /*          */ , DO_A_R /* */ , DO_V( ASFORMAT_GEN_DIRS )} /*           */ , DO_CL( PRINT ) /* */ ,
    /*      */ DO_OC( ARGV, opt.output.as_formats.gen.dirs ) /*                                              */ , DO_H( format ) /*          */ },

   {.o = {DO_Q( "max-width" ) /*        */ , DO_A_R /* */ , DO_V( MAXWIDTH )} /*                            */ , DO_CL( NODESC ) /*   */ ,
    /*      */ DO_OC( NUM, opt.output.max_width ) /* */ , DO_AT_STAGE( SETUP ) /*                            */ , DO_H(  .... ) /*           */ },

   {.o = {.name = NULL}}
   }
};

/* vi: et ft=c colorcolumn=3,52,67,68,112,113,131,145,146
*/
