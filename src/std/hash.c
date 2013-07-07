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

#include "twinshadow/hash.h"

TS_VEC_NEW(ts_tabmap, struct ts_table_item_s *);
TS_VEC_FREE(ts_tabmap);

ts_table_key_t ts_oat_hash(void *key, size_t len)
{
	unsigned char *chr = key;
	ts_table_key_t hash = 0;
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

struct ts_table_s *
ts_table_new(size_t logsize)
{
	struct ts_table_s *buf;
	size_t count;

	count = 1 << logsize;

	buf = calloc(1, sizeof(struct ts_table_s));
	if (buf == NULL)
	{
		errno = ENOMEM;
		return NULL;
	}

	buf->table = ts_tabmap_new(count);
	if (buf->table == NULL)
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
ts_table_free(struct ts_table_s *ptr)
{
	struct ts_table_item_s **item;
	if (ptr == NULL)
		return;

	TS_VEC_FOREACH(item, ptr->table) {
		if (*item != NULL)
			free(*item);
	}
	ts_tabmap_free(ptr->table);
	free(ptr);
	ptr = NULL;
}

struct ts_table_item_s **
ts_table_lookup(void *ptr, size_t len, struct ts_table_s *table)
{
	ts_table_key_t key;
	struct ts_table_item_s **item;

	if (table->hash == NULL)
		return (NULL);

	key = table->hash(ptr, len);
	item = &table->table->vec[key % table->len];
	if (*item == NULL)
		return (item);
	if (key == (*item)->hash)
		return (item);

	for (item = &((*item)->next); item != NULL; item = &((*item)->next)) {
		if (key == (*item)->hash)
			return (item);
	}

	return (item);
}

struct ts_table_item_s *
ts_table_item_new(void *ptr, size_t len, struct ts_table_s *table)
{
	struct ts_table_item_s *buf;
	buf = calloc(1, sizeof(struct ts_table_item_s));
	if (buf == NULL) {
		errno = ENOMEM;
		return (NULL);
	}
	buf->hash = table->hash(ptr, len);
	return (buf);
}

struct ts_table_item_s *
ts_table_add(void *ptr, size_t len, struct ts_table_s *table)
{
	struct ts_table_item_s **item;
	item = ts_table_lookup(ptr, len, table);
	if (*item != NULL)
		return (NULL);
	*item = ts_table_item_new(ptr, len, table);
	table->count++;
	return (*item);
}

struct ts_table_item_s *
ts_table_fetch(void *ptr, size_t len, struct ts_table_s *table)
{
	return (*ts_table_lookup(ptr, len, table));
}

void
ts_table_rem(void *ptr, size_t len, struct ts_table_s *table)
{
	struct ts_table_item_s **item;
	struct ts_table_item_s *next = NULL;
	item = ts_table_lookup(ptr, len, table);
	if (*item == NULL)
		return;
	if ((*item)->next != NULL)
		next = (*item)->next;
	free(*item);
	table->count--;
	*item = next;
}
