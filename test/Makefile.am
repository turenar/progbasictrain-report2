AM_CFLAGS = $(warn_flags) -isystem $(top_srcdir)/externals -I$(top_srcdir)/src '-Dtestdatadir="$(srcdir)/data"'

if DEBUG
AM_TESTS_ENVIRONMENT = LSAN_OPTIONS=suppressions=$(top_srcdir)/misc/lsan_ignore.txt; \
					   export LSAN_OPTIONS
else
AM_TESTS_ENVIRONMENT =
endif

AM_TESTS_ENVIRONMENT+= \
	EXEEXT=$(EXEEXT); \
	srcdir=$(srcdir); \
	export EXEEXT srcdir;

TESTS = fns_test
check_PROGRAMS = fns_test
fns_test_SOURCES = fns/fns_test.c \
				   fns/fn_divide_test.c \
				   fns/fn_plus_test.c \
				   fns/fn_subtract_test.c \
				   fns/fn_times_test.c \
				   fns/fns_test_common.c
fns_test_LDADD = $(top_builddir)/src/libmatfunc.la $(top_builddir)/externals/libcutest.a
