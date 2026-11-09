#include "Assets/Core/GUID.h"

#include <random>
#include <memory>
#include <sstream>
#include <iomanip>

#include <Core/Logger/DebugLogger.h>

namespace Eclipse::Assets
{
	bool GUID::IsEmpty() const
	{
		return high == 0 && low == 0;
	}

	GUID GUID::GenerateGUID()
	{
		static std::random_device rd;
		static std::mt19937_64 gen(rd());

		GUID guid;
		guid.high = gen();
		guid.low = gen();

		if (guid.low == 0 || guid.high == 0)
			return GenerateGUID();
		else return guid;
	}

	void GUID::Generate()
	{
		*this = GenerateGUID();
	}

	void GUID::FromString(const std::string& hashStr)
	{
		if (hashStr.empty()) return;

		if (hashStr.size() != 32 || hashStr.find_first_not_of("0123456789abcdefABCDEF") != std::string::npos)
		{
			LOG_ERROR("Invalid GUID string '" + hashStr + "' expected 32 characters.");
			high = 0;
			low = 0;
			return;
		}

		high = std::stoull(hashStr.substr(0, 16), nullptr, 16);
		low= std::stoull(hashStr.substr(16, 32), nullptr, 16);
	}

	std::string GUID::ToString() const
	{
		std::stringstream ss;
		ss << std::hex
			<< std::setw(16) << std::setfill('0') << high
			<< std::setw(16) << std::setfill('0') << low;


		std::string hexGuid = ss.str();

		unsigned long long parsedHigh = std::stoull(hexGuid.substr(0, 16), nullptr, 16);
		unsigned long long parsedLow = std::stoull(hexGuid.substr(16), nullptr, 16);

		if (high != parsedHigh && parsedLow != low)
		{
			LOG_ERROR("Failed to parse GUID to std::string: ")
		}

		return hexGuid;
	}

	bool GUID::operator==(const GUID& other) const
	{
		return high == other.high && low == other.low;
	}

	bool GUID::operator!=(const GUID& other) const
	{
		return !(*this == other);
	}
}