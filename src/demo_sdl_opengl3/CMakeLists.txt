cmake_minimum_required(VERSION 3.7.2)

project(demo)


########################

add_executable(demo_sdl_opengl3
	main.cpp
)

target_link_libraries(demo_sdl_opengl3
	imgui
	imgui_impl_opengl3_glad
	imgui_impl_sdl

	EnTT

	imgui_entt_entity_editor
)

