#pragma once

#include <memory>
#include <unordered_map>
#include <filesystem>
#include <typeindex>

#include "EclipsedEngine/Editor/Windows/WindowTypes/Inspector/ITypeInspector.h"

namespace Eclipse::Editor {
	class AssetInspectorRegistry {
	public:
		template<typename T>
		static IInspector* Find();
		static IInspector* Find(const std::filesystem::path& path);
		static void SetFallback(std::unique_ptr<IInspector> inspector);

		static void RegisterAll();

	private:
		template<typename T, typename... Args>
		static T* Register(Args&&... args);

		std::unordered_map<std::type_index, std::unique_ptr<IInspector>> inspectors;
		std::unique_ptr<IInspector> fallbackInspector;

		static AssetInspectorRegistry& Get();
	};

	template<typename T, typename ...Args>
	inline T* AssetInspectorRegistry::Register(Args && ...args)
	{
		auto ptr = std::make_unique<T>(std::forward<Args>(args)...);
		T* raw = ptr.get();
		Get().inspectors[typeid(T)] = std::move(ptr);

		return raw;
	}

	template<typename T>
	inline IInspector* AssetInspectorRegistry::Find()
	{
		auto it = Get().inspectors.find(typeid(T));
		if (it == Get().inspectors.end()) {
			return Get().fallbackInspector.get();
		}

		return it->second.get();
	}
}