#ifndef _EVAL_H_
#define _EVAL_H_
#include "mpc.h"
#include "cell.h"
#include "environment.h"
#include "lassert.h"

#include <apr_pools.h>

cell* eval_cell(apr_pool_t* pool, environment* env, cell* c);

cell* call_cell(apr_pool_t* pool, environment* e, cell* function, cell* a);
#endif
