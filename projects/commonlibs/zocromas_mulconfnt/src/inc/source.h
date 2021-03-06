#ifndef MUCS_SOURCE_H
# define MUCS_SOURCE_H

# include "mulconfnt_types.h"

static inline int
mucs_source_check( const mucs_source_t * osrc )
{
  return osrc ? 1 : 0;
}

int mucs_source_open( mucs_source_t * osrc );
int mucs_source_close( mucs_source_t * osrc );

char *mucs_source_load_string( mucs_source_t * osrc );
int mucs_source_load_targ( mucs_source_t * osrc );

int mucs_source_lookup_seq( mucs_source_t * osrc, const mucs_option_table_list_t * tablist, void *userdata );
int mucs_source_lookup_all( mucs_source_t * osrc, const mucs_option_table_list_t * tablist, void *userdata );

/* int mucs_source_lookup( mucs_source_t * osrc, const mucs_option_table_list_t * tablist, void *userdata ); */

int mucs_source_argsno_count( mucs_source_t * osrc );
char **mucs_source_argv_no( mucs_source_t * osrc );
int mucs_source_argc_no( mucs_source_t * osrc );
const char *mucs_source_arg_no( mucs_source_t * osrc, int i );


int mucs_source_set_common_callback( mucs_source_t * osrc, mucs_option_callback_t cb );
int mucs_source_set_type_callback( mucs_source_t * osrc, mucs_restypex_t restype, mucs_option_callback_t cb );

char *mucs_source_wd( const mucs_source_t * osrc );
char *mucs_source_absfile( const mucs_source_t * osrc, const char *fname );

int mucs_source_found_opt( mucs_source_t * osrc, mucs_option_t * opt, void *userdata );

#endif
