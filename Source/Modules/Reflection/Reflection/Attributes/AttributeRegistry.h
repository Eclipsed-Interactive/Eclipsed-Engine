//#pragma once
//
//#include "Attributes.hpp"
//
//#include <unordered_map>
//#include <any>
//#include <string>
//
//#include <functional>
//
//namespace Eclipse::Reflection
//{
//	struct AttributeDescription
//	{
//		void (*createAttribute)();
//	};
//
//	class AttributeRegistry
//	{
//	public:
//		template<typename Attribute>
//		static void Register(const char* name);
//
//	private:
//		static inline std::unordered_map<std::string, std::any> registeredAttributes;
//	};
//
//	template<typename Attribute>
//	inline void AttributeRegistry::Register(const char* name)
//	{
//		registeredAttributes.insert_or_assign(std::to_string(name), Attribute{ std) })
//	}
//}