#include <apr_pools.h>
#include <check.h>
#include <stdlib.h>

#include "test_cell.h"

int main(void)
{
	apr_initialize();
	apr_pool_initialize();
	int number_failed = 0;
	Suite *cell_tests = cell_suite();
	SRunner *cell_runner = srunner_create(cell_tests);
	srunner_run_all(cell_runner, CK_NORMAL);
	number_failed += srunner_ntests_failed(cell_runner);
	srunner_free(cell_runner);
	return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
	apr_terminate();
}
