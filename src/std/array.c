#include "twinshadow/array.h"

struct ts_array_s *
ts_array_new(size_t count) {
	struct ts_array_s *head;

	head = calloc(1, sizeof(struct ts_array_s));

	if (head == NULL) {
		return NULL;
	}

	if (head->head == NULL) {
		head->head = calloc(count, sizeof(void**));
		head->tail = &head->head[count - 1];
	}

	if (head->head == NULL) {
		free(head);
		return NULL;
	}

	return head;
}

void
ts_array_free(struct ts_array_s *head) {
	if (head == NULL)
		return;

	if (head->head) {
		free(head->head);
		head->head = NULL;
		head->tail = NULL;
	}

	free(head);
	head = NULL;
}

void
ts_array_resize(struct ts_array_s *head, size_t count) {
	size_t oldcount;
	oldcount = (head->tail - head->head);

	head->head = realloc(head->head, count * sizeof(void**));
	if (head->head) {
		head->tail = &head->head[count - 1];
		if (oldcount < count) {
			memset(&head->head[oldcount], '\0',
			    count - oldcount);
		}
	}
}
