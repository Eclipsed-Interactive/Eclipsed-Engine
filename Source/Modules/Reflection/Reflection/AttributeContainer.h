#pragma once

#include <unordered_map>
#include <typeindex>

#include <any>


namespace Eclipse::Registry
{

	void t() {

	}


	struct Range 
	{
		int min, max;
	};

	class AttributeContainer
	{
	public:
		template<typename Attribute, typename... Args>
		void Add(Args... args);

		std::unordered_map<std::type_info, std::any> attributes;
	};

	template<typename Attribute, typename... Args>
	inline void AttributeContainer::Add(Args... args)
	{
		attributes.insert_or_assign(typeid(Attribute), Attribute{ std::forward(args) });
	}
}