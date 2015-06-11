#ifndef _PARSER_H_
#define _PARSER_H_
#include "libs/mpc/mpc.h"

mpc_parser_t* number = NULL;
mpc_parser_t* symbol = NULL;
mpc_parser_t* sexpr  = NULL;
mpc_parser_t* pexpr = NULL;
mpc_parser_t* qexpr = NULL;
mpc_parser_t* expr = NULL;
mpc_parser_t* stsh = NULL;

void init_parser(void)
{
	number = mpc_new("number");
	symbol = mpc_new("symbol");
	sexpr  = mpc_new("sexpr");
	pexpr = mpc_new("pexpr");
	qexpr = mpc_new("qexpr");
	expr = mpc_new("expr");
	stsh = mpc_new("stsh");


	mpca_lang(MPCA_LANG_DEFAULT,
	"																\
	number  : /-?[0-9]+/ ;											\
	symbol  : /[a-zA-Z0-9_+\\-*\\/\\\\=<>!&]+/ ;					\
	sexpr   : '(' <expr>* ')' ;										\
	pexpr   : '[' <expr>* ']' ;										\
	qexpr	: '{' <expr>* '}' ;										\
	expr    : <number> | <symbol> | <sexpr> | <pexpr> | <qexpr> ;	\
	stsh    : /^/ <expr>* /$/ ;										\
	", number, symbol, sexpr, pexpr, qexpr, expr, stsh);
}

void parser_cleanup(void)
{
	mpc_cleanup(7, number, symbol, sexpr, pexpr, qexpr, expr, stsh);
}
#endif
