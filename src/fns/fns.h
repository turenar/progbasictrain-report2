#pragma once

#include "../expr.h"
#include "../fwddecl.h"
#include "../op.h"

extern const mat_op_def_t mat_fn_const;
extern const mat_op_def_t mat_fn_variable;

extern const mat_op_def_t mat_fn_plus;
extern const mat_op_def_t mat_fn_subtract;
extern const mat_op_def_t mat_fn_times;
extern const mat_op_def_t mat_fn_divide;

extern const mat_op_def_t mat_fn_sin;
extern const mat_op_def_t mat_fn_cos;
extern const mat_op_def_t mat_fn_exp;
extern const mat_op_def_t mat_fn_log;
extern const mat_op_def_t mat_fn_power;


void mat_fn_put_stdfunc(mat_world_t* w);
