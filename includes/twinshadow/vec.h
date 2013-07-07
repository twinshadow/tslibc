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

#ifndef TWINSHADOW_VEC_H
#define TWINSHADOW_VEC_H
/* A structure wrapper around the existing array type to provide some
 * operational-safety */

#include <errno.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/* Twinshadow vector meta-typing */
#define TS_VEC_HEAD(__name, __type)		\
	struct __name {				\
		__type *sentinal;		\
		__type *cursor;			\
		__type *vec;			\
	}

/* iterators */
#define TS_VEC_FOREACH(__var, __head)	\
	for (__var = __head->vec;	\
	     __var <= __head->sentinal;	\
	     __var++)

#define TS_VEC_RFOREACH(__var, __head)	\
	for (__var = __head->sentinal;	\
	     __var >= __head->vec;	\
	     __var--)

/* Memory functions */
#define TS_VEC_NEW(__name, __type)				\
	struct __name *						\
	__name##_new(size_t count){				\
		struct __name *ptr;				\
		ptr = calloc(1, sizeof(struct __name));		\
		if (ptr == NULL) {				\
			errno = ENOMEM;				\
			return NULL;				\
		}						\
		ptr->vec = calloc(count, sizeof(__type));	\
		if (ptr->vec == NULL) {				\
			free(ptr);				\
			errno = ENOMEM;				\
			return NULL;				\
		}						\
		ptr->sentinal = &ptr->vec[count - 1];		\
		ptr->cursor = 0;				\
		return ptr;					\
	}

#define TS_VEC_FREE(__name)					\
	void							\
	__name##_free(struct __name *ptr){			\
		if (ptr == NULL)				\
			return;					\
		free(ptr->vec);					\
		free(ptr);					\
		ptr = NULL;					\
	}

#define TS_VEC_RESIZE(__name, __type)					\
	struct __name *							\
	__name##_resize(struct __name *ptr, size_t count){		\
		size_t old_count, old_cursor;				\
		if (ptr == NULL) {					\
			errno = EACCES;					\
			return NULL;					\
		}							\
		old_count = (ptr->sentinal - ptr->vec) / sizeof(__type);\
		old_cursor = (ptr->cursor - ptr->vec) / sizeof(__type);	\
		ptr->vec = realloc(ptr->vec, count * sizeof(__type));	\
		if (ptr->vec == NULL) {					\
			errno = ENOMEM;					\
			return NULL;					\
		}							\
		ptr->sentinal = &ptr->vec[count - 1];			\
		if (old_count < count) {				\
			memset(&ptr->vec[old_count],			\
			    '\0', (count - old_count)			\
			    * sizeof(__type));				\
		}							\
		if (old_cursor > count) {				\
			ptr->cursor = ptr->sentinal;			\
		}							\
		else {							\
			ptr->cursor = &ptr->vec[old_cursor];		\
		}							\
		return ptr;						\
	}

#endif /* TWINSHADOW_VEC_H */
