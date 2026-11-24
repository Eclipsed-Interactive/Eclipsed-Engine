#pragma once

#include "Editor/Views/IEditorView.h"

#include <vector>
#include <unordered_set>
#include <filesystem>

#include <rapidjson/document.h>

#include "ECS/Types.h"

namespace Eclipse
{
	class Canvas;
	class GameObject;
}

namespace Eclipse::Editor
{
	class HierarchyView : public EditorView<HierarchyView>
	{
		BASIC_VIEW("Hierarchy")

	public:
		void Draw() override;

		static void OpenParents(GameObjectID aParentID);
		void HierarchyButton(GameObject* aGameObject, float totalIndent);
		void AssignParentChildren(GameObject* targetGO, GameObject* aGameObject);

		static void CreatePrefab(GameObjectID aGameobjectID, std::filesystem::path aPath);

	public:
		static inline GameObjectID CurrentGameObjectID;

		Canvas* GetParentCanvas(GameObject* BaseObject);

		bool CheckCopomentType(GameObject* aGameobject, GameObject* aParent);

		static inline std::unordered_set<GameObjectID, GUIDHash> gameobjectIdsThatAreOpen;

		GameObjectID SelectedGameobjectID;

		bool gameobjectrightclicked;
	};
}	