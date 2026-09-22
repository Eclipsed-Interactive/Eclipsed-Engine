#pragma once

namespace Eclipse::Audio
{
	enum class SpeakerMode : char
	{
        Default,
        Mono,
        Stereo,
        Surround,
        Surround51,
        Surround71
	};

    struct AudioConfig
    {
        SpeakerMode speakerMode = SpeakerMode::Default;
    };
}