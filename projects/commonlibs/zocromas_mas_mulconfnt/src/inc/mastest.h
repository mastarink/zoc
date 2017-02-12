#ifndef MASTEST_H
# define MASTEST_H

void mastest_print_allocated( const char *msg, int line, const char *func );
int mastest_vexam( int line, int cond, const char *goodmsg, const char *badmsg, const char *fmt, va_list args );
int mastest_exam( int line, int cond, const char *goodmsg, const char *badmsg, const char *fmt, ... );
void mastest_series( int nseries, const char *suff );
void mastest_next_group( void );
void mastest_next( void );

#endif
