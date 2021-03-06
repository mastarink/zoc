#ifndef MUCS_OPTION_TABLIST_BASE_H
# define MUCS_OPTION_TABLIST_BASE_H

# include "mulconfnt_types.h"

void mucs_config_option_tabnode_init( mucs_option_table_list_t * tablist );
mucs_option_table_list_t *mucs_config_option_tabnode_create( void );

mucs_option_table_list_t *mucs_config_soption_tabnode_add( mucs_option_table_list_t * tablist, const char *name,
                                                           const mucs_option_static_t * soptions, size_t count, mucs_flags_t more_flags );
/* mucs_option_table_list_t *mucs_config_option_tabnode_add( mucs_option_table_list_t * tablist, const char *name, const mucs_option_t * options, */
/*                                                           size_t count );                                                                      */

void mucs_config_option_tabnode_delete( mucs_option_table_list_t * tablist, int fcan_autofree );
void mucs_config_option_tabnode_reset_cust( mucs_option_table_list_t * tabnode, int fcan_autofree );
void mucs_config_option_tabnode_reset( mucs_option_table_list_t * tablist, int fcan_autofree );

void mucs_config_option_tablist_reset_cust( mucs_option_table_list_t * tablist, int fcan_autofree );
void mucs_config_option_tablist_reset( mucs_option_table_list_t * tablist, int fcan_autofree );
void mucs_config_option_tablist_delete( mucs_option_table_list_t * tablist, int fcan_autofree );

#endif
