#pragma once

#include "Core/Math/Vector/Vector2.h"

namespace Eclipse::Editor
{
	class BaseRenderingView
	{
	protected:
		void DrawGizmoPopup();
		void DrawGizmoButtons(bool& drawGizmo);

	public:
		static inline bool DrawGizmo = false;
	};
}