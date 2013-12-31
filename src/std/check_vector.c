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
#include "twinshadow/vector.h"

struct ts_vector_s *buf_vector = NULL;
uint8_t numbuf[] = { 42, 36, 18, 64, 101, 88, 19, 21, 76, 48, 88, 24 };

START_TEST(push_value)
{
	char *strbuf = "Dingo";
	ck_assert(ts_vector_unshift(buf_vector, &strbuf) == 0);
}
END_TEST

START_TEST(push_int)
{
	uint8_t buf = 42, foo = 48;
	ts_vector_free(buf_vector);
	buf_vector = ts_vector_new(sizeof(uint8_t));
	ck_assert(ts_vector_push(buf_vector, &buf) == 0);
	ck_assert(ts_vector_push(buf_vector, &foo) == 0);
	TS_DEBUG("%d", *(uint8_t*)ts_vector_get(buf_vector, 0));
	TS_DEBUG("%d", *(uint8_t*)ts_vector_get(buf_vector, 1));
}
END_TEST

START_TEST(push_shift_array) {
	uint8_t *iptr;
	uint8_t *iidx;
	int idx;
	iptr = calloc(1, sizeof(uint8_t));
	ts_vector_free(buf_vector);
	buf_vector = ts_vector_new(sizeof(uint8_t));
	REPEAT(idx, 12) {
		ck_assert(ts_vector_push(buf_vector, &numbuf[idx]) == 0);
		iidx = buf_vector->tail;
		ck_assert_int_eq(*iidx, numbuf[idx]);
		iidx = buf_vector->head;
		ck_assert_int_eq(*iidx, numbuf[0]);
	}
	REPEAT(idx, 12) {
		ck_assert(ts_vector_shift(buf_vector, iptr) == 0);
		ck_assert_int_eq(*iptr, numbuf[idx]);
	}
	free(iptr);
}
END_TEST

START_TEST(unshift_pop_array) {
	uint8_t *iptr;
	uint8_t *iidx;
	int idx;
	iptr = calloc(1, sizeof(uint8_t));
	ts_vector_free(buf_vector);
	buf_vector = ts_vector_new(sizeof(uint8_t));
	REPEAT(idx, 12) {
		ck_assert(ts_vector_unshift(buf_vector, &numbuf[idx]) == 0);
		iidx = buf_vector->head;
		ck_assert_int_eq(*iidx, numbuf[idx]);
		iidx = buf_vector->tail;
		ck_assert_int_eq(*iidx, numbuf[0]);
	}
	REPEAT(idx, 12) {
		ck_assert(ts_vector_pop(buf_vector, iptr) == 0);
		ck_assert_int_eq(*iptr, numbuf[idx]);
	}
	free(iptr);
}
END_TEST

START_TEST(test_vector_to_array) {
	int idx;
	uint8_t *up1, *up2;
	struct ts_array_s *arr;
	ts_vector_free(buf_vector);
	buf_vector = ts_vector_new(sizeof(uint8_t));
	REPEAT(idx, 12) {
		ck_assert(ts_vector_push(buf_vector, &numbuf[idx]) == 0);
	}
	arr = ts_vector_to_array(buf_vector);
	REPEAT(idx, 12) {
		up1 = ts_array_get(arr, idx);
		up2 = ts_vector_get(buf_vector, idx);
		ck_assert_int_eq(*up1, *up2);
	}
	ts_array_free(arr);
}
END_TEST

START_TEST(test_array_to_vector) {
}
END_TEST

void
setup_vector(void) {
	buf_vector = ts_vector_new(sizeof(char*));
}

void
teardown_vector(void) {
	ts_vector_free(buf_vector);
}

TCase *
tcase_vector(void) {
	TCase *tc = tcase_create("vector");
	tcase_add_checked_fixture(tc, setup_vector, teardown_vector);
	tcase_add_test(tc, push_value);
	tcase_add_test(tc, push_int);
	tcase_add_test(tc, push_shift_array);
	tcase_add_test(tc, unshift_pop_array);
	tcase_add_test(tc, test_vector_to_array);
	tcase_add_test(tc, test_array_to_vector);
	return tc;
}

CHECK_MAIN_STANDALONE(vector);
