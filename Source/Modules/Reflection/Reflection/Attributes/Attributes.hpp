#pragma once

namespace Eclipse::Reflection
{
	struct SerializeField {};

	struct Range
	{
		int min, max;
	};

	template<typename T>
	struct Step
	{
		T step;
	};
}