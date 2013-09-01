TOP=		$(realpath ${PWD})
SUBDIRS=	$(shell find src -maxdepth 1 -mindepth 1 -type d -exec basename \{\} \;)

CHECK_TARGETS=		$(addprefix check-,${SUBDIRS})
ANALYZE_TARGETS=	$(addprefix analyze-,${SUBDIRS})
CLEAN_TARGETS=		$(addprefix clean-,${SUBDIRS})
INSTALL_TARGETS=	$(addprefix install-,${SUBDIRS})

${SUBDIRS}:
	cd src/$@ && ${MAKE} all

${CHECK_TARGETS}:
	cd src/$(shell echo $@ | cut -d'-' -f2) && ${MAKE} check

${ANALYZE_TARGETS}:
	cd src/$(shell echo $@ | cut -d'-' -f2) && ${MAKE} analyze

${CLEAN_TARGETS}:
	cd src/$(shell echo $@ | cut -d'-' -f2) && ${MAKE} clean

${INSTALL_TARGETS}:
	${MKDIR} -p obj
	cd src/$(shell echo $@ | cut -d'-' -f2) && ${MAKE} install

all: ${SUBDIRS}

check: ${CHECK_TARGETS}

analyze: ${ANALYZE_TARGETS}

clean: ${CLEAN_TARGETS}
	${RM} -rf pkg
	${RM} -rf doc

install: ${INSTALL_TARGETS}
	${MKDIR} -p ${includedir}
	${MKDIR} -p ${libdir}
	${CP} -r includes/twinshadow ${includedir}
	${AR} -cvq ${libdir}/twinshadow.a $(shell find src -name '*.o')
	${CC} ${CFLAGS} ${LDFLAGS} -shared -o ${libdir}/twinshdow.so $(shell find src -name '*.o')

docs:
	cldoc ${CFLAGS} -- \
		--language=c \
		--report \
		--merge=${TOP}/docs/_cldoc_static \
		--output=${TOP}/docs/html \
		$(shell find ${TOP}/src \( -name '*.c' -or -name '*.h' \) -and -not -name 'check_*')

everything: clean analyze all docs install check

.PHONY: clean docs
