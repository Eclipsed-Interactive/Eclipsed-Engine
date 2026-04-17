#pragma once

#include "WindowBase.h"

HELP // Hover this for some help.

namespace Eclipse::Editor
{
	class TemplateWindow final : public AbstractWindow
	{
		// UnComment this
		//BASE_SELECTION(TemplateWindow, "TemplateWindow");

	public:
		void Open() override {};
		void Update() override {};
		void Close() override {};
	};
}