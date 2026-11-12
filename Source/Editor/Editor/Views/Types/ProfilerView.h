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
		void S(const Profiler::ProfilerNode* node);
	};
}