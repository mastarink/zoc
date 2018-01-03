#ifndef MUCS_ENUMS_H
# define MUCS_ENUMS_H
enum mucs_error_id_e
{
  MUCS_ERROR_ID_NONE,
  MUCS_ERROR_ID_UNRECOGNIZED_OPTION,
  MUCS_ERROR_ID_NO_VALUE,
  MUCS_ERROR_ID_WRONG_VALUE,
  MUCS_ERROR_ID_UNEXPECTED_VALUE,
  MUCS_ERROR_ID_WRONG_CALL,
  MUCS_ERROR_ID_UNKNOWN,
};

enum mucs_error_e
{
  MUCS_ERROR_NONE = 1UL << MUCS_ERROR_ID_NONE,
  MUCS_ERROR_UNRECOGNIZED_OPTION = 1UL << MUCS_ERROR_ID_UNRECOGNIZED_OPTION,
  MUCS_ERROR_NO_VALUE = 1UL << MUCS_ERROR_ID_NO_VALUE,
  MUCS_ERROR_WRONG_VALUE = 1UL << MUCS_ERROR_ID_WRONG_VALUE,
  MUCS_ERROR_UNEXPECTED_VALUE = 1UL << MUCS_ERROR_ID_UNEXPECTED_VALUE,
  MUCS_ERROR_WRONG_CALL = 1UL << MUCS_ERROR_ID_WRONG_CALL,
  MUCS_ERROR_UNKNOWN = 1UL << MUCS_ERROR_ID_UNKNOWN,
};
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
  MUCS_SOURCE_READLINE,
  MUCS_SOURCE_MAX = MUCS_SOURCE_READLINE,
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

enum mucs_bitwise_e
{
  MUCS_RTYP_BW_AND = 0x1ULL << 56UL,
  MUCS_RTYP_BW_OR = MUCS_RTYP_BW_AND >> 1UL,
  MUCS_RTYP_BW_XOR = MUCS_RTYP_BW_OR >> 1UL,
  MUCS_RTYP_BW_NOT = MUCS_RTYP_BW_XOR >> 1UL,
  MUCS_RTYP_BW_NOR = MUCS_RTYP_BW_NOT >> 1UL,
  MUCS_RTYP_FLAG_ALL = MUCS_RTYP_BW_AND | MUCS_RTYP_BW_OR | MUCS_RTYP_BW_XOR | MUCS_RTYP_BW_NOT | MUCS_RTYP_BW_NOR,
};

enum mucs_restypei_e
{
  MUCS_RTYPI_NONE,
  MUCS_RTYPI_STRING,
  MUCS_RTYPI_TARG,
  MUCS_RTYPI_CHAR,
  MUCS_RTYPI_UCHAR,
  MUCS_RTYPI_SHORT,
  MUCS_RTYPI_USHORT,
  MUCS_RTYPI_INT,
  MUCS_RTYPI_UINT,
  MUCS_RTYPI_LONG,
  MUCS_RTYPI_ULONG,
  MUCS_RTYPI_LONG_LONG,
  MUCS_RTYPI_LLONG = MUCS_RTYPI_LONG_LONG,
  MUCS_RTYPI_ULONG_LONG,
  MUCS_RTYPI_ULLONG = MUCS_RTYPI_ULONG_LONG,
  MUCS_RTYPI_DOUBLE,
  MUCS_RTYPI_LDOUBLE,
  MUCS_RTYPI_ALIAS,
  MUCS_RTYPI_MAX = MUCS_RTYPI_ALIAS,
  MUCS_RTYPI_ULONG_OR = MUCS_RTYPI_ULONG,
  MUCS_RTYPI_ULONG_NOR = MUCS_RTYPI_ULONG_OR,
};
enum mucs_restype_e
{
  MUCS_RTYP_NONE = MUCS_RTYPI_NONE << 8UL,
  MUCS_RTYP_STRING = MUCS_RTYPI_STRING << 8UL,
  MUCS_RTYP_TARG = MUCS_RTYPI_TARG << 8UL,
  MUCS_RTYP_CHAR = MUCS_RTYPI_CHAR << 8UL,
  MUCS_RTYP_UCHAR = MUCS_RTYPI_UCHAR << 8UL,
  MUCS_RTYP_SHORT = MUCS_RTYPI_SHORT << 8UL,
  MUCS_RTYP_USHORT = MUCS_RTYPI_USHORT << 8UL,
  MUCS_RTYP_INT = MUCS_RTYPI_INT << 8UL,
  MUCS_RTYP_UINT = MUCS_RTYPI_UINT << 8UL,
  MUCS_RTYP_LONG = MUCS_RTYPI_LONG << 8UL,
  MUCS_RTYP_ULONG = MUCS_RTYPI_ULONG << 8UL,
  MUCS_RTYP_LONG_LONG = MUCS_RTYPI_LONG_LONG << 8UL,
  MUCS_RTYP_LLONG = MUCS_RTYPI_LLONG << 8UL,
  MUCS_RTYP_ULONG_LONG = MUCS_RTYPI_ULONG_LONG << 8UL,
  MUCS_RTYP_ULLONG = MUCS_RTYPI_ULLONG << 8UL,
  MUCS_RTYP_DOUBLE = MUCS_RTYPI_DOUBLE << 8UL,
  MUCS_RTYP_LDOUBLE = MUCS_RTYPI_LDOUBLE << 8UL,
  MUCS_RTYP_ALIAS = MUCS_RTYPI_ALIAS << 8UL,
  MUCS_RTYP_MAX = MUCS_RTYPI_MAX << 8UL,
  MUCS_RTYP_ULONG_OR = MUCS_RTYPI_ULONG_OR << 8UL | MUCS_RTYP_BW_OR,
  MUCS_RTYP_ULONG_NOR = MUCS_RTYPI_ULONG_NOR << 8UL | MUCS_RTYP_BW_NOR,
};
enum mucs_flag_id_e
{
  MUCS_FLAG_ID_AUTOFREE,
  MUCS_FLAG_ID_LASTOPT,
  MUCS_FLAG_ID_LAST_IN_TABLE,
  MUCS_FLAG_ID_NEED_EQ,
/* MUCS_FLAG_ID_SILENT, */
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
  MUCS_FLAG_ID_CONTINUE_ON_UNRECOGNIZED,
  MUCS_FLAG_ID_USE_VPASS,
  MUCS_FLAG_ID_USE_CBPASS,
  MUCS_FLAG_ID_USE_TTY,
};
enum mucs_flag_e
{
  MUCS_FLAG_AUTOFREE = 1L << MUCS_FLAG_ID_AUTOFREE,
  MUCS_FLAG_LAST_IN_TABLE = 1L << MUCS_FLAG_ID_LAST_IN_TABLE,
  MUCS_FLAG_LASTOPT = 1L << MUCS_FLAG_ID_LASTOPT,
  MUCS_FLAG_NEED_EQ = 1L << MUCS_FLAG_ID_NEED_EQ,
/* MUCS_FLAG_SILENT = 1L << MUCS_FLAG_ID_SILENT, */
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
  MUCS_FLAG_CONTINUE_ON_UNRECOGNIZED = 1L << MUCS_FLAG_ID_CONTINUE_ON_UNRECOGNIZED,
  MUCS_FLAG_USE_VPASS = 1L << MUCS_FLAG_ID_USE_VPASS,
  MUCS_FLAG_USE_CBPASS = 1L << MUCS_FLAG_ID_USE_CBPASS,
  MUCS_FLAG_USE_PASS = MUCS_FLAG_USE_VPASS | MUCS_FLAG_USE_CBPASS,
  MUCS_FLAG_USE_TTY = 1L << MUCS_FLAG_ID_USE_TTY,
  MUCS_FLAG_ONLY_DEF_SVALUE = MUCS_FLAG_USE_DEF_SVALUE | MUCS_FLAG_NO_VALUE,
  MUCS_FLAG_ONLY_DEF_NVALUE = MUCS_FLAG_USE_DEF_NVALUE | MUCS_FLAG_NO_VALUE,
};

#endif
