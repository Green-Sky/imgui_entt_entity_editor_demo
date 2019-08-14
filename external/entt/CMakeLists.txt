#
# EnTT
#

cmake_minimum_required(VERSION 3.7.2)

#
# Project configuration
#

project(EnTT VERSION 3.1.0)

set(SETTINGS_ORGANIZATION "Michele Caini")
set(SETTINGS_APPLICATION ${PROJECT_NAME})
set(PROJECT_AUTHOR "Michele Caini")
set(PROJECT_AUTHOR_EMAIL "michele.caini@gmail.com")

message("*")
message("* ${PROJECT_NAME} v${PROJECT_VERSION} (${CMAKE_BUILD_TYPE})")
message("* Copyright (c) 2017-2019 ${PROJECT_AUTHOR} <${PROJECT_AUTHOR_EMAIL}>")
message("*")

#
# Add EnTT target
#

add_library(EnTT INTERFACE)

target_include_directories(
    EnTT INTERFACE
	"./single_include"
)

target_compile_features(EnTT INTERFACE cxx_std_17)

