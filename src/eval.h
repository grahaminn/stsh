#ifndef _EVAL_H_
#define _EVAL_H_
#include "libs/mpc/mpc.h"
#include "cell.h"
#include "environment.h"

cell* eval_cell(environment* env, cell* c);

#endif
