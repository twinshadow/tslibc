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

#include <string.h>
#include "twinshadow/net.h"
#include <sys/un.h>
#include <stdlib.h>

struct addrinfo*
ts_getaddr_unix(const char *path,
              const int socktype,
	      const int flags,
	      const int proto) {
	struct addrinfo *adr;
	struct sockaddr_un *sun;

	sun = calloc(1, sizeof(struct sockaddr_un));
	TS_ERR_NULL(sun);
	sun->sun_family = AF_UNIX;

	TS_ERR_STR(path);
	/* ensure the path fits in the struct */
	if (strlen(path) >= sizeof sun->sun_path) {
		goto error;
	}
	strcpy(sun->sun_path, path);

	adr = calloc(1, sizeof(struct addrinfo));
	TS_ERR_NULL(adr);
	adr->ai_family = AF_UNIX;
	adr->ai_socktype = socktype;
	adr->ai_flags = flags;
	adr->ai_protocol = proto;
	adr->ai_addrlen = SUN_LEN(sun);
	adr->ai_addr = (struct sockaddr *)sun;
	goto out;

error:
	if (sun != NULL)
		free(sun);
	if (adr != NULL)
		free(adr);
	adr = NULL;
out:
	return (adr);
}

/*
 * Return the addrinfo from the selected options
 */
struct addrinfo*
ts_getaddr(const char *address,
	const char *port,
	const int family,
	const int socktype,
	const int flags,
	const int proto)
{
	struct addrinfo hint;
	struct addrinfo *adr = NULL;
	int err;

	if (family == AF_UNIX) {
		return ts_getaddr_unix(address, socktype, flags, proto);
	}

	memset(&hint, 0, sizeof(hint));
	hint.ai_family = family;
	hint.ai_socktype = socktype;
	hint.ai_flags = flags;
	hint.ai_protocol = proto;

	err = getaddrinfo(address, port, &hint, &adr);
	if (err != 0)
		goto error;
	else
		goto out;

error:
	perror(gai_strerror(err));
	if (adr != NULL) {
		/* glibc 2.6 freeaddrinfo will segfault if ai_addr is NULL */
		if (adr->ai_addr != NULL)
			freeaddrinfo(adr);
		else
			free(adr);
	}
	adr = NULL;
out:
	return (adr);
}
