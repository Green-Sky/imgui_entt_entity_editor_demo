cmake_minimum_required(VERSION 3.7.2)

add_library(systems
	./velocity.hpp
	./velocity.cpp
)

target_compile_features(systems PRIVATE cxx_std_17)

target_include_directories(systems PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(systems
	EnTT

	imgui
	imgui_entt_entity_editor
)

