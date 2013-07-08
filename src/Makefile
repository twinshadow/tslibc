OBJ_NAME=	$(shell basename "${PWD}")

SRCS=		$(shell find \( -name '*.c' -not -name 'check_*' \) -printf '%P\n')
CHECK_SRCS=	$(wildcard check_*.c)
OBJS=		$(SRCS:.c=.o)
CHECK_OBJS=	$(CHECK_SRCS:.c=.co)

ifdef DEBUG
CFLAGS+=	-O0
endif

ifndef TOP
TOP=		../..
CFLAGS+=	-I${TOP}/includes -g -fPIC -Wall -Werror
PREFIX=		${TOP}/pkg
endif

${OBJ_NAME}.a : ${OBJS}
	ar -cvq $@ $^

${OBJ_NAME}.so: ${OBJS}
	${CC} ${CFLAGS} ${LDFLAGS} -shared -o $@ $^

%.o : %.c
	${CC} ${CFLAGS} -c -o $@ $^

%.co : %.c
	${CC} ${CFLAGS} -o $@ $^ ${OBJS} $(shell pkg-config --cflags --libs check)

all: ${OBJS}

check-build: ${OBJS} ${CHECK_OBJS}

check-run: check-build
	set ERROR_CHECK=0;			\
	for CHK in ${CHECK_OBJS}; do		\
		valgrind --suppressions=${TOP}/ts_valgrind.supp --leak-check=full --quiet ./$$CHK;\
		if [ "$$?" -ne 0 ]; then	\
			set ERROR_CHECK=1;	\
		fi;				\
	done;					\
	exit $$ERROR_CHECK

check: check-run

analyze:
	clang --analyze ${CFLAGS} ${SRCS}

install: ${OBJ_NAME}.a ${OBJ_NAME}.so
	mkdir -p ${PREFIX}/lib
	install -m0644 ${OBJ_NAME}.a ${PREFIX}/lib/${OBJ_NAME}.a
	install -m0755 ${OBJ_NAME}.so ${PREFIX}/lib/${OBJ_NAME}.so

clean:
	rm -f *.o *.co ${OBJ_NAME}.a ${OBJ_NAME}.so

.PHONY: clean check install
