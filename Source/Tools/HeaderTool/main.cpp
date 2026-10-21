#include <iostream>

#include "Reflection/Reflection.hpp"
#include "Core/Standard/Core/Asserts.hpp"

using namespace Eclipse;

int main(int argc, char** argv)
{
    if (argc != 4)
    {
        std::cerr << "Expected: HeaderTool <content> <output> <useStaticLinkage>\n";
        return 1;
    }

    const std::filesystem::path contentPath = argv[1];
    const std::filesystem::path generatedDestination = argv[2];
    const bool useStatic = std::string_view(argv[3]) == "true";

    if (!std::filesystem::is_directory(contentPath))
    {
        std::cerr << "Content directory missing: " << contentPath << '\n';
        return 1;
    }

    std::filesystem::create_directories(generatedDestination);
    Reflection::Reflect(contentPath, generatedDestination, useStatic);
    return 0;
}