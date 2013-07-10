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

#ifndef TWINSHADOW_SET_H
#define TWINSHADOW_SET_H

#include "twinshadow/hash.h"
#include "twinshadow/tree.h"

#define TS_SET_ITEM(__name, __value)	\
struct __name##_item {			\
	ts_hash_int_t hash;		\
	__value *value;			\
}

#define TS_SET_VEC(__name) \
TS_VEC_HEAD(__name##_vec, struct __name##_item *);

#define TS_SET_HEAD(__name)				\
struct __name##_set {					\
	ts_map_key_t (*hash)(void *ptr, size_t len);	\
	struct __name##_vec *set;			\
	size_t count;					\
	size_t len;					\
}

#define TS_SET_FOREACH(__var, __set) \
TS_VEC_FOREACH(__var, __set##->set)

#define TS_SET_RFOREACH(__var, __set) \
TS_VEC_RFOREACH(__var, __set##->set)

#define TS_SET_NEW(__name)			\
struct __name *					\
__name##_new(size_t count) {			\
	struct __name *buf;			\
	buf = calloc(1, sizeof(struct __name));	\
	if (buf == NULL) 			\
		return NULL;			\
	buf->set = __name##_vec_new(count);	\
	if (buf->map == NULL) {			\
		free(buf);			\
		return NULL;			\
	}					\
	buf->len = count;			\
	return (buf);				\
}

#define TS_SET_FREE(__name, __free)	\
void					\
__name##_free(struct __name *set) {	\
	struct __name##_item *item;	\
	if (set == NULL)		\
		return;			\
	TS_SET_FOREACH(item, set) {	\
		if (item == NULL)	\
			continue;	\
		free(item);		\
	}				\
	__name##_vec_free(set->set);	\
	__free(set);			\
	set = NULL;			\
}

#define TS_SET_RESIZE(__name)				\
void							\
__name##_resize(struct __name *set, size_t count) {	\
	size_t count;					\
	if (set == NULL)				\
		return;					\
	if (count < 1 || set->len >= count)		\
		return;					\
	set->set = __name##_vec_resize(count);		\
	if (set->set == NULL)				\
		return;					\
	set->len = count;				\
}

/*
#define TS_SET_LOOKUP(__name)

#define TS_SET_ADD(__name)
#define TS_SET_REM(__name)
*/

#define TS_SET_PROTOTYPES(__name, __value)		\
	TS_SET_ITEM(__name##_item, __value);		\
	TS_SET_VEC(__name##_vec, __name##_item);	\
	TS_SET_HEAD(__name, __name##_vec);		\

#endif /* TWINSHADOW_SET_H */
