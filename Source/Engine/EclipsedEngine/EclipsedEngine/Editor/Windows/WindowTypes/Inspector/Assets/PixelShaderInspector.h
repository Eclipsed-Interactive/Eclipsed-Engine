#pragma once

#include "EclipsedEngine/Editor/Windows/WindowTypes/Inspector/ITypeInspector.h"

namespace Eclipse::Editor {
	class PixelShaderInspector : public IInspector {
	public:
		PixelShaderInspector() = default;

		bool CanInspect(const InspectableTarget& target) override;
		void Draw(const InspectableTarget& target) override;
	};
}