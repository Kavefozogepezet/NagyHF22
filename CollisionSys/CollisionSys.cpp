#include "tests/tests.h"
#include "Sandbox.h"
#include <signal.h>

#ifdef COLLSYS_LIB
void CollSys::sandboxInit(CollSys::Sandbox& sandbox) {}
#endif

void sighandler(int signum) {
	std::cout << signum << std::endl;
	exit(signum);
}

int main(int argc, char** argv) {
#ifdef CPORTA
	CSTests::tests_main();
#endif
	signal(SIGINT, sighandler);
	return CollSys::csMain(argc, argv);
}
