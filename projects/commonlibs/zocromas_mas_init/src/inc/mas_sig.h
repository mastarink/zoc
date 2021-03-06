#ifndef MAS_SIG_H
#  define MAS_SIG_H

/* void sigabrt_han( int s ); */
void sigterm_han( int s );
void sigint_han( int s );
void sigquit_han( int s );
void sigusr1_han( int s );
void sigusr2_han( int s );
void sigpoll_han( int s );
void sighup_han( int s );
void sigpipe_han( int s );
void sigwinch_han( int s );

/* int mas_sig_init( mas_options_t * popts, const char **message ); */
INIT_HANDLER( mas_sig_init );
void mas_dfl_sig( void );

/* #include <mastar/types/mas_common_cdefs.h> */
#endif
