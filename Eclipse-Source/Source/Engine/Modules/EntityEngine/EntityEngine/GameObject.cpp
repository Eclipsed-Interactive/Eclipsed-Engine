#include "GameObject.h"

namespace Eclipse
{
	
	
	void GameObject::Delete()
	{
		for (auto& Child : GetChildren())
			Child->SetParent(nullptr);
		
		if (parent)
		{
			std::vector<GameObject*>& ParentChildren = parent->GetChildren();
			for (int i = myChildIndex; i < ParentChildren.size() - 1; ++i)
			{
				ParentChildren[i] = ParentChildren[i + 1];
				ParentChildren[i]->myChildIndex = i;
			}
			
			ParentChildren.pop_back();
		}
	}
	
	template <typename T>
	std::vector<T*> GameObject::GetComponents()
	{
		return ComponentManager::GetComponent<T>(myID);
	}


	GameObject::GameObject(GameObjectID aId)
		: myID(aId)
	{
		Math::Vector4ui colorInt;

		colorInt.x = (myID & 0x000000FF) >> 0;
		colorInt.y = (myID & 0x0000FF00) >> 8;
		colorInt.z = (myID & 0x00FF0000) >> 16;

		myPixelPickColor = Math::Vector4f((float)colorInt.x / 255.f, (float)colorInt.y / 255.f, (float)colorInt.z / 255.f, 1);
	}


	GameObject*& GameObject::GetParent()
	{
		return parent;
	}
	void GameObject::SetParent(GameObject* aGO)
	{
		parent = aGO;
		if (!parent)
			return;
		
		parent->children.emplace_back(this);
		SetChildIndex(parent->GetChildCount() - 1);
	}

	GameObject*& GameObject::GetChild(int index)
	{
		return children[index];
	}
	std::vector<GameObject*>& GameObject::GetChildren()
	{
		return children;
	}
	void GameObject::AddChild(GameObject* aChild)
	{
		children.emplace_back(aChild);
		
		aChild->parent = this;
		aChild->SetChildIndex(GetChildCount() - 1);
	}
	size_t GameObject::GetChildCount()
	{
		return children.size();
	}

	void GameObject::SetChildIndex(size_t index)
	{
		myChildIndex = index;
	}
	size_t GameObject::GetChildIndex()
	{
		return myChildIndex;
	}

	void GameObject::SetName(const std::string& aName)
	{
		myName = aName;
	}

	const std::string& GameObject::GetName() const
	{
		return myName;
	}

	std::vector<Component*> GameObject::GetComponents()
	{
		return ComponentManager::GetComponents(myID);
	}

	GameObjectID GameObject::GetID() const
	{
		return myID;
	}

	GameObject::operator Eclipse::GameObjectID() const
	{
		return myID;
	}
}