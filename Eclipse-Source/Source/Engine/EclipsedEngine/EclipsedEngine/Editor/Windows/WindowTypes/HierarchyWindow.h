#ifdef ECLIPSED_EDITOR
#pragma once

#include "EclipsedEngine/Editor/Windows/WindowBase.h"

#include <vector>

#include <rapidjson/document.h>

#include <set>

namespace Eclipse
{
	class GameObject;
	namespace Editor
	{
		class HierarchyWindow final : public AbstractWindow
		{
			EDITOR_WINDOW_BASE_2(HierarchyWindow, "Hierarchy")

		public:
			void HierarchyButton(GameObject* aGameObject, float totalIndent);
			void AssignParentChildren(GameObject* targetGO, GameObject* aGameObject);
			void Update() override;

			static void CreatePrefab(unsigned aGameobjectID, std::filesystem::path aPath);

		public:
			static inline unsigned CurrentGameObjectID;

			void RecursiveDeleteChildren(GameObject*& aGameObject);

			bool CheckCopomentType(GameObject* aGameobject, GameObject* aParent);

			std::set<unsigned> gameobjectIdsThatAreOpen;

			unsigned SelectedGameobjectID;
			
			bool gameobjectrightclicked;
		};
	}
}
#endif