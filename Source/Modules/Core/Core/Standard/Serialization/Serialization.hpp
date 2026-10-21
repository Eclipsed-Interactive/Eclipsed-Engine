#pragma once

#include <string>
#include <functional>
#include <stdexcept>

#include "Core/Standard/TypeIDGenerator.hpp"

#include "Core.Core.hpp"

namespace Eclipse::Serialization
{
	using ToJsonFunction = std::function<std::string(const void*)>;
	using FromJsonFunction = std::function<void(void*, std::string_view)>;

	using ToBinaryFunction = std::function<std::vector<std::byte>(const void*)>;

	struct JsonSerializer {
		ToJsonFunction toJson;
		FromJsonFunction fromJson;
	};

	class CORE_API Serialization final
	{
	public:
		template<typename T>
		static void RegisterJson(std::string(*toJson)(const T&), void(*fromJson)(void* v, std::string_view sv));

		template<typename T>
		static const JsonSerializer& GetJson();

	private:
		static inline std::unordered_map<unsigned long long, JsonSerializer> jsonRegistry;
	};

	template<typename T>
	inline std::string ToJson(const T& v)
	{
		return Serialization::GetJson<T>().toJson(reinterpret_cast<const void*>(&v));
	}

	template<typename T>
	inline T FromJson(std::string_view v)
	{
		T t{};
		Serialization::GetJson<T>().fromJson(&t, v);

		return t;
	}

	template<typename T>
	inline void FromJson(T* t, std::string_view v)
	{
		Serialization::GetJson<T>().fromJson(t, v);
	}

	template<typename T>
	inline std::string ToBinary(const T& v)
	{

	}

	template<typename T>
	inline void Serialization::RegisterJson(std::string(*toJson)(const T&), void(*fromJson)(void* v, std::string_view sv))
	{
		JsonSerializer serializer{
			.toJson = [toJson](const void* v) {return toJson(*reinterpret_cast<const T*>(v));},
			.fromJson = [fromJson](void* v, std::string_view sv) {return fromJson(v, sv);},
		};

		jsonRegistry.insert_or_assign(Type::GetTypeID<T>(), serializer);
	}

	template<typename T>
	inline const JsonSerializer& Serialization::GetJson()
	{
		TypeID id = Type::GetTypeID<T>();
		if (!jsonRegistry.contains(id))
			throw std::runtime_error("Serializer not registered.");

		return jsonRegistry.at(id);
	}
}