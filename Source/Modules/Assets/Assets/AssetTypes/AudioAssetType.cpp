#include "AudioAssetType.h"

#include <fstream>
#include <iterator>

#include "fmod/fmod.hpp"
#include "Core/MainSingleton.h"
#include <Audio/AudioPluginLoader.h>

#include <Audio/ISound.h>

namespace Eclipse::Assets
{
	ImportedData AudioAssetType::Import(const AssetMeta& file)
	{
		ImportedAudio audio;

		audio.Storage = AudioStorage::Compressed;

		std::ifstream in(file.fullPath, std::ios::binary);
		audio.Data.assign(std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>());
		in.close();

		if (audio.Data.empty()) return audio;

		FMOD::System* system = *MainSingleton::GetPointer<FMOD::System*>();

		FMOD::Sound* sound = nullptr;
		FMOD_RESULT result = system->createSound(
			file.fullPath.generic_string().c_str(),
			FMOD_OPENONLY,
			nullptr,
			&sound
		);

		if (result != FMOD_OK) 
			return audio;

		FMOD_SOUND_TYPE type;
		FMOD_SOUND_FORMAT format;
		int channels = 0;
		int bits = 0;
		sound->getFormat(&type, &format, &channels, &bits);

		float frequency = 0.f;
		sound->getDefaults(&frequency, nullptr);

		unsigned int frameCount = 0;
		sound->getLength(&frameCount, FMOD_TIMEUNIT_PCM);

		sound->release();

		switch (type)
		{
		case FMOD_SOUND_TYPE_MPEG:      audio.Format = AudioFormat::MP3; 
			break;
		
		case FMOD_SOUND_TYPE_OGGVORBIS: audio.Format = AudioFormat::OGG; 
			break;

		case FMOD_SOUND_TYPE_WAV:       audio.Format = AudioFormat::WAV; 
			break;
		}

		audio.SampleRate = static_cast<unsigned int>(frequency);
		audio.Channels = static_cast<unsigned short>(channels);
		audio.FrameCount = frameCount;

		return audio;
	}

	ProcessedData AudioAssetType::Process(const ImportedData& file)
	{
		return file;
	}

	void AudioAssetType::Serialize(BinaryWriter& writer, const ProcessedData& data)
	{
		if (!writer.IsOpen()) return;

		const ImportedAudio& _data = std::get<ImportedAudio>(data);

		writer.Write(DATA_SIZE_PAIR(_data.Storage));
		writer.Write(DATA_SIZE_PAIR(_data.Format));
		writer.Write(DATA_SIZE_PAIR(_data.SampleRate));
		writer.Write(DATA_SIZE_PAIR(_data.FrameCount));
		writer.Write(DATA_SIZE_PAIR(_data.Channels));
		writer.Write(DATA_SIZE_PAIR(_data.Streaming));

		const size_t size = _data.Data.size();
		writer.Write(&size, sizeof(size_t));
		writer.Write(_data.Data.data(), size);
	}

	void AudioAssetType::Load(BinaryReader& reader, const AssetMeta& meta, AssetData* data)
	{
		AudioData* _data = reinterpret_cast<AudioData*>(data);

		delete _data->sound;
		_data->sound = nullptr;

		_data->guid = meta.guid;

		reader.Read(DATA_SIZE_PAIR(_data->Storage));
		reader.Read(DATA_SIZE_PAIR(_data->Format));
		reader.Read(DATA_SIZE_PAIR(_data->SampleRate));
		reader.Read(DATA_SIZE_PAIR(_data->FrameCount));
		reader.Read(DATA_SIZE_PAIR(_data->Channels));
		reader.Read(DATA_SIZE_PAIR(_data->Streaming));

		size_t size;
		reader.Read(&size, sizeof(size_t));
		_data->Data.resize(size);

		reader.Read(_data->Data.data(), size);

		if (size == 0) return;

		FMOD_CREATESOUNDEXINFO exinfo{};
		exinfo.cbsize = sizeof(exinfo);
		exinfo.length = static_cast<unsigned int>(size);

		FMOD_MODE mode = FMOD_OPENMEMORY_POINT;

		if (_data->Storage == AudioStorage::PCM)
		{
			exinfo.numchannels = _data->Channels;
			exinfo.defaultfrequency = _data->SampleRate;
			exinfo.format = _data->Format == AudioFormat::PCM32F ? FMOD_SOUND_FORMAT_PCMFLOAT : FMOD_SOUND_FORMAT_PCM16;

			mode |= FMOD_OPENRAW | FMOD_CREATESAMPLE;
		}
		else
		{
			mode |= _data->Streaming ? FMOD_CREATESTREAM : FMOD_CREATECOMPRESSEDSAMPLE;
		}

		FMOD::System* system = *MainSingleton::GetPointer<FMOD::System*>();
		const char* bytes = reinterpret_cast<const char*>(_data->Data.data());

		FMOD::Sound* sound = nullptr;
		FMOD_RESULT result = system->createSound(bytes, mode, &exinfo, &sound);

		if (result != FMOD_OK)
		{
			mode = (mode & ~FMOD_OPENMEMORY_POINT) | FMOD_OPENMEMORY;
			result = system->createSound(bytes, mode, &exinfo, &sound);
		}

		if (result != FMOD_OK) return;

		_data->sound = Audio::AudioManager::GetBackend()->CreateSound();
		_data->sound->SetSound(sound);
	}

	void AudioAssetType::LoadFromBinary(BinaryReader& reader, const AssetMeta& meta, AssetData* data)
	{
		reader.SetRead(meta.offset);

		Load(reader, meta, data);
	}
}
