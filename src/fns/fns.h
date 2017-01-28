#pragma once

#include "../fwddecl.h"
#include "../op.h"

extern mat_op_def_t mat_fn_const;
extern mat_op_def_t mat_fn_variable;

extern mat_op_def_t mat_fn_plus;
extern mat_op_def_t mat_fn_subtract;
extern mat_op_def_t mat_fn_times;
extern mat_op_def_t mat_fn_divide;

extern mat_op_def_t mat_fn_sin;
extern mat_op_def_t mat_fn_cos;
extern mat_op_def_t mat_fn_exp;
extern mat_op_def_t mat_fn_log;
extern mat_op_def_t mat_fn_power;


void mat_fn_put_stdfunc(mat_world_t* w);
