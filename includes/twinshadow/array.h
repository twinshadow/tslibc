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

#ifndef TWINSHADOW_ARRAY_H
#define TWINSHADOW_ARRAY_H
/* A structure wrapper around the existing array type to provide some
 * operational-safety */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "twinshadow/macro.h"

#define TS_ARRAY_INLINE_NEW(__head, __count, __size) do {                  \
	if ((__head)->array_head == NULL) {                                \
		(__head)->array_head = calloc(__count, __size);            \
		(__head)->array_tail = &(__head)->array_head[__count - 1]; \
	}                                                                  \
} while (0)

#define TS_ARRAY_INLINE_FREE(__head) do {    \
	if ((__head)->array_head) {          \
		free((__head)->array_head);  \
		(__head)->array_head = NULL; \
		(__head)->array_tail = NULL; \
	}                                    \
} while (0)

#define TS_ARRAY_INLINE_RESIZE(__head, __count, __size) do {                    \
	size_t oldcount;                                                        \
	oldcount = ((__head)->array_tail - (__head)->array_head) / __size;      \
	(__head)->array_head = realloc((__head)->array_head, __count * __size); \
	if ((__head)->array_head) {                                             \
		(__head)->array_tail = &(__head)->array_head[__count - 1];      \
		if (oldcount < __count) {                                       \
			memset(&(__head)->array_head[oldcount],                 \
			    '\0', (__count - oldcount)                          \
			    * __size);                                          \
		}                                                               \
	}                                                                       \
} while (0)

/* iterators */
#define TS_ARRAY_FOREACH(__var, __head) \
for (__var = (__head)->array_head;      \
     __var <= (__head)->array_tail;     \
     (__var)++)

#define TS_ARRAY_RFOREACH(__var, __head) \
for (__var = (__head)->array_tail;       \
     __var >= (__head)->array_head;      \
     (__var)--)


#define TS_ARRAY_ATTRIBUTES(__type) \
	__type *array_tail; \
	__type *array_head;

/* Twinshadow vector meta-typing */
#define TS_ARRAY_HEAD(__name, __type) \
struct __name {                       \
	TS_ARRAY_ATTRIBUTES(__type);  \
}

/* Memory functions */
#define TS_ARRAY_NEW(__name, __type)                      \
struct __name *                                           \
__name##_new(size_t count) {                              \
	struct __name *head;                              \
	head = calloc(1, sizeof(struct __name));          \
	if (head == NULL) {                               \
		return NULL;                              \
	}                                                 \
	TS_ARRAY_INLINE_NEW(head, count, sizeof(__type)); \
	if (head->array_head == NULL) {                   \
		free(head);                               \
		return NULL;                              \
	}                                                 \
	return head;                                      \
}

#define TS_ARRAY_FREE(__name)        \
void                                 \
__name##_free(struct __name *head) { \
	if (head == NULL)            \
		return;              \
	TS_ARRAY_INLINE_FREE(head);  \
	free(head);                  \
	head = NULL;                 \
}

#define TS_ARRAY_RESIZE(__name, __type)                      \
void                                                         \
__name##_resize(struct __name *head, size_t count) {         \
	TS_ARRAY_INLINE_RESIZE(head, count, sizeof(__type)); \
}

#define TS_ARRAY_PROTOTYPES(__name, __type)              \
TS_ARRAY_HEAD(__name, __type);                           \
struct __name * __name##_new(size_t count);              \
void __name##_resize(struct __name *head, size_t count); \
void __name##_free(struct __name *head);

#define TS_ARRAY_FUNCTIONS(__name, __type) \
TS_ARRAY_NEW(size_t count);                \
TS_ARRAY_FREE(struct __name *head);        \
TS_ARRAY_RESIZE(struct __name *head, size_t count);

/* Generic functions */
TS_ARRAY_PROTOTYPES(ts_array, void *);

#endif /* TWINSHADOW_ARRAY_H */
