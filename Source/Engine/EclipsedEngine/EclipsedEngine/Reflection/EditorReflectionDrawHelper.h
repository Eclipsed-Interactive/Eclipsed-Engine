#pragma once

#include "CoreEngine/Macros/defines.h"
#include "EclipsedEngine/Reflection/SerializedVariable.h"

namespace Eclipse::Reflection
{
	template<typename T>
	class SerializedVariable;
}

class ECLIPSED_API EditorReflectionDrawHelper
{
public:
	template<typename T>
	static void DrawReflectedVariable(Eclipse::Reflection::SerializedVariable<T>& var);
};

#include "EditorReflectionDrawHelper.inl"