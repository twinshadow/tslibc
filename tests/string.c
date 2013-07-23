#include "check/twinshadow.h"
#include "check/string.h"

int
main(void)
{
	int number_failed;

	Suite *s = suite_create("check_vec");
	suite_add_tcase(s, tcase_strrev());
	suite_add_tcase(s, tcase_strrot());
	suite_add_tcase(s, tcase_strrtok());
	suite_add_tcase(s, tcase_strshift());
	suite_add_tcase(s, tcase_strstrip());

	SRunner *sr = srunner_create(s);
	srunner_run_all(sr, CK_VERBOSE);
	number_failed = srunner_ntests_failed(sr);
	srunner_free(sr);

	return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
