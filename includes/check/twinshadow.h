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

#ifndef TWINSHADOW_CHECK_H
#define TWINSHADOW_CHECK_H

#include <check.h>
#include <stdlib.h>
#include <stdio.h>

/*
 * find "${TOP}/src" -mindepth 1 -maxdepth 1 -type d -printf '%f\n'
 * find "${TOP}/src" -name 'check_*.c' | cut -d'_' -f2 | xargs -I% -- basename % .c
 */

/* srunner_cora
 * 	Does a srunner create or add, more smarter */

#ifdef CHECK_STANDALONE
#define CHECK_MAIN_STANDALONE(__name)              \
int                                                \
main(void) {                                       \
	int number_failed;                         \
	Suite *s = suite_create(__FILE__);   \
	suite_add_tcase(s, tcase_##__name());      \
	SRunner *sr = srunner_create(s);           \
	srunner_run_all(sr, CK_VERBOSE);           \
	number_failed = srunner_ntests_failed(sr); \
	srunner_free(sr);                          \
	return ((number_failed == 0) ?             \
		EXIT_SUCCESS : EXIT_FAILURE);      \
}
#else /* CHECK_STANDALONE */
#define CHECK_MAIN_STANDALONE(__name) void __not_##__name##_main__(void){}
#endif /* CHECK_STANDALONE */

#endif /* TWINSHADOW_CHECK_H */
