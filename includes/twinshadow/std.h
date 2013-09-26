#ifndef TWINSHADOW_STD_H
#define TWINSHADOW_STD_H

typedef int ts_return_t;
enum {
	TS_RETURN_OK,
	TS_RETURN_ERROR,
	TS_RETURN_FAIL
};

typedef char ts_compare_t;
enum {
	TS_COMPARE_LT = -1,
	TS_COMPARE_EQ = 0,
	TS_COMPARE_GT = 1
};

ts_return_t insert_sort(void *array, size_t count, size_t size, int (*compare)(void *a, void *b));

#endif /* TWINSHADOW_STD_H */
