/* #undef MAS_TRACING */
#include <assert.h>
#include <string.h>
#include <dlfcn.h>

#include "duf_tracen_defs_preset.h"                                  /* MAST_TRACE_CONFIG; etc. ✗ */
#include "duf_errorn_defs_preset.h"                                  /* MAST_ERRORS_FILE; etc. ✗ */

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>                                  /* mas_(malloc|free|strdup); etc. ▤ */
#include <mastar/tools/mas_arg_tools.h>                              /* mas_strcat_x; etc. ▤ */
#include <mastar/tools/mas_utils_path.h>                             /* mas_normalize_path; mas_pathdepth; mas_realpath etc. ▤ */
#include <mastar/trace/mas_trace.h>
#include <mastar/error/mas_error_defs_ctrl.h>
#include <mastar/error/mas_error_defs_make.h>
#include <mastar/error/mas_error_defs.h>

#include "duf_se_only.h"                                             /* Only DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ✗ */

#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ✗ */

#include "duf_sccbh_shortcuts.h"                                     /* H_SCCB; H_PDI; H_* ... ✗ */
#include "std_mod_sets.h"

#include "duf_mod_handle.h"

#include "duf_sccb_structs.h"                                        /* duf_scan_callbacks_s; duf_sccb_data_row_s; duf_scanner_fun_s; ✗ */

/* Working with sccb structure */
/* ###################################################################### */
#include "duf_sccb.h"
/* ###################################################################### */

SRX( OTHER, int, n, -1, set_name2set_index, const char *set_name, duf_sql_set_t * std_sets, int nsets )
{
  if ( set_name )
    for ( unsigned i = 0; i < ( unsigned ) nsets; i++ )
      if ( std_sets[i].name && 0 == strcmp( std_sets[i].name, set_name ) )
        n = i;
  ERX( OTHER, int, n, -1, set_name2set_index, const char *set_name, duf_sql_set_t * std_sets, int nsets );
}

SRX( OTHER, const duf_sql_set_t *, set, NULL, set_index2set, unsigned set_index, duf_sql_set_t * std_sets, int nsets )
{
  set = ( set_index <= ( unsigned ) nsets ) ? &std_sets[set_index - 1] : NULL;
  ERX( OTHER, const duf_sql_set_t *, set, NULL, set_index2set, unsigned set_index, duf_sql_set_t * std_sets, int nsets );
}

SRX( OTHER, const duf_sql_set_t *, set, NULL, set_name2set, const char *set_name, duf_sql_set_t * std_sets, int nsets )
{
  int n;

  n = CRX( set_name2set_index, set_name, std_sets, nsets );
  if ( n >= 0 )
    set = &std_sets[n];
  ERX( OTHER, const duf_sql_set_t *, set, NULL, set_name2set, const char *set_name, duf_sql_set_t * std_sets, int nsets );
}

/* TODO rename _duf_uni_scan_action_title => _duf_sccb_title */
static const char *
uni_scan_action_title( const duf_scan_callbacks_t * sccb )
{
  const char *stitle = NULL;

  if ( sccb && sccb->title )
  {
    stitle = strrchr( sccb->title, '/' );
    if ( stitle )
      stitle++;
    else
      stitle = sccb->title;
  }
  else
  {
    stitle = "";
  }
  return stitle;
}

/* TODO rename duf_uni_scan_action_title => duf_sccb_title */
const char *
duf_uni_scan_action_title( const duf_scan_callbacks_t * sccb )
{
  static char tbuf[1024];

  snprintf( tbuf, sizeof( tbuf ), "◁ %s ▷", uni_scan_action_title( sccb ) );
  return tbuf;
}

/* 20160324.113111 */
static
SRX( SCCB, const duf_scan_callbacks_t *, sccb, NULL, find_sccb_by_evnamen, const char *name, size_t namelen, const duf_scan_callbacks_t * first )
{
/* const duf_scan_callbacks_t *sccb = NULL; */

#if 1
  for ( sccb = first; sccb && strncmp( name, sccb->name, namelen ); sccb = sccb->next );
#else
  sccb = first;
  while ( sccb && strncmp( name, sccb->name, namelen ) )
    sccb = sccb->next;
#endif
/* return sccb; */
  ERX( SCCB, const duf_scan_callbacks_t *, sccb, NULL, find_sccb_by_evnamen, const char *name, size_t namelen, const duf_scan_callbacks_t * first );
}

static
SRP( MOD, duf_scan_callbacks_t *, sccb, NULL, load_sccb_symbol, const char *path, const char *symbol )
{
/* duf_scan_callbacks_t *sccb = NULL; */

  MAST_TRACE( sccb, 0, "@@@@@@to load %s", path );
  sccb = ( duf_scan_callbacks_t * ) CRP( load_symbol, path, symbol, NULL );
  MAST_TRACE( sccb, 0, "%s : %s", symbol, sccb ? sccb->name : NULL );
/* return sccb; */
  ERP( MOD, duf_scan_callbacks_t *, sccb, NULL, load_sccb_symbol, const char *path, const char *symbol );
}

static void
duf_register_sccb( duf_scan_callbacks_t * first, duf_scan_callbacks_t * sccb )
{
  if ( sccb )
  {
    duf_scan_callbacks_t *prev = NULL;

  /* sccb->dlhan = han; */
  /* MAST_TRACE( sccb, 0, "[%p] %s => %s", han, path, sccb->name ); */
    for ( duf_scan_callbacks_t * tsccb = first; tsccb; tsccb = tsccb->next )
      prev = tsccb;
    if ( prev )
    {
      prev->next = sccb;
    /* MAST_TRACE( sccb, 0, "[han:%p] : %s", han, sccb ? sccb->name : NULL ); */
    }
    else
    {
    /* dlclose ?? */
    }

    {
      assert( sccb->use_std_leaf_set_num == -1 || CRX( set_name2set, sccb->std_leaf_set_name, std_leaf_sets, std_leaf_nsets ) ==
              CRX( set_index2set, sccb->use_std_leaf_set_num, std_leaf_sets, std_leaf_nsets ) );
      assert( sccb->use_std_node_set_num == -1 || CRX( set_name2set, sccb->std_node_set_name, std_node_sets, std_node_nsets ) ==
              CRX( set_index2set, sccb->use_std_node_set_num, std_node_sets, std_node_nsets ) );
    }
  }
}

static
SRX( SCCB, duf_scan_callbacks_t *, sccb, NULL, load_sccb_by_evnamen, const char *name, size_t namelen, duf_scan_callbacks_t * first )
{
/* duf_scan_callbacks_t *sccb = NULL; */

  if ( name && namelen )
  {
    char *libname;

    libname = mas_strndup( name, namelen );
    if ( libname && first )
    {
      sccb = CRP( load_mod_handler_symbol_find, libname, "sccb" );
      if ( !sccb )
        sccb = CRP( load_sccb_symbol, libname, "duf_mod_sccb_handler" );
    /* QT( "@A sccb:%p (%s)", sccb, "duf_mod_sccb_handler" ); */
      assert( 0 == strcmp( libname, sccb->name ) );
      if ( !sccb )
      {
        char *symbol = NULL;

        symbol = mas_strdup( "duf_" );
        symbol = mas_strcat_x( symbol, libname );
        symbol = mas_strcat_x( symbol, "_callbacks" );
        sccb = CRP( load_sccb_symbol, libname, symbol );
        mas_free( symbol );
      /* QT( "@B sccb:%p (%s)", sccb, symbol ); */
      }
      if ( QNOERR && sccb )
        duf_register_sccb( first, sccb );

    }
    mas_free( libname );
  }
  MAST_TRACE( sccb, 0, "loaded %s", sccb ? sccb->name : NULL );
/* return sccb; */
  ERX( SCCB, duf_scan_callbacks_t *, sccb, NULL, load_sccb_by_evnamen, const char *name, size_t namelen, duf_scan_callbacks_t * first );
}

SRX( SCCB, const duf_scan_callbacks_t *, sccb, NULL, find_or_load_sccb_by_evnamen, const char *name, size_t namelen, duf_scan_callbacks_t * first )
{
/* const duf_scan_callbacks_t *sccb = NULL; */

  sccb = CRX( find_sccb_by_evnamen, name, namelen, first );
  if ( !sccb )
    sccb = CRX( load_sccb_by_evnamen, name, namelen, first );
/* return sccb; */
  ERX( SCCB, const duf_scan_callbacks_t *, sccb, NULL, find_or_load_sccb_by_evnamen, const char *name, size_t namelen, duf_scan_callbacks_t * first );
}

SRX( SCCB, const duf_scan_callbacks_t **, psccb, NULL, find_or_load_sccb_by_evnamen_plus, const char *name0, size_t namelen,
     duf_scan_callbacks_t * first )
{
/* const duf_scan_callbacks_t **psccb = NULL; */
  size_t cnt = 0;
  char *name = NULL;
  const char *p;
  const char *sname;

  name = mas_strndup( name0, namelen );
  for ( cnt = 0, p = name; p < name + namelen && p && *p; p++, cnt++ )
  {
  /* QT( "@-- %lu: %s", cnt, p ); */
    sname = p;
    p = strchr( p, '+' );
    if ( !p )
      p = sname + strlen( sname );
  /* QT( "@++ %lu: %s ~ %lu", cnt, sname, p - sname ); */
  }
  if ( cnt )
  {
    cnt += 2;
    psccb = mas_malloc( sizeof( const duf_scan_callbacks_t ) * cnt );
    memset( psccb, 0, sizeof( const duf_scan_callbacks_t ) * cnt );
    for ( cnt = 0, p = name; p < name + namelen && p && *p; p++, cnt++ )
    {
      sname = p;
      p = strchr( p, '+' );
      if ( !p )
        p = sname + strlen( sname );
      psccb[cnt] = CRX( find_or_load_sccb_by_evnamen, sname, p - sname, first );
    /* QT( "@!! %lu: %s ~ %lu = %p", cnt, sname, p - sname, psccb[cnt] ); */
    }
  }
  mas_free( name );
/* while ( p < name + namelen && p && *p ); */
/* return psccb; */
  ERX( SCCB, const duf_scan_callbacks_t **, psccb, NULL, find_or_load_sccb_by_evnamen_plus, const char *name0, size_t namelen,
       duf_scan_callbacks_t * first );
}

SRN( SCCB, void, sccb_dlclose, duf_scan_callbacks_t * first )
{
  for ( duf_scan_callbacks_t * sccb = first; sccb; sccb = sccb->next )
  {
    if ( sccb->dlhan )
      dlclose( sccb->dlhan );
    sccb->dlhan = NULL;
  }
  ERN( SCCB, void, sccb_dlclose, duf_scan_callbacks_t * first );
}

SRX( SCCB, const duf_scan_callbacks_t *, sccb, NULL, find_sccb_by_evname, const char *name, const duf_scan_callbacks_t * first )
{
/* const duf_scan_callbacks_t *sccb = NULL; */

  sccb = duf_find_sccb_by_evnamen( name, strlen( name ), first );
/* return sccb; */
  ERX( SCCB, const duf_scan_callbacks_t *, sccb, NULL, find_sccb_by_evname, const char *name, const duf_scan_callbacks_t * first );
}
