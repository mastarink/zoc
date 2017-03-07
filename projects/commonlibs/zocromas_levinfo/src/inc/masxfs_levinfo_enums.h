#ifndef MASXFS_LEVINFO_ENUMS_H
# define MASXFS_LEVINFO_ENUMS_H

# include "masxfs_levinfo_types.h"

enum masxfs_entry_type_e
{
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
  MASXFS_CB_NAME_NUM,
  MASXFS_CB_PATH_NUM,
  MASXFS_CB_STAT_NUM,
  MASXFS_CB_FD_NUM,
  MASXFS_CB_RECURSIVE_NUM,
  MASXFS_CB_PREFIX_NUM,
  MASXFS_CB_TRAILINGSLASH_NUM,
  MASXFS_CB_MULTIPLE_CBS_NUM,
  MASXFS_CB_SKIP_NUM,
  MASXFS_CB_MODE_FS_NUM,
  MASXFS_CB_MODE_DB_NUM,
};
enum masxfs_cb_flag_bit_e
{
  MASXFS_CB_NAME = 1L << MASXFS_CB_NAME_NUM,
  MASXFS_CB_PATH = 1L << MASXFS_CB_PATH_NUM,
  MASXFS_CB_STAT = 1L << MASXFS_CB_STAT_NUM,
  MASXFS_CB_FD = 1L << MASXFS_CB_FD_NUM,
  MASXFS_CB_RECURSIVE = 1L << MASXFS_CB_RECURSIVE_NUM,
  MASXFS_CB_PREFIX = 1L << MASXFS_CB_PREFIX_NUM,
  MASXFS_CB_TRAILINGSLASH = 1L << MASXFS_CB_TRAILINGSLASH_NUM,
  MASXFS_CB_MULTIPLE_CBS = 1L << MASXFS_CB_MULTIPLE_CBS_NUM,
  MASXFS_CB_SKIP = 1L << MASXFS_CB_SKIP_NUM,
  MASXFS_CB_MODE_FS = 1L << MASXFS_CB_MODE_FS_NUM,
  MASXFS_CB_MODE_DB = 1L << MASXFS_CB_MODE_DB_NUM,
};

enum masxfs_scan_mode_e {
  MASXFS_SCAN_MODE_FS,
  MASXFS_SCAN_MODE_DB,
};

#endif
