#include "eval.h"

cell* builtin_op(apr_pool_t* pool, cell* a, char* op) 
{
  
	/* Ensure all arguments are numbers */
	for (int i = 0; i < a->count; i++) 
	{
		if (a->cells[i]->type != NUM_CELL) 
		{
			return err_cell(pool, "Cannot operate on non-number!");
		}
	}
  
	/* Pop the first element */
	cell* x = pop_cell(pool, a, 0);

	/* If no arguments and sub then perform unary negation */
	if ((strcmp(op, "-") == 0) && a->count == 0) 
	{
		x->num = -x->num;
	}

	/* While there are still elements remaining */
	while (a->count > 0) 
	{

		/* Pop the next element */
		cell* y = pop_cell(pool, a, 0);

		if (strcmp(op, "+") == 0) { x->num += y->num; }
		if (strcmp(op, "-") == 0) { x->num -= y->num; }
		if (strcmp(op, "*") == 0) { x->num *= y->num; }
		if (strcmp(op, "/") == 0) 
		{
			if (y->num == 0) 
			{
				x = err_cell(pool, "Division By Zero!"); break;
			}
			x->num /= y->num;
		}
	}
	return x;
}

cell* sexpr_cell_eval(apr_pool_t* pool, environment* env, cell* v) 
{
  
	/* Evaluate Children */
	for (int i = 0; i < v->count; i++) 
	{
		v->cells[i] = eval_cell(pool, env, v->cells[i]);
	}
  
	/* Error Checking */
	for (int i = 0; i < v->count; i++) 
	{
		if (v->cells[i]->type == ERR_CELL) { return take_cell(pool, v, i); }
	}
  
	/* Empty Expression */
	if (v->count == 0) { return v; }
  
	/* Single Expression */
	if (v->count == 1) { return take_cell(pool, v, 0); }
  
	/* Ensure First Element is Symbol */
	cell* f = pop_cell(pool, v, 0);
	if (f->type != FUN_CELL) 
	{
		return err_cell(pool, "first element is not a function.");
	}
  
	/* Call builtin with operator */
	cell* result = f->fun(env, v);
	return result;
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
		cell* x = environment_get(env, v);
		return x;
	}
	if (v->type == SEXPR_CELL) { return sexpr_cell_eval(pool, env, v); }
	if (v->type == PEXPR_CELL) { return pexpr_cell_eval(pool, env, v); }
	/* All other lval types remain the same */
	return v;
}
