#ifndef MAS_MUC_OPTIONS_H
# define MAS_MUC_OPTIONS_H

# include "muc_option_types.h"                                       /* muc_longval_extended_t; muc_longval_extended_vtable_t ♠ */
# include "muc_option_stage_types.h"                                 /* muc_option_stage_t ♠ */
# include "muc_fun_types.h"                                          /* muc_int_void_func_t; muc_cpchar_void_func_t */
# include "muc_config_cli_types.h"                                   /* muc_config_cli_t ♠ */
# include "muc_se.h"                                                 /* muc_DR; muc_SR; muc_ER; muc_CR; muc_QSTR; QERRIND; QERRNAME etc. ♠ */

/* naming convention:
 * <gen.prefix>_<subject prefix><entity>[_<entity-details>]_<action-result>
 * 
 * ** subject is entity "incarnation"
 *
 * 1. gen.prefix: duf
 * 2. subject prefix: depends on main argument(s) type
 *   <nothing>					=>
 * 3. entity: all_options
 *   3.1. entity-details: 
 * 4. action: ~parse,~clarify,~exec...?
 * */

muc_DR( OPTIONS, all_options, muc_config_cli_t * cli, muc_option_stage_t istage, muc_int_void_func_t cb_do_interactive,
    muc_cpchar_void_func_t cb_prompt_interactive, muc_option_adata_t * paod, int from_paod );

#endif