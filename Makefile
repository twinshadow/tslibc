TOP=		$(realpath ${PWD})
SUBDIRS=	$(shell find src -maxdepth 1 -mindepth 1 -type d -exec basename \{\} \;)
TARGETS=	analyze build check clean install checko

define WRAPPER_TARGETS =
${1}_TARGETS=		$$(addprefix ${1}-,${SUBDIRS})
endef
$(foreach target,${TARGETS},$(eval $(call WRAPPER_TARGETS,${target})))

all: ${build_TARGETS}
analyze: ${analyze_TARGETS}
check: ${check_TARGETS}
checko: ${checko_TARGETS}
clean: ${clean_TARGETS}
	${RM} -rf pkg
	${RM} -rf doc

install: ${install_TARGETS}
	${MKDIR} -p ${includedir}
	${MKDIR} -p ${libdir}
	${CP} -r includes/twinshadow ${includedir}
	${AR} -cvq ${libdir}/libts.a $(shell find src -name '*.o')
	${CC} ${CFLAGS} ${LDFLAGS} -shared -o ${libdir}/libts.so $(shell find src -name '*.o')

docs:
	cldoc ${CFLAGS} -- \
		--language=c \
		--report \
		--merge=${TOP}/docs/_cldoc_static \
		--output=${TOP}/docs/html \
		$(shell find ${TOP}/src \( -name '*.c' -or -name '*.h' \) -and -not -name 'check_*')

tests:
	cd ${TOP}/tests && ${MAKE} clean all

define WRAPPER_TEMPLATE =
$${${1}_TARGETS}:
	cd src/$$(@:${1}-%=%) && ${MAKE} ${1}
endef
$(foreach target,${TARGETS},$(eval $(call WRAPPER_TEMPLATE,${target})))

everything: clean analyze all docs check install

.PHONY: analyze clean docs install tests
