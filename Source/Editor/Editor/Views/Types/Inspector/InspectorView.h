#pragma once

#include "Editor/Views/IEditorView.h"

#include "ECS/Component.h"
#include "Editor/Views/Types/Inspector/InspectableTarget.h"

namespace Eclipse::Editor
{
	enum ActiveItemTypes_
	{
		ActiveItemTypes_None,
		ActiveItemTypes_GameObject,
		ActiveItemTypes_Asset
	};

	class InspectorView final : public EditorView<InspectorView>
	{
		BASIC_VIEW("Inspector")


	public:
		void OnOpen() override;
		void Draw() override;


	private:
		void DrawInspectorHeader();
		void RegisterInspectors();

		InspectableTarget currentTarget = std::monostate{};
		bool lockInspector = false;



	};
}