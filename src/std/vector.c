#include "twinshadow/vector.h"

struct ts_vector_s *
ts_vector_new(size_t count) {
	struct ts_vector_s *head;

	head = calloc(1, sizeof(struct ts_vector_s));
	TS_ERR_NULL(head);

	head->array = ts_array_new(count);
	TS_ERR_NULL(head->array);

	head->cursor = head->array->head;
	head->count = 0;
	head->len = count;

	return head;

error:
	if (head)
		free(head);
	return NULL;
}

void
ts_vector_free(struct ts_vector_s *head) {
	if (head == NULL)
		return;

	if (head->array) {
		ts_array_free(head->array);
		head->cursor = NULL;
		head->count = 0;
		head->len = 0;
	}

	free(head);
	head = NULL;
}

void
ts_vector_resize(struct ts_vector_s *head, size_t count) {
	TS_ERR_NULL(head);
	TS_ERR_NULL(head->array);
	if (count < 1 || head->len > count)
		return;
	ts_array_resize(head->array, count);
	TS_ERR_NULL(head->array);

error:
	return;
}

/* ACC XXX: totally broken due to cognitive dissonance */
void
ts_vector_operate(struct ts_vector_s *head, int oper, size_t idx) {
	void **ptr = NULL;

	TS_ERR_NULL(head);
	TS_CHECK_DEBUG(head->count > 0, "No items to remove");
	TS_CHECK_DEBUG(idx < head->len, "Index outside of range");

	if (TS_VECTOR_OPER_SET & oper
	    && head->count + 1 >= head->len) {
		/* resize the vector */
	}

	/*
	switch (oper) {
		case TS_VECTOR_OPER_POP:
			ptr = head->cursor;
			head->cursor--;
			break;
		case TS_VECTOR_OPER_PUSH:
			++head->cursor;
			break;
		case TS_VECTOR_OPER_SHIFT:
			ptr = head->array->head;
			memmove(head->array->head + 1,
			    head->array->head,
			    (head->count - 1) * sizeof(void**));
			break;
		case TS_VECTOR_OPER_UNSHIFT:
			memmove(head->array->head,
			    head->array->head + 1,
			    head->count * sizeof(void**));
			head->array->head = ptr;
			break;
		case TS_VECTOR_OPER_REMOVE:
			ptr = &head->array->head[idx];
			memmove(head->array->head[idx + 1],
			    head->array->head[idx],
			    (head->count - 1) * sizeof(void**));
			break;
		case TS_VECTOR_OPER_INSERT:
			memmove(&head->array->head[idx],
			    &head->array->head[idx + 1],
			    head->count * sizeof(void**));
			head->array->head[idx] = ptr;
			break;
		default:
			break;
	}
	*/

	if (TS_VECTOR_OPER_SET & oper)
		head->count++;
	else
		head->count--;

error:
	return;
}
