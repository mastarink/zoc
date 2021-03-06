/* #undef MAS_TRACING */
#include <assert.h>
#include <string.h>
#include <stdlib.h>                                                  /* free */

#include <dirent.h>                                                  /* struct dirent  */
#include <errno.h>

#include "duf_tracen_defs_preset.h"                                  /* MAST_TRACE_CONFIG; etc. ✗ */
#include "duf_errorn_defs_preset.h"                                  /* MAST_ERRORS_FILE; etc. ✗ */

#include <mastar/wrap/mas_std_def.h>
#include <mastar/trace/mas_trace.h>
#include <mastar/error/mas_error_defs_ctrl.h>
#include <mastar/error/mas_error_defs_make.h>
#include <mastar/error/mas_error_defs.h>

#include "duf_se_only.h"                                             /* Only DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ✗ */

#include "duf_config.h"                                              /* duf_get_config ✗ */
#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ✗ */

#include "duf_pdi_ref.h"
#include "duf_pdi_pi_ref.h"                                          /* duf_pdi_levinfo; duf_pdi_*depth; ✗ */

#include "duf_levinfo_ref.h"                                         /* duf_levinfo_*; etc. ✗ */
#include "duf_levinfo_updown.h"
#include "duf_levinfo_openclose.h"
#include "duf_levinfo_stat.h"
#include "duf_levinfo_structs.h"

#include "duf_sccb_def.h"
#include "duf_sccb.h"

#include "duf_dirent.h"

#include "duf_sccb_scanstage.h"                                      /* duf_scanstage_name; duf_scanstage_scanner; ✗ */
#include "duf_sccb_structs.h"

#include "duf_sccbh_structs.h"                                       /* duf_sccb_handle_s (from duf_sccbh_types: duf_sccb_handle_t; duf_sccbh_fun_t; duf_rsccbh_fun_t) ✗ */
#include "duf_sccbh_ref.h"
#include "duf_sccbh_shortcuts.h"                                     /* H_SCCB; H_PDI; H_* ... ✗ */
#include "duf_sccbh_scanner.h"

#include "duf_nodetype.h"                                            /* duf_nodetype_name ✗ */

#include "duf_pdi_structs.h"
/* ###################################################################### */
#include "duf_sccbh_eval_fs.h"
/* ###################################################################### */

/* 20151027.144614 */
static
SR( SCCBH, sccbh_eval_fs_w_scanner_here, duf_sccb_handle_t * sccbh, /* duf_stmnt_t * pstmt_unused MAS_UNUSED, */ duf_scanstage_t scanstage MAS_UNUSED,
    duf_scanner_fun_t scanner )
{
  CR( levinfo_statat_dh, H_PDI );

  if ( QNOERR )
  {
  /* call hook frmo mod_ */
  /* if ( scanner ) */
    {
    /* sccbh->current_scanner = scanner; */
      MAST_TRACE( scan, 2, "@@@@@@@%s +%d :: %s | %s", CRX( nodetype_name, CRX( levinfo_node_type, H_PDI ) ), CRX( pdi_depth, H_PDI ),
                  CRX( levinfo_relpath, H_PDI ), CRX( levinfo_itemtruename, H_PDI ) );
      CRV( ( scanner ), /* ( duf_stmnt_t * ) NULL, */ H_PDI, sccbh );
      {
        duf_levinfo_t *pli = NULL;

        pli = CRX( levinfo_ptr_up, H_PDI );
        if ( pli )
        {
          if ( CRX( levinfo_node_type, H_PDI ) == DUF_NODE_NODE )
            pli->scanned_childs.nodes++;
          else if ( CRX( levinfo_node_type, H_PDI ) == DUF_NODE_LEAF )
            pli->scanned_childs.leaves++;
          else
          {
            assert( 0 );
          }
        }
      }
      /* assert( sccbh->assert__current_node_type == DUF_NODE_FS ); */
      if ( sccbh->atom_cb )                                          /* atom is fs-direntry(dir or reg) or item(node or leaf) */
        sccbh->atom_cb( sccbh, /* ( duf_stmnt_t * ) NULL, */ scanstage, scanner, DUF_NODE_FS, QERRIND );
    }
  }
  else if ( QISERR_N( STATAT_ENOENT ) )
  {
  /* DUF_SHOW_ERROR( "No such entry %s/%s", CRX(levinfo_path, H_PDI ), CRX(levinfo_itemshowname, H_PDI ) ); */
    ERRMAKE_M( STAT, "No such entry %s/%s", CRX( levinfo_path, H_PDI ), CRX( levinfo_itemshowname, H_PDI ) );
  }
  ER( SCCBH, sccbh_eval_fs_w_scanner_here, duf_sccb_handle_t * sccbh, /* duf_stmnt_t * pstmt_unused MAS_UNUSED, */ duf_scanstage_t scanstage MAS_UNUSED,
      duf_scanner_fun_t scanner );
}

/* 20151027.104729 */
static
SR( SCCBH, sccbh_eval_fs_direntry, duf_sccb_handle_t * sccbh, /* duf_stmnt_t * pstmt_unused MAS_UNUSED, */ struct dirent *de,
    duf_scanstage_t scanstage MAS_UNUSED )
{
  duf_node_type_t nt;

  nt = ( de->d_type == DT_DIR ) ? DUF_NODE_NODE : DUF_NODE_LEAF;
/* --> */
  CR( levinfo_godown, H_PDI, de->d_name, nt );
  {
    duf_scanner_fun_t scanner;

    scanner = CRX( sccb_scanstage_scanner, H_SCCB, DUF_SCANSTAGE_FS_ITEMS, 0, nt );
  /* assert(CRX(sccb_scanstage_scanner, H_SCCB, DUF_SCANSTAGE_FS_ITEMS, 0, nt )); */
  /* QT( "SCANNER: %d : %s", CRX(sccb_scanstage_scanner, H_SCCB, DUF_SCANSTAGE_FS_ITEMS, 0, nt ) ? 1 : 0, CRX(uni_scan_action_title, H_SCCB ) ); */
    if ( scanner )
    {
      MAST_TRACE( scan, 2, "@@@@%s +%d :: %s | %-17s", CRX( nodetype_name, CRX( levinfo_node_type, H_PDI ) ), CRX( pdi_depth, H_PDI ),
                  CRX( levinfo_relpath, H_PDI ), CRX( levinfo_itemtruename, H_PDI ) );
      CR( sccbh_eval_fs_w_scanner_here, sccbh, /* pstmt_unused, */ scanstage, scanner );
    }
  }
/* <-- */
  CR( levinfo_goup, H_PDI );

  ER( SCCBH, sccbh_eval_fs_direntry, duf_sccb_handle_t * sccbh, /* duf_stmnt_t * pstmt_unused MAS_UNUSED, */ struct dirent *de,
      duf_scanstage_t scanstage );
}

static
SR( SCCBH, sccbh_eval_fs_direntry_scanner_set, duf_sccb_handle_t * sccbh, /* duf_stmnt_t * pstmt_unused MAS_UNUSED, */ struct dirent *de,
    duf_scanstage_t scanstage MAS_UNUSED )
{
  duf_node_type_t nt;

  nt = ( de->d_type == DT_DIR ) ? DUF_NODE_NODE : DUF_NODE_LEAF;
/* --> */
  CR( levinfo_godown, H_PDI, de->d_name, nt );

  for ( const duf_scanner_set_t * scanner_set = H_SCCB->scanners; scanner_set && scanner_set->fun; scanner_set++ )
  {
    if ( ( scanner_set->flags & DUF_SCANNER_SET_FLAG_DIRENT )
         && !( scanner_set->flags & DUF_SCANNER_SET_FLAG_DISABLED )
         && ( ( scanner_set->scanstage & scanstage ) || scanner_set->scanstage == DUF_SCANSTAGE_NONE )
         && ( ( scanner_set->type & nt ) || scanner_set->type == DUF_NODE_NONE ) )
    {
      CR( sccbh_eval_fs_w_scanner_here, sccbh, /* pstmt_unused, */ scanstage, scanner_set->fun );
    }
  }
/* <-- */
  CR( levinfo_goup, H_PDI );

  ER( SCCBH, sccbh_eval_fs_direntry_scanner_set, duf_sccb_handle_t * sccbh, /* duf_stmnt_t * pstmt_unused MAS_UNUSED, */ struct dirent *de,
      duf_scanstage_t scanstage );
}

static
SR( SCCBH, sccbh_eval_fs_direntry_new, duf_sccb_handle_t * sccbh, /* duf_stmnt_t * pstmt_unused MAS_UNUSED, */ struct dirent *de,
    duf_scanstage_t scanstage MAS_UNUSED )
{
  CR( sccbh_eval_fs_direntry, sccbh, /* pstmt_unused, */ de, scanstage );
  CR( sccbh_eval_fs_direntry_scanner_set, sccbh, /* pstmt_unused, */ de, scanstage );
  ER( SCCBH, sccbh_eval_fs_direntry_new, duf_sccb_handle_t * sccbh, /* duf_stmnt_t * pstmt_unused MAS_UNUSED, */ struct dirent *de,
      duf_scanstage_t scanstage MAS_UNUSED );
}

#if 0
/* 20151013.130021 */
SR( SCCBH, sccbh_eval_fs_w2scanners_sd, duf_scanstage_t scanstage MAS_UNUSED, duf_stmnt_t * pstmt_unused MAS_UNUSED, duf_sccb_handle_t * sccbh )
{
  int ry = 0;
  struct dirent **list = NULL;

/* QT( "@@@@%s", CRX(levinfo_path, H_PDI ) ); */
  ry = scandirat( CRX( levinfo_dfd, H_PDI ), ".", &list, duf_direntry_filter, NULL /* alphasort */  );
  if ( ry >= 0 )
  {
    int nlist = ry;

    for ( int il = 0; il < nlist; il++ )
    {
      MAST_TRACE( scan, 2, "scan dirent %d: %s", il, list[il]->d_name );
      CR( sccbh_eval_fs_direntry, sccbh, pstmt_unused, list[il], scanstage );

      if ( list[il] )
        free( list[il] );
    }
    MAST_TRACE( scan, 10, "passed scandirat='.../%s'", CRX( levinfo_itemshowname, H_PDI ) );
    if ( list )
      free( list );
  }
  else
  {
  /* system level: scandirat - register errors */
    int errorno = errno;

    if ( !CRX( levinfo_path, H_PDI ) )
      ERRMAKE( PATH );
    else if ( errorno != EACCES )
    {
    /* DUF_ERRSYSE( errorno, "(%d) path '%s'/'%s'", r, CRX(levinfo_path_q, H_PDI, "?" ), CRX(levinfo_itemshowname, H_PDI ) ); */
      ERRMAKE_M( SCANDIR, "(%d) path '%s'/'%s'", errorno, CRX( levinfo_path_q, H_PDI, "?" ), CRX( levinfo_itemshowname, H_PDI ) );
    }
    else
    {
    /* ??? */
    /* DUF_ERRSYSE( errorno, "(%d) path '%s'/'%s'", r, CRX(levinfo_path_q, H_PDI, "?" ), CRX(levinfo_itemshowname, H_PDI ) ); */
      ERRMAKE_M( SCANDIR, "(%d) path '%s'/'%s'", errorno, CRX( levinfo_path_q, H_PDI, "?" ), CRX( levinfo_itemshowname, H_PDI ) );
    }
  }
  ER( SCCBH, sccbh_eval_fs_w2scanners_sd, duf_scanstage_t scanstage MAS_UNUSED, duf_stmnt_t * pstmt_unused MAS_UNUSED, duf_sccb_handle_t * sccbh );
}
#endif

static
SR( SCCBH, sccbh_eval_fs_w2scanners_rd, duf_scanstage_t scanstage MAS_UNUSED, /* duf_stmnt_t * pstmt_unused MAS_UNUSED, */ duf_sccb_handle_t * sccbh )
{
  DIR *dh = NULL;

  dh = opendir( CRX( levinfo_path, H_PDI ) );
  if ( dh )
  {
    struct dirent *de;
    int il = 0;

    while ( ( de = readdir( dh ) ) )
    {
      if ( CRX( direntry_filter, de ) )
      {
        MAST_TRACE( scan, 2, "@@@@@@dirent %s", de->d_name );
#if 0
        CR( sccbh_eval_fs_direntry, sccbh, pstmt_unused, de, scanstage );
#else
        CR( sccbh_eval_fs_direntry_new, sccbh, /* pstmt_unused, */ de, scanstage );
#endif
        il++;
      }
    }
    closedir( dh );
  }
  else
  {
    int errorno = errno;

    if ( !CRX( levinfo_path, H_PDI ) )
      ERRMAKE( PATH );
    else if ( errorno != EACCES )
    {
    /* DUF_ERRSYSE( errorno, "(%d) path '%s'/'%s'", r, CRX(levinfo_path_q, H_PDI, "?" ), CRX(levinfo_itemshowname, H_PDI ) ); */
      ERRMAKE_M( SCANDIR, "(%d) path '%s'/'%s'", errorno, CRX( levinfo_path_q, H_PDI, "?" ), CRX( levinfo_itemshowname, H_PDI ) );
    }
    else
    {
    /* ??? */
    /* DUF_ERRSYSE( errorno, "(%d) path '%s'/'%s'", r, CRX(levinfo_path_q, H_PDI, "?" ), CRX(levinfo_itemshowname, H_PDI ) ); */
      ERRMAKE_M( SCANDIR, "(%d) path '%s'/'%s'", errorno, CRX( levinfo_path_q, H_PDI, "?" ), CRX( levinfo_itemshowname, H_PDI ) );
    }
  }
  ER( SCCBH, sccbh_eval_fs_w2scanners_rd, duf_scanstage_t scanstage MAS_UNUSED, /* duf_stmnt_t * pstmt_unused MAS_UNUSED, */
      duf_sccb_handle_t * sccbh );
}

/* 20151013.130037 */
static
SR( SCCBH, sccbh_eval_fs_in, duf_sccb_handle_t * sccbh, /* duf_stmnt_t * pstmt_unused MAS_UNUSED, */ duf_scanstage_t scanstage MAS_UNUSED )
{
  if ( H_SCCB->dirent_dir_scan_before2 || H_SCCB->dirent_file_scan_before2
       || CRX( sccb_has_new_scanner, sccbh, DUF_NODE_NODE | DUF_NODE_LEAF, scanstage, 0 /* db */ , 1 /* dirent */  ) )
  {
  /* assert( CRX(pdi_opendir,H_PDI) == 1 ); */
    CRX( pdi_set_opendir, H_PDI, 1 );                                /* TODO */
    MAST_TRACE( scan, 2, "@@@@@@@´%s´ : %s to scan FS at %s : %s", CRX( scanstage_name, scanstage ),
                CRX( nodetype_name, CRX( levinfo_node_type, H_PDI ) ), CRX( levinfo_relpath, H_PDI ), CRX( levinfo_itemtruename, H_PDI ) );
    {
      {
        ERRLOWER( STATAT_ENOENT );
        CR( levinfo_if_statat_dh, H_PDI );
        ERRUPPER( STATAT_ENOENT );
      }
      assert( QISERR || CRX( levinfo_stat, H_PDI ) );

/* check if parent really existing directory - by st_dir : S_ISDIR(st_dir.st_mode) */
      if ( S_ISDIR( CRX( levinfo_stat_mode, H_PDI ) ) )
      {
        CR( levinfo_if_openat_dh, H_PDI );
        /* sccbh->assert__current_node_type = DUF_NODE_FS; */
#if 0
        DOR( r, duf_sccbh_eval_fs_w2scanners_sd( scanstage, pstmt_unused, sccbh ) );
#else
        CR( sccbh_eval_fs_w2scanners_rd, scanstage, /* pstmt_unused, */ sccbh );
#endif
      }
      else
      {
      /* system level: S_ISDIR - register errors */
/* no such entry */
      /* DUF_SHOW_ERROR( "No such entry '%s'/'%s'", CRX(levinfo_path_q, H_PDI, "?" ), CRX(levinfo_itemshowname, H_PDI ) ); */
      /* TODO mark as absent or remove from db */

      /* MAST_TRACE( scan, 0, "No such entry %s", CRX(levinfo_itemshowname, H_PDI ) ); */
        ERRMAKE_M( STAT, "No such entry '%s'/'%s'", CRX( levinfo_path_q, H_PDI, "?" ), CRX( levinfo_itemshowname, H_PDI ) );
      }
    }
  }
  ER( SCCBH, sccbh_eval_fs_in, duf_sccb_handle_t * sccbh, /* duf_stmnt_t * pstmt_unused MAS_UNUSED, */ duf_scanstage_t scanstage MAS_UNUSED );
}

/* 20151027.114354 */
/*
 * call corresponding callback (by dir/regular)
 *   for each direntry from filesystem with necessary info:
 *
 * call from CRX(sccbh_eval_all,_wrap)
 *
 * fn of type: duf_str_cb2_t
 * */
SR( SCCBH, sccbh_eval_fs, duf_sccb_handle_t * sccbh, /* duf_stmnt_t * pstmt_unused MAS_UNUSED, */ duf_scanstage_t scanstage MAS_UNUSED )
{
  assert( H_PDI );

  DUF_SCCB_PDI( MAST_TRACE, scan, 10 + CRX( pdi_reldepth, H_PDI ), H_PDI, " >>>q +dirent" );
  MAST_TRACE( scan, 4, "@scan dirent by %5llu:%s; %s", CRX( levinfo_dirid, H_PDI ), CRX( uni_scan_action_title, H_SCCB ),
              CRX( levinfo_path, H_PDI ) );

  MAST_TRACE( sccbh, 2, "(%s) stat (%s) %s", QERRNAME, CRX( uni_scan_action_title, H_SCCB ), H_SCCB->name );

/* assert( CRX(levinfo_dfd, H_PDI ) ); */
/*
 *   -- call for each direntry
 *      - for directory                - sccb->dirent_dir_scan_before2
 *      - for other (~ regular) entry  - sccb->dirent_file_scan_before2
 * XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX */

  CR( sccbh_eval_fs_in, sccbh, /* pstmt_unused, */ scanstage );
  MAST_TRACE( sccbh, 2, "(%s) stat (%s) %s", QERRNAME, CRX( uni_scan_action_title, H_SCCB ), H_SCCB->name );

  ER( SCCBH, sccbh_eval_fs, duf_sccb_handle_t * sccbh, /* duf_stmnt_t * pstmt_unused MAS_UNUSED, */ duf_scanstage_t scanstage MAS_UNUSED );
}
