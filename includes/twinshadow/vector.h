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

#include "twinshadow/array.h"
#include "twinshadow/macro.h"
#include "twinshadow/error.h"

typedef int64_t ts_vector_idx_t;

#define TS_VECTOR_IDX_HEAD 0
#define TS_VECTOR_IDX_TAIL -1

typedef enum _ts_vector_operation {
	TS_VECTOR_OP_REMOVE,
	TS_VECTOR_OP_INSERT,
} ts_vector_operation_t;

typedef enum _ts_vector_policy {
	TS_VECTOR_POLICY_IMMEDIATE = 1<<0, /* high: always modify memory on operation */
	TS_VECTOR_POLICY_EAGER = 1<<1, /* high: allocate extra memory on insert */
	TS_VECTOR_POLICY_SNAKE = 1<<2, /* high: never deallocate memory */
	TS_VECTOR_POLICY_RESV0 = 1<<6,
	TS_VECTOR_POLICY_RESV1 = 1<<7
} ts_vector_policy_t;

/* TODO: Make this hold the methods and properties for memory-management */
struct ts_vector_policy_s {
	ts_vector_policy_t flags;
};

struct ts_vector_s {
	void *head;
	void *tail;
	void *array;
	size_t count;
	size_t length;
	size_t size;
	struct ts_vector_policy_s policy;
};

#define TS_VECTOR_IS_VALID(__head) do { \
	TS_ERR_NULL(__head);            \
	TS_ERR_ZERO((__head)->size);    \
} while (0)

#define TS_VECTOR_OFFSET(__head, __idx) (                \
	(__idx) == TS_VECTOR_IDX_TAIL ? (__head)->tail : \
	(__idx) == TS_VECTOR_IDX_HEAD ? (__head)->head : \
	(__idx) < (__head)->count ? PTR_OFFSET((__head)->head, (__idx), (__head)->size) : \
	NULL)

#define TS_VECTOR_CHECK_IDX(__head, __idx)            \
	TS_CHECK((__idx) == TS_VECTOR_IDX_HEAD        \
		|| (__idx) == TS_VECTOR_IDX_TAIL      \
		|| POSITIVE(__idx) < (__head)->count, \
		"Index outside of vector items.")

struct ts_vector_s *ts_vector_new(size_t size);
void ts_vector_free(struct ts_vector_s *head);

// getters
void *ts_vector_get(struct ts_vector_s *head, ts_vector_idx_t idx);
void *ts_vector_find(struct ts_vector_s *head, void *data);

// modifiers
int ts_vector_push(struct ts_vector_s *head, void *data);
int ts_vector_unshift(struct ts_vector_s *head, void *data);
int ts_vector_insert(struct ts_vector_s *head, void *data,  ts_vector_idx_t idx);
int ts_vector_pop(struct ts_vector_s *head, void *data);
int ts_vector_shift(struct ts_vector_s *head, void *data);
int ts_vector_remove(struct ts_vector_s *head, void *data, ts_vector_idx_t idx);

#endif /* TWINSHADOW_VECTOR_H */
