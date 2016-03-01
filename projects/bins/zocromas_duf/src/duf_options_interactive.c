/* #undef MAS_TRACING */
#define MAST_TRACE_CONFIG duf_get_cli_options_trace_config(cli)
#include "duf_errorn_defs_preset.h" 

#include <string.h>
#include <stdlib.h>                                                  /* free */
#include <stdio.h>
#include <errno.h>

#include <readline/readline.h>
#include <readline/history.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/tools/mas_arg_tools.h>                              /* mas_strcat_x; etc. ♣ */
# include <mastar/error/mas_error_defs_ctrl.h>
# include <mastar/error/mas_error_defs.h>                            /* MASE_TEST_R; MASE_TEST_R_LOWERE; ... */
# include <mastar/error/mas_error_defs_make.h>                       /* MASE_MAKE_ERROR; MASE_MAKE_ERRORFL; MASE_MAKE_ERRORM  ... */

#include "duf_tracen_defs.h"                                         /* DUF_TRACE ♠ */
/* #include "duf_errorn_defs.h"                                         (* DUF_NOERROR; DUF_CLEAR_ERROR; DUF_E_(LOWER|UPPER); DUF_TEST_R ... ♠ *) */

#include "duf_se_only.h"                                             /* Only DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ♠ */

#include "duf_option_stage.h"                                        /* duf_optstage_name ♠ */
#include "duf_option_source.h"                                       /* duf_optsource_name ♠ */

#include "duf_option_config.h"                                       /* duf_get_cli_options_trace_config ♠ */
#include "duf_option_clarify_batch.h"

/* ###################################################################### */
#include "duf_options_interactive.h"
/* ###################################################################### */

SR( OPTIONS, source_interactive_parse, duf_config_cli_t * cli, duf_option_stage_t istage, duf_int_void_func_t cb_do_interactive
    __attribute__ ( ( unused ) ), duf_cpchar_void_func_t cb_prompt_interactive
    __attribute__ ( ( unused ) ), duf_option_adata_t * paod, duf_option_source_code_t sourcecode MAS_UNUSED )
{

#if 0
  static char rl_prompt[256 * 10] = "";
#endif
  const char *prompt = NULL;

  T( "@   source:%s", duf_optsourcecode_name( cli, sourcecode ) );
  if ( istage == DUF_OPTION_STAGE_INTERACTIVE /* XXX ???? XXX */  )
  {
    DUF_TRACE( options, 0, "@@@@stage:%s source: interactive", duf_optstage_name( cli, istage ) );
#if 0
    if ( !*rl_prompt )
      snprintf( rl_prompt, sizeof( rl_prompt ), "%s> ", "CMD" );
#endif

#if 0
    add_history( "quit" );
    add_history( "pwd" );
    add_history( "max-depth" );
    add_history( "max-seq" );
    add_history( "selection" );
    add_history( "same-md5 /mnt/new_media/media/photo/Pictures/photos/sel/catfly.jpg" );
    add_history( "lsfiles" );
    add_history( "list-sccb" );
    add_history( "list-targ" );
    add_history( "clear-targ" );
    add_history( "add-targ /mnt/new_media/media/photo/Pictures/photos/sel/" );
    add_history( "cd /mnt/new_media/media/photo/Pictures/photos/" );
    add_history( "tree" );
    add_history( "recursive" );
    add_history( "no-recursive" );
#endif
    {
#if 0
      if ( DUF_CONFIGG( cli.history_filename ) )
        read_history( DUF_CONFIGG( cli.history_filename ) );
#else
      if ( duf_cli_options_get_history_filename( cli ) )
        read_history( duf_cli_options_get_history_filename( cli ) );
#endif

#if 0
      {
        HISTORY_STATE *phstate;

        phstate = history_get_history_state(  );
        if ( phstate->length == 0 )
        {
          add_history( "quit" );
          add_history( "lsfiles" );
          add_history( "cd /" );
        }
        else
        {
          for ( int i = 0; i < phstate->length; i++ )
          {
            HIST_ENTRY *he;

            he = history_get( i );
            if ( he )
              DUF_PRINTF( 0, "%s:%s", he->timestamp, he->line );
          }
        }
        DUF_TRACE( temp, 0, "@@history length:%d; offset:%d; file:%s", phstate->length, phstate->offset, DUF_CONFIGG( cli.history_filename ) );
      }
#endif
      while ( QNOERR /* && DUF_ACTG_FLAG( interactive ) */  && cb_do_interactive(  ) /* don't remove: this is for quit */
              && isatty( STDIN_FILENO ) /* only when stdin is tty */  )
      {
        char *rl_buffer = NULL, *s = NULL;

/* TODO : via callback of some kind */

        prompt = cb_prompt_interactive(  );

        while ( !rl_buffer )
          rl_buffer = readline( prompt );
        s = rl_buffer;
        if ( s && *s )
        {
          s = mas_chomp( s );
          add_history( s );
          DUF_TRACE( options, 0, "@@@@read cmd '%s'", s );
          if ( s && ( ( *s == '#' ) || !*s ) )
            continue;
        /* DUF_TRACE( any, 0, "buffer:[%s]", buffer ); */
          DUF_TRACE( explain, 0, "read config line %s", s );
          {
            {
              CR( boption_xclarify_at_stdx, cli, 0 /* value_separator */ , istage, DUF_OPTION_SOURCE( INTERACTIVE ), s, 0, paod );
            }
            DUF_TRACE( options, 0, "@@@@executed cmd; QERRIND=%d; s=%s", QERRIND, s );
          }
          free( rl_buffer );
          rl_buffer = NULL;
        }
      /* DUF_CLEAR_ERROR( r, DUF_ERROR_OPTION_NOT_FOUND ); */
        ERRCLEAR( OPTION_NOT_FOUND );
        QTR;
      }
#if 0
      if ( DUF_CONFIGG( cli.history_filename ) )
        write_history( DUF_CONFIGG( cli.history_filename ) );
#else
      if ( duf_cli_options_get_history_filename( cli ) )
        write_history( duf_cli_options_get_history_filename( cli ) );
#endif
    }
  }

  ER( OPTIONS, source_interactive_parse, duf_config_cli_t * cli, duf_option_stage_t istage, duf_int_void_func_t cb_do_interactive
      __attribute__ ( ( unused ) ), duf_cpchar_void_func_t cb_prompt_interactive
      __attribute__ ( ( unused ) ), duf_option_adata_t * paod, duf_option_source_code_t sourcecode );
}
