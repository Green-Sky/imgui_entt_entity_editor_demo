cmake_minimum_required(VERSION 3.7.2)

project(imgui)


add_library(imgui
	./imgui.h
	./imgui_internal.h
	./imstb_rectpack.h
	./imstb_textedit.h
	./imstb_truetype.h

	./imgui.cpp
	./imgui_demo.cpp
	./imgui_draw.cpp
	./imgui_widgets.cpp
)

target_include_directories(imgui PUBLIC
	"${CMAKE_CURRENT_SOURCE_DIR}"
)

########################

find_package(SDL2 REQUIRED)

add_library(imgui_impl_sdl
	./impl/imgui_impl_sdl.h
	./impl/imgui_impl_sdl.cpp
)

target_include_directories(imgui_impl_sdl PUBLIC
	${SDL2_INCLUDE_DIRS}
)

target_link_libraries(imgui_impl_sdl
	${SDL2_LIBRARIES}
	imgui
)

########################

add_library(imgui_impl_opengl3
	./impl/imgui_impl_opengl3.h
	./impl/imgui_impl_opengl3.cpp
)

target_compile_definitions(imgui_impl_opengl3 PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLAD)

target_link_libraries(imgui_impl_opengl3
	glad
	imgui
)

