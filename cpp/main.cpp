#include "kernel.h"

extern int userMain(int us_argc, char* us_argv[]);

int main(int argc, char* argv[]){

	Kernel::system_initialization();

	int ret = userMain(argc, argv);

	Kernel::system_restore();

	return ret;
}
