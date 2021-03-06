#ifndef DUF_START_END_H
#define DUF_START_END_H

#  define DUF_START() {
#  define DUF_STARTTYP(typ, _rt) typ _rt=(typ)0; DUF_START()
#  define DUF_STARTR(_rt)         DUF_STARTTYP(int, _rt)
#  define DUF_STARTULL(_rt)       DUF_STARTTYP(unsigned long long, _rt)
#  define DUF_END() }
#  define DUF_ENDRET(_rt)                                                       DUF_END(); return _rt
#  define DUF_ENDRETT(_rt, _typ)                                                       DUF_END(); { _typ rrrt=_rt; return rrrt; }
#  define DUF_ENDR(_rt)           DUF_TEST_R(_rt);                                              DUF_ENDRETT(_rt, int)
#  define DUF_ENDR_CLEAR(_rt,...) DUF_TEST_R(_rt); DUF_CLEAR_ERROR( _rt, __VA_ARGS__ );		DUF_ENDRETT(_rt, int)
#  define DUF_ENDR_NOE(_rt, ...)  DUF_TEST_R_NOE(_rt, __VA_ARGS__);				DUF_ENDRETT(_rt, int)
#  define DUF_ENDR_YES(_rt,...)	  DUF_TEST_R(_rt); DUF_E_YES(__VA_ARGS__); 			DUF_ENDRETT(_rt, int)
#  define DUF_ENDR_YES_CLEAR(_rt,...) \
  				  DUF_TEST_R(_rt); DUF_E_YES(__VA_ARGS__);  \
					DUF_CLEAR_ERROR( _rt, __VA_ARGS__ ); \
												DUF_ENDRETT(_rt)
/* #  define DUF_ENDRQ(_rt,_cond)    DUF_TEST_RQ(_rt,_cond);                                       DUF_ENDRETT(_rt) */
/* #  define DUF_ENDRN(_rt)          DUF_TEST_RN(_rt);                                             DUF_ENDRETT(_rt, int) */
#  define DUF_ENDR3(_rt3)         DUF_TEST_R( DUF_SQLITE2R_ERROR_CODE(_rt3) );                    DUF_ENDRETT(_rt3, int)
#  define DUF_ENDULL(_rt)                                                                       DUF_ENDRETT(_rt, unsigned long long)
#  define DUF_ENDS(_s)                                                                          DUF_ENDRETT(_s, char *)
#  define DUF_ENDCS(_s)                                                                          DUF_ENDRETT(_s, const char *)


#endif

/*
vi: ft=c
*/
