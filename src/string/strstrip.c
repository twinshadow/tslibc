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

#include <ctype.h>
#include <stdint.h>
#include <stdio.h>

#include "twinshadow/error.h"
#include "twinshadow/string.h"

void
ts_strnstrip(char *str, size_t len)
{
	char *head, *tail;

	TS_CHECK_DEBUG(str, "A NULL pointer was passed in.");
	TS_CHECK_DEBUG(str[0] != '\0', "Empty string.");
	TS_CHECK_DEBUG(len > 1, "Empty string.");

	for (head = str;
	    isspace(*head);
	    head++, len--);

	tail = head + len;
	if (*tail == '\0')
		tail--;
	for (;
	    tail != head && isspace(*tail);
	    tail--, len--);

	memmove(str, head, len * sizeof(char));
	str[len] = '\0';

	goto out;

error:
out:
	return;
}

void
ts_strstrip(char *str)
{
	ts_strnstrip(str, strnlen(str, SIZE_MAX - 1));
}
