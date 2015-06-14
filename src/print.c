#include "print.h"
#include <stdio.h>

void cell_expr_print(cell* v, char open, char close) 
{
	putchar(open);
	for (int i = 0; i < v->count; i++) 
	{
		/* Print Value contained within */
		print_cell(v->cells[i]);

		/* Don't print trailing space if last element */
		if (i != (v->count-1)) 
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
		case SEXPR_CELL: cell_expr_print(v, '(', ')'); break;
		case PEXPR_CELL: cell_expr_print(v, '[', ']'); break;
	}
}

