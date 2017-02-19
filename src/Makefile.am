AM_CFLAGS = $(warn_flags)
bin_PROGRAMS = treeize differential extreme_value
lib_LTLIBRARIES = libmatcore.la libmatfunc.la

libmatcore_la_SOURCES = error.c \
						expr.c \
						tokenizer.c \
						newton.c \
						op.c \
						op_hash.c \
						parser.c \
						logger.c \
						world.c \
						fns/fn_const.c \
						fns/fn_variable.c
if !USE_GMP
libmatcore_la_SOURCES+= compat/gmp_wrapper.c \
						compat/mpfr_wrapper.c
endif
libmatcore_la_LIBADD = $(gmp_LIBS) $(mpfr_LIBS) $(math_LIBS)
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
libmatfunc_la_LIBADD = libmatcore.la
libmatfunc_la_LDFLAGS = -version-info $(mat_so_version)
treeize_SOURCES = treeize_main.c
treeize_LDADD = libmatcore.la libmatfunc.la $(math_LIBS)
treeize_LDFLAGS = -static
differential_SOURCES = differential_main.c
differential_LDADD = libmatcore.la libmatfunc.la $(math_LIBS)
differential_LDFLAGS = -static
extreme_value_SOURCES = extreme_value_main.c
extreme_value_LDADD = libmatcore.la libmatfunc.la $(math_LIBS)
extreme_value_LDFLAGS = -static
