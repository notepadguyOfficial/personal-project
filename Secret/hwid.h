#pragma once

#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <fstream>
#include <intrin.h>

#include <winsock2.h>
#include <iphlpapi.h>
#include <windows.h>

#pragma comment(lib, "iphlpapi.lib")

class hwid
{
public:
	void getHWID();
	std::string getCPU();
	std::string getHDS();
	std::string getMBS();
	std::string getMAC();
	bool SaveToDat(const std::string& filename, const std::string& input);
	std::string ReadDatFile(const std::string& filename);
	std::string HardwareIdentification;
};

extern hwid HWID;