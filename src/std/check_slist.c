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
#include "twinshadow/slist.h"

struct ts_slist_s *buf_slist = NULL;
struct ts_snode_s *buf_snode = NULL;
char *snode_string_list[] = {
	"One",
	"Two",
	"Three",
	"Four",
	NULL
};
char **snode_string_buf = NULL;

START_TEST(test_slist_frees_when_populated) {
	ts_slist_push(buf_slist, NULL);
	/* teardown does the free */
} END_TEST

START_TEST(test_slist_pop_when_empty_returns_null) {
	ck_assert_ptr_eq(ts_slist_pop(buf_slist), NULL);
} END_TEST

START_TEST(test_slist_pushes) {
	while (*snode_string_buf)
		ck_assert_ptr_ne(ts_slist_push(buf_slist, *snode_string_buf++), NULL);
	ck_assert_int_eq(buf_slist->count, 4);
} END_TEST

START_TEST(test_slist_pops) {
	while (*snode_string_buf)
		ts_slist_unshift(buf_slist, *snode_string_buf++);
	snode_string_buf = snode_string_list;
	while (*snode_string_buf++)
		ck_assert_ptr_ne(ts_slist_pop(buf_slist), NULL);
	ck_assert_int_eq(buf_slist->count, 0);
} END_TEST

START_TEST(test_slist_unshifts) {
	while (*snode_string_buf)
		ck_assert_ptr_ne(ts_slist_unshift(buf_slist, *snode_string_buf++), NULL);
} END_TEST

START_TEST(test_slist_does_not_free_when_empty)
{
	/* this will segfault otherwise! */
	return;
}
END_TEST

START_TEST(test_slist_reverse) {
	while (*snode_string_buf)
		ts_slist_push(buf_slist, *snode_string_buf++);
	ts_slist_reverse(buf_slist);
	snode_string_buf = snode_string_list;
	while (*snode_string_buf)
		ck_assert_str_eq(ts_slist_pop(buf_slist), *snode_string_buf++);
} END_TEST

void
setup_slist_test(void) {
	buf_slist = ts_slist_new();
	buf_snode = NULL;
	snode_string_buf = snode_string_list;
}

void
teardown_slist_test(void) {
	ts_slist_free(buf_slist);
}

TCase *
tcase_slist(void) {
	TCase *tc = tcase_create("slist");
	tcase_add_checked_fixture(tc, setup_slist_test, teardown_slist_test);
	tcase_add_test(tc, test_slist_does_not_free_when_empty);
	tcase_add_test(tc, test_slist_unshifts);
	tcase_add_test(tc, test_slist_pops);
	tcase_add_test(tc, test_slist_pushes);
	tcase_add_test(tc, test_slist_pop_when_empty_returns_null);
	tcase_add_test(tc, test_slist_frees_when_populated);
	tcase_add_test(tc, test_slist_reverse);
	return tc;
}

CHECK_MAIN_STANDALONE(slist);
