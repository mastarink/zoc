#ifndef MUCS_STRUCTS_H
# define MUCS_STRUCTS_H

# include <mastar/tools/mas_argvc_tools.h>

# include "mulconfnt_types.h"
# include "mulconfnt_enums.h"

struct mucs_prefix_encoder_s
{
  char *string;
  mucs_variant_t id;
};

# if 0
struct mucs_error_s
{
  int nerror;
  int line;
  const char *func;
  const char *file;
  char *msg;
};
# endif
struct mucs_source_extra_cb_s
{
  size_t callback_called;
  const mucs_option_table_list_t *tablist;
  const mucs_source_t *source;
};
struct mucs_source_s
{
  mucs_source_list_t *list;
  mucs_source_t *next;
  mucs_source_type_t type;
  mucs_source_mode_t mode;
  mucs_flags_t flags;
  int count;
  int lastoptpos;                                                    /*number of arg with last opt */
  int curarg;                                                        /* number of arg withc current arg */
  int npos;
  const void *data_ptr;
  char delim;
  char *delims;
  char *eq;
  mucs_prefix_encoder_t pref_ids[MUCS_VARIANTS];
  mucs_source_check_fun_t check_fun;
  mucs_source_open_fun_t open_fun;
  mucs_source_close_fun_t close_fun;
  mucs_source_load_string_fun_t load_string_fun;
  mucs_source_load_targ_fun_t load_targ_fun;

  void *ptr_internal;
  int error;
  int targ_loaded;                                                   /* sequential number of targ set */
  char *string;
  mas_argvc_t oldtarg;
  mas_argvc_t targ;
  mas_argvc_t targno;

/* mucs_error_t error; */
  mucs_option_callback_t common_callback;
  mucs_option_callback_t type_callbacks[MUCS_RTYP_MAX + 1];
  mucs_source_extra_cb_t extra_cb;
};

struct mucs_source_list_s
{
  mucs_source_t *first;
  mas_argvc_t targ;

/* mucs_error_t error; */
};

union nvalue_u
{
  char v_char;
  unsigned char v_uchar;
  short v_short;
  unsigned short v_ushort;
  int v_int;
  unsigned int v_uint;
  long v_long;
  unsigned long v_ulong;
  long long v_long_long;
  unsigned long long v_ulong_long;
  double v_double;
  long double v_ldouble;
};

struct mucs_option_s
{
  char *name;
  char shortn;
  mucs_restype_t restype;
  void *cust_ptr;
  int val;
  char *desc;
  char *argdesc;
  const char *def_string_value;
  nvalue_t def_nvalue;
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
  int has_value;
  char *string_value;
  nvalue_t nvalue;
  mucs_flags_t flags;

  mucs_source_t *source;
  mucs_option_callback_t callback;
  mucs_source_extra_cb_t extra_cb;
  int value_is_set;
/* mucs_error_t error; */
};

struct mucs_option_table_list_s
{
  unsigned created:1;
  mucs_option_table_list_t *next;
  unsigned count;
  char *name;
  const mucs_option_t *options;
};

struct mucs_optscanner_s
{
  const char *arg;
  const char *at_arg;
  int preflen;
  mucs_variant_t variantid;
  int has_value;
  const char *string_value;
  const char *force_value;
  const char *nextarg;
  const mucs_option_t *found_topt;
};

struct mucs_option_interface_s
{
  mucs_option_table_list_t *tablist;
  mucs_source_list_t *source_list;
};

#endif
