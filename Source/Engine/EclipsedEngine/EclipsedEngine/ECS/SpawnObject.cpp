#include "SpawnObject.h"

#include "rapidjson/document.h"
#include "rapidjson/rapidjson.h"

#include "EntityEngine/GameObject.h"

#include "EclipsedEngine/Reflection/Registry/ComponentRegistry.h"
#include "EclipsedEngine/Reflection/Reflection.h"
#include "EclipsedEngine/Scenes/SceneLoader.h"

namespace Eclipse
{
    void InternalSpawnObjectClass::StartChildren(std::vector<GameObject*>& aChildComponents)
    {
        for (auto& child : aChildComponents)
        {
            if (child->GetChildCount() > 0)
                StartChildren(child->GetChildren());

            for (auto& component : ComponentManager::GetComponents(child->GetID()))
                component->OnComponentAddedNoCreations();

            for (auto& component : ComponentManager::GetComponents(child->GetID()))
                component->OnSceneLoaded();

            for (auto& component : ComponentManager::GetComponents(child->GetID()))
            {
                component->OnComponentAdded();
                //component->ComponentCreated();
            }
        }
    }

    void InternalSpawnObjectClass::PasteGameObject(GameObject*& aGameObject, rapidjson::Value& gameobject, rapidjson::Document::AllocatorType& anAllocator)
    {
        aGameObject = ComponentManager::CreateGameObject();
        aGameObject->SetName(gameobject["Name"].GetString());

        for (auto& componentJson : gameobject["Components"].GetArray())
        {
            auto coIt = componentJson.MemberBegin();

            Component* component;
            component = ComponentRegistry::GetAddComponent(coIt->name.GetString())(*aGameObject, Component::GetNextComponentID());

            auto& reflectedList = Reflection::ReflectionManager::GetList();
            if (reflectedList.find(component) == reflectedList.end())
                continue;

            auto& reflectedVars = reflectedList.at(component);

            int refIndex = 0;

            for (auto varIt = coIt->value.MemberBegin(); varIt != coIt->value.MemberEnd(); varIt++)
            {
                auto& reflectedVariable = reflectedVars.at(refIndex++);
                SceneLoader::LoadType(reflectedVariable, coIt->value);
            }

            if (componentJson.HasMember("IsReplicated"))
                component->IsReplicated = componentJson["IsReplicated"].GetBool();
        }

        if (gameobject.HasMember("Children"))
        {
            auto childArray = gameobject["Children"].GetArray();
            for (auto& child : childArray)
            {
                GameObject* newGameObject;
                PasteGameObject(newGameObject, child, anAllocator);

                aGameObject->AddChild(newGameObject);
            }
        }
    }


    GameObject* InternalSpawnObjectClass::CreateObjectFromJsonString(const char* aData)
    {
        rapidjson::Document d;
        d.SetObject();
        rapidjson::Document::AllocatorType& jsonAllocator = d.GetAllocator();

        if (d.Parse(aData).HasParseError())
            return nullptr;

        if (!d.IsObject())
            return nullptr;

        if (!d.HasMember("CopyType"))
            return nullptr;
        if (d["CopyType"].GetInt() != 1)
            return nullptr;

        for (auto& gameobject : d["Gameobjects"].GetArray())
        {
            GameObject* newGameobject;
            PasteGameObject(newGameobject, gameobject, jsonAllocator);

            if (newGameobject->GetChildCount() > 0)
                StartChildren(newGameobject->GetChildren());

            auto components = ComponentManager::GetComponents(newGameobject->GetID());

            std::sort(components.begin(), components.end(), [&](Component* aComp0, Component* aComp1)
            {
                return aComp0->GetUpdatePriority() > aComp1->GetUpdatePriority();
            });

            for (auto& component : components)
                component->OnComponentAddedNoCreations();

            for (auto& component : components)
                component->OnComponentAdded();

            for (auto& component : components)
                component->OnSceneLoaded();

            return newGameobject;
        }

        return nullptr;
    }

    GameObject* InternalSpawnObjectClass::CreateObjectFromJsonStringSpecifiedIds(const char* aData,
                                                                                 int aGameobjectID, const std::vector<unsigned>& aComponentsID, bool fromReplicated)
    {
        rapidjson::Document d;
        d.SetObject();
        rapidjson::Document::AllocatorType& jsonAllocator = d.GetAllocator();

        if (d.Parse(aData).HasParseError())
            return nullptr;

        if (!d.IsObject())
            return nullptr;

        if (!d.HasMember("CopyType"))
            return nullptr;
        if (d["CopyType"].GetInt() != 1)
            return nullptr;

        for (auto& gameobject : d["Gameobjects"].GetArray())
        {
            GameObject* newGameobject;
            PasteGameObjectSpecifiedIds(newGameobject, gameobject, jsonAllocator, aGameobjectID, aComponentsID, fromReplicated);

            if (newGameobject->GetChildCount() > 0)
                StartChildren(newGameobject->GetChildren());

            auto components = ComponentManager::GetComponents(newGameobject->GetID());
            std::sort(components.begin(), components.end(), [&](Component* aComp0, Component* aComp1)
            {
                return aComp0->GetUpdatePriority() > aComp1->GetUpdatePriority();
            });

            for (auto& component : components)
                component->OnComponentAddedNoCreations();

            for (auto& component : components)
                component->OnComponentAdded();

            for (auto& component : components)
                component->OnSceneLoaded();

            return newGameobject;
        }

        return nullptr;
    }

    void InternalSpawnObjectClass::PasteGameObjectSpecifiedIds(GameObject*& aGameObject, rapidjson::Value& gameobject, rapidjson::Document::AllocatorType& anAllocator,
                                                               int aGameobjectID, const std::vector<unsigned>& aComponentsID, bool fromReplicated)
    {
        aGameObject = ComponentManager::CreateGameObject(aGameobjectID);
        aGameObject->SetName(gameobject["Name"].GetString());
        aGameObject->SetIsOwner(false);

        int iterator = 0;
        for (auto& componentJson : gameobject["Components"].GetArray())
        {
            auto coIt = componentJson.MemberBegin();

            std::string str = coIt->name.GetString();
            bool isReplicated = false;
            if (componentJson.HasMember("IsReplicated"))
            {
                isReplicated = componentJson["IsReplicated"].GetBool();
                if (!isReplicated && fromReplicated)
                    continue;
            }

            int ComponentID = aComponentsID[iterator++];

            Component* component;
            component = ComponentRegistry::GetAddComponent(coIt->name.GetString())(*aGameObject, ComponentID);

            auto& reflectedList = Reflection::ReflectionManager::GetList();
            if (reflectedList.find(component) == reflectedList.end())
                continue;

            auto& reflectedVars = reflectedList.at(component);

            int refIndex = 0;

            for (auto varIt = coIt->value.MemberBegin(); varIt != coIt->value.MemberEnd(); varIt++)
            {
                auto& reflectedVariable = reflectedVars.at(refIndex++);
                SceneLoader::LoadType(reflectedVariable, coIt->value);
            }

            component->IsReplicated = isReplicated;

            component->SetIsOwner(false);
        }

        if (gameobject.HasMember("Children"))
        {
            auto childArray = gameobject["Children"].GetArray();
            for (auto& child : childArray)
            {
                GameObject* newGameObject;
                PasteGameObject(newGameObject, child, anAllocator);

                aGameObject->AddChild(newGameObject);
            }
        }
    }
}
