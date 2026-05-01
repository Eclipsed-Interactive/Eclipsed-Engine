#pragma once

#include "EclipsedEngine/Editor/Windows/WindowBase.h"
#include "EntityEngine/Component.h"

namespace Eclipse::Editor
{
	enum ActiveItemTypes_
	{
		ActiveItemTypes_None,
		ActiveItemTypes_GameObject,
		ActiveItemTypes_Asset
	};

	class InspectorWindow final : public AbstractWindow
	{
		EDITOR_WINDOW_BASE_2(InspectorWindow, "Inspector");

	public:
		void Open() override;
		void Update() override;


	private:
		void DrawInspectorHeader();
		void RegisterInspectors();

		InspectableTarget currentTarget = std::monostate{};
		bool lockInspector = false;



	};
}