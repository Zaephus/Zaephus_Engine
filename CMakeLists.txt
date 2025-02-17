
cmake_minimum_required(VERSION 3.28)
project(Zaephus_Engine C CXX)

set_property(GLOBAL PROPERTY LINKER_LANGUAGE CXX)

set(CMAKE_CXX_STANDARD 20)

function(CopyResources target dir)
    add_custom_command(
        TARGET ${target}
        PRE_BUILD
        COMMAND ${Python_EXECUTABLE} ${CMAKE_SOURCE_DIR}/CopyResources.py ${CMAKE_BINARY_DIR} ${PROJECT_SOURCE_DIR}/source ${dir}
    )
endfunction()

set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}")
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}")

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_USE_HYBRID_HPG ON CACHE BOOL "" FORCE)

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

message(before adding subdirectories)

add_subdirectory(source)
add_subdirectory(examples)

message(after adding subdirectories)

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