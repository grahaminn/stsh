#include "lval.h"
#include <stdlib.h>
#include <string.h>

/* Construct a pointer to a new Number lval */ 
lval* lval_num(long x) 
{
	lval* v = malloc(sizeof(lval));
	v->type = LVAL_NUM;
	v->num = x;
	return v;
}

/* Construct a pointer to a new Error lval */ 
lval* lval_err(char* m) 
{
	lval* v = malloc(sizeof(lval));
	v->type = LVAL_ERR;
	v->err = malloc(strlen(m) + 1);
	strcpy(v->err, m);
	return v;
}

/* Construct a pointer to a new Symbol lval */ 
lval* lval_sym(char* s) 
{
	lval* v = malloc(sizeof(lval));
	v->type = LVAL_SYM;
	v->sym = malloc(strlen(s) + 1);
	strcpy(v->sym, s);
	return v;
}

/* A pointer to a new empty Sexpr lval */
lval* lval_sexpr(void) 
{
	lval* v = malloc(sizeof(lval));
	v->type = LVAL_SEXPR;
	v->count = 0;
	v->cell = NULL;
	return v;
}

/* A pointer to a new empty Pexpr lval */
lval* lval_pexpr(void) 
{
	lval* v = malloc(sizeof(lval));
	v->type = LVAL_PEXPR;
	v->count = 0;
	v->cell = NULL;
	return v;
}

/*A point to a new empty Qexpr lval */
lval* lval_qexpr(void)
{
	lval* v = malloc(sizeof(lval));
	v->type = LVAL_QEXPR;
	v->count = 0;
	v->cell = NULL;
	return v;
}

void lval_del(lval* v) 
{

	switch (v->type) 
	{
		/* Do nothing special for number type */
		case LVAL_NUM: break;

		/* For Err or Sym free the string data */
		case LVAL_ERR: free(v->err); break;
		case LVAL_SYM: free(v->sym); break;

	
		/* If P|Q|Sexpr then delete all elements inside */
		case LVAL_PEXPR:
		case LVAL_QEXPR:
		case LVAL_SEXPR:
			for (int i = 0; i < v->count; i++) 
			{
				lval_del(v->cell[i]);
			}
			/* Also free the memory allocated to contain the pointers */
			free(v->cell);
			break;
	}	

	/* Free the memory allocated for the "lval" struct itself */
	free(v);
}

lval* lval_pop(lval* v, int i) 
{
	/* Find the item at "i" */
	lval* x = v->cell[i];
  
	/* Shift memory after the item at "i" over the top */
	memmove(&v->cell[i], &v->cell[i+1],
	sizeof(lval*) * (v->count-i-1));
  
	/* Decrease the count of items in the list */
	v->count--;
  
	/* Reallocate the memory used */
	v->cell = realloc(v->cell, sizeof(lval*) * v->count);
	return x;
}

lval* lval_take(lval* v, int i) 
{
	lval* x = lval_pop(v, i);
	lval_del(v);
	return x;
}

lval* lval_add(lval* v, lval* x) 
{
	v->count++;
 	v->cell = realloc(v->cell, sizeof(lval*) * v->count);
	v->cell[v->count-1] = x;
	return v;
}
