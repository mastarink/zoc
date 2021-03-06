#ifndef MAS_LCONTROL_OBJECT_H
#  define MAS_LCONTROL_OBJECT_H

#  include <mastar/types/mas_listener_control_types.h>



mas_lcontrol_t *mas_lcontrol_create( void );

/* naming : create + init + register = make */
mas_lcontrol_t *mas_lcontrol_make( const mas_options_t * popts, const char *host, unsigned port );
int mas_lcontrol_init( mas_lcontrol_t * plcontrol, const mas_options_t * popts, const char *host, unsigned port );
int mas_lcontrol_register( mas_lcontrol_t * plcontrol );
int mas_lcontrol_remove_delete( mas_lcontrol_t * plcontrol );


#endif
