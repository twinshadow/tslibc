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

void
ts_memshift(int offset, void *ptr, size_t len)
{
	void *buf;
	size_t buflen;

	TS_CHECK_DEBUG(ptr, "A NULL pointer was passed in.");
	TS_CHECK_DEBUG(len > 1, "String too small.");

	TS_CHECK_DEBUG(offset != 0, "Offset does nothing");
	REDUCE_OFFSET(offset, len);
	TS_CHECK_DEBUG(offset != 0 || len != offset, "Offset does nothing.");

	buflen = (offset < len - offset) ? offset : len - offset;
	TS_CHECK_DEBUG(buflen != 0, "Buflen does nothing");
	buf = malloc(buflen);
	TS_CHECK_DEBUG(buf, "Out of memory.");

	if (buflen < offset) {
		memmove(buf, ptr, buflen);
		memmove(ptr, ptr + buflen, len - buflen);
		memmove(ptr + (len - buflen), buf, buflen);
	}
	else {
		memmove(buf, ptr + (len - offset), offset);
		memmove(ptr + offset, ptr, len - offset);
		memmove(ptr, buf, offset);
	}
	free(buf);
	goto out;

error:
out:
	return;
}

void
ts_strshift(int offset, char *str)
{
	TS_CHECK_DEBUG(str[0] != '\0', "Empty string.");
	ts_memshift(offset, str, strnlen(str, SIZE_MAX));
error:
	return;
}
