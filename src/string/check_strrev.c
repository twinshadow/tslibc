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
#include "twinshadow/string.h"

char *buf_strrev;

START_TEST(strrev_reverses_the_string_order)
{
	ts_strrev(buf_strrev);
	ck_assert_str_eq(buf_strrev, "54321");
}
END_TEST

START_TEST(strrev_does_not_affect_string_length)
{
	int original, modified;
	original = strlen(buf_strrev);
	ts_strrev(buf_strrev);
	modified = strlen(buf_strrev);
	ck_assert_int_eq(modified, original);
}
END_TEST

START_TEST(strrev_does_not_segfault_on_null)
{
	ts_strrev(NULL);
}
END_TEST

void
setup_strrev(void) {
	buf_strrev = strdup("12345");
}

void
teardown_strrev(void) {
	free(buf_strrev);
}

TCase *
tcase_strrev(void) {
	TCase *tc = tcase_create("strrev");
	tcase_add_checked_fixture(tc, setup_strrev, teardown_strrev);
	tcase_add_test(tc, strrev_reverses_the_string_order);
	tcase_add_test(tc, strrev_does_not_segfault_on_null);
	tcase_add_test(tc, strrev_does_not_affect_string_length);
	return tc;
}

CHECK_MAIN_STANDALONE(strrev);
