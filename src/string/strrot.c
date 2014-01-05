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
ts_strnrot(int offset, char *str, size_t len)
{
	const char *ciphers[3] = {
		"abcdefghijklmnopqrstuvwxyz",
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ",
		"0123456789",
	};
	const size_t ciphers_len = LENGTH(ciphers);
	size_t cipher_strlen;
	size_t cipher_idx;
	char *chr_idx;
	size_t idx;

	TS_CHECK_DEBUG(str, "A NULL pointer was passed in.");
	TS_CHECK_DEBUG(str[0] != '\0', "Empty string.");
	TS_CHECK_DEBUG(offset != 0, "Offset does nothing");

	for (idx = 0; idx < len; idx++)
	{
		for (cipher_idx = 0; cipher_idx < ciphers_len; cipher_idx++)
		{
			UNLESS ((chr_idx = strchr(ciphers[cipher_idx], str[idx])))
				continue;

			cipher_strlen = strlen(ciphers[cipher_idx]);
			REDUCE_OFFSET(offset, cipher_strlen);
			if (offset == 0 || offset == cipher_strlen)
				continue;

			str[idx] = (((chr_idx - ciphers[cipher_idx]) + offset
						< cipher_strlen)
					? *(chr_idx + offset)
					: *(ciphers[cipher_idx] +
						(((chr_idx - ciphers[cipher_idx]) + offset) - cipher_strlen)
					)
			);
		}
	}
	goto out;
error:
out:
	return;
}

void
ts_strrot(int offset, char *str)
{
	ts_strnrot(offset, str, strnlen(str, SIZE_MAX));
}
