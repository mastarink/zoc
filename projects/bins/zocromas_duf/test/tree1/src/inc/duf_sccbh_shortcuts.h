#ifndef DUF_SCCBH_SHORTCUTS_H
#  define DUF_SCCBH_SHORTCUTS_H

#  define PDI (sccbh->pdi)
#  define PDICLONED (sccbh->pdi_cloned)
#  define SCCB (sccbh->sccb)
#  define TOTITEMS (sccbh->total_items)
#if 1
#  define PU (sccbh->pu_x)
#else
#  define PU duf_pdi_pu(PDI)
#endif
#  define HCHANGES (sccbh->changes)
#  define PARGC (sccbh->parg.argc)
#  define PARGV (sccbh->parg.argv)

#endif
