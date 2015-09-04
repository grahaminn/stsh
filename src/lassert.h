#ifndef _LASSERT_H_
#define _LASSERT_H_

#define LASSERT_NUM(pool, func, a, num) \
if (a->count != num) \
{ \
	cell* err = err_cell(pool, func); \
	return err; \
}

#define LASSERT_TYPE(pool, func, a, i, checkedtype) \
cell* currenttype##a##i = a->first_child; \
int j##i = i; \
while (j##i--) { currenttype##a##i = currenttype##a##i->next_sibling; } \
if (currenttype##a##i->type != checkedtype) \
{ \
	cell* err = err_cell(pool, func); \
	return err; \
}

#define LASSERT(pool, args, cond, fmt, ...) \
if (!(cond)) \
{ \
    cell* err = err_cell(pool, fmt, ##__VA_ARGS__); \
    return err; \
}
#endif
