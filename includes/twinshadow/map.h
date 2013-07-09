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
#include "twinshadow/vec.h"

typedef ts_hash_int_t ts_map_key_t;

/* Typed structures */

#define TS_MAP_ITEM(__item, __proto)	\
struct __item {				\
	struct __item *next;		\
	ts_map_key_t hash;		\
	__proto;			\
}

#define TS_MAP_VEC(__vec, __item)	\
TS_VEC_HEAD(__vec, struct __item *);

#define TS_MAP_HEAD(__map, __vec)			\
struct __map {						\
	ts_map_key_t (*hash)(void *ptr, size_t len);	\
	struct __vec *map;				\
	size_t logsize;					\
	size_t count;					\
	size_t len;					\
}

#define TS_MAP_TYPES(__name, __proto)		\
TS_MAP_ITEM(__name##_item, __proto);		\
TS_MAP_VEC(__name##_vec, __name##_item);	\
TS_MAP_HEAD(__name, __name##_vec);

/* Typed Memory Functions */

#define TS_MAP_NEW(__map, __vec)		\
struct __map *					\
__map##_new(size_t logsize){			\
	struct __map *buf;			\
	size_t count;				\
	count = 1 << logsize;			\
	if (count < 1)				\
		return NULL;			\
	buf = calloc(1, sizeof(struct __map));	\
	if (buf == NULL) 			\
		return NULL;			\
	buf->map = __vec##_new(count);		\
	if (buf->map == NULL) {			\
		free(buf);			\
		return NULL;			\
	}					\
	buf->logsize = logsize;			\
	buf->len = count;			\
	buf->hash = ts_hash_oat;		\
	return (buf);				\
}

#define TS_MAP_FREE(__map, __vec)		\
void						\
__map##_free(struct __map *map) {		\
	struct __map##_item **item, *next;	\
	if (map == NULL)			\
		return;				\
	TS_VEC_FOREACH(item, map->map) {	\
		for (;*item != NULL;		\
		     *item = next) {		\
			next = (*item)->next;	\
			free(*item);		\
		}				\
	}					\
	__vec##_free(map->map);			\
	free(map);				\
	map = NULL;				\
}

#define TS_MAP_RESIZE(__map, __vec)			\
void							\
__map##_resize(struct __map *map, size_t logsize) {	\
	size_t count;					\
	if (map == NULL)				\
		return;					\
	if (map->logsize >= logsize)			\
		return;					\
	count = 1 << logsize;				\
	if (count < 1)					\
		return;					\
	map->map = __vec##_resize(count);		\
	if (map->map == NULL)				\
		return;					\
	map->logsize = logsize;				\
	map->len = count;				\
}

/* Typed Manipulation Functions */

#define TS_MAP_LOOKUP(__map, __item)				\
struct __item **						\
__map##_lookup(void *ptr, size_t len, struct __map *map) {	\
	ts_map_key_t key;					\
	struct __item **item;					\
	if (map->hash == NULL)					\
		return (NULL);					\
	key = map->hash(ptr, len);				\
	item = &map->map->vec[key % map->len];			\
	if (*item == NULL)					\
		return (item);					\
	if (key == (*item)->hash)				\
		return (item);					\
	for (item = &((*item)->next);				\
	     *item != NULL;					\
	     item = &((*item)->next)) {				\
		if (key == (*item)->hash)			\
			return (item);				\
	}							\
	return (item);						\
}

#define TS_MAP_ADD(__map, __item)				\
struct __item *							\
__map##_add(void *ptr, size_t len, struct __map *map) {		\
	struct __item **item;					\
	item = __map##_lookup(ptr, len, map);			\
	if (*item != NULL)					\
		return (NULL);					\
	*item = calloc(1, sizeof(struct __item));		\
	if (*item == NULL)					\
		return (NULL);					\
	(*item)->hash = map->hash(ptr, len);			\
	map->count++;						\
	return (*item);						\
}

#define TS_MAP_REM(__map, __item)				\
void								\
__map##_rem(void *ptr, size_t len, struct __map *map) {		\
	struct __item **item, *next;				\
	if (map == NULL)					\
		return;						\
	item = __map##_lookup(ptr, len, map);			\
	if (*item == NULL)					\
		return;						\
	next = (*item)->next;					\
	free(*item);						\
	map->count--;						\
	*item = next;						\
}

#define TS_MAP_PROTOTYPES(__map, __vec, __item, __proto)	\
TS_MAP_ITEM(__item, __proto);						\
TS_MAP_VEC(__vec, __item);						\
TS_MAP_HEAD(__map, __vec);						\
struct __map * __map##_new(size_t logsize);				\
void __map##_free(struct __map *map);					\
void __map##_resize(struct __map *map, size_t logsize);		\
struct __item ** __map##_lookup(void *ptr, size_t len, struct __map *map);\
struct __item * __map##_add(void *ptr, size_t len, struct __map *map);	\
void __map##_rem(void *ptr, size_t len, struct __map *map);

#endif /* TWINSHADOW_HASH_H */
