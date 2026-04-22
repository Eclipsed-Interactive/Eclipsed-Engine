#pragma once

#include <stdexcept>

#define EXCEPTION(Message) Exception(Message, __LINE__, __FILE__)

namespace Eclipse
{
	class Exception : public std::runtime_error
	{
	public:
		using std::runtime_error::runtime_error;

		Exception(const std::string& message, int line, const char* path)
			: std::runtime_error(message), errorLine(line), errorPath(path)
		{
			
		}

		int errorLine = 0;
		std::string errorPath = 0;
	};
}