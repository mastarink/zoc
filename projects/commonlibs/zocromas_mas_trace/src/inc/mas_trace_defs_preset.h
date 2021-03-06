#ifndef MAS_MAST_TRACE_DEFS_PRESET_H
# define MAS_MAST_TRACE_DEFS_PRESET_H

# ifndef MAST_FL
#  define MAST_FL __func__,__LINE__
# endif

/* # ifndef MAST_TRACE_CONFIG       */
/* #  define MAST_TRACE_CONFIG NULL */
/* # endif                          */

# ifndef MAST_TRACE_AT_TCFG_CR
#  define MAST_TRACE_AT_TCFG_CR(_cfg)  *(_cfg)
# endif
# ifndef MAST_TRACE_AT_TCFG_
#  define MAST_TRACE_AT_TCFG_
# endif
/* #  ifndef MAST_DBG_AT_TCFG            */
/* #    define MAST_DBG_AT_TCFG  opt.dbg */
/* #  endif                             */

/* #  ifndef MAST_TRACE_LOADTIME_C                                                 */
/* (* #    define MAST_TRACE_LOADTIME_C( _cfg )  ( _cfg ? _cfg->loadtime : 0.0) *) */
/* #    define MAST_TRACE_LOADTIME_C( _cfg )  0.0                                  */
/* #  endif                                                                        */
/* #  ifndef MAST_TRACE_LOADTIME                                                    */
/* #    define MAST_TRACE_LOADTIME  MAST_TRACE_LOADTIME_C(mas_get_trace_config(  )) */
/* #  endif                                                                         */

/* # ifndef MAST_TRACE_FILE_C                                                                                                                   */
/* (* #    define MAST_TRACE_FILE_C( _cfg ) ( _cfg && _cfg->MAST_TRACE_AT_TCFG_ output.out ? _cfg->MAST_TRACE_AT_TCFG_ output.out : stdout ) *) */
/* #  define MAST_TRACE_FILE_C( _cfg ) stdout                                                                                                   */
/* # endif                                                                                                                                      */
/* # ifndef MAST_TRACE_FILE                                                                                                                     */
/* (* #    define MAST_TRACE_FILE         MAST_TRACE_FILE_C(mas_get_config(  )) *)                                                              */
/* #  define MAST_TRACE_FILE         stdout                                                                                                     */
/* # endif                                                                                                                                      */
/*                                                                                                                                              */
/* # ifndef MAST_TRACE_FUNWIDTH_C                                                                                                               */
/* (* #    define MAST_TRACE_FUNWIDTH( _cfg ) ( _cfg?_cfg->opt.output.fun_width : 18 ) *)                                                       */
/* #  define MAST_TRACE_FUNWIDTH_C( _cfg ) 18                                                                                                   */
/* # endif                                                                                                                                      */
/*                                                                                                                                              */
/* # ifndef MAST_TRACE_FORCE_COLOR_C                                                                                                            */
/* (* #    define MAST_TRACE_FORCE_COLOR_C( _cfg ) ( _cfg?_cfg->opt.output.fun_width : 18 ) *)                                                  */
/* #  define MAST_TRACE_FORCE_COLOR_C( _cfg ) 0                                                                                                 */
/* # endif                                                                                                                                      */
/*                                                                                                                                              */
/* # ifndef MAST_TRACE_FORCE_COLOR                                                                                                              */
/* (* #    define MAST_TRACE_FORCE_COLOR_C( _cfg ) ( _cfg?_cfg->opt.output.fun_width : 18 ) *)                                                  */
/* #  define MAST_TRACE_FORCE_COLOR 0                                                                                                           */
/* # endif                                                                                                                                      */
/*                                                                                                                                              */
/* # ifndef MAST_TRACE_NOCOLOR_C                                                                                                                */
/* (* #    define MAST_TRACE_NOCOLOR( _cfg ) ( _cfg?_cfg->opt.output.fun_width : 18 ) *)                                                        */
/* #  define MAST_TRACE_NOCOLOR_C( _cfg ) 0                                                                                                     */
/* # endif                                                                                                                                      */
/*                                                                                                                                              */
/* # ifndef MAST_TRACE_NOCOLOR                                                                                                                  */
/* (* #    define MAST_TRACE_NOCOLOR( _cfg ) ( _cfg?_cfg->opt.output.fun_width : 18 ) *)                                                        */
/* #  define MAST_TRACE_NOCOLOR 0                                                                                                               */
/* # endif                                                                                                                                      */

/* # ifndef MAST_TRACE_LEVEL_C                                                                           */
/* (* #    define MAST_TRACE_LEVEL_C( _cfg, _what, _name ) MAST_IF_TRACE_WHAT_C( _cfg, _what, _name ) *) */
/* #  define MAST_TRACE_LEVEL_C( _cfg, _what, _name ) 0                                                  */
/* # endif                                                                                               */
/* # ifndef MAST_TRACE_INDEX             */
/* #  define MAST_TRACE_INDEX(_name)  -1 */
/* # endif                               */

#endif
