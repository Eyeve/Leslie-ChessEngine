//
// Created by Eyeve on 10.03.2025.
//

#include "uci.h"

int main(int argc, char* argv[])
{
    Leslie::UCI_Engine uci(argc, argv);

	int return_code = uci.start();
	return return_code;
}
