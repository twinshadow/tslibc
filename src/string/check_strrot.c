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

char *buf_strrot;

START_TEST(rotates_alphanumeric_strings)
{
	ts_strrot(13, buf_strrot);
	ck_assert_str_eq(buf_strrot, "nopklm NOPKLM 345012");
}
END_TEST

START_TEST(handles_zero_offset) {
	ts_strrot(26, buf_strrot);
	ck_assert_str_eq(buf_strrot, "abcxyz ABCXYZ 678345");
}
END_TEST

void
setup_strrot(void) {
	buf_strrot = strdup("abcxyz ABCXYZ 012789");
}

void
teardown_strrot(void) {
	free(buf_strrot);
}

TCase *
tcase_strrot(void) {
	TCase *tc = tcase_create("strrot");
	tcase_add_checked_fixture(tc, setup_strrot, teardown_strrot);
	tcase_add_test(tc, rotates_alphanumeric_strings);
	tcase_add_test(tc, handles_zero_offset);
	return tc;
}

CHECK_MAIN_STANDALONE(strrot);
