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

char *buf_strrtok;

START_TEST(strrtok_returns_string_tokens_in_reverse)
{
	char *xpct[] = {
		"five",
		"four",
		"three",
		"two",
		"one"
	};

	int i;
	char *ptr = NULL;
	for (i = 0; (ptr = ts_strrtok(buf_strrtok, ' ')); i++)
		ck_assert_str_eq(ptr, xpct[i]);
}
END_TEST

void
setup_strrtok(void) {
	buf_strrtok = strdup("one two three four five");
}

void
teardown_strrtok(void) {
	free(buf_strrtok);
}

TCase *
tcase_strrtok(void) {
	TCase *tc = tcase_create("strrtok");
	tcase_add_checked_fixture(tc, setup_strrtok, teardown_strrtok);
	tcase_add_test(tc, strrtok_returns_string_tokens_in_reverse);
	return tc;
}

CHECK_MAIN_STANDALONE(strrtok);
