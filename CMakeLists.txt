
cmake_minimum_required(VERSION 3.28)
project(Zaephus_Engine C CXX)

set_property(GLOBAL PROPERTY LINKER_LANGUAGE CXX)

set(CMAKE_CXX_STANDARD 20)

set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}")
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}")

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_INJECT_DEBUG_POSTFIX OFF CACHE BOOL "" FORCE)
set(ASSIMP_INSTALL OFF CACHE BOOL "" FORCE)
set(ASSIMP_WARNINGS_AS_ERRORS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF CACHE BOOL "" FORCE)
set(ASSIMP_NO_EXPORT ON CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_FBX_IMPORTER ON CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_OBJ_IMPORTER ON CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_GLTF_IMPORTER ON CACHE BOOL "" FORCE)

add_subdirectory(${PROJECT_SOURCE_DIR}/include/glfw include/glfw)
add_subdirectory(${PROJECT_SOURCE_DIR}/include/glad include/glad)
add_subdirectory(${PROJECT_SOURCE_DIR}/include/assimp include/assimp)
add_subdirectory(${PROJECT_SOURCE_DIR}/include/Zaephus_Math include/Zaephus_Math)

add_subdirectory(source)
add_subdirectory(examples)

target_link_libraries(Zaephus_Engine glfw)
target_link_libraries(Zaephus_Engine glad)
target_link_libraries(Zaephus_Engine assimp)
target_link_libraries(Zaephus_Engine Zaephus_Math)

target_include_directories(Zaephus_Engine PUBLIC
    ${PROJECT_SOURCE_DIR}/include/stb_image
    ${PROJECT_SOURCE_DIR}/include/Zaephus_Math/source
    source
    source/graphics
)