#pragma once

#include "Editor/Views/IEditorView.h"

namespace Eclipse::Profiler { class ProfilerNode; }

namespace Eclipse::Editor
{
	class ProfilerView : public EditorView<ProfilerView>
	{
		BASIC_VIEW("Profiler")

	public:
		void OnOpen() override;
		void Draw() override;

	private:
		void DrawNode(const Profiler::ProfilerNode* node);

	private:
		static inline constexpr double FrameBudgetMicroSecond = 16666.0;
	};
}