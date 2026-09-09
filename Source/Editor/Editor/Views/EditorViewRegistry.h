#pragma once

#include <unordered_map>
#include <string>
#include <string_view>
#include <typeindex>

#include "EditorViewType.h"
#include "EditorViewInstance.h"

#include "Core/Math/Random.h"
#include "Core/Math/Hash.h"

namespace Eclipse::Editor
{
	class EditorViewRegistry
	{
	public:
		template<typename T>
		void RegisterView();

		const EditorViewType* GetViewType(std::string_view viewName);
		std::vector<EditorViewType*> GetViewTypes();

	private:
		std::unordered_map<std::string, EditorViewType*> registeredViewsTypes;
	};

	template<typename T>
	inline void EditorViewRegistry::RegisterView()
	{
		EditorViewType* type = new EditorViewType;
		type->name = T::GetStaticName();
		type->id = Hash::FNV1A(T::GetStaticName());

		type->create = [](int id) -> EditorViewInstance* 
			{
				EditorViewInstance* instance = new EditorViewInstance;

				if (id == -1)  instance->id = Random::Rand<int>();
				else instance->id = id;

				instance->view = new T;
				instance->idString =
					std::string(instance->view->GetName()) +
					"##" +
					std::to_string(instance->id);

				return instance;
			};

		registeredViewsTypes[type->name] = type;
	}
}