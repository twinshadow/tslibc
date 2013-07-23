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

#include "twinshadow/error.h"

/* Operation enums
 *
 * The first enum defines properties of the operation to be performed
 *
 * The second enum defines which operations the properties apply to
 */
enum {
	TS_VECTOR_OPER_SET = 1<<0; /* high: insert, low: remove */
	TS_VECTOR_OPER_POS = 1<<1; /* high: tail, low: head */
	TS_VECTOR_OPER_IDX  = 1<<2; /* high: require an index, low: ignore index value */
};

enum {
	TS_VECTOR_OPER_SHIFT   = 0;
	TS_VECTOR_OPER_UNSHIFT = TS_VECTOR_OPER_SET;
	TS_VECTOR_OPER_POP     = TS_VECTOR_OPER_POS;
	TS_VECTOR_OPER_PUSH    = TS_VECTOR_OPER_POS + TS_VECTOR_OPER_SET;
	TS_VECTOR_OPER_REMOVE  = TS_VECTOR_OPER_IDX;
	TS_VECTOR_OPER_INSERT  = TS_VECTOR_OPER_IDX + TS_VECTOR_OPER_SET;
};

#define TS_VECTOR_FOREACH(__var, __head) \
for (__var = __head->vector;             \
     __var <= __head->cursor;            \
     __var++)

#define TS_VECTOR_RFOREACH(__var, __head) \
for (__var = __head->cursor;              \
     __var >= __head->vector;             \
     __var--)

/*
 _                _
| |    ___   __ _(_) ___
| |   / _ \ / _` | |/ __|
| |__| (_) | (_| | | (__
|_____\___/ \__, |_|\___|
            |___/
*/


#define TS_VECTOR_INLINE_NEW(__head, __size) do {                \
	if ((__head)->vector_head == NULL) {                     \
		(__head)->vector_cursor = (__head)->vector_head; \
		(__head)->vector_head = calloc(1, __size);       \
		(__head)->vector_tail = (__head)->vector_head;   \
		(__head)->vector_count = 0;                      \
		(__head)->vector_len = 1;                        \
	}                                                        \
} while (0)


#define TS_VECTOR_INLINE_FREE(__head) do {      \
	if ((__head)->vector_head) {            \
		free((__head)->vector_head);    \
		(__head)->vector_cursor = NULL; \
		(__head)->vector_head = NULL;   \
		(__head)->vector_tail = NULL;   \
		(__head)->vector_count = 0;     \
		(__head)->vector_len = 0;       \
	}                                       \
} while (0)

#define TS_VECTOR_INLINE_RESIZE(__head, __count, __size) do {                     \
	size_t oldcount;                                                          \
	oldcount = ((__head)->vector_tail - (__head)->vector_head) / __size;      \
	(__head)->vector_head = realloc((__head)->vector_head, __count * __size); \
	if ((__head)->vector_head) {                                              \
		(__head)->vector_tail = &(__head)->vector_head[__count - 1];      \
		if (oldcount < __count) {                                         \
			memset(&(__head)->vector_head[oldcount],                  \
			    '\0', (__count - oldcount)                            \
			    * __size);                                            \
		}                                                                 \
	}                                                                         \
} while (0)

#define TS_VECTOR_ATTRIBUTES(__type) \
	__type *vector_cursor;       \
	__type *vector_tail;         \
	__type *vector_head;         \
	size_t vector_count;         \
	size_t vector_len;           \

/*
 _____                    _       _
|_   _|__ _ __ ___  _ __ | | __ _| |_ ___
  | |/ _ \ '_ ` _ \| '_ \| |/ _` | __/ _ \
  | |  __/ | | | | | |_) | | (_| | ||  __/
  |_|\___|_| |_| |_| .__/|_|\__,_|\__\___|
                   |_|
*/

#define TS_VECTOR_HEAD(__name, __type) \
struct __name {                        \
	TS_VECTOR_ATTRIBUTES(__type)   \
}

#define TS_VECTOR_NEW(__name, __type)                      \
struct __name *                                            \
__name##_new(size_t count){                                \
	struct __name *head;                               \
	head = calloc(1, sizeof(struct __name));           \
	if (head == NULL) {                                \
		return NULL;                               \
	}                                                  \
	TS_VECTOR_INLINE_NEW(head, count, sizeof(__type)); \
	return head;                                       \
}

#define TS_VECTOR_FREE(__name)       \
void                                 \
__name##_free(struct __name *head){  \
	if (head == NULL)            \
		return;              \
	TS_VECTOR_INLINE_FREE(head); \
	free(head);                  \
	head = NULL;                 \
}

#define TS_VECTOR_RESIZE(__name, __type)                        \
void                                                            \
__name##_resize(struct __name *head, size_t count){             \
	size_t old_count, old_cursor;                           \
	if (head == NULL)                                       \
		return;                                         \
	if (count < 1 || head->len > count)                     \
		return;                                         \
	old_count = head->len;                                  \
	old_cursor = (head->cursor > head->vector) ?            \
	    (head->cursor - head->vector) / sizeof(__type) : 0; \
	head->vector = realloc(head->vector,                    \
	    count * sizeof(__type));                            \
	if (head->vector == NULL)                               \
		return;                                         \
	head->sentinal = &head->vector[count - 1];              \
	if (old_count < count) {                                \
		memset(&head->vector[old_count],                \
		    '\0', (count - old_count)                   \
		    * sizeof(__type));                          \
	}                                                       \
	return head;                                            \
}

/*
check(__idx < (__head)->count)
for (
__var = ((TS_VECTOR_OPER_IDX)
    ? &(__head)->vector_head[__idx]
    : ((TS_VECTOR_OPER_IDX)
        ? (__head)->vector_tail
	: (__head)->vector_head));
__var >= (__head)->vector_head && __var <= (__head)->vector_tail;

)
*/

#define TS_VECTOR_OPERATE(__head, __ptr, __oper, __idx) do {       \
	TS_CKDB_NULL(__head);                                      \
	TS_CHECK_DEBUG(count > 0, "No items to remove");           \
	TS_CHECK_DEBUG(__idx < (__head)->count,                    \
	    "Index outside of range");                             \
	if (TS_VECTOR_OPER_SET & __oper                            \
	    && (__head)->count + 1 => (__head)->len) {             \
		/* resize the vector */                            \
	}                                                          \
	switch (__oper) {                                          \
		case TS_VECTOR_OPER_POP:                           \
			__ptr = (__head)->vector_cursor;           \
			(__head)->vector_cursor-- = NULL;          \
			break;                                     \
		case TS_VECTOR_OPER_PUSH:                          \
			++(__head)->vector_cursor = __ptr;         \
			break;                                     \
		case TS_VECTOR_OPER_SHIFT:                         \
			__ptr = (__head)->vector_head;             \
			memmove((__head)->vector_head + 1,         \
			    (__head)->vector_head,                 \
			    (count - 1) * sizeof(__type));         \
			break;                                     \
		case TS_VECTOR_OPER_UNSHIFT:                       \
			memmove((__head)->vector_head,             \
			    (__head)->vector_head + 1,             \
			    count * sizeof(__type));               \
			(__head)->vector_head = __ptr;             \
			break;                                     \
		case TS_VECTOR_OPER_REMOVE:                        \
			__ptr = &(__head)->vector_head[__idx];     \
			memmove(&(__head)->vector_head[__idx + 1], \
			    &(__head)->vector_head[__idx],         \
			    (count - 1) * sizeof(__type));         \
			break;                                     \
		case TS_VECTOR_OPER_INSERT:                        \
			memmove(&(__head)->vector_head[__idx],     \
			    &(__head)->vector_head[__idx + 1],     \
			    count * sizeof(__type));               \
			&(__head)->vector_head[__idx] = __ptr;     \
			break;                                     \
		default:                                           \
			break;                                     \
	}                                                          \
	if (TS_VECTOR_OPER_SET & __oper)                           \
		(__head)->count++;                                 \
	else                                                       \
		(__head)->count--;                                 \
} while (0);

#define TS_VECTOR_PUSH(__name, __type) \
void \
__name##_push(struct __name *head, __type *val) { \
	TS_VECTOR_OPERATE(head, val, TS_VECTOR_OPER_PUSH);
}

/*
 ____            _        _
|  _ \ _ __ ___ | |_ ___ | |_ _   _ _ __   ___  ___
| |_) | '__/ _ \| __/ _ \| __| | | | '_ \ / _ \/ __|
|  __/| | | (_) | || (_) | |_| |_| | |_) |  __/\__ \
|_|   |_|  \___/ \__\___/ \__|\__, | .__/ \___||___/
                              |___/|_|
*/

#define TS_VECTOR_PROTOTYPES(__name, __type)             \
TS_VECTOR_HEAD(__name, __type);                          \
struct __name * __name##_new(size_t count);              \
void __name##_resize(struct __name *head, size_t count); \
void __name##_free(struct __name *head);

#define TS_VECTOR_FUNCTIONS(__name, __type) \
TS_VECTOR_NEW(__name, __type);              \
TS_VECTOR_FREE(__name);                     \
TS_VECTOR_RESIZE(__name, __type);

#endif /* TWINSHADOW_VECTOR_H */
