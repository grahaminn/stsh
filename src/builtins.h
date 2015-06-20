#ifndef _BUILTINS_H_
#define _BUILTINS_H_

#include "environment.h"
#define LASSERT(pool, args, cond, fmt, ...) \
if (!(cond)) \
{ \
	cell* err = err_cell(pool, fmt, ##__VA_ARGS__); \
	return err; \
}

void add_builtins(environment* env);
#endif
