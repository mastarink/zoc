#include <string.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance_options.h"

#include "duf_status.h"

#include "duf_option_find.h"
#include "duf_option_cfind.h"
#include "duf_option_lfind.h"

#include "duf_option_descr.h"

#include "duf_option_typed.h"

#include "duf_option_stage.h"
#include "duf_option_source.h"



#include "duf_xtended_table.h"
#include "duf_option_clarify.h"

/* ###################################################################### */
#include "duf_option_clarify_new.h"
/* ###################################################################### */

SR( OPTIONS, split_string_od, duf_option_data_t * pod DUF_UNUSED )
{
  char *barg = NULL;
  char *endn = NULL;
  char *name = NULL;
  char *arg = NULL;
  char *string;

  string = mas_strdup( pod->string );
  switch ( pod->vseparator )
  {
  case ' ':
    barg = endn = strpbrk( string, "\t\r\n " );
    while ( barg && *barg && strchr( "\t\r\n ", *barg ) )
      barg++;
    break;
  case 0:
    barg = endn = strpbrk( string, "\t\r\n =" );
    while ( barg && *barg && strchr( "\t\r\n =", *barg ) )
      barg++;
    break;
  default:
    barg = endn = strchr( string, '=' );
    if ( barg )
      barg++;
    break;
  }
#if 0                           /* 20160113.153243 */
  if ( endn )
    name = mas_strndup( string, endn - string );
  else
    name = mas_strdup( string );
#else
  name = endn ? mas_strndup( string, endn - string ) : mas_strdup( string );
#endif
  if ( barg )
  {
    /* arg = mas_strdup( barg ); */
    /* T( ">>>> barg:'%s'", barg ); */
    arg = duf_string_options_expand( barg, '?' );
    /* T( ">> arg:'%s' => %s", barg, arg ); */
  }
  pod->name = name;
  pod->optarg = arg;
  mas_free( string );
  ER( OPTIONS, split_string_od, duf_option_data_t * pod );
}

SR( OPTIONS, soption_xclarify_new_register, const duf_longval_extended_t * xtended, int soft, duf_option_data_t * pod )
{
  size_t alloc_step = 10;
  size_t index = 0;

  T( "$$$$ SOFT:%d; %s:%s:%s -- %s", soft, pod->name, pod->optarg, pod->string, xtended->o.name );

  index = pod->xfound.count_hard + pod->xfound.count_soft;
  if ( pod->xfound.allocated_size == 0 )
  {
    assert( pod->xfound.array == NULL );
    pod->xfound.array = mas_malloc( alloc_step * sizeof( pod->xfound.array[0] ) );

    memset( pod->xfound.array + pod->xfound.allocated_size * sizeof( pod->xfound.array[0] ), 0, alloc_step * sizeof( pod->xfound.array[0] ) );
    pod->xfound.allocated_size += alloc_step;
  }
  else if ( pod->xfound.allocated_size == index )
  {
    assert( pod->xfound.array );
    pod->xfound.array = mas_realloc( pod->xfound.array, ( pod->xfound.allocated_size + alloc_step ) * sizeof( pod->xfound.array[0] ) );

    memset( pod->xfound.array + pod->xfound.allocated_size * sizeof( pod->xfound.array[0] ), 0, alloc_step * sizeof( pod->xfound.array[0] ) );
    pod->xfound.allocated_size += alloc_step;
  }
  pod->xfound.array[index].xtended = xtended;
  pod->xfound.array[index].soft = soft;
  pod->xfound.array[index].xtable = pod->xtable;

  if ( soft )
  {
    pod->xfound.count_soft++;
    pod->xfound.soft_index = index;
  }
  else
  {
    pod->xfound.count_hard++;
    pod->xfound.hard_index = index;
  }
  ER( OPTIONS, soption_xclarify_new_register, const duf_longval_extended_t * xtended, int soft, duf_option_data_t * pod );
}

SR( OPTIONS, soption_xclarify_new_at_xarr_od, const duf_longval_extended_t * xarr, duf_option_data_t * pod )
{
  for ( const duf_longval_extended_t * xtended = xarr; xtended->o.name; xtended++ )
  {
    size_t len_ask, len_tb;

    len_ask = strlen( pod->name );
    len_tb = strlen( xtended->o.name );
    if ( 0 == strcmp( pod->name, xtended->o.name ) )
    {
      assert( len_tb == len_ask );
      CR( soption_xclarify_new_register, xtended, 0 /* soft */ , pod );
    }
    else if ( 0 == strncmp( pod->name, xtended->o.name, len_ask ) )
    {
      assert( len_tb > len_ask );
      CR( soption_xclarify_new_register, xtended, len_tb - len_ask /* soft */ , pod );
    }
  }

  ER( OPTIONS, soption_xclarify_new_at_xarr_od, const duf_longval_extended_t * xarr, duf_option_data_t * pod );
}

SR( OPTIONS, soption_xclarify_new_at_xtable_od, const duf_longval_extended_table_t * xtable, duf_option_data_t * pod )
{
  pod->xtable = xtable;
  CR( soption_xclarify_new_at_xarr_od, xtable->table, pod );
  ER( OPTIONS, soption_xclarify_new_at_xtable_od, const duf_longval_extended_table_t * xtable, duf_option_data_t * pod );
}

SR( OPTIONS, soption_xclarify_new_at_multix_od, const duf_longval_extended_table_t ** xtables, duf_option_data_t * pod )
{
  for ( ; *xtables; xtables++ )
  {
    const duf_longval_extended_table_t *xtable = *xtables;

    CR( soption_xclarify_new_at_xtable_od, xtable, pod );
/* if not found : clear error */
  }
  if ( pod->xfound.count_hard == 1 )
  {
    if ( pod->xfound.array[pod->xfound.hard_index].soft == 0
         && DUF_OPTION_CHECK_STAGE( pod->stage, pod->xfound.array[pod->xfound.hard_index].xtended, pod->xfound.array[0].xtable ) )
    {
      T( "@Do it : %s", pod->xfound.array[pod->xfound.hard_index].xtended->o.name );
    }
  }
  else if ( pod->xfound.count_hard == 0 )
  {
    if ( pod->xfound.count_soft == 1 )
    {
      if ( pod->xfound.array[pod->xfound.soft_index].soft > 0
           && DUF_OPTION_CHECK_STAGE( pod->stage, pod->xfound.array[pod->xfound.hard_index].xtended, pod->xfound.array[0].xtable ) )
        T( "@Do it : %s", pod->xfound.array[pod->xfound.soft_index].xtended->o.name );
    }
    else if ( pod->xfound.count_soft > 1 )
    {
      /* MULTI-ERROR : may show list to choose */
    }
    else                        /* if ( pod->xfound.count_soft < 1 ) */
    {
      /* NOT-FOUND-ERROR */
    }
  }
  else
  {
    /* BAD-MULTI-ERROR : same command defined, needs fixing tables */
  }
  T( "@-------- %lu:%lu ----------", pod->xfound.count_hard, pod->xfound.count_soft );
  for ( size_t n = 0; n < pod->xfound.count_hard + pod->xfound.count_soft; n++ )
  {
    T( "@@ ========== %d: --%s='%s' =======================", pod->xfound.array[n].soft, pod->xfound.array[n].xtended->o.name, pod->optarg );
  }
  mas_free( pod->xfound.array );
  pod->xfound.array = NULL;
  mas_free( pod->name );
  pod->name = NULL;
  mas_free( pod->optarg );
  pod->optarg = NULL;

  ER( OPTIONS, soption_xclarify_new_at_multix_od, const duf_longval_extended_table_t ** xtables, duf_option_data_t * pod );
}

SR( OPTIONS, soption_xclarify_new_at_stdx_od, duf_option_data_t * pod )
{
  CR( soption_xclarify_new_at_multix_od, duf_extended_table_multi(  ), pod );

  ER( OPTIONS, soption_xclarify_new_at_stdx_od, duf_option_data_t * pod );
}

SR( OPTIONS, soption_xclarify_new_at_stdx, const char *string, const char *name, const char *arg, duf_xclarifier_t clarifier, char vseparator,
    duf_option_stage_t istage, duf_option_source_t source )
{
  duf_option_data_t od = { 0 };
  od.string = string;
  od.name = mas_strdup( name );
  od.optarg = mas_strdup( arg );
  od.vseparator = vseparator;
  od.clarifier = clarifier;
  od.stage = istage;
  od.source = source;

  if ( string && !od.name )
    CR( split_string_od, &od );
  CR( soption_xclarify_new_at_stdx_od, &od );

  mas_free( od.xfound.array );
  od.xfound.array = NULL;
  mas_free( od.name );
  od.name = NULL;
  mas_free( od.optarg );
  od.optarg = NULL;

  ER( OPTIONS, soption_xclarify_new_at_stdx, const char *string, const char *name, const char *arg, duf_xclarifier_t clarifier, char vseparator,
      duf_option_stage_t istage, duf_option_source_t source );
}

SR( OPTIONS, soption_xclarify_new, const char *string, const char *name, const char *arg, char vseparator, duf_option_stage_t istage,
    duf_option_source_t source )
{
  CR( soption_xclarify_new_at_stdx, string, name, arg, DUF_WRAPPED( duf_xoption_clarify ), vseparator, istage, source );

  ER( OPTIONS, soption_xclarify_new, const char *string, const char *name, const char *arg, char vseparator, duf_option_stage_t istage,
      duf_option_source_t source );
}
