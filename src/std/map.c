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

#include "twinshadow/map.h"

TS_VEC_NEW(ts_mapvec, struct ts_map_item_s *);
TS_VEC_FREE(ts_mapvec);

ts_map_key_t ts_oat_hash(void *key, size_t len)
{
	unsigned char *chr = key;
	ts_map_key_t hash = 0;
	size_t idx;

	for (idx = 0; idx < len; idx++) {
		hash += chr[idx];
		hash += (hash << 10);
		hash ^= (hash >> 6);
	}

	hash += (hash << 3);
	hash ^= (hash >> 11);
	hash += (hash << 15);

	return hash;
}

struct ts_map_s *
ts_map_new(size_t logsize)
{
	struct ts_map_s *buf;
	size_t count;

	count = 1 << logsize;

	buf = calloc(1, sizeof(struct ts_map_s));
	if (buf == NULL)
	{
		errno = ENOMEM;
		return NULL;
	}

	buf->map = ts_mapvec_new(count);
	if (buf->map == NULL)
	{
		free(buf);
		errno = ENOMEM;
		return NULL;
	}

	buf->logsize = logsize;
	buf->len = count;
	buf->count = 0;

	buf->hash = ts_oat_hash;

	return (buf);
}

void
ts_map_free(struct ts_map_s *ptr)
{
	struct ts_map_item_s **item, *next, *swap;
	if (ptr == NULL)
		return;

	TS_VEC_FOREACH(item, ptr->map) {
		for (next = *item; next != NULL; next = swap){
			swap = next->next;
			if (next != NULL)
				free(next);
		}
	}
	ts_mapvec_free(ptr->map);
	free(ptr);
	ptr = NULL;
}

struct ts_map_item_s **
ts_map_lookup(void *ptr, size_t len, struct ts_map_s *map)
{
	ts_map_key_t key;
	struct ts_map_item_s **item;

	if (map->hash == NULL)
		return (NULL);

	key = map->hash(ptr, len);
	item = &map->map->vec[key % map->len];
	if (*item == NULL)
		return (item);
	if (key == (*item)->hash)
		return (item);

	for (item = &((*item)->next); *item != NULL; item = &((*item)->next)) {
		if (key == (*item)->hash)
			return (item);
	}

	return (item);
}

struct ts_map_item_s *
ts_map_item_new(void *ptr, size_t len, struct ts_map_s *map)
{
	struct ts_map_item_s *buf;
	buf = calloc(1, sizeof(struct ts_map_item_s));
	if (buf == NULL) {
		errno = ENOMEM;
		return (NULL);
	}
	buf->hash = map->hash(ptr, len);
	return (buf);
}

struct ts_map_item_s *
ts_map_add(void *ptr, size_t len, struct ts_map_s *map)
{
	struct ts_map_item_s **item;
	item = ts_map_lookup(ptr, len, map);
	if (*item != NULL)
		return (NULL);
	*item = ts_map_item_new(ptr, len, map);
	map->count++;
	return (*item);
}

struct ts_map_item_s *
ts_map_fetch(void *ptr, size_t len, struct ts_map_s *map)
{
	return (*ts_map_lookup(ptr, len, map));
}

void
ts_map_rem(void *ptr, size_t len, struct ts_map_s *map)
{
	struct ts_map_item_s **item;
	struct ts_map_item_s *next;
	item = ts_map_lookup(ptr, len, map);
	if (*item == NULL)
		return;
	next = (*item)->next;
	free(*item);
	map->count--;
	*item = next;
}
