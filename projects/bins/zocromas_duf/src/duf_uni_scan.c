/* #include <stdio.h> */
/* #include <stdlib.h> */
/* #include <stdarg.h> */
#include <string.h>
/* #include <unistd.h> */
/* #include <sys/stat.h> */

/* #include <sys/types.h> */
/* #include <fcntl.h> */
/* #include <unistd.h> */
#include <assert.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_types.h"

#include "duf_utils.h"
#include "duf_service.h"
#include "duf_config.h"


#include "duf_sql.h"
#include "duf_sql_field.h"
#include "duf_levinfo.h"

#include "duf_path.h"

#include "duf_dir_scan.h"

#include "duf_dbg.h"


/* ###################################################################### */
#include "duf_uni_scan.h"
/* ###################################################################### */


/*
 * run   --uni-scan /home/mastar/a/down/ --max-depth=4  --max-items=70 -R --tree --print  --files
 * run   --uni-scan /home/mastar/a/down/ --max-depth=4  --max-items=70 -R --tree --print
*/

/* duf_uni_scan_dir:
 * this is callback of type: duf_scan_callback_file_t (second range):
 *
 * if recursive flag set, call duf_scan_dirs_by_parentid + pdi (built from str_cb_udata) with duf_uni_scan_dir
 *       for each <dir> record by pathid (i.e. children of pathid) with corresponding args 
 * otherwise do nothing
 *
 *   i.e.
 *     1. for <current> dir call sccb->node_scan_before
 *     2. for each leaf in <current> dir call sccb->leaf_scan
 *     3. for <current> dir call sccb->node_scan_middle
 *   recursively from <current> dir (if recursive flag set):
 *     4. for each dir in <current> dir call duf_uni_scan_dir + &di as str_cb_udata
 *     5. for <current> dir call sccb->node_scan_after
 * */
int
duf_uni_scan_dir( void *str_cb_udata, duf_depthinfo_t * xpdi, duf_scan_callbacks_t * sccb, duf_record_t * precord,
                  const duf_dirhandle_t * pdhu )
{
  int r = 0;
  duf_depthinfo_t *pdi;

  pdi = ( duf_depthinfo_t * ) str_cb_udata;

  DUF_UFIELD( dirid );
  /* DUF_SFIELD( dfname ); */
  duf_dbgfunc( DBG_START, __func__, __LINE__ );


  DUF_OINV_OPENED( pdi-> );

  if ( DUF_IF_TRACE( scan ) )
  {
    char *path = duf_pathid_to_path_s( dirid, pdi, &r );

    DUF_OINV_OPENED( pdi-> );


    DUF_TRACE( scan, 0, "TOP B %llu:%s", dirid, path );
    mas_free( path );
  }
  if ( r >= 0 )
  {
    if ( pdi->u.recursive && ( !pdi->u.maxdepth || pdi->depth <= pdi->u.maxdepth ) )
    {
/* duf_scan_fil_by_pi:
 * call duf_uni_scan_dir + pdi (also) as str_cb_udata for each <dir> record by pathid (i.e. children of pathid) with corresponding args
 *
 * i.e. if recursive, call duf_scan_dirs_by_parentid + pdi (built from str_cb_udata) with duf_uni_scan_dir
 *       for each <dir> record by pathid (i.e. children of pathid) with corresponding args 
 *         otherwise do nothing
 *
 *   i.e.
 *     1. for <current> dir call sccb->node_scan_before
 *     2. for each leaf in <current> dir call sccb->leaf_scan
 *     3. for <current> dir call sccb->node_scan_middle
 *   recursively from <current> dir (if recursive flag set):
 *     4. for each dir in <current> dir call str_cb + str_cb_udata
 *     5. for <current> dir call sccb->node_scan_after
 * */

      DUF_OINV_OPENED( pdi-> );
      r = duf_scan_dirs_by_parentid( dirid, pdhu, duf_uni_scan_dir, pdi, sccb, precord );
      DUF_OINV_OPENED( pdi-> );
      /* if ( r == DUF_ERROR_MAX_REACHED )                   */
      /* {                                                   */
      /*   if ( pdi->depth == 0 )                            */
      /*     DUF_TRACE( action, 0, "Maximum reached ...." ); */
      /* }                                                   */
      /* else if ( r < 0 )                                   */
      /* {                                                   */
      /*   DUF_ERROR( "r=%d", r );                           */
      /* }                                                   */
    }
  }
  DUF_TEST_R( r );
  DUF_OINV_OPENED( pdi-> );
  duf_dbgfunc( DBG_END, __func__, __LINE__ );
  return r;
}

/*
 *   i.e. 
 *     1. for <current> dir call sccb->node_scan_before
 *     2. for each leaf in <current> dir call sccb->leaf_scan
 *     3. for <current> dir call sccb->node_scan_middle
 *   recursively from <current> dir (if recursive flag set):
 *     4. for each dir in <current> dir call str_cb + str_cb_udata
 *     5. for <current> dir call sccb->node_scan_after
 */
int
duf_uni_scan( const char *path, duf_ufilter_t u, duf_scan_callbacks_t * sccb )
{
  int r = 0;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );

  DUF_TRACE( scan, 0, "sccb %c", sccb ? '+' : '-' );
  if ( sccb )
  {
    duf_depthinfo_t di = {.depth = 0,
      .seq = 0,
      .levinfo = NULL,
      .u = u,
      /* .name = path, */
    };

    duf_dbgfunc( DBG_STEP, __func__, __LINE__ );

    DUF_OINV( di. );

/* create level-control array, open 0 level */
    r = duf_levinfo_create( &di, path );
    /* if ( !duf_config->cli.noopenat ) */
    /*   r = duf_open_dh( pdh, path );  */
    DUF_TRACE( scan, 0, "di.levinfo %c", di.levinfo ? '+' : '-' );
    DUF_OINV( di. );

    {
      unsigned long long pathid;

      pathid = duf_path_to_pathid( path, &di, &r );
      DUF_TEST_R( r );
      DUF_TRACE( action, 0, "pathid %llu for %s", pathid, path );

      if ( DUF_IF_TRACE( scan ) )
      {
        int tr = 0;
        char *path = duf_pathid_to_path_s( pathid, &di, &tr );

        DUF_TRACE( scan, 0, "TOP %llu:%s", pathid, path );
        mas_free( path );
      }
      DUF_OINV( di. );
      DUF_TEST_R( r );

      if ( r >= 0 && ( pathid || !path ) )
      {
        DUF_TRACE( scan, 0, "%llu:%s  duf_scan_dirs_by_parentid with str_cb=duf_uni_scan_dir(%p)", pathid,
                   path, ( void * ) ( unsigned long long ) duf_uni_scan_dir );
/* duf_uni_scan_dir:
 * if recursive, call duf_scan_dirs_by_parentid + pdi (built from str_cb_udata)
 *       for each <dir> record by pathid (i.e. children of pathid) with corresponding args 
 * otherwise do nothing
 *
 * call duf_uni_scan_dir with pdi for each dir at db by pathid (i.e. children of pathid) 
 *
 *   i.e.
 *     1. for <current> dir call sccb->node_scan_before
 *     2. for each leaf in <current> dir call sccb->leaf_scan
 *     3. for <current> dir call sccb->node_scan_middle
 *   recursively from <current> dir (if recursive flag set):
 *     4. for each dir in <current> dir call duf_uni_scan_dir + &di as str_cb_udata
 *     5. for <current> dir call sccb->node_scan_after
 * */
        DUF_OINV( di. );
        if ( r >= 0 )
          r = duf_scan_dirs_by_parentid( pathid, NULL /* pdh */ , duf_uni_scan_dir, &di, sccb, ( duf_record_t * ) NULL /* precord */  );
        DUF_OINV( di. );
        DUF_TEST_R( r );

        /* if ( r == DUF_ERROR_MAX_REACHED )                 */
        /*   DUF_TRACE( action, 0, "Maximum reached ...." ); */
        /* else if ( r < 0 )                                 */
        /*   DUF_ERROR( "code: %d", r );                     */
      }
/* delete level-control array, close 0 level */
      DUF_OINV( di. );
      duf_levinfo_delete( &di );
      DUF_OINV( di. );
      /* if ( r == DUF_ERROR_MAX_REACHED )                 */
      /*   DUF_TRACE( action, 0, "Maximum reached ...." ); */
      /* else if ( r < 0 )                                 */
      /*   DUF_ERROR( "code: %d", r );                     */
    }

    if ( duf_config->cli.act.totals )
    {
      FILE *f;

      f = duf_config->cli.trace.out ? duf_config->cli.trace.out : stdout;
      fprintf( f, " seq:%llu", di.seq );
      if ( duf_config->u.maxseq )
        fprintf( f, " of %llu", duf_config->u.maxseq );
      fprintf( f, "\n" );
    }

    /* if ( !duf_config->cli.noopenat ) */
    /*   duf_close_dh( pdh );           */
    /* pdh->dfd = 0;                    */
    printf( "\n\n********************************************************** %d **\n", duf_config->cli.act.totals );
  }
  /* if ( r == DUF_ERROR_MAX_REACHED )                 */
  /*   DUF_TRACE( action, 0, "Maximum reached ...." ); */
  /* else if ( r < 0 )                                 */
  /*   DUF_ERROR( "code: %d", r );                     */
  DUF_TEST_R( r );
  duf_dbgfunc( DBG_END, __func__, __LINE__ );
  return r;
}

int
duf_uni_scan_targ( duf_scan_callbacks_t * sccb )
{
  int r = 0;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );
  if ( duf_config )
  {
    if ( r >= 0 && sccb && sccb->init_scan )
      r = sccb->init_scan( sccb );
    DUF_TRACE( action, 0, "inited scan" );
    if ( r >= 0 && ( sccb->node_scan_before || sccb->node_scan_middle || sccb->node_scan_after || sccb->leaf_scan ) )
    {
      DUF_TRACE( action, 0, "targc:%u", duf_config->targc );
      for ( int ia = 0; r >= 0 && ia < duf_config->targc; ia++ )
        DUF_TRACE( action, 0, "targv[%d]='%s'", ia, duf_config->targv[ia] );
      if ( duf_config->targc > 0 )
        for ( int ia = 0; r >= 0 && ia < duf_config->targc; ia++ )
          r = duf_uni_scan( duf_config->targv[ia], duf_config->u, sccb );
      else
        r = duf_uni_scan( NULL, duf_config->u, sccb );
    }
    if ( r >= 0 && sccb && sccb->final_sql_argv )
    {
      char **p = sccb->final_sql_argv;

      while ( r >= 0 && p && *p )
      {
        int changes = 0;

        /* DUF_TRACE( action, 0, "final sql : %s", *p ); */
        DUF_TRACE( action, 0, "final sql" );
        r = duf_sql( *p, &changes );
        p++;
      }
    }
  }
  duf_dbgfunc( DBG_END, __func__, __LINE__ );
  return r;
}

int
duf_uni_scan_all( void )
{
  int r = 0;
  duf_scan_callbacks_t **ppscan_callbacks;
  int max_steps = 100;
  int steps = 0;

  extern duf_scan_callbacks_t duf_integrity_callbacks /* __attribute( ( weak ) ) */ ;
  extern duf_scan_callbacks_t duf_fill_callbacks /* __attribute( ( weak ) ) */ ;
  extern duf_scan_callbacks_t duf_fill_md5_callbacks /* __attribute( ( weak ) ) */ ;
  extern duf_scan_callbacks_t duf_print_md5_callbacks /* __attribute( ( weak ) ) */ ;
  extern duf_scan_callbacks_t duf_print_tree_callbacks /* __attribute( ( weak ) ) */ ;
  extern duf_scan_callbacks_t duf_print_dir_callbacks /* __attribute( ( weak ) ) */ ;
  extern duf_scan_callbacks_t duf_sample_callbacks /* __attribute( ( weak ) ) */ ;
  extern duf_scan_callbacks_t duf_sampupd_callbacks /* __attribute( ( weak ) ) */ ;
  extern duf_scan_callbacks_t duf_fill_mdpath_callbacks /* __attribute( ( weak ) ) */ ;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );

  DUF_TRACE( action, 0, "prep" );
  ppscan_callbacks = mas_malloc( max_steps * sizeof( duf_scan_callbacks_t * ) );
  if ( steps < max_steps && duf_config->cli.act.fill )
  {
    DUF_TRACE( action, 0, "prep fill ..." );
    ppscan_callbacks[steps++] = &duf_fill_callbacks;
  }

  if ( steps < max_steps && duf_config->cli.act.mdpath && duf_config->cli.act.fill )
  {
    DUF_TRACE( action, 0, "prep mdpath" );
    ppscan_callbacks[steps++] = &duf_fill_mdpath_callbacks;
  }
  if ( steps < max_steps && duf_config->cli.act.md5 && duf_config->cli.act.fill )
  {
    DUF_TRACE( action, 0, "prep fill md5" );
    ppscan_callbacks[steps++] = &duf_fill_md5_callbacks;
  }
  if ( steps < max_steps && duf_config->cli.act.integrity )
  {
    DUF_TRACE( action, 0, "prep integrity ..." );
    ppscan_callbacks[steps++] = &duf_integrity_callbacks;
  }

  if ( steps < max_steps && duf_config->cli.act.md5 && duf_config->cli.act.print )
  {
    DUF_TRACE( action, 0, "prep print md5" );
    ppscan_callbacks[steps++] = &duf_print_md5_callbacks;
  }

  if ( steps < max_steps && duf_config->cli.act.print && duf_config->cli.act.tree && !duf_config->cli.act.md5 )
  {
    DUF_TRACE( action, 0, "prep print tree" );
    ppscan_callbacks[steps++] = &duf_print_tree_callbacks;
  }

  if ( steps < max_steps && duf_config->cli.act.print && !duf_config->cli.act.tree && !duf_config->cli.act.md5 )
    ppscan_callbacks[steps++] = &duf_print_dir_callbacks;

  if ( steps < max_steps && duf_config->cli.act.sample )
  {
    for ( int i = 0; i < duf_config->cli.act.sample && steps < max_steps; i++ )
      ppscan_callbacks[steps++] = &duf_sample_callbacks;
  }

  if ( steps < max_steps && duf_config->cli.act.sampupd )
    ppscan_callbacks[steps++] = &duf_sampupd_callbacks;

  for ( int step = 0; r >= 0 && step < steps; step++ )
  {
    if ( ppscan_callbacks[step] )
    {
      const char *title = ppscan_callbacks[step]->title;

      DUF_TRACE( action, 0, "step %d [%s]", step, title );
      r = duf_uni_scan_targ( ppscan_callbacks[step] );
    }
  }
  /* if ( r == DUF_ERROR_MAX_REACHED )                 */
  /*   DUF_TRACE( action, 0, "Maximum reached ...." ); */
  /* else if ( r < 0 )                                 */
  /*   DUF_ERROR( "code: %d", r );                     */
  mas_free( ppscan_callbacks );
  duf_dbgfunc( DBG_ENDR, __func__, __LINE__, r );
  return r;
}