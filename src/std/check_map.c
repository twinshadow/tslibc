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
#include "twinshadow/map.h"

struct ts_map_s *buf_map = NULL;

START_TEST(test_map_add)
{
	struct ts_map_item_s **item;
	char *test;

	test = "Hello World!";

	ck_assert(ts_map_add(test, strlen(test), buf_map) != NULL);
	item = ts_map_lookup(test, strlen(test), buf_map);
	ck_assert(*item != NULL);
}
END_TEST

START_TEST(test_map_rem)
{
	struct ts_map_item_s **item;
	char *test;

	test = "Hello World!";

	ts_map_add(test, strlen(test), buf_map);
	item = ts_map_lookup(test, strlen(test), buf_map);
	ck_assert(*item != NULL);

	ts_map_rem(test, strlen(test), buf_map);
	item = ts_map_lookup(test, strlen(test), buf_map);
	ck_assert(*item == NULL);
}
END_TEST

START_TEST(test_map_add_collisions)
{
	struct ts_map_item_s **item;
	int idx, limit;

	char *test[] = {
	   "The void",
	   "The Great Gatsby",
	   "The purge",
	   "The Weather Channel",
	   "The World",
	};
	limit = 5;

	for (idx = 0; idx < limit; idx++)
	{
		ts_map_add(test[idx], strlen(test[idx]), buf_map);
		item = ts_map_lookup(test[idx], strlen(test[idx]), buf_map);
		ck_assert(*item != NULL);
	}
}
END_TEST

START_TEST(test_map_rem_collisions)
{
	struct ts_map_item_s **item;
	int idx, idx2, limit;

	char *test[] = {
	   "The void",
	   "The Great Gatsby",
	   "The purge",
	   "The Weather Channel",
	   "The World",
	};
	limit = 5;

	for (idx = 0; idx < limit; idx++)
	{
		ts_map_add(test[idx], strlen(test[idx]), buf_map);
		item = ts_map_lookup(test[idx], strlen(test[idx]), buf_map);
		ck_assert(*item != NULL);
	}

	for (idx = 0; idx < limit; idx++)
	{
		ts_map_rem(test[idx], strlen(test[idx]), buf_map);
		item = ts_map_lookup(test[idx], strlen(test[idx]), buf_map);
		ck_assert(*item == NULL);
		for (idx2 = idx + 1; idx2 < limit; idx2++)
		{
			item = ts_map_lookup(test[idx2], strlen(test[idx2]), buf_map);
			ck_assert_msg(*item != NULL, "idx2: %d\nstr: %s\n", idx2, test[idx2]);
		}
	}
}
END_TEST

void
setup_map_test(void) {
	buf_map = ts_map_new(5);
}

void
teardown_map_test(void) {
	ts_map_free(&buf_map);
}

TCase *
tcase_map(void) {
	TCase *tc = tcase_create("map");
	tcase_add_checked_fixture(tc, setup_map_test, teardown_map_test);
	tcase_add_test(tc, test_map_add);
	tcase_add_test(tc, test_map_rem);
	tcase_add_test(tc, test_map_add_collisions);
	tcase_add_test(tc, test_map_rem_collisions);
	return (tc);
}

CHECK_MAIN_STANDALONE(map);
