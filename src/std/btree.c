#include "twinshadow/btree.h"

/* node funcs */

struct ts_bnode_s *
ts_bnode_new(void *data) {
	struct ts_bnode_s *node;
	node = calloc(1, sizeof(struct ts_bnode_s));
	TS_ERR_NULL(node);
	node->key = data;
	return node;
error:
	return NULL;
}

void
ts_bnode_free(struct ts_bnode_s *node) {
	TS_ERR_NULL(node);
	free(node);
error:
	return;
}

struct ts_bnode_s *
ts_bnode_walk_r(struct ts_bnode_s *node, ts_bwalk_strategy_t strategy, void **queue) {
	struct ts_slist_s *row = NULL;
	struct ts_slist_s *qlist;
	TS_ERR_NULL(node);
	TS_ERR_NULL(queue);
	if (*queue == NULL) {
		*queue = ts_slist_new();
	}
	qlist = *queue;

	switch (strategy) {
		case TS_BWALK_PREORDER:
			if (qlist->count > 0)
				node = ts_slist_pop(qlist);
			else
				ts_slist_push(qlist, NULL);

			if (node == NULL)
				goto error;

			if (node->gt)
				ts_slist_push(qlist, node->gt);
			if (node->lt)
				ts_slist_push(qlist, node->lt);
			return (node);
		case TS_BWALK_BREADTHFIRST:
			if (qlist->count > 0) {
				row = ts_slist_peek_head(qlist);
				node = ts_slist_pop(row);
			}
			else {
				row = ts_slist_push(qlist, ts_slist_new());
			}

			if (node == NULL)
				goto error;

			if (node->lt)
				ts_slist_unshift(row, node->lt);
			if (node->gt)
				ts_slist_unshift(row, node->gt);
			return (node);
		case TS_BWALK_INORDER:
			if (qlist->count > 0) {
				row = ts_slist_peek_tail(qlist);
				if (qlist->count == 1) {
					node = ts_slist_pop(row);
					if (node)
						return(node);
				}
				node = ts_slist_pop(qlist);
			}
			else {
				row = ts_slist_push(qlist, ts_slist_new());
			}

			if (node == NULL)
				goto error;

			while (node->lt) {
				ts_slist_push(qlist, node);
				node = node->lt;
			}
			ts_slist_unshift(row, node);
			while (qlist->count > 1) {
				node = ts_slist_pop(qlist);
				ts_slist_unshift(row, node);
				if (node->gt) {
					ts_slist_push(qlist, node->gt);
					node = ts_slist_pop(row);
					return (node);
				}
			}
			return (ts_slist_pop(row));
		case TS_BWALK_POSTORDER:
			if (qlist->count > 0) {
				node = ts_slist_pop(qlist);
				if (node)
					return (node);
			}
			else {
				row = ts_slist_push(qlist, ts_slist_new());
				ts_slist_push(qlist, NULL);
			}

			if (node == NULL)
				goto error;
ts_bwalk_postorder:
			ts_slist_push(qlist, node);
			if (node->gt) {
				ts_slist_unshift(row, node->gt);
			}
			if (node->lt) {
				ts_slist_unshift(row, node->lt);
			}
			node = ts_slist_pop(row);
			if (node == NULL)
				return ts_slist_pop(qlist);
			goto ts_bwalk_postorder;
	}
error:
	if (queue && *queue)
		ts_slist_free(*queue);
	return (NULL);
}

struct ts_bnode_s **
ts_bnode_lookup(struct ts_bnode_s **node,
              void *data,
	      ts_btree_compare_t (*compare)(void *a, void *b),
	      struct ts_bnode_s **prev) {
	ts_btree_compare_t result;
	struct ts_bnode_s *current = NULL;
	struct ts_bnode_s *holder = NULL;
	TS_ERR_NULL(node);
	TS_ERR_NULL(*node);
	TS_ERR_NULL(data);
	TS_ERR_NULL(compare);
	if (prev == NULL)
		prev = &holder;
	current = *node;
	while (current) {
		result = compare(data, current->key);
		if (result & (TS_BTREE_COMPARE_GT + TS_BTREE_COMPARE_LT))
			holder = current;
			if (prev)
				*prev = holder;
		switch(result) {
			case TS_BTREE_COMPARE_GT:
				current = current->gt;
				break;
			case TS_BTREE_COMPARE_LT:
				current = current->lt;
				break;
			case TS_BTREE_COMPARE_EQ:
				return (node);
			default:
				goto error;
		}
	}
	if (result == TS_BTREE_COMPARE_GT)
		return (&holder->gt);
	else if (result == TS_BTREE_COMPARE_LT)
		return (&holder->lt);
error:
	return (NULL);
}

/* TODO: This function does not denote if a value or was added or a dupe was found */
struct ts_bnode_s *
ts_bnode_operate(struct ts_bnode_s **node,
                 void *data,
                 ts_btree_compare_t (*compare)(void *a, void *b),
                 ts_bop_t operation) {
	struct ts_bnode_s *prev;
	void *val = NULL;
	struct ts_bnode_s *checkthisout = NULL;
	struct ts_bnode_s **dingodestroyer = NULL;
	TS_ERR_NULL(node);
	TS_ERR_NULL(*node);
	TS_ERR_NULL(data);
	TS_ERR_NULL(compare);
	val = ((operation & TS_BOP_APPEND)
	      ? ((struct ts_bnode_s*)data)->key
	      : data);
	dingodestroyer = ts_bnode_lookup(node, val, compare, &prev);
	if (dingodestroyer && *dingodestroyer)
		checkthisout = *dingodestroyer;
	if (checkthisout) {
		if (operation & TS_BOP_REMOVE) {
			if (prev == NULL) {
				if (checkthisout->lt == NULL && checkthisout->gt == NULL)
					*node = NULL;
				else if (checkthisout->lt == NULL && checkthisout->gt)
					*node = checkthisout->gt;
				else if (checkthisout->gt == NULL && checkthisout->lt)
					*node = checkthisout->lt;
				else {
					*node = checkthisout->lt;
					ts_bnode_operate(node, (*node)->gt, compare, TS_BOP_INSERT + TS_BOP_APPEND);
				}
			}
			else if (prev->gt == checkthisout) {
				if (checkthisout->lt == NULL && checkthisout->gt == NULL)
					prev->gt = NULL;
				else if (checkthisout->lt == NULL && checkthisout->gt)
					prev->gt = checkthisout->gt;
				else if (checkthisout->gt == NULL && checkthisout->lt)
					prev->gt = checkthisout->lt;
				else {
					prev->gt = checkthisout->lt;
					ts_bnode_operate(&prev, checkthisout->gt, compare, TS_BOP_INSERT + TS_BOP_APPEND);
				}
			}
			else {
				if (checkthisout->lt == NULL && checkthisout->gt == NULL)
					prev->lt = NULL;
				else if (checkthisout->lt == NULL && checkthisout->gt)
					prev->lt = checkthisout->gt;
				else if (checkthisout->gt == NULL && checkthisout->lt)
					prev->lt = checkthisout->lt;
				else {
					prev->lt = checkthisout->gt;
					ts_bnode_operate(&prev, checkthisout->lt, compare, TS_BOP_INSERT + TS_BOP_APPEND);
				}
			}
			checkthisout->lt = NULL;
			checkthisout->gt = NULL;
		}
		return (checkthisout);
	}
	else {
		*dingodestroyer = ((operation & TS_BOP_APPEND)
		    ? data
		    : ts_bnode_new(data));
		return (*dingodestroyer);
	}
error:
	return (NULL);
}

/*
 ____ _____
| __ )_   _| __ ___  ___
|  _ \ | || '__/ _ \/ _ \
| |_) || || | |  __/  __/
|____/ |_||_|  \___|\___|

*/

struct ts_btree_s *
ts_btree_new(void) {
	struct ts_btree_s *tree;
	tree = calloc(1, sizeof(struct ts_btree_s));
	TS_ERR_NULL(tree);
	return tree;

error:
	return NULL;
}

void
ts_btree_free(struct ts_btree_s *tree) {
	TS_ERR_NULL(tree);
	TS_ERR_NULL(tree->head);

	if (tree->head->lt)
		ts_bnode_free(tree->head->lt);
	if (tree->head->gt)
		ts_bnode_free(tree->head->gt);
error:
	return;
}

int
ts_btree_add(struct ts_btree_s *tree,
            void *data) {
	TS_ERR_NULL(tree);
	if (tree->head == NULL) {
		tree->head = ts_bnode_new(data);
		tree->count++;
		return (0);
	}
	if (ts_bnode_operate(&tree->head, data, tree->compare, TS_BOP_INSERT)) {
		tree->count++;
		return (0);
	}
error:
	return -1;
}

int
ts_btree_remove(struct ts_btree_s *tree,
		void *data) {
	TS_ERR_NULL(tree);
	if (tree->head == NULL)
		goto error;
	if (ts_bnode_operate(&tree->head, data, tree->compare, TS_BOP_REMOVE)) {
		tree->count--;
		return (0);
	}
error:
	return -1;
}

int
ts_btree_exists(struct ts_btree_s *tree,
		void *data) {
	TS_ERR_NULL(tree);
	if (tree->head == NULL)
		goto error;
	if (*ts_bnode_lookup(&tree->head, data, tree->compare, NULL))
		return (0);
error:
	return (-1);
}

void *
ts_btree_walk_preorder_r(struct ts_btree_s *tree, void **ptr) {
	struct ts_bnode_s *node;
	node = ts_bnode_walk_r(tree->head, TS_BWALK_PREORDER, ptr);
	if (node)
		return (node->key);
	return (NULL);
}

void *
ts_btree_walk_preorder(struct ts_btree_s *tree) {
	static void *ptr = NULL;
	return ts_btree_walk_preorder_r(tree, &ptr);
}

void *
ts_btree_walk_postorder_r(struct ts_btree_s *tree, void **ptr) {
	struct ts_bnode_s *node;
	node = ts_bnode_walk_r(tree->head, TS_BWALK_POSTORDER, ptr);
	if (node)
		return (node->key);
	return (NULL);
}

void *
ts_btree_walk_postorder(struct ts_btree_s *tree) {
	static void *ptr = NULL;
	return ts_btree_walk_postorder_r(tree, &ptr);
}

void *
ts_btree_walk_inorder_r(struct ts_btree_s *tree, void **ptr) {
	struct ts_bnode_s *node;
	node = ts_bnode_walk_r(tree->head, TS_BWALK_INORDER, ptr);
	if (node)
		return (node->key);
	return (NULL);
}

void *
ts_btree_walk_inorder(struct ts_btree_s *tree) {
	static void *ptr = NULL;
	return ts_btree_walk_inorder_r(tree, &ptr);
}

void *
ts_btree_walk_breadthfirst_r(struct ts_btree_s *tree, void **ptr) {
	struct ts_bnode_s *node;
	node = ts_bnode_walk_r(tree->head, TS_BWALK_BREADTHFIRST, ptr);
	if (node)
		return (node->key);
	return (NULL);
}

void *
ts_btree_walk_breadthfirst(struct ts_btree_s *tree) {
	static void *ptr = NULL;
	return ts_btree_walk_breadthfirst_r(tree, &ptr);
}
