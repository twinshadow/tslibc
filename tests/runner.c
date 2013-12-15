#include "check/twinshadow.h"
#include "twinshadow/twinshadow.h"

int
main (void) {
	int number_failed;
	Suite *s = suite_create("Twinshadow");
	suite_add_tcase(s, tcase_array());
	suite_add_tcase(s, tcase_map());
	suite_add_tcase(s, tcase_isort());
	suite_add_tcase(s, tcase_slist());
	suite_add_tcase(s, tcase_btree());
	suite_add_tcase(s, tcase_strrot());
	suite_add_tcase(s, tcase_strrev());
	suite_add_tcase(s, tcase_strrtok());
	suite_add_tcase(s, tcase_strshift());
	suite_add_tcase(s, tcase_strstrip());
	suite_add_tcase(s, tcase_strdup());
	SRunner *sr = srunner_create(s);
	srunner_run_all(sr, CK_VERBOSE);
	number_failed = srunner_ntests_failed(sr);
	srunner_free(sr);
	return ((number_failed == 0) ?
		EXIT_SUCCESS : EXIT_FAILURE);
}
