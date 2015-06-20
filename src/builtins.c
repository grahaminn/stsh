#include <string.h>

#include "builtins.h"
#include "cell.h"
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

cell* builtin_add(apr_pool_t* pool, environment* env, cell* a)
{
        return builtin_op(pool, a, "+");
}

cell* builtin_sub(apr_pool_t* pool, environment* env, cell* a)
{
        return builtin_op(pool, a, "-");
}

cell* builtin_mul(apr_pool_t* pool, environment* env, cell* a)
{
        return builtin_op(pool, a, "*");
}

cell* builtin_div(apr_pool_t* pool, environment* env, cell* a)
{
        return builtin_op(pool, a, "/");
}

void add_builtin(environment* env, char* name, lbuiltin func)
{
        cell* value = fun_cell(env->pool, func);
        environment_put(env, name, value);
}

void add_halting_builtin(environment* env, char* name, lbuiltin func)
{
        cell* value = halting_fun_cell(env->pool, func);
        environment_put(env, name, value);
}

cell* builtin_car(apr_pool_t* pool, environment* env, cell* a) 
{
	/* Check Error Conditions */
	if (a->count != 1) 
	{
		return err_cell(pool, "Function 'car' passed too many arguments!");
	}
  
	if (a->cells[0]->type != SEXPR_CELL) 
	{
		return err_cell(pool, "Function 'car' passed incorrect types!");
	}
  
	if (a->cells[0]->count == 0) 
	{
		return err_cell(pool, "Function 'car' passed {}!");
	}

	/* Otherwise take first argument */
	cell* c = take_cell(pool, a, 0);
	return c;
}

cell* builtin_cdr(apr_pool_t* pool, environment* env, cell* a) 
{
	/* Check Error Conditions */
	if (a->count != 1) 
	{
		return err_cell(pool, "Function 'cdr' passed too many arguments!");
	}
  
	if (a->cells[0]->type != SEXPR_CELL) 
	{
		return err_cell(pool, "Function 'cdr' passed incorrect types!");
	}  
  
	if (a->cells[0]->count == 0) // tail of empty list is empty list
	{
		return a->cells[0];
	}

	/* Take first argument */
	cell* c = take_cell(pool, a, 0);

	/* Delete first element and return */
	return c;
}

cell* builtin_eval(apr_pool_t* pool, environment* env, cell* a) 
{
	LASSERT(pool, a, a->count == 1, "Function 'eval' passed too many arguments! Got %i expected %i", a->count, 1);
	LASSERT(pool, a, a->cells[0]->type == SEXPR_CELL, "Function 'eval' passed incorrect type!");

	cell* x = take_cell(pool, a, 0);
	x->type = SEXPR_CELL;
	return eval_cell(pool, env, x);
}

cell* builtin_list(apr_pool_t* pool, environment* env, cell* a) 
{

	for (int i = 0; i < a->count; i++) 
	{
		LASSERT(pool, a, a->cells[i]->type == SEXPR_CELL, "Function 'join' passed incorrect type.");
	}

	cell* x = pop_cell(pool, a, 0);

	while (a->count) 
	{
		x = join_cell(pool, x, pop_cell(pool, a, 0));
	}

	return x;
}

cell* builtin_if(apr_pool_t* pool, environment* env, cell* a)
{
	return NULL;
}

cell* builtin_define(apr_pool_t* pool, environment* env, cell* a) 
{
	LASSERT(pool, a, a->cells[0]->type == SYM_CELL, "Function 'define' passed incorrect type!");

	/* First argument is symbol list */
	cell* name = pop_cell(pool, a, 0);

	LASSERT(pool, a, name->type == SYM_CELL, "Function 'define' cannot define non-symbol");

	/* Check correct number of symbols and values */
	LASSERT(pool, a, a->count == 1, "Function 'define' cannot define incorrect number of values to symbols, value count: %i", a->count);

	cell* popped_cell = pop_cell(pool, a, 0);
	cell* value_cell = eval_cell(pool, env, popped_cell);
	environment_put(env, name->sym, copy_cell(pool, value_cell));

	return value_cell;
}


void add_builtins(environment* env)
{ 
        /* List Functions */
	add_builtin(env, "car", builtin_car);
	add_builtin(env, "cdr", builtin_cdr);
	add_builtin(env, "eval", builtin_eval);
	add_builtin(env, "list", builtin_list);
	add_builtin(env, "define", builtin_define);
	add_halting_builtin(env, "if", builtin_if);

        /* Mathematical Functions */
	add_builtin(env, "+", builtin_add);
	add_builtin(env, "-", builtin_sub);
	add_builtin(env, "*", builtin_mul);
	add_builtin(env, "/", builtin_div);
}

