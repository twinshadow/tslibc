#include "twinshadow/array.h"

int
ts_array_init(struct ts_array_s *head, size_t count, size_t size, void *data) {
	TS_ERR_NULL(head);
	TS_ERR_ZERO(count);
	TS_ERR_ZERO(size);
	if (data == NULL)
		head->head = calloc(count, size);
	else
		head->head = data;
	TS_ERR_NULL(head->head);
	head->size = size;
	head->tail = TS_ARRAY_TAIL(head, count);
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
	if (ts_array_init(head, count, size, NULL))
		goto error;

	return (head);
error:
	if (head)
		free(head);
	return (NULL);
}

void
ts_array_free(struct ts_array_s *head) {
	if (head == NULL)
		return;
	if (head->head)
		free(head->head);
	ts_array_unset(head);
	free(head);
}

void
ts_array_resize(struct ts_array_s *head, size_t count, size_t size) {
	size_t oldcount;
	void *map;
	size_t idx;

	TS_ERR_ARRAY_IS_VALID(head);
	oldcount = TS_ARRAY_COUNT(head);

	if (count == 0)
		count = oldcount;

	if (size == 0)
		size = head->size;

	if (size == head->size && count == oldcount)
		return;

	TS_CHECK(size >= head->size && count >= oldcount, "Shrinking arrays is not supported.")

	/* Shift the old data into the re-sized buckets */
	/* TODO: Handle shrinking data? Drop bits? */
	if (size > head->size && count >= oldcount) {
		map = calloc(count, size);
		TS_ERR_NULL(map);
		REPEAT(idx, oldcount) {
			memcpy(PTR_OFFSET(map, idx, size), PTR_OFFSET(head->head, idx, head->size), head->size);
		}
		head->size = size;
		free(head->head);
	}
	else {
		map = realloc(head->head, count * size);
		TS_ERR_NULL(map);
	}
	head->head = map;

	if (oldcount < count) {
		/* clear out junk memory from realloc */
		memset(PTR_OFFSET(head->head, oldcount, head->size), '\0',
		    count - oldcount);
	}

	head->tail = TS_ARRAY_TAIL(head, count);
	return;
error:
	if (map != NULL)
		free(map);
	return;
}

void *
ts_array_get(struct ts_array_s *head, size_t offset) {
	TS_ERR_NULL(head);
	return PTR_OFFSET(head->head, offset, head->size);
error:
	return (NULL);
}

struct ts_array_s*
ts_mem_to_array(void *ptr, size_t count, size_t size) {
	struct ts_array_s *head;
	void *buf;
	head = calloc(1, sizeof(struct ts_array_s));
	TS_ERR_NULL(head);
	buf = malloc(count * size);
	TS_ERR_NULL(buf);
	TS_ERR_NULL(memcpy(buf, ptr, count * size));
	TS_ERR_NONZERO(ts_array_init(head, count, size, buf));
	return (head);
error:
	if (head)
		ts_array_free(head);
	if (buf)
		free(buf);
	return (NULL);
}
