#ifndef MAS_CLIENT_READLINE_H
#  define MAS_CLIENT_READLINE_H

/* int mas_client_init_readline( mas_options_t * popts, const char * *message ); */
INIT_HANDLER( mas_client_readline_init );
void mas_client_destroy_readline( mas_options_t * popts );
int mas_exchange_with_readline( mas_channel_t * pchannel );

#endif
