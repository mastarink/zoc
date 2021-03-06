#ifndef MAS_DUF_SCCB_DEF_H
#  define MAS_DUF_SCCB_DEF_H

#  include "duf_fmt_defs.h"

#  if defined(__GNUC__)
#    define DUF_SCCB( _macr, _name, _min, _fmt, ... )			_macr( _name, _min, "%" DUF_ACTION_TITLE_FMT "; "  _fmt, \
    			CRX( uni_scan_action_title, H_SCCB ), ##__VA_ARGS__ )
#    define DUF_SCCB_PDI( _macr, _name, _min, _pdi, _fmt, ... )	        _macr( _name, _min, "%" DUF_ACTION_TITLE_FMT "; seq:%llu "  _fmt, \
    			CRX( uni_scan_action_title, H_SCCB ), CRX( pdi_seq_gen, _pdi ), ##__VA_ARGS__ )
#  endif


#endif
