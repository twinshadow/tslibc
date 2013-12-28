#ifndef TWINSHADOW_DEBUG_H
#define TWINSHADOW_DEBUG_H

#include <stdio.h>
#include <errno.h>
#include <string.h>

#ifdef TS_SYSLOG
#include <syslog.h>
#endif /* TS_SYSLOG */

#ifndef TS_NODEBUG

#define TS_DEBUG(__msg, ...)                         \
	fprintf(stderr, "DEBUG: %s:%d: " __msg "\n", \
	    __FILE__, __LINE__, ##__VA_ARGS__)
#else

#define TS_DEBUG(__msg, ...)

#endif /* TS_NODEBUG */

#define STRERR (errno ? strerror(errno) : "None")

#ifdef TS_SYSLOG

#define TS_LOG_ERR(__msg, ...)                              \
	syslog(LOG_ERR, "%s:%u: errno %d, %s: " __msg "\n", \
	   __FILE__, __LINE__, errno, ERRSTR, ##__VA_ARGS__)

#define TS_LOG_WARN(__msg, ...)                              \
	syslog(LOG_WARN, "%s:%u: errno %d, %s: " __msg "\n", \
	    __FILE__, __LINE__, errno, STRERR, ##__VA_ARGS__)

#define TS_LOG_INFO(__msg, ...)                \
	syslog(LOG_INFO, "%s:%u: " __msg "\n", \
	    __FILE__, __LINE__, ##__VA_ARGS__)

#else /* ifdef TS_SYSLOG */

#define TS_LOG_ERR(__msg, ...)                                     \
	fprintf(stderr, "ERROR: %s:%u: errno %d, %s: " __msg "\n", \
	    __FILE__, __LINE__, errno, STRERR, ##__VA_ARGS__)

#define TS_LOG_WARN(__msg, ...)                                   \
	fprintf(stderr, "WARN: %s:%u: errno %d, %s: " __msg "\n", \
	    __FILE__, __LINE__, errno, STRERR, ##__VA_ARGS__)

#define TS_LOG_INFO(__msg, ...)                     \
	fprintf(stderr, "INFO: %s:%u: " __msg "\n", \
	    __FILE__, __LINE__, ##__VA_ARGS__)

#endif /* ifdef TS_SYSLOG */

#define TS_CHECK(__cond, __msg, ...)              \
	if (!(__cond)) {                          \
		TS_LOG_ERR(__msg, ##__VA_ARGS__); \
		errno=0;                          \
		goto error;                       \
	}

#define TS_CHECK_DEBUG(__cond, __msg, ...)      \
	if (!(__cond)) {                        \
		TS_DEBUG(__msg, ##__VA_ARGS__); \
		errno=0;                        \
		goto error;                     \
	}

#define TS_ERR_NULL(__val, ...) \
	TS_CHECK(__val, "Value should not be NULL")

#define TS_ERR_ZERO(__val, ...) \
	TS_CHECK(__val != 0, "Value should not be zero")

#define TS_DEBUG_PTR(__ptr) TS_DEBUG("%08lx", (long int)__ptr)

#endif /* TWINSHADOW_DEBUG_H */
