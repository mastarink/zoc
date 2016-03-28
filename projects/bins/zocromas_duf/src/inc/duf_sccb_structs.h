#ifndef MAS_DUF_SCCB_STRUCTS_H
# define MAS_DUF_SCCB_STRUCTS_H

#include "duf_sccb_types.h"

struct duf_sccb_data_value_s
{
/*
   DUF_SQLTYPE_NONE,
   DUF_SQLTYPE_INTEGER,
   DUF_SQLTYPE_FLOAT,
   DUF_SQLTYPE_TEXT,
   DUF_SQLTYPE_BLOB,
   DUF_SQLTYPE_NULL,
 */
  duf_sqltype_t typ;
  char *name;
  union
  {
    unsigned long long n;
  /* char *s; */
  } value;
  char *svalue;
};

struct duf_sccb_data_row_s
{
  size_t cnt;
  duf_sccb_data_value_t *fields;
  duf_sccb_data_row_t *prev;
};

struct duf_scan_callbacks_s
{
  unsigned def_opendir:1;
/* unsigned scan_mode_2:1; */
  unsigned disabled:1;
  const char *title;
  const char *name;
  duf_scan_callbacks_t *next;

  int use_std_node_set_num;
  const char *std_node_set_name;
  duf_sql_set_t node;

  int use_std_leaf_set_num;
  const char *std_leaf_set_name;
  duf_sql_set_t leaf;

  unsigned count_nodes:1;
/* unsigned count_leaves:1; */
  unsigned no_count:1;
  unsigned no_progress:1;
/* const char *leaf_selector_total2; */
  void *dlhan;
# if 1
  duf_scanner_t init_scan;

  duf_scanner_t node_scan_before2;
  duf_scanner_t node_scan_before2_deleted;

  duf_scanner_t node_scan_middle2;
  duf_scanner_t node_scan_middle2_deleted;

  duf_scanner_t node_scan_after2;
  duf_scanner_t node_scan_after2_deleted;

  duf_scanner_t leaf_scan2;
  duf_scanner_t leaf_scan2_deleted;
/* duf_scanner_t leaf_scan_fd2; */
  duf_scanner_t leaf_scan_fd2;
  duf_scanner_t leaf_scan_fd2_deleted;

  duf_scanner_t dirent_file_scan_before2;
  duf_scanner_t dirent_dir_scan_before2;
# else
  duf_scan_hook_init_t init_scan;

  duf_scan_hook2_dir_t node_scan_before2;
  duf_scan_hook2_dir_t node_scan_before2_deleted;

  duf_scan_hook2_dir_t node_scan_middle2;
  duf_scan_hook2_dir_t node_scan_middle2_deleted;

  duf_scan_hook2_dir_t node_scan_after2;
  duf_scan_hook2_dir_t node_scan_after2_deleted;

  duf_scan_hook2_file_t leaf_scan2;
  duf_scan_hook2_file_t leaf_scan2_deleted;
/* duf_scan_hook2_file_fd_t leaf_scan_fd2; */
  duf_scan_hook2_file_t leaf_scan_fd2;
  duf_scan_hook2_file_t leaf_scan_fd2_deleted;

  duf_scan_hook2_dirent_t dirent_file_scan_before2;
  duf_scan_hook2_dirent_t dirent_dir_scan_before2;
# endif
  duf_sql_sequence_t *beginning_sql_seq;
  duf_sql_sequence_t *final_sql_seq;
};

struct duf_sccb_handle_s
{
  unsigned total_counted:1;
  unsigned long long total_items;
# if 0
  int targc;
  char *const *targv;
# else
  mas_cargvc_t parg;
# endif
# if 0
  const duf_ufilter_t *pu_x;
# endif
  int pdi_cloned;
  duf_depthinfo_t *pdi;
  unsigned long long changes;
  int sccb_index;
  const duf_scan_callbacks_t *const *sccb_array;
/* const duf_scan_callbacks_t *sccb; */
/* const duf_sql_set_t *active_leaf_set; */
/* const duf_sql_set_t *second_leaf_set; */
/* const duf_sql_set_t *active_node_set; */
/* const duf_sql_set_t *second_node_set; */
  duf_sccbh_fun_t progress_leaf_cb;
  duf_sccbh_fun_t progress_node_cb;
/* duf_scanstage_t current_scanstage; */
/* duf_stmnt_t *current_statement; */
/* duf_scanner_t current_scanner; */
  duf_node_type_t assert__current_node_type;
  duf_rsccbh_fun_t atom_cb;
  /* duf_sccb_data_row_t previous_row; */
  duf_sccb_data_row_t *rows;
};

#endif
