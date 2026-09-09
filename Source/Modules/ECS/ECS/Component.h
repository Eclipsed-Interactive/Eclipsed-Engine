#pragma once

#include <string>
#include "Types.h"

#include "Core/Math/Random.h"

#include "ECS.Core.hpp"

namespace Eclipse
{
	namespace Editor
	{
		class SceneView;
		class GameObjectInspector;
		class HierarchyView;
		class EditorActions;
	}
	namespace Reflection
	{
		class ReflectionManager;
		class AbstractSerializedVariable;
	}
}

namespace Eclipse
{
	typedef unsigned RegisteredTypeIndex;


	class GameObject;
	class ComponentManager;
	class ECS_API Component
	{
		friend class ReplicatedVariable;
		friend class Editor::EditorActions;
		friend class Editor::HierarchyView;
		friend class Editor::SceneView;
		friend class Editor::GameObjectInspector;
		friend class Reflection::ReflectionManager;
		friend class Reflection::AbstractSerializedVariable;

		friend class ComponentManager;
		friend class SceneLoader;

	public:
		Component() = default;
		virtual ~Component() = default;

		static unsigned GetNextComponentID()
		{
			return Random::Rand<unsigned>();
		}

		virtual float GetZIndex() { return -1; }

		void SetComponentID()
		{
			myInstanceComponentID = GetNextComponentID();

			// If ID is 0 then try again once only
			if (!myInstanceComponentID)
				GetNextComponentID();
		}

		void SetComponentID(unsigned compID)
		{
			myInstanceComponentID = compID;
		}

		void UpdateReplicationVars();

		virtual unsigned GetUpdatePriority() const { return 0; }

	public:
		//void ComponentCreated() { HasBeenCreated = true; }
		
		virtual void OnSceneLoaded() {}

		virtual void OnComponentAddedNoCreations() {}

		virtual void OnComponentAdded() {}
		virtual void OnDestroy() {}

		virtual void Awake() {}
		virtual void Start() {}
		virtual void StartNextFrame() {}

		virtual void EditorUpdate() {}
		virtual void EditorLateUpdate() {}

		virtual void EarlyUpdate() {}
		virtual void Update() {}
		virtual void LateUpdate() {}

#ifdef ECL_EDITOR
		virtual bool OnDrawInspector() { return false; }
#endif

		virtual void Render() {}

		virtual void OnCollisionEnter(Eclipse::GameObject* Other) {}
		virtual void OnCollisionExit(Eclipse::GameObject* Other) {}

		virtual void OnTriggerEnter(Eclipse::GameObject* Other) {}
		virtual void OnTriggerExit(Eclipse::GameObject* Other) {}

		virtual void OnDrawGizmos() {}

		virtual const char* GetComponentName() { return "Component"; }

		bool IsOwner() { return myIsOwner; }
		void SetIsOwner(bool anIsOwner) { myIsOwner = anIsOwner; }

	protected:

	protected:
#ifdef ECL_EDITOR
		virtual void UpdateInspector() {}
#endif
		//#ifdef ECL_EDITOR
		//		void UpdateInspector() { Reflection::ReflectionManager::DrawInspector(this, GetComponentName()); }
		//#endif


	public:
		//SERIALIZED_FIELD_DEFAULT(bool, IsReplicated, true);
		
		bool HasStarted = false;
		bool HasBeenCreated = false;

		bool IsReplicated = false;

		GameObject* gameObject;

		unsigned myInstanceComponentID = 0;

		bool IsDeleted = false;

		size_t componentSize = 0;

		RegisteredTypeIndex myComponentComponentID;

	protected:
		unsigned myComponentIndex = 0;

	protected:
		// IF DEF EDITOR
		bool myInspectorWasDrawn = false;

		virtual bool HasDrawInspector() const { return false; }
		virtual bool DrawDefaultInspector() const { return true; }

		bool myIsOwner = true;
		// Used with replication macros
		int ReplicationIndex = 0;
	};
}