#include "mas_std_def.h"

#include <assert.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

#ifndef MAS_NO_THREADS
# include <pthread.h>
#endif
#include <errno.h>

/* #include "mas_common.h" */

#include "mas_memory.h"

/*
this:
  mas_memory.c
related:
  mas_lib.c
  mas_lib0.c
  mas_tools.c
  mas_cs.c
  mas_cs.h
  mas_control.c
  mas_opts.c
  
  mas_load_module.c

*/

const mas_memfun_block_t *mas_pmemfun_block = NULL;
static void constructor_main( void ) __attribute__ ( ( constructor( 101 ) ) );

static void
constructor_main( void )
{
  if ( mas_memory_allocation )
    mas_pmemfun_block = mas_memory_allocation(  );
//fprintf( stderr, "setting mas_pmemfun_block=%p\n", mas_pmemfun_block );
}

static void destructor_global_status( void ) __attribute__ ( ( destructor( 5001 ) ) );

static void
destructor_global_status( void )
{
//  int r = 0;

#define DUF_MEM_ROW " ◇◇◇◇◇◇◇◇◇ "
#define DUF_MEM_ROWL " ▷▷▷▷▷▷ "
#define DUF_MEM_ROWR " ◁◁◁◁◁◁ "

#ifdef MAS_TRACEMEM
  if ( mas_pmemfun_block == MAS_MEMORY_ALLOCATION_MASTAR /* i.e.  !mas_pmemfun_block */  )
  {
    FILE *out = NULL;

    out = fopen( "mas_debug_memory.tmp", "w" );
//    r = 
    print_memlist_msg( FL, out,                                      /* */
                       "\x1b[0;1;7;44;35m" DUF_MEM_ROWL "<  1.",     /* */
                       "\n\x1b[0;1;7;46;37m" DUF_MEM_ROWL "<  2.",   /* */
                       "\x1b[0;1;7;32m    MEMORY FREE    ",          /* */
                       "\x1b[0;1;7;44;35m  >" DUF_MEM_ROWR "\x1b[0m", "\n\n\x1b[0;1;33;41m                              MEMORY TABLE                              \x1b[0m" /* */
             );
/*  if ( r >= 0 )
      fprintf( out, "[%d]\n", r ); */
    fclose( out );
  }
#endif
}

#ifndef MAS_NO_THREADS
pthread_mutex_t malloc_mutex = PTHREAD_MUTEX_INITIALIZER;
#endif

#ifdef MAS_TRACEMEM
int mas_tracemem_flag = 1;
#else
int mas_tracemem_flag = 0;
#endif
#ifdef MAS_TRACEMEM
# if 0
char *
_mas_strncat_xt( const char *func, int line, char *s1, const char *s2, size_t maxs2 )
{
  char *r = NULL;

  if ( s1 || s2 )
  {
    size_t l, l1, l2;

    l1 = s1 ? strlen( s1 ) : 0;
    l2 = s2 ? strlen( s2 ) : 0;
    if ( l2 > maxs2 )
      l2 = maxs2;
    l = l1 + l2;
  /* if ( l > 0 ) -- fixed 20151007.142906 - empty string should be allocated too!! */
    {
      r = _mas_malloc( func, line, l + 1 );
      if ( s1 )
      {
        memcpy( r, s1, l1 );
        _mas_free( func, line, s1 );
      }
      if ( s2 )
        memcpy( r + l1, s2, l2 );
      r[l] = 0;
    }
  }
  return r;
}
# endif

static void
memerror( mas_mem_head_t * m )
{
  fprintf( stderr, "\nMEMORY ERROR 0x%lx > [sig:0x%llx : id:0x%lx]\n", ( unsigned long ) ( m + 1 ), m->sig, m->id );
/* sleep( 98 ); */
  assert( 12 - 12 );
  exit( 11 );
}

#endif

unsigned long memory_allocated = 0;
unsigned long memory_allocated_cnt = 0;
unsigned long memory_freed = 0;
unsigned long memory_freed_cnt = 0;
unsigned long memory_balance = 0;
unsigned long memory_balance_cnt = 0;

/* calloc: The memory is set to zero */
void *
mas_other_calloc( size_t nmemb, size_t size )
{
  return MAS_CALLOC( nmemb, size );
}

void *
mas_other_realloc( void *ptr, size_t new_size )
{
  return MAS_REALLOC( ptr, new_size );
}

void *
mas_other_malloc( size_t size )
{
  return MAS_MALLOC( size );
}

char *
mas_other_strdup( const char *str )
{
  return str ? MAS_STRDUP( str ) : NULL;
}

char *
mas_other_strndup( const char *str, size_t n )
{
  return str ? MAS_STRNDUP( str, n ) : NULL;
}

void
mas_other_free( void *p )
{
  if ( p )
    MAS_FREE( p );
}

#ifdef MAS_TRACEMEM
static int imemar = 0;
static mas_mem_head_t *memar[1024 * 512];
int mas_mem_disable_print_usage = 0;

static int
_print_memlist( const char *func, int line, int fn_f, int s_f, FILE * f, const char *msg1, const char *msg2, const char *msg3, const char *msg4,
                int *prm )
{
  int h = 0;
  int r = -1;

  if ( f && !mas_mem_disable_print_usage )
  {
    int rm = 0;

    r = 0;
    if ( r >= 0 && msg1 )
      r = fprintf( f, msg1 );
    for ( unsigned im = 0; r >= 0 && im < ( sizeof( memar ) / sizeof( memar[0] ) ); im++ )
    {
      if ( memar[im] )
      {
        mas_mem_head_t *data_ptr;

        if ( !h++ )
          r = fprintf( f, "%s:%d %s\n", func, line, msg4 ? msg4 : "MEMORY TABLE" );

        data_ptr = ( mas_mem_head_t * ) memar[im];
        data_ptr++;
        if ( r >= 0 )
        {
          mas_mem_head_t *_mhp;
          unsigned long _id;
          unsigned long _size;
          unsigned _line;
          const char *_func;

          _mhp = memar[im];
          _id = _mhp->id;
          _size = _mhp->size;
          _func = _mhp->func;
          _line = _mhp->line;
          if ( fn_f )
            fprintf( f, "id: %lx; sz:%lu(%lX); %s:%u [%s] [%p]\n", _id, _size, _size, _func, _line, s_f ? ( char * ) data_ptr : "-",
                     ( void * ) data_ptr );
          else
            fprintf( f, "id: %lx; sz:%lu(%lX); fun#%lx:%u [%s] [%p]\n", _id, _size, _size, ( unsigned long ) _func, _line,
                     s_f ? ( char * ) data_ptr : "-", ( void * ) data_ptr );
          rm++;
        }
      }
    }
    if ( !h && r >= 0 && msg2 )
      r = fprintf( f, msg2 );
  /* r = fprintf( f, "** EMPTY MEMORY TABLE ** %s:%u", func, line ); */
    if ( r >= 0 && msg3 )
      r = fprintf( f, msg3 );

  /* if ( r >= 0 )                    */
  /*   r = fprintf( f, "[%d]\n", r ); */
    if ( prm )
      *prm = rm;
  }
  return r;
}

int
print_memlist_vany( const char *func, int line, int cnt, va_list args )
{

  for ( int i = 0; i < cnt; i++ )
  {
    int r = -1;
    FILE *f = NULL;

    f = va_arg( args, FILE * );
    if ( f && ( r = print_memlist( func, line, f ) ) >= 0 )
      return r;
  }
  return -1;
}

int
print_memlist_any( const char *func, int line, int cnt, ... )
{
  int r = -1;
  va_list args;

  va_start( args, cnt );
  r = print_memlist_vany( func, line, cnt, args );
  va_end( args );
  return r;
}

int
print_memlist_msg( const char *func, int line, FILE * f, const char *msg1a, const char *msg1b, const char *msg2, const char *msg3, const char *msg4 )
{
  int r;
  int rm = 0;

  r = _print_memlist( func, line, 0, 0, f, msg1a ? msg1a : "WMWMOMWMWM", msg2 ? msg2 : "<<EMP MEMT>>", msg3, msg4, &rm );
/* _print_memlist( f, func, line, 1, 0 ); */

  if ( rm )
    _print_memlist( func, line, 1, 1, f, msg1b ? msg1b : ( msg1a ? msg1a : "WMWMOMWMWM" ), msg2 ? msg2 : "<<EMP MEMT>>", msg3, msg4, NULL );
  return r;
}

int
print_memlist( const char *func, int line, FILE * f )
{
  int r;

  r = print_memlist_msg( func, line, f, NULL, NULL, NULL, NULL, NULL );
  return r;
}

/********************************************************************************************/
/********************************************************************************************/
mas_memfun_block_t mas_memfun_block = { 0 };

const mas_memfun_block_t mas_memfun_block_z = {
  .mfree = free,
  .mmalloc = malloc,
  .mcalloc = calloc,
  .mrealloc = realloc,
  .mstrdup = strdup,
  .mstrndup = strndup
};

const mas_memfun_block_t mas_memfun_block_o = {
  .mfree = mas_other_free,
  .mmalloc = mas_other_malloc,
  .mcalloc = mas_other_calloc,
  .mrealloc = mas_other_realloc,
  .mstrdup = mas_other_strdup,
  .mstrndup = mas_other_strndup
};

/* void                                                       */
/* mas_memory_allocation_init( const mas_memfun_block_t * m ) */
/* {                                                          */
/* (* if ( ( unsigned long long ) m == 1 )       *)           */
/* (* {                                          *)           */
/* (*   mas_pmemfun_block = &mas_memfun_block_z; *)           */
/* (* }                                          *)           */
/* (* else if ( ( unsigned long long ) m == 2 )  *)           */
/* (* {                                          *)           */
/* (*   mas_pmemfun_block = &mas_memfun_block;   *)           */
/* (* }                                          *)           */
/* (* else                                       *)           */
/*   {                                                        */
/*     mas_pmemfun_block = m;                                 */
/*   }                                                        */
/* }                                                          */

void
_mas_free( const char *func __attribute__ ( ( unused ) ), int line __attribute__ ( ( unused ) ), void *ptr )
{
/* assert( 0 ); */
  if ( mas_pmemfun_block && mas_pmemfun_block->mfree )
  {
    mas_pmemfun_block->mfree( ptr );
  }
  else if ( 1 )
  {
    size_t size;

    if ( ptr )
    {
      mas_mem_head_t *real_ptr;

      real_ptr = ( mas_mem_head_t * ) ptr;
      real_ptr--;
      if ( real_ptr->sig == 0x6cdb7c9c89c7ad3a )
      {
        {
          unsigned long id;

# ifndef MAS_NO_THREADS
          pthread_mutex_lock( &malloc_mutex );
# endif
          id = real_ptr->id;
          if ( id < sizeof( memar ) / sizeof( memar[0] ) )
            memar[id] = 0;
          size = real_ptr->size;
          memory_freed += size;
          memory_freed_cnt++;
          memory_balance -= size;
          memory_balance_cnt--;
          real_ptr->sig = 0xa6cdb7c9c89c7ad3;
        /* real_ptr->size = 0; */
# ifndef MAS_NO_THREADS
          pthread_mutex_unlock( &malloc_mutex );
# endif
        /* memMSG( "@ _mas_free %lx > id:%lx; %lu", ( unsigned long ) real_ptr, id, size ); */
        }
        mas_other_free( real_ptr );
      }
      else
      {
      /* memMSG( "@ _mas_free %lx > id:?; ?", ( unsigned long ) real_ptr ); */
        memerror( real_ptr );
      }
    }
  }
  else
  {
    mas_other_free( ptr );
  }
}

/* calloc: The memory is set to zero */
void *
_mas_calloc( const char *func, int line, size_t nmemb, size_t isize )
{
  void *ptr = NULL;

/* assert( 0 ); */
  if ( mas_pmemfun_block && mas_pmemfun_block->mcalloc )
  {
    ptr = mas_pmemfun_block->mcalloc( nmemb, isize );
  }
  else
  {
    size_t size;

    size = isize * nmemb;
    ptr = _mas_malloc( func, line, size );
    memset( ptr, 0, size );                                          /* calloc: The memory is set to zero - sys doc*/
  }
  return ptr;
}

void *
_mas_malloc( const char *func, int line, size_t size )
{
  void *ptr = NULL;

/* assert( 0 ); */
  if ( mas_pmemfun_block && mas_pmemfun_block->mmalloc )
  {
    ptr = mas_pmemfun_block->mmalloc( size );
  }
  else
  {
    if ( !imemar )
    {
      memset( memar, 0, sizeof( memar ) );
      imemar = 1;
    }
    if ( 1 )
    {
      mas_mem_head_t *real_ptr;
      size_t real_size;

      real_size = size + sizeof( mas_mem_head_t );
      real_ptr = ( mas_mem_head_t * ) mas_other_malloc( real_size );
      if ( real_ptr )
      {
# ifndef MAS_NO_THREADS
        pthread_mutex_lock( &malloc_mutex );
# endif
        memory_allocated += size;
        memory_allocated_cnt++;
        memory_balance += size;
        memory_balance_cnt++;
        real_ptr->sig = 0x6cdb7c9c89c7ad3a;
        real_ptr->id = memory_allocated_cnt;
      /* assert( memory_allocated_cnt < sizeof( memar ) / sizeof( memar[0] ) ); */
        if ( memory_allocated_cnt < sizeof( memar ) / sizeof( memar[0] ) )
          memar[memory_allocated_cnt] = real_ptr;
        real_ptr->func = func;
        real_ptr->line = line;
        real_ptr->size = size;
# ifndef MAS_NO_THREADS
        pthread_mutex_unlock( &malloc_mutex );
# endif
      /* memMSG( "@ _mas_malloc %lx > id:%lx; %lu", ( unsigned long ) real_ptr, real_ptr->id, size ); */

        real_ptr++;
        ptr = ( void * ) real_ptr;
      }
      else
      {
        fprintf( stderr, "\nMALLOC - CAN'T ALLOCATE [%lu]\n", ( unsigned long ) real_size );
        exit( 11 );
      }
    }
    else
    {
      ptr = mas_other_malloc( size );
    }
  }
/* fprintf( stderr, "\nMALLOC 0x%p\n", ptr ); */
  return ptr;
}

void *
_mas_realloc( const char *func, int line, void *ptr, size_t new_size )
{
  char *mem = NULL;

/* assert( 0 ); */
  if ( mas_pmemfun_block && mas_pmemfun_block->mrealloc )
  {
    mem = mas_pmemfun_block->mrealloc( ptr, new_size );
  }
  else if ( 1 )
  {
    mas_mem_head_t *real_ptr = NULL;
    size_t min_size;

    mem = _mas_malloc( func, line, new_size );
    real_ptr = ( mas_mem_head_t * ) mem;
    real_ptr--;
    min_size = new_size;
    if ( ptr )
    {
      size_t old_size;
      mas_mem_head_t *old_real_ptr;

      old_real_ptr = ( mas_mem_head_t * ) ptr;
      old_real_ptr--;
      old_size = old_real_ptr->size;

      if ( old_size < min_size )
        min_size = old_size;
      memcpy( mem, ptr, min_size );
      _mas_free( func, line, ptr );
    }
  }
  else
  {
    mem = mas_other_realloc( ptr, new_size );
  }
  return ( void * ) mem;
}

char *
_mas_strndup( const char *func, int line, const char *str, size_t n )
{
  char *s = NULL;

/* assert( 0 ); */
  if ( mas_pmemfun_block && mas_pmemfun_block->mstrndup )
  {
    s = str ? mas_pmemfun_block->mstrndup( str, n ) : NULL;
  }
  else if ( 1 )
  {
    if ( str )
    {
      size_t l;

      l = strlen( str );
    /* if ( l ) */
      {
        if ( n < l )
          l = n;
        s = _mas_malloc( func, line, l + 1 );
        if ( s )
        {
          if ( l > 0 )
            memcpy( s, str, l );
          s[l] = 0;
        }
      }
    }
  }
  else
  {
    s = mas_other_strndup( str, n );
  }
  return s;
}

char *
_mas_strdup( const char *func, int line, const char *str )
{
  char *s = NULL;

/* assert( 0 ); */
  if ( mas_pmemfun_block && mas_pmemfun_block->mstrdup )
  {
    s = str ? mas_pmemfun_block->mstrdup( str ) : NULL;
  }
  else if ( 1 )
  {
    if ( str )
      s = _mas_strndup( func, line, str, strlen( str ) );
  }
  else
  {
    s = mas_other_strdup( str );
  }
  return s;
}
#endif

#if 0
# if 0

void *__real_malloc( size_t size );
void *
__wrap_malloc( size_t size )
{
  return __real_malloc( size );
}

void __real_free( void *ptr );
void
__wrap_free( void *ptr )
{
  __real_free( ptr );
}

void *__real_realloc( void *ptr, size_t size );
void *
__wrap_realloc( void *ptr, size_t size )
{
  return __real_realloc( ptr, size );
}

# else
/* MUTEX !*/

/*
void __real_strdup( void *ptr );
void
__wrap_strdup( void *ptr )
{
}
*/

void *
__wrap_malloc( size_t size )
{
  void *ptr = NULL;

/* if ( size > 0 ) */
  {
    size_t real_size;
    mas_mem_head_t *real_ptr = NULL;

    real_size = size;
    real_size += sizeof( mas_mem_head_t );
    real_ptr = ( mas_mem_head_t * ) __real_malloc( real_size );

    if ( real_ptr )
    {
      {
        unsigned long id;

        pthread_mutex_lock( &malloc_mutex );
        real_ptr->sig = 0x6cdb7c9c89c7ad3a;
        real_ptr->id = memory_allocated_cnt;
        real_ptr->size = size;
        real_ptr++;

        id = memory_allocated_cnt;
        memory_allocated += size;
        memory_allocated_cnt++;
        memory_balance += size;
        memory_balance_cnt++;
        pthread_mutex_unlock( &malloc_mutex );
        MSG( "@ malloc %lx > id:%lx; %lu", ( unsigned long ) real_ptr, id, size );
      }
      ptr = ( void * ) real_ptr;
    }
  }
  return ptr;
}

void *
__wrap_calloc( size_t nmemb, size_t size )
{
/* MFP( "M%d : cmalloc\n", __LINE__ ); */
  return mas_other_malloc( nmemb * size );
}

void *
__wrap_xmalloc( size_t size )
{
/* MFP( "M%d : xmalloc\n", __LINE__ ); */
  return mas_other_malloc( size );
}

void
__wrap_free( void *ptr )
{
  if ( ptr )
  {
    mas_mem_head_t *real_ptr = NULL;

    real_ptr = ( mas_mem_head_t * ) ptr;
    real_ptr--;
    if ( real_ptr->sig != 0x6cdb7c9c89c7ad3a )
    {
      memerror( real_ptr );
    }
    else
    {
      unsigned long id;
      size_t size = 0;

      pthread_mutex_lock( &malloc_mutex );
      size = real_ptr->size;
      id = real_ptr->id;
      memory_freed += size;
      memory_freed_cnt++;
      memory_balance -= size;
      memory_balance_cnt--;
      pthread_mutex_unlock( &malloc_mutex );
      MSG( "@ free id:%lx; %lu", id, size );
      __real_free( ( void * ) real_ptr );
    }
  }
}

void *
__wrap_realloc( void *ptr, size_t size )
{
  char *mem;
  mas_mem_head_t *real_ptr = NULL;
  size_t min_size;

  mem = mas_other_malloc( size );
  real_ptr = ( mas_mem_head_t * ) mem;
  real_ptr--;
  min_size = size;
  if ( real_ptr->size < min_size )
    min_size = real_ptr->size;
  memcpy( mem, ptr, min_size );
  mas_other_free( ptr );
/* return __real_realloc( ptr, size ); */
  return ( void * ) mem;
}

char *
__wrap___strdup( const char *ptr )
{
  char *new;
  size_t len;

  len = strlen( ptr );
  new = mas_other_malloc( len + 1 );
  mas_other_free( new );
  new = mas_other_malloc( len + 1 );
  memcpy( new, ptr, len + 1 );
/* MFP( "M%d : strdup %p\n", __LINE__, new ); */
  return new;
}
# endif
#endif
