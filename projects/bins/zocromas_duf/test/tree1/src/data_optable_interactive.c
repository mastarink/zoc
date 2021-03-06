#include <stddef.h>
#include "duf_config_types.h"
#include "duf_ufilter_types.h"
#include "duf_levinfo_types.h"

#include "duf_config_ref.h"
#include "duf_optable_def.h"

#include "duf_option_misc.h"

/*
At duf_options_table.c:
  1. extern const duf_longval_extended_table_t optable_interactive[];
  2.  static const duf_longval_extended_table_t *_lo_extended_multi[] = {
         ...
	optable_interactive,
	 ...

	NULL
      };
*/

extern duf_scan_callbacks_t duf_print_dir_callbacks /* __attribute( ( weak ) ) */ ;

/*
void duf_list_sccb( int ) __attribute__ ( ( weak ) );
void duf_list_targ( int *targc, char ***targv, long n ) __attribute__ ( ( weak ) );
void duf_add_targ( int *targc, char ***targv, const char *s ) __attribute__ ( ( weak ) );
void duf_clear_targ( int *targc, char ***targv, long n ) __attribute__ ( ( weak ) );
*/
const duf_longval_extended_table_t optable_interactive = {
  /* DO_AT_STAGE( DUF_OPTION_STAGE_INTERACTIVE ), */
  .table =                      /* */
  {

   /*
    * DO_N: quit
    * DO_A_N: valued: no
    * DO_OC: set: .m_hasoff=1, .m_offset = offsetof(duf_config_t, cli.act.v)
    * DO_FL: .afl.act={.interactive=1}
    * */
#if 0
   {.o = {DO_N( lsfiles ) /*             */ , DO_A_O, DO_V( LSFILES )} /*                                    */ , DO_CL( CONTROL ) /*    */ ,
    /*      */ DO_OC( PDISCCB, pdi ),.call = {.fdesc = {.hi = {.sccb = &duf_print_dir_callbacks}}}, DO_H( pdi cd ) /*      */ },
#endif
   /* {.o = {DO_N( list-sccb ) (*           *) , DO_A_O}, DO_TF( VI_CALL, list_sccb, 0 ) (*                     *) }, */
   {.o = {DO_Q( "list-targ" ) /*           */ , DO_A_O, DO_V( LIST_TARG )}, DO_TF( TN_CALL, list_targ ) /*                        */ },
   {.o = {DO_Q( "add-targ" ) /*            */ , DO_A_O, DO_V( ADD_TARG )}, DO_TF( TS_CALL, add_targ ) /*                                      */ },
   {.o = {DO_Q( "clear-targ" ) /*          */ , DO_A_O, DO_V( CLEAR_TARG )}, DO_T( TN_CALL ), DO_TN_CALL( clear_targ ) /*                       */ },

   {.o = {DO_Q( "history" ) /*           */ , DO_A_O, DO_V( HISTORY )}, DO_TF( VV_CALL, history ) /*                        */ },

   {.o = {.name = NULL}}
   }
};



/* vi: ft=c colorcolumn=3,44,59,60,95,96,123,145,146
*/
