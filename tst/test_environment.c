#include <apr_pools.h>

#include "../src/cell.h"
#include "../src/environment.h"
#include "test_environment.h"

START_TEST (test_environment_new)
{
	apr_pool_t* pool = NULL;
	apr_pool_create(&pool, NULL);
	environment* env = environment_new(pool, NULL);
	environment* child = environment_new(pool, env);
	ck_assert_ptr_eq(child->parent, env);
	ck_assert_ptr_ne(env, NULL);		
	ck_assert_ptr_eq(env->parent, NULL);	

	apr_pool_destroy(pool);
}
END_TEST

START_TEST (test_environment_copy)
{
	apr_pool_t* pool = NULL;
    apr_pool_create(&pool, NULL);
	
	environment* env = environment_new(pool, NULL);
	cell* test = sym_cell(pool, "test");
	environment_put(env, "test", test);

	environment* clone = environment_copy(pool, env);
 
	ck_assert_ptr_ne(clone, NULL);	
	cell* cloned_test = environment_get(pool, env, "test");	
	ck_assert_ptr_ne(cloned_test, NULL);	
	ck_assert_str_eq(cloned_test->sym, "test");	

	apr_pool_destroy(pool);
}
END_TEST

START_TEST (test_environment_put_get)
{
	apr_pool_t* pool = NULL;
    apr_pool_create(&pool, NULL);

	environment* env = environment_new(pool, NULL);
	cell* a = sym_cell(pool, "a is for apple");
	environment_put(env, "a", a);
	cell* result = environment_get(pool, env, "a");
	ck_assert_ptr_ne(result, NULL);
	ck_assert_str_eq(result->sym, "a is for apple");
	
	cell* b = sexpr_cell(pool);
	cell* child1 = num_cell(pool, 1);
	cell* child3 = num_cell(pool, 3);
	cell* child2 = num_cell(pool, 2);

	add_cell(pool, b, child1);
	add_cell(pool, b, child3);
	add_cell(pool, b, child2);

	ck_assert_ptr_ne(b, NULL);
	ck_assert_int_eq(b->count, 3);
	
	environment_put(env, "b", b);
	cell* next_result = environment_get(pool, env, "b");
	ck_assert_ptr_ne(next_result, NULL);
	ck_assert_int_eq(next_result->type, SEXPR_CELL);
	ck_assert_int_eq(next_result->count, 3);	
	cell* result_child_1 = pop_cell(pool, next_result, 0);
	cell* result_child_2 = pop_cell(pool, next_result, 0);
	cell* result_child_3 = pop_cell(pool, next_result, 0);
	ck_assert_int_eq(result_child_1->num, 1);
	ck_assert_int_eq(result_child_2->num, 3);
	ck_assert_int_eq(result_child_3->num, 2);

    apr_pool_destroy(pool);
}
END_TEST

START_TEST (test_environment_global_define)
{

}
END_TEST

Suite* environment_suite(void)
{
	Suite *s = suite_create("Environment");
	TCase *test_case = tcase_create("Environment Operations");
	tcase_add_test(test_case, test_environment_new);
	tcase_add_test(test_case, test_environment_copy);
	tcase_add_test(test_case, test_environment_put_get);
	tcase_add_test(test_case, test_environment_global_define);
	suite_add_tcase(s, test_case);

	return s;
}
