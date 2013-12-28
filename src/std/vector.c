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
	void **buf;
	size_t head_offset = 0;
	int ret = 0;
	TS_VECTOR_IS_VALID(head);
	if (head->head)
		head_offset = PTR_COUNT(head->head, head->array, head->size);
	head->length++;
	buf = realloc(head->array, head->length * head->size);
	TS_ERR_NULL(buf);
	if (head->array == NULL) {
		head->head = buf;
		head->tail = buf;
	}
	head->array = buf;
	head->head = PTR_OFFSET(head->array, head_offset, head->size);
	head->tail = PTR_OFFSET(head->head, head->count - 1, head->size);
	goto out;
error:
	ret = 1;
out:
	return (ret);
}

int
ts_vector_operate(
    struct ts_vector_s *head,
    ts_vector_operation_t operation,
    void *data,
    ts_vector_idx_t idx) {
	void *ptr = NULL;
	int result = 0;

	TS_ERR_NULL(head);
	if (idx == TS_VECTOR_IDX_TAIL && head->count < 2)
		idx = 0;
	TS_CHECK(POSITIVE(idx) <= head->count, "Index outside of range");
	idx = (idx >= 0 ? idx : head->count + idx);

	switch (operation) {
		case TS_VECTOR_OP_REMOVE:
			TS_CHECK(head->count < 1, "No items to remove");
			ptr = TS_VECTOR_OFFSET(head, idx);
			/* copy-out if data-pointer is provided, otherwise it is overwritten */
			if (data != NULL)
				memmove(data, ptr, head->size);
			memmove(ptr, ptr + head->size, (head->count - idx - 1) * head->size);
			ptr = NULL;
			head->count--;
			head->tail -= head->size;
			break;

		case TS_VECTOR_OP_INSERT:
			TS_ERR_NULL(data);
			if(ts_vector_resize(head))
				goto error;
			ptr = TS_VECTOR_OFFSET(head, idx);
			if (head->count)
				memmove(ptr + head->size, ptr, (head->count - idx) * head->size);
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
	return (ts_vector_operate(head, TS_VECTOR_OP_INSERT, data, TS_VECTOR_IDX_TAIL));
}

int
ts_vector_unshift(struct ts_vector_s *head, void *data) {
	return ts_vector_operate(head, TS_VECTOR_OP_INSERT, data, TS_VECTOR_IDX_HEAD);
}

int
ts_vector_insert(struct ts_vector_s *head, void *data, ts_vector_idx_t idx) {
	return ts_vector_operate(head, TS_VECTOR_OP_INSERT, data, idx);
}

// modifiers - remove
int
ts_vector_pop(struct ts_vector_s *head, void *data) {
	return ts_vector_operate(head, TS_VECTOR_OP_REMOVE, data, TS_VECTOR_IDX_TAIL);
}

int
ts_vector_shift(struct ts_vector_s *head, void *data) {
	return ts_vector_operate(head, TS_VECTOR_OP_REMOVE, data, TS_VECTOR_IDX_HEAD);
}

int
ts_vector_remove(struct ts_vector_s *head, void* data, ts_vector_idx_t idx) {
	return ts_vector_operate(head, TS_VECTOR_OP_REMOVE, data, idx);
}
