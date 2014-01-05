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

#include "twinshadow/net.h"

/*
 * return socket connection from addrinfo
 */
int
soconnect(struct addrinfo *ad)
{
	int sock_fd = socket(ad->ai_family, ad->ai_socktype, ad->ai_protocol);

	if(sock_fd < 0)
		return sock_fd;

	if(connect(sock_fd, ad->ai_addr, ad->ai_addrlen) == -1)
	{
		close(sock_fd);
		return -1;
	}

	return sock_fd;
}

/*
 * return socket binding from addrinfo
 */
int
sobind(struct addrinfo *ad)
{
	int sock_fd = socket(ad->ai_family, ad->ai_socktype, ad->ai_protocol);

	if(sock_fd < 0)
		return sock_fd;

	if(bind(sock_fd, ad->ai_addr, ad->ai_addrlen) == -1)
	{
		close(sock_fd);
		return -1;
	}

	return sock_fd;
}
