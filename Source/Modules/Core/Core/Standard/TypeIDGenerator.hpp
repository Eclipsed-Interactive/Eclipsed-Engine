#pragma once

#include <type_traits>
#include "Core/Hash.hpp"

namespace Eclipse
{
	using TypeID = unsigned long long;

	class Type
	{
	public:
		template<typename T>
		static TypeID GetTypeID();

	private:
		template<typename T>
		static const char* Name();
	};
}


namespace Eclipse
{
	template<typename T>
	inline TypeID Type::GetTypeID()
	{
		using Type = std::remove_cv_t<std::remove_reference<T>>;
		return Hash::FNV1A(Name<Type>());
	}

	template<typename T>
	inline const char* Type::Name()
	{
		return __FUNCSIG__;
	}
}