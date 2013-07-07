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

#include <errno.h>
#include "twinshadow/string.h"
#include "twinshadow/twinshadow.h"

void
ts_memshift(int offset, void *ptr, size_t len)
{
	void *buf;
	int buflen;

	if (len < 2)
		return;

	REDUCE_OFFSET(offset, len);
	/* Eliminate conditions that would result in no changes */
	if (offset == 0 || len == offset)
		return;

	buflen = (offset < len - offset) ? offset : len - offset;
	buf = malloc(buflen);
	if (buf == NULL)
	{
		errno = ENOMEM;
		return;
	}

	if (buflen < offset)
	{
		memmove(buf, ptr, buflen);
		memmove(ptr, ptr + buflen, len - buflen);
		memmove(ptr + (len - buflen), buf, buflen);
	}
	else
	{
		memmove(buf, ptr + (len - offset), offset);
		memmove(ptr + offset, ptr, len - offset);
		memmove(ptr, buf, offset);
	}
	free(buf);
}

void
ts_strshift(int offset, char *str)
{
	ts_memshift(offset, str, strnlen(str, SIZE_MAX - 1));
}
