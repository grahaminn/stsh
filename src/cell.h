#ifndef _CELL_H_
#define _CELL_H_

#include <apr_pools.h>

struct environment;
struct cell;
typedef struct environment environment;
typedef struct cell cell;

typedef cell* (*lbuiltin) (environment*, cell*);

enum { ERR_CELL, NUM_CELL, SYM_CELL, FUN_CELL, SEXPR_CELL, PEXPR_CELL};

typedef struct cell 
{
	int type;
	long num;
	/* Error and Symbol types have some string data */
	char* err;
	char* sym;
	lbuiltin fun;

	/* Count and Pointer to a list of "lval*" */
	int count;
	struct cell** cells;
} cell;

cell* num_cell(apr_pool_t* pool, long x);

cell* err_cell(apr_pool_t* pool, char* m);

cell* sym_cell(apr_pool_t* pool, char *s);

cell* sexpr_cell(apr_pool_t* pool);

cell* pexpr_cell(apr_pool_t* pool);

cell* pop_cell(apr_pool_t* pool, cell* v, int i); 

cell* take_cell(apr_pool_t* pool, cell* v, int i); 

cell* add_cell(apr_pool_t* pool, cell* v, cell* x);

cell* copy_cell(apr_pool_t* pool, cell* c);
#endif
