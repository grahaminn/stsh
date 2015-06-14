#include "read.h"
#include <string.h>

cell* read_cell_num(mpc_ast_t* t) 
{
	errno = 0;
	long x = strtol(t->contents, NULL, 10);
	return errno != ERANGE ? num_cell(x) : err_cell("invalid number");
}

cell* read_cell(mpc_ast_t* tree)
{
	/* If Symbol or Number return conversion to that type */
	if (strstr(tree->tag, "number")) { return read_cell_num(tree); }
	if (strstr(tree->tag, "symbol")) { return sym_cell(tree->contents); }
  
	/* If root (>) or sexpr then create empty list */
	cell* x = NULL;
	if (strcmp(tree->tag, ">") == 0) { x = sexpr_cell(); } 
	if (strstr(tree->tag, "sexpr"))  { x = sexpr_cell(); }
	if (strstr(tree->tag, "pexpr"))  { x = pexpr_cell(); }
  
	/* Fill this list with any valid expression contained within */
	for (int i = 0; i < tree->children_num; i++) 
	{
		if (strcmp(tree->children[i]->contents, "(") == 0) { continue; }
		if (strcmp(tree->children[i]->contents, ")") == 0) { continue; }
		if (strcmp(tree->children[i]->contents, "]") == 0) { continue; }
		if (strcmp(tree->children[i]->contents, "[") == 0) { continue; }
		if (strcmp(tree->children[i]->tag,  "regex") == 0) { continue; }
		x = add_cell(x, read_cell(tree->children[i]));
	}
  
	return x;
}

