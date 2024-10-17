#include <iostream>
#include "Logs.h"
#include "hwid.h"
#include "Date.h"

int main(int argc, const char* argv[])
{
	Log.Initialized();
	HWID.getHWID();

#if NDEBUG
	Log.Info("HWID: " + HWID.HardwareIdentification);
#endif

	if (!HWID.SaveToDat("hwid.dat", HWID.HardwareIdentification))
	{
		Log.Error("Unable to Save HWID!");
		exit(EXIT_FAILURE);
	}
	else
		Log.Info("Successfully Saved HWID");

	int days;

	Log.Info("How Many Days before it expires: ");
	std::cin >> days;

	std::time_t days_timestamp = date_t.GetExpiryDate(days);
	std::string time_temp = date_t.TimeToString(days_timestamp);
	Log.Info("Expires on: " + time_temp);

	if (date_t.SaveToBin("License.bin", days_timestamp))
		Log.Info("Successfully Saved Date Data");
	else
	{
		Log.Error("Unable to Save Date Data");
		exit(EXIT_FAILURE);
	}

	return 0;
}