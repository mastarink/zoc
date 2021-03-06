#ifndef MAS_DUF_SQL_SET_TYPES_H
# define MAS_DUF_SQL_SET_TYPES_H

# include "duf_scan_types.h"                                         /* duf_node_type_t; duf_scanstage_t ✗ */

typedef struct duf_sql_set_s duf_sql_set_t;
typedef struct duf_sql_set_pair_s duf_sql_set_pair_t;
typedef struct duf_selector_s duf_selector_t;
typedef struct duf_fieldset_s duf_fieldset_t;

struct duf_sql_set_s
{
/* unsigned use_std_fieldset:1; */
/* unsigned use_std_selector2:1; */
  unsigned expand_sql:1;
  duf_node_type_t type;
  const char *name;
  const char *fieldset;
  const char *fieldsets[10];
/* const char *selector; */
  const char *selector2;

  const char *cte;
  const char *selector2_cte;
  const char *matcher;
  const char *amatcher[5];
  const char *filter;
  const char *afilter[5];
  const char *filter_fresh;
  const char *afilter_fresh[10];
  const char *filter_fast;
  const char *afilter_fast[10];
  const char *group;
  const char *order;
  const char *orders[20];
  const char *count_aggregate;
/* const char *selector_total2; */
};

struct duf_sql_set_pair_s
{
  unsigned orderid;
  const duf_sql_set_t *active;
  const duf_sql_set_t *second;
};

struct duf_selector_s
{
  duf_node_type_t type;
  const char *name;
  const char *selector2;
};

struct duf_fieldset_s
{
  duf_node_type_t type;
  const char *name;
  const char *set;
};

#endif
