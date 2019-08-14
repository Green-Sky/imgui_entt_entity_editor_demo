cmake_minimum_required(VERSION 3.7.2)

project(demo)


########################

add_executable(demo_emscripten_gles3
	main.cpp
)

set_target_properties(demo_emscripten_gles3 PROPERTIES COMPILE_FLAGS "-s USE_SDL=2")
set_target_properties(demo_emscripten_gles3 PROPERTIES LINK_FLAGS "-s USE_SDL=2")

target_link_libraries(demo_emscripten_gles3
	imgui
	imgui_impl_sdl
	imgui_impl_opengl3_es

	EnTT

	imgui_entt_entity_editor
)

