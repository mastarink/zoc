#ifndef MAS_DUF_OPTION_TYPES_H
#  define MAS_DUF_OPTION_TYPES_H

typedef enum
{
  DUF_OPTION_CLASS_NONE,

  DUF_OPTION_CLASS_ANY,
  DUF_OPTION_CLASS_MIN,

  DUF_OPTION_CLASS_HELP,
  DUF_OPTION_CLASS_SYSTEM,
  DUF_OPTION_CLASS_CONTROL,
  DUF_OPTION_CLASS_REFERENCE,
  DUF_OPTION_CLASS_COLLECT,
  DUF_OPTION_CLASS_SCAN,
  DUF_OPTION_CLASS_UPDATE,
  DUF_OPTION_CLASS_REQUEST,
  DUF_OPTION_CLASS_PRINT,
  
  DUF_OPTION_CLASS_TRACE,
  DUF_OPTION_CLASS_DEBUG,
  DUF_OPTION_CLASS_NODESC,
  DUF_OPTION_CLASS_OTHER,

  DUF_OPTION_CLASS_MAX,
} duf_option_class_t;

typedef struct
{
  int val;
  const char *help;
  duf_option_class_t oclass;
  int shown;
} duf_longval_extended_t;

#endif