#pragma once



namespace Eclipse
{
    enum class ErrorCode : int
    {
        // Do not return in your function this is internal only
        FAILED = -1,
        SUCCESS = 0x00,
        GLFW_FAILED_TO_INITILIZE = 0x01,
        GLFW_WINDOW_FAILED_TO_CREATE = 0x02,
        GLAD_FAILED_TO_INITILIZE = 0x03,
    };
}