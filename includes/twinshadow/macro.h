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

#ifndef TWINSHADOW_MACROS_H
#define TWINSHADOW_MACROS_H

#define MATCH_SIGNEDNESS(X, Y) ((X > 0) ? ((Y > 0) ? Y : -Y) : ((Y < 0) ? Y : -Y))
#define POSITIVE(X) ((X > 0) ? X : -X)
#define NEGATIVE(X) ((X < 0) ? X : -X)
#define LARGEST_DIFFERENCE(X, Y) ((X - Y > Y) ? X - Y : Y)
#define SMALLEST_DIFFERENCE(X, Y) ((X - Y < Y) ? X - Y : Y)

#define LENGTH(X) (sizeof(X) / sizeof(X[0]))
#define UNLESS(X) if (!(X))
#define SWAP(X, Y, Z) { Z = X; \
			X = Y; \
			Y = Z; }
#define ROTATE_RIGHT(W, X, Y, Z) { SWAP(W, X, Z); \
				   SWAP(X, Y, Z); \
				   SWAP(Y, W, Z); }
#define ROTATE_LEFT(W, X, Y, Z) ROTATE_RIGHT(X, Y, W, Z);

#define REPEAT(__idx, __comp) \
	for (__idx = 0; __idx < __comp; __idx++)

#define ITERATE(__idx, __comp) \
	for (__idx = 0; __idx; __idx++)

/* This takes ridiculous offset amounts and reduces them to the actual amount
 * needed to rotate the appropriate amount. It then converts negative values
 * into their positive equivalents, for simplicity.
*/
#define REDUCE_OFFSET(__offset, __len)	{			\
	if (__offset > __len)					\
		__offset %= MATCH_SIGNEDNESS(__offset, __len);	\
	if (__offset < 0)					\
		__offset = __len - -__offset;			\
}

#endif /* TWINSHADOW_MACROS_H */
