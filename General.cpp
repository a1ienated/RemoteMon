#include "General.h"
#include <iostream>

General::General(char * flags[], int length) : isValidateOn(false)
{
	allowedMode.push_back("-serverUDP");
	allowedMode.push_back("-clientUDP");
	allowedMode.push_back("-serverTCP");
	allowedMode.push_back("-clientTCP");

	if (length > 1)
		isValidateOn = validateFlags(flags, length);
	else
		printf("Help command invoked: \n-serverUDP\n-clientUDP\n \
			\r-serverTCP\n-clientTCP\n");
}

bool General::validateFlags(char * flags[], int length)
{
	bool flagExist = false;
	int incorrectFlagIndex = -1;
	size_t lengthFlags = sizeof flags / sizeof flags[0];

	std::vector<std::string> rcvdFlags;

	for (int i = 1; i < length; i++)
		rcvdFlags.push_back(flags[i]);

	for (size_t i = 0; i < rcvdFlags.size(); i++)
	{
		flagExist = false;

		for (size_t itMode = 0; itMode < allowedMode.size(); itMode++) {
			if (rcvdFlags.at(i) == allowedMode.at(itMode))
			{
				modeSelected = itMode;
				flagExist = true;
				incorrectFlagIndex = -1;
			}
			else
				incorrectFlagIndex = i; //сохраняем индекс некорректного флага
		}

		if (!flagExist) {
			std::cout << "[-] Error: " << rcvdFlags[incorrectFlagIndex] << " incorrect flag was given!\n";
		}
	}

	return flagExist;
}

void General::run()
{
	switch (modeSelected)
	{
	case 0:
		{
			std::cout << "Mode UDP server!\n";
			ServerUDP serverUDP(5555);
			break;
		}
	case 1:
		{
			std::cout << "Mode UDP client!\n";
			ClientUDP clientUDP(5555);
			break;
		}
	case 2:
		{
			std::cout << "Mode TCP server!\n";
			ServerTCP serverTCP(5555);
			break;
		}
	case 3:
	{
		std::cout << "Mode TCP client!\n";
		ClientTCP clientTCP(5555);
		break;
	}
	}

}
