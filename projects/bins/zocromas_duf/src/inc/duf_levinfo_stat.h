#ifndef MAS_DUF_LEVINFO_STAT_H
# define MAS_DUF_LEVINFO_STAT_H

# include "duf_levinfo_types.h"                                      /* duf_levinfo_t ✗ */
# include "duf_se.h"                                                 /* DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ✗ */

DR( PDI, levinfo_if_statat_dh_d, duf_depthinfo_t * pdi, int d );

DR( PDI, levinfo_if_statat_dh, duf_depthinfo_t * pdi );
DR( PDI, levinfo_if_statat_dh_up, duf_depthinfo_t * pdi );

DR( PDI, levinfo_statat_dh_d, duf_depthinfo_t * pdi, int d );

DR( PDI, levinfo_statat_dh, duf_depthinfo_t * pdi );
DR( PDI, levinfo_statat_dh_up, duf_depthinfo_t * pdi );

DRX( PDI, int, rd, 0, levinfo_if_deleted_d, duf_depthinfo_t * pdi, int d );

DRX( PDI, int, rd, 0, levinfo_if_deleted, duf_depthinfo_t * pdi );
DRX( PDI, int, rd, 0, levinfo_if_deleted_up, duf_depthinfo_t * pdi );

#endif
