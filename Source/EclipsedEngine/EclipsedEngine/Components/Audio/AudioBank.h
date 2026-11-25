#pragma once

#include <vector>
#include <string>
#include "EclipsedEngine.Core.hpp"

#include "EclipsedEngine/Components/Component.h"

namespace FMOD::Studio {
	class Bank;
}

namespace Eclipse {
	class ECL_API AudioBank : public Component
	{
		MAKE_REFLECTABLE(AudioBank);
		REFLECTION_FLAG(COMPONENT);

		BASE_SELECTION(AudioBank, 10)

	public:
		void Awake() override;
		void OnDestroy() override;

	private: // Replace this with a vector once its supported.
		[[SerializeField]] std::string bank0;
		[[SerializeField]] std::string bank1;
		[[SerializeField]] std::string bank2;
		[[SerializeField]] std::string bank3;
		[[SerializeField]] std::string bank4;

	private:
		std::vector<FMOD::Studio::Bank*> loadedBanks;
	};
}