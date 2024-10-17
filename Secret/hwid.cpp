#include "Logs.h"
#include "hwid.h"
#include "hash.h"


hwid HWID;

void hwid::getHWID()
{
	std::string cpuId = getCPU();
	std::string motherboardSerial = getHDS();
	std::string hardDriveSerial = getMBS();
	std::string macAddress = getMAC();
	std::string combined = cpuId + motherboardSerial + hardDriveSerial + macAddress;
	HardwareIdentification = hash.GenerateHash(combined);
}

std::string hwid::getCPU()
{
	int cpuInfo[4];
	__cpuid(cpuInfo, 0);
	std::stringstream ss;
	ss << std::hex << cpuInfo[1] << cpuInfo[2] << cpuInfo[3];
	return ss.str();
}

std::string hwid::getHDS()
{
	char systemDrive[] = "C:\\";
	DWORD volumeSerialNumber = 0;

	if (GetVolumeInformationA(systemDrive, NULL, 0, &volumeSerialNumber, NULL, NULL, NULL, 0))
	{
		return std::to_string(volumeSerialNumber);
	}
}

std::string hwid::getMBS()
{
	HKEY hKey;
	const char* subKey = "HARDWARE\\DESCRIPTION\\System\\BIOS";
	const char* valueName = "SystemProductName";

	if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, subKey, 0, KEY_READ, &hKey) != ERROR_SUCCESS)
	{
		Log.Error("Error opening registry key");
		exit(EXIT_FAILURE);
	}

	char serialNumber[256];
	DWORD serialNumberSize = sizeof(serialNumber);
	if (RegQueryValueExA(hKey, valueName, NULL, NULL, (LPBYTE)serialNumber, &serialNumberSize) != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		Log.Error("Error reading registry value");
		exit(EXIT_FAILURE);
	}

	RegCloseKey(hKey);

	return std::string(serialNumber);
}

std::string hwid::getMAC()
{
	ULONG flags = GAA_FLAG_INCLUDE_PREFIX;
	ULONG family = AF_UNSPEC;
	ULONG outBufLen = 15000;
	PIP_ADAPTER_ADDRESSES pAddresses = (IP_ADAPTER_ADDRESSES*)malloc(outBufLen);

	if (GetAdaptersAddresses(family, flags, NULL, pAddresses, &outBufLen) == NO_ERROR) {
		std::stringstream macAddress;
		for (PIP_ADAPTER_ADDRESSES pCurrAddresses = pAddresses; pCurrAddresses != NULL; pCurrAddresses = pCurrAddresses->Next)
		{
			if (pCurrAddresses->PhysicalAddressLength != 0)
			{
				for (UINT i = 0; i < pCurrAddresses->PhysicalAddressLength; i++)
				{
					macAddress << std::hex << std::setfill('0') << std::setw(2)
						<< (int)pCurrAddresses->PhysicalAddress[i];

					if (i < pCurrAddresses->PhysicalAddressLength - 1)
					{
						macAddress << ":";
					}
				}

				break;
			}
		}
		free(pAddresses);
		return macAddress.str();
	}
	free(pAddresses);

	return "Error retrieving MAC address";
}

/*
* 
* depricated at 10/18/2024 12:08 AM
* 
* 
bool hwid::SaveToDat(const std::string& filename, const std::string& input)
{
	unsigned char result[MD5_DIGEST_LENGTH];
	std::ofstream outFile(filename, std::ios::binary);
	if (!outFile) {
		Log.Error("Error opening file for writing: " + filename);
		return false;
	}
	
	outFile.write(reinterpret_cast<const char*>(result), MD5_DIGEST_LENGTH);
	if (!outFile) {
		Log.Error("Error writing to file: " + filename);
		return false;
	}

	outFile.close();
	return true;
}
*/

bool hwid::SaveToDat(const std::string& filename, const std::string& input)
{
	std::string hashHex = hash.GenerateHash(input);
	unsigned char result[MD5_DIGEST_LENGTH];

	for (int i = 0; i < MD5_DIGEST_LENGTH; ++i) {
		result[i] = static_cast<unsigned char>(std::stoi(hashHex.substr(i * 2, 2), nullptr, 16));
	}

	std::ofstream outFile(filename, std::ios::binary);
	if (!outFile) {
		Log.Error("Error opening file for writing: " + filename);
		return false;
	}

	outFile.write(reinterpret_cast<const char*>(result), MD5_DIGEST_LENGTH);
	if (!outFile) {
		Log.Error("Error writing to file: " + filename);
		return false;
	}

	outFile.close();
	return true;
}

std::string hwid::ReadDatFile(const std::string& filename)
{
	unsigned char result[MD5_DIGEST_LENGTH];

	std::ifstream inFile(filename, std::ios::binary);
	if (!inFile) {
		std::cerr << "Error opening file for reading: " << filename << std::endl;
		exit(EXIT_FAILURE);
	}

	inFile.read(reinterpret_cast<char*>(result), MD5_DIGEST_LENGTH);
	if (!inFile) {
		std::cerr << "Error reading from file: " << filename << std::endl;
		exit(EXIT_FAILURE);
	}

	inFile.close();

	std::ostringstream hexStream;
	for (int i = 0; i < MD5_DIGEST_LENGTH; i++) {
		hexStream << std::setw(2) << std::setfill('0') << std::hex << static_cast<int>(result[i]);
	}

	return hexStream.str();
}