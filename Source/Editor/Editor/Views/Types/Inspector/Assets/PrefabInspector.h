#pragma once

#include "Editor/Views/Types/Inspector/ITypeInspector.h"

namespace Eclipse::Editor {
	class PrefabInspector : public IInspector {
	public:
		PrefabInspector() = default;

		bool CanInspect(const InspectableTarget& target) override;
		void Draw(const InspectableTarget& target) override;
	};
}