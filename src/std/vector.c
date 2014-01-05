#include "twinshadow/vector.h"
#include <stdint.h>

void
ts_vector_init(struct ts_vector_s *head, size_t size) {
	TS_ERR_ZERO(size);
	head->size = size;
error:
	return;
}

void
ts_vector_unset(struct ts_vector_s *head) {
	TS_ERR_NULL(head);
	if (head->array)
		free(head->array);
	head->array = NULL;
	head->head = NULL;
	head->tail = NULL;
	head->count = 0;
	head->length = 0;
	head->size = 0;
	head->policy.flags = 0;
error:
	return;
}

int
ts_vector_init_data(struct ts_vector_s *head, size_t count, size_t size, void *data) {
	TS_ERR_NULL(head);
	TS_ERR_NULL(data);
	TS_ERR_ZERO(size);
	TS_ERR_ZERO(count);
	head->size = size;
	head->head = head->array = data;
	head->length = head->count = count;
	head->tail = head->count > 1 ? PTR_OFFSET(head->head, head->count - 1, head->size) : head->head;
	return (0);
error:
	ts_vector_unset(head);
	return (1);
}

struct ts_vector_s *
ts_vector_new(size_t size) {
	struct ts_vector_s *head;
	head = calloc(1, sizeof(struct ts_vector_s));
	TS_ERR_NULL(head);
	ts_vector_init(head, size);
	return head;
error:
	if (head)
		free(head);
	return NULL;
}

void
ts_vector_free(struct ts_vector_s *head) {
	TS_ERR_NULL(head);
	ts_vector_unset(head);
	free(head);
error:
	return;
}

int
ts_vector_resize(struct ts_vector_s *head) {
	void *buf;
	size_t head_offset = 0;
	if (head->head && head->array != head->array)
		head_offset = PTR_COUNT(head->head, head->array, head->size);
	buf = realloc(head->array, head->length * head->size);
	TS_ERR_NULL(buf);
	head->array = buf;
	head->head = (head_offset) ? PTR_OFFSET(head->array, head_offset, head->size) : buf;
	head->tail = PTR_OFFSET(head->head, head->count > 1 ? head->count - 1 : 0, head->size);
	return (0);
error:
	return (-1);
}

int
ts_vector_grow(struct ts_vector_s *head, size_t count) {
	TS_VECTOR_IS_VALID(head);
	head->length++;
	return (ts_vector_resize(head));
error:
	return (-1);
}

int
ts_vector_shrink(struct ts_vector_s *head, size_t count) {
	TS_VECTOR_IS_VALID(head);
	return (0);
error:
	return (-1);
}

int
ts_vector_operate(
    struct ts_vector_s *head,
    void *data,
    size_t count,
    ts_vector_operation_t operation,
    ts_vector_idx_t idx) {
	void *ptr = NULL;
	int result = 0;
	TS_ERR_NULL(head);

	switch (operation) {
		case TS_VECTOR_OP_REMOVE:
			TS_CHECK(head->count > 0, "No items to remove");
			TS_VECTOR_CHECK_IDX(head, idx);
			if(ts_vector_shrink(head, 1))
				goto error;
			ptr = TS_VECTOR_OFFSET(head, idx);
			/* copy-out if data-pointer is provided, otherwise it is overwritten */
			if (data != NULL)
				memcpy(data, ptr, head->size);
			if (idx != TS_VECTOR_IDX_TAIL)
				memmove(ptr, ptr + head->size, (head->count - idx - 1) * head->size);
			ptr = NULL;
			head->count--;
			head->tail -= head->size;
			break;

		case TS_VECTOR_OP_INSERT:
			TS_ERR_NULL(data);
			TS_VECTOR_CHECK_IDX(head, idx);
			if(ts_vector_grow(head, 1))
				goto error;
			ptr = TS_VECTOR_OFFSET(head, idx);
			if (head->count) {
				head->tail += head->size;
				if (idx != TS_VECTOR_IDX_TAIL)
					memmove(ptr + head->size, ptr, (head->count - idx) * head->size);
				if (idx == TS_VECTOR_IDX_TAIL)
					ptr = head->tail;
			}
			memcpy(ptr, data, head->size);
			head->count++;
			break;
		default:
			goto error;
	}

	goto out;
error:
	result = -1;
out:
	return (result);
}

// getters
void *
ts_vector_get(struct ts_vector_s *head, ts_vector_idx_t idx) {
	return TS_VECTOR_OFFSET(head, idx);
}

void *
ts_vector_find(struct ts_vector_s *head, void *data) {
	return (NULL);
}

// modifiers - insert
int
ts_vector_push(struct ts_vector_s *head, void *data) {
	return (ts_vector_operate(head, data, 1, TS_VECTOR_OP_INSERT, TS_VECTOR_IDX_TAIL));
}

int
ts_vector_unshift(struct ts_vector_s *head, void *data) {
	return ts_vector_operate(head, data, 1, TS_VECTOR_OP_INSERT, TS_VECTOR_IDX_HEAD);
}

int
ts_vector_insert(struct ts_vector_s *head, void *data, ts_vector_idx_t idx) {
	return ts_vector_operate(head, data, 1, TS_VECTOR_OP_INSERT, idx);
}

// modifiers - remove
int
ts_vector_pop(struct ts_vector_s *head, void *data) {
	return ts_vector_operate(head, data, 1, TS_VECTOR_OP_REMOVE, TS_VECTOR_IDX_TAIL);
}

int
ts_vector_shift(struct ts_vector_s *head, void *data) {
	return ts_vector_operate(head, data, 1, TS_VECTOR_OP_REMOVE, TS_VECTOR_IDX_HEAD);
}

int
ts_vector_remove(struct ts_vector_s *head, void* data, ts_vector_idx_t idx) {
	return ts_vector_operate(head, data, 1, TS_VECTOR_OP_REMOVE, idx);
}

// conversion
struct ts_array_s*
ts_vector_to_array(struct ts_vector_s *head) {
	struct ts_array_s *buf;
	TS_VECTOR_IS_POPULATED(head);
	buf = ts_mem_to_array(head->head, head->count, head->size);
	TS_ERR_NULL(buf);
	TS_ERR_NULL(buf->head);
	return (buf);
error:
	if (buf != NULL)
		free(buf);
	return (NULL);
}

struct ts_vector_s*
ts_array_to_vector(struct ts_array_s *head) {
	struct ts_vector_s *buf;
	TS_ERR_ARRAY_IS_VALID(head);
	buf = ts_mem_to_vector(head->head, TS_ARRAY_COUNT(head), head->size);
	TS_ERR_NULL(buf);
	return (buf);
error:
	if (buf != NULL)
		ts_vector_free(buf);
	return (NULL);
}

struct ts_vector_s*
ts_mem_to_vector(void *ptr, size_t count, size_t size) {
	struct ts_vector_s *head;
	void *data;
	TS_ERR_NULL(ptr);
	TS_ERR_ZERO(count);
	TS_ERR_ZERO(size);
	head = ts_vector_new(size);
	TS_ERR_NULL(head);
	data = malloc(count * size);
	TS_ERR_NULL(data);
	TS_ERR_NULL(memcpy(data, ptr, count * size));
	TS_ERR_NONZERO(ts_vector_init_data(head, count, size, data));
	return (head);
error:
	if (head != NULL)
		ts_vector_free(head);
	return (NULL);
}
