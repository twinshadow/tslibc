/* Copyright (c) 2013, 2014, Anthony Cornehl
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

static char **ptr;
static char *rptr;

static char *buf_strtok;
static char *buf_strrtok;
static char **buf_strrtok_xpct;

void
test_me (char *test_str, char *test_tok, size_t token_count) {
	buf_strrtok = strdup(test_str);
	buf_strtok = strdup(test_str);
	buf_strrtok_xpct = calloc(token_count, sizeof(char*));
	ptr = buf_strrtok_xpct;
	STRTOK(*ptr, buf_strtok, test_tok) {
		ptr++;
	}

	STRRTOK(rptr, buf_strrtok, test_tok) {
		ptr--;
		ck_assert(*ptr >= *buf_strrtok_xpct);
		ck_assert(rptr != NULL);
		TS_DEBUG("'%s' == '%s'", rptr, *ptr);
		ck_assert_str_eq(rptr, *ptr);
	}
	ck_assert(*ptr == *buf_strrtok_xpct);
}

START_TEST(strrtok_returns_string_tokens_in_reverse)
{
	test_me("one.two.three.four.five", ".", 6);
}
END_TEST

START_TEST(ignores_extra_tokens_inside_string) {
	test_me("one...two....three..........four.five", ".", 6);
}
END_TEST

START_TEST(ignores_tokens_outside_string) {
	test_me("...one.two.three.four.five...", ".", 6);
}
END_TEST

START_TEST(multichar_delimiters) {
	test_me(".-.one./.two.-../three/..-..four..-..five//-.", "./-", 6);
}
END_TEST

void
setup_strrtok(void) {
	return;
}

void
teardown_strrtok(void) {
	free(buf_strrtok);
	free(buf_strtok);
	free(buf_strrtok_xpct);
}

TCase *
tcase_strrtok(void) {
	TCase *tc = tcase_create("strrtok");
	tcase_add_checked_fixture(tc, setup_strrtok, teardown_strrtok);
	tcase_add_test(tc, strrtok_returns_string_tokens_in_reverse);
	tcase_add_test(tc, ignores_extra_tokens_inside_string);
	tcase_add_test(tc, ignores_tokens_outside_string);
	tcase_add_test(tc, multichar_delimiters);
	return tc;
}

CHECK_MAIN_STANDALONE(strrtok);
