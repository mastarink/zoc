#ifndef MAS_MUC_OPTION_STAGE_TYPES_H
# define MAS_MUC_OPTION_STAGE_TYPES_H

typedef enum
{
  MUC_OPTION_STAGE_NONE = 0,
  MUC_OPTION_STAGE_MIN = 1,
  MUC_OPTION_STAGE_DEBUG = MUC_OPTION_STAGE_MIN,
  MUC_OPTION_STAGE_BOOT,
  MUC_OPTION_STAGE_PRESETUP,
  MUC_OPTION_STAGE_PRESET = MUC_OPTION_STAGE_PRESETUP,
  MUC_OPTION_STAGE_SETUP,
  MUC_OPTION_STAGE_INIT,
  MUC_OPTION_STAGE_FIRST,
  MUC_OPTION_STAGE_LOOP,
  MUC_OPTION_STAGE_LOOPE = MUC_OPTION_STAGE_LOOP + 1000,
  MUC_OPTION_STAGE_INTERACTIVE,
  MUC_OPTION_STAGE_INTERAC = MUC_OPTION_STAGE_INTERACTIVE,
  MUC_OPTION_STAGE_IA = MUC_OPTION_STAGE_INTERACTIVE,
  MUC_OPTION_STAGE_MAX = MUC_OPTION_STAGE_INTERACTIVE,
  MUC_OPTION_STAGE_ANY = 8888888,
  MUC_OPTION_STAGE_ALL = 9999999,
} muc_option_stage_t;

typedef struct
{
/* unsigned flag:1; ??? */
  muc_option_stage_t min;
  muc_option_stage_t max;
} muc_limits_stage_t;

typedef struct muc_extended_stageopts_s
{
  unsigned use_stage:1;
  unsigned use_stage_mask:1;
  muc_limits_stage_t stage;
  unsigned long stage_mask;
} muc_extended_stageopts_t;



#endif
