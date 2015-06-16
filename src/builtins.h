#ifndef _BUILTINS_H_
#define _BUILTINS_H_

#include "environment.h"

#define LASSERT(pool, args, cond, err) if (!(cond)) { return err_cell(pool, err); }

void add_builtins(environment* env);
#endif
