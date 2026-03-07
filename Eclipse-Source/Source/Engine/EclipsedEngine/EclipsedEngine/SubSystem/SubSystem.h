#pragma once 

#include "CoreEngine/Macros/defines.h"

namespace Eclipse
{
	class ECLIPSED_API SubSystem {
	public:
		virtual void Init() {};
		virtual void Execute() {};
	};
}