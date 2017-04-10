#define R_GOOD(_r) ((_r)>=0)
/* #include <stdio.h> */
#include <string.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_utils_path.h>

#include <mastar/minierr/minierr.h>
/* #include <mastar/regerr/masregerr.h> */
#include <mastar/regerr/masregerr_defs.h>

#include <mastar/mulconfnt/mulconfnt_structs.h>

#include <mastar/mulconfnt/option_interface_base.h>
#include <mastar/mulconfnt/option_interface.h>

#include <mastar/masxfs/masxfs_pathinfo_base.h>
#include <mastar/masxfs/masxfs_pathinfo.h>

/* #include <mastar/levinfo/masxfs_levinfo_types.h> */
#include <mastar/levinfo/masxfs_levinfo_structs.h>
#include <mastar/levinfo/masxfs_levinfo_ref.h>

#include <mastar/qstd/qstd_mstmt_base.h>

#include "structs.h"
#include "mysql.h"
#include "tree.h"

#include "dufnx.h"

static int
arg_process( mucs_option_t * opt, void *userdata )
{
  rDECLGOOD;
  if ( opt && opt->npos > 0 ) /* to skip argv[0] */
  {
    char *path;
    int len;
    mas_dufnx_data_t *pdufnx_data = ( mas_dufnx_data_t * ) userdata;

    /* WARN( "%d. ARG:'%s'", opt->npos, opt->string_value ); */
#if 0
    path = mas_normalize_path_cwd( opt->string_value );
#else
    path = mas_normalize_path_cwd_dots( opt->string_value );
#endif
    len = strlen( path );
    mas_free( opt->string_value );
    if ( path[0] && path[1] && path[len - 1] == '/' )
      path[len - 1] = 0;
    opt->string_value = path;

    rC( mas_tree( opt->string_value, pdufnx_data->max_depth, stdout, pdufnx_data->levinfo_flags, &pdufnx_data->mysql ) );
  }
  rRET;
}

mucs_option_interface_t *
dufnx_config_interface( mas_dufnx_data_t * pdufnx_data )
{
  mucs_option_t options[] = {
    {.name = "treedb",.shortn = '\0',.restype = MUCS_RTYP_ULONG | MUCS_RTYP_BW_OR,.cust_ptr = &pdufnx_data->levinfo_flags,
     .def_nvalue.v_ulong = MASXFS_CB_MODE_DB,.flags = MUCS_FLAG_NO_VALUE | MUCS_FLAG_USE_DEF_NVALUE}
    , {.name = "treefs",.shortn = '\0',.restype = MUCS_RTYP_ULONG | MUCS_RTYP_BW_OR,.cust_ptr = &pdufnx_data->levinfo_flags,
       .def_nvalue.v_ulong = MASXFS_CB_MODE_FS,.flags = MUCS_FLAG_NO_VALUE | MUCS_FLAG_USE_DEF_NVALUE}
    , {.name = "max-depth",.shortn = '\0' /* 'd' */ ,.restype = MUCS_RTYP_UINT,.cust_ptr = &pdufnx_data->max_depth}
    , {.name = MUCS_NONOPT_NAME,.restype = MUCS_RTYP_TARG,.flags = MUCS_FLAG_AUTOFREE,.cust_ptr = &pdufnx_data->targv,.callback = arg_process}
    , {.name = NULL,.shortn = 0,.restype = 0,.cust_ptr = NULL,.def_string_value = NULL,.val = 0,.desc = NULL,.argdesc = NULL} /* */
  };
  mucs_option_interface_t *interface = mucs_config_option_interface_create_setup( "main-table", options, TRUE );

  return interface;
}

int
dufnx( int argc, char *argv[] )
{
  rDECLGOOD;
  mas_dufnx_data_t dufnx_data = { 0 };

  mucs_option_interface_t *interface = dufnx_config_interface( &dufnx_data );

  dufnx_config_mysql( interface, &dufnx_data );

/* mucs_option_interface_add_source( interface, MUCS_SOURCE_LIBCONFIG, 0, NULL ); */
  mucs_option_interface_add_source( interface, MUCS_SOURCE_CONFIG, 0, MULCONFNT_ETC_CONFIG );
  mucs_option_interface_add_source( interface, MUCS_SOURCE_ENV, 0, "MAS_DUFNX" );
/* mucs_option_interface_add_source( interface, MUCS_SOURCE_STDIN, 0, NULL ); */
  mucs_option_interface_add_source( interface, MUCS_SOURCE_ARGV, argc, argv );

  rC( mucs_option_interface_lookup_all( interface, &dufnx_data ) );

  mucs_config_option_interface_delete( interface );
  interface = NULL;

  rRET;
}
