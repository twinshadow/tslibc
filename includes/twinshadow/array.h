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
#include "twinshadow/error.h"
#include "twinshadow/std.h"

struct ts_array_s {
	size_t size;
	void *head;
	void *tail;
};

/* iterators */
#define __TS_ARRAY_FOREACH(__var, __head, __offset, __size, __limit) \
for (__var  = PTR_OFFSET((__head)->head, __offset, (__head)->size);  \
     (void*)__var <= __limit;                                        \
     __var += __size)

#define __TS_ARRAY_RFOREACH(__var, __head, __offset, __size, __limit) \
for (__var  = PTR_OFFSET((__head)->tail, -__offset, (__head)->size);  \
     (void*)__var >= __limit;                                         \
     __var -= __size)

#define TS_ARRAY_FOREACH_OFFSET_2D(__var, __head, __offset) \
__TS_ARRAY_FOREACH(__var, __head, __offset, 1, (__head)->tail)

#define TS_ARRAY_RFOREACH_OFFSET_2D(__var, __head, __offset) \
__TS_ARRAY_RFOREACH(__var, __head, __offset, 1, (__head)->head)

#define TS_ARRAY_FOREACH_OFFSET(__var, __head, __offset) \
__TS_ARRAY_FOREACH(__var, __head, __offset, (__head)->size, (__head)->tail)

#define TS_ARRAY_RFOREACH_OFFSET(__var, __head, __offset) \
__TS_ARRAY_RFOREACH(__var, __head, __offset, (__head)->size, (__head)->head)

#define TS_ARRAY_FOREACH_2D(__var, __head) \
__TS_ARRAY_FOREACH(__var, __head, 0, 1, (__head)->tail)

#define TS_ARRAY_RFOREACH_2D(__var, __head) \
__TS_ARRAY_RFOREACH(__var, __head, 0, 1, (__head)->head)

#define TS_ARRAY_FOREACH(__var, __head) \
__TS_ARRAY_FOREACH(__var, __head, 0, (__head)->size, (__head)->tail)

#define TS_ARRAY_RFOREACH(__var, __head) \
__TS_ARRAY_RFOREACH(__var, __head, 0, (__head)->size, (__head)->head)

#define TS_ERR_ARRAY_IS_VALID(__array) do { \
	TS_ERR_NULL(__array);               \
	TS_ERR_NULL((__array)->head);       \
	TS_ERR_NULL((__array)->tail);       \
	TS_ERR_ZERO((__array)->size);       \
} while (0)

struct ts_array_s *ts_array_new(size_t count, size_t size);
void ts_array_free(struct ts_array_s **head);
void ts_array_resize(struct ts_array_s *head, size_t count, size_t size);
void *ts_array_get(struct ts_array_s *head, size_t offset);

#endif /* TWINSHADOW_ARRAY_H */
