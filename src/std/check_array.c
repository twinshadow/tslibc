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

struct ts_array_s *buf_array = NULL;

START_TEST(test_array)
{
	void **idx;
	int i;
	char *expect[] = {
		"lorem",
		"ipsum",
		"delorum",
		"sit",
		"amet"
	};

	i = 0;
	TS_ARRAY_FOREACH(idx, buf_array) {
		*idx = strdup(expect[i++]);
	}

	i = 4;
	TS_ARRAY_RFOREACH(idx, buf_array) {
		ck_assert_str_eq((char*)*idx, expect[i--]);
	}

	TS_ARRAY_FOREACH(idx, buf_array) {
		free(*idx);
	}
}
END_TEST

void
setup_array(void) {
	buf_array = ts_array_new(5);
}

void
teardown_array(void) {
	ts_array_free(buf_array);
}

TCase *
tcase_array(void) {
	TCase *tc = tcase_create("array");
	tcase_add_checked_fixture(tc, setup_array, teardown_array);
	tcase_add_test(tc, test_array);
	return tc;
}

CHECK_MAIN_STANDALONE(array);
