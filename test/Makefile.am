AM_CFLAGS = $(warn_flags) -isystem $(top_srcdir)/externals -I$(top_srcdir)/src '-Dtestdatadir="$(srcdir)/data"'
AM_LDFLAGS = -static

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

shell_tests = treeize_test.sh differential_test.sh
TESTS = fns_test tokenizer_test parser_test
if !cross_compiling
TESTS+= $(shell_tests)
endif
check_PROGRAMS = fns_test tokenizer_test parser_test check_double
check_LTLIBRARIES = libtest_common.la
common_ldadd = ../src/libmatcore.la \
			   ../src/libmatfunc.la \
			   ../externals/libcutest.la \
			   $(gmp_LIBS) $(mpfr_LIBS) $(math_LIBS)

libtest_common_la_SOURCES = cutest_util.c
libtest_common_la_LIBADD = $(common_ldadd)

fns_test_SOURCES = fns/fns_test.c \
				   fns/fn_cos_test.c \
				   fns/fn_divide_test.c \
				   fns/fn_exp_test.c \
				   fns/fn_log_test.c \
				   fns/fn_plus_test.c \
				   fns/fn_power_test.c \
				   fns/fn_sin_test.c \
				   fns/fn_subtract_test.c \
				   fns/fn_times_test.c \
				   fns/fns_test_common.c
fns_test_LDADD = libtest_common.la

tokenizer_test_SOURCES = tokenizer_test.c
tokenizer_test_LDADD = libtest_common.la

parser_test_SOURCES = parser_test.c
parser_test_LDADD = libtest_common.la

check_double_SOURCES = check_double.c
check_double_LDADD = libtest_common.la
