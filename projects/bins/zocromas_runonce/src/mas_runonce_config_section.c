/* #include <stdio.h>  */
/* #include <stdlib.h> */
#include <string.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_arg_tools.h>

#include "mas_runonce_config_types.h"



#include "mas_runonce_config_section.h"


config_section_t *
runonce_config_section_create( char *name, config_group_t * cfg_group )
{
  int id = 0;

  /* printf( "-= %s =-\n", name ); */
  if ( cfg_group )
  {
    if ( cfg_group->sections )
    {
      if ( *name != '@' )
      {
        id = cfg_group->num_sections++;
        cfg_group->sections = mas_realloc( cfg_group->sections, cfg_group->num_sections * sizeof( config_section_t ) );
        memset( cfg_group->sections + id, 0, sizeof( config_section_t ) );
      }
    }
    else
    {
      id = cfg_group->num_sections++;
      cfg_group->sections = mas_malloc( cfg_group->num_sections * sizeof( config_section_t ) );
      memset( cfg_group->sections + id, 0, sizeof( config_section_t ) );
    }
    mas_free( cfg_group->sections[id].name );
    cfg_group->sections[id].id = id;
    cfg_group->sections[id].name = NULL;
    cfg_group->sections[id].name = mas_strdup( name );
  }
  return &cfg_group->sections[id];
}

void
runonce_config_section_delete( config_section_t * section )
{
  if ( section )
  {
    if ( section->values )
      for ( int ivalue = 0; ivalue < RUNONCE_MAX; ivalue++ )
      {
        mas_free( section->values[ivalue] );
        section->values[ivalue] = NULL;
      }
    mas_del_argv( section->largc, section->largv, 0 );
    section->largc = 0;
    section->largv = NULL;

    mas_del_argv( section->lenvc, section->lenvp, 0 );
    section->lenvc = 0;
    section->lenvp = NULL;

    mas_del_argv( section->qargc, section->qargv, 0 );
    section->qargc = 0;
    section->qargv = NULL;

    mas_free( section->values );
    section->values = NULL;

    mas_free( section->pids );
    section->pids = NULL;

    mas_free( section->name );
    section->name = NULL;
  }
}

const char *
runonce_config_replacement( config_section_t * section, item_id_t id )
{
  const char *repl;

  repl = section->values[id];
  if ( !repl )
    repl = "";
  return repl;
}

char *
runonce_config_section_fill_env( config_section_t * section, char *string )
{
  char *pp = NULL;

  if ( configuration.flags.verbose > 4 )
    printf( "+ config section fill env %s '%s'\n", section->name, string );
  while ( string && ( pp = strchr( string, '$' ) ) )
  {
    char *vn, *vne, *vnext;
    int changed;

    changed = 0;
    if ( configuration.flags.verbose > 4 )
      printf( "+ config 1 section fill env %s '%s'\n", section->name, string );
    /* char *vvalue; */

    /* printf( "A ENV REPL: %s\n", string ); */
    vn = vne = vnext = pp + 1;
    if ( *vn == '{' )
    {
      vn++;
      while ( vne && *vne && *vne != '}' )
        vne++;
      vnext = vne + 1;
    }
    else
    {
      while ( vne && *vne && ( ( *vne >= 'A' && *vne <= 'Z' ) || ( *vne >= '0' && *vne <= '9' && vne != pp ) || ( *vne == '_' ) ) )
        vne++;
      vnext = vne;
    }
    if ( configuration.flags.verbose > 4 )
      printf( "+ config 2 section fill env %s '%s'\n", section->name, string );
    if ( vne > vn )
    {
      char *vname, *vvalue;

      vname = mas_strndup( vn, vne - vn );
      vvalue = getenv( vname );
      /* printf( "@@@@ %s=`%s`\n", vname, vvalue ); */
      mas_free( vname );
      if ( vvalue )
      {
        char *s;

        s = mas_strndup( string, pp - string );
        s = mas_strcat_x( s, vvalue );
        if ( configuration.flags.verbose > 4 )
          printf( "+ config 2.1 section fill env %s '%s' %s='%s'\n", section->name, string, vname, vvalue );
        s = mas_strcat_x( s, vnext );
        mas_free( string );
        string = s;
        changed = 1;
      }
      else
      {
        char *s;

        s = mas_strndup( string, pp - string );
        if ( configuration.flags.verbose > 4 )
          printf( "+ config 2.1 section fill env %s '%s' %s=NIL\n", section->name, string, vname );
        s = mas_strcat_x( s, vnext );
        mas_free( string );
        string = s;
        changed = 1;
      }
      /* printf( "B ENV REPL: %s\n", string ); */
    }
    if ( configuration.flags.verbose > 4 )
      printf( "+ config 3 (%d) section fill env %s '%s'\n", changed, section->name, string );
    if ( !changed )
    {
      if ( configuration.flags.verbose > 4 )
        printf( "+ --------------- section fill env %s '%s'\n", section->name, string );
      break;
    }
    if ( configuration.flags.verbose > 4 )
      printf( "+ config 4 (%d) section fill env %s '%s'\n", changed, section->name, string );
  }
  return string;
}

char *
runonce_config_section_fill_percented( config_section_t * section, char *string )
{
  char *pp = NULL;

  while ( string && ( pp = strchr( string, '%' ) ) )
  {
    ssize_t len;
    ssize_t l1;
    const char *repl = NULL;
    int space = 0;

    pp++;

    /* printf( "S:[%s]\n", string ); */
    l1 = pp - string - 1;
    len = strlen( string );
    switch ( *pp )
    {
    case 'r':
      repl = section->name;
      break;
    case 'l':
      repl = runonce_config_replacement( section, RUNONCE_LAUNCHER );
      break;
    case 'o':
      space = 1;
      repl = runonce_config_replacement( section, RUNONCE_OPTIONS );
      break;
    default:
      repl = "X";
      break;
    }
    if ( repl )
    {
      char *s = NULL;

      if ( l1 > 0 )
        s = mas_strncat_x( s, string, l1 );
      if ( *repl )
      {
        if ( space )
          s = mas_strcat_x( s, " " );
        s = mas_strcat_x( s, repl );
      }
      if ( pp + 2 < string + len )
        s = mas_strcat_x( s, pp + 1 );
      mas_free( string );
      string = s;
    }
  }
  return string;
}

int
runonce_config_section_fill( config_group_t * group, config_section_t * section )
{
  if ( group && section )
  {
    if ( configuration.flags.verbose > 4 )
      printf( "+ config section fill [%s:%s]\n", group->name, section->name );
    if ( !section->values )
    {
      section->values = mas_malloc( RUNONCE_MAX * sizeof( char * ) );
      memset( section->values, 0, RUNONCE_MAX * sizeof( char * ) );
    }
    if ( configuration.flags.verbose > 4 )
      printf( "+ 2 config section fill [%s:%s]\n", group->name, section->name );
    if ( !section->id )
    {
      if ( !section->values[RUNONCE_LAUNCHER] )
        section->values[RUNONCE_LAUNCHER] = mas_strdup( "%r" );
      if ( !section->values[RUNONCE_COMMAND] )
        section->values[RUNONCE_COMMAND] = mas_strdup( "%l%o" );
    }
    if ( configuration.flags.verbose > 4 )
      printf( "+ 3 config section fill [%s:%s]\n", group->name, section->name );
    for ( int ivalue = 0; ivalue < RUNONCE_MAX; ivalue++ )
    {
      if ( configuration.flags.verbose > 4 )
        printf( "+ 3.1 config section fill [%s:%s] value %d of %d\n", group->name, section->name, ivalue, RUNONCE_MAX );
      if ( section->id )
      {
        if ( configuration.flags.verbose > 4 )
          printf( "+ 3.1.1 config section fill [%s:%s] value %d of %d\n", group->name, section->name, ivalue, RUNONCE_MAX );
        if ( group->sections[0].values && group->sections[0].values[ivalue] && !section->values[ivalue] )
          section->values[ivalue] = mas_strdup( group->sections[0].values[ivalue] );
        if ( configuration.flags.verbose > 4 )
          printf( "+ 3.1.2 config section fill [%s:%s] value %d of %d\n", group->name, section->name, ivalue, RUNONCE_MAX );
        section->values[ivalue] = runonce_config_section_fill_percented( section, section->values[ivalue] );
        if ( configuration.flags.verbose > 4 )
          printf( "+ 3.1.3 config section fill [%s:%s] value %d of %d\n", group->name, section->name, ivalue, RUNONCE_MAX );
        if ( ivalue != RUNONCE_WINDOWRE )
          section->values[ivalue] = runonce_config_section_fill_env( section, section->values[ivalue] );
        if ( configuration.flags.verbose > 4 )
          printf( "+ 3.1.4 config section fill [%s:%s] value %d of %d\n", group->name, section->name, ivalue, RUNONCE_MAX );
      }
      if ( configuration.flags.verbose > 4 )
        printf( "- 3.1 config section fill [%s:%s] value %d of %d\n", group->name, section->name, ivalue, RUNONCE_MAX );
    }
    if ( configuration.flags.verbose > 4 )
      printf( "+ 4 config section fill [%s:%s]\n", group->name, section->name );
    if ( section->id )
    {
      if ( section->values[RUNONCE_LAUNCHER] )
      {
        section->largc = mas_add_argv_args( section->largc, &section->largv, section->values[RUNONCE_LAUNCHER], 0 );
        if ( section->values[RUNONCE_OPTIONS] )
          section->largc = mas_add_argv_args( section->largc, &section->largv, section->values[RUNONCE_OPTIONS], 0 );
      }
      if ( section->values[RUNONCE_ENV] )
      {
        const char *s0 = section->values[RUNONCE_ENV];

        section->lenvc = mas_add_argv_args( section->lenvc, &section->lenvp, s0, 0 );
        for ( int i = 0; i < section->lenvc; i++ )
        {
          char *eq = NULL, *qu1 = NULL, *qu2 = NULL, *a0 = NULL;
          size_t l;

          a0 = section->lenvp[i];
          l = strlen( a0 );
          eq = strchr( a0, '=' );
          qu1 = strchr( a0, '"' );
          if ( qu1 )
            qu2 = strchr( qu1 + 1, '"' );
          if ( qu1 && qu1 == ( eq + 1 ) && qu2 && ( ( size_t ) ( qu2 - a0 ) == ( l - 1 ) ) )
          {
            char *s;

            s = mas_strndup( a0, eq - a0 + 1 );
            s = mas_strncat_x( s, qu1 + 1, qu2 - ( qu1 + 1 ) );
            mas_free( section->lenvp[i] );
            section->lenvp[i] = s;
          }
        }
      }
      if ( section->values[RUNONCE_QUITTER] )
        section->qargc = mas_add_argv_args( section->qargc, &section->qargv, section->values[RUNONCE_QUITTER], 0 );
    }
    if ( configuration.flags.verbose > 4 )
      printf( "- config section fill [%s:%s]\n", group->name, section->name );
  }
  return 0;
}

int
runonce_config_section_item_create( config_section_t * section, const char *string, int nl )
{
  const char *pval;
  char *value = NULL, *name = NULL;
  int id = RUNONCE_NONE;

  pval = strchr( string, '=' );
  if ( pval )
  {
    name = mas_strndup( string, pval - string );
    pval++;
    value = mas_strdup( pval );
  }
  else
  {
    name = mas_strdup( string );
    value = mas_strdup( "yes" );
  }

  if ( !name )
  {
    printf( "UNKNOWN (no name '%s') %2d. %-15s::\t%-15s = '%s'\n", string, nl, section->name, name, value );
  }
  if ( 0 == strcmp( name, "launcher" ) )
    id = RUNONCE_LAUNCHER;
  else if ( 0 == strcmp( name, "wrapper" ) )
    id = RUNONCE_WRAPPER;
  else if ( 0 == strcmp( name, "nolaunch" ) )
    id = RUNONCE_NOLAUNCH;
  else if ( 0 == strcmp( name, "stdenv" ) )
    id = RUNONCE_STDENV;
  else if ( 0 == strcmp( name, "noexit" ) )
    id = RUNONCE_NOEXIT;
  else if ( 0 == strcmp( name, "nostop" ) )
    id = RUNONCE_NOSTOP;
  else if ( 0 == strcmp( name, "noterminate" ) )
    id = RUNONCE_NOTERMINATE;
  else if ( 0 == strcmp( name, "nonice" ) )
    id = RUNONCE_NONICE;
  else if ( 0 == strcmp( name, "windowclose" ) )
    id = RUNONCE_CLOSE;
  else if ( 0 == strcmp( name, "nogroup" ) )
    id = RUNONCE_NOGROUP;
  else if ( 0 == strcmp( name, "prefix" ) )
    id = RUNONCE_PREFIX;
  else if ( 0 == strcmp( name, "opts4pid" ) )
    id = RUNONCE_OPTS4PID;
  else if ( 0 == strcmp( name, "nice" ) )
    id = RUNONCE_NICE;
  else if ( 0 == strcmp( name, "path" ) )
    id = RUNONCE_PATH;
  else if ( 0 == strcmp( name, "psname" ) )
    id = RUNONCE_PSNAME;
  else if ( 0 == strcmp( name, "globenv" ) )
    id = RUNONCE_GLOBENV;
  else if ( 0 == strcmp( name, "env" ) )
    id = RUNONCE_ENV;
  else if ( 0 == strcmp( name, "options" ) )
    id = RUNONCE_OPTIONS;
  else if ( 0 == strcmp( name, "pfin" ) )
    id = RUNONCE_PFIN;
  else if ( 0 == strcmp( name, "process" ) )
    id = RUNONCE_PROCESS;
  else if ( 0 == strcmp( name, "windowre" ) )
    id = RUNONCE_WINDOWRE;
  else if ( 0 == strcmp( name, "xgroup" ) )
    id = RUNONCE_XGROUP;
  else if ( 0 == strcmp( name, "quitter" ) )
    id = RUNONCE_QUITTER;
  else if ( 0 == strcmp( name, "restarter" ) )
    id = RUNONCE_RESTARTER;
  else if ( 0 == strcmp( name, "reloader" ) )
    id = RUNONCE_RELOADER;
  else if ( 0 == strcmp( name, "turn_oner" ) )
    id = RUNONCE_TURN_ONER;
  else if ( 0 == strcmp( name, "turn_offer" ) )
    id = RUNONCE_TURN_OFFER;
  else if ( 0 == strcmp( name, "disabler" ) )
    id = RUNONCE_DISABLER;
  else if ( 0 == strcmp( name, "enabler" ) )
    id = RUNONCE_ENABLER;
  else if ( 0 == strcmp( name, "toggler" ) )
    id = RUNONCE_TOGGLER;
  else if ( 0 == strcmp( name, "await[launch]" ) )
    id = RUNONCE_AWAIT;
  else if ( 0 == strcmp( name, "rwait" ) )
    id = RUNONCE_RWAIT;
  else
  {
    printf( "UNKNOWN ('%s') %2d. %-15s::\t%-15s = '%s'\n", string, nl, section->name, name, value );
  }
  if ( !section->values )
  {
    section->values = mas_malloc( RUNONCE_MAX * sizeof( char * ) );
    memset( section->values, 0, RUNONCE_MAX * sizeof( char * ) );
  }
  if ( value )
  {
    switch ( id )
    {
    case RUNONCE_ENV:
      if ( section->values[id] )
        section->values[id] = mas_strcat_x( section->values[id], " " );
      section->values[id] = mas_strcat_x( section->values[id], value );
      /*No: break; */
    case RUNONCE_NONE:
      mas_free( value );
      break;
    default:
      section->values[id] = value;
      break;
    }
  }
  mas_free( name );

  switch ( id )
  {
  case RUNONCE_WRAPPER:
    section->largc = mas_add_argv_args( section->largc, &section->largv, section->values[id], 0 );
    break;
  default:
    break;
  }

  return 0;
}
