#include "GPUInfo.h"

#include "glad/glad.h"

namespace Eclipse
{
#pragma region GPU_MEMORY

#define GPU_MEMORY_INFO_DEDICATED_VIDMEM_NVX         0x9047
#define GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX   0x9048
#define GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX 0x9049
#define GPU_MEMORY_INFO_EVICTION_COUNT_NVX           0x904A
#define GPU_MEMORY_INFO_EVICTED_MEMORY_NVX           0x904B

    int GPUInfo::GetTotalMemory()
    {
        if (!GLAD_GL_NVX_gpu_memory_info)
            return -1;

        int totalMemory = 0;
        glGetIntegerv(GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX, (GLint*)&totalMemory);

        return totalMemory;
    }

    int GPUInfo::GetTotalMemoryLeft()
    {
        if (!GLAD_GL_NVX_gpu_memory_info)
            return -1;

        int memoryLeft = 0;
        glGetIntegerv(GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX, (GLint*)&memoryLeft);

        return memoryLeft;
    }

    int GPUInfo::GetCurrentMemoryUsage()
    {
        if (!GLAD_GL_NVX_gpu_memory_info)
            return -1;

        int totalMemoryAvaible = GetTotalMemory();
        int memoryLeft = GetTotalMemoryLeft();

        int memoryUsage = totalMemoryAvaible - memoryLeft;

        return memoryUsage;
    }

    // int GPUInfo::GetDedicatedMemoryUsage()
    // {
    //     if (!GLAD_GL_NVX_gpu_memory_info)
    //         return -1;

    //     int total = GetTotalMemory();

    //     int memoryLeft = 0;
    //     glGetIntegerv(GPU_MEMORY_INFO_DEDICATED_VIDMEM_NVX, (GLint*)&memoryLeft);

    //     return total - memoryLeft;
    // }

#pragma endregion
}
