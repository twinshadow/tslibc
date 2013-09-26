#include "twinshadow/vector.h"

void
ts_vector_init(struct ts_vector_s *head, ts_vector_policy_t policy) {
	size_t count = 1;
	head->array = calloc(count, sizeof(void*));
	TS_ERR_NULL(head->array);

	head->head = head->array;
	head->tail = head->array;
	head->count = 0;
	head->length = count;
	head->policy = policy;
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
	head->policy = 0;
error:
	return;
}

struct ts_vector_s *
ts_vector_new(ts_vector_policy_t policy) {
	struct ts_vector_s *head;

	head = calloc(1, sizeof(struct ts_vector_s));
	TS_ERR_NULL(head);
	ts_vector_init(head, policy);
	return head;
error:
	if (head)
		free(head);
	return NULL;
}

void
ts_vector_free(struct ts_vector_s **head) {
	TS_ERR_NULL(head);
	TS_ERR_NULL(*head);

	ts_vector_unset(*head);

	free(*head);
	*head = NULL;
error:
	return;
}

void
ts_vector_resize(struct ts_vector_s *head,
    ts_vector_operation_t operation) {
	void **buf;
	size_t count,
	       head_idx,
	       tail_idx;
	TS_VECTOR_IS_VALID(head);
	if (head->policy & TS_VECTOR_POLICY_SNAKE &&
	    ~operation & TS_VECTOR_OP_SET)
		/* This vector never stops growing, it must be free'd */
		return;
	head_idx = PTR_COUNT(head->head, head->array, sizeof(void*)) - 1;
	tail_idx = head_idx + head->count;
	if (head->policy & TS_VECTOR_POLICY_IMMEDIATE)
		count = head->length + 1;
	if (head->policy & TS_VECTOR_POLICY_EAGER)
		NULL;
	buf = realloc(head->array, count * sizeof(void*));
error:
	return;
}

void *
ts_vector_operate(
    struct ts_vector_s *head,
    ts_vector_operation_t operation,
    void *data,
    int idx) {
	void **ptr = NULL;

	TS_ERR_NULL(head);
	if (operation & TS_VECTOR_OP_IDX) {
		TS_CHECK_DEBUG(idx > head->length, "Index outside of range");
		if (idx == 0) {
			operation = ((operation & TS_VECTOR_OP_SET)
			    ? TS_VECTOR_OP_UNSHIFT
			    : TS_VECTOR_OP_SHIFT);
		}
		else if (idx == (head->count - 1)) {
			operation = ((operation & TS_VECTOR_OP_SET)
			    ? TS_VECTOR_OP_PUSH
			    : TS_VECTOR_OP_POP);
		}
	}

	if (operation & TS_VECTOR_OP_SET) {
		ts_vector_resize(head, operation);
	}
	else {
		TS_CHECK_DEBUG(head->count < 1, "No items to remove");
	}

	switch (operation) {
		case TS_VECTOR_OP_POP:
			data = *head->tail;
			head->tail = NULL;
			head->tail--;
			break;
		case TS_VECTOR_OP_PUSH:
			head->tail++;
			*head->tail = data;
			break;
		case TS_VECTOR_OP_SHIFT:
			data = *head->head;
			head->head = NULL;
			head->head++;
			break;
		case TS_VECTOR_OP_UNSHIFT:
			head->head--;
			*head->head = data;
			break;
		case TS_VECTOR_OP_REMOVE:
			ptr = head->head + (idx * sizeof(void*));
			data = *ptr;
			memmove(ptr, ptr + sizeof(void*), (head->count - idx - 1) * sizeof(void*));
			*ptr = NULL;
			break;
		case TS_VECTOR_OP_INSERT:
			ptr = head->head + (idx * sizeof(void*));
			memmove(ptr + sizeof(void*), ptr, (head->count - idx - 1) * sizeof(void*));
			*ptr = data;
			break;
		default:
			goto error;
	}

	if (operation & TS_VECTOR_OP_SET) {
		head->count++;
	}
	else {
		ts_vector_resize(head, operation);
		head->count--;
	}
	return data;
error:
	return NULL;
}

void
ts_vector_push(struct ts_vector_s *head, void *data) {
	ts_vector_operate(head, TS_VECTOR_OP_PUSH, data, 0);
}

void
ts_vector_unshift(struct ts_vector_s *head, void *data) {
	ts_vector_operate(head, TS_VECTOR_OP_UNSHIFT, data, 0);
}

void
ts_vector_insert(struct ts_vector_s *head, void *data, int idx) {
	ts_vector_operate(head, TS_VECTOR_OP_INSERT, data, idx);
}

void *
ts_vector_pop(struct ts_vector_s *head) {
	return (ts_vector_operate(head, TS_VECTOR_OP_POP, NULL, 0));
}

void *
ts_vector_shift(struct ts_vector_s *head) {
	return (ts_vector_operate(head, TS_VECTOR_OP_SHIFT, NULL, 0));
}

void *
ts_vector_remove(struct ts_vector_s *head, int idx) {
	return (ts_vector_operate(head, TS_VECTOR_OP_REMOVE, NULL, idx));
}
