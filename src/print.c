#include "print.h"
#include <stdio.h>

void cell_expr_print(cell* v, char open, char close) 
{
	putchar(open);
	cell* child = v->first_child;
	while (child != NULL)
	{
		print_cell(child);
		child = child->next_sibling;
		if (child != NULL)
		{
			putchar(' ');
		}
	}
	putchar(close);
}

void print_cell(cell* v)
{
	switch (v->type) 
	{
		case NUM_CELL:   printf("%li", v->num); break;
		case ERR_CELL:   printf("Error: %s", v->err); break;
		case SYM_CELL:   printf("%s", v->sym); break;
		case FUN_CELL: 
			if (v->builtin) 
			{
				printf("<builtin>");
			} 
			else 
			{
				printf("(λ "); 
				print_cell(v->formals);
				putchar(' '); 
				print_cell(v->body); 
				putchar(')');
			}
			break;
		case SEXPR_CELL: cell_expr_print(v, '(', ')'); break;
		case PEXPR_CELL: cell_expr_print(v, '[', ']'); break;
	}
}

