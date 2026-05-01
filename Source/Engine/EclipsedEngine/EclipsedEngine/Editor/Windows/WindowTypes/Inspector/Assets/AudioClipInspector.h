#pragma once

#include "EclipsedEngine/Editor/Windows/WindowTypes/Inspector/ITypeInspector.h"

namespace Eclipse::Editor {
	class AudioClipInspector : public IInspector {
	public:
		AudioClipInspector() = default;

		bool CanInspect(const InspectableTarget& target) override;
		void Draw(const InspectableTarget& target) override;
	};
}