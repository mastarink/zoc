#include <string.h>

#include "duf_maintenance.h"

#include "duf_config_ref.h"

/* ###################################################################### */
#include "duf_tracen_config.h"
/* ###################################################################### */
#ifdef MAS_TRACING
const duf_config_t *duf_config4trace = NULL;

duf_config_opt_trace_t *
duf_get_trace_config( void )
{
  assert( duf_config );
  return &duf_config->opt.trace;
}

FILE *
duf_trace_file_c( const duf_config_opt_trace_t * tcfg )
{
/* return tcfg && tcfg->MAST_TRACE_AT_CFG output.out ? tcfg->MAST_TRACE_AT_CFG output.out : stdout; */
  return tcfg && tcfg->output.out ? tcfg->output.out : stdout;
}

FILE *
duf_trace_file( void )
{
  return duf_trace_file_c( duf_get_trace_config(  ) );
}

#endif

int
duf_output_force_color( void )
{
  return duf_output_force_color_c( duf_config );
}

int
duf_output_nocolor( void )
{
  return duf_output_nocolor_c( duf_config );
}

int
duf_output_progress( void )
{
  return duf_output_progress_c( duf_config );
}

int
duf_output_use_binformat( void )
{
  return duf_output_use_binformat_c( duf_config );
}