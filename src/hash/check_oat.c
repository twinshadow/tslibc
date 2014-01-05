#include "check/twinshadow.h"
#include "check/hash.h"

START_TEST (oat_test) {
}
END_TEST

TCase *
tcase_oat(void) {
	TCase *tc = tcase_create("array");
	tcase_add_test(tc, oat_test);
	return tc;
}

CHECK_MAIN_STANDALONE(oat);
