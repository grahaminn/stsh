#include "environment.h"
#include "cell.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* Construct a pointer to a new Number cell */ 
cell* num_cell(apr_pool_t* pool, long x) 
{
	cell* c = apr_palloc(pool, sizeof(cell));
	c->type = NUM_CELL;
	c->num = x;
	return c;
}

/* Construct a pointer to a new Error cell */ 
cell* err_cell(apr_pool_t* pool, char* fmt, ...) 
{
	cell* c = apr_palloc(pool, sizeof(cell));
	c->type = ERR_CELL;
	va_list va;
	va_start(va, fmt);
	c->err = apr_palloc(pool, 512);
	vsnprintf(c->err, 511, fmt, va);
	va_end(va);
	return c;
}

/* Construct a pointer to a new Symbol lval */ 
cell* sym_cell(apr_pool_t* pool, char* s) 
{
	cell* c = apr_palloc(pool, sizeof(cell));
	c->type = SYM_CELL;
	c->sym = apr_palloc(pool, strlen(s) + 1);
	strcpy(c->sym, s);
	return c;
}

cell* halting_fun_cell(apr_pool_t* pool, lbuiltin func)
{
	cell* c = apr_palloc(pool, sizeof(cell));
        c->type = HALTING_FUN_CELL;
        c->fun = func;
        return c;
}

cell* fun_cell(apr_pool_t* pool, lbuiltin func) 
{
	cell* c = apr_palloc(pool, sizeof(cell));
	c->type = FUN_CELL;
	c->fun = func;
	return c;
}

/* A pointer to a new empty Sexpr lval */
cell* sexpr_cell(apr_pool_t* pool) 
{
	cell* v = apr_palloc(pool, sizeof(cell));
	v->type = SEXPR_CELL;
	v->count = 0;
	v->cells = NULL;
	return v;
}

/* A pointer to a new empty Pexpr lval */
cell* pexpr_cell(apr_pool_t* pool) 
{
	cell* v = apr_palloc(pool, sizeof(cell));
	v->type = PEXPR_CELL;
	v->count = 0;
	v->cells = NULL;
	return v;
}

cell* pop_cell(apr_pool_t* pool, cell* v, int i) 
{
	/* Find the item at "i" */
	cell* x = v->cells[i];
  
	/* Shift memory after the item at "i" over the top */
	memmove(&v->cells[i], &v->cells[i+1],
	sizeof(cell*) * (v->count-i-1));
  
	/* Decrease the count of items in the list */
	v->count--;
  
	/* Reallocate the memory used */
	v->cells = realloc(v->cells, sizeof(cell*) * v->count);
	return x;
}

cell* take_cell(apr_pool_t* pool, cell* v, int i) 
{
	cell* x = pop_cell(pool, v, i);
	return x;
}

cell* add_cell(apr_pool_t* pool, cell* v, cell* x) 
{
	v->count++;
 	v->cells = realloc(v->cells, sizeof(cell*) * v->count);
	v->cells[v->count-1] = x;
	return v;
}

cell* join_cell(apr_pool_t* pool, cell* x, cell* y) 
{
	/* For each cell in 'y' add it to 'x' */
	while (y->count) 
	{
		add_cell(pool, x, pop_cell(pool, y, 0));
	}

	/* Delete the empty 'y' and return 'x' */
	return x;
}

cell* copy_cell(apr_pool_t* pool, cell* v)
{
	cell* x = apr_palloc(pool, sizeof(cell));
	x->type = v->type;
	if (v->sym != NULL)
	{
		x->sym = apr_palloc(pool, strlen(v->sym) + 1);
		strcpy(x->sym, v->sym);
	}
	if (v->sym != NULL)
	{
		x->err = apr_palloc(pool, strlen(v->err) + 1);
                strcpy(x->err, v->err);
	}
	x->num = v->num;
	x->count = v->count;
	if (x->count > 0)
	{
		x->cells = apr_palloc(pool, sizeof(cell*) * x->count);
		for (int i = 0; i < x->count; i++)
		{
			x->cells[i] = copy_cell(pool, v->cells[i]);
		}
	}
	x->fun = v->fun;
	return x;
}
