#include <stdlib.h>
#include <stdio.h>

#include "twinshadow/std.h"
#include "twinshadow/macro.h"
#include "twinshadow/error.h"

ts_return_t
insert_sort(
    void *array,
    size_t count,
    size_t size,
    int (*compare)(void *a, void *b)) {
	size_t idx, sub;
	void *a, *b;
	void *swap;

	TS_ERR_NULL(array);
	TS_ERR_NULL(compare);
	TS_ERR_ZERO(count);
	TS_ERR_ZERO(size);

	swap = calloc(1, size);
	TS_ERR_NULL(swap);

	REPEAT(idx, count) {
		for (sub = idx;
		    sub > 0 &&
		        compare(PTR_OFFSET(array, sub, size),
				PTR_OFFSET(array, sub - 1, size)) < 0;
		    sub--) {
			a = PTR_OFFSET(array, sub, size);
			b = PTR_OFFSET(array, sub - 1, size);
			MEMSWAP(a, b, swap, size);
		}
	}

	if (swap)
		free(swap);
	return (TS_RETURN_OK);
error:
	return (TS_RETURN_ERROR);
}
