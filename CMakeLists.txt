#
cmake_minimum_required(VERSION 3.12)

project(ucan-monitor VERSION 0.1)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake/git-info)
include(GitInfo)
GitInfoSetup()

################################################################################
# Threads
set(CMAKE_THREAD_PREFER_PTHREAD ON)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

################################################################################
# Gtk and Vala
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK4 REQUIRED gtk4 libadwaita-1 gee-0.8)

list(APPEND CMAKE_MODULE_PATH
    ${CMAKE_SOURCE_DIR}/cmake/vala
)
find_package(Vala REQUIRED)
include(${VALA_USE_FILE})

include_directories(${GTK4_INCLUDE_DIRS})
link_directories(${GTK4_LIBRARY_DIRS})

add_definitions(${GTK4_CFLAGS_OTHER})

################################################################################
# Boost
find_package(Boost 1.56 REQUIRED COMPONENTS system filesystem iostreams)

################################################################################
# Headers and Sources
set(app_incdirs
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/src
)

file(GLOB_RECURSE app_sources CONFIGURE_DEPENDS
    ${PROJECT_SOURCE_DIR}/include/*.c
    ${PROJECT_SOURCE_DIR}/include/*.cpp
    ${PROJECT_SOURCE_DIR}/src/*.c
    ${PROJECT_SOURCE_DIR}/src/*.cpp
)

################################################################################
# XML and UI Resources
find_program(GLIB_COMPILE_RESOURCES NAMES glib-compile-resources REQUIRED)
set(gresource_xml gresource.xml)
set(gresource_c ${CMAKE_CURRENT_BINARY_DIR}/gresource.c)
#message("glib-compile-resources output: " ${gresource_c})

file(GLOB_RECURSE ui_sources RELATIVE ${PROJECT_SOURCE_DIR} CONFIGURE_DEPENDS ${PROJECT_SOURCE_DIR}/src/ui/*.ui)

add_custom_command(
    OUTPUT ${gresource_c}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMAND ${GLIB_COMPILE_RESOURCES} --generate-source --target=${gresource_c} ${gresource_xml}
    VERBATIM
    MAIN_DEPENDENCY ${gresource_xml}
    DEPENDS	${ui_sources}
)

add_custom_target(
    gresource-target
    DEPENDS ${gresource_c}
)

################################################################################
# Vala Sources
file(GLOB_RECURSE vala_sources RELATIVE ${PROJECT_SOURCE_DIR} CONFIGURE_DEPENDS
    ${PROJECT_SOURCE_DIR}/src/ui/*.vala
)

vala_precompile(vala_c
    ${vala_sources}
PACKAGES
    gtk4
    libadwaita-1
    gee-0.8
OPTIONS
    --gresources=${PROJECT_SOURCE_DIR}/${gresource_xml}
)
#message("valac output: " ${vala_c})

################################################################################
# Executable
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(executable ${CMAKE_PROJECT_NAME})

add_executable(${executable}
    ${app_sources}
    ${gresource_c}
    ${vala_c}
)

add_dependencies(${executable} gresource-target)

target_compile_definitions(${executable} PRIVATE
    $<$<CONFIG:Debug>:DEBUG>
    #${GTK4_CFLAGS_OTHER}
)

target_include_directories(${executable} PRIVATE
    ${app_incdirs}
)

target_link_libraries(${executable}
    ${GTK4_LIBRARIES}
    Threads::Threads
    git-info
)

target_link_libraries(${executable}
    Boost::system
    Boost::filesystem
    Boost::iostreams
)

################################################################################
# Compiler Options and Features
target_compile_options(${executable} PRIVATE
    #$<$<CONFIG:Debug>:-Og -g3 -ggdb>
    #$<$<CONFIG:Release>:-Og -g0>
    -Wall
    -Wextra
    -Wpedantic
    -fdiagnostics-color=always
)

target_compile_features(${executable} PRIVATE
    c_std_11
    cxx_std_23
)

target_link_options(${executable} PRIVATE
    -Wl,--export-dynamic # sic! for signals
)

################################################################################
# Misc Options
set(CMAKE_VERBOSE_MAKEFILE OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

################################################################################
# Set install target for flatpak-builder
install(TARGETS ${CMAKE_PROJECT_NAME}
    RUNTIME
    DESTINATION /app/bin
)


