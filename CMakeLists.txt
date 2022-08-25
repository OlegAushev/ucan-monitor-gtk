# STM32 Minimal CMake project for C/C++ projects
cmake_minimum_required(VERSION 3.12)

################################################################################
project(ucan-monitor VERSION 0.1)
set(EXECUTABLE ${CMAKE_PROJECT_NAME})

################################################################################
# GTK
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK4 REQUIRED gtk4 libadwaita-1)

include_directories(${GTK4_INCLUDE_DIRS})
link_directories(${GTK4_LIBRARY_DIRS})

add_definitions(${GTK4_CFLAGS_OTHER})

################################################################################
# Set headers and sources
set(PROJECT_INCLUDE_DIRECTORIES
	${PROJECT_SOURCE_DIR}
	${PROJECT_SOURCE_DIR}/include
	${PROJECT_SOURCE_DIR}/src
)

file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS
	${PROJECT_SOURCE_DIR}/include/*.c
	${PROJECT_SOURCE_DIR}/include/*.cpp
	${PROJECT_SOURCE_DIR}/src/*.c
	${PROJECT_SOURCE_DIR}/src/*.cpp
)

################################################################################
# Set executable
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

add_executable(${EXECUTABLE}
	${PROJECT_SOURCES}
)

target_compile_definitions(${EXECUTABLE} PRIVATE
	$<$<CONFIG:Debug>:DEBUG>
	#${GTK4_CFLAGS_OTHER}
)

target_include_directories(${EXECUTABLE} PRIVATE
	${PROJECT_INCLUDE_DIRECTORIES}
)

target_link_libraries(${EXECUTABLE} ${GTK4_LIBRARIES})

################################################################################
# Set compiler options and features
target_compile_options(${EXECUTABLE} PRIVATE
	-Wall
	-Wextra
	-Wpedantic
	$<$<CONFIG:Debug>:-Og -g3 -ggdb>
	$<$<CONFIG:Release>:-Og -g0>
)

target_compile_features(${EXECUTABLE} PRIVATE
	cxx_std_17
)

################################################################################
set(CMAKE_VERBOSE_MAKEFILE OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
