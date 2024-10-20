#include "Logs.h"
#include <mutex>
#include "XOR.h"

Logs& Logs::GetInstance()
{
	static Logs instance;
	return instance;
}

void Logs::SetLogFile(const std::string& filename)
{
	std::lock_guard<std::mutex> guard(mutex_);
	filestream_.open(filename, std::ios_base::app | std::ios_base::out);

	if (!filestream_)
	{
		std::cerr << XorStr("Error opening the file!") << std::endl;
	}
}

void Logs::SetLogLevel(LOG_LEVEL level)
{
	LogLevel_ = level;
}

std::string Logs::GetCurrentTime()
{
	std::time_t now = std::time(nullptr);
	char buffer[100];
	std::strftime(buffer, sizeof(buffer), XorStr("%Y-%m-%d %H:%M:%S"), std::localtime(&now));
	return std::string(buffer);
}

std::string Logs::FormatFileName()
{
	std::time_t now = std::time(nullptr);
	char buffer[100];
	std::strftime(buffer, sizeof(buffer), XorStr("%Y-%m-%d_%H-%M-%S"), std::localtime(&now));
	return XorStr("Secret_") + std::string(buffer) + XorStr(".log");
}

std::string Logs::LogLevelToString(LOG_LEVEL level)
{
	switch (level)
	{
	case LOG_LEVEL::DEBUG: return XorStr("DEBUG");
	case LOG_LEVEL::INFO: return XorStr("INFO");
	case LOG_LEVEL::WARNING: return XorStr("WARNING");
	case LOG_LEVEL::ERROR: return XorStr("ERROR");
	default: return XorStr("UKNOWN");
	}
}

std::string Logs::FormatMessage(const std::string& message, LOG_LEVEL level)
{
	std::ostringstream ss;
	ss << "[" << GetCurrentTime() << "] [" << LogLevelToString(level) << "] : " << message;
	return ss.str();
}

void Logs::Print(const std::string& message, LOG_LEVEL level)
{
	if (level >= LogLevel_)
	{
		std::lock_guard<std::mutex> guard(mutex_);
		std::string output = FormatMessage(message, level);

		std::cout << output << std::endl;

		if (filestream_.is_open())
		{
			filestream_ << output << std::endl;
		}
	}
}

void Logs::Debug(const std::string& message)
{
	Print(message, LOG_LEVEL::DEBUG);
}

void Logs::Info(const std::string& message)
{
	Print(message, LOG_LEVEL::INFO);
}

void Logs::Warning(const std::string& message)
{
	Print(message, LOG_LEVEL::WARNING);
}

void Logs::Error(const std::string& message)
{
	Print(message, LOG_LEVEL::ERROR);
}

void Logs::Input(std::string& input, const std::string& prompt)
{
	std::cout << prompt;
	std::cin >> input;

	std::string temp = prompt + " " + input;

	Info(temp);
}

void Logs::Initialized()
{
	GetInstance();
#if _DEBUG
	SetLogFile(XorStr("Test.log"));
#else
	SetLogFile(FormatFileName());
#endif
	SetLogLevel(LOG_LEVEL::INFO);
}

Logs& Log = Logs::GetInstance();