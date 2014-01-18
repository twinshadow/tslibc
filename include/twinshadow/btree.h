#ifndef TWINSHADOW_BTREE_H
#define TWINSHADOW_BTREE_H

#include <stdlib.h>

#include "twinshadow/error.h"
#include "twinshadow/macro.h"
#include "twinshadow/slist.h"

typedef char ts_btree_compare_t;
typedef char ts_bop_t;
typedef char ts_bwalk_strategy_t;

struct ts_bnode_s {
	void *key;
	struct ts_bnode_s *lt;
	struct ts_bnode_s *gt;
};

struct ts_btree_s {
	struct ts_bnode_s *head;
	size_t count;
	ts_btree_compare_t (*compare)(void *a, void *b);
};

enum {
	TS_BWALK_PREORDER,
	TS_BWALK_POSTORDER,
	TS_BWALK_INORDER,
	TS_BWALK_BREADTHFIRST
};

enum {
	TS_BOP_INSERT = 1<<0,
	TS_BOP_REMOVE = 1<<1,
	TS_BOP_RETRIEVE = 1<<2,
	TS_BOP_APPEND = 1<<3
};

enum {
	TS_BTREE_COMPARE_ERROR = 1<<0,
	TS_BTREE_COMPARE_LT = 1<<1,
	TS_BTREE_COMPARE_EQ = 1<<2,
	TS_BTREE_COMPARE_GT = 1<<3
};

struct ts_btree_s *ts_btree_new(void);
void ts_btree_free(struct ts_btree_s *tree);
int ts_btree_add(struct ts_btree_s *tree, void *data);
int ts_btree_remove(struct ts_btree_s *tree, void *data);
int ts_btree_exists(struct ts_btree_s *tree, void *data);
void *ts_btree_walk_preorder_r(struct ts_btree_s *tree, void **ptr);
void *ts_btree_walk_preorder(struct ts_btree_s *tree);
void *ts_btree_walk_postorder_r(struct ts_btree_s *tree, void **ptr);
void *ts_btree_walk_postorder(struct ts_btree_s *tree);
void *ts_btree_walk_inorder_r(struct ts_btree_s *tree, void **ptr);
void *ts_btree_walk_inorder(struct ts_btree_s *tree);
void *ts_btree_walk_breadthfirst_r(struct ts_btree_s *tree, void **ptr);
void *ts_btree_walk_breadthfirst(struct ts_btree_s *tree);

#endif /* TWINSHADOW_BTREE_H */
