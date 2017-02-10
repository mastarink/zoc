#include <stdarg.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_argvc_tools.h>

#include "mulconfnt_defs.h"
#include "mulconfnt_structs.h"

#include "option_base.h"

void
mulconfnt_config_option_init( config_option_t * opt )
{
  memset( opt, 0, sizeof( config_option_t ) );
}

config_option_t *
mulconfnt_config_option_create( void )
{
  config_option_t *opt = mas_malloc( sizeof( config_option_t ) );

  mulconfnt_config_option_init( opt );
  return opt;
}

void
mulconfnt_config_option_close( config_option_t * opt )
{
  if ( opt->name )
    mas_free( opt->name );
  if ( opt->desc )
    mas_free( opt->desc );
  if ( opt->argdesc )
    mas_free( opt->argdesc );
  if ( opt->string_value )
    mas_free( opt->string_value );
}

void
mulconfnt_config_option_delete( config_option_t * opt )
{
  mulconfnt_config_option_close( opt );
  mas_free( opt );
}

config_option_t *
mulconfnt_config_option_clone( const config_option_t * topt )
{
  config_option_t *opt = mulconfnt_config_option_create(  );

  *opt = *topt;
  opt->name = mas_strdup( opt->name );
  opt->desc = mas_strdup( opt->desc );
  opt->argdesc = mas_strdup( opt->argdesc );
  opt->string_value = mas_strdup( opt->string_value );
  return opt;
}