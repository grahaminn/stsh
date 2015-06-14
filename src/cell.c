#include "environment.h"
#include "cell.h"
#include <stdlib.h>
#include <string.h>

/* Construct a pointer to a new Number cell */ 
cell* num_cell(apr_pool_t* pool, long x) 
{
	cell* v = apr_palloc(pool, sizeof(cell));
	v->type = NUM_CELL;
	v->num = x;
	return v;
}

/* Construct a pointer to a new Error cell */ 
cell* err_cell(apr_pool_t* pool, char* m) 
{
	cell* v = apr_palloc(pool, sizeof(cell));
	v->type = ERR_CELL;
	v->err = malloc(strlen(m) + 1);
	strcpy(v->err, m);
	return v;
}

/* Construct a pointer to a new Symbol lval */ 
cell* sym_cell(apr_pool_t* pool, char* s) 
{
	cell* v = apr_palloc(pool, sizeof(cell));
	v->type = SYM_CELL;
	v->sym = apr_palloc(pool, strlen(s) + 1);
	strcpy(v->sym, s);
	return v;
}

cell* fun_cell(apr_pool_t* pool, lbuiltin func) 
{
	cell* v = apr_palloc(pool, sizeof(cell));
	v->type = FUN_CELL;
	v->fun = func;
	return v;
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

cell* copy_cell(apr_pool_t* pool, cell* v)
{
	cell* x = apr_palloc(pool, sizeof(cell));
	x->type = v->type;

	switch (v->type)
	{

		/* Copy Functions and Numbers directly */	
		case FUN_CELL: x->fun = v->fun; break;
		case NUM_CELL: x->num = v->num; break;

		/* Copy Strings using malloc and strcpy */
		case ERR_CELL:
			x->err = apr_palloc(pool, strlen(v->err) + 1);
			strcpy(x->err, v->err);
			break;

		case SYM_CELL:
			x->sym = apr_palloc(pool, strlen(v->sym) + 1);
			strcpy(x->sym, v->sym);
			break;

		/* Copy Lists by copying each sub-expression */
		case PEXPR_CELL:
		case SEXPR_CELL:
			x->count = v->count;
			x->cells = apr_palloc(pool, sizeof(cell*) * x->count);
			for (int i = 0; i < x->count; i++)
			{
				x->cells[i] = copy_cell(pool, v->cells[i]);
			}
			break;
	}
	return x;
}
