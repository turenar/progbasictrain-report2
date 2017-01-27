project(pbt-report1)

set(CMAKE_C_STANDARD 99)
include_directories(src)
add_executable(treeize src/treeize_main.c src/parser.c src/parser.h src/op.h src/expr.h src/config.inc.h
        src/tokenizer.c src/tokenizer.h src/error.h src/fwddecl.h src/logger.c src/logger.h src/error.c src/expr.c)
