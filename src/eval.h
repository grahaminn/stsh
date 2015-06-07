#ifndef _EVAL_H_
#define _EVAL_H_
#include "libs/mpc/mpc.h"
#include "lval.h"

lval* lval_eval(mpc_ast_t* tree);

#endif
