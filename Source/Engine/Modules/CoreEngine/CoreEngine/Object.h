#pragma once

#include <string>




namespace Eclipse
{
	class Object
	{
	public:
		Object() = default;
		Object(unsigned instanceId);
		virtual ~Object() = default;

		Object(const Object&) = delete;
		Object& operator=(const Object&) = delete;

		bool operator==(const Object& other) const;
		bool operator!=(const Object& other) const;
		bool Equals(const Object& other) const;

	public:
		const std::string& GetName() const;
		void SetName(const std::string& aName);

		const char* ToString() const;

	protected:
		std::string myName = "";
		unsigned myInstanceID = 0;
	};
}