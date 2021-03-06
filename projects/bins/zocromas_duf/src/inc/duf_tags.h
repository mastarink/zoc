#ifndef MAS_DUF_TAGS_H
# define MAS_DUF_TAGS_H

# include "duf_pdi_types.h"                                          /* duf_depthinfo_t ✗ */

# include "duf_se.h"                                                 /* DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ✗ */

/* unsigned long long duf_add_tag( duf_depthinfo_t * pdi, const char *itemtype, unsigned long long itemid, const char *tag_name, int *pr ); */
DRP( OTHER, unsigned long long, tagid, 0, add_tag, duf_depthinfo_t * pdi, const char *itemtype, unsigned long long itemid, const char *tag_name );

#endif
