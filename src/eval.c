#include "eval.h"
#include "print.h"

cell* sexpr_cell_eval(apr_pool_t* pool, environment* env, cell* v) 
{
	/* Evaluate Children */
	cell* child = v->first_child;
	cell* next = child->next_sibling;
	child = eval_cell(pool, env, child);
	v->first_child = child;
	v->first_child->next_sibling = next;

	while (next != NULL) 
	{	
		cell* evaluated = eval_cell(pool, env, next);
		child->next_sibling = evaluated;
		child = evaluated;
		next = next->next_sibling;
	}
	v->last_child=child;

	child = v->first_child;
    int i = 0;		
	while(child != NULL)
	{
		if (child->type == ERR_CELL) { return pop_cell(pool, v, i); }
		child = child->next_sibling;
		++i;
	}

	/* Empty Expression */
    if (v->count == 0) { return v; }

    /* Single Expression */
    if (v->count == 1) { return pop_cell(pool, v, 0); }
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
