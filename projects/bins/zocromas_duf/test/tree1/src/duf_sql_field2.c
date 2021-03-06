#include <string.h>
#include <assert.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>



#include "duf_maintenance.h"

#include "duf_sccb.h"
#include "duf_sql2.h"

#include "duf_sccbh_shortcuts.h"
/* ###################################################################### */
#include "duf_sql_field2.h"
/* ###################################################################### */



int
__duf_sql_pos_by_name( const char *name, duf_record_t * precord, int *phave, int optional )
{
  int pos = DUF_ERROR_NO_FIELD;

  if ( precord )
  {
    for ( int i = 0; i < precord->ncolumns; i++ )
    {
      assert( name );
      assert( precord->pnames );
      assert( precord->pnames[i] );
      if ( 0 == strcmp( name, precord->pnames[i] ) )
      {
        pos = i;
        break;
      }
    }
    if ( phave )
    {
      *phave = pos;
    }
    else
    {
      if ( pos < 0 && !optional )
      {
        for ( int i = 0; i < precord->ncolumns; i++ )
        {
          DUF_SHOW_ERROR( "  Have '%s'\n", precord->pnames[i] );
        }
        DUF_SHOW_ERROR( "no such field '%s' {%p} (%u)\n", name, ( void * ) precord, precord->ncolumns );
      }
      assert( pos >= 0 || optional );
    }
  }
  return pos;
}

const char *
__duf_sql_str_by_name( const char *name, duf_record_t * precord, int *phave, int optional )
{
  const char *ptr = NULL;
  int pos = __duf_sql_pos_by_name( name, precord, phave, optional );

  if ( pos >= 0 )
    ptr = precord->presult[pos];

  return ptr;
}

const char *
__duf_sql_val_by_name( const char *name, duf_record_t * precord, int *phave, int optional )
{
  const char *ptr = NULL;
  int pos = __duf_sql_pos_by_name( name, precord, phave, optional );

  if ( pos >= 0 )
    ptr = precord->presult[pos];
  return ptr;
}

unsigned long long
__duf_sql_ull_by_name( const char *name, duf_record_t * precord, int *phave, int optional )
{
  const char *ptr = NULL;

  ptr = __duf_sql_val_by_name( name, precord, phave, optional );
  return ptr ? strtoll( ptr, NULL, 10 ) : 0;
}

/* 
 * this is callback of type:duf_sel_cb_t (second range; ; sel_cb)
 * */
int
duf_sel_cb_field_by_sccb( duf_record_t * precord, void *sel_cb_udata, duf_str_cb_t str_cb_unused, void *str_cb_udata_unused,
                          duf_sccb_handle_t * sccbh )
{
  int r_ = 0;

  if ( SCCB )
  {
    unsigned long long *pvalue;

    pvalue = ( unsigned long long * ) sel_cb_udata;
    if ( pvalue
#ifdef DUF_RECORD_WITH_NROWS
         && precord->nrow == 0
#endif
           )
    {
      int have_pos = 0;
      const duf_sql_set_t *leaf_set = duf_get_sql_set( SCCB, DUF_NODE_LEAF );

      *pvalue = __duf_sql_ull_by_name( leaf_set->fieldset, precord, &have_pos, 0 );
      if ( have_pos >= 0 )
      {
#ifdef DUF_RECORD_WITH_NROWS
        r_ = precord->nrow;
#else
        r_ = 0;
#endif
      }
      else
      {
        DUF_MAKE_ERROR( r_, DUF_ERROR_NO_FIELD_OPTIONAL );
        DUF_SHOW_ERROR( "r_=%d; no field %s", r_, leaf_set->fieldset );
      }
    }
#ifdef DUF_RECORD_WITH_NROWS
    else
    {
      DUF_SHOW_ERROR( "something is wrong" );
      DUF_MAKE_ERROR( r_, DUF_ERROR_GET_FIELD );
    }
#endif
  }
  return r_;
}
