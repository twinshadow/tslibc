#include "twinshadow/map.h"

struct ts_map_s *
ts_map_new(size_t logsize) {
	struct ts_map_s *head;
	size_t count;

	count = 1 << logsize;
	if (count < 1)
		return NULL;

	head = calloc(1, sizeof(struct ts_map_s));
	TS_ERR_NULL(head);

	head->map = calloc(count, sizeof(void*));
	TS_ERR_NULL(head->map);

	head->logsize = logsize;
	head->len = count;
	head->hash = ts_hash_oat;
	return (head);

error:
	if (head)
		free(head);
	return NULL;
}

void
ts_map_free(struct ts_map_s **head) {
	struct ts_map_item_s **item;
	struct ts_map_item_s *swap = NULL;
	size_t idx, count;

	TS_ERR_NULL(head);
	TS_ERR_NULL(*head);

	if ((*head)->map) {
		count = 1 << (*head)->logsize;
		REPEAT(idx, count) {
			item = &(*head)->map[idx];
			while (*item != NULL) {
				swap = *item;
				*item = (*item)->next;
				free(swap);
			}
		}
		free((*head)->map);
	}

	free(*head);
	*head = NULL;

error:
	return;
}

void
ts_map_resize(struct ts_map_s *head, size_t logsize) {
	size_t count;
	struct ts_map_item_s **buf;
	if (head == NULL)
		return;
	if (head->logsize >= logsize)
		return;
	count = 1 << logsize;
	if (count < 1)
		return;
	buf = realloc(head->map, count);
	TS_ERR_NULL(buf);
	head->map = buf;
	head->logsize = logsize;
	head->len = count;
	return;
error:
	return;
}

struct ts_map_item_s **
ts_map_lookup(void *ptr, size_t len, struct ts_map_s *head) {
	ts_map_key_t key;
	struct ts_map_item_s **item;

	TS_ERR_NULL(head);
	TS_ERR_NULL(head->map);

	key = head->hash(ptr, len);
	item = &head->map[key % head->len];
	if (*item == NULL)
		return (item);
	if (key == (*item)->hash)
		return (item);
	for (item = &((*item)->next);
	     *item != NULL;
	     item = &((*item)->next)) {
		if (key == (*item)->hash)
			return (item);
	}
	return (item);

error:
	return NULL;
}

struct ts_map_item_s *
ts_map_add(void *ptr, size_t len, struct ts_map_s *head) {
	struct ts_map_item_s **item;
	item = ts_map_lookup(ptr, len, head);
	if (*item != NULL)
		return (NULL);
	*item = calloc(1, sizeof(struct ts_map_item_s));
	TS_ERR_NULL(*item);
	(*item)->hash = head->hash(ptr, len);
	(*item)->key = ptr;
	head->count++;
	return (*item);

error:
	return NULL;
}

void
ts_map_rem(void *ptr, size_t len, struct ts_map_s *head) {
	struct ts_map_item_s **item, *next;

	TS_ERR_NULL(head)
	item = ts_map_lookup(ptr, len, head);
	if (*item == NULL)
		return;

	next = (*item)->next;
	free(*item);
	head->count--;
	*item = next;

error:
	return;
}
