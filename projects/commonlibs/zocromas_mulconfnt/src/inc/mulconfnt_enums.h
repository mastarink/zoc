#ifndef MUCS_ENUMS_H
# define MUCS_ENUMS_H

enum mucs_source_type_e
{
  MUCS_SOURCE_NONE,
  MUCS_SOURCE_STRING,
  MUCS_SOURCE_ENV,
  MUCS_SOURCE_ARGV,
  MUCS_SOURCE_MARGV,
  MUCS_SOURCE_STREAM,
  MUCS_SOURCE_STDIN,
  MUCS_SOURCE_FILE,
  MUCS_SOURCE_CONFIG,
  MUCS_SOURCE_LIBCONFIG,
  MUCS_SOURCE_MAX = MUCS_SOURCE_LIBCONFIG,
};

enum mucs_source_mode_e
{
  MUCS_MODE_NONE,
  MUCS_MODE_LOAD,
  MUCS_MODE_SERIAL,
  MUCS_MODE_INTERACTIVE,
  MUCS_MODE_MAX = MUCS_MODE_INTERACTIVE,
};

enum mucs_variant_e
{
  MUCS_VARIANT_SHORT,
  MUCS_VARIANT_LONG,
  MUCS_VARIANT_NONOPT,
  MUCS_VARIANT_IGNORE,
  MUCS_VARIANT_BAD,
  MUCS_VARIANT_MAX = MUCS_VARIANT_BAD,
  MUCS_VARIANTS,
};

enum mucs_restype_e
{
  MUCS_RTYP_NONE,
  MUCS_RTYP_STRING,
  MUCS_RTYP_TARG,
  MUCS_RTYP_CHAR,
  MUCS_RTYP_UCHAR,
  MUCS_RTYP_SHORT,
  MUCS_RTYP_USHORT,
  MUCS_RTYP_INT,
  MUCS_RTYP_UINT,
  MUCS_RTYP_LONG,
  MUCS_RTYP_ULONG,
  MUCS_RTYP_LONG_LONG,
  MUCS_RTYP_LLONG = MUCS_RTYP_LONG_LONG,
  MUCS_RTYP_ULONG_LONG,
  MUCS_RTYP_ULLONG = MUCS_RTYP_ULONG_LONG,
  MUCS_RTYP_DOUBLE,
  MUCS_RTYP_LDOUBLE,
  MUCS_RTYP_ALIAS,
  MUCS_RTYP_MAX = MUCS_RTYP_ALIAS,
};
enum mucs_bitwise_e
{
  MUCS_RTYP_BW_AND = 0x10000000L,
  MUCS_RTYP_BW_OR = MUCS_RTYP_BW_AND >> 1L,
  MUCS_RTYP_BW_XOR = MUCS_RTYP_BW_OR >> 1L,
  MUCS_RTYP_BW_NOT = MUCS_RTYP_BW_XOR >> 1L,
  MUCS_RTYP_FLAG_ALL = MUCS_RTYP_BW_AND | MUCS_RTYP_BW_OR | MUCS_RTYP_BW_XOR | MUCS_RTYP_BW_NOT,
};

enum mucs_flag_id_e
{
  MUCS_FLAG_ID_AUTOFREE,
  MUCS_FLAG_ID_LASTOPT,
  MUCS_FLAG_ID_LAST_IN_TABLE,
  MUCS_FLAG_ID_NEED_EQ,
  MUCS_FLAG_ID_SILENT,
  MUCS_FLAG_ID_UNQUOTE,
  MUCS_FLAG_ID_NO_COMMON_CB,
  MUCS_FLAG_ID_NO_TYPE_CB,
  MUCS_FLAG_ID_NO_OPT_CB,
  MUCS_FLAG_ID_NO_COMMON_CB_IF_VALUE,
  MUCS_FLAG_ID_NO_TYPE_CB_IF_VALUE,
  MUCS_FLAG_ID_NO_OPT_CB_IF_VALUE,
  MUCS_FLAG_ID_USE_DEF_SVALUE,
  MUCS_FLAG_ID_USE_DEF_NVALUE,
  MUCS_FLAG_ID_OPTIONAL_VALUE,
  MUCS_FLAG_ID_NO_VALUE,
};
enum mucs_flag_e
{
  MUCS_FLAG_AUTOFREE = 1L << MUCS_FLAG_ID_AUTOFREE,
  MUCS_FLAG_LAST_IN_TABLE = 1L << MUCS_FLAG_ID_LAST_IN_TABLE,
  MUCS_FLAG_LASTOPT = 1L << MUCS_FLAG_ID_LASTOPT,
  MUCS_FLAG_NEED_EQ = 1L << MUCS_FLAG_ID_NEED_EQ,
  MUCS_FLAG_SILENT = 1L << MUCS_FLAG_ID_SILENT,
  MUCS_FLAG_UNQUOTE = 1L << MUCS_FLAG_ID_UNQUOTE,
  MUCS_FLAG_NO_COMMON_CB = 1L << MUCS_FLAG_ID_NO_COMMON_CB,
  MUCS_FLAG_NO_TYPE_CB = 1L << MUCS_FLAG_ID_NO_TYPE_CB,
  MUCS_FLAG_NO_OPT_CB = 1L << MUCS_FLAG_ID_NO_OPT_CB,
  MUCS_FLAG_NO_CB = MUCS_FLAG_NO_COMMON_CB | MUCS_FLAG_NO_TYPE_CB | MUCS_FLAG_NO_OPT_CB,
  MUCS_FLAG_NO_COMMON_CB_IF_VALUE = 1L << MUCS_FLAG_ID_NO_COMMON_CB_IF_VALUE,
  MUCS_FLAG_NO_TYPE_CB_IF_VALUE = 1L << MUCS_FLAG_ID_NO_TYPE_CB_IF_VALUE,
  MUCS_FLAG_NO_OPT_CB_IF_VALUE = 1L << MUCS_FLAG_ID_NO_OPT_CB_IF_VALUE,
  MUCS_FLAG_NO_CB_IF_VALUE = MUCS_FLAG_NO_COMMON_CB_IF_VALUE | MUCS_FLAG_NO_TYPE_CB_IF_VALUE | MUCS_FLAG_NO_OPT_CB_IF_VALUE,
  MUCS_FLAG_USE_DEF_SVALUE = 1L << MUCS_FLAG_ID_USE_DEF_SVALUE,
  MUCS_FLAG_USE_DEF_NVALUE = 1L << MUCS_FLAG_ID_USE_DEF_NVALUE,
  MUCS_FLAG_OPTIONAL_VALUE = 1L << MUCS_FLAG_ID_OPTIONAL_VALUE,
  MUCS_FLAG_NO_VALUE = 1L << MUCS_FLAG_ID_NO_VALUE,
};

#endif
