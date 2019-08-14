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

if(NOT EMSCRIPTEN)
	find_package(SDL2 REQUIRED)
endif()

add_library(imgui_impl_sdl
	./impl/imgui_impl_sdl.h
	./impl/imgui_impl_sdl.cpp
)

target_include_directories(imgui_impl_sdl PUBLIC
	${SDL2_INCLUDE_DIRS}
)

if(EMSCRIPTEN)
	set_target_properties(imgui_impl_sdl PROPERTIES COMPILE_FLAGS "-s USE_SDL=2")
	set_target_properties(imgui_impl_sdl PROPERTIES LINK_FLAGS "-s USE_SDL=2")
endif()

target_link_libraries(imgui_impl_sdl
	${SDL2_LIBRARIES}
	imgui
)

########################

if(NOT EMSCRIPTEN)

add_library(imgui_impl_opengl3_glad
	./impl/imgui_impl_opengl3.h
	./impl/imgui_impl_opengl3.cpp
)

target_compile_definitions(imgui_impl_opengl3_glad PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLAD)

target_link_libraries(imgui_impl_opengl3_glad
	glad
	imgui
)

endif()

########################

add_library(imgui_impl_opengl3_es
	./impl/imgui_impl_opengl3.h
	./impl/imgui_impl_opengl3.cpp
)

target_compile_definitions(imgui_impl_opengl3_es PUBLIC IMGUI_IMPL_OPENGL_ES2)
if(EMSCRIPTEN)
	set_target_properties(imgui_impl_opengl3_es PROPERTIES COMPILE_FLAGS "-s USE_WEBGL2=1 -s FULL_ES2=1")
	#set_target_properties(imgui_impl_opengl3_es PROPERTIES LINK_FLAGS "-s USE_WEBGL2=1 -s FULL_ES3=1")
endif()

target_link_libraries(imgui_impl_opengl3_es
	imgui
	GLESv2
)

