#include <mastar/wrap/mas_std_def.h>
#include <mastar/types/mas_common_defs.h>

#include <stddef.h>
#include <stdlib.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/wrap/mas_lib.h>

#include <mastar/tools/mas_tools.h>
#include <mastar/tools/mas_arg_tools.h>



#include <mastar/types/mas_control_types.h>
#include <mastar/types/mas_opts_types.h>
extern mas_control_t ctrl;


#include <mastar/msg/mas_msg_def.h>
#include <mastar/msg/mas_msg_tools.h>

#include <mastar/log/mas_log.h>

#include "mas_opts_common.h"
#include "mas_opts_storage.h"
/* #include "mas_opts.h" */

#include "mas_opts_save.h"

/*
this:
  mas_opts.c
related:
  mas_opts_common.c
  mas_opts_types.h
  mas_opts_data.c
  mas_msg_tools.c
  mas_control.c

*/




static int
mas_opts_check_old_file( mas_options_t * popts,  const char *fpath, int deep, int remove_ext, int backup, int overwrite )
{
  int r = 0;
  FILE *f = NULL;

  f = mas_fopen( fpath, "r" );
  if ( f /* exists old file */  )
  {
    r = -1;
    if ( overwrite )
    {
      r = -1;
      deep++;
      if ( f )
        mas_fclose( f );
      if ( backup )
      {
        char *fpath_bak;

        fpath_bak = mas_strdup( fpath );
        {
          char ext[64];

          if ( remove_ext )
          {
            char *p;

            p = fpath_bak + strlen( fpath_bak );
            while ( p > fpath_bak && *( p - 1 ) >= '0' && *( p - 1 ) <= '9' )
              p--;
            if ( *( p - 1 ) == '.' )
              p--;
            *p = 0;
          }
          sprintf( ext, ".%03d", deep );
          fpath_bak = mas_strcat_x( fpath_bak, ext );
        }
        if ( deep < popts-> max_config_backup )
        {
          r = mas_opts_check_old_file( popts, fpath_bak, deep, 1, backup, overwrite );
        }
        if ( r >= 0 )
        {
          r = rename( fpath, fpath_bak );
          if ( r < 0 )
          {
            P_ERR;
          }
        }
        mas_free( fpath_bak );
      }
    }
  }
  else
  {
    r = overwrite;
  }
  return r;
}

int
_mas_opts_save( mas_options_t * popts,  const char *dirname, const char *filename, int backup, int overwrite )
{
  int r = 0;
  int rtot = -1;

  IEVALM( r, mas_opts_set_configdir( popts, dirname ), "(%d)set config dir: '%s'", dirname );
  IEVALM( r, mas_opts_set_configfilename( popts, filename ), "(%d)opts file:'%s'", filename );
  IEVALM( r, mas_opts_check_dir( popts ), "(%d)config dir: '%s'", popts-> dir.config );
  if ( r == 0 )
  {
    char *fpath = NULL;

    fpath = mas_strdup( popts-> dir.config );
    fpath = mas_strcat_x( fpath, "/" );
    fpath = mas_strcat_x( fpath, popts-> configfilename );
    if ( fpath )
    {
      if ( mas_opts_check_old_file( popts, fpath, 0, 0, backup, overwrite ) == 0 )
      {
        FILE *f;

        f = mas_fopen( fpath, "w" );
        if ( f )
        {
          char outstr[128] = "#   ";

          rtot = 0;
          {
            strftime( outstr, sizeof( outstr ), "%a, %d %b %Y %T %z", mas_xlocaltime(  ) );
            IEVAL( r, fprintf( f, "# %s\n#\n", outstr ) );
            if ( r > 0 )
              rtot += r;
          }
          if ( popts-> uuid )
          {
            IEVAL( r, fprintf( f, "uuid=%s\n", popts-> uuid ) );
            if ( r > 0 )
              rtot += r;
          }

          {
            IEVAL( r, fprintf( f,
                               "# common\nenv_optsname=%s\nenv_hostname=%s\nhistorydir=%s\npostdir=%s\nlogdir=%s\nlog=%d\n"
                               "max_config_backup=%u\nmessages=%u\n"
                               "default_port=%u\nsave_user_opts=%u\nsave_user_opts_plus=%u\n" "restart_sleep=%lg\n"
                               "# -\n", popts-> env_optsname, popts-> env_hostname, popts-> dir.history,
                               popts-> dir.post, popts-> dir.log, !popts-> nolog,
                               popts-> max_config_backup, !popts-> nomessages, popts-> default_port,
                               popts-> save_user_opts, popts-> save_user_opts_plus,
                               popts-> restart_sleep ) );
            if ( r > 0 )
              rtot += r;
          }
          if ( ctrl.is_server )
          {
            IEVAL( r, fprintf( f,
                               "# server\ndaemon=%u\nread_user_opts=%u\nread_user_opts_plus=%u\n"
                               "single_instance=%u\nsingle_child=%u\nlogger=%d\nmodsdir=%s\n" "pidsdir=%s\nprotodir=%s\n# -\n", ctrl.daemon,
                               popts-> read_user_opts, popts-> read_user_opts_plus,
                               popts-> single_instance, popts-> single_child, !popts-> nologger,
                               popts-> dir.mods, popts-> dir.pids, popts-> dir.proto ) );
            if ( r > 0 )
              rtot += r;
          }
          else if ( ctrl.is_client )
          {
            IEVAL( r,
                   fprintf( f, "# client\ndisconnect_prompt=%u\nwait_server=%u\n# -\n", popts-> disconnect_prompt,
                            popts-> wait_server ) );
            if ( r > 0 )
              rtot += r;
          }
          {
            {
              IEVAL( r, fprintf( f, "\n[%s %d]\n", ctrl.is_client ? "hosts" : "listen", popts-> hostsv.c ) );
              if ( r > 0 )
                rtot += r;
            }

            if ( popts-> hostsv.c )
            {
              for ( int ih = 0; ih < popts-> hostsv.c; ih++ )
              {
                IEVAL( r, fprintf( f, "host=%s\n", popts-> hostsv.v[ih] ) );
                if ( r > 0 )
                  rtot += r;
              }
            }
          }
          {
            {
              IEVAL( r, fprintf( f, "\n[%s %d]\n", "protos", popts-> protosv.c ) );
              if ( r > 0 )
                rtot += r;
            }

            if ( popts-> protosv.c )
            {
              for ( int ih = 0; ih < popts-> protosv.c; ih++ )
              {
                IEVAL( r, fprintf( f, "proto=%s\n", popts-> protosv.v[ih] ) );
                if ( r > 0 )
                  rtot += r;
              }
            }
          }
          if ( popts-> commandsv.c )
          {
            IEVAL( r, fprintf( f, "\n[commands %d]\n", popts-> commandsv.c ) );
            if ( r > 0 )
              rtot += r;
            for ( int ih = 0; ih < popts-> commandsv.c; ih++ )
            {
              IEVAL( r, fprintf( f, "command=%s\n", popts-> commandsv.v[ih] ) );
              if ( r > 0 )
                rtot += r;
            }
          }
        }
        if ( f )
        {
          mas_fclose( f );
          f = NULL;
          ctrl.opts_saved = 1;
        }
      }
      mas_free( fpath );
    }
  }
  return rtot;
}

int
mas_opts_save_user( mas_options_t * popts,  const char *dirname, const char *filename )
{
  int r = -1;

  if ( popts-> save_user_opts )
  {
    MAS_LOG( "to save opts %s", filename );
    IEVAL( r, _mas_opts_save( popts, dirname, filename, 1, popts-> overwrite_user_opts ) );
    MAS_LOG( "saved opts : %d", r );
  }
  return r;
}

int
_mas_opts_save_plus( mas_options_t * popts,  const char *dirname, const char *filename, int backup, int overwrite, va_list args )
{
  int r = 0;
  char *s = NULL;
  int x = 0;
  char *fn = NULL;

  fn = mas_strdup( filename );
  while ( ( s = va_arg( args, char * ) ) )
  {
    if ( s )
    {
      fn = mas_strcat_x( fn, s );
      x++;
    }
  }
  if ( x )
  {
    IEVAL( r, _mas_opts_save( popts, dirname, fn, backup, overwrite ) );
  }
  else
  {
    IEVAL( r, -1 );
  }
  mas_free( fn );
  return r;
}

int
mas_opts_save_user_plus( mas_options_t * popts,  const char *dirname, const char *filename, ... )
{
  int r = 0;
  va_list args;

  va_start( args, filename );
  if ( popts-> save_user_opts_plus )
  {
    MAS_LOG( "to save opts plus %s", filename );
    IEVAL( r, _mas_opts_save_plus( popts, dirname, filename, 1, popts-> overwrite_user_opts_plus, args ) );
    MAS_LOG( "saved opts plus : %d", r );
  }
  /* else                                    */
  /* {                                       */
  MAS_LOG( "already saved opts plus" );
  /* }                                       */
  va_end( args );
  return r;
}
