TOP=		../..

# ____
#|  _ \ _ __ ___   __ _ _ __ __ _ _ __ ___  ___
#| |_) | '__/ _ \ / _` | '__/ _` | '_ ` _ \/ __|
#|  __/| | | (_) | (_| | | | (_| | | | | | \__ \
#|_|   |_|  \___/ \__, |_|  \__,_|_| |_| |_|___/
#                 |___/

AR:=		ar
CHMOD:=		chmod
INSTALL:=	install
MKDIR:=		mkdir
RM:=		rm

PKG_NAME=	$(shell basename "${PWD}")

SRCS=		$(shell find -maxdepth 1 \( -name '*.c' -and -not -name 'check_*' \) -printf '%P\n')
OBJS=		$(SRCS:.c=.o)

CHECK_SRCS:=	$(wildcard check_*.c)
CHECK_OBJS:=	$(CHECK_SRCS:.c=.co)
CHECK_BINS:=	$(CHECK_SRCS:.c=)
CHECK_PKG:=	check_${PKG_NAME}
CHECKSUPP_SRCS:=	$(wildcard checksupp_*.c)
CHECKSUPP_OBJS:=	$(CHECKSUPP_SRCS:.c=.co)

# _____ _
#|  ___| | __ _  __ _ ___
#| |_  | |/ _` |/ _` / __|
#|  _| | | (_| | (_| \__ \
#|_|   |_|\__,_|\__, |___/
#               |___/
#
ifdef DEBUG
CFLAGS+=	-O0
endif

ifdef WITH_COVERAGE
COV_CFLAGS+=	-fprofile-arcs -ftest-coverage
LDFLAGS+=	-lgcov -fprofile-arcs -ftest-coverage
endif

CFLAGS+=	-I${TOP}/includes -g -fPIC -Wall -Werror
prefix=		${TOP}/pkg
exec_prefix=	${prefix}
libdir=		${prefix}/lib
includedir=	${prefix}/include
checklibs=	$(shell pkg-config --libs check)

# _____                    _       _
#|_   _|__ _ __ ___  _ __ | | __ _| |_ ___  ___
#  | |/ _ \ '_ ` _ \| '_ \| |/ _` | __/ _ \/ __|
#  | |  __/ | | | | | |_) | | (_| | ||  __/\__ \
#  |_|\___|_| |_| |_| .__/|_|\__,_|\__\___||___/
#                   |_|

%.a: ${OBJS}
	${AR} -cvq $@ $^

%.so: ${OBJS}
	${CC} ${CFLAGS} ${LDFLAGS} -shared -o $@ $^

%.co : %.c
	${CC} ${CFLAGS} -c -o $@ $^

%.o: %.c
	${CC} ${CFLAGS} ${COV_CFLAGS} -c -o $@ $^

${CHECK_BINS}: ${CHECKSUPP_OBJS}
	CHK_SRC=$(@:=.c); \
	FUN_OBJ=$(@:check_%=%.c); \
	${MAKE} $$FUN_OBJ; \
	${CC} ${CFLAGS} ${COV_CFLAGS} -Wno-unused-function -DCHECK_STANDALONE ${LDFLAGS} ${checklibs} -o $@ $$CHK_SRC $$FUN_OBJ $^

# _____                    _
#|_   _|_ _ _ __ __ _  ___| |_ ___
#  | |/ _` | '__/ _` |/ _ \ __/ __|
#  | | (_| | | | (_| |  __/ |_\__ \
#  |_|\__,_|_|  \__, |\___|\__|___/
#               |___/

build: ${OBJS}

all: build

checko: ${CHECK_OBJS}

check-build: ${CHECK_BINS}

check-run: check-build
ifdef VALGRIND
	@for CHK in ${CHECK_BINS}; do \
	valgrind                                   \
	    --suppressions=${TOP}/ts_valgrind.supp \
	    --leak-check=full                      \
	    --quiet                                \
	    ./$$CHK;\
	done;
else
	@for CHK in ${CHECK_BINS}; do \
	    ./$$CHK;\
	done;
endif

check: check-run

analyze:
	@clang --analyze ${CFLAGS} ${SRCS}

install: ${PKG_NAME}.a ${PKG_NAME}.so
	${MKDIR} -p ${libdir}
	${INSTALL} -m0644 ${PKG_NAME}.a ${libdir}/${PKG_NAME}.a
	${INSTALL} -m0755 ${PKG_NAME}.so ${libdir}/${PKG_NAME}.so

clean:
	${RM} -f *.o *.co *.gcda *.gcno *.gcov *.check *.plist ${PKG_NAME}.a ${PKG_NAME}.so ${CHECK_BINS}

.PHONY: analyze clean install
