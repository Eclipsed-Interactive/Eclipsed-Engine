#pragma once

#include "EntityEngine/Component.h"
#include "EntityEngine/GameObject.h"

#include <vector>
#include <string>
#include <unordered_map>
#include <typeindex>
#include <assert.h>
#include "CoreEngine/Macros/defines.h"

#include <string>

#include <functional>

#define GetComp(Type, GOID)\
ComponentManager::GetComponent<Type>(GOID)

#define MAX_COMPONENT_MEMORY_BYTES 2'000'000

namespace Eclipse
{
	namespace Editor {
		class HierarchyWindow;
		class InspectorWindow;
		class EditorActions;
	}

	typedef unsigned ComponentIndex;

	class GameObject;
	class Component;
	class ECLIPSED_API ComponentManager
	{
		friend class Editor::EditorActions;
		friend class Editor::HierarchyWindow;
		friend class Editor::InspectorWindow;
		friend class SceneLoader;

	public:
		ComponentManager() = default;
		~ComponentManager() = default;

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
		static void AfterRenderUpdateComponents();

		static void EarlyUpdateComponents();
		static void UpdateComponents();
		static void LateUpdateComponents();
		static void RenderComponents();

		static void SortComponents();

		static void BeginCollisions(unsigned aGOID);
		static void EndCollisions(unsigned aGOID);

		template <typename T>
		static void GetAllComponentsOfType(unsigned aGOID, std::vector<T*>& aComponents);

		template <typename T>
		static T* GetComponent(unsigned aGOID);

		template <typename T>
		static T* AddComponent(unsigned aGOID, bool IsReplicated = false);
		template <typename T>
		static T* AddComponentWithID(unsigned aGOID, unsigned aComponentID, bool IsReplicated = false);

		static void DeleteComponent(unsigned aGOID, unsigned aUniqueComponentID, unsigned aComponentID);


		static Eclipse::Component* AddComponent(unsigned aGOID, Eclipse::Component* (__cdecl* createFunc)(unsigned char* address), size_t size);
		static Eclipse::Component* AddComponentWithID(unsigned aGOID, unsigned aComponentID, Eclipse::Component* (__cdecl* createFunc)(unsigned char* address), size_t size);


		template <typename T>
		static unsigned GetComponentID();

		static const std::vector<Component*>& GetAllComponents();

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
		static inline std::function<void(Component*)> CreateComponentReplicated;

		static inline std::function<void()> BeforeComponentConstruction;
		static inline std::function<void()> AfterComponentConstruction;

		static inline std::function<void(unsigned)> DestroyGameObjectReplicated;
		static inline std::function<void(unsigned)> DeleteReplicatedComponent;


		static inline size_t myComponentMemoryTracker = 0;
		static inline uint8_t* myComponentData;

		static inline std::vector<Component*> myComponents;

		static inline std::vector<Component*> myComponentsToStartBuffer;
		static inline std::vector<Component*> myComponentsToStart;

		// Gameobject to components
		static inline std::unordered_map<unsigned, GameObject*> myEntityIdToEntity;
		static inline std::unordered_map<unsigned, std::unordered_map<unsigned, std::vector<ComponentIndex>>> myEntityIDToVectorOfComponentIDs;

		static inline std::vector<unsigned> gameobjectsToRemove;

	};
}

#include "ComponentManager.inl"