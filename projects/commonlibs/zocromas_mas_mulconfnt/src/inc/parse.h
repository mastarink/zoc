#ifndef MULCONF_H
# define MULCONF_H

#include "mulconfnt_types.h"

int mulconfnt_parse( int argc, char *argv[] );
mas_argvc_t mulconfnt_load_targ( config_source_desc_t * optr );
char *mulconfnt_load_string( config_source_desc_t * optr );

#endif
