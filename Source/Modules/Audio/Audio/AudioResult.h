#pragma once

namespace Eclipse::Audio
{
    enum class AudioError : char
    {
        NoPluginLoaded = -1,

        None = 0,

        Succeded,

        Unknown,
        InvalidArgument,
        InvalidState,
        NotInitialized,
        AlreadyInitialized,

        BackendUnavailable,
        BackendInitializationFailed,
        BackendOperationFailed,
        BackendVersionMismatch,

        DeviceUnavailable,
        DeviceLost,
        UnsupportedFormat,

        ResourceNotFound,
        ResourceAlreadyExists,
        ResourceLoadFailed,

        OutOfMemory
    };

    struct AudioResult
    {
        AudioError error;
    };
}