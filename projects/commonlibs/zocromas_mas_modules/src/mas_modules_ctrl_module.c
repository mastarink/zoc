#include <mastar/wrap/mas_std_def.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dlfcn.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_arg_tools.h>

#include <mastar/types/mas_control_types.h>
/* #include <mastar/types/mas_opts_types.h> */
extern mas_control_t ctrl;

/* extern mas_options_t opts; */

#include <mastar/msg/mas_msg_def.h>
#include <mastar/msg/mas_msg_tools.h>
/* #include <mastar/log/mas_log.h> */

#include "mas_modules_ctrl_module.h"


static void
__register_module( const char *name, void *module_handle )
{
  pthread_rwlock_wrlock( &ctrl.thglob.modules_list_rwlock );
  {
    unsigned cnt, size;
    void **array;

    array = ctrl.loaded_modules;
    /* ctrl.loaded_modules = NULL;  */
    /* ctrl.loaded_modules_cnt = 0; */
    cnt = ctrl.loaded_modules_cnt;
    size = cnt + 1;

    if ( array )
      array = mas_realloc( array, sizeof( void * ) * size );
    else
      array = mas_malloc( sizeof( void * ) * size );
    array[cnt] = module_handle;

    ctrl.loaded_modules = array;
    ctrl.loaded_modules_cnt = size;
  }
  pthread_rwlock_unlock( &ctrl.thglob.modules_list_rwlock );
  WMSG( "REG.MODULE %u. %s", ctrl.loaded_modules_cnt, name );
}

void
mas_modules_register_module( const char *name, void *module_handle )
{
  if ( module_handle )
    __register_module( name, module_handle );
}

typedef int ( *mas_v_fun_t ) ( void );
void
mas_modules_unregister( void )
{
  if ( ctrl.loaded_modules_cnt && ctrl.loaded_modules )
  {
    for ( int im = 0; im < ctrl.loaded_modules_cnt; im++ )
    {
      int unload = 1;
      mas_v_fun_t fun;

      fun = ( mas_v_fun_t ) ( unsigned long ) dlsym( ctrl.loaded_modules[im], "module_before_close" );
      WMSG( "MODULE DESTROY %u. %p [%d] %s", im, ( void * ) ctrl.loaded_modules[im], fun ? 1 : 0, dlerror(  ) );
      if ( fun )
        unload = ( fun ) (  );
      if ( unload && ctrl.loaded_modules[im] )
        dlclose( ctrl.loaded_modules[im] );
      ctrl.loaded_modules[im] = NULL;
    }
    ctrl.loaded_modules_cnt = 0;
    mas_free( ctrl.loaded_modules );
    ctrl.loaded_modules = NULL;
  }
}