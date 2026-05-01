#pragma once

#include "EclipsedEngine/Editor/Windows/WindowTypes/Inspector/ITypeInspector.h"

namespace Eclipse::Editor {
	class MaterialInspector: public IInspector {
	public:
		MaterialInspector() = default;

		bool CanInspect(const InspectableTarget& target) override;
		void Draw(const InspectableTarget& target) override;
	};
}