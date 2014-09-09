#ifndef DUF_OPTION_H
#  define DUF_OPTION_H

int duf_parse_option( duf_option_code_t opt, int longindex, const char *optarg, int stage );

int duf_parse_option_long_typed( const duf_longval_extended_t * extended, const char *optarg, int stage );

/* int duf_parse_option_long( int longindex, const char *optarg ); */
int duf_parse_option_long_old( const duf_longval_extended_t * extended, const char *optarg, int stage );


#endif
