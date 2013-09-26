#include "twinshadow/array.h"

ts_return_t
ts_array_init(struct ts_array_s *head, size_t count, size_t size) {
	TS_ERR_NULL(head);
	head->head = calloc(count, size);
	TS_ERR_NULL(head->head);
	head->tail = head->head + ((count - 1) * size);
	head->size = size;
	return TS_RETURN_OK;
error:
	return TS_RETURN_ERROR;
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
	oldcount = PTR_COUNT(head->tail, head->head, head->size);

	if (!count)
		count = oldcount;

	if (!size)
		size = head->size;

	if (size == head->size && count == oldcount)
		return;

	if (size != head->size) {
		map = calloc(count, size);
		REPEAT(idx, oldcount) {
			memcpy(map + (idx * size), head->head + (idx * head->size), head->size);
		}
	}
	else {
		map = realloc(head->head, count * size);
	}

	TS_ERR_NULL(map);
	if (head->head && head->head != map)
		free(head->head);
	head->head = map;

	if (count != oldcount) {
		head->tail = head->head + ((count - 1) * size);
		if (oldcount < count && size == head->size) {
			/* clear out unused memory from realloc */
			memset(head->head + (oldcount * size), '\0',
			    count - oldcount);
		}
	}
error:
	return;
}