#pragma once

#include <string>
#include <vector>
#include "ComponentManager.h"

#include "Core/Math/Vector/Vector4.h"

#include "Types.h"

#include "Core/GUID.h"

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

		std::vector<Component*> GetAllComponents() const;

		template <typename T>
		void GetComponents(std::vector<T*>& aVector);

		GameObject* GetTopParent();

		GameObject*& GetParent();
		const GameObject* GetParent() const;

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

		bool Enabled() const { return myIsEnabled; }
		void Enable(bool isEnabled) { myIsEnabled = isEnabled; }

	public:
		void SetName(const std::string& aName);
		const std::string& GetName() const;

		GameObjectID GetID() const;

		operator Eclipse::GameObjectID() const;

		class Transform2D* transform = nullptr;

		bool IsPrefab = false;
		std::string prefabAssetIDStr;

#ifdef ECL_EDITOR
		// Small editor-only id encoded into the pixel-picking color (a GUID does not fit in RGB8).
		unsigned GetPickID() const { return myPickID; }

		Math::Vector4f myPixelPickColor;
	private:
		unsigned myPickID = 0;
#endif
	private:
		bool myIsEnabled = true;

		GameObject* GetTopParentInternal(GameObject* aGameobject);

		GameObject* parent = nullptr;
		std::vector<GameObject*> children;
		size_t myChildIndex = 0;

		std::string myName = "Gameobject";

		bool myIsOwner = true;

	public:
		[[nodiscard]] bool GetIsStatic() const { return IsStatic; }
		void SetIsStatic(bool aIsStatic) { IsStatic = aIsStatic; }

	private:
		bool IsStatic = true;

	public:
		[[nodiscard]] GUID GetGUID() const { return id; }

	private:
		GUID id;
	};

	template<typename T>
	inline T* GameObject::AddComponent(bool IsReplicated)
	{
		return ComponentManager::template AddComponent<T>(id, IsReplicated);
	}

	template<typename T>
	inline T* GameObject::GetComponent()
	{
		return ComponentManager::template GetComponent<T>(id);
	}

	template <typename T>
	void GameObject::GetComponents(std::vector<T*>& aVector)
	{
		ComponentManager::template GetAllComponentsOfType<T>(id, aVector);
	}
}