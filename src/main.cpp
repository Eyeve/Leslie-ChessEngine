#include "lichess_shell.h"


int main(int argc, char* argv[])
{

    Leslie::LichessShell shell(argc, argv);

	int return_code = shell.start();
	return return_code;
}
