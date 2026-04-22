#pragma once

namespace Eclipse
{
    class GPUInfo
    {
    public:
        static int GetTotalMemory();

        static int GetTotalMemoryLeft();
        static int GetCurrentMemoryUsage();
        //static int GetDedicatedMemoryUsage();
    };
}