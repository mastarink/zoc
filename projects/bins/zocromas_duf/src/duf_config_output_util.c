#include <string.h>
#include <errno.h>

#include <mastar/tools/mas_tools.h>                                  /* mas_tstrflocaltime */
#include <mastar/tools/mas_utils_path.h>                             /* mas_concat_path */

#include "duf_maintenance.h"

#include "duf_expandable.h"

#include "duf_config.h"
#include "duf_config_defs.h"
#include "duf_config_ref.h"

/* ###################################################################### */
#include "duf_config_output_util.h"
/* ###################################################################### */

int
duf_output_level( void )
{
  return duf_config ? duf_config->opt.output.stream.level : 0;
}

FILE *
duf_output_file( void )
{
  return duf_output_file_c( duf_config );
}

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

#if 0
int
duf_output_use_binformat( void )
{
  return duf_output_use_binformat_c( duf_config );
}
#endif
