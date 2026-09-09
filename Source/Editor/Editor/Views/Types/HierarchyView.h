#pragma once

#include "Editor/Views/IEditorView.h"

#include <vector>
#include <set>
#include <filesystem>

#include <rapidjson/document.h>

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

		static void OpenParents(unsigned aParentID);
		void HierarchyButton(GameObject* aGameObject, float totalIndent);
		void AssignParentChildren(GameObject* targetGO, GameObject* aGameObject);

		static void CreatePrefab(unsigned aGameobjectID, std::filesystem::path aPath);

	public:
		static inline unsigned CurrentGameObjectID;

		Canvas* GetParentCanvas(GameObject* BaseObject);

		bool CheckCopomentType(GameObject* aGameobject, GameObject* aParent);

		static inline std::set<unsigned> gameobjectIdsThatAreOpen;

		unsigned SelectedGameobjectID;

		bool gameobjectrightclicked;
	};
}	