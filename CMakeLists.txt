
cmake_minimum_required(VERSION 3.28)
project(Zaephus_Engine C CXX)

set_property(GLOBAL PROPERTY LINKER_LANGUAGE CXX)

set(CMAKE_CXX_STANDARD 20)

set(ENGINE_PATH ${PROJECT_SOURCE_DIR} CACHE STRING "" FORCE)

function(CopyResources target dir)
    add_custom_command(
        TARGET ${target}
        PRE_BUILD
        COMMAND ${ENGINE_PATH}/python.exe ${ENGINE_PATH}/CopyResources.py ${CMAKE_BINARY_DIR} ${CMAKE_SOURCE_DIR} ${ENGINE_PATH}/resources ${dir}
    )
endfunction()

message(STATUS ${ENGINE_PATH})
message(STATUS ${CMAKE_SOURCE_DIR})

set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include CACHE STRING "" FORCE)

set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}")
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}")

set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

set(GLFW_BUILD_DOCS     OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_USE_HYBRID_HPG ON CACHE BOOL "" FORCE)

set(ASSIMP_BUILD_TESTS                    OFF CACHE BOOL "" FORCE)
set(ASSIMP_INJECT_DEBUG_POSTFIX           OFF CACHE BOOL "" FORCE)
set(ASSIMP_INSTALL                        OFF CACHE BOOL "" FORCE)
set(ASSIMP_WARNINGS_AS_ERRORS             OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF CACHE BOOL "" FORCE)
set(ASSIMP_NO_EXPORT                      ON CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_FBX_IMPORTER             ON CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_OBJ_IMPORTER             ON CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_GLTF_IMPORTER            ON CACHE BOOL "" FORCE)

add_subdirectory(source)
add_subdirectory(examples)

add_subdirectory(${INCLUDE_DIR}/glfw include/glfw)
add_subdirectory(${INCLUDE_DIR}/glad include/glad)
add_subdirectory(${INCLUDE_DIR}/assimp include/assimp)
add_subdirectory(${INCLUDE_DIR}/Zaephus_Math include/Zaephus_Math)

if(ENABLE_PROFILING)
    message(profiling enabled)
    set(TRACY_ENABLE    ON CACHE BOOL "" FORCE)
    set(TRACY_ON_DEMAND ON CACHE BOOL "" FORCE)

    add_subdirectory(${INCLUDE_DIR}/tracy include/tracy)
    target_link_libraries(Zaephus_Engine Tracy::TracyClient)
    target_include_directories(Zaephus_Engine PUBLIC ${INCLUDE_DIR}/tracy/public)
else()
    message(profiling disabled)
    set(TRACY_ENABLE    OFF CACHE BOOL "" FORCE)
    set(TRACY_ON_DEMAND OFF CACHE BOOL "" FORCE)
endif()

target_link_libraries(Zaephus_Engine glfw)
target_link_libraries(Zaephus_Engine glad)
target_link_libraries(Zaephus_Engine assimp)
target_link_libraries(Zaephus_Engine Zaephus_Math)

target_include_directories(Zaephus_Engine PUBLIC
    ${INCLUDE_DIR}/stb_image
    ${INCLUDE_DIR}/Zaephus_Math/source
    source
)

if(NOT ${ENGINE_PATH} EQUAL ${CMAKE_SOURCE_DIR})
    add_custom_target(copy_resources ALL)
    CopyResources(copy_resources ${CMAKE_SOURCE_DIR})

    add_dependencies(Zaephus_Engine copy_resources)
endif()