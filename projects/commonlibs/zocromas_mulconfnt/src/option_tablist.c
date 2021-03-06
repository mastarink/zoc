#define R_GOOD(_r) ((_r)>=0)
#include "mulconfnt_defs.h"
#include <string.h>
/* #include <math.h> */

#include <mastar/wrap/mas_memory.h>
/* #include <mastar/tools/mas_argvc_tools.h> */

#include <mastar/minierr/minierr.h>

#include "mulconfnt_structs.h"

#include "global.h"

#include "aoptions.h"

#include "option.h"
/* #include "option_flags.h" */
#include "option_value.h" // for QRGOPTM -> mucs_config_option_check  only
/* #include "option_base.h" */

#include "option_tablist.h"

/*
 *
 * mucs_config_option_tablist_...
 *
 * */

int
mucs_config_option_tablist_lookup( const mucs_option_table_list_t * tablist, const char *arg_nopref, const char *eq, mucs_optscanner_t * optscan )
{
  rDECLBAD;
  optscan->found_topt = NULL;
/* optscan->at_arg = NULL; */
  if ( tablist )
  {
#if 0
    rSETGOOD;
    while ( rGOOD && !optscan->found_topt && tablist )
    {
      rC( mucs_config_option_lookup_options( mucs_config_option_tabnode_aoptions( tablist ), tablist->optcount, arg_nopref, eq, optscan ) );
      tablist = tablist->next;
    }
#else
    do
    {
      const mucs_option_t *options = mucs_config_option_tabnode_aoptions( tablist );

      rC( mucs_config_option_lookup_options( options, tablist->optcount, arg_nopref, eq, optscan ) );
      tablist = tablist->next;
    } while ( rGOOD && !optscan->found_topt && tablist );
#endif
    if ( !optscan->found_topt && optscan->variantid != MUCS_VARIANT_NONOPT )
    {
      rSETBAD;
      if ( optscan->at_arg )
        QRGOPTM( optscan->found_topt, rCODE, "unrecognized option \"%s\" at \"%s\"", optscan->at_arg, optscan->arg );
      else
        QRGOPTM( optscan->found_topt, rCODE, "unrecognized option at \"%s\"", optscan->arg );
      optscan->errors |= MUCS_ERROR_UNRECOGNIZED_OPTION;
      optscan->at_arg = NULL;
      if ( mucs_global_flag( MUCS_FLAG_CONTINUE_ON_UNRECOGNIZED ) )
        rSETGOOD;
    }
  }
  rRET;
}
