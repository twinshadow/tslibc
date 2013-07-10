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
#include "twinshadow/array.h"

TS_ARRAY_HEAD(ivec, int);
TS_ARRAY_NEW(ivec, int);
TS_ARRAY_FREE(ivec);
TS_ARRAY_RESIZE(ivec, int);

START_TEST(test_vec)
{
	struct ivec *test;
	int i, *idx;

	test = ivec_new(10);
	i = 0;
	TS_ARRAY_FOREACH(idx, test) {
		*idx = i++;
	}

	i = 9;
	TS_ARRAY_RFOREACH(idx, test) {
		ck_assert_int_eq(*idx, i--);
	}

	ivec_resize(test, 20);
	i = 0;
	TS_ARRAY_FOREACH(idx, test) {
		*idx = i++;
	}

	i = 19;
	TS_ARRAY_RFOREACH(idx, test) {
		ck_assert_int_eq(*idx, i--);
	}
	ivec_free(test);
}
END_TEST

TS_ARRAY_HEAD(svec, char*);
TS_ARRAY_NEW(svec, char*);
TS_ARRAY_FREE(svec);
TS_ARRAY_RESIZE(svec, char*);

START_TEST(test_vec2)
{
	struct svec *test;
	char **idx;
	int i;
	char *expect[] = {
		"lorem",
		"ipsum",
		"delorum",
		"sit",
		"amet"
	};

	test = svec_new(5);
	i = 0;
	TS_ARRAY_FOREACH(idx, test) {
		*idx = strdup(expect[i++]);
	}

	i = 4;
	TS_ARRAY_RFOREACH(idx, test) {
		ck_assert_str_eq(*idx, expect[i--]);
	}

	TS_ARRAY_FOREACH(idx, test) {
		free(*idx);
	}
	svec_free(test);
}
END_TEST

int
main(void)
{
	int number_failed;

	Suite *s = suite_create("check_vec");
	TCase *tc = tcase_create("Main");
	tcase_add_test(tc, test_vec);
	tcase_add_test(tc, test_vec2);
	suite_add_tcase(s, tc);

	SRunner *sr = srunner_create(s);
	srunner_run_all(sr, CK_VERBOSE);
	number_failed = srunner_ntests_failed(sr);
	srunner_free(sr);

	return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
