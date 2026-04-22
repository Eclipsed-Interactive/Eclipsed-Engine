#pragma once

#include "EclipsedEngine/Editor/Windows/WindowBase.h"

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
		void Update() override;

		static void SetActiveType(ActiveItemTypes_ aType)
		{
			if (lockInspector)
				return;

			activeType = aType;
		}

	private:
		void DrawGameObjectInspector();
		void DrawComponentInspector(class Component* comp);

		void DrawAssetInspector();

		void DrawTextureAssetInspector();
		void DrawMaterialAssetInspector();

		template<typename T>
		void DrawComponentInspector() {}

	public:
		static inline unsigned CurrentGameObjectID;

		static inline constexpr int NAME_BUFFER_LENGTH = 256;
		char nameBuffer[NAME_BUFFER_LENGTH] = {};

	private:
		static inline bool lockInspector = false;
		static inline ActiveItemTypes_ activeType;

		class Eclipse::Component* myComponentRightClicked = nullptr;
	};
}