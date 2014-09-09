#ifndef MAS_DUF_OPTION_FIND_H
#  define MAS_DUF_OPTION_FIND_H

#  include "duf_option_types.h"
#  include "duf_options_def.h"

int duf_find_name_long( const char *name, const duf_longval_extended_t * xtable, unsigned xtable_size, int soft );
int duf_find_name_long_exact( const char *name, const duf_longval_extended_t * xtable, unsigned xtable_size );
int duf_find_codeval_long_std( duf_option_code_t code );
const duf_longval_extended_t *duf_find_codeval_extended_std( duf_option_code_t codeval, int *pr );

#endif

