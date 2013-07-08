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

#include "twinshadow/vec.h"

TS_VEC_HEAD(ts_mapvec, struct ts_map_item_s *);

typedef uint16_t ts_map_key_t;

struct ts_map_item_s {
	struct ts_map_item_s *next;
	ts_map_key_t hash;
	void *ptr;
};

struct ts_map_s {
	ts_map_key_t (*hash)(void *ptr, size_t len);
	struct ts_mapvec *map;
	size_t logsize;
	size_t count;
	size_t len;
};

/* Memory */
struct ts_map_item_s * ts_map_item_new(void *ptr, size_t len, struct ts_map_s *map);
struct ts_map_s * ts_map_new(size_t logsize);
void ts_map_free(struct ts_map_s *map);
void ts_map_init(struct ts_map_s *map);
void ts_map_resize(struct ts_map_s *map);

/* Manipulation */
struct ts_map_item_s ** ts_map_lookup(void *ptr, size_t len, struct ts_map_s *map);
struct ts_map_item_s * ts_map_fetch(void *ptr, size_t len, struct ts_map_s *map);
struct ts_map_item_s * ts_map_add(void *ptr, size_t len, struct ts_map_s *map);
void ts_map_rem(void *ptr, size_t len, struct ts_map_s *map);

#endif /* TWINSHADOW_HASH_H */
