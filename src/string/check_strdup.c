#include "check/twinshadow.h"
#include "twinshadow/string.h"

char *buf_strdup;

START_TEST(strdup_test) {
}
END_TEST

void
setup_strdup_test(void) {
	buf_strdup = ts_strdup("one two three four five");
}

void
teardown_strdup_test(void) {
	free(buf_strdup);
}

TCase *
tcase_strdup(void) {
	TCase *tc = tcase_create("strdup");
	tcase_add_checked_fixture(tc, setup_strdup_test, teardown_strdup_test);
	tcase_add_test(tc, strdup_test);
	return tc;
}
