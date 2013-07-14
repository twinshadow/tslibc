ifndef TOP
TOP=		../..
endif
include ${TOP}/mk/Makefile.pre

OBJ_NAME=	$(shell basename "${PWD}")

SRCS=		$(shell find \( -name '*.c' -not -name 'check_*' \) -printf '%P\n')
OBJS=		$(SRCS:.c=.o)

include ${TOP}/mk/Makefile.check

${OBJ_NAME}.a: ${OBJS}
	ar -cvq $@ $^

${OBJ_NAME}.so: ${OBJS}
	${CC} ${CFLAGS} ${LDFLAGS} -shared -o $@ $^

%.o : %.c
	${CC} ${CFLAGS} -c -o $@ $^

all: ${OBJS}

analyze:
	clang --analyze ${CFLAGS} ${SRCS}

install: ${OBJ_NAME}.a ${OBJ_NAME}.so
	${MKDIR} -p ${libdir}
	${INSTALL} -m0644 ${OBJ_NAME}.a ${libdir}/${OBJ_NAME}.a
	${INSTALL} -m0755 ${OBJ_NAME}.so ${libdir}/${OBJ_NAME}.so

*.co: ${OBJS}

clean:
	${RM} -f *.o *.co *.plist ${OBJ_NAME}.a ${OBJ_NAME}.so

.PHONY: clean install
