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

#include "twinshadow/check.h"
#include "twinshadow/hash.h"
#include "twinshadow/twinshadow.h"

ts_table_key_t ts_collision_hash(void *key, size_t len)
{
	return ((unsigned char*)key)[0];
}

START_TEST(test_hash_add)
{
	struct ts_table_s *table;
	struct ts_table_item_s **item;
	char *test;

	table = ts_table_new(4);
	test = "Hello World!";

	ts_table_add(test, strlen(test), table);
	item = ts_table_lookup(test, strlen(test), table);
	ck_assert(*item != NULL);

	ts_table_free(table);
}
END_TEST

START_TEST(test_hash_rem)
{
	struct ts_table_s *table;
	struct ts_table_item_s **item;
	char *test;

	table = ts_table_new(4);
	test = "Hello World!";

	ts_table_add(test, strlen(test), table);
	item = ts_table_lookup(test, strlen(test), table);
	ck_assert(*item != NULL);

	ts_table_rem(test, strlen(test), table);
	item = ts_table_lookup(test, strlen(test), table);
	ck_assert(*item == NULL);

	ts_table_free(table);
}
END_TEST

START_TEST(test_hash_add_collisions)
{
	struct ts_table_s *table;
	struct ts_table_item_s **item;
	int idx, limit;

	table = ts_table_new(4);
	table->hash = ts_collision_hash;
	char *test[] = {
	   "The void",
	   "The Great Gatsby",
	   "The purge",
	   "The Weather Channel",
	};

	for (idx = 0, limit = 4; idx < limit; idx++)
	{
		ts_table_add(test[idx], strlen(test[idx]), table);
		item = ts_table_lookup(test[idx], strlen(test[idx]), table);
		ck_assert(*item != NULL);
	}

	ts_table_free(table);
}
END_TEST

START_TEST(test_hash_rem_collisions)
{
	struct ts_table_s *table;
	struct ts_table_item_s **item;
	int idx, limit;
	char *swap;

	table = ts_table_new(4);
	table->hash = ts_collision_hash;
	char *test[] = {
	   "The void",
	   "The Great Gatsby",
	   "The purge",
	   "The Weather Channel",
	};

	for (idx = 0, limit = 4; idx < limit; idx++)
	{
		ts_table_add(test[idx], strlen(test[idx]), table);
		item = ts_table_lookup(test[idx], strlen(test[idx]), table);
		ck_assert(*item != NULL);
	}

	SWAP(test[0], test[2], swap);
	SWAP(test[1], test[3], swap);
	for (idx = 0, limit = 4; idx < limit; idx++)
	{
		ts_table_rem(test[idx], strlen(test[idx]), table);
		item = ts_table_lookup(test[idx], strlen(test[idx]), table);
		ck_assert(*item == NULL);
	}

	ts_table_free(table);
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
