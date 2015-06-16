#ifndef _EVAL_H_
#define _EVAL_H_
#include "mpc.h"
#include "cell.h"
#include "environment.h"
#include <apr_pools.h>

cell* eval_cell(apr_pool_t* pool, environment* env, cell* c);

#endif
