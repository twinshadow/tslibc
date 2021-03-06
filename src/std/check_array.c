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
const char *ts_array_expect[] = {
	"lorem",
	"ipsum",
	"delorum",
	"sit",
	"amet",
	"consectetur",
	"adipisicing",
	"elit",
	"sed",
	"do",
};

START_TEST(test_array)
{
	char **idx;
	int i;

	i = 0;
	TS_ARRAY_FOREACH(idx, buf_array) {
		*idx = (char*)ts_array_expect[i];
		ck_assert_ptr_eq(*idx, ts_array_expect[i]);
		i++;
	}

	i = 4;
	TS_ARRAY_RFOREACH(idx, buf_array) {
		TS_DEBUG("%s", *idx);
		ck_assert_ptr_eq(*idx, ts_array_expect[i]);
		i--;
	}
}
END_TEST

START_TEST(test_array_get_value) {
	char **idx;
	int i;

	REPEAT(i, 5) {
		idx = ts_array_get(buf_array, i);
		*idx = (char*)ts_array_expect[i];
	}

	REPEAT(i, 5) {
		idx = ts_array_get(buf_array, i);
		ck_assert_ptr_eq(*idx, ts_array_expect[i]);
	}
}
END_TEST

START_TEST(test_resize_array_count) {
	char **idx;
	int i;

	i = 0;
	TS_ARRAY_FOREACH(idx, buf_array) {
		*idx = (char*)ts_array_expect[i];
		TS_DEBUG("%s", *idx);
		i++;
	}

	ts_array_resize(buf_array, 10, 0);

	i = 5;
	TS_ARRAY_FOREACH_OFFSET(idx, buf_array, 5) {
		*idx = (char*)ts_array_expect[i];
		TS_DEBUG("%s", *idx);
		i++;
	}

	i = 0;
	TS_ARRAY_FOREACH(idx, buf_array) {
		TS_DEBUG("%s", *idx);
		ck_assert_ptr_eq(*idx, ts_array_expect[i]);
		i++;
	}
}
END_TEST

START_TEST(test_resize_array_size) {
	int i;
	uint8_t *i1;
	uint64_t *i2;
	uint8_t numarray[] = { 255, 128, 64, 32, 0 };
	size_t numarray_count = 5;

	ts_array_free(buf_array);
	buf_array = ts_array_new(5, sizeof(uint8_t));
	REPEAT(i, numarray_count) {
		i1 = ts_array_get(buf_array, i);
		TS_DEBUG_PTR(i1);
		memcpy(i1, &numarray[i], sizeof(uint8_t));
	}
	ts_array_resize(buf_array, 0, sizeof(uint64_t));
	REPEAT(i, 5) {
		i2 = ts_array_get(buf_array, i);
		TS_DEBUG_PTR(i2);
		ck_assert_int_eq(*i2, numarray[i]);
	}
}
END_TEST

START_TEST(test_convert_mem_to_array) {
	int *idx;
	int i;
	int iter[] = { 12, 24, 36, 48, 60 };

	ts_array_free(buf_array);
	buf_array = ts_mem_to_array(iter, 5, sizeof(int));
	i = 0;
	TS_ARRAY_FOREACH(idx, buf_array) {
		TS_DEBUG("%d", *idx);
		ck_assert_int_eq(*idx, iter[i]);
		i++;
	}
}
END_TEST

void
setup_array_test(void) {
	buf_array = ts_array_new(5, sizeof(char*));
}

void
teardown_array_test(void) {
	ts_array_free(buf_array);
}

TCase *
tcase_array(void) {
	TCase *tc = tcase_create("array");
	tcase_add_checked_fixture(tc, setup_array_test, teardown_array_test);
	tcase_add_test(tc, test_array);
	tcase_add_test(tc, test_array_get_value);
	tcase_add_test(tc, test_resize_array_count);
	tcase_add_test(tc, test_resize_array_size);
	tcase_add_test(tc, test_convert_mem_to_array);
	return tc;
}

CHECK_MAIN_STANDALONE(array);
