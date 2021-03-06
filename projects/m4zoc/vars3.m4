# AM_CXXFLAGS="-O0"
# AC_SUBST(AM_CXXFLAGS)

AC_DEFINE_UNQUOTED(MAS_CFLAGS, "${AM_CFLAGS}", [compile flags])
AC_SUBST(MAS_CFLAGS)

MAS_CFLAGS1="${AM_CFLAGS}"
AC_SUBST(MAS_CFLAGS1)

AM_CPPFLAGS="-I\$(srcdir)/inc -I\$(top_srcdir)/src/inc -I\$(top_srcdir)/src -I\$(includedir)"
AC_SUBST(AM_CPPFLAGS)

AC_DEFINE_UNQUOTED(MAS_LDFLAGS, "${AM_LDFLAGS}", [ld flags])
AC_SUBST(MAS_LDFLAGS)

AH_TEMPLATE([MAS_C_DATE],[mas_c_date])
AC_DEFINE_UNQUOTED( MAS_C_DATE, "`date +%Y%m%d.%H%M%S`", [cdate])
AC_SUBST([MAS_C_DATE])

AH_TEMPLATE([MAS_MALLOC])
AC_DEFINE( MAS_MALLOC, malloc)
AC_SUBST([MAS_MALLOC])

AH_TEMPLATE([MAS_FREE])
AC_DEFINE( MAS_FREE, free)
AC_SUBST([MAS_FREE])

AH_TEMPLATE([MAS_CALLOC])
AC_DEFINE( MAS_CALLOC, calloc)
AC_SUBST([MAS_CALLOC])

AH_TEMPLATE([MAS_REALLOC])
AC_DEFINE( MAS_REALLOC, realloc)
AC_SUBST([MAS_REALLOC])

AH_TEMPLATE([MAS_STRDUP])
AC_DEFINE( MAS_STRDUP, strdup)
AC_SUBST([MAS_STRDUP])

AH_TEMPLATE([MAS_STRNDUP])
AC_DEFINE( MAS_STRNDUP, strndup)
AC_SUBST([MAS_STRNDUP])


AH_TEMPLATE([MAS_CONFIG_ARGS],[mas_config_args])
AC_DEFINE_UNQUOTED( MAS_CONFIG_ARGS, "$ac_configure_args", [config_args])
AC_SUBST([MAS_CONFIG_ARGS])

AH_TEMPLATE([MAS_CONFIG_PREFIX],[mas_config_prefix])
AC_DEFINE_UNQUOTED( MAS_CONFIG_PREFIX, "$prefix", [config_prefix])
AC_SUBST([MAS_CONFIG_PREFIX])

AH_TEMPLATE([MAS_CONFIG_EXEC_PREFIX],[mas_config_exec_prefix])
AC_DEFINE_UNQUOTED( MAS_CONFIG_EXEC_PREFIX, "$exec_prefix", [config_exec_prefix])
AC_SUBST([MAS_CONFIG_EXEC_PREFIX])

AH_TEMPLATE([MAS_CONFIG_SYSCONFDIR],[mas_config_sysconfdir])
AC_DEFINE_UNQUOTED( MAS_CONFIG_SYSCONFDIR, "${sysconfdir}", [config_sysconfdir])
AC_SUBST([MAS_CONFIG_SYSCONFDIR])

AH_TEMPLATE([MAS_CONFIG_LIBDIR],[mas_config_libdir])
AC_DEFINE_UNQUOTED( MAS_CONFIG_LIBDIR, "$libdir", [config_libdir])
AC_SUBST([MAS_CONFIG_LIBDIR])

# AC_SUBST([MAS_CONFIGURED], ["$ac_configure_args"])


AH_TEMPLATE([MAS_UNAME],[mas_uname])
AC_DEFINE_UNQUOTED( MAS_UNAME, "`uname -a`", [uname])
AC_SUBST([MAS_UNAME])

AH_TEMPLATE([MAS_OSVER],[mas_osver])
AC_DEFINE_UNQUOTED( MAS_OSVER, "`cat /proc/version`", [os version])
AC_SUBST([MAS_OSVER])

AH_TEMPLATE([MAS_UUID],[mas_uuid])
AC_DEFINE_UNQUOTED( MAS_UUID, "`uuidgen`", [uuidgen])
AC_SUBST([MAS_UUID])

AH_TEMPLATE([MAS_S_DATE],[mas_s_date])
AC_DEFINE_UNQUOTED( MAS_S_DATE, "`date +%s`", [sdate])
AC_SUBST([MAS_S_DATE])
