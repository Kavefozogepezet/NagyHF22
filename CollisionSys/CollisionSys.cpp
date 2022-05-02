#include "debug/memtrace.h"

#include "tests/tests.h"
#include "Sandbox.h"

#ifdef COLLSYS_LIB
void CollSys::sandboxInit(CollSys::Sandbox& sandbox) {}
#endif

#ifdef CPORTA
int main(int argc, char** argv) {
	return CSTests::tests_main();
}
#else
int main(int argc, char** argv) {
	return CollSys::csMain(argc, argv);
}
#endif
