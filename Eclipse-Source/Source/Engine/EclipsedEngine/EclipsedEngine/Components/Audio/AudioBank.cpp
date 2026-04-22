#include "AudioBank.h"

#include "fmod/fmod_studio.h"
#include "fmod/fmod_studio.hpp"


#include "CoreEngine/MainSingleton.h"
#include "CoreEngine/PathManager.h"

namespace Eclipse {
	void AudioBank::Awake()
	{
		FMOD::Studio::System* studio = MainSingleton::GetRaw<FMOD::Studio::System*>();

		if (!bank0->empty())
		{
			FMOD::Studio::Bank* bank;
			FMOD_RESULT result = studio->loadBankFile(
				(PathManager::GetAssetsPath() / bank0.Get()).generic_string().c_str(),
				FMOD_STUDIO_LOAD_BANK_NORMAL,
				&bank
			);

			if (result == FMOD_OK) {
				loadedBanks.push_back(bank);
			}
		}

		if (!bank1->empty())
		{
			FMOD::Studio::Bank* bank;
			FMOD_RESULT result = studio->loadBankFile(
				(PathManager::GetAssetsPath() / bank1.Get()).generic_string().c_str(),
				FMOD_STUDIO_LOAD_BANK_NORMAL,
				&bank
			);

			if (result == FMOD_OK) {
				loadedBanks.push_back(bank);
			}
		}

		if (!bank2->empty())
		{
			FMOD::Studio::Bank* bank;
			FMOD_RESULT result = studio->loadBankFile(
				(PathManager::GetAssetsPath() / bank2.Get()).generic_string().c_str(),
				FMOD_STUDIO_LOAD_BANK_NORMAL,
				&bank
			);

			if (result == FMOD_OK) {
				loadedBanks.push_back(bank);
			}
		}

		if (!bank3->empty())
		{
			FMOD::Studio::Bank* bank;
			FMOD_RESULT result = studio->loadBankFile(
				(PathManager::GetAssetsPath() / bank3.Get()).generic_string().c_str(),
				FMOD_STUDIO_LOAD_BANK_NORMAL,
				&bank
			);

			if (result == FMOD_OK) {
				loadedBanks.push_back(bank);
			}
		}

		if (!bank4->empty())
		{
			FMOD::Studio::Bank* bank;
			FMOD_RESULT result = studio->loadBankFile(
				(PathManager::GetAssetsPath() / bank4.Get()).generic_string().c_str(),
				FMOD_STUDIO_LOAD_BANK_NORMAL,
				&bank
			);

			if (result == FMOD_OK) {
				loadedBanks.push_back(bank);
			}
		}
	}

	void AudioBank::OnDestroy()
	{
		for (FMOD::Studio::Bank* bank : loadedBanks) {
			bank->unload();
		}
	}
}
