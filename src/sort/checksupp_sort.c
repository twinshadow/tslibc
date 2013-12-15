#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>
#include <check/sort.h>

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
