#include "twinshadow/array.h"

int
ts_array_init(struct ts_array_s *head, size_t count, size_t size) {
	TS_ERR_NULL(head);
	TS_ERR_ZERO(count);
	TS_ERR_ZERO(size);
	head->head = calloc(count, size);
	TS_ERR_NULL(head->head);
	head->tail = PTR_OFFSET(head->head, count - 1, size);
	head->size = size;
	return (0);
error:
	return (1);
}

ts_return_t
ts_array_unset(struct ts_array_s *head) {
	TS_ERR_NULL(head);
	head->head = NULL;
	head->tail = NULL;
	head->size = 0;
	return TS_RETURN_OK;
error:
	return TS_RETURN_ERROR;
}

struct ts_array_s *
ts_array_new(size_t count, size_t size) {
	struct ts_array_s *head;

	head = calloc(1, sizeof(struct ts_array_s));
	TS_ERR_NULL(head);

	if (ts_array_init(head, count, size))
		goto error;

	return (head);
error:
	if (head)
		free(head);
	return (NULL);
}

void
ts_array_free(struct ts_array_s **head) {
	if (*head == NULL)
		return;

	if ((*head)->head)
		free((*head)->head);
	ts_array_unset(*head);

	free(*head);
	*head = NULL;
}

void
ts_array_resize(struct ts_array_s *head, size_t count, size_t size) {
	size_t oldcount;
	void *map;
	size_t idx;

	TS_ERR_ARRAY_IS_VALID(head);
	oldcount = PTR_COUNT(head->tail, head->head, head->size) + 1;

	if (!count)
		count = oldcount;

	if (!size)
		size = head->size;

	if (size == head->size && count == oldcount)
		return;

	map = realloc(head->head, count * size);

	TS_ERR_NULL(map);

	if (size != head->size) {
		REPEAT(idx, oldcount)
			memcpy(map + (idx * size), PTR_OFFSET(head->head, idx, head->size), head->size);
	}
	head->head = map;

	if (count != oldcount) {
		if (oldcount < count && size == head->size) {
			/* clear out unused memory from realloc */
			memset(PTR_OFFSET(head->head, oldcount, size), '\0',
			    count - oldcount);
		}
	}
	head->tail = PTR_OFFSET(head->head, count - 1, size);

error:
	return;
}

void *
ts_array_get(struct ts_array_s *head, size_t offset) {
	return PTR_OFFSET(head->head, offset, head->size);
}
