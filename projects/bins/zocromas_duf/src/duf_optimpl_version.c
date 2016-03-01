#include <assert.h>
#include <string.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>                                  /* mas_(malloc|free|strdup); etc. ♣ */
#include <mastar/tools/mas_arg_tools.h>                              /* mas_strcat_x; etc. ♣ */

#include "duf_tracen_defs.h"                                         /* DUF_TRACE ♠ */
#include "duf_errorn_defs.h"                                         /* DUF_NOERROR; DUF_CLEAR_ERROR; DUF_E_(LOWER|UPPER); DUF_TEST_R ... ♠ */

#include "duf_start_end.h"                                           /* DUF_STARTR ; DUF_ENDR ♠ */
#include "duf_dodefs.h"                                              /* DOR ♠ */

#include "duf_output_defs.h"
#include "duf_printn_defs.h"                                         /* DUF_PRINTF etc. ♠ */

#include "duf_config.h"                                              /* duf_get_config ♠ */
#include "duf_config_ref.h"
#include "duf_config_defs.h"                                         /* DUF_CONF... ♠ */
#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ♠ */
#include "duf_config_output_util.h"

#include "duf_action_table.h"
#include "duf_sccb.h"

#include "duf_xtended_table.h"

#include "duf_options_file.h"                                        /* duf_options_infilepath */

#include "duf_option_descr.h"
#include "duf_option_stage.h"                                        /* duf_optstage_name ♠ */
#include "duf_option_extended.h"                                     /* duf_longindex_extended_count etc. ♠ */

#include "duf_option_names.h"                                        /* duf_coption_names_d etc... ♠ */
#include "duf_option_class.h"                                        /* duf_optclass2string ♠ */

/* ###################################################################### */
#include "duf_optimpl_version.h"
/* ###################################################################### */

mas_error_code_t
duf_option_O_version( void )
{
  char *sargv1;

/* char *sargv2; */

  DUF_STARTR( r );

  sargv1 = mas_argv_string( DUF_CONFIGG( pcli->carg.argc ), DUF_CONFIGG( pcli->carg.argv ), 1 );
/* sargv2 = duf_restore_some_options( DUF_CONFIGG(pcli->carg.argv)[0] ); */
#if 1
  {
    extern int __MAS_LINK_DATE__, __MAS_LINK_TIME__;

    DUF_PRINTF( 0, "CFLAGS:          (%s)", MAS_CFLAGS );
    DUF_PRINTF( 0, "LDFLAGS:         (%s)", MAS_LDFLAGS );
    DUF_PRINTF( 0, "configire        (%s)", MAS_CONFIG_ARGS );
    DUF_PUTSL( 0 );
    DUF_PRINTF( 0, "UUID             %s", MAS_UUID );

    DUF_PUTSL( 0 );
    DUF_PRINTF( 0, "prefix    [%2lu]   %s", sizeof( MAS_CONFIG_PREFIX ), MAS_CONFIG_PREFIX );
    DUF_PRINTF( 0, "C version:[%2lu]   %lu", sizeof( __STDC_VERSION__ ), __STDC_VERSION__ );
    DUF_PRINTF( 0, "O.        [%2lu]   %s", sizeof( MAS_OSVER ), MAS_OSVER );
    DUF_PRINTF( 0, "U.        [%2lu]   %s", sizeof( MAS_UNAME ), MAS_UNAME );
    DUF_PRINTF( 0, "V.        [%2lu]   %s", sizeof( PACKAGE_STRING ), PACKAGE_STRING );
    DUF_PRINTF( 0, "d.        [%2lu]   %s", sizeof( MAS_C_DATE ), MAS_C_DATE );
    DUF_PRINTF( 0, "Link d.   [%lu+%lu]  %lx.%06lx", sizeof( ( unsigned long ) & __MAS_LINK_DATE__ ),
                sizeof( ( unsigned long ) & __MAS_LINK_TIME__ ), ( unsigned long ) &__MAS_LINK_DATE__, ( unsigned long ) &__MAS_LINK_TIME__ );
    DUF_PRINTF( 0, "DATE/TIME          %s/%s", __DATE__, __TIME__ );
    DUF_PRINTF( 0, "MAS_LIBDIR:%s", MAS_LIBDIR );
# ifdef MAS_SPLIT_DB
    DUF_PRINTF( 0, "MAS_SPLIT_DB is set" );
# endif

    DUF_PUTSL( 0 );
    DUF_PRINTF( 0, "args:            (%s)", sargv1 ? sargv1 : "" );
  /* DUF_PRINTF( 0, "restored opts:   (%s)", sargv2 ); */

    DUF_PUTSL( 0 );
# if 0
    DUF_PRINTF( 0, "config from %s ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~", DUF_CONFIGG( config_file_path ) );
# else
    DUF_PRINTF( 0, "config from %s ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~", duf_options_infilepath( duf_get_config_cli(  ) ) );
# endif
    DUF_PRINTF( 0, "flow.      [%2lu]   %x", sizeof( DUF_CONFIGG( opt.flow.v.sbit ) ), DUF_CONFIGG( opt.flow.v.sbit ) );
    DUF_PRINTF( 0, "puz->      [%2lu]   %x", sizeof( DUF_CONFIGG( vars.puz )->v.sbit ), DUF_CONFIGG( vars.puz )->v.sbit );
  }
#endif
/* mas_free( sargv2 ); */
  mas_free( sargv1 );

  DUF_ENDR( r );
}
