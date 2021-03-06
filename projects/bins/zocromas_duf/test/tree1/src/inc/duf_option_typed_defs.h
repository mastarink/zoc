#ifndef DUF_OPTION_TYPED_DEFS_H
#  define DUF_OPTION_TYPED_DEFS_H

#define DUF_NUMOPT( _no, _rt, _typ, _dopls, _conv) \
      if (_rt>=0) \
      { \
	if ( !_no ) \
	{ \
	  int __rl = 0; \
	  _typ *p; \
	  p = ( _typ * ) byteptr; /* byteptr only valid if extended->m_hasoff == 1 */ \
	  if ( extended->m_hasoff == 1 && optargg ) /* if  extended->m_hasoff == 1, then mcfg_offset is offset */ \
	  { \
	    _typ __v; \
	    if (*optargg=='?') \
	    { DUF_PRINTF(0, "%s=%lld", extended->o.name, (unsigned long long)( *p )); } \
	    else \
	    { \
	      __v = _conv( optargg, &__rl ); \
	      if ( __rl < 0 ) \
	      { DUF_MAKE_ERROR(_rt, DUF_ERROR_OPTION_VALUE);DUF_TEST_R( _rt ); } \
	      else \
	      { \
		if ( _dopls && ( *optargg=='-' || *optargg=='+' ) ) \
		  ( *p ) += __v; \
		else \
		  ( *p ) = __v; \
		DUF_TRACE( options, 4, "%s : number set:%llu", extended->o.name, (unsigned long long) __v ); \
	      } \
	    } \
	  } \
	  else if ( _dopls ) \
	  { \
	    ( *p )++; \
	  } \
	} \
	else \
	{ \
	  DOR(_rt, DUF_ERROR_OPTION); \
	} \
      }
#define DUF_MINMAXOPT(_no, _rt, _typ,_conv) \
      if (_rt>=0) \
      { \
	if ( !_no ) \
	{ \
	  int __rl = 0; \
	  _typ *mm; \
	  mm= ( _typ * ) byteptr; /* byteptr only valid if extended->m_hasoff == 1 */ \
	  if ( extended->m_hasoff == 1 ) /* if  extended->m_hasoff == 1, then mcfg_offset is offset */ \
	  { \
	    if( optargg ) \
	    { \
	      _typ __v; \
	      char c=0; \
	      mm->flag = 1; \
	      if ( *optargg == '+' ) \
		c = *optargg++; \
	      else if ( *optargg == '-' ) \
		c = *optargg++; \
	      __v.min = _conv( optargg, &__rl ); \
	      if ( __rl < 0 ) \
	      { DUF_MAKE_ERROR(_rt, DUF_ERROR_OPTION_VALUE); DUF_TEST_R( _rt ); } \
	      else \
	      { \
		if ( c=='+' ) \
		{ \
		  mm->min = __v.min; \
		  mm->max = 0; \
		} \
		else if ( c=='-' ) \
		{ \
		  mm->min = 0; \
		  mm->max = __v.min; \
		} \
		else \
		{ \
		  mm->min = mm->max = __v.min; \
		} \
		DUF_TRACE( options, 4, "%s: min/max set:%llu", extended->o.name, (unsigned long long) __v.min ); \
	      } \
	    } \
	    else \
	    { \
	      mm->flag = 1; \
	      mm->min = extended->call.value.u; \
	      mm->max = extended->call.value2.u; \
	      DUF_TRACE( options, 4, "%s: min/max set:%llu:%llu", extended->o.name, \
		  (unsigned long long) extended->call.value.u,  (unsigned long long) extended->call.value2.u ); \
	    } \
	  } \
	} \
	else \
	{ \
	  DOR(_rt, DUF_ERROR_OPTION); \
	} \
      }
#define DUF_MOPT(_no, _rt,_typ,_mix,_conv) \
      if(_rt>=0) \
      { \
	if ( !_no ) \
	{ \
	  int __rl = 0; \
	  _typ *mm; \
	  mm= ( _typ * ) byteptr; /* byteptr only valid if extended->m_hasoff == 1 */ \
	  if ( extended->m_hasoff == 1 && optargg ) /* if  extended->m_hasoff == 1, then mcfg_offset is offset */ \
	  { \
	    mm->flag = 1; \
	    mm->_mix = _conv( optargg, &__rl ); \
	    if ( __rl < 0 ) \
	    {  DUF_MAKE_ERROR(_rt, DUF_ERROR_OPTION_VALUE);DUF_TEST_R( _rt ); } \
	    else \
	    { DUF_TRACE( options, 4, #_mix "%s: set:%llu", extended->o.name, (unsigned long long) mm->_mix ); } \
	  } \
	} \
	else \
	{ \
	  DOR(_rt, DUF_ERROR_OPTION); \
	} \
      }

#define DUF_OUTPUTFILE_A(_no, _rt, _typ, _defoptarg, _defout ) \
      if(_rt>=0) \
      { \
	if( !_no ) \
	{ \
	  int __rl = 0; \
	  const char *s; \
	  _typ *mm; \
	  s=optargg ? optargg : _defoptarg; \
	  mm= ( _typ * ) byteptr; /* byteptr only valid if extended->m_hasoff == 1 */ \
	  if ( extended->m_hasoff == 1 /* && (s || extended->call.value.u */ ) /* if  extended->m_hasoff == 1, then mcfg_offset is offset */ \
	  { \
	    DOR(__rl, duf_set_file_special( s, &mm->file, &mm->out, _defout, extended->call.value.u )); \
	    if (mm->out && mm->header) { fprintf( mm->out, mm->header ); } \
	    if ( __rl < 0 ) \
	    {  DOR(_rt, __rl); } \
	  } \
	} \
	else \
	{ \
	  DOR(_rt, DUF_ERROR_OPTION); \
	} \
      }
#define DUF_OUTPUTFILE(_no, _rt, _typ, _defout ) DUF_OUTPUTFILE_A(_no, _rt, _typ, NULL, _defout )


#endif
