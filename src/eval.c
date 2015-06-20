#include "eval.h"

cell* sexpr_cell_eval(apr_pool_t* pool, environment* env, cell* v) 
{
	/* Evaluate Children */
	for (int i = 0; i < v->count; i++) 
	{
		v->cells[i] = eval_cell(pool, env, v->cells[i]);
	}
		
	for (int i = 0; i < v-> count; i++)
	{
		if (v->cells[i]->type == ERR_CELL) { return take_cell(pool, v, i); }
	}

	/* Empty Expression */
        if (v->count == 0) { return v; }

        /* Single Expression */
        if (v->count == 1) { return take_cell(pool, v, 0); }
 	cell* f = pop_cell(pool, v, 0);
	if (f->type != FUN_CELL && f->type != HALTING_FUN_CELL)
        {
                return err_cell(pool, "first element is not a function.");
        }
	return f->fun(pool, env, v);
}

cell* pexpr_cell_eval(apr_pool_t* pool, environment* env, cell *v)
{
	//TODO set up pipes in and out of this expression, and run it in a new process if necessary
	return v; // effectively a NO-OP for now
}

cell* eval_cell(apr_pool_t* pool, environment* env, cell* v) 
{
	/* Evaluate Sexpressions */
	if (v->type == SYM_CELL)
	{
		cell* x = environment_get(pool, env, v->sym);
		if (x == NULL)
		{
			return v;
		}
		else 
		{
			return eval_cell(pool, env, x);
		}
	}
	if (v->type == SEXPR_CELL) { return sexpr_cell_eval(pool, env, v); }
	if (v->type == PEXPR_CELL) { return pexpr_cell_eval(pool, env, v); }
	/* All other lval types remain the same */
	return v;
}
