#ifndef DUF_PDI_H
#  define DUF_PDI_H

int duf_pdi_init( duf_depthinfo_t * pdi, const char *path );
void duf_pdi_close( duf_depthinfo_t * pdi );

void duf_clear_context( duf_context_t * pcontext );
void *duf_context( duf_context_t * pcontext );
void duf_set_context( duf_context_t * pcontext, void *ptr );
void duf_set_context_destructor( duf_context_t * pcontext, duf_void_voidp_t destr );

void duf_pdi_set_context( duf_depthinfo_t * pdi, void *ctx );
void duf_pdi_set_context_destructor( duf_depthinfo_t * pdi, duf_void_voidp_t destr );
void *duf_pdi_context( duf_depthinfo_t * pdi );

int duf_pdi_max_filter( const duf_depthinfo_t * pdi );
int duf_pdi_depth( const duf_depthinfo_t * pdi );

void duf_pdi_set_topdepth( duf_depthinfo_t * pdi );
int duf_pdi_topdepth( const duf_depthinfo_t * pdi );

int duf_pdi_reldepth( const duf_depthinfo_t * pdi );
int duf_pdi_deltadepth( const duf_depthinfo_t * pdi, int d );


#endif
