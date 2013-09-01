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

#ifndef TWINSHADOW_VECTOR_H
#define TWINSHADOW_VECTOR_H

#include <stdlib.h>

#include "twinshadow/error.h"
#include "twinshadow/array.h"

struct ts_vector_s {
	void **cursor;
	struct ts_array_s *array;
	size_t count;
	size_t len;
};

/* Operation enums
 *
 * The first enum defines properties of the operation to be performed
 *
 * The second enum defines which operations the properties apply to
 */

enum {
	TS_VECTOR_OPER_SET = 1<<0, /* high: insert, low: remove */
	TS_VECTOR_OPER_POS = 1<<1, /* high: tail, low: head */
	TS_VECTOR_OPER_IDX = 1<<2 /* high: require an index, low: ignore index value */
};

enum {
	TS_VECTOR_OPER_SHIFT   = 0,
	TS_VECTOR_OPER_UNSHIFT = TS_VECTOR_OPER_SET,
	TS_VECTOR_OPER_POP     = TS_VECTOR_OPER_POS,
	TS_VECTOR_OPER_PUSH    = TS_VECTOR_OPER_POS + TS_VECTOR_OPER_SET,
	TS_VECTOR_OPER_REMOVE  = TS_VECTOR_OPER_IDX,
	TS_VECTOR_OPER_INSERT  = TS_VECTOR_OPER_IDX + TS_VECTOR_OPER_SET,
};

#define TS_VECTOR_FOREACH(__var, __head) \
for (__var = __head->vector;             \
     __var <= __head->cursor;            \
     __var++)

#define TS_VECTOR_RFOREACH(__var, __head) \
for (__var = __head->cursor;              \
     __var >= __head->vector;             \
     __var--)

struct ts_vector_s *ts_vector_new(size_t count);
void ts_vector_free(struct ts_vector_s *head);
void ts_vector_resize(struct ts_vector_s *head, size_t count);
void ts_vector_operate(struct ts_vector_s *head, int oper, size_t idx);

#endif /* TWINSHADOW_VECTOR_H */
