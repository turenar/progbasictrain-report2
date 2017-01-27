project(pbt-report1)

set(CMAKE_C_STANDARD 99)
add_definitions(-D_DEBUG)
include_directories(src)
add_executable(treeize
        src/treeize_main.c src/parser.c src/parser.h src/op.h src/expr.h src/config.inc.h
        src/tokenizer.c src/tokenizer.h src/error.h src/fwddecl.h src/logger.c src/logger.h src/error.c src/expr.c
        src/op_hash.c src/op_hash.h src/world.c src/world.h src/fns/fns.h src/fns/fn_const.c src/fns/fn_variable.c
        src/fns/fn_plus.c src/fns/fn_common.c src/fns/fn_common.h src/fns/fn_times.c src/fns/fn_sin.c
        src/fns/fn_subtract.c src/fns/fn_divide.c src/fns/fn_cos.c src/fns/fn_exp.c)
