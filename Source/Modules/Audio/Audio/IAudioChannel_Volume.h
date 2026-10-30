#pragma once

namespace Eclipse::Audio
{
	class AudioVolume
	{
	public:
		virtual ~AudioVolume() = default;

	public:
		float GetVolume() { return myVolume; }
		void SetVolume(float aVolume) { myVolume = aVolume; }

	public:
		__declspec(property(get = GetVolume, put = SetVolume)) float Volume;

	private:
		float myVolume = 0.f;
	};
}