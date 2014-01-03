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

#ifndef TWINSHADOW_STRING_H
#define TWINSHADOW_STRING_H

#include <string.h>
#include <twinshadow/error.h>
#include <twinshadow/macro.h>

/* ts_strrtok_r

   TwinShadow string reverse-tokenizer (re-entrant)
*/
char *ts_strrtok_r(char *str, const char *delim, char **saveptr);
char *ts_strrtok(char *str, const char *delim);

void ts_strnstrip(char *str, size_t len);
void ts_strstrip(char *str);

void ts_memshift(int offset, void *ptr, size_t len);
void ts_strshift(int offset, char *str);

char *ts_strndup(const char *src, size_t len);
char *ts_strdup(const char *src);

void ts_strnrev(char *str, size_t len);
void ts_strrev(char *str);

void ts_strnrot(int offset, char *str, size_t len);
void ts_strrot(int offset, char *str);

#endif /* TWINSHADOW_STRING_H */
