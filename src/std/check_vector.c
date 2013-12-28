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

START_TEST(push_value)
{
	char *strbuf = "Dingo";
	ts_vector_unshift(buf_vector, &strbuf);
}
END_TEST

START_TEST(push_int)
{
	uint8_t buf = 42;
	ts_vector_free(buf_vector);
	buf_vector = ts_vector_new(sizeof(uint8_t));
	ts_vector_unshift(buf_vector, &buf);
	TS_DEBUG("%d", *(uint8_t*)ts_vector_get(buf_vector, 0));
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
	return tc;
}

CHECK_MAIN_STANDALONE(vector);
