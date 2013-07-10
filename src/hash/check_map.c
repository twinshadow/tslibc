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

TS_MAP_PROTOTYPES(ts_map, void*, void*);

TS_ARRAY_NEW(ts_map_array, struct ts_map_item *);
TS_ARRAY_FREE(ts_map_array);

TS_MAP_NEW(ts_map);
TS_MAP_FREE(ts_map, free);
TS_MAP_LOOKUP(ts_map);
TS_MAP_ADD(ts_map);
TS_MAP_REM(ts_map);

START_TEST(test_hash_add)
{
	struct ts_map *map;
	struct ts_map_item **item;
	char *test;

	map = ts_map_new(4);
	test = "Hello World!";

	ck_assert(ts_map_add(test, strlen(test), map) != NULL);
	item = ts_map_lookup(test, strlen(test), map);
	ck_assert(*item != NULL);

	ts_map_free(map);
}
END_TEST

START_TEST(test_hash_rem)
{
	struct ts_map *map;
	struct ts_map_item **item;
	char *test;

	map = ts_map_new(4);
	test = "Hello World!";

	ts_map_add(test, strlen(test), map);
	item = ts_map_lookup(test, strlen(test), map);
	ck_assert(*item != NULL);

	ts_map_rem(test, strlen(test), map);
	item = ts_map_lookup(test, strlen(test), map);
	ck_assert(*item == NULL);

	ts_map_free(map);
}
END_TEST

START_TEST(test_hash_add_collisions)
{
	struct ts_map *map;
	struct ts_map_item **item;
	int idx, limit;

	map = ts_map_new(1);
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
		ts_map_add(test[idx], strlen(test[idx]), map);
		item = ts_map_lookup(test[idx], strlen(test[idx]), map);
		ck_assert(*item != NULL);
	}

	ts_map_free(map);
}
END_TEST

START_TEST(test_hash_rem_collisions)
{
	struct ts_map *map;
	struct ts_map_item **item;
	int idx, idx2, limit;

	map = ts_map_new(1);
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
		ts_map_add(test[idx], strlen(test[idx]), map);
		item = ts_map_lookup(test[idx], strlen(test[idx]), map);
		ck_assert(*item != NULL);
	}

	for (idx = 0; idx < limit; idx++)
	{
		ts_map_rem(test[idx], strlen(test[idx]), map);
		item = ts_map_lookup(test[idx], strlen(test[idx]), map);
		ck_assert(*item == NULL);
		for (idx2 = idx + 1; idx2 < limit; idx2++)
		{
			item = ts_map_lookup(test[idx2], strlen(test[idx2]), map);
			ck_assert_msg(*item != NULL, "idx2: %d\nstr: %s\n", idx2, test[idx2]);
		}
	}

	ts_map_free(map);
}
END_TEST

int
main(void)
{
	int number_failed;

	Suite *s = suite_create("check_hash");
	TCase *tc = tcase_create("Main");
	tcase_add_test(tc, test_hash_add);
	tcase_add_test(tc, test_hash_rem);
	tcase_add_test(tc, test_hash_add_collisions);
	tcase_add_test(tc, test_hash_rem_collisions);
	suite_add_tcase(s, tc);

	SRunner *sr = srunner_create(s);
	srunner_run_all(sr, CK_VERBOSE);
	number_failed = srunner_ntests_failed(sr);
	srunner_free(sr);

	return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
