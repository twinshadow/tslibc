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

#include "twinshadow/array.h"

/* structures */

#define TS_VECTOR_HEAD(__name, __type)	\
struct __name {				\
	__type *vector;			\
	__type *sentinal;		\
	__type *cursor;			\
	size_t count;			\
	size_t len;			\
}

/* iterators */

#define TS_VECTOR_FOREACH(__var, __head)	\
for (__var = __head->vector;		\
     __var <= __head->cursor;		\
     __var++)

#define TS_VECTOR_RFOREACH(__var, __head)	\
for (__var = __head->cursor;		\
     __var >= __head->vector;		\
     __var--)

/* Memory functions */

#define TS_VECTOR_NEW(__name, __type)			\
struct __name *						\
__name##_new(size_t count){				\
	struct __name *head;				\
	head = calloc(1, sizeof(struct __name));	\
	if (head == NULL) {				\
		return NULL;				\
	}						\
	head->vector = calloc(count, sizeof(__type));	\
	if (head->vector == NULL) {			\
		free(head);				\
		return NULL;				\
	}						\
	head->sentinal = &head->vector[count - 1];		\
	return head;					\
}

#define TS_VECTOR_FREE(__name)		\
void					\
__name##_free(struct __name *head){	\
	if (head == NULL)		\
		return;			\
	free(head->vector);		\
	free(head);			\
	head = NULL;			\
}

#define TS_VECTOR_RESIZE(__name, __type)		\
void							\
__name##_resize(struct __name *head, size_t count){	\
	size_t old_count, old_cursor;			\
	if (head == NULL)				\
		return;					\
	if (count < 1 || head->len > count)		\
		return;					\
	old_count = head->len;				\
	old_cursor = (head->cursor > head->vector) ?	\
	    (head->cursor - head->vector) / sizeof(__type) : 0;	\
	head->vector = realloc(head->vector,		\
	    count * sizeof(__type));			\
	if (head->vector == NULL)			\
		return;					\
	head->sentinal = &head->vector[count - 1];	\
	if (old_count < count) {			\
		memset(&head->vector[old_count],	\
		    '\0', (count - old_count)		\
		    * sizeof(__type));			\
	}						\
	return head;					\
}

#define TS_VECTOR_PROTOTYPES(__name, __type)		\
TS_ARRAY_PROTOTYPES(__name##_array, __type);		\
TS_VECTOR_HEAD(__name, __type);				\
struct __name * __name##_new(size_t count);		\
void __name##_resize(struct __name *head, size_t count);	\
void __name##_free(struct __name *head);

#endif /* TWINSHADOW_VECTOR_H */
