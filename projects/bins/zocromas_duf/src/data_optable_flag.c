#include <stddef.h>

#include "duf_maintenance.h"

#include "duf_optable_def.h"
#include "duf_options_enum.h"

/*
At duf_options_table.c:
  1. extern const duf_longval_extended_table_t optable_flag[];
  2.  static const duf_longval_extended_table_t *_lo_extended_multi[] = {
         ...
	optable_flag,
	 ...

	NULL
      };
*/


const duf_longval_extended_table_t optable_flag = {
  .name = "flag",
  .table =                      /* */
  {
#if 0
   /* SEE --disable-memusage */
   {.o = {DO_Q( "memusage" ) /*          */ , DO_A_N /* */ , DO_V( MEMUSAGE )} /*   */ , DO_CL( DEBUG ) /*   */ , DO_H( debug ) /*        */ },
#endif
   {.o = {DO_Q( "create-database" ) /*  */ , DO_A_N /* */ , DO_VF( CREATE_DB )} /*         */ , DO_CL( SYSTEM ) /*  */ ,
    /*      */ DO_OC( FLAG, opt.act.v ), DO_FL( act, create_database ) /*                   */ , DO_H( create db ) /*                        */ },
   {.o = {DO_Q( "create-tables" ) /*    */ , DO_A_N /* */ , DO_VF( CREATE_TABLES )} /*     */ , DO_CL( SYSTEM ) /*  */ ,
    /*      */ DO_OC( FLAG, opt.act.v ), DO_FL( act, create_tables ) /*                     */ , DO_H( create tables ) /*                    */ },
   {.o = {DO_Q( "drop-tables" ) /*      */ , DO_A_N /* */ , DO_VF( DROP_TABLES )} /*       */ , DO_CL( SYSTEM ) /*  */ ,
    /*      */ DO_OC( FLAG, opt.act.v ), DO_FL( act, drop_tables ) /*                       */ , DO_H( drop tables ) /*                      */ },
   {.o = {DO_Q( "clean-tables" ) /*     */ , DO_A_N /* */ , DO_VF( CLEAN_TABLES )} /*      */ , DO_CL( SYSTEM ) /*  */ ,
    /*      */ DO_OC( FLAG, opt.act.v ), DO_FL( act, clean_tables ) /*                      */ , DO_H( clean tables ) /*                     */ },

   {.o = {DO_Q( "quit" ) /*             */ , DO_A_N /* */ , DO_VF( QUIT )} /*              */ , DO_CL( CONTROL ) /*    */ ,
    /*      */ DO_OC( NOFLAG, opt.act.v ), DO_FL( act, interactive ) /*                    */ , DO_H( quit ) /*                              */ },
   {.o = {DO_Q( "add-path" ) /*         */ , DO_A_N /* */ , DO_VF( ADD_PATH )} /*          */ , DO_CL( CONTROL ) /* */ ,
    /*      */ DO_OC( FLAG, opt.act.v ), DO_FL( act, add_path ) /*                          */ , DO_H( reg.init path ) /*                    */ },
   {.o = {DO_Q( "interactive" ) /*      */ , DO_A_N /* */ , DO_VF( INTERACTIVE )} /*       */ , DO_CL( CONTROL ) /* */ ,
    /*      */ DO_OC( FLAG, opt.act.v ), DO_FL( act, interactive ) /*                       */ , DO_H(  ... ) /*                             */ },


   {.o = {DO_Q( "force-color" ) /*      */ , DO_A_N /* */ , DO_VF( FORCE_COLOR )} /*       */ , DO_CL( CONTROL ) /* */ ,
    /* */ DO_OC( FLAG, opt.act.v ), DO_FL( act, force_color ), DO_STAGE_ALL /*              */ , DO_H(  ... ) /*                             */ },
   {.o = {DO_Q( "color" ) /*             */ , DO_A_N /* */ , DO_VF( NOCOLOR )} /*        */ , DO_CL( CONTROL ) /* */ ,
    /*      */ DO_OC( NOFLAG, opt.act.v ), DO_FL( act, nocolor ) /*                           */ , DO_H(  ... ) /*                             */ },


   {.o = {DO_Q( "fast" ) /*             */ , DO_A_N /* */ , DO_VF( FAST )} /*              */ , DO_CL( CONTROL ) /* */ ,
    /*      */ DO_OC( FLAG, opt.act.v ), DO_FL( act, fast ) /*                              */ , DO_H(  ... ) /*                             */ },
#if 0
   {.o = {DO_Q( "selection" ) /*        */ , DO_A_N /* */ , DO_VF( BEGINNING_SQL ) /* */ } /* */ , DO_CL( CONTROL ) /*   */ ,
    /*      */ DO_OC( FLAG, opt.act.v ), DO_FL( act, beginning_sql ), DO_STAGE_SPLS /*        */ , DO_H(  ... ) /*                            */ },
#endif
   {.o = {DO_Q( "recursive" ) /*        */ , DO_A_N /* */ , DO_VF( RECURSIVE )} /*         */ , DO_CL( CONTROL ) /* */ ,
    /*      */ DO_OU( SFLAG, v ), DO_FL( rec_etc, recursive ), DO_STAGE_SPLS /*              */ , DO_H( recursive ) /*                        */ },
   {.o = {DO_Q( "linear" ) /*        */ , DO_A_N /* */ , DO_VF( LINEAR )} /*         */ , DO_CL( CONTROL ) /* */ ,
    /*      */ DO_OU( SFLAG, v ), DO_FL( rec_etc, linear ), DO_STAGE_SPLS /*              */ , DO_H( recursive ) /*                        */ },
#if 0
   {.o = {DO_Q( "if-recursive" ) /*     */ , DO_A_N} /*                                    */ , DO_CL( CONTROL ) /* */ ,
    /*      */ DO_OU( PSFLAG, v ), DO_FL( rec_etc, recursive ) /*                           */ , DO_H( recursive ) /*                        */ },
#endif



   {.o = {DO_Q( "dirent" ) /*           */ , DO_A_N /* */ , DO_VF( DIRENT )} /*            */ , DO_CL( COLLECT ) /* */ ,
    /*      */ DO_OC( FLAG, opt.act.v ), DO_FL( act, dirent ) /*                            */ , DO_H( dirent ) /*                           */ },

   /* 20150907.122946 renames directories --> allow-directories */
   {.o = {DO_Q( "allow-directories" ) /* */ , DO_A_N /* */ , DO_VF( ALLOW_DIRECTORIES )} /**/, DO_CL( COLLECT ) /* */ ,
    /*      */ DO_OC( FLAG, opt.act.v ), DO_FL( act, allow_dirs ), DO_STAGE_SPLS /*                */ ,
    DO_H( collect dir info ) /*                 */ },
   /* 20150907.122956 renames dirs --> allow-dirs */
   {.o = {DO_Q( "allow-dirs" ) /*       */ , DO_A_N /* */ , DO_VF( ALLOW_DIRECTORIES )} /* */ , DO_CL( COLLECT ) /* */ ,
    /*      */ DO_OC( FLAG, opt.act.v ), DO_FL( act, allow_dirs ) /*                        */ , DO_H( get dir info ) /*                     */ },
   {.o = {DO_Q( "allow-sub" ) /*       */ , DO_A_N /* */ , DO_VF( ALLOW_SUB )} /* */ , DO_CL( COLLECT ) /* */ ,
    /*      */ DO_OC( FLAG, opt.act.v ), DO_FL( act, allow_sub ) /*                        */ , DO_H( get sub info ) /*                      */ },
   /* renames files --> allow-files */
   {.o = {DO_Q( "allow-files" ) /*            */ , DO_A_N /* */ , DO_VF( ALLOW_FILES )} /*             */ , DO_CL( COLLECT ) /* */ ,
    /*      */ DO_OC( FLAG, opt.act.v ), DO_FL( act, allow_files ), DO_STAGE_SPLS /*         */ , DO_H( get file info ) /*                    */ },


   {.o = {DO_Q( "info" ) /*             */ , DO_A_N /* */ , DO_VF( INFO )} /*              */ , DO_CL( REFERENCE ) /* */ ,
    /*      */ DO_OC( FLAG, opt.act.v ), DO_FL( act, info ) /*                              */ , DO_H( db info ) /*                          */ },
   {.o = {DO_Q( "information" ) /*      */ , DO_A_N /* */ , DO_VF( INFO )} /*              */ , DO_CL( REFERENCE ) /* */ ,
    /*      */ DO_OC( FLAG, opt.act.v ), DO_FL( act, info ) /*                              */ , DO_H( db info ) /*                          */ },
   {.o = {DO_Q( "dry-run" ) /*          */ , DO_A_N /* */ , DO_VF( DRY_RUN )} /*           */ , DO_CL( DEBUG ) /*   */ ,
    /*      */ DO_OC( FLAG, opt.v ), DO_FL( opt, dry_run ) /*                               */ , DO_H( dry ) /*                              */ },


   {.o = {DO_Q( "use-binformat" ) /*        */ , DO_A_N /* */ , DO_VF( USE_BINFORMAT )} /* */ , DO_CL( PRINT ) /*   */ ,
    /*      */ DO_OC( FLAG, opt.act.v ), DO_FL( act, use_binformat ) /*                     */ , DO_H( use bin format ) /*                   */ },
#if 0
   {.o = {DO_Q( "print" ) /*             */ , DO_A_N /* */ , DO_VF( PRINT )} /*            */ , DO_CL( PRINT ) /*   */ ,
    /*      */ DO_OC( FLAG, opt.act.v ), DO_FL( act, print ) /*                             */ , DO_H( print ) /*                            */ },
   {.o = {DO_Q( "tree" ) /*              */ , DO_A_N /* */ , DO_VF( TREE )} /*             */ , DO_CL( PRINT ) /*   */ ,
    /*      */ DO_OC( FLAG, opt.act.v ), DO_FL( act, tree ) /*                              */ , DO_H( tree ) /*                             */ },
#endif
   {.o = {DO_Q( "progress" ) /*         */ , DO_A_N /* */ , DO_VF( PROGRESS )} /*          */ , DO_CL( REFERENCE ) /* */ ,
    /*      */ DO_OC( FLAG, opt.act.v ), DO_FL( act, progress ) /*                          */ , DO_H( progress ) /*                         */ },
   {.o = {DO_Q( "remove-database" ) /*  */ , DO_A_N /* */ , DO_VF( REMOVE_DATABASE )} /*   */ , DO_CL( SYSTEM ) /*      */ ,
    /*      */ DO_OC( FLAG, opt.act.v ), DO_FL( act, remove_database ) /*                   */ , DO_H(  .... ) /*                            */ },

   {.o = {DO_Q( "summary" ) /*          */ , DO_A_N /* */ , DO_VF( SUMMARY )} /*           */ , DO_CL( REFERENCE ) /* */ ,
    /*      */ DO_OC( FLAG, opt.act.v ), DO_FL( act, summary ) /*                 */ , DO_H( summary ) /*                          */ },


#if 0
   {.o = {DO_Q( "uni-scan" ) /*         */ , DO_A_N /* */ , DO_VF( UNI_SCAN )} /*          */ , DO_CL( OBSOLETE ) /* */ ,
    /*      */ DO_OC( FLAG, opt.act.v ), DO_FL( act, uni_scan ) /*                          */ , DO_H(  .... ) /*                            */ },
   {.o = {DO_Q( "nouni-scan" ) /*       */ , DO_A_N /* */ , DO_VF( UNI_SCAN )} /*          */ , DO_CL( OBSOLETE ) /* */ ,
    /*      */ DO_OC( FLAG, opt.act.v ), DO_FL( act, uni_scan ),.invert = 1 /*              */ , DO_H(  .... ) /*                            */ },
#endif

#if 0
   {.o = {DO_Q( "sccbs" ) /*            */ , DO_A_N /* */ , DO_VF( DO_SCCBS )} /*      */ ,
    /*      */ DO_OC( FLAG, opt.act.v ), DO_FL( act, do_sccbs ) /*                          */ , DO_H(  .... ) /*                            */ },
   {.o = {DO_Q( "nosccbs" ) /*          */ , DO_A_N /* */ , DO_VF( DO_SCCBS )} /*      */ ,
    /*      */ DO_OC( FLAG, opt.act.v ), DO_FL( act, do_sccbs ),.invert = 1 /*              */ , DO_H(  .... ) /*                            */ },
   {.o = {DO_Q( "show-sccbs" ) /*       */ , DO_A_N /* */ , DO_VF( SHOW_SCCBS )} /*      */ ,
    /*      */ DO_OC( FLAG, opt.act.v ), DO_FL( act, show_sccbs ) /*                        */ , DO_H(  .... ) /*                            */ },
   {.o = {DO_Q( "show-sccbh" ) /*       */ , DO_A_N /* */ , DO_VF( SHOW_SCCBH )} /*      */ ,
    /*      */ DO_OC( FLAG, opt.act.v ), DO_FL( act, show_sccbh ) /*                        */ , DO_H(  .... ) /*                            */ },
#endif
   {.o = {DO_Q( "vacuum" ) /*           */ , DO_A_N /* */ , DO_VF( VACUUM )} /*            */ , DO_CL( SYSTEM ) /*  */ ,
    /*      */ DO_OC( FLAG, opt.act.v ), DO_FL( act, vacuum ) /*                            */ , DO_H( vacuum db ) /*                        */ },


   {.o = {.name = NULL}}
   }
};

/* vi: ft=c colorcolumn=3,44,59,60,95,96,123,145,146
*/
