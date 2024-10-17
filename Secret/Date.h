#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <string>
#include <chrono>

class DATE_TIME
{
public:
	std::time_t GetExpiryDate(int days);
	std::string TimeToString(std::time_t time);
	std::time_t ReadFromBin(const std::string& filename);
	bool SaveToBin(const std::string& filename, std::time_t date);
	std::time_t ExpiryDate;
};

extern DATE_TIME date_t;