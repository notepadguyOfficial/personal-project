#include <iostream>
#include "Logs.h"
#include "hwid.h"
#include "Date.h"
#include "XOR.h"

int main(int argc, const char* argv[])
{
	Log.Initialized();
	Log.Info(XorStr("Program Started!"));

	Log.Info(XorStr("Opening hwid.dat file."));
	HWID.HardwareIdentification = HWID.ReadDatFile(XorStr("hwid.dat"));

	Log.Info(XorStr("Hardware ID: ") + HWID.HardwareIdentification);

	date_t.ExpiryDate = date_t.ReadFromBin(XorStr("License.bin"));

	if (date_t.ExpiryDate != 0)
	{
		std::string readable_date = date_t.TimeToString(date_t.ExpiryDate);
		Log.Info(XorStr("Recovered Expiry Date: ") + readable_date);
	}
	else
	{
		Log.Error(XorStr("Failed to read expiry date."));
		exit(EXIT_FAILURE);
	}

	return 0;
}