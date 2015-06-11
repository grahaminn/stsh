#include "read.h"
#include <string.h>

lval* lval_read_num(mpc_ast_t* t) 
{
	errno = 0;
	long x = strtol(t->contents, NULL, 10);
	return errno != ERANGE ? lval_num(x) : lval_err("invalid number");
}

lval* lval_read(mpc_ast_t* tree)
{
	/* If Symbol or Number return conversion to that type */
	if (strstr(tree->tag, "number")) { return lval_read_num(tree); }
	if (strstr(tree->tag, "symbol")) { return lval_sym(tree->contents); }
  
	/* If root (>) or sexpr then create empty list */
	lval* x = NULL;
	if (strcmp(tree->tag, ">") == 0) { x = lval_sexpr(); } 
	if (strstr(tree->tag, "sexpr"))  { x = lval_sexpr(); }
	if (strstr(tree->tag, "pexpr"))  { x = lval_pexpr(); }
  
	/* Fill this list with any valid expression contained within */
	for (int i = 0; i < tree->children_num; i++) 
	{
		if (strcmp(tree->children[i]->contents, "(") == 0) { continue; }
		if (strcmp(tree->children[i]->contents, ")") == 0) { continue; }
		if (strcmp(tree->children[i]->contents, "]") == 0) { continue; }
		if (strcmp(tree->children[i]->contents, "[") == 0) { continue; }
		if (strcmp(tree->children[i]->tag,  "regex") == 0) { continue; }
		x = lval_add(x, lval_read(tree->children[i]));
	}
  
	return x;
}

