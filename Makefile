TOP=		$(realpath ${PWD})
SUBDIRS=	$(shell find src -maxdepth 1 -mindepth 1 -type d -exec basename \{\} \;)
PREFIX=		${TOP}/pkg

INCLUDES=	-I${PWD}/includes

CFLAGS+=	-g -fPIC -Wall -Werror ${INCLUDES}

CHECK_TARGETS=		$(addprefix check-,${SUBDIRS})
ANALYZE_TARGETS=	$(addprefix analyze-,${SUBDIRS})
CLEAN_TARGETS=		$(addprefix clean-,${SUBDIRS})
INSTALL_TARGETS=	$(addprefix install-,${SUBDIRS})

all: ${SUBDIRS}

${SUBDIRS}:
	cd src/$@ && ${MAKE} CFLAGS="${CFLAGS}" TOP="${TOP}"

${CHECK_TARGETS}:
	cd src/$(shell echo $@ | cut -d'-' -f2) && ${MAKE} CFLAGS="${CFLAGS}" TOP="${TOP}" check

${ANALYZE_TARGETS}:
	cd src/$(shell echo $@ | cut -d'-' -f2) && ${MAKE} CFLAGS="${CFLAGS}" TOP="${TOP}" analyze

${CLEAN_TARGETS}:
	cd src/$(shell echo $@ | cut -d'-' -f2) && ${MAKE} clean

${INSTALL_TARGETS}:
	mkdir -p "${TOP}/obj"
	cd src/$(shell echo $@ | cut -d'-' -f2) && ${MAKE} CFLAGS="${CFLAGS}" TOP="${TOP}" PREFIX="${PREFIX}" install

all: ${SUBDIRS}

check: ${CHECK_TARGETS}

analyze: ${ANALYZE_TARGETS}

clean: ${CLEAN_TARGETS}
	rm -rf pkg
	rm -rf doc

install: ${INSTALL_TARGETS}
	mkdir -p ${PREFIX}/include
	mkdir -p ${PREFIX}/lib
	cp -r includes/twinshadow ${PREFIX}/include
	ar -cvq ${PREFIX}/lib/twinshadow.a $(shell find src -name '*.o')
	${CC} ${CFLAGS} ${LDFLAGS} -shared -o ${PREFIX}/lib/twinshdow.so $(shell find src -name '*.o')

docs:
	cldoc ${CFLAGS} -- \
		--language=c --report \
		--merge=${TOP}/docs/_static \
		--output=${TOP}/docs \
		$(shell find \( -name '*.c' -or -name '*.h' \) -not -name 'check_*')

everything: clean analyze check all docs install

.PHONY: clean docs
