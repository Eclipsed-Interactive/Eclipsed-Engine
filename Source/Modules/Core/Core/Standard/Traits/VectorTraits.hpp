#pragma once

#include <vector>

#include "TypeTraits.hpp"

namespace Eclipse::Traits
{
	template<typename T> struct IsVector : std::false_type {};
	template<typename T, typename A> struct IsVector<std::vector<T, A>> : std::true_type { using Element = T; };

	template<typename T>
	inline constexpr bool is_vector_v = IsVector<std::remove_cv_t<T>>::value;

	template<typename T, typename A>
	struct TypeTrait<std::vector<T, A>>
	{
		using Container = std::vector<T, A>;
		using Element = T;
		static constexpr size_t ElementSize = sizeof(T);
	};
}