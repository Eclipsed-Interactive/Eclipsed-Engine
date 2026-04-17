#include "Object.h"

namespace Eclipse
{
	Object::Object(unsigned instanceId) : myInstanceID(instanceId)
	{
	}

	bool Object::operator==(const Object& other) const
	{
		return myInstanceID == other.myInstanceID;
	}

	bool Object::operator!=(const Object& other) const
	{
		return myInstanceID != other.myInstanceID;
	}

	bool Object::Equals(const Object& other) const
	{
		return myInstanceID == other.myInstanceID;
	}

	const std::string& Object::GetName() const
	{
		return myName;
	}

	void Object::SetName(const std::string& aName)
	{
		myName = aName;
	}

	const char* Object::ToString() const
	{
		return myName.c_str();
	}
}