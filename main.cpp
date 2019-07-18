#include "General.h"
#include "info.h"
#include <thread>
#include <iostream>

int main(int argc, char* argv[])
{
	General general(argv, argc);
	if (!general.isValidateOn)
		return EXIT_FAILURE;
	std::cout << "ID thread = " << std::this_thread::get_id() << std::endl;
	general.run();

	

	INFO("Exiting program");
	return EXIT_SUCCESS;
}
