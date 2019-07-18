#pragma once
#include <string>
#include <vector>
#include "ServerUDP.h"
#include "ClientUDP.h"
#include "ServerTCP.h"
#include "ClientTCP.h"

class General
{
public:
	General(char * flags[], int length);
	void run();
	bool isValidateOn;

private:
	std::vector<std::string> allowedMode;
	int modeSelected;

	bool validateFlags(char * flags[], int length);
};

