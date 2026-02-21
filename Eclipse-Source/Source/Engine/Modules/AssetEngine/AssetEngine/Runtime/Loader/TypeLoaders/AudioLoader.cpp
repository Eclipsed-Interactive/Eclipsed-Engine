#include "AudioLoader.h"

#include <fstream>
#include "CoreEngine/MainSingleton.h"

#include "fmod/fmod.hpp"
#include "fmod/fmod_errors.h"

#include <iostream>

namespace Eclipse
{
	AudioClipData* AudioLoader::Load(std::ifstream& in) const
	{
        auto* data = new AudioClipData;

        size_t size = 0;
        in.read(reinterpret_cast<char*>(&size), sizeof(size_t));

        if (size > UINT_MAX)
            return nullptr;

        data->audioData = new char[size];
        in.read(data->audioData, size);

        FMOD_CREATESOUNDEXINFO exinfo{};
        exinfo.cbsize = sizeof(exinfo);
        exinfo.length = static_cast<unsigned int>(size);

        FMOD::System* system = MainSingleton::GetRaw<FMOD::System*>();
        unsigned int version = 0;
        FMOD_RESULT result = system->getVersion(&version);


        result = system->createSound(
            data->audioData,
            FMOD_OPENMEMORY | FMOD_CREATESTREAM,
            &exinfo,
            &data->sound
        );

        if (result != FMOD_OK)
        {
            delete data;
            return nullptr;
        }

        return data;
	}
}