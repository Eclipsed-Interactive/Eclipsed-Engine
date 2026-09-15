enable_language(CSharp)

add_executable(ReflectionGenerator    ${CMAKE_CURRENT_SOURCE_DIR}/Main.cs)

set_target_properties(ReflectionGenerator PROPERTIES
    FOLDER "Tools"

    VS_DOTNET_TARGET_FRAMEWORK_VERSION "v10.0"

    VS_GLOBAL_LangVersion "14.0"

    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/Bin"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/Bin"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/Bin"
)

set_property(TARGET ReflectionGenerator PROPERTY VS_DOTNET_REFERENCES
    "System"
    "System.Xml"
    "System.Xml.Linq"
)
