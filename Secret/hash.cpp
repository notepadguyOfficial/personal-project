#include "hash.h"

HASH_MD5 hash;

std::string HASH_MD5::GenerateHash(const std::string& input)
{
	unsigned char result[MD5_DIGEST_LENGTH];

	MD5(reinterpret_cast<const unsigned char*>(input.c_str()), input.size(), result);

	std::ostringstream hexStream;
	for (int i = 0; i < MD5_DIGEST_LENGTH; i++) {
		hexStream << std::setw(2) << std::setfill('0') << std::hex << static_cast<int>(result[i]);
	}

	return hexStream.str();
}

std::string HASH_MD5::ToBinary(std::time_t number)
{
	std::string binary;
	while (number > 0)
	{
		binary = (number % 2 ? "1" : "0") + binary;
		number /= 2;
	}

	return binary.empty() ? "0" : binary;
}

std::string HASH_MD5::ToHex(std::time_t number)
{
	std::stringstream ss;
	ss << std::hex << number;
	return ss.str();
}