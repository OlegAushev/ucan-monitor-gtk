# STM32 Minimal CMake project for C/C++ projects
cmake_minimum_required(VERSION 3.12)

################################################################################
project(ucan-monitor VERSION 0.1)
set(EXECUTABLE ${CMAKE_PROJECT_NAME})

################################################################################
# THREADS
set(CMAKE_THREAD_PREFER_PTHREAD ON)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

################################################################################
# GTK and VALA
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK4 REQUIRED gtk4 libadwaita-1)

list(APPEND CMAKE_MODULE_PATH
    ${CMAKE_SOURCE_DIR}/cmake/vala
)
find_package(Vala REQUIRED)
include(${VALA_USE_FILE})

include_directories(${GTK4_INCLUDE_DIRS})
link_directories(${GTK4_LIBRARY_DIRS})

add_definitions(${GTK4_CFLAGS_OTHER})

################################################################################
# HEADERS and SOURCES
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
# XML and UI resources
find_program(GLIB_COMPILE_RESOURCES NAMES glib-compile-resources REQUIRED)
set(GRESOURCE_XML gresource.xml)
set(GRESOURCE_C ${CMAKE_CURRENT_BINARY_DIR}/gresource.c)
message(msg: ${CMAKE_CURRENT_BINARY_DIR}/${GRESOURCE_C})

add_custom_command(
	OUTPUT ${GRESOURCE_C}
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
	COMMAND ${GLIB_COMPILE_RESOURCES} --generate-source --target=${GRESOURCE_C} ${GRESOURCE_XML}
	VERBATIM
	#MAIN_DEPENDENCY ${GRESOURCE_XML}
	DEPENDS
		src/gui/control_panel.ui
		src/gui/data_tables.ui
		src/gui/window_canbusprefs.ui
		src/gui/window.ui
		src/gui/components/slider_with_text.ui
		src/gui/components/table_entry.ui
)

add_custom_target(
	gresource-target
	DEPENDS ${GRESOURCE_C}
)

################################################################################
# VALA
vala_precompile(VALA_C
	src/gui/application.vala
	src/gui/control_panel.vala
	src/gui/data_tables.vala
	src/gui/main.vala
	src/gui/window_canbusprefs.vala
	src/gui/window.vala
	src/gui/components/slider_with_text.vala
	src/gui/components/table_entry.vala
PACKAGES
	gtk4
	libadwaita-1
OPTIONS
	--gresources=${PROJECT_SOURCE_DIR}/${GRESOURCE_XML}
)


################################################################################
# EXECUTABLE
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

add_executable(${EXECUTABLE}
	${PROJECT_SOURCES}
	${GRESOURCE_C}
	${VALA_C}
)

set_source_files_properties(
	${GRESOURCE_C}
	PROPERTIES GENERATED TRUE
)

add_dependencies(${EXECUTABLE} gresource-target)

target_compile_definitions(${EXECUTABLE} PRIVATE
	$<$<CONFIG:Debug>:DEBUG>
	#${GTK4_CFLAGS_OTHER}
	STD_COUT_ENABLED
)

target_include_directories(${EXECUTABLE} PRIVATE
	${PROJECT_INCLUDE_DIRECTORIES}
)

target_link_libraries(${EXECUTABLE}
	${GTK4_LIBRARIES}
	Threads::Threads
)

################################################################################
# Compiler OPTIONS and FEATURES
target_compile_options(${EXECUTABLE} PRIVATE
	#$<$<CONFIG:Debug>:-Og -g3 -ggdb>
	#$<$<CONFIG:Release>:-Og -g0>
	-Wall
	-Wextra
	-Wpedantic
)

target_compile_features(${EXECUTABLE} PRIVATE
	c_std_11
	cxx_std_20
)

target_link_options(${EXECUTABLE} PRIVATE
	-Wl,--export-dynamic # sic! for signals
)

################################################################################
set(CMAKE_VERBOSE_MAKEFILE OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

################################################################################
# Set install target for flatpak-builder
install(TARGETS ${CMAKE_PROJECT_NAME}
	RUNTIME
	DESTINATION /app/bin
)


