#include <iostream>

#include "Reflection/Reflection.h"
#include "Core/Standard/Core/Asserts.hpp"

using namespace Eclipse;

int main(int argc, char** argv)
{
    if (argc != 3)
    {
        std::cerr << "Expected: HeaderTool <content> <output>\n";
        return 1;
    }

    const std::filesystem::path contentPath = argv[1];
    const std::filesystem::path generatedDestination = argv[2];

    if (!std::filesystem::is_directory(contentPath))
    {
        std::cerr << "Content directory missing: " << contentPath << '\n';
        return 1;
    }

    std::filesystem::create_directories(generatedDestination);
    Reflection::Reflect(contentPath, generatedDestination);
    return 0;
}