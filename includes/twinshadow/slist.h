#ifndef TWINSHADOW_LIST_H
#define TWINSHADOW_LIST_H

#include <stdlib.h>

#include "twinshadow/error.h"

typedef char ts_snode_op_t;

struct ts_snode_s {
	void *key;
	struct ts_snode_s *next;
};

struct ts_slist_s {
	struct ts_snode_s *head;
	struct ts_snode_s *tail;
	size_t count;
};

enum {
	TS_SNODE_OP_PUSH = 1<<0,
	TS_SNODE_OP_POP = 1<<1,
	TS_SNODE_OP_SHIFT = 1<<2,
	TS_SNODE_OP_UNSHIFT = 1<<3,
	TS_SNODE_OP_APPEND = 1<<4
};

struct ts_slist_s *ts_slist_new(void);
void ts_slist_free(struct ts_slist_s *list);
void *ts_slist_push(struct ts_slist_s *list, void *data);
void *ts_slist_pop(struct ts_slist_s *list);
void *ts_slist_unshift(struct ts_slist_s *list, void *data);
void *ts_slist_peek_head(struct ts_slist_s *list);
void *ts_slist_peek_tail(struct ts_slist_s *list);
void ts_slist_reverse(struct ts_slist_s *list);

#endif /* TWINSHADOW_LIST_H */
