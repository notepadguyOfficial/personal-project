#include <iostream>
#include "Logs.h"
#include "hwid.h"
#include "Date.h"
#include "XOR.h"

int main(int argc, const char* argv[])
{
	Log.Initialized();
	HWID.getHWID();

#if NDEBUG
	Log.Info(XorStr("HWID: ") + HWID.HardwareIdentification);
#endif

	if (!HWID.SaveToDat(XorStr("hwid.dat"), HWID.HardwareIdentification))
	{
		Log.Error(XorStr("Unable to Save HWID!"));
		exit(EXIT_FAILURE);
	}
	else
		Log.Info(XorStr("Successfully Saved HWID"));

	int days;

	Log.Input(days, XorStr("How Many Days before it expires: "));

	std::time_t days_timestamp = date_t.GetExpiryDate(days);
	std::string time_temp = date_t.TimeToString(days_timestamp);
	Log.Info(XorStr("Expires on: ") + time_temp);

	if (date_t.SaveToBin(XorStr("License.bin"), days_timestamp))
		Log.Info(XorStr("Successfully Saved Date Data"));
	else
	{
		Log.Error(XorStr("Unable to Save Date Data"));
		exit(EXIT_FAILURE);
	}

	system(XorStr("pause"));

	return 0;
}