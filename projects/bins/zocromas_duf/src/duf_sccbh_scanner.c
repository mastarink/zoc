/* #undef MAS_TRACING */
#include <assert.h>
#include <string.h>
#include <stddef.h>
#include <errno.h>

#include "duf_tracen_defs_preset.h"                                  /* MAST_TRACE_CONFIG; etc. ✗ */
#include "duf_errorn_defs_preset.h"                                  /* MAST_ERRORS_FILE; etc. ✗ */

#include <mastar/trace/mas_trace.h>
#include <mastar/error/mas_error_defs_ctrl.h>
#include <mastar/error/mas_error_defs_make.h>
#include <mastar/error/mas_error_defs.h>

#include "duf_se_only.h"                                             /* Only DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ✗ */

/* #include "duf_config.h"                                              (* duf_get_config ✗ *) */
#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ✗ */

#include "duf_levinfo_ref.h"                                         /* duf_levinfo_*; etc. ✗ */
/* #include "duf_levinfo_openclose.h" */
/* #include "duf_levinfo_stat.h" */

#include "duf_pdi_structs.h"

#include "duf_sccb.h"
#include "duf_sccb_structs.h"
/* #include "duf_sccb_scanstage.h"                                      (* duf_scanstage_name; duf_scanstage_scanner; ✗ *) */
#include "duf_sccb_row.h"                                            /* datarow_*; duf_sccbh_row_get_*; sccbh_rows_eval ✗ */

#include "duf_sccbh_shortcuts.h"                                     /* H_SCCB; H_PDI; H_* ... ✗ */

/* #include "duf_sccbh_eval.h" */
/* ###################################################################### */
#include "duf_sccbh_scanner.h"
/* ###################################################################### */

SRX( SCCBH, unsigned, has, 0, sccb_has_new_scanner, duf_sccb_handle_t * sccbh, duf_node_type_t nt, duf_scanstage_t scanstage, int db, int dirent )
{
  if ( sccbh && H_SCCB )
    for ( const duf_scanner_set_t * scanner_set = H_SCCB->scanners; scanner_set && scanner_set->fun; scanner_set++ )
    {
      if ( ( db == 0 || ( db == 1 && ( scanner_set->flags & DUF_SCANNER_SET_FLAG_DB ) )
             || ( db == -1 && !( scanner_set->flags & DUF_SCANNER_SET_FLAG_DB ) ) )
           && ( dirent == 0 || ( dirent == 1 && ( scanner_set->flags & DUF_SCANNER_SET_FLAG_DIRENT ) )
                || ( dirent == -1 && !( scanner_set->flags & DUF_SCANNER_SET_FLAG_DIRENT ) ) )
           && ( !( scanner_set->flags & DUF_SCANNER_SET_FLAG_DISABLED ) )
           && ( ( scanner_set->scanstage & scanstage ) || scanner_set->scanstage == DUF_SCANSTAGE_NONE )
           && ( ( scanner_set->type & nt ) || scanner_set->type == DUF_NODE_NONE ) )
      {
        has++;
      }
    }
  ERX( SCCBH, unsigned, has, 0, sccb_has_new_scanner, duf_sccb_handle_t * sccbh, duf_node_type_t nt, duf_scanstage_t scanstage, int db, int dirent );
}

static
SR( SCCBH, sccbh_preset_leaf_scanner, duf_sccb_handle_t * sccbh )
{
  int fd;

  fd = CRX( levinfo_dfd, H_PDI );
  {
    off_t rls MAS_UNUSED = 0;
    off_t rls1 = 0;

    if ( fd > 0 )
    {
      rls = lseek( fd, 0, SEEK_SET );
      if ( ( ( int ) rls ) < 0 )
      {
        QT( "@========================================ERROR %s %d / LEAF", strerror( errno ), fd );
      }
      rls1 = lseek( fd, 0, SEEK_CUR );
      if ( ( ( int ) rls ) < 0 )
      {
        QT( "@========================================ERROR %s %d / LEAF", strerror( errno ), fd );
      }
      assert( fd <= 0 || rls1 == 0 );
    }
  }
  ER( SCCBH, sccbh_preset_leaf_scanner, duf_sccb_handle_t * sccbh );
}

static
SR( SCCBH, sccbh_preset_scanner, duf_sccb_handle_t * sccbh, duf_node_type_t node_type )
{
  H_PDI->items.total++;
/* sccbh->current_scanner = scanner; */
  sccbh->assert__current_node_type = node_type;
  if ( node_type == DUF_NODE_NODE )
  {
    H_PDI->items.dirs++;
  }
  else if ( node_type == DUF_NODE_LEAF )
  {
    H_PDI->items.files++;
  /* QT( "@X %d : %s : %p", H_SCCBI, H_SCCB->name,scanner  ); */
    CRX( sccbh_preset_leaf_scanner, sccbh );
  }
  ER( SCCBH, sccbh_preset_scanner, duf_sccb_handle_t * sccbh, duf_node_type_t node_type );
}

SR( SCCBH, sccbh_call_leaf_pack_scanner, duf_sccb_handle_t * sccbh, duf_scanstage_t scanstage MAS_UNUSED )
{
/* assert(  !sccbh->rows->cnt ||  duf_levinfo_node_type( H_PDI ) == DUF_NODE_LEAF ); */
  CRX( sccbh_preset_leaf_scanner, sccbh );
  {
    int eq = 0;

  /* duf_sccb_data_row_t *new_row = NULL; */

  /* sccbh->new_row = CRX( datarow_create, pstmt_arg, CRX( pdi_pathinfo, H_PDI ) ); */
  /* QT( "@A %d : %d", sccbh->pdi->pathinfo.levinfo[17].node_type, new_row->pathinfo.levinfo[17].node_type ); */
    if ( sccbh->rows && sccbh->rows->prev )
    {
      unsigned long long sha1id0;
      unsigned long long sha1id1;

      sha1id0 = CRX( datarow_get_number, sccbh->rows, "sha1id" );
      sha1id1 = CRX( datarow_get_number, sccbh->rows->prev, "sha1id" );
    /* QT( "@B %d : %d", sccbh->pdi->pathinfo.levinfo[17].node_type, sccbh->new_row->pathinfo.levinfo[17].node_type ); */
      eq = ( sha1id0 == sha1id1 );
      {
        if ( !eq )
        {
          MAST_TRACE( temp, 5, "@@---A %lld ? %lld : %p:%p", sha1id0, sha1id1, sccbh->rows, sccbh->rows->prev );
        /* assert(0); */
          CRX( sccbh_rows_eval, sccbh );
        }
      }
    }

  /* if ( sccbh->new_row )                 */
  /* {                                     */
  /*   sccbh->new_row->prev = sccbh->rows; */
  /*   sccbh->rows = sccbh->new_row;       */
  /*   sccbh->new_row = NULL;              */
  /* }                                     */
  }
  assert( H_PDI == sccbh->pdi );
/* CRV( ( scanner ), pstmt_arg, H_PDI, sccbh ); */
/* if ( sccbh->atom_cb )                                            (* atom is fs-direntry(dir or reg) or item(node or leaf) *) */
/* sccbh->atom_cb( sccbh, pstmt_arg, scanstage, scanner, node_type, QERRIND ); */

  ER( SCCBH, sccbh_call_leaf_pack_scanner, duf_sccb_handle_t * sccbh, duf_scanstage_t scanstage );
}

SR( SCCBH, sccbh_call_scanner, duf_sccb_handle_t * sccbh, duf_stmnt_t * pstmt_arg MAS_UNUSED, duf_scanstage_t scanstage, duf_scanner_fun_t scanner,
    duf_node_type_t node_type )
{
  if ( scanner )
  {
    CRX( sccbh_preset_scanner, sccbh, node_type );
    assert( H_PDI == sccbh->pdi );
    CRV( ( scanner ), /* pstmt_arg, */ H_PDI, sccbh );
    if ( sccbh->atom_cb )                                            /* atom is fs-direntry(dir or reg) or item(node or leaf) */
      sccbh->atom_cb( sccbh, /* pstmt_arg, */ scanstage, scanner, node_type, QERRIND );
    assert( sccbh->assert__current_node_type == node_type );
  }
/* QT( "@@@ %p scanstage: %s @ %s (%s)", scanner, CRX(scanstage_name, scanstage ), CRX(uni_scan_action_title, H_SCCB ), QERRNAME ); */
  ER( SCCBH, sccbh_call_scanner, duf_sccb_handle_t * sccbh, duf_stmnt_t * pstmt_arg, duf_scanstage_t scanstage, duf_scanner_fun_t scanner,
      duf_node_type_t node_type );
}