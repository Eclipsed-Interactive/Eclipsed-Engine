#include "IEditorView.h"

#include "ImGui/imgui.h"

namespace Eclipse::Editor
{
	void IEditorView::SetID(std::string id)
	{
		viewId = id;
	}

	void IEditorView::Indent(int indent)
	{
		currentIndent += indent;
		ImGui::Indent(indent);
	}

	void IEditorView::Unindent(int indent)
	{
		currentIndent -= indent;
		ImGui::Unindent(indent);
	}
}