#include <string.h>
#include <stddef.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance_options.h"

#include "duf_xtended_table.h"
#include "duf_option_stage.h"   /* duf_optstage_name */
#include "duf_option_class.h"
#include "duf_option_val.h"
#include "duf_option_vtype.h"

/* ###################################################################### */
#include "duf_option_extended.h"
/* ###################################################################### */

/* TODO rename file ...... */

const char *
duf_offset2stringid( unsigned offset, duf_offset_to_t relto )
{
  const char *rs = NULL;

  switch ( relto )
  {
  case DUF_OFFSET_none:
    break;
  case DUF_OFFSET_config:
    {
      switch ( offset )
      {
      case offsetof( duf_config_t, help_string ):
        rs = "help_string";
        break;
      case offsetof( duf_config_t, loadtime ):
        rs = "loadtime";
        break;
      case offsetof( duf_config_t, config_dir ):
        rs = "config_dir";
        break;
      case offsetof( duf_config_t, cmds_dir ):
        rs = "cmds_dir";
        break;
      case offsetof( duf_config_t, config_file_path ):
        rs = "config_file_path";
        break;
      case offsetof( duf_config_t, dir_priority ):
        rs = "dir_priority";
        break;
      case offsetof( duf_config_t, db.dir ):
        rs = "db.dir";
        break;
      case offsetof( duf_config_t, db.subdir ):
        rs = "db.subdir";
        break;
      case offsetof( duf_config_t, db.adm.name ):
        rs = "db.adm.name";
        break;
      case offsetof( duf_config_t, db.tempo.name ):
        rs = "db.tempo.name";
        break;
      case offsetof( duf_config_t, db.selected.name ):
        rs = "db.selected.name";
        break;
      case offsetof( duf_config_t, db.main.name ):
        rs = "db.main.name";
        break;
      case offsetof( duf_config_t, tag.dir ):
        rs = "tag.dir";
        break;
      case offsetof( duf_config_t, tag.file ):
        rs = "tag.file";
        break;
      case offsetof( duf_config_t, save.path ):
        rs = "save.path";
        break;
      case offsetof( duf_config_t, opt.option_delimiter ):
        rs = "opt.option_delimiter";
        break;
      case offsetof( duf_config_t, opt.act.v ):
        rs = "opt.act.v";
        break;
      case offsetof( duf_config_t, opt.v ):
        rs = "opt.v";
        break;
      case offsetof( duf_config_t, opt.output ):
        rs = "opt.output";
        break;
      case offsetof( duf_config_t, opt.output.fun_width ):
        rs = "opt.output.fun_width";
        break;
      case offsetof( duf_config_t, opt.output.header_tty ):
        rs = "opt.output.header_tty";
        break;
      case offsetof( duf_config_t, opt.output.level ):
        rs = "opt.output.level";
        break;
      case offsetof( duf_config_t, opt.disable ):
        rs = "opt.disable";
        break;
        /* case offsetof( duf_config_t, opt.output.history_filename ): */
        /*   rs = "opt.output.history_filename";                       */
        /*   break;                                                    */

        /* case offsetof( duf_config_t, cli ):  */
        /*   break;                             */
        /* case offsetof( duf_config_t, opt ):  */
        /*   break;                             */
        /* case offsetof( duf_config_t, db ):   */
        /*   break;                             */
        /* case offsetof( duf_config_t, save ): */
        /*   break;                             */
      default:
        if ( offset > offsetof( duf_config_t, cli ) && offset < offsetof( duf_config_t, opt ) )
        {
        }
        else if ( offset >= offsetof( duf_config_t, opt ) && offset < offsetof( duf_config_t, db ) )
        {
          /* {                                                                                                 */
          /*   if ( offsetof( duf_config_opt_t, option_delimiter ) == offset - offsetof( duf_config_t, opt ) ) */
          /*     rs = "opt.option_delimiter";                                                                  */
          /* }                                                                                                 */
        }
        else if ( offset >= offsetof( duf_config_t, db ) && offset < offsetof( duf_config_t, save ) )
        {
        }
        else if ( offset >= offsetof( duf_config_t, save ) && offset < offsetof( duf_config_t, config_dir ) )
        {
        }
        else if ( offset >= offsetof( duf_config_t, scn ) && offset < offsetof( duf_config_t, dh ) )
        {
        }
        else if ( offset >= offsetof( duf_config_t, dh ) && offset < offsetof( duf_config_t, tag ) )
        {
        }
        break;
      }
    }
  case DUF_OFFSET_ufilter:
    {
      switch ( offset )
      {
      case offsetof( duf_ufilter_t, globx.include_fs_files ):
        rs = "globx.include_fs_files";
        break;
      case offsetof( duf_ufilter_t, globx.exclude_fs_files ):
        rs = "globx.exclude_fs_files";
        break;
      case offsetof( duf_ufilter_t, max_rel_depth ):
        rs = "max_rel_depth";
        break;
      case offsetof( duf_ufilter_t, max_seq ):
        rs = "max_seq";
        break;
      case offsetof( duf_ufilter_t, same.md5 ):
        rs = "same.md5";
        break;
      case offsetof( duf_ufilter_t, same.sha1 ):
        rs = "same.sha1";
        break;
      case offsetof( duf_ufilter_t, same.exif ):
        rs = "same.exif";
        break;
      case offsetof( duf_ufilter_t, same.mime ):
        rs = "same.mime";
        break;
      case offsetof( duf_ufilter_t, same.data ):
        rs = "same.data";
        break;
      case offsetof( duf_ufilter_t, same.size ):
        rs = "same.size";
        break;
      case offsetof( duf_ufilter_t, size ):
        rs = "size";
        break;
      case offsetof( duf_ufilter_t, nameid ):
        rs = "nameid";
        break;
      case offsetof( duf_ufilter_t, dataid ):
        rs = "dataid";
        break;
      case offsetof( duf_ufilter_t, dirid ):
        rs = "dirid";
        break;
      case offsetof( duf_ufilter_t, mtime ):
        rs = "mtime";
        break;
      case offsetof( duf_ufilter_t, atime ):
        rs = "atime";
        break;
      case offsetof( duf_ufilter_t, ctime ):
        rs = "ctime";
        break;
      case offsetof( duf_ufilter_t, exifdt ):
        rs = "exifdt";
        break;
      case offsetof( duf_ufilter_t, inode ):
        rs = "inode";
        break;
      case offsetof( duf_ufilter_t, md5id ):
        rs = "md5id";
        break;
      case offsetof( duf_ufilter_t, sha1id ):
        rs = "sha1id";
        break;
      case offsetof( duf_ufilter_t, sd5id ):
        rs = "sd5id";
        break;
      case offsetof( duf_ufilter_t, crc32id ):
        rs = "crc32id";
        break;
      case offsetof( duf_ufilter_t, mimeid ):
        rs = "mimeid";
        break;
      case offsetof( duf_ufilter_t, exifid ):
        rs = "exifid";
        break;
      case offsetof( duf_ufilter_t, filename ):
        rs = "filename";
        break;
      case offsetof( duf_ufilter_t, glob_db ):
        rs = "glob_db";
        break;
      case offsetof( duf_ufilter_t, glob_db_include ):
        rs = "glob_db_include";
        break;
      case offsetof( duf_ufilter_t, glob_db_exclude ):
        rs = "glob_db_exclude";
        break;
      case offsetof( duf_ufilter_t, tag.dir ):
        rs = "tag.dir";
        break;
      case offsetof( duf_ufilter_t, exif.camera ):
        rs = "exif.camera";
        break;
      case offsetof( duf_ufilter_t, mime.type ):
        rs = "mime.type";
        break;
      case offsetof( duf_ufilter_t, same_as.md5 ):
        rs = "same_as.md5";
        break;
      case offsetof( duf_ufilter_t, same_as.sha1 ):
        rs = "same_as.sha1";
        break;
      case offsetof( duf_ufilter_t, same_as.exif ):
        rs = "same_as.exif";
        break;
      case offsetof( duf_ufilter_t, dirfiles.min ):
        rs = "dirfiles.min";
        break;
      case offsetof( duf_ufilter_t, dirfiles.max ):
        rs = "dirfiles.max";
        break;
      case offsetof( duf_ufilter_t, maxitems.dirs ):
        rs = "maxitems.dirs";
        break;
      case offsetof( duf_ufilter_t, maxitems.files ):
        rs = "maxitems.files";
        break;
      case offsetof( duf_ufilter_t, maxitems.total ):
        rs = "maxitems.total";
        break;
      case offsetof( duf_ufilter_t, v ):
        rs = "v";
        break;
      }
    }
    break;
  }
  return rs;
}

/* duf_xarr_print */
/* TODO : duf_codeval2string depends on optimpl !! */
static void
duf_xarr_print( const duf_longval_extended_table_t * xtable, const char *name )
{
  DUF_PRINTF( 0, ".@@@ [%s]", xtable->name );
  duf_optstage_print( xtable->use_stage, xtable->use_stage_mask, xtable->stage, xtable->stage_mask, 0 );
  DUF_PUTSL( 0 );

  for ( const duf_longval_extended_t * xtended = xtable->table; xtended->o.name; xtended++ )
  {
    if ( !name || strstr( xtended->o.name, name ) )
    {
      DUF_PRINTF( 0, "cmd %s", xtended->o.name );
      DUF_PRINTF( 0, "{" );
      if ( xtended->o.has_arg == 0 )
        DUF_PRINTF( 0, "  optarg: %s", "no" );
      else if ( xtended->o.has_arg == 1 )
        DUF_PRINTF( 0, "  optarg: %s", "required" );
      else if ( xtended->o.has_arg == 2 )
        DUF_PRINTF( 0, "  optarg: %s", "optional" );
      else
        DUF_PRINTF( 0, "  optarg: %s", "error" );
      if ( xtended->can_no || xtended->invert || xtended->m_hasoff )
      {
        DUF_PRINTF( 0, ".%s", "  " );
        DUF_PRINTF( 0, ".%s", xtended->can_no ? "can_no " : "" );
        DUF_PRINTF( 0, ".%s", xtended->invert ? "invert " : "" );
        if ( xtended->m_hasoff )
        {
          const char *srelto = NULL;

          switch ( xtended->relto )
          {
          case DUF_OFFSET_none:
            break;
          case DUF_OFFSET_config:
            srelto = "config";
            break;
          case DUF_OFFSET_ufilter:
            srelto = "ufilter";
            break;
          }
          DUF_PRINTF( 0, ".offset( %lu, %s(%d), %s ) ", xtended->m_offset, srelto, xtended->relto,
                      duf_offset2stringid( xtended->m_offset, xtended->relto ) );
        }
        DUF_PUTSL( 0 );
      }
      duf_optstage_print( xtended->use_stage, xtended->use_stage_mask, xtended->stage, xtended->stage_mask, 1 );
      {
        DUF_PRINTF( 0, ".%s", "  " );
/* TODO : duf_codeval2string depends on optimpl !! */
        DUF_PRINTF( 0, ".code:%s(%d)", duf_codeval2string( xtended->o.val ), xtended->o.val );
        DUF_PUTSL( 0 );
      }
      DUF_PRINTF( 0, "  class:%s(%d)", duf_optclass2string( xtended->oclass ), xtended->oclass );
      DUF_PRINTF( 0, "  vtype:%s(%d)", duf_vtype2string( xtended->vtype ), xtended->vtype );
      if ( xtended->call.funcname )
        DUF_PRINTF( 0, "  funcname:%s", xtended->call.funcname );
#if 0
      switch ( xtended->vtype )
      {
      case DUF_OPTION_VTYPE_EIA_CALL:
        DUF_PRINTF( 0, "  func:%p(%d)", xtended->call.fdesc.eia.func, xtended->call.fdesc.eia.arg );
        break;
      case DUF_OPTION_VTYPE_EV_CALL:
        DUF_PRINTF( 0, "  func:%p( )", xtended->call.fdesc.ev.func );
        break;
      case DUF_OPTION_VTYPE_SAS_CALL:
        DUF_PRINTF( 0, "  func:%p(%s)", xtended->call.fdesc.sas.func, xtended->call.fdesc.sas.arg );
        break;
      case DUF_OPTION_VTYPE_SAN_CALL:
        DUF_PRINTF( 0, "  func:%p(%ld)", xtended->call.fdesc.san.func, xtended->call.fdesc.san.arg );
        break;
      case DUF_OPTION_VTYPE_A_CALL:
        DUF_PRINTF( 0, "  func:%p(duf_cli_options_get_carg(  )->argc,duf_cli_options_get_carg(  )->argv)", xtended->call.fdesc.a.func );
        break;
      case DUF_OPTION_VTYPE_N_CALL:
        /* int rxx DUF_UNUSED = 0; */
        /* DUF_PRINTF( 0, "  func:%p(%ld)", xtended->call.fdesc.n.func, duf_strtol_suff( optargg, &rxx ) ); */
        DUF_PRINTF( 0, "  func:%p(duf_strtol_suff( optargg, &r ))", xtended->call.fdesc.n.func );
        break;
      case DUF_OPTION_VTYPE_S_CALL:
        DUF_PRINTF( 0, "  func:%p(optargg)", xtended->call.fdesc.s.func );
        break;
      case DUF_OPTION_VTYPE_AA_CALL:
        DUF_PRINTF( 0, "  func:%p(duf_cli_options_get_carg(  ))", xtended->call.fdesc.aa.func );
        break;
      case DUF_OPTION_VTYPE_TN1_CALL:
        DUF_PRINTF( 0, "  func:%p(duf_cli_options_get_targ(  ), duf_strtol_suff( optargg, &r ))", xtended->call.fdesc.tn1.func );
        break;
      case DUF_OPTION_VTYPE_TS1_CALL:
        DUF_PRINTF( 0, "  func:%p(duf_cli_options_get_targ(  ), optargg)", xtended->call.fdesc.ts1.func );
        break;
      case DUF_OPTION_VTYPE_TS2_CALL:
        DUF_PRINTF( 0, "  func:%p(duf_cli_options_get_ptargc(  ), duf_cli_options_get_ptargv(  ), optargg)", xtended->call.fdesc.ts2.func );
        break;
      default:
        break;
      }
#endif
      if ( xtended->help )
        DUF_PRINTF( 0, "  help(%s)", xtended->help );

      DUF_PRINTF( 0, ".{.o = {" );
      DUF_PRINTF( 0, ".  DO_Q( \"%s\" )", xtended->o.name );
      {
        char c;

        if ( xtended->o.has_arg == 0 )
          c = 'N';
        else if ( xtended->o.has_arg == 1 )
          c = 'R';
        else if ( xtended->o.has_arg == 2 )
          c = 'O';
        else
          c = '_';
        DUF_PRINTF( 0, "., DO_A_%c", c );
      }
      DUF_PRINTF( 0, "., DO_V( %s )", duf_codeval2string( xtended->o.val ) );
      DUF_PRINTF( 0, ".  }" );
      DUF_PRINTF( 0, "., DO_CL( %s )", duf_optclass2string( xtended->oclass ) );
      if ( xtended->m_hasoff )
      {
        switch ( xtended->relto )
        {
        case DUF_OFFSET_none:
          break;
        case DUF_OFFSET_config:
          DUF_PRINTF( 0, "., DO_OC( %s,%s )", duf_vtype2string( xtended->vtype ), duf_offset2stringid( xtended->m_offset, xtended->relto ) );
          if ( xtended->afl.bit )
            DUF_PRINTF( 0, "., DO_FL( %s,%s )", "??", "??" );
          break;
        case DUF_OFFSET_ufilter:
          DUF_PRINTF( 0, "., DO_OU( %s,%s )", duf_vtype2string( xtended->vtype ), duf_offset2stringid( xtended->m_offset, xtended->relto ) );
          if ( xtended->afl.bit )
            DUF_PRINTF( 0, "., DO_FL( %s,%s )", "??", "??" );
          break;
        }
      }

      if ( xtended->help )
        DUF_PRINTF( 0, "., DO_H( %s )", xtended->help );
      DUF_PRINTF( 0, "}," );
      DUF_PRINTF( 0, "}" );
    }
  }
}

/* duf_multix_print */
/* TODO :  depends on optimpl !! */
static void
duf_multix_print( const duf_longval_extended_table_t ** xtables, const char *name )
{
  for ( const duf_longval_extended_table_t ** xt = xtables; xt && *xt; xt++ )
  {
    char *se = NULL;
    char *matchtab = NULL;
    char *matchopt = NULL;

    if ( name )
    {
      se = strchr( name, ':' );
      if ( se )
      {
        matchtab = mas_strndup( name, se - name );
        matchopt = mas_strdup( se + 1 );
      }
      else
        matchtab = mas_strdup( name );
    }
    /* if ( ( !matchtab || !*matchtab || 0 == strncmp( ( *xt )->name, matchtab, strlen( matchtab ) ) ) ) */
    if ( ( !matchtab || !*matchtab || strstr( ( *xt )->name, matchtab ) ) )
      duf_xarr_print( *xt, matchopt ); /* TODO :  depends on optimpl !! */
    mas_free( matchtab );
    mas_free( matchopt );
  }
}

/* duf_stdx_print *//* TODO :  depends on optimpl !! */
void
duf_stdx_print( const char *name )
{
  duf_multix_print( duf_extended_table_multi(  ), name );
}