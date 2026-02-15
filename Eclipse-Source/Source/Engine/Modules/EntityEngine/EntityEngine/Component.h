#pragma once

#include "CoreEngine/Object.h"

#include <string>
#include "Types.h"
#include "CoreEngine/Macros/defines.h"

#include "CoreEngine/Math/Random.h"

namespace Eclipse
{
	namespace Editor
	{
		class SceneWindow;
		class InspectorWindow;
		class HierarchyWindow;
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
	class Component : public Object
	{
		friend class ReplicatedVariable;
		friend class Editor::EditorActions;
		friend class Editor::HierarchyWindow;
		friend class Editor::SceneWindow;
		friend class Editor::InspectorWindow;
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

		virtual unsigned GetUpdatePriority() const = 0;

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
		virtual void AfterRenderUpdate() {}

		virtual void EarlyUpdate() {}
		virtual void Update() {}
		virtual void LateUpdate() {}

		// If defined then the default inspector will be overwritten.
		virtual void OnDrawInspector() {}

		virtual void Render() {}

		virtual void OnCollisionEnter() {}
		virtual void OnCollisionExit() {}

		virtual void OnDrawGizmos() {}

		virtual const char* GetComponentName() { return "Component"; }

		bool IsOwner() { return myIsOwner; }
		void SetIsOwner(bool anIsOwner) { myIsOwner = anIsOwner; }

	protected:

	protected:
#ifdef ECLIPSED_EDITOR
		virtual void UpdateInspector() {}
#endif
		//#ifdef ECLIPSED_EDITOR
		//		void UpdateInspector() { Reflection::ReflectionManager::DrawInspector(this, GetComponentName()); }
		//#endif


	public:
		//SERIALIZED_FIELD_DEFAULT(bool, IsReplicated, true);

		bool HasBeenCreated = false;

		bool IsReplicated = false;

		GameObject* gameObject;

		unsigned myInstanceComponentID = 0;

		bool IsDeleted = false;

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