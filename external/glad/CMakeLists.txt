cmake_minimum_required(VERSION 3.7.2)

project(glad)

add_library(glad
	./include/glad/glad.h
	./include/KHR/khrplatform.h

	./src/glad.c
)

target_include_directories(glad PUBLIC
	"${CMAKE_CURRENT_SOURCE_DIR}/include"
)

if(UNIX)
	target_link_libraries(glad dl)
endif()


