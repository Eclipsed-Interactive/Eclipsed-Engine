#pragma once

#ifdef ECL_EDITOR

#include "Editor/Views/Types/Inspector/ITypeInspector.h"

namespace Eclipse::Editor {
	class TextureInspector : public IInspector {
	public:
		TextureInspector() = default;

		bool CanInspect(const InspectableTarget& target) override;
		void Draw(const InspectableTarget& target) override;
	};
}

#endif