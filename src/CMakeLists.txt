cmake_minimum_required(VERSION 3.7.2)

if(EMSCRIPTEN)
	add_subdirectory(./emscripten)
else()
	add_subdirectory(./demo_sdl_opengl3)
endif()

