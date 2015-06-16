#include "read.h"
#include <string.h>

cell* read_cell_num(apr_pool_t* pool, mpc_ast_t* t) 
{
	errno = 0;
	long x = strtol(t->contents, NULL, 10);
	return errno != ERANGE ? num_cell(pool, x) : err_cell(pool, "invalid number");
}

cell* read_cell(apr_pool_t* pool, mpc_ast_t* tree)
{
	/* If Symbol or Number return conversion to that type */
	if (strstr(tree->tag, "number")) { return read_cell_num(pool, tree); }
	if (strstr(tree->tag, "symbol")) { return sym_cell(pool, tree->contents); }
  
	/* If root (>) or sexpr then create empty list */
	cell* x = NULL;
	if (strcmp(tree->tag, ">") == 0) { x = sexpr_cell(pool); } 
	if (strstr(tree->tag, "sexpr"))  { x = sexpr_cell(pool); }
	if (strstr(tree->tag, "pexpr"))  { x = pexpr_cell(pool); }
  
	/* Fill this list with any valid expression contained within */
	for (int i = 0; i < tree->children_num; i++) 
	{
		if (strcmp(tree->children[i]->contents, "(") == 0) { continue; }
		if (strcmp(tree->children[i]->contents, ")") == 0) { continue; }
		if (strcmp(tree->children[i]->contents, "]") == 0) { continue; }
		if (strcmp(tree->children[i]->contents, "[") == 0) { continue; }
		if (strcmp(tree->children[i]->tag,  "regex") == 0) { continue; }
		x = add_cell(pool, x, read_cell(pool, tree->children[i]));
	}
  
	return x;
}

