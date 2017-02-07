AM_CFLAGS = $(warn_flags)
bin_PROGRAMS = treeize
lib_LTLIBRARIES = libmatcore.la libmatfunc.la

libmatcore_la_SOURCES = error.c \
						expr.c \
						tokenizer.c \
						op.c \
						op_hash.c \
						parser.c \
						logger.c \
						world.c \
						fns/fn_const.c \
						fns/fn_variable.c
libmatcore_la_LIBADD = $(gmp_LIBS)
libmatcore_la_LDFLAGS = -version-info $(mat_so_version)
libmatfunc_la_SOURCES = fns/fn_common.c \
						fns/fn_cos.c \
						fns/fn_divide.c \
						fns/fn_exp.c \
						fns/fn_log.c \
						fns/fn_times.c \
						fns/fn_plus.c \
						fns/fn_power.c \
						fns/fn_sin.c \
						fns/fn_subtract.c
libmatfunc_la_LIBADD = libmatcore.la $(mpfr_LIBS)
libmatfunc_la_LDFLAGS = -version-info $(mat_so_version)
treeize_SOURCES = treeize_main.c
treeize_LDADD = libmatcore.la libmatfunc.la
treeize_LDFLAGS = -static
