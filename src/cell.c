#include "builtins.h"
#include "cell.h"
#include "environment.h"
#include "print.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* cell_type_name(int t) 
{
	switch(t) 
	{
		case FUN_CELL: return "Function";
		case NUM_CELL: return "Number";
		case ERR_CELL: return "Error";
		case SYM_CELL: return "Symbol";
		case SEXPR_CELL: return "S-Expression";
		case PEXPR_CELL: return "P-Expression";
		default: return "Unknown";
	}
}

/* Construct a pointer to a new Number cell */ 
cell* num_cell(apr_pool_t* pool, long x) 
{
	cell* c = apr_palloc(pool, sizeof(cell));
	c->type = NUM_CELL;
	c->next_sibling = NULL;
	c->num = x;
	return c;
}

/* Construct a pointer to a new Error cell */ 
cell* err_cell(apr_pool_t* pool, char* fmt, ...) 
{
	cell* c = apr_palloc(pool, sizeof(cell));
	c->type = ERR_CELL;
	c->next_sibling = NULL;
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
	c->next_sibling = NULL;
	c->sym = apr_palloc(pool, strlen(s) + 1);
	strcpy(c->sym, s);
	return c;
}

cell* halting_fun_cell(apr_pool_t* pool, environment* parent, lbuiltin builtin)
{
	cell* c = apr_palloc(pool, sizeof(cell));
	c->type = HALTING_FUN_CELL;
	c->env = environment_new(pool, parent);
	c->next_sibling = NULL;
	c->builtin = builtin;
	return c;
}

cell* fun_cell(apr_pool_t* pool, environment* parent, lbuiltin builtin) 
{
	cell* c = apr_palloc(pool, sizeof(cell));
	c->type = FUN_CELL;
	c->env = environment_new(pool, parent);
	c->next_sibling = NULL;
	c->builtin = builtin;
	return c;
}

cell* lambda_cell(apr_pool_t* pool, environment* parent, cell* formals, cell* body)
{
	cell* c = apr_palloc(pool, sizeof(cell));
	c->type = FUN_CELL;
	c->builtin = NULL;
	c->env = environment_new(pool, parent);
	c->formals = formals;
	c->body = body;
	return c;
}

/* A pointer to a new empty Sexpr lval */
cell* sexpr_cell(apr_pool_t* pool) 
{
	cell* v = apr_palloc(pool, sizeof(cell));
	v->type = SEXPR_CELL;
	v->count = 0;
	v->first_child = NULL;
	v->last_child = NULL;
	v->next_sibling = NULL;
	return v;
}

/* A pointer to a new empty Pexpr lval */
cell* pexpr_cell(apr_pool_t* pool) 
{
	cell* v = apr_palloc(pool, sizeof(cell));
	v->type = PEXPR_CELL;
	v->count = 0;
	v->first_child = NULL;
	v->last_child = NULL;
	v->next_sibling = NULL;
	return v;
}

cell* pop_cell(apr_pool_t* pool, cell* v, int i) 
{
	
	if (v->count == 0 || i >= v->count) {	return NULL; }
	/* Find the item at "i" */
	cell* child = v->first_child;
	cell* result = NULL;
	
	if (i == 0) 
	{
		result = child;
	
		if (child->next_sibling != NULL)
		{
			v->first_child = child->next_sibling;
		}
		else
		{
			v->first_child = NULL;
			v->last_child = NULL;
		}
	}
	else
	{
		cell* previous = NULL;
		while (child != NULL && i-- > 0)
		{
			previous = child;
			child = child->next_sibling;
		}
        result = child;
        if (result->next_sibling == NULL)
        {
			v->last_child = child;
        }
        previous->next_sibling = child->next_sibling;
	}
	/* Decrease the count of items in the list */
	--(v->count);
	return result;
}

cell* add_cell(apr_pool_t* pool, cell* v, cell* x) 
{
	cell* child = v->first_child;
	if (child == NULL)
	{	
		v->first_child = x;
		v->last_child = x;
	}
	else
	{
		v->last_child->next_sibling = x;
		v->last_child = x;
	}
	++(v->count);	
	return v;
}

cell* join_cell(apr_pool_t* pool, cell* x, cell* y) 
{
	/* For each cell in 'y' add it to 'x' */
	int count = y->count;
	while (count--) 
	{
		add_cell(pool, x, pop_cell(pool, y, 0));
	}

	return x;
}

cell* copy_cell(apr_pool_t* pool, cell* v)
{
	if (v == NULL) return NULL;
	cell* x = apr_palloc(pool, sizeof(cell));
	x->type = v->type;
	if (v->sym != NULL)
	{
		x->sym = apr_palloc(pool, strlen(v->sym) + 1);
		strcpy(x->sym, v->sym);
	}
	if (v->err != NULL)
	{
		x->err = apr_palloc(pool, strlen(v->err) + 1);
                strcpy(x->err, v->err);
	}
	x->num = v->num;
	if (v->count > 0)
	{
		cell* vcell = v->first_child;
		while (vcell != NULL)
		{
			add_cell(pool, x,  copy_cell(pool, vcell));
			vcell = vcell->next_sibling;
		}
	}
    x->formals = copy_cell(pool, v->formals);
    x->body = copy_cell(pool, v->body);
	x->builtin = v->builtin;
	return x;
}
