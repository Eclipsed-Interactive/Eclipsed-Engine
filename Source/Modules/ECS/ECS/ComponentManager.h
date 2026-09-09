#pragma once

#include "ECS/Component.h"
#include "ECS/GameObject.h"

#include <vector>
#include <unordered_map>

//#include "Core/Macros/defines.h"

#include <functional>

#include "ECS.Core.hpp"

#define GetComp(Type, GOID)\
ComponentManager::GetComponent<Type>(GOID)

#define MAX_COMPONENT_MEMORY_BYTES 20'000'000

namespace Eclipse
{
	namespace Editor {
		class HierarchyView;
		class InspectorWindow;
		class GameObjectInspector;
		class EditorActions;
	}

	typedef unsigned ComponentIndex;

	class GameObject;
	class Component;
	class ECS_API ComponentManager
	{
		friend class Editor::EditorActions;
		friend class Editor::HierarchyView;
		friend class Editor::GameObjectInspector;
		friend class Editor::InspectorWindow;
		friend class SceneLoader;

	public:
		template<typename T>
		struct FloatVectorContainer
		{
			unsigned Value = 0;
			std::vector<T> vector;
		};

		typedef FloatVectorContainer<Component*> UpdatePriority;
		typedef std::vector<UpdatePriority> RenderLayers;

		static void SetCreateComponentReplicated(std::function<void(Component*)> aCreateComponentReplicated) { CreateComponentReplicated = aCreateComponentReplicated; }

		static void SetDeleteReplicationComponent(std::function<void(unsigned)> aDeleteReplicatedComponent) { DeleteReplicatedComponent = aDeleteReplicatedComponent; }
		static void SetDestroyGameObjectReplicated(std::function<void(unsigned)> aDestroyGameObjectReplicated) { DestroyGameObjectReplicated = aDestroyGameObjectReplicated; }

		static void SetBeforeAfterComponentConstruction(std::function<void()> aBeforeComponentConstruction, std::function<void()> aAfterComponentConstruction)
		{
			BeforeComponentConstruction = aBeforeComponentConstruction;
			AfterComponentConstruction = aAfterComponentConstruction;
		}

		static void Init();

		static void OnAddedAllComponentsLoadScene();
		static void OnLoadScene();

		static void AwakeStartComponents();

		static void AwakeComponents();
		static void StartComponents();

		static void EditorUpdateComponents();
		static void EditorLateUpdateComponents();

		static void EarlyUpdateComponents();
		static void UpdateComponents();
		static void LateUpdateComponents();
		static void RenderComponents();

		static void SortUpdatePrioComponents();
		static void SortZIndexComponents(UpdatePriority& aVec);

		static void CheckDeletedStartComponents(std::vector<Component*>& ComponentsToStartBuffer);
		
		//void SortRenderComponents();

		static void BeginCollisions(unsigned aGOID, unsigned aGOID1);
		static void EndCollisions(unsigned aGOID, unsigned aGOID1);

		static void BeginTrigger(unsigned aGOID, unsigned aGOID1);
		static void EndTrigger(unsigned aGOID, unsigned aGOID1);

		template <typename T>
		static void GetAllComponentsOfTypePtr(std::vector<int>& aComponents);
		
		template <typename T>
		static void GetAllComponentsOfType(unsigned aGOID, std::vector<T*>& aComponents);

		template <typename T>
		static T* GetComponent(unsigned aGOID);

		template <typename T>
		static T* AddComponent(unsigned aGOID, bool IsReplicated = false);
		template <typename T>
		static T* AddComponentWithID(unsigned aGOID, unsigned aComponentID, bool IsReplicated = false);

		static void DeleteComponent(unsigned aGOID, unsigned aUniqueComponentID, unsigned aComponentID);

		static GameObject* FindObjectByName(const char* aName);


		static Component* AddComponent(unsigned aGOID, Eclipse::Component* (__cdecl* createFunc)(unsigned char* address), size_t size);
		static Component* AddComponentWithID(unsigned aGOID, unsigned aComponentID, Eclipse::Component* (__cdecl* createFunc)(unsigned char* address), size_t size);


		template <typename T>
		static unsigned GetComponentID();

		static const Eclipse::ComponentManager::RenderLayers& GetAllComponents();

		static std::vector<Component*> GetComponents(unsigned aGOID);


		static bool HasGameObject(unsigned aGOID);
		static GameObject* GetGameObject(unsigned aGOID);

		static void CommitDestroy();

		static void Destroy(unsigned aGOID);

		static unsigned GetNewGOID()
		{
			return Random::RandRange<unsigned>(0, 0x00FFFFFF);
		}

		static GameObject* CreateGameObject(GameObjectID aId);
		static GameObject* CreateGameObject();

		static void Clear();

	private:
		static std::function<void(Component*)> CreateComponentReplicated;

		static std::function<void()> BeforeComponentConstruction;
		static std::function<void()> AfterComponentConstruction;

		static std::function<void(unsigned)> DestroyGameObjectReplicated;
		static std::function<void(unsigned)> DeleteReplicatedComponent;


		static size_t myComponentMemoryTracker;
		static uint8_t* myComponentData;

		//std::vector<Component*> myRenderComponents;

		static RenderLayers myComponents;

		static std::vector<Component*> myComponentsToStartBuffer;
		static std::vector<Component*> myComponentsToStart;

		// Gameobject to components
		static std::unordered_map<unsigned, GameObject*> myEntityIdToEntity;
		static std::unordered_map<unsigned, std::vector<Component*>> myEntityIDToVectorOfComponentIDs;

		static std::vector<unsigned> gameobjectsToRemove;

		struct Graveyard
		{
			void* ptr;
			size_t size;
		};
		static std::vector<Graveyard> graveyard;
	};
}

#include "ComponentManager.inl"