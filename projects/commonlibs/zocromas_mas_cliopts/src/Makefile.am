SUBDIRS= inc

moduledir=$(libdir)/zocromod/base
module_LTLIBRARIES = cliopts.la
cliopts_la_LDFLAGS  = -module -avoid-version -no-undefined @AM_LDFLAGS@
cliopts_la_SOURCES = mas_cliopts.c mas_cliopts_init.c mas_cliopts_data.c mas_longopts_table_object.c



lib_LTLIBRARIES = libzocromas_mas_cliopts.la 

libzocromas_mas_cliopts_la_SOURCES = mas_cliopts.c mas_cliopts_init.c mas_cliopts_data.c mas_longopts_table_object.c
libzocromas_mas_cliopts_la_CFLAGS = @AM_CFLAGS@
libzocromas_mas_cliopts_la_LDFLAGS = @AM_LDFLAGS@ @DEPS_LIBS@
# libmas_tools_la_CFLAGS = ${AM_CFLAGS} -lmas_maslibother

if MAS_TEST
noinst_PROGRAMS=mtest

mtest_LDFLAGS=@AM_LDFLAGS@  @DEPS_LIBS@ lib@PACKAGE_NAME@.la
# mtest_LDADD=lib@PACKAGE_NAME@.la
mtest_CFLAGS=@AM_CFLAGS@  -pthread

mtest_SOURCES=mas_cliopts_mtest.c
else

endif
