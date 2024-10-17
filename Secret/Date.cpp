#include "Date.h"
#include "Logs.h"
#include "hash.h"

DATE_TIME date_t;

std::time_t DATE_TIME::GetExpiryDate(int days)
{
	auto now = std::chrono::system_clock::now();
	auto expiry_time = now + std::chrono::hours(24 * days);
	return std::chrono::system_clock::to_time_t(expiry_time);
}

std::string DATE_TIME::TimeToString(std::time_t time)
{
	char buffer[100];
	std::tm* tm = std::localtime(&time);
	std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", tm);
	return std::string(buffer);
}

std::time_t DATE_TIME::ReadFromBin(const std::string& filename)
{
	std::ifstream input(filename, std::ios::binary);
	if (!input.is_open()) {
		Log.Error("Error opening file for reading: " + filename);
		exit(EXIT_FAILURE);
	}

	std::string binary_data((std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>());

	if (binary_data.empty()) {
		Log.Error("No data read from file: " + filename);
		exit(EXIT_FAILURE);
	}

	std::time_t date = 0;
	for (char bit : binary_data) {
		if (bit != '0' && bit != '1') {
			Log.Error("Invalid character in binary data: " + std::string(1, bit));
			 exit(EXIT_FAILURE);
		}
		date = (date << 1) | (bit - '0');
	}

	return date;
}

bool DATE_TIME::SaveToBin(const std::string& filename, std::time_t date)
{
	std::string result = hash.ToBinary(date);
	
	std::ofstream output(filename, std::ios::binary);

	if (!output.is_open())
	{
		Log.Error("Error opening file for writing.");
		return false;
	}

	if (result.empty())
	{
		Log.Error("Binary string is empty, nothing to write.");
		output.close();
		return false;
	}
	
	output.write(result.c_str(), result.size());

	if (!output)
	{
		Log.Error("Error writing to file: " + filename);
		return false;
	}

	output.close();
		
	return true;
}