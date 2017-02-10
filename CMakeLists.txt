project(pbt-report1)

set(CMAKE_C_STANDARD 99)
add_definitions(-D_DEBUG)
include_directories(src externals)

add_library(matcore src/parser.c src/parser.h src/op.h src/expr.h src/config.inc.h
        src/tokenizer.c src/tokenizer.h src/error.h src/fwddecl.h src/logger.c src/logger.h src/error.c src/expr.c
        src/op_hash.c src/op_hash.h src/world.c src/world.h src/op.c)
add_library(matfunc src/fns/fns.h src/fns/fn_const.c src/fns/fn_variable.c
        src/fns/fn_plus.c src/fns/fn_common.c src/fns/fn_common.h src/fns/fn_times.c src/fns/fn_sin.c
        src/fns/fn_subtract.c src/fns/fn_divide.c src/fns/fn_cos.c src/fns/fn_exp.c src/fns/fn_log.c src/fns/fn_power.c)
add_executable(treeize
        src/treeize_main.c)
add_dependencies(treeize matcore matfunc)
add_executable(fn_test test/fns/fns_test.h test/fns/fns_test.c test/fns/fns_test_common.h test/fns/fns_test_common.c
        test/fns/fn_cos_test.c test/fns/fn_divide_test.c test/fns/fn_exp_test.c test/fns/fn_log_test.c
        test/fns/fn_plus_test.c test/fns/fn_power_test.c test/fns/fn_sin_test.c test/fns/fn_subtract_test.c
        test/fns/fn_times_test.c test/tokenizer_test.c test/cutest_util.h test/cutest_util.c)
add_executable(tokenizer_test test/tokenizer_test.c)
add_executable(paser_test test/parser_test.c)
add_executable(check_double test/check_double.c)
