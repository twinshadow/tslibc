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

#ifndef TWINSHADOW_MAP_H
#define TWINSHADOW_MAP_H

#include <errno.h>
#include <stdint.h>
#include <stdlib.h>

#include "twinshadow/hash.h"

typedef ts_hash_int_t ts_map_key_t;

/* Typed structures */

#define TS_MAP_ITEM(__name, __key, __value)	\
struct __name##_item {				\
	struct __name##_item *next;		\
	ts_map_key_t hash;			\
	__key *key;				\
	__value *value;				\
}

#define TS_MAP_HEAD(__name)			\
struct __name {					\
	ts_map_key_t (*hash)(void *ptr,		\
			     size_t len);	\
	struct __name##_item **sentinal;	\
	struct __name##_item **map;		\
	size_t logsize;				\
	size_t count;				\
	size_t len;				\
}

/* iterators */

#define TS_MAP_FOREACH(__var, __map)	\
for (__var = __head->map;		\
     __var <= __head->sentinal;		\
     __var++)				\
	for (;*__var != NULL;		\
	     *__var = (*__var)->next)

#define TS_MAP_RFOREACH(__var, __head)	\
for (__var = __head->sentinal;		\
     __var >= __head->map;		\
     __var--)				\
	for (;*__var != NULL;		\
	     *__var = (*__var)->next)


/* Memory functions */

#define TS_MAP_NEW(__name)			\
struct __name *					\
__name##_new(size_t logsize){			\
	struct __name *head;			\
	size_t count;				\
	count = 1 << logsize;			\
	if (count < 1)				\
		return NULL;			\
	head = calloc(1, sizeof(struct __name));\
	if (head == NULL) 			\
		return NULL;			\
	head->map = calloc(count,		\
	    sizeof(struct __name##_item *));	\
	if (head->map == NULL) {		\
		free(head);			\
		return NULL;			\
	}					\
	head->sentinal = &head->map[count - 1];	\
	head->logsize = logsize;		\
	head->len = count;			\
	head->hash = ts_hash_oat;		\
	return (head);				\
}

#define TS_MAP_FREE(__name, __free)		\
void						\
__name##_free(struct __name *head) {		\
	struct __name##_item **item;		\
	struct __name##_item *swap = NULL;	\
	if (head == NULL)			\
		return;				\
	for (item = head->map;			\
	     item <= head->sentinal;		\
	     item++)				\
		while (*item != NULL) {		\
			swap = *item;		\
			*item = (*item)->next;	\
			__free(swap);		\
		}				\
	free(head->map);			\
	free(head);				\
	head = NULL;				\
}

#define TS_MAP_RESIZE(__name)				\
void							\
__name##_resize(struct __name *head,			\
    size_t logsize) {					\
	size_t count;					\
	if (head == NULL)				\
		return;					\
	if (head->logsize >= logsize)			\
		return;					\
	count = 1 << logsize;				\
	if (count < 1)					\
		return;					\
	head->map = realloc(				\
	    count * sizeof(struct __name##_vec *));	\
	if (head->map == NULL)				\
		return;					\
	head->sentinal = head->map[count - 1];		\
	head->logsize = logsize;			\
	head->len = count;				\
}

/* Typed Manipulation Functions */

#define TS_MAP_LOOKUP(__name)				\
struct __name##_item **					\
__name##_lookup(void *ptr,				\
    size_t len,						\
    struct __name *head) {				\
	ts_map_key_t key;				\
	struct __name##_item **item;			\
	if (head->hash == NULL)				\
		return (NULL);				\
	key = head->hash(ptr, len);			\
	item = &head->map[key % head->len];		\
	if (*item == NULL)				\
		return (item);				\
	if (key == (*item)->hash)			\
		return (item);				\
	for (item = &((*item)->next);			\
	     *item != NULL;				\
	     item = &((*item)->next)) {			\
		if (key == (*item)->hash)		\
			return (item);			\
	}						\
	return (item);					\
}

#define TS_MAP_ADD(__name)					\
struct __name##_item *						\
__name##_add(void *ptr,						\
    size_t len,							\
    struct __name *head) {					\
	struct __name##_item **item;				\
	item = __name##_lookup(ptr, len, head);			\
	if (*item != NULL)					\
		return (NULL);					\
	*item = calloc(1, sizeof(struct __name##_item));	\
	if (*item == NULL)					\
		return (NULL);					\
	(*item)->hash = head->hash(ptr, len);			\
	(*item)->key = ptr;					\
	head->count++;						\
	return (*item);						\
}

#define TS_MAP_REM(__name)				\
void							\
__name##_rem(void *ptr,					\
    size_t len,						\
    struct __name *head) {				\
	struct __name##_item **item, *next;		\
	if (head == NULL)				\
		return;					\
	item = __name##_lookup(ptr, len, head);		\
	if (*item == NULL)				\
		return;					\
	next = (*item)->next;				\
	free(*item);					\
	head->count--;					\
	*item = next;					\
}

#define TS_MAP_PROTOTYPES(__name, __key, __value)			\
TS_MAP_ITEM(__name, __key, __value);					\
TS_MAP_HEAD(__name);							\
struct __name * __name##_new(size_t logsize);				\
void __name##_free(struct __name *head);				\
void __name##_resize(struct __name *head, size_t logsize);		\
struct __name##_item ** __name##_lookup(void *ptr, size_t len, struct __name *head);\
struct __name##_item * __name##_add(void *ptr, size_t len, struct __name *head);\
void __name##_rem(void *ptr, size_t len, struct __name *head);

#endif /* TWINSHADOW_HASH_H */