#include "AudioAssetType.h"

#include "fmod/fmod.hpp"
#include "Core/MainSingleton.h"
#include <Audio/AudioPluginLoader.h>

#include <Audio/ISound.h>

namespace Eclipse::Assets
{
	ImportedData AudioAssetType::Import(const AssetMeta& file)
	{
		ImportedAudio audio;

		FMOD::Sound* sound = nullptr;

		FMOD::System* system = *MainSingleton::GetPointer<FMOD::System*>();

		system->createSound(
			file.fullPath.generic_string().c_str(),
			FMOD_DEFAULT | FMOD_CREATESAMPLE,
			nullptr,
			&sound
		);

		unsigned length = 0;
		sound->getLength(&length, FMOD_TIMEUNIT_PCMBYTES);


		FMOD_SOUND_FORMAT format;
		int channels;
		int bits;

		sound->getFormat(nullptr, &format, &channels, &bits);


		float frequency;
		sound->getDefaults(&frequency, nullptr);

		audio.SampleRate = static_cast<uint32_t>(frequency);
		audio.Channels = static_cast<uint16_t>(channels);

		void* ptr1 = nullptr;
		void* ptr2 = nullptr;
		unsigned int len1 = 0;
		unsigned int len2 = 0;

		sound->lock(
			0,
			length,
			&ptr1, &ptr2,
			&len1, &len2
		);

		std::vector<unsigned char> rawData(len1 + len2);
		memcpy(rawData.data(), ptr1, len1);

		if (ptr2 && len2 > 0)
		{
			memcpy(rawData.data() + len1, ptr2, len2);
		}

		sound->unlock(ptr1, ptr2, len1, len2);
		sound->release();

		// FMOD_CREATESAMPLE always decodes to raw PCM, never to a compressed
		// buffer, so every branch below must produce PCM16 or PCM32F data -
		// anything stored with a mismatched Format/bit-depth pair gets
		// misread by exinfo.format on load (e.g. 8-bit samples read back as
		// 16-bit halves the apparent sample count and doubles playback speed).
		switch (format)
		{
		case FMOD_SOUND_FORMAT_PCM16:
		{
			audio.Format = AudioFormat::PCM16;
			audio.Storage = AudioStorage::PCM;
			audio.Data = std::move(rawData);
			break;
		}

		case FMOD_SOUND_FORMAT_PCMFLOAT:
		{
			audio.Format = AudioFormat::PCM32F;
			audio.Storage = AudioStorage::PCM;
			audio.Data = std::move(rawData);
			break;
		}

		case FMOD_SOUND_FORMAT_PCM8:
		{
			// Unsigned 8-bit, silence = 128.
			audio.Format = AudioFormat::PCM16;
			audio.Storage = AudioStorage::PCM;

			size_t sampleCount = rawData.size();
			audio.Data.resize(sampleCount * sizeof(int16_t));
			int16_t* out = reinterpret_cast<int16_t*>(audio.Data.data());
			for (size_t i = 0; i < sampleCount; ++i)
			{
				out[i] = static_cast<int16_t>((static_cast<int>(rawData[i]) - 128) << 8);
			}
			break;
		}

		case FMOD_SOUND_FORMAT_PCM24:
		{
			// Signed 24-bit little-endian, 3 bytes per sample.
			audio.Format = AudioFormat::PCM16;
			audio.Storage = AudioStorage::PCM;

			size_t sampleCount = rawData.size() / 3;
			audio.Data.resize(sampleCount * sizeof(int16_t));
			int16_t* out = reinterpret_cast<int16_t*>(audio.Data.data());
			for (size_t i = 0; i < sampleCount; ++i)
			{
				int32_t sample24 = (rawData[i * 3 + 0]) |
					(rawData[i * 3 + 1] << 8) |
					(rawData[i * 3 + 2] << 16);
				if (sample24 & 0x800000)
					sample24 |= ~0xFFFFFF;

				out[i] = static_cast<int16_t>(sample24 >> 8);
			}
			break;
		}

		case FMOD_SOUND_FORMAT_PCM32:
		{
			// Signed 32-bit little-endian.
			audio.Format = AudioFormat::PCM16;
			audio.Storage = AudioStorage::PCM;

			size_t sampleCount = rawData.size() / sizeof(int32_t);
			audio.Data.resize(sampleCount * sizeof(int16_t));
			const int32_t* in = reinterpret_cast<const int32_t*>(rawData.data());
			int16_t* out = reinterpret_cast<int16_t*>(audio.Data.data());
			for (size_t i = 0; i < sampleCount; ++i)
			{
				out[i] = static_cast<int16_t>(in[i] >> 16);
			}
			break;
		}

		default:
			audio.Storage = AudioStorage::Compressed;
			audio.Data = std::move(rawData);
			break;
		}

		size_t bytesPerSample = (audio.Format == AudioFormat::PCM32F) ? sizeof(float) : sizeof(int16_t);
		audio.FrameCount = audio.Channels > 0
			? static_cast<unsigned int>(audio.Data.size() / (audio.Channels * bytesPerSample))
			: 0;

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

		reader.Read(DATA_SIZE_PAIR(_data->Storage));
		reader.Read(DATA_SIZE_PAIR(_data->Format));
		reader.Read(DATA_SIZE_PAIR(_data->SampleRate));
		reader.Read(DATA_SIZE_PAIR(_data->FrameCount));
		reader.Read(DATA_SIZE_PAIR(_data->Channels));
		reader.Read(DATA_SIZE_PAIR(_data->Streaming));

		size_t size;
		reader.Read(&size, sizeof(size_t));
		_data->Data.resize(size);
		_data->guid = meta.guid;

		reader.Read(_data->Data.data(), size);



		FMOD_CREATESOUNDEXINFO exinfo{};
		exinfo.cbsize = sizeof(exinfo);
		exinfo.length = static_cast<unsigned int>(size);
		exinfo.numchannels = _data->Channels;
		exinfo.defaultfrequency = _data->SampleRate;

		switch (_data->Format)
		{
		case AudioFormat::PCM16: 
			exinfo.format = FMOD_SOUND_FORMAT_PCM16;
			break;
		case AudioFormat::PCM32F:
			exinfo.format = FMOD_SOUND_FORMAT_PCMFLOAT;
			break;
		}

		FMOD::System* system = *MainSingleton::GetPointer<FMOD::System*>();

		FMOD_MODE mode = FMOD_OPENMEMORY | FMOD_OPENRAW;

		FMOD::Sound* sound = nullptr;
		FMOD_RESULT result = system->createSound(
			reinterpret_cast<const char*>(_data->Data.data()),
			mode,
			&exinfo,
			&sound
		);

		if (result != FMOD_OK)
		{
			// log it
			return;
		}

		_data->sound = Audio::AudioManager::GetBackend()->CreateSound();
		_data->sound->SetSound(sound);
	}

	void AudioAssetType::LoadFromBinary(BinaryReader& reader, const AssetMeta& meta, AssetData* data)
	{
		AudioData* _data = reinterpret_cast<AudioData*>(data);

		data->guid = meta.guid;
		reader.SetRead(meta.offset);

		reader.Read(DATA_SIZE_PAIR(_data->Storage));
		reader.Read(DATA_SIZE_PAIR(_data->Format));
		reader.Read(DATA_SIZE_PAIR(_data->SampleRate));
		reader.Read(DATA_SIZE_PAIR(_data->FrameCount));
		reader.Read(DATA_SIZE_PAIR(_data->Channels));
		reader.Read(DATA_SIZE_PAIR(_data->Streaming));

		size_t size;
		reader.Read(&size, sizeof(size_t));
		_data->Data.resize(size);
		_data->guid = meta.guid;

		reader.Read(_data->Data.data(), size);


		FMOD_CREATESOUNDEXINFO exinfo{};
		exinfo.cbsize = sizeof(exinfo);
		exinfo.length = static_cast<unsigned int>(size);
		exinfo.numchannels = _data->Channels;
		exinfo.defaultfrequency = _data->SampleRate;

		switch (_data->Format)
		{
		case AudioFormat::PCM16:
			exinfo.format = FMOD_SOUND_FORMAT_PCM16;
			break;
		case AudioFormat::PCM32F:
			exinfo.format = FMOD_SOUND_FORMAT_PCMFLOAT;
			break;
		}

		FMOD::System* system = *MainSingleton::GetPointer<FMOD::System*>();

		FMOD_MODE mode = FMOD_OPENMEMORY | FMOD_OPENRAW;

		_data->sound = Audio::AudioManager::GetBackend()->CreateSound();
		FMOD::Sound* sound = static_cast<FMOD::Sound*>(_data->sound->GetSound());

		FMOD_RESULT result = system->createSound(
			reinterpret_cast<const char*>(_data->Data.data()),
			mode,
			&exinfo,
			&sound
		);
	}
}