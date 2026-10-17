#pragma once

#include <string>
#include <vector>
#include "ComponentManager.h"

#include "Core/Math/Vector/Vector4.h"

#include "Types.h"

#include "ECS.Core.hpp"

namespace Eclipse
{

	class ComponentManager;
	class ECS_API GameObject
	{
		friend class ComponentManager;

	public:
		GameObject(GameObjectID aId);

		void Delete();
		
	public:
		template<typename T>
		T* AddComponent(bool IsReplicated = false);

		template<typename T>
		T* GetComponent();

		std::vector<Component*> GetAllComponents();

		template <typename T>
		void GetComponents(std::vector<T*>& aVector);

		GameObject* GetTopParent();

		GameObject*& GetParent();

		void SetParent(GameObject* aGO);

		GameObject*& GetChild(int index);
		std::vector<GameObject*>& GetChildren();
		void RemoveChild(int index);
		void AddChild(GameObject* aChild);
		size_t GetChildCount();
		bool HasChildren();

		size_t GetChildIndex();
		void SetChildIndex(size_t index);

		bool IsOwner() { return myIsOwner; }
		void SetIsOwner(bool anIsOwner) { myIsOwner = anIsOwner; }

	public:
		void SetName(const std::string& aName);
		const std::string& GetName() const;

		unsigned GetID() const;

		operator Eclipse::GameObjectID() const;

		class Transform2D* transform = nullptr;

		bool IsPrefab = false;
		std::string prefabAssetIDStr;

#ifdef ECL_EDITOR
		Math::Vector4f myPixelPickColor;
#endif
	private:
		GameObject* GetTopParentInternal(GameObject* aGameobject);

		GameObject* parent = nullptr;
		std::vector<GameObject*> children;
		size_t myChildIndex = 0;

		unsigned myID;
		std::string myName = "Gameobject";

		bool myIsOwner = true;

		bool IsStatic;
	};

	template<typename T>
	inline T* GameObject::AddComponent(bool IsReplicated)
	{
		return ComponentManager::template AddComponent<T>(myID, IsReplicated);
	}

	template<typename T>
	inline T* GameObject::GetComponent()
	{
		return ComponentManager::template GetComponent<T>(myID);
	}

	template <typename T>
	void GameObject::GetComponents(std::vector<T*>& aVector)
	{
		ComponentManager::template GetAllComponentsOfType<T>(myID, aVector);
	}
}