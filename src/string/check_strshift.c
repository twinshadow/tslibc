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

#include "twinshadow/string.h"
#include "twinshadow/check.h"

START_TEST(test_strshift_pos1)
{
	char *buf = calloc(12, sizeof(char));
	strcpy(buf, "0123456789");
	ts_strshift(3, buf);
	ck_assert_str_eq(buf, "7890123456");
	free(buf);
}
END_TEST

START_TEST(test_strshift_pos2)
{
	char *buf = calloc(12, sizeof(char));
	strcpy(buf, "0123456789");
	ts_strshift(8, buf);
	ck_assert_str_eq(buf, "2345678901");
	free(buf);
}
END_TEST

START_TEST(test_strshift_empty)
{
	char *buf = calloc(12, sizeof(char));
	strcpy(buf, "");
	ts_strshift(3, buf);
	ck_assert_str_eq(buf, "");
	free(buf);
}
END_TEST

START_TEST(test_strshift_neg1)
{
	char *buf = calloc(12, sizeof(char));
	strcpy(buf, "0123456789");
	ts_strshift(-3, buf);
	ck_assert_str_eq(buf, "3456789012");
	free(buf);
}
END_TEST

START_TEST(test_strshift_neg2)
{
	char *buf = calloc(12, sizeof(char));
	strcpy(buf, "0123456789");
	ts_strshift(-8, buf);
	ck_assert_str_eq(buf, "8901234567");
	free(buf);
}
END_TEST

START_TEST(test_memshift_int)
{
	int i;
	int offset = 3;
	int buf[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	int shifted_buf[] = {7, 8, 9, 0, 1, 2, 3, 4, 5, 6};

	ts_memshift(offset * sizeof(int), &buf, 10 * sizeof(int));

	for(i = 0; i < 10; i++)
		ck_assert_int_eq(buf[i], shifted_buf[i]);
}
END_TEST

int
main(void)
{
	int number_failed;

	Suite *s = suite_create("String");
	TCase *tc_strshift = tcase_create("strshift");
	tcase_add_test(tc_strshift, test_strshift_pos1);
	tcase_add_test(tc_strshift, test_strshift_pos2);
	tcase_add_test(tc_strshift, test_strshift_neg1);
	tcase_add_test(tc_strshift, test_strshift_neg2);
	tcase_add_test(tc_strshift, test_strshift_empty);
	tcase_add_test(tc_strshift, test_memshift_int);
	suite_add_tcase(s, tc_strshift);

	SRunner *sr = srunner_create(s);
	srunner_run_all(sr, CK_VERBOSE);
	number_failed = srunner_ntests_failed(sr);
	srunner_free(sr);
	return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
