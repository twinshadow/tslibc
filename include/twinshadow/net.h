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

#ifndef TWINSHADOW_NET_H
#define TWINSHADOW_NET_H

#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "twinshadow/error.h"

#define INADDR(X) {(X->sa_family == AF_INET) ? \
			(((struct sockaddr_in*)X)->sin_addr) : \
			(((struct sockaddr_in6*)X)->sin6_addr)}

#define TS_SOCK_OPEN(__addrinfo, __fd) do {       \
	__fd = socket((__addrinfo)->ai_family,    \
	              (__addrinfo)->ai_socktype,  \
	              (__addrinfo)->ai_protocol); \
	TS_ERR_FD(__fd);                          \
} while (0)

#define TS_SOCK_CLOSE(__fd) do { \
	if (__fd >= 0) {         \
		close(__fd);     \
	}                        \
	__fd = -1;               \
} while (0)

#define TS_SOCK_CONNECT(__addrinfo, __fd) do {                                     \
	if(connect(__fd, (__addrinfo)->ai_addr, (__addrinfo)->ai_addrlen) == -1) { \
		perror("connect");                                                 \
		goto error;                                                        \
	}                                                                          \
} while (0)

#define TS_SOCK_BIND(__addrinfo, __fd) do {                                     \
	if(bind(__fd, (__addrinfo)->ai_addr, (__addrinfo)->ai_addrlen) == -1) { \
		perror("bind");                                                 \
		goto error;                                                     \
	}                                                                       \
} while (0)

#define TS_GAI_TCPIP(__addr, __port) \
	ts_getaddr(__addr, __port, AF_INET, SOCK_STREAM, 0, 0)

int soconnect(struct addrinfo *ad);
int sobind(struct addrinfo *ad);
struct addrinfo* ts_getaddr(const char *address,
			  const char *port,
			  const int family,
			  const int socktype,
			  const int flags,
			  const int proto);
struct addrinfo* ts_getaddr_unix(const char *path,
			         const int socktype,
			         const int flags,
			         const int proto);

#endif /* TWINSHADOW_NET_H */
