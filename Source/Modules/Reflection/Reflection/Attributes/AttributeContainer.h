#pragma once

#include <unordered_map>
#include <typeindex>

#include <any>

#include "Reflection.Core.hpp"


namespace Eclipse::Reflection
{
	class REFLECTION_API AttributeContainer
	{
	public:
		AttributeContainer() = default;

	public:
		template<typename Attribute, typename... Args>
		void Add(Args&&... args);

		template<typename Attribute>
		const Attribute* Get() const ;

		template<typename Attribute>
		const Attribute* TryGet() const;

	private:
		std::unordered_map<std::type_index, std::any> attributes;
	};
}

#include "AttributeContainer.inl"