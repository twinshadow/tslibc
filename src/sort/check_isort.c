#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

#include "check/twinshadow.h"
#include "twinshadow/std.h"

int *buf_isort = NULL;

/*****************************************************************************/

int*
random_int_list(int count)
{
	int *buf;
	int iter;
	struct timeval time;

	if (count < 1)
		return NULL;

	buf = calloc(count, sizeof(int));
	if (buf == NULL)
		return NULL;

	gettimeofday(&time, NULL);
	srand((unsigned long)time.tv_usec);
	for (iter = 0; iter < count; iter++)
	{
		buf[iter] = rand() % 20;
	}

	return buf;
}

void
print_int_list(int count, int *list)
{
	int iter;
	for (iter = 0; iter < count; iter++)
	{
		printf("%02d ", list[iter]);
	}
	puts("");
}

int
issorted(int count, int *list)
{
	int iter;
	for (iter = 1; iter < count; iter++)
		if (list[iter] < list[iter - 1])
			return 0;
	return 1;
}

int
compare_int(void *a, void *b) {
	if (*(int*)a == *(int*)b)
		return 0;
	else if (*(int*)a < *(int*)b)
		return -1;
	else
		return 1;
}

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
