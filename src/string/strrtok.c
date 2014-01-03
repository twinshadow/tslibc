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
#include <twinshadow/string.h>

void
ts_strrstrip(char *str, const char *delim) {
	char *cptr;
	const char *dptr, *sentinal;

	sentinal  = PTR_OFFSET(delim, strlen(delim), sizeof(char));
	cptr = PTR_OFFSET(str, strlen(str) - 1, sizeof(char));
	for (; cptr >= str; cptr--) {
		for (dptr = delim; dptr < sentinal; dptr++) {
			if (*cptr == *dptr) {
				*cptr = '\0';
				break;
			}
		}
		if (dptr == sentinal)
			break;
	}
	return;
}

char*
ts_strrtok_r(char *str, const char *delim, char **saveptr)
{
	char *cptr = NULL;
	const char *dptr, *sentinal;

	TS_ERR_STR(delim);
	TS_ERR_NULL(saveptr);
	if (str) {
		TS_ERR_STR_EMPTY(str);
		*saveptr = str;
	}
	TS_ERR_STR(*saveptr);
	ts_strrstrip(*saveptr, delim);
	TS_ERR_STR(*saveptr);

	sentinal  = PTR_OFFSET(delim, strlen(delim), sizeof(char));
	cptr = PTR_OFFSET(*saveptr, strlen(*saveptr) - 1, sizeof(char));
	for (; cptr >= *saveptr; cptr--) {
		for (dptr = delim; dptr < sentinal; dptr++) {
			if (*cptr == *dptr) {
				*cptr = '\0';
				break;
			}
		}
		if (dptr != sentinal) {
			str = cptr + sizeof(char);
			goto out;
		}
	}

	/* Reached the beginning of the string, print and unset saveptr */
	str = *saveptr;
	*saveptr = NULL;
	goto out;

error:
	str = NULL;

out:
	if (str && str[0] == '\0') {
		TS_DEBUG("Whups");
		return (NULL);
	}
	return (str);
}

char*
ts_strrtok(char *str, const char *delim)
{
	static char *lastptr = NULL;
	return (ts_strrtok_r(str, delim, &lastptr));
}
