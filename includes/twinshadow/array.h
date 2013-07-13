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

#define TS_ARRAY_PROTO_NEW(__head, __count, __size) do {    \
	(__head)->array = calloc(__count, __size);          \
	(__head)->sentinal = &(__head)->array[__count - 1]; \
} while (0)


#define TS_ARRAY_PROTO_FREE(__head) do { \
	free((__head)->array);           \
	(__head)->sentinal = NULL;       \
} while (0)

#define TS_ARRAY_PROTO_REALLOC(__head, __count, __size) do {        \
	size_t oldcount;                                            \
	oldcount = ((__head)->sentinal - (__head)->array) / __size; \
	(__head)->array realloc((__head)->array, __count * __size); \
	if ((__head)->array) {                                      \
		(__head)->sentinal = &(__head)->array[__count - 1]; \
		if (oldcount < __count) {                           \
			memset(&(__head)->array[oldcount],          \
			    '\0', (__count - oldcount)              \
			    * __size);                              \
		}                                                   \
	}                                                           \
} while (0)

/* Twinshadow vector meta-typing */
#define TS_ARRAY_HEAD(__name, __type) \
struct __name {                       \
	__type *sentinal;             \
	__type *array;                \
}

/* iterators */
#define TS_ARRAY_FOREACH(__var, __head) \
for ((__var) = (__head)->array;             \
     (__var) <= (__head)->sentinal;         \
     (__var)++)

#define TS_ARRAY_RFOREACH(__var, __head) \
for ((__var) = (__head)->sentinal;           \
     (__var) >= (__head)->array;             \
     (__var)--)

/* Memory functions */
#define TS_ARRAY_NEW(__name, __type)             \
struct __name *                                  \
__name##_new(size_t count) {                     \
	struct __name *head;                     \
	head = calloc(1, sizeof(struct __name)); \
	if (head == NULL) {                      \
		return NULL;                     \
	}                                        \
	__name##_alloc(head, count);             \
	if (head->array == NULL) {               \
		free(head);                      \
		return NULL;                     \
	}                                        \
	return head;                             \
}

#define TS_ARRAY_FREE(__name)        \
void                                 \
__name##_free(struct __name *head) { \
	if (head == NULL)            \
		return;              \
	__name##_alloc(head, 0);     \
	free(head);                  \
	head = NULL;                 \
}

#define TS_ARRAY_RESIZE(__name, __type)              \
void                                                 \
__name##_resize(struct __name *head, size_t count) { \
	__name##_alloc(head, count);                 \
}

#define TS_ARRAY_ALLOC(__name, __type)                               \
void                                                                 \
__name##_alloc(struct __name *head, size_t count) {                  \
	UNLESS (head->array) {                                       \
		TS_ARRAY_PROTO_NEW(head, count, sizeof(__type));\
		return;                                              \
	}                                                            \
	else UNLESS (count) {                                        \
		TS_ARRAY_PROTO_FREE(head); \
		return;                                              \
	}                                                            \
	size_t old_count;                                            \
	__type *swap;                                                \
	old_count = (head->sentinal - head->array) / sizeof(__type); \
	swap = realloc(head->array, count * sizeof(__type));         \
	if (swap) {                                                  \
		head->array = swap;                                  \
		head->sentinal = &head->array[count - 1];            \
		if (old_count < count) {                             \
			memset(&head->array[old_count],              \
			    '\0', (count - old_count)                \
			    * sizeof(__type));                       \
		}                                                    \
	}                                                            \
}                                                                    \

#define TS_ARRAY_PROTOTYPES(__name, __type)              \
TS_ARRAY_HEAD(__name, __type);                           \
void __name##_alloc(struct __name *head, size_t count);  \
struct __name * __name##_new(size_t count);              \
void __name##_resize(struct __name *head, size_t count); \
void __name##_free(struct __name *head);

#endif /* TWINSHADOW_ARRAY_H */
