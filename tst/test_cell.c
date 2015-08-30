#include "../src/cell.h"
#include "test_cell.h"

apr_pool_t* pool;

START_TEST (test_join_cell)
{
	apr_pool_t* pool = NULL;
	apr_pool_create(&pool, NULL);
	cell* parenta = sexpr_cell(pool);
	cell* parentb = sexpr_cell(pool);

	cell* child1 = num_cell(pool, 1);
	cell* child2 = num_cell(pool, 2);

	cell* childx = sym_cell(pool, "x");
	cell* childy = sym_cell(pool, "y");

	add_cell(pool, parenta, child1);
	add_cell(pool, parenta, childx);
	add_cell(pool, parentb, child2);
	add_cell(pool, parentb, childy);

	cell* result = join_cell(pool, parenta, parentb);
	ck_assert_int_eq(4, result->count);
	ck_assert_ptr_eq(child1, pop_cell(pool, result, 0));
	ck_assert_ptr_eq(childx, pop_cell(pool, result, 0));
	ck_assert_ptr_eq(child2, pop_cell(pool, result, 0));
	ck_assert_ptr_eq(childy, pop_cell(pool, result, 0));
	ck_assert_int_eq(0, result->count);

	apr_pool_destroy(pool);
}
END_TEST

START_TEST (test_add_pop_cell)
{
	apr_pool_t* pool = NULL;
    apr_pool_create(&pool, NULL);
	cell* parent = sexpr_cell(pool);
	cell* childa = sym_cell(pool, "a");
	cell* childb = sym_cell(pool, "b");
	cell* childc = sym_cell(pool, "c");

	//we start with a childless parent
	ck_assert_ptr_eq(NULL, pop_cell(pool, parent, 0));
	ck_assert_ptr_eq(NULL, pop_cell(pool, parent, 1));
	ck_assert_int_eq(0, parent->count);

	//adding something increases the count
    add_cell(pool, parent, childa);
	ck_assert_int_eq(1, parent->count);

	//we popping off a cell that doesn't exist should return NULL and not change the count
	cell* popped1 = pop_cell(pool, parent, 1);
	ck_assert_int_eq(1, parent->count);
	ck_assert_ptr_eq(NULL, popped1);

	//now we pop off a cell that should exist
	cell* popped2 = pop_cell(pool, parent, 0);
	ck_assert_int_eq(0, parent->count);
	ck_assert_ptr_eq(childa, popped2);

	//now the parent should be empty
	ck_assert_ptr_eq(NULL, pop_cell(pool, parent, 0));

	add_cell(pool, parent, childb);
	add_cell(pool, parent, childa);
	ck_assert_ptr_eq(NULL, pop_cell(pool, parent, 2));
	ck_assert_ptr_eq(childb, pop_cell(pool, parent, 0));
    ck_assert_ptr_eq(childa, pop_cell(pool, parent, 0));
	ck_assert_ptr_eq(NULL, pop_cell(pool, parent, 0));
	
    add_cell(pool, parent, childc);
	add_cell(pool, parent, childb);
    add_cell(pool, parent, childa);
	ck_assert_int_eq(3, parent->count);

	ck_assert_ptr_eq(NULL, pop_cell(pool, parent, 3));
    ck_assert_ptr_eq(childb, pop_cell(pool, parent, 1));
    ck_assert_ptr_eq(childc, pop_cell(pool, parent, 0));
	ck_assert_ptr_eq(childa, pop_cell(pool, parent, 0));
	ck_assert_ptr_eq(NULL, pop_cell(pool, parent, 0));	

	apr_pool_destroy(pool);
}
END_TEST

START_TEST (test_copy_cell)
{
	apr_pool_t* pool = NULL;
    apr_pool_create(&pool, NULL);
	cell* parent = sexpr_cell(pool);
    cell* childa = sym_cell(pool, "a");
    cell* childb = sym_cell(pool, "b");
    cell* childc = sym_cell(pool, "c");	
	
	add_cell(pool, parent, childc);
    add_cell(pool, parent, childb);
    add_cell(pool, parent, childa);

	cell* clone = copy_cell(pool, parent);
	ck_assert_int_eq(3, clone->count);
	cell* clonec = pop_cell(pool, clone, 0);
	ck_assert_ptr_ne(clonec, childc);
	ck_assert_str_eq(clonec->sym, childc->sym);
	cell* cloneb = pop_cell(pool, clone, 0);
	ck_assert_ptr_ne(cloneb, childb);
	ck_assert_str_eq(cloneb->sym, childb->sym);
	cell* clonea = pop_cell(pool, clone, 0);
	ck_assert_ptr_ne(clonea, childa);
	ck_assert_str_eq(clonea->sym, childa->sym);
	ck_assert_int_eq(0, clone->count);	

    apr_pool_destroy(pool);
}
END_TEST

Suite* cell_suite(void)
{
	Suite *s = suite_create("Cell");
	TCase *test_case = tcase_create("Cell Operations");
	tcase_add_test(test_case, test_join_cell);
	tcase_add_test(test_case, test_add_pop_cell);
	tcase_add_test(test_case, test_copy_cell);
	suite_add_tcase(s, test_case);

	return s;
}
