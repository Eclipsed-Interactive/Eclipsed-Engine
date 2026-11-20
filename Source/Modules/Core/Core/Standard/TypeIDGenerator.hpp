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
		static constexpr TypeID GetTypeID();

	private:
		template<typename T>
		static constexpr const char* Name();
	};
}

namespace Eclipse
{
	template<typename T>
	inline constexpr TypeID Type::GetTypeID()
	{
		using Type = std::remove_cvref_t<T>;
		return Hash::FNV1A(Name<Type>());
	}

	template<typename T>
	inline constexpr const char* Type::Name()
	{
		return __FUNCSIG__;
	}
}