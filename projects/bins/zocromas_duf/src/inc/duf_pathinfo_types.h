#ifndef MAS_DUF_PATHINFO_TYPES_H
# define MAS_DUF_PATHINFO_TYPES_H

# include "duf_levinfo_types.h"                                      /* duf_levinfo_t */

typedef struct
{
  unsigned maxdepth;
  int depth;                                                         /* signed !! */
  int topdepth;                                                      /* signed !! */
/* duf_node_type_t node_type; */
/* char *path; */
/* size_t levinfo_count; => maxdepth + 3 */
  duf_levinfo_t *levinfo;
} duf_pathinfo_t;

#endif