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

#include "mpc.h"
#include "builtins.h"
#include "environment.h"
#include "cell.h"
#include "read.h"
#include "eval.h"
#include "print.h"
#include "parser.h"

int main(int argc, char **argv) 
{
	apr_initialize();
	apr_pool_initialize();
	apr_pool_t* environment_pool = NULL;
	apr_pool_create(&environment_pool, NULL);

	environment* env = environment_new(environment_pool);
	init_parser();
	add_builtins(env);
	puts("stsh version 0.0.1"); 
    
	while (1)
	{
		char *input = readline("stsh> ");
    
		add_history(input);
		mpc_result_t r;
		if (mpc_parse("<stdin>", input, stsh, &r)) 
		{
			apr_pool_t* eval_pool = NULL;
			apr_pool_create(&eval_pool, NULL);
			cell* x = eval_cell(eval_pool, env, read_cell(eval_pool, r.output));
			print_cell(x);
			puts("");
			mpc_ast_delete(r.output);
			apr_pool_destroy(eval_pool);
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
	apr_pool_destroy(environment_pool);
	apr_terminate();
	return 0;
}
