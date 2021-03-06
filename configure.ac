#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.60])
AC_INIT([matlang], [0.1], [snswinhaiku dot lo at gmail dot com])
AC_CONFIG_SRCDIR([src/treeize_main.c])
AC_CONFIG_HEADERS([src/config.h])
AC_CONFIG_AUX_DIR([m4/tools])
AC_CONFIG_MACRO_DIR([m4/generated])

AC_CANONICAL_SYSTEM

AC_SUBST([mat_so_version], [1:0:0])

__conf_saved_cflags="$CFLAGS"
AC_PROG_CC
CFLAGS="$__conf_saved_cflags"
LT_INIT([])
AM_INIT_AUTOMAKE([foreign subdir-objects])
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])
AC_ARG_ENABLE(debug,
	AS_HELP_STRING(
		[--enable-debug],
		[enable debugging, default: no]),
	[case "${enableval}" in
		yes) debug=true;;
		no)  debug=false;;
		*)   AC_MSG_ERROR([bad value ${enableval} for --enable-debug]);;
	esac],
	[debug=false])

debug_flag="-g"
if test "$debug" = true; then
	AX_CHECK_COMPILE_FLAG([-Og], [AX_APPEND_FLAG([-Og], [debug_flag])], [AX_APPEND_FLAG([-O0], [debug_flag])])
	AC_DEFINE([_DEBUG], [], [Debug mode])
else
	AX_APPEND_FLAG([-O2], [debug_flag])
	AC_DEFINE([NDEBUG], [], [Release mode])
fi
CFLAGS="${debug_flag}${CFLAGS:+ }${CFLAGS-}"
AM_CONDITIONAL([DEBUG], [test x$debug = xtrue])

AX_CHECK_COMPILE_FLAG([-std=c99], [AX_APPEND_FLAG([-std=c99], [warn_flags])], [])
AX_CHECK_COMPILE_FLAG([-Wall], [AX_APPEND_FLAG([-Wall], [warn_flags])], [])
AX_CHECK_COMPILE_FLAG([-Wextra], [AX_APPEND_FLAG([-Wextra], [warn_flags])], [])
AX_CHECK_COMPILE_FLAG([-Wconversion], [AX_APPEND_FLAG([-Wconversion], [warn_flags])], [])
AX_CHECK_COMPILE_FLAG([-Werror=implicit-function-declaration], [AX_APPEND_FLAG([-Werror=implicit-function-declaration], [warn_flags])], [])
AX_CHECK_COMPILE_FLAG([-pedantic], [AX_APPEND_FLAG([-pedantic], [warn_flags])], [])
# for libSegFault.so, backtrace
AX_CHECK_LINK_FLAG([-rdynamic], [AX_APPEND_FLAG([-rdynamic], [LDFLAGS])], [])

# Checks for libraries.

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
saved_LIBS="$LIBS"


AC_ARG_ENABLE([gmp],
	[AS_HELP_STRING(
		[--disable-gmp],
		[disable gmp, default: enabled])],
	[case "${enableval}" in
		yes) use_gmp=yes;;
		no)  use_gmp=no;;
		*)   AC_MSG_ERROR([bad value ${enableval} for --enable-gmp]);;
	esac],
	[use_gmp=auto])

AC_MSG_CHECKING([whether gmp should be used])
AC_MSG_RESULT([$use_gmp])
if test "$use_gmp" = "no"; then
	: # do nothing
else
	LIBS=
	AC_SEARCH_LIBS([__gmpz_init], [gmp], [gmp_found=1], [gmp_found=0])
	gmp_LIBS="$LIBS"
	LIBS=
	AC_SEARCH_LIBS([mpfr_add], [mpfr], [mpfr_found=1], [mpfr_found=0])
	mpfr_LIBS="$LIBS"

	if test "$gmp_found$mpfr_found" = 11; then
		gmp_usable=yes
	else
		gmp_usable=no
	fi

	if test "$use_gmp$gmp_usable" = yesno; then
		AC_MSG_ERROR([request gmp is not found!])
	elif test "$use_gmp" = auto; then
		use_gmp="$gmp_usable"
	fi
fi

if test "$use_gmp" = yes; then
	AC_DEFINE([USE_GMP], [1], [gmp should be used])
fi


LIBS=
AC_SEARCH_LIBS([sqrt], [m], [], [AC_MSG_ERROR([sqrt() not found.])])
math_LIBS="$LIBS"
LIBS="$saved_LIBS"

# leak sanitizer spends more time
AC_ARG_ENABLE(maintainer-mode,
	AS_HELP_STRING(
		[--enable-maintainer-mode],
		[enable compiler option for maintainer, default: no]),
	[case "${enableval}" in
		yes) maintainer_mode=true;;
		no)  maintainer_mode=false;;
		*)   AC_MSG_ERROR([bad value ${enableval} for --enable-maintainer-mode]);;
	esac],
	[maintainer_mode=false])

if test "$maintainer_mode" = true; then
	# -fsanitize=leak fails with linker error (-llsan is not found)
	AX_CHECK_COMPILE_FLAG([-fsanitize=leak],
						  [AX_CHECK_LINK_FLAG([-fsanitize=leak], [AX_APPEND_FLAG([-fsanitize=leak], [])])], [])
	AX_CHECK_COMPILE_FLAG([-fsanitize=address],
						  [AX_CHECK_LINK_FLAG([-fsanitize=address], [AX_APPEND_FLAG([-fsanitize=address], [])])], [])
	AX_CHECK_COMPILE_FLAG([-fstack-protector-strong],
						  [AX_APPEND_FLAG([-fstack-protector-strong], [])], [])
fi

AC_ARG_ENABLE(coverage,
	AS_HELP_STRING(
		[--enable-coverage],
		[enable coverage option, default: no]),
	[case "${enableval}" in
		yes)  coverage_mode=true;;
		no)   coverage_mode=false;;
		*)    AC_MSG_ERROR([bad value ${enableval} for --enable-coverage]);;
	esac],
	[coverage_mode=false])

if test "$coverage_mode" = true; then
	AX_CHECK_COMPILE_FLAG([-fprofile-arcs -ftest-coverage],
						  [AX_APPEND_FLAG([-fprofile-arcs -ftest-coverage], [])],
						  [AC_MSG_ERROR([--enable-coverage passed but compiler not supported])])
fi

AC_SUBST([CFLAGS])
AC_SUBST([warn_flags])
AC_SUBST([gmp_LIBS])
AC_SUBST([mpfr_LIBS])
AC_SUBST([math_LIBS])
AM_CONDITIONAL([cross_compiling], [test x$build != x$host])
AM_CONDITIONAL([USE_GMP], [test x$use_gmp = xyes])
AC_CONFIG_FILES([Makefile externals/Makefile src/Makefile test/Makefile])
AC_OUTPUT

echo "------------------------------------"
echo "CFLAGS:        $warn_flags $CFLAGS"
echo "USE_GMP:       $use_gmp"
