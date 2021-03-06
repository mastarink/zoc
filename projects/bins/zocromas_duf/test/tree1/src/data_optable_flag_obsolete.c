#include <stddef.h>
#include "duf_config_types.h"
#include "duf_ufilter_types.h"
#include "duf_levinfo_types.h"

#include "duf_optable_def.h"
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


const duf_longval_extended_table_t optable_flag_obsolete = {
  .table =                      /* */
  {



   {.o = {DO_Q( "md5" ) /*              */ , DO_A_N /* */ , DO_VF( MD5 )} /*               */ , DO_CL( SCAN ) /*    */ ,
    /*      */ DO_OC( FLAG, cli.act.v ), DO_FL( act, md5_obs ) /*                               */ , DO_H( collect md5 data ) /*                 */ },
   {.o = {DO_Q( "mdpath" ) /*           */ , DO_A_N /* */ , DO_VF( MDPATH )} /*            */ , DO_CL( SCAN ) /*    */ ,
    /*      */ DO_OC( FLAG, cli.act.v ), DO_FL( act, mdpath_obs ) /*                            */ , DO_H(  ... ..... ) /*                     */ },

   {.o = {DO_Q( "mime" ) /*             */ , DO_A_N /* */ , DO_VF( MIME )} /*              */ , DO_CL( SCAN ) /*    */ ,
    /*      */ DO_OC( FLAG, cli.act.v ), DO_FL( act, mime_obs ) /*                              */ , DO_H( collect mime data ) /*              */ },

   {.o = {DO_Q( "filedata" ) /*         */ , DO_A_N /* */ , DO_VF( FILEDATA )} /*          */ , DO_CL( COLLECT ) /* */ ,
    /*      */ DO_OC( FLAG, cli.act.v ), DO_FL( act, filedata ) /*                          */ , DO_H( get filedata ) /*                     */ },
   {.o = {DO_Q( "filenames" ) /*        */ , DO_A_N /* */ , DO_VF( FILENAMES )} /*         */ , DO_CL( COLLECT ) /* */ ,
    /*      */ DO_OC( FLAG, cli.act.v ), DO_FL( act, filenames ) /*                         */ , DO_H( get filenames ) /*                    */ },

   {.o = {DO_Q( "exif" ) /*             */ , DO_A_N /* */ , DO_VF( EXIF )} /*              */ , DO_CL( SCAN ) /*    */ ,
    /*      */ DO_OC( FLAG, cli.act.v ), DO_FL( act, exif_obs ) /*                          */ , DO_H( collect exif data ) /*                */ },

   {.o = {DO_Q( "sd5" ) /*              */ , DO_A_N /* */ , DO_VF( SD5 )} /*               */ , DO_CL( SCAN ) /*    */ ,
    /*      */ DO_OC( FLAG, cli.act.v ), DO_FL( act, sd5_obs ) /*                           */ , DO_H( collect sd5 data ) /*                 */ },

   {.o = {DO_Q( "crc32" ) /*            */ , DO_A_N /* */ , DO_VF( CRC32 )} /*             */ , DO_CL( SCAN ) /*    */ ,
    /*      */ DO_OC( FLAG, cli.act.v ), DO_FL( act, crc32_obs ) /*                         */ , DO_H( collect crc32 ) /*                    */ },

   {.o = {.name = NULL}}
   }
};

/* vi: ft=c colorcolumn=3,44,59,60,95,96,123,145,146
*/

