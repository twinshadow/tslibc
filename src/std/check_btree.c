/* Copyright (c) 2013, Anthony Cornehl
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   - Redistributions of source code must retain the above copyright notice, this
 *     list of conditions and the following disclaimer.
 *
 *   - Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "check/twinshadow.h"
#include "twinshadow/btree.h"

struct ts_btree_s *buf_btree = NULL;
struct ts_bnode_s *buf_bnode = NULL;
/* A balanced tree of dogs */
char *btree_test_str[] = {
	"Dingo",
	"Canis",
	"Canine",
	"Cur",
	"Hyena",
	"Dog",
	"Mutt",
	NULL
};
int row_order[] = {0,1,4,2,3,5,6,-1};
int post_order[] = {2,3,5,6,1,4,0,-1};
int in_order[] = {2,1,3,0,5,4,6,-1};
char **btree_test_ptr = NULL;
int *int_ptr = NULL;

ts_btree_compare_t
btree_strcmp(void *a, void *b) {
	int cmp = strcmp(a, b);
	if (cmp < 0)
		return (TS_BTREE_COMPARE_LT);
	else if (cmp > 0)
		return (TS_BTREE_COMPARE_GT);
	return (TS_BTREE_COMPARE_EQ);
}

START_TEST(test_btree) {
	return;
} END_TEST

START_TEST(test_btree_add_inserts_a_value) {
	while (*btree_test_ptr)
		ts_btree_add(buf_btree, *btree_test_ptr++);
} END_TEST

START_TEST(test_btree_remove_removes_a_value) {
	while (*btree_test_ptr)
		ts_btree_add(buf_btree, *btree_test_ptr++);
	btree_test_ptr = btree_test_str;
	while (*btree_test_ptr)
		ts_btree_remove(buf_btree, *btree_test_ptr++);
	printf("%lu\n", buf_btree->count);
} END_TEST

START_TEST(test_btree_exists_finds_a_value) {
	while (*btree_test_ptr)
		ts_btree_add(buf_btree, *btree_test_ptr++);
	ck_assert(ts_btree_exists(buf_btree, "Cur") == 0);
} END_TEST

START_TEST(test_btree_exists_finds_no_value) {
	while (*btree_test_ptr)
		ts_btree_add(buf_btree, *btree_test_ptr++);
	ck_assert(ts_btree_exists(buf_btree, "Cats") != 0);
} END_TEST

START_TEST(test_btree_walk_preorder) {
	while (*btree_test_ptr)
		ts_btree_add(buf_btree, *btree_test_ptr++);
	btree_test_ptr = btree_test_str;
	while (*btree_test_ptr)
		ck_assert_str_eq(ts_btree_walk_preorder(buf_btree), *btree_test_ptr++);
} END_TEST

START_TEST(test_btree_walk_postorder) {
	while (*btree_test_ptr)
		ts_btree_add(buf_btree, *btree_test_ptr++);
	int_ptr = post_order;
	while (*int_ptr >= 0)
		ck_assert_str_eq(ts_btree_walk_postorder(buf_btree), btree_test_str[*int_ptr++]);
} END_TEST

START_TEST(test_btree_walk_inorder) {
	while (*btree_test_ptr)
		ts_btree_add(buf_btree, *btree_test_ptr++);
	int_ptr = in_order;
	while (*int_ptr >= 0)
		ck_assert_str_eq(ts_btree_walk_inorder(buf_btree), btree_test_str[*int_ptr++]);
} END_TEST

START_TEST(test_btree_walk_breadth_first) {
	while (*btree_test_ptr)
		ts_btree_add(buf_btree, *btree_test_ptr++);
	int_ptr = row_order;
	while (*int_ptr >= 0)
		ck_assert_str_eq(ts_btree_walk_breadthfirst(buf_btree), btree_test_str[*int_ptr++]);
} END_TEST

void
setup_btree_test(void) {
	buf_btree = ts_btree_new();
	buf_btree->compare = btree_strcmp;
	buf_bnode = NULL;
	btree_test_ptr = btree_test_str;
}

void
teardown_btree_test(void) {
	ts_btree_free(buf_btree);
}

TCase *
tcase_btree(void) {
	TCase *tc = tcase_create("btree");
	tcase_add_checked_fixture(tc, setup_btree_test, teardown_btree_test);
	tcase_add_test(tc, test_btree_walk_breadth_first);
	tcase_add_test(tc, test_btree_walk_inorder);
	tcase_add_test(tc, test_btree_walk_postorder);
	tcase_add_test(tc, test_btree_walk_preorder);
	tcase_add_test(tc, test_btree_exists_finds_no_value);
	tcase_add_test(tc, test_btree_exists_finds_a_value);
	tcase_add_test(tc, test_btree_remove_removes_a_value);
	tcase_add_test(tc, test_btree_add_inserts_a_value);
	tcase_add_test(tc, test_btree);
	return tc;
}

CHECK_MAIN_STANDALONE(btree);
