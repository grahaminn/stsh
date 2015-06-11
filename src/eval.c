#include "eval.h"

lval* builtin_op(lval* a, char* op) {
  
  /* Ensure all arguments are numbers */
  for (int i = 0; i < a->count; i++) {
    if (a->cell[i]->type != LVAL_NUM) {
      lval_del(a);
      return lval_err("Cannot operate on non-number!");
    }
  }
  
  /* Pop the first element */
  lval* x = lval_pop(a, 0);

  /* If no arguments and sub then perform unary negation */
  if ((strcmp(op, "-") == 0) && a->count == 0) {
    x->num = -x->num;
  }

  /* While there are still elements remaining */
  while (a->count > 0) {

    /* Pop the next element */
    lval* y = lval_pop(a, 0);

    if (strcmp(op, "+") == 0) { x->num += y->num; }
    if (strcmp(op, "-") == 0) { x->num -= y->num; }
    if (strcmp(op, "*") == 0) { x->num *= y->num; }
    if (strcmp(op, "/") == 0) {
      if (y->num == 0) {
        lval_del(x); lval_del(y);
        x = lval_err("Division By Zero!"); break;
      }
      x->num /= y->num;
    }

    lval_del(y);
  }

  lval_del(a); return x;
}

lval* lval_eval_sexpr(lval* v) 
{
  
	/* Evaluate Children */
	for (int i = 0; i < v->count; i++) 
	{
		v->cell[i] = lval_eval(v->cell[i]);
	}
  
	/* Error Checking */
	for (int i = 0; i < v->count; i++) 
	{
		if (v->cell[i]->type == LVAL_ERR) { return lval_take(v, i); }
	}
  
	/* Empty Expression */
	if (v->count == 0) { return v; }
  
	/* Single Expression */
	if (v->count == 1) { return lval_take(v, 0); }
  
	/* Ensure First Element is Symbol */
	lval* f = lval_pop(v, 0);
	if (f->type != LVAL_SYM) 
	{
		lval_del(f); lval_del(v);
		return lval_err("S-expression Does not start with symbol.");
	}
  
	/* Call builtin with operator */
	lval* result = builtin_op(v, f->sym);
	lval_del(f);
	return result;
}

lval* lval_eval_pexpr(lval *v)
{
	//TODO set up pipes in and out of this expression, and run it in a new process if necessary
	return v; // effectively a NO-OP for now
}

lval* lval_eval(lval* v) 
{
	/* Evaluate Sexpressions */
	if (v->type == LVAL_SEXPR) { return lval_eval_sexpr(v); }
	if (v->type == LVAL_PEXPR) { return lval_eval_pexpr(v); }
	/* All other lval types remain the same */
	return v;
}
