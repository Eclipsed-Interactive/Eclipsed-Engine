#pragma once

#include "EclipsedEngine/Editor/Windows/WindowTypes/Inspector/ITypeInspector.h"

namespace Eclipse::Editor {
	class FontInspector : public IInspector {
	public:
		FontInspector() = default;

		bool CanInspect(const InspectableTarget& target) override;
		void Draw(const InspectableTarget& target) override;
	};
}