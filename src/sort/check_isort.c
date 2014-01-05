#include "check/twinshadow.h"
#include "twinshadow/std.h"
#include "check/sort.h"

int *buf_isort = NULL;

/*****************************************************************************/


/*****************************************************************************/

START_TEST(test_isort)
{
	print_int_list(5, buf_isort);
	insert_sort(buf_isort, 5, sizeof(int), compare_int);
	print_int_list(5, buf_isort);
	ck_assert(issorted(5, buf_isort) != 0);
}
END_TEST

void
setup_isort_test(void) {
	buf_isort = random_int_list(5);
}

void
teardown_isort_test(void) {
	free(buf_isort);
}

TCase *
tcase_isort(void) {
	TCase *tc = tcase_create("isort");
	tcase_add_checked_fixture(tc, setup_isort_test, teardown_isort_test);
	tcase_add_test(tc, test_isort);
	return tc;
}

CHECK_MAIN_STANDALONE(isort);
