#pragma once

#include <type_traits>

namespace Eclipse::Traits
{
	template<typename T>
	struct TypeTrait 
	{
		using Element = T;
		static constexpr size_t Size = sizeof(T);
	};
}