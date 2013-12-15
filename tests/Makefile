TOP=		..

PROGS=		runner
CFLAGS+=	-I${TOP}/includes -g -fPIC -Wall -Werror
OBJS:=		$(shell find ${TOP}/src -name '*.o' -or -name '*.co')
LDFLAGS+=	-lcheck

ifdef WITH_COVERAGE
CFLAGS+=	-fprofile-arcs -ftest-coverage
LDFLAGS+=	-lgcov -fprofile-arcs -ftest-coverage
endif

all: build-runner ${PROGS}
build-runner:
	cd ${TOP} && ${MAKE} all checko

runner: runner.o ${OBJS}
clean:
	@rm -rf *.o ${PROGS}

.PHONY: build-runner clean
