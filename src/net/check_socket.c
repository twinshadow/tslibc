#include "check/twinshadow.h"
#include "twinshadow/net.h"
#include <fcntl.h>

START_TEST(test_socket)
{
	int bindfd = -1;
	int sendfd = -1;
	int recvfd = -1;
	struct addrinfo* testai;

	testai = TS_GAI_TCPIP("127.0.0.1", "29867");
	bindfd = sobind(testai);
	ck_assert_int_gt(bindfd, -1);
	printf("bindfd: %d\n", bindfd);
	fcntl(bindfd, F_SETFL, O_NONBLOCK);
	listen(bindfd, 10);
	sendfd = soconnect(testai);
	ck_assert_int_gt(sendfd, -1);
	printf("sendfd: %d\n", sendfd);
	recvfd = accept(bindfd, NULL, NULL);
	ck_assert_int_gt(recvfd, -1);
	printf("recvfd: %d\n", recvfd);
	close(sendfd);
	close(recvfd);
	close(bindfd);
}
END_TEST

void
setup_socket_test(void) {
}

void
teardown_socket_test(void) {
}

TCase *
tcase_socket(void) {
	TCase *tc = tcase_create("socket");
	tcase_add_checked_fixture(tc, setup_socket_test, teardown_socket_test);
	tcase_add_test(tc, test_socket);
	return tc;
}

CHECK_MAIN_STANDALONE(socket);
