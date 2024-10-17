#pragma once
#define OPENSSL_SUPPRESS_DEPRECATED
#include <openssl/md5.h>

#include <iostream>
#include <chrono>
#include <string>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <fstream>

#pragma comment(lib, "libcrypto.lib")
#pragma comment(lib, "libssl.lib")

class HASH_MD5
{
public:
	std::string GenerateHash(const std::string& input);
	std::string ToBinary(std::time_t number);
	std::string ToHex(std::time_t number);
};

extern HASH_MD5 hash;