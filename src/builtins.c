#include <string.h>

#include "builtins.h"
#include "cell.h"
#include "eval.h"
#include "print.h"

cell* builtin_op(apr_pool_t* pool, cell* a, char* op) 
{
	/* Ensure all arguments are numbers */
	cell* child = a->first_child;
	while(child != NULL) 
	{
    	if (child->type != NUM_CELL) 
		{
printf("error cell:"); print_cell(a);
      		return err_cell(pool, "Cannot operate on non-number!");
    	}
		child = child->next_sibling;
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

cell* builtin_lambda(apr_pool_t* pool, environment* env, cell* a)
{
	LASSERT_NUM(pool, "lambda", a, 2);
	LASSERT_TYPE(pool, "lambda", a, 0, SEXPR_CELL);
	LASSERT_TYPE(pool, "lambda", a, 1, SEXPR_CELL);

	cell* first = a->first_child;
	cell* current = first->first_child;
	for (int i = 0; i < first->count; i++) 
	{
		LASSERT(pool, a, (current->type == SYM_CELL),
      "Cannot define non-symbol. Got %s, Expected %s.",
      cell_type_name(current->type), cell_type_name(SYM_CELL));
		current = current->next_sibling;
	}
	cell* formals = pop_cell(pool, a, 0);
	cell* body = pop_cell(pool, a, 0);

	return lambda_cell(pool, env, formals, body);	
}

void add_builtin(environment* env, char* name, lbuiltin func)
{
        cell* value = fun_cell(env->pool, env, func);
        environment_global_define(env, name, value);
}

void add_halting_builtin(environment* env, char* name, lbuiltin func)
{
        cell* value = halting_fun_cell(env->pool, env, func);
        environment_global_define(env, name, value);
}

cell* builtin_car(apr_pool_t* pool, environment* env, cell* a) 
{
	/* Check Error Conditions */
	if (a->count != 1) 
	{
		return err_cell(pool, "Function 'car' passed too many arguments!");
	}
  
	if (a->first_child->type != SEXPR_CELL) 
	{
		return err_cell(pool, "Function 'car' passed incorrect types!");
	}
  
	if (a->first_child->count == 0) 
	{
		return err_cell(pool, "Function 'car' passed ()!");
	}

	/* Otherwise take first argument */
	cell* c = pop_cell(pool, a, 0);
	return c;
}

cell* builtin_cdr(apr_pool_t* pool, environment* env, cell* a) 
{
	/* Check Error Conditions */
	if (a->count != 1) 
	{
		return err_cell(pool, "Function 'cdr' passed too many arguments!");
	}
  
	if (a->first_child->type != SEXPR_CELL) 
	{
		return err_cell(pool, "Function 'cdr' passed incorrect types!");
	}  
  
	if (a->first_child->count == 0) // tail of empty list is empty list
	{
		return a->first_child;
	}

	/* Take first argument */
	cell* c = pop_cell(pool, a, 0);

	/* Delete first element and return */
	return c;
}

cell* builtin_eval(apr_pool_t* pool, environment* env, cell* a) 
{
	LASSERT(pool, a, a->count == 1, "Function 'eval' passed too many arguments! Got %i expected %i", a->count, 1);
	LASSERT(pool, a, a->first_child->type == SEXPR_CELL, "Function 'eval' passed incorrect type!");

	cell* x = pop_cell(pool, a, 0);
	x->type = SEXPR_CELL;
	return eval_cell(pool, env, x);
}

cell* builtin_list(apr_pool_t* pool, environment* env, cell* a) 
{

	cell* child = a->first_child;
	while (child != NULL) 
	{
		
		LASSERT(pool, a, child->type == SEXPR_CELL, "Function 'join' passed incorrect type.");
		child = child->next_sibling;
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
	LASSERT(pool, a, a->first_child->type == SYM_CELL, "Function 'define' passed incorrect type!");

	/* First argument is symbol list */
	cell* name = pop_cell(pool, a, 0);

	LASSERT(pool, a, name->type == SYM_CELL, "Function 'define' cannot define non-symbol");

	/* Check correct number of symbols and values */
	LASSERT(pool, a, a->count == 1, "Function 'define' cannot define incorrect number of values to symbols, value count: %i", a->count);

	cell* popped_cell = pop_cell(pool, a, 0);
	cell* value_cell = eval_cell(pool, env, popped_cell);
	environment_global_define(env, name->sym, copy_cell(pool, value_cell));

	return value_cell;
}

cell* builtin_var(environment* env, cell* a, char* func)
{
	LASSERT_TYPE(env->pool, func, a, 0, SEXPR_CELL);

	cell* syms = a->first_child;

	cell* current_sym = syms->first_child;
	for(int i = 0; i < syms->count; i++) 
	{
		LASSERT(env->pool, a, (current_sym->type == SYM_CELL),
		"Function '%s' cannot define non-symbol. "
		"Got %s, Expected %s.", func, 
		cell_type_name(current_sym->type),
		cell_type_name(SYM_CELL));
		current_sym = current_sym->next_sibling; 
	}

	LASSERT(env->pool, a, (syms->count == a->count-1),
    "Function '%s' passed too many arguments for symbols. "
    "Got %i, Expected %i.", func, syms->count, a->count-1);
	
	current_sym = syms->first_child;
	cell* currenta = a->first_child->next_sibling;
	for (int i = 0; i < syms->count; i++)
	{
		if (strcmp(func, "define") == 0) 
		{
			environment_global_define(env, current_sym->sym, currenta); 
		}
		
		if (strcmp(func, "let") == 0)
		{
			environment_put(env, current_sym->sym, currenta);
		}
		current_sym = current_sym->next_sibling;
		currenta = currenta->next_sibling;
	}

	return sexpr_cell(env->pool);
}

cell* builtin_put(environment* env, cell* a)
{
    return builtin_var(env, a, "let");
}

cell* builtin_def(environment* env, cell* a)
{
    return builtin_var(env, a, "define");
}

void add_builtins(environment* env)
{ 
        /* List Functions */
	add_builtin(env, "car", builtin_car);
	add_builtin(env, "cdr", builtin_cdr);
	add_builtin(env, "eval", builtin_eval);
	add_builtin(env, "list", builtin_list);

		/* language primitives */
	add_builtin(env, "define", builtin_define);
	add_halting_builtin(env, "if", builtin_if);
	add_builtin(env, "lambda", builtin_lambda);

        /* Mathematical Functions */
	add_builtin(env, "+", builtin_add);
	add_builtin(env, "-", builtin_sub);
	add_builtin(env, "*", builtin_mul);
	add_builtin(env, "/", builtin_div);
}

