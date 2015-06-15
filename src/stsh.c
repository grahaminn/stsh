#include <apr_pools.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#include <string.h>

static char buffer[2048];

/* Fake readline function */
char* readline(char* prompt) 
{
	fputs(prompt, stdout);
	fgets(buffer, 2048, stdin);
	char* cpy = malloc(strlen(buffer)+1);
	strcpy(cpy, buffer);
	cpy[strlen(cpy)-1] = '\0';
	return cpy;
}

/* Fake add_history function */
void add_history(char* unused) {}

#elif __APPLE__
#include <editline/readline.h>
#include <histedit.h>
#else
#include <editline/readline.h>
#include <editline/history.h>
#endif

#include "libs/mpc/mpc.h"
#include "environment.h"
#include "cell.h"
#include "read.h"
#include "eval.h"
#include "print.h"
#include "parser.h"

int main(int argc, char **argv) 
{
	apr_pool_t* pool = NULL;
	apr_pool_initialize();
	apr_pool_create(&pool, NULL);

	environment* env = environment_new(pool);
	init_parser();
	puts("stsh version 0.0.1"); 
    
    while (1)
    {
		char *input = readline("stsh> ");
    
		add_history(input);
		mpc_result_t r;
		if (mpc_parse("<stdin>", input, stsh, &r)) 
		{
			cell* x = eval_cell(env, read_cell(r.output));
			print_cell(x);
			mpc_ast_delete(r.output);
		} 
		else 
		{    
			mpc_err_print(r.error);
			mpc_err_delete(r.error);
		}
		free(input);
	}
	
	/* Undefine and Delete our Parsers */
	parser_cleanup();
	apr_pool_destroy(pool);
	return 0;
}
