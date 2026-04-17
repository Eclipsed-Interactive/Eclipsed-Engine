include("${CMAKE_SOURCE_DIR}/Tools/CMake/common.cmake")

CreateProject(STATIC)

target_include_directories(PhysicsEngine PRIVATE ${CoreEngine_Include})
target_link_libraries(PhysicsEngine PRIVATE CoreEngine)

# ---- Box2D
target_link_directories(PhysicsEngine PUBLIC "${CMAKE_SOURCE_DIR}/Tools/Libs/box2d")
target_link_libraries(PhysicsEngine PUBLIC debug box2d-d)
target_link_libraries(PhysicsEngine PUBLIC optimized box2d)

set_target_properties(PhysicsEngine PROPERTIES UNITY_BUILD ON UNITY_BUILD_BATCH_SIZE 50)
