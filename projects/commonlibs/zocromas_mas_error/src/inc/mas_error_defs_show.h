#ifndef MAS_ERROR_DEFS_SHOW_H
#  define MAS_ERROR_DEFS_SHOW_H



#  define MASE_SHOW_ERRORO_WP( _prefix, ... )		MAST_TRACE_WP( _prefix, errors  /* opt.trace.errors */, 0, __VA_ARGS__ )
#  define MASE_SHOW_ERRORO( ... )				MASE_SHOW_ERRORO_WP( "@@  ERRor", __VA_ARGS__)

#  define MASE_SHOW_ERRORN( _ri, _verb ) 		mas_error_report_i(_ri, 0, MAST_TRACE_FILE, _verb);
#  define MASE_SHOW_ERRORN_TEST( _ri, _verb ) 		mas_error_report_i(_ri, 1, MAST_TRACE_FILE, _verb);

/* #  define MASE_SHOW_ERRORiV( v )                         MASE_SHOW_ERRORO( #v ":%d" , v ) */
/* ###################################################################### */

/* error message if arg is not 0 */
/* #  define MASE_TEST_RX(_rval)         if (_rval) MASE_SHOW_ERRORO_TEST( "[%s] (#%d)", _rval<0?mas_error_name_i(_rval):"+", _rval ) */


#  if 0
#    define MASE_SHOW_ERRORO_TEST( _fmt, ... )             MASE_SHOW_ERRORO_WP( "@@ [TEst] ", "@@@@@@@@@"_fmt,  __VA_ARGS__ )
#    define MASE_SHOW_ERRORO_TEST_WP_STD(_rval, _prefix) MASE_SHOW_ERRORO_WP( _prefix, "[%s] (ri:%d) {en:%d} lsz:%ld rep:%u:%u", \
					  (_rval)<0?mas_error_name_i(_rval):"+", _rval, mas_enabled_ereport_n_i(_rval), \
    						mas_error_list_size(), mas_ecount_reported_i(_rval), mas_icount_reported_i(_rval) )
/* MASE_SHOW_ERRORO_TEST_STD : takes mas_error_index_t! */
#    define MASE_SHOW_ERRORO_TEST_STD(_rval)           MASE_SHOW_ERRORO_TEST(          "[%s] (ri:%d) {en:%d} lsz:%ld rep:%u:%u", \
					  (_rval)<0?mas_error_name_i(_rval):"+", _rval, mas_enabled_ereport_n_i(_rval), \
    						mas_error_list_size(), mas_ecount_reported_i(_rval), mas_icount_reported_i(_rval) )
#  else
#    define MASE_SHOW_ERRORN_TEST_STD(_rval, _verb)  mas_error_report_i(_rval, 1, MAST_TRACE_FILE, _verb)
#  endif



#endif

/*
vi: ft=c
*/