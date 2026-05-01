#pragma once

#include "Windows/WindowTypes/Inspector/InspectableTarget.h"

namespace Eclipse::Editor
{
	class SelectionContext final
	{
	public:
		static void SetActiveContext(InspectableTarget target);

		static InspectableTarget& GetCurrentData();

		template<typename T>
		static T GetCurrentData()
		{
			if (std::holds_alternative<T>(Get().activeTarget))
			{
				return std::get<T>(Get().activeTarget);
			}

			return {};
		}

	private:
		static SelectionContext& Get()
		{
			static SelectionContext instance;
			return instance;
		}

		InspectableTarget activeTarget = std::monostate{};
	};
}