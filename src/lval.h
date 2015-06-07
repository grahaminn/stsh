#ifndef _LVAL_H_
#define _LVAL_H_

enum { LVAL_ERR, LVAL_NUM, LVAL_SYM, LVAL_SEXPR, LVAL_PEXPR};

typedef struct lval 
{
  int type;
  long num;
  /* Error and Symbol types have some string data */
  char* err;
  char* sym;
  /* Count and Pointer to a list of "lval*" */
  int count;
  struct lval** cell;
} lval;

lval* lval_num(long x);

lval* lval_err(char* m);

lval* lval_symbol(char *s);

lval* lval_sexpr(void);

lval* lval_pexpr(void);

void lval_del(lval* v);
#endif
