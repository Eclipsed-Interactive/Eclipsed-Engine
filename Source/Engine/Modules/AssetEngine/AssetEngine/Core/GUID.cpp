#include "GUID.h"

#include <random>
#include <memory>
#include <sstream>
#include <iomanip>
#include <iostream>

namespace Eclipse::Assets
{
	GUID GUID::GenerateGUID()
	{
		static std::random_device rd;
		static std::mt19937_64 gen(rd());

		GUID guid;
		guid.high = gen();
		guid.low = gen();

		return guid;
	}

	void GUID::Generate()
	{
		*this = GenerateGUID();
	}

	void GUID::FromString(const std::string& hashStr)
	{
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
			std::cout << "Its wrong somehwere" << std::endl;
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