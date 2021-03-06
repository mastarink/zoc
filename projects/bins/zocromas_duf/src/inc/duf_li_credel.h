#ifndef MAS_DUF_LI_CREDEL_H
# define MAS_DUF_LI_CREDEL_H

# include "duf_levinfo_types.h"                                      /* duf_levinfo_t ✗ */
# include "duf_scan_types.h"                                         /* duf_node_type_t; duf_scanstage_t ✗ */
# include "duf_se.h"                                                 /* DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ✗ */

/* size_t duf_li_size_array( unsigned maxdepth, unsigned r ); */

/* int duf_li_closed_array( const duf_levinfo_t * pli, unsigned maxdepth ); */
DRX( LI, int, closed, 1, li_closed_array, const duf_levinfo_t * pli, unsigned maxdepth );

/* void duf_li_clear( duf_levinfo_t * pli ); */
DRN( LI, void, li_clear, duf_levinfo_t * pli );

/* void duf_li_clear_array( duf_levinfo_t * pli, unsigned maxdepth ); */
DRN( LI, void, li_clear_array, duf_levinfo_t * pli, unsigned maxdepth );

/* void duf_li_init( duf_levinfo_t * pli, const char *itemname, unsigned long long dirid, duf_node_type_t node_type ); */
DRN( LI, void, li_init, duf_levinfo_t * pli, const char *itemname, unsigned long long dirid, duf_node_type_t node_type );

/* void duf_li_delete_array( duf_levinfo_t * pli, unsigned maxdepth ); */
DRN( LI, void, li_delete_array, duf_levinfo_t * pli, unsigned maxdepth );

DRX( LI, duf_levinfo_t *, pli, NULL, li_create_array, unsigned maxdepth );

DRX( LI, duf_levinfo_t *, pli, NULL, li_clone_array, const duf_levinfo_t * lisrc, unsigned maxdepth );

/* duf_levinfo_t *duf_dirid2li( unsigned long long dirid, const char *itemname, unsigned maxdepth, int *pr ); */
DRP( LI, duf_levinfo_t *, pli, NULL, nameid2li, unsigned long long nameid, unsigned maxdepth );

#endif
