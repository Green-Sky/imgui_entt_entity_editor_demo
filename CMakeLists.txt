cmake_minimum_required(VERSION 3.7.2)

#
# Building in-tree is not allowed (we take care of your craziness).
#

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR "Prevented in-tree built. Please create a build directory outside of the source code and call cmake from there. Thank you.")
endif()

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


if(NOT EMSCRIPTEN)
	add_subdirectory(./external/glad)
endif()

add_subdirectory(./external/imgui)

add_subdirectory(./external/entt)
add_subdirectory(./external/imgui_entt_entity_editor)

add_subdirectory(./src)

