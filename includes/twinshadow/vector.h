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

#include "twinshadow/macro.h"
#include "twinshadow/error.h"
#include "twinshadow/std.h"

typedef char ts_vector_operation_t;
typedef char ts_vector_policy_t;

struct ts_vector_s {
	void **head;
	void **tail;
	void **array;
	size_t count;
	size_t length;
	ts_vector_policy_t policy;
};

/* Operation enums
 *
 * The first enum defines properties of the operation to be performed
 *
 * The second enum defines which operations the properties apply to
 */

enum {
	TS_VECTOR_OP_SET = 1<<0, /* high: insert, low: remove */
	TS_VECTOR_OP_POS = 1<<1, /* high: tail, low: head */
	TS_VECTOR_OP_IDX = 1<<2 /* high: require an index, low: ignore index value */
};

enum {
	TS_VECTOR_OP_SHIFT   = 0,
	TS_VECTOR_OP_UNSHIFT = TS_VECTOR_OP_SET,
	TS_VECTOR_OP_POP     = TS_VECTOR_OP_POS,
	TS_VECTOR_OP_PUSH    = TS_VECTOR_OP_POS + TS_VECTOR_OP_SET,
	TS_VECTOR_OP_REMOVE  = TS_VECTOR_OP_IDX,
	TS_VECTOR_OP_INSERT  = TS_VECTOR_OP_IDX + TS_VECTOR_OP_SET,
};

enum {
	TS_VECTOR_POLICY_IMMEDIATE = 1<<0, /* high: always modify memory on operation */
	TS_VECTOR_POLICY_EAGER = 1<<1, /* high: allocate extra memory on insert */
	TS_VECTOR_POLICY_SNAKE = 1<<2, /* high: never deallocate memory */
	TS_VECTOR_POLICY_RESV0 = 1<<6,
	TS_VECTOR_POLICY_RESV1 = 1<<7
};

#define TS_VECTOR_FOREACH(__var, __head) \
for (__var = __head->array;              \
     __var <= __head->cursor;            \
     __var++)

#define TS_VECTOR_RFOREACH(__var, __head) \
for (__var = __head->cursor;              \
     __var >= __head->array;              \
     __var--)

#define TS_VECTOR_IS_VALID(__head) do { \
	TS_ERR_NULL(__head);            \
	TS_ERR_NULL((__head)->array);   \
	TS_ERR_NULL((__head)->head);    \
	TS_ERR_NULL((__head)->tail);    \
	TS_ERR_ZERO((__head)->length);  \
} while (0)

struct ts_vector_s *ts_vector_new(ts_vector_policy_t policy);
void ts_vector_free(struct ts_vector_s **head);

void ts_vector_push(struct ts_vector_s *head, void *data);
void ts_vector_unshift(struct ts_vector_s *head, void *data);
void ts_vector_insert(struct ts_vector_s *head, void *data, int idx);
void * ts_vector_pop(struct ts_vector_s *head);
void * ts_vector_shift(struct ts_vector_s *head);
void * ts_vector_remove(struct ts_vector_s *head, int idx);

#endif /* TWINSHADOW_VECTOR_H */
