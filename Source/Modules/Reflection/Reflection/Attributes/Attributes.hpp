#pragma once

#include <functional>

namespace Eclipse::Reflection
{
	struct SerializeField {};

	struct Range
	{
		float min, max;
	};

	struct Step
	{
		float step;
	};

	template<auto Method, typename T>
	void InvokeMember(void* obj) { (static_cast<T*>(obj)->*Method)(); }

	struct OnChange
	{
		std::function<void(void*)> event;
	};
}