#pragma once

#include <vector>
#include <string>


#include "EclipsedEngine/Components/Component.h"

namespace FMOD::Studio {
	class Bank;
}

namespace Eclipse {
	class ECLIPSED_API AudioBank : public Component
	{
		BASE_SELECTION(AudioBank, 10)

	public:
		void Awake() override;
		void OnDestroy() override;

	private: // Replace this with a vector once its supported.
		SERIALIZED_FIELD(std::string, bank0);
		SERIALIZED_FIELD(std::string, bank1);
		SERIALIZED_FIELD(std::string, bank2);
		SERIALIZED_FIELD(std::string, bank3);
		SERIALIZED_FIELD(std::string, bank4);

	private:
		std::vector<FMOD::Studio::Bank*> loadedBanks;
	};
}