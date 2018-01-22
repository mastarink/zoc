#ifndef MASXFS_LEVINFO_ENUMS_H
# define MASXFS_LEVINFO_ENUMS_H

/* # include "masxfs_levinfo_types.h" */

enum masxfs_entry_type_e
{
  MASXFS_ENTRY_NONE_NUM,
  MASXFS_ENTRY_UNKNOWN_NUM,
  MASXFS_ENTRY_BLK_NUM,
  MASXFS_ENTRY_CHR_NUM,
  MASXFS_ENTRY_DIR_NUM,
  MASXFS_ENTRY_FIFO_NUM,
  MASXFS_ENTRY_LNK_NUM,
  MASXFS_ENTRY_LINK_NUM = MASXFS_ENTRY_LNK_NUM,
  MASXFS_ENTRY_REG_NUM,
  MASXFS_ENTRY_SOCK_NUM,
};

enum masxfs_entry_type_bit_e
{
  MASXFS_ENTRY_NONE = 1L << MASXFS_ENTRY_NONE_NUM,
  MASXFS_ENTRY_UNKNOWN = 1L << MASXFS_ENTRY_UNKNOWN_NUM,
  MASXFS_ENTRY_BLK = 1L << MASXFS_ENTRY_BLK_NUM,
  MASXFS_ENTRY_CHR = 1L << MASXFS_ENTRY_CHR_NUM,
  MASXFS_ENTRY_DIR = 1L << MASXFS_ENTRY_DIR_NUM,
  MASXFS_ENTRY_FIFO = 1L << MASXFS_ENTRY_FIFO_NUM,
  MASXFS_ENTRY_LNK = 1L << MASXFS_ENTRY_LNK_NUM,
  MASXFS_ENTRY_LINK = 1L << MASXFS_ENTRY_LINK_NUM,
  MASXFS_ENTRY_REG = 1L << MASXFS_ENTRY_REG_NUM,
  MASXFS_ENTRY_SOCK = 1L << MASXFS_ENTRY_SOCK_NUM,
};

enum masxfs_cb_flag_e
{
  MASXFS_CB_OFF_NAME_NUM,
  MASXFS_CB_PATH_NUM,
  MASXFS_CB_STAT_NUM,
  MASXFS_CB_NO_FD_NUM,
  MASXFS_CB_RECURSIVE_NUM,
  MASXFS_CB_PREFIX_NUM,
  MASXFS_CB_TRAILINGSLASH_NUM,
  MASXFS_CB_SINGLE_CB_NUM,
  MASXFS_CB_SKIP_NUM,
  MASXFS_CB_SKIP_EMPTY_NUM,
  MASXFS_CB_MODE_FS_NUM,
  MASXFS_CB_MODE_DB_NUM,
  MASXFS_CB_UP_ROOT_NUM,
  MASXFS_CB_SELF_NUM,
  MASXFS_CB_UP_NUM,
  MASXFS_CB_COUNT_NUM,
  MASXFS_CB_FROM_ROOT_NUM,
/* MASXFS_CB_CAN_UPDATE_DB_NUM, */
  MASXFS_CB_DIGESTS_NUM,
  /* MASXFS_CB_USE_SIMPLE_CB_NUM, */
  MASXFS_CB_USE_STAT_CB_NUM,
};
enum masxfs_cb_flag_bit_e
{
  MASXFS_CB_OFF_NAME = 1UL << MASXFS_CB_OFF_NAME_NUM,
  MASXFS_CB_PATH = 1UL << MASXFS_CB_PATH_NUM,
  MASXFS_CB_STAT = 1UL << MASXFS_CB_STAT_NUM,
  MASXFS_CB_NO_FD = 1UL << MASXFS_CB_NO_FD_NUM,
  MASXFS_CB_RECURSIVE = 1UL << MASXFS_CB_RECURSIVE_NUM,
  MASXFS_CB_PREFIX = 1UL << MASXFS_CB_PREFIX_NUM,
  MASXFS_CB_TRAILINGSLASH = 1UL << MASXFS_CB_TRAILINGSLASH_NUM,
  MASXFS_CB_SINGLE_CB = 1UL << MASXFS_CB_SINGLE_CB_NUM,
  MASXFS_CB_SKIP = 1UL << MASXFS_CB_SKIP_NUM,
  MASXFS_CB_SKIP_EMPTY = 1UL << MASXFS_CB_SKIP_EMPTY_NUM,
  MASXFS_CB_MODE_FS = 1UL << MASXFS_CB_MODE_FS_NUM,
  MASXFS_CB_MODE_DB = 1UL << MASXFS_CB_MODE_DB_NUM,
  MASXFS_CB_UP_ROOT = 1UL << MASXFS_CB_UP_ROOT_NUM,
  MASXFS_CB_SELF = 1UL << MASXFS_CB_SELF_NUM,
  MASXFS_CB_UP = 1UL << MASXFS_CB_UP_NUM,
  MASXFS_CB_COUNT = 1UL << MASXFS_CB_COUNT_NUM,
  MASXFS_CB_FROM_ROOT = 1UL << MASXFS_CB_FROM_ROOT_NUM,
  MASXFS_CB_SELF_N_UP = MASXFS_CB_SELF | MASXFS_CB_UP,
/* MASXFS_CB_CAN_UPDATE_DB = 1UL << MASXFS_CB_CAN_UPDATE_DB_NUM, */
  MASXFS_CB_DIGESTS = 1UL << MASXFS_CB_DIGESTS_NUM,
  /* MASXFS_CB_USE_SIMPLE_CB = 1UL << MASXFS_CB_USE_SIMPLE_CB_NUM, */
  MASXFS_CB_USE_STAT_CB = 1UL << MASXFS_CB_USE_STAT_CB_NUM,
  MASXFS_CB_MODE_ALL = MASXFS_CB_MODE_FS | MASXFS_CB_MODE_DB,
};

#endif
