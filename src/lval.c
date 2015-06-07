#include "lval.h"

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
lval* lval_sexpr(void) 
{
	lval* v = malloc(sizeof(lval));
	v->type = LVAL_PEXPR;
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

	
		/* If Sexpr or Pexpr then delete all elements inside */
		case LVAL_SEXPR:
		case LVAL_PEXPR:
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
