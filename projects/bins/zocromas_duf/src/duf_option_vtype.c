#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance_options.h"


/* ###################################################################### */
#include "duf_option_vtype.h"
/* ###################################################################### */



const char *
duf_vtype2string( duf_option_vtype_t ovtype )
{
  const char *rs = NULL;

  switch ( ovtype )
  {
#define ENUM_WRAP(_n)       case DUF_OPTION_VTYPE_ ## _n: rs= #_n; break;
#define ENUM_WRAP_V(_n, _v) case DUF_OPTION_VTYPE_ ## _n: rs= #_n; break;
#include "duf_option_vtype_enum.def"
#undef ENUM_WRAP
#undef ENUM_WRAP_V
  }
  return rs;
}

const char *
duf_extended_call_type2string( duf_option_call_type_t call_type )
{
  const char *rs = NULL;

  switch ( call_type )
  {
#define ENUM_WRAP(_n)       case DUF_OPTION_CALL_TYPE_ ## _n: rs= #_n; break;
#define ENUM_WRAP_V(_n, _v) case DUF_OPTION_CALL_TYPE_ ## _n: rs= #_n; break;
#include "duf_option_call_type_enum.def"
#undef ENUM_WRAP
#undef ENUM_WRAP_V
  }
  return rs;
}

