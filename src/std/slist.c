#include "twinshadow/slist.h"

struct ts_snode_s *
ts_snode_new(void *data) {
	struct ts_snode_s *node;
	node = calloc(1, sizeof(struct ts_snode_s));
	TS_ERR_NULL(node);
	node->key = data;
	return (node);
error:
	return (NULL);
}

void
ts_snode_free(struct ts_snode_s *node) {
	TS_ERR_NULL(node);
	memset(node, '\0', sizeof(struct ts_snode_s));
	free(node);
error:
	return;
}

struct ts_snode_s *
ts_snode_operation(struct ts_snode_s **node, struct ts_snode_s *link, ts_snode_op_t operation)
{
	TS_ERR_NULL(node);
	if (operation & (TS_SNODE_OP_PUSH + TS_SNODE_OP_UNSHIFT))
		TS_ERR_NULL(link);
	if (operation & TS_SNODE_OP_PUSH) {
		link->next = *node;
		*node = link;
	}
	else if (operation & TS_SNODE_OP_UNSHIFT) {
		if (*node) {
			link->next = (*node)->next;
			(*node)->next = link;
		}
		*node = link;
	}
	else if (operation & TS_SNODE_OP_POP) {
		link = *node;
		if (link) {
			*node = link->next;
			link->next = NULL;
		}
	}
	else
		goto error;
	return (link);
error:
	return (NULL);
}

/*
 _               _ _     _
| |_ ___     ___| (_)___| |_
| __/ __|   / __| | / __| __|
| |_\__ \   \__ \ | \__ \ |_
 \__|___/___|___/_|_|___/\__|
       |_____|
*/

struct ts_slist_s *
ts_slist_new(void) {
	struct ts_slist_s *list;
	list = calloc(1, sizeof(struct ts_slist_s));
	TS_ERR_NULL(list);
	return (list);
error:
	return (NULL);
}

void
ts_slist_free(struct ts_slist_s *list) {
	struct ts_snode_s *node = NULL;
	TS_ERR_NULL(list);
	while ((node = ts_snode_operation(&list->head, NULL, TS_SNODE_OP_POP)) != NULL)
		ts_snode_free(node);
	free(list->head);
	memset(list, '\0', sizeof(struct ts_slist_s));
	free(list);
error:
	return;
}

void *
ts_slist_push(struct ts_slist_s *list, void *data) {
	TS_ERR_NULL(list);
	ts_snode_operation(&list->head, ts_snode_new(data), TS_SNODE_OP_PUSH);
	if (list->tail == NULL)
		list->tail = list->head;;
	list->count++;
	return (ts_slist_peek_head(list));
error:
	return (NULL);
}

void *
ts_slist_pop(struct ts_slist_s *list) {
	struct ts_snode_s *buf;
	void *data;
	TS_ERR_NULL(list);
	buf = ts_snode_operation(&list->head, NULL, TS_SNODE_OP_POP);
	if (buf) {
		list->count--;
		data = buf->key;
		ts_snode_free(buf);
		return (data);
	}
	else
		list->tail = NULL;
error:
	return (NULL);
}

void *
ts_slist_unshift(struct ts_slist_s *list, void *data) {
	TS_ERR_NULL(list);
	if (list->head == NULL || list->tail == NULL)
		return ts_slist_push(list, data);
	ts_snode_operation(&list->tail, ts_snode_new(data), TS_SNODE_OP_UNSHIFT);
	list->count++;
	return (ts_slist_peek_tail(list));
error:
	return (NULL);
}

void *
ts_slist_peek_head(struct ts_slist_s *list) {
	if (list->head)
		return (list->head->key);
	return (NULL);
}

void *
ts_slist_peek_tail(struct ts_slist_s *list) {
	if (list->tail)
		return (list->tail->key);
	return (NULL);
}

void
ts_slist_reverse(struct ts_slist_s *list) {
	struct ts_snode_s *buf, *next;
	TS_ERR_NULL(list);
	TS_ERR_NULL(list->head);
	buf = ts_snode_operation(&list->head, NULL, TS_SNODE_OP_POP);
	list->tail = buf;
	while ((next = ts_snode_operation(&list->head, NULL, TS_SNODE_OP_POP)) != NULL)
		ts_snode_operation(&buf, next, TS_SNODE_OP_PUSH);
	list->head = buf;
error:
	return;
}
