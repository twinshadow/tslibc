ifndef TOP
TOP=		../..
endif

include ${TOP}/mk/Makefile.pre

include ${TOP}/mk/Makefile.check

PKG_NAME=	$(shell basename "${PWD}")

SRCS=		$(shell find \( -name '*.c' -and -not -name 'check_*' \) -printf '%P\n')
OBJS=		$(SRCS:.c=.o)

all: ${OBJS}

${PKG_NAME}.check: ${OBJS}

analyze:
	clang --analyze ${CFLAGS} ${SRCS}

install: ${PKG_NAME}.a ${PKG_NAME}.so
	${MKDIR} -p ${libdir}
	${INSTALL} -m0644 ${PKG_NAME}.a ${libdir}/${PKG_NAME}.a
	${INSTALL} -m0755 ${PKG_NAME}.so ${libdir}/${PKG_NAME}.so

clean:
	${RM} -f *.o *.co *.check *.plist ${PKG_NAME}.a ${PKG_NAME}.so

.PHONY: analyze clean install
