#ifndef _EVAL_H_
#define _EVAL_H_
#include "mpc.h"
#include "cell.h"
#include "environment.h"
#include <apr_pools.h>
#define LASSERT(pool, args, cond, fmt, ...) \
if (!(cond)) \
{ \
    cell* err = err_cell(pool, fmt, ##__VA_ARGS__); \
    return err; \
}


cell* eval_cell(apr_pool_t* pool, environment* env, cell* c);

#endif
