#pragma once

#include <string>
#include <unordered_map>

namespace Eclipse::Reflection
{
	class TypeHandler
	{
	public:
		template<typename T>
		static void RegisterType();
	};

	template<typename T>
	inline void TypeHandler::RegisterType()
	{

	}
}