#include <iostream>
#include "Logs.h"
#include "hwid.h"
#include "Date.h"

int main(int argc, const char* argv[])
{
	Log.Initialized();
	Log.Info("Program Started!");

	Log.Info("Opening hwid.dat file.");
	HWID.HardwareIdentification = HWID.ReadDatFile("hwid.dat");

	Log.Info("Hardware ID: " + HWID.HardwareIdentification);

	date_t.ExpiryDate = date_t.ReadFromBin("License.bin");

	if (date_t.ExpiryDate != 0)
	{
		std::string readable_date = date_t.TimeToString(date_t.ExpiryDate);
		Log.Info("Recovered Expiry Date: " + readable_date);
	}
	else
	{
		Log.Error("Failed to read expiry date.");
		exit(EXIT_FAILURE);
	}

	return 0;
}