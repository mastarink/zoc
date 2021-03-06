#include <assert.h>
#include <string.h>
#include <sys/time.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "mas_trace_types.h"
#include "mas_trace_defs.h"

/* ###################################################################### */
#include "mas_trace_credel.h"
/* ###################################################################### */

mas_config_trace_t *
mas_config_trace_create( size_t level_max )
{
  mas_config_trace_t *tcfg = NULL;

  tcfg = mas_malloc( sizeof( mas_config_trace_t ) );
  memset( tcfg, 0, sizeof( mas_config_trace_t ) );
  {
    int ry;
    struct timeval tv;

    ry = gettimeofday( &tv, NULL );
    if ( ry >= 0 )
      tcfg->loadtime = /* cfg->loadtime = */ ( ( double ) tv.tv_sec ) + ( ( double ) tv.tv_usec ) / 1.0E6;
  }
  tcfg->num_classes = level_max;
#if 0
  tcfg->class_levels = mas_calloc( level_max, sizeof( tcfg->class_levels[0] ) );
#else
  tcfg->class_levels = mas_malloc( level_max * sizeof( tcfg->class_levels[0] ) );
  memset( tcfg->class_levels, 0, level_max * sizeof( tcfg->class_levels[0] ) );
#endif

#if 0
  tcfg->errors += 2;
/* tcfg->options = 1; */
/* tcfg->fs += 1; */

/* tcfg->options  = 11; */
/* tcfg->options  = 71; */
  tcfg->temp += 2;
#else
  /* tcfg->class_levels[DUF_TRACE_LEVEL_errors] += 2; */
/* tcfg->class_levels[DUF_TRACE_LEVEL_options] = 1;  */
/* tcfg->class_levels[DUF_TRACE_LEVEL_fs] += 1;      */
/* tcfg->class_levels[DUF_TRACE_LEVEL_options] = 11; */
/* tcfg->class_levels[DUF_TRACE_LEVEL_options] = 71; */
  /* tcfg->class_levels[DUF_TRACE_LEVEL_temp] += 2; */
#endif
  return tcfg;
}

void
mas_config_trace_delete( mas_config_trace_t * tcfg )
{
  mas_free( tcfg->stream.file );
  tcfg->stream.file = NULL;
  mas_free( tcfg->class_levels );
  tcfg->class_levels = NULL;
  tcfg->num_classes = 0;
  mas_free( tcfg );
}
