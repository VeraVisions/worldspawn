cmake_minimum_required(VERSION 2.8.12 FATAL_ERROR)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
project(WorldSpawn C CXX)

option(BUILD_RADIANT "Build the GUI" ON)

if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "${PROJECT_SOURCE_DIR}/install" CACHE PATH "..." FORCE)
endif ()

#-----------------------------------------------------------------------
# Version
#-----------------------------------------------------------------------

# CMake 3.0+ would allow this in project()
set(WorldSpawn_VERSION_MAJOR 1)
set(WorldSpawn_VERSION_MINOR 1)
set(WorldSpawn_VERSION_PATCH 0)
set(WorldSpawn_VERSION "${WorldSpawn_VERSION_MAJOR}.${WorldSpawn_VERSION_MINOR}.${WorldSpawn_VERSION_PATCH}")

SET(CMAKE_C_COMPILER gcc-9)
SET(CMAKE_CXX_COMPILER g++-9)

file(WRITE "${PROJECT_BINARY_DIR}/WorldSpawn_MAJOR" ${WorldSpawn_VERSION_MAJOR})
file(WRITE "${PROJECT_BINARY_DIR}/WorldSpawn_MINOR" ${WorldSpawn_VERSION_MINOR})
file(WRITE "${PROJECT_BINARY_DIR}/WorldSpawn_PATCH" ${WorldSpawn_VERSION_PATCH})

#set(WorldSpawn_ABOUTMSG "Custom build" CACHE STRING "About message")

find_package(Git REQUIRED)
execute_process(
        COMMAND ${GIT_EXECUTABLE} rev-parse --short HEAD
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        OUTPUT_VARIABLE GIT_VERSION
        OUTPUT_STRIP_TRAILING_WHITESPACE
)
set(WorldSpawn_VERSION_STRING "${WorldSpawn_VERSION}n")
if (GIT_VERSION)
    set(WorldSpawn_VERSION_STRING "${WorldSpawn_VERSION_STRING}-git-${GIT_VERSION}")
endif ()

message(STATUS "Building ${PROJECT_NAME} ${WorldSpawn_VERSION_STRING} ${WorldSpawn_ABOUTMSG}")

#-----------------------------------------------------------------------
# Language standard
#-----------------------------------------------------------------------

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
if (CMAKE_VERSION VERSION_LESS "3.1")
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR CMAKE_COMPILER_IS_GNUCXX)
        include(CheckCXXCompilerFlag)
        check_cxx_compiler_flag(--std=c++${CMAKE_CXX_STANDARD} STD_CXX)
        if (STD_CXX)
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++${CMAKE_CXX_STANDARD}")
        else ()
            message(SEND_ERROR "Requires C++${CMAKE_CXX_STANDARD} or better")
        endif ()
    else ()
        message(WARNING "Unrecognized compiler: ${CMAKE_CXX_COMPILER_ID}, make sure it supports C++${CMAKE_CXX_STANDARD}")
    endif ()
endif ()

#-----------------------------------------------------------------------
# Flags
#-----------------------------------------------------------------------

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-exceptions -fno-rtti")
macro(addflags_c args)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${args}")
endmacro()
macro(addflags_cxx args)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${args}")
endmacro()
macro(addflags args)
    addflags_c("${args}")
    addflags_cxx("${args}")
endmacro()
addflags("-fno-strict-aliasing")
if (NOT WIN32)
    addflags("-fvisibility=hidden")
endif ()

if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
	#disabled due to GTK bug      addflags("-Werror")
    addflags("-pedantic-errors")
endif ()

addflags("-Wall")
addflags("-Wextra")
addflags("-pedantic")

addflags_c("-Wno-deprecated-declarations") # vfs.c: g_strdown

addflags("-Wno-unused-function")
addflags("-Wno-unused-variable")
addflags("-Wno-unused-parameter")

set(CMAKE_POSITION_INDEPENDENT_CODE 1)
set(GTK_TARGET 2 CACHE STRING "GTK target")
add_definitions(-DGTK_TARGET=${GTK_TARGET})

#-----------------------------------------------------------------------
# Defs
#-----------------------------------------------------------------------

add_definitions(-DWorldSpawn_VERSION="${WorldSpawn_VERSION}")
add_definitions(-DWorldSpawn_MAJOR_VERSION="${WorldSpawn_VERSION_MAJOR}")
add_definitions(-DWorldSpawn_MINOR_VERSION="${WorldSpawn_VERSION_MINOR}")
add_definitions(-DWorldSpawn_PATCH_VERSION="${WorldSpawn_VERSION_PATCH}")

add_definitions(-DWorldSpawn_ABOUTMSG="${WorldSpawn_ABOUT}")

if (NOT CMAKE_BUILD_TYPE MATCHES Release)
    add_definitions(-D_DEBUG=1)
endif ()

macro(disable_deprecated name gtk2only)
    add_definitions(-D${name}_DISABLE_SINGLE_INCLUDES)
    if ((${gtk2only} EQUAL 0) OR (GTK_TARGET EQUAL 2))
        add_definitions(-D${name}_DISABLE_DEPRECATED)
    endif ()
endmacro()

disable_deprecated(ATK 0)
disable_deprecated(G 0)
disable_deprecated(GDK 0)
disable_deprecated(GDK_PIXBUF 0)
disable_deprecated(GTK 1)
disable_deprecated(PANGO 0)

if (APPLE)
    option(XWINDOWS "Build against X11" ON)
    add_definitions(
            -DPOSIX=1
    )
elseif (WIN32)
    add_definitions(
            -DWIN32=1
            -D_WIN32=1
    )
else ()
    set(XWINDOWS ON)
    add_definitions(
            -DPOSIX=1
    )
endif ()

if (XWINDOWS)
    find_package(X11 REQUIRED)
    include_directories(${X11_INCLUDE_DIR})
    add_definitions(-DXWINDOWS=1)
endif ()

include_directories("${PROJECT_SOURCE_DIR}/include")
include_directories("${PROJECT_SOURCE_DIR}/libs")

if (WIN32 AND NOT CMAKE_CROSSCOMPILING)
    set(BUNDLE_LIBRARIES_DEFAULT ON)
else ()
    set(BUNDLE_LIBRARIES_DEFAULT OFF)
endif ()
option(BUNDLE_LIBRARIES "Bundle libraries" ${BUNDLE_LIBRARIES_DEFAULT})

macro(copy_dlls target)
    if (BUNDLE_LIBRARIES)
        add_custom_command(TARGET ${target} POST_BUILD
                COMMAND bash
                ARGS -c "ldd '$<TARGET_FILE:${target}>' | grep -v /c/Windows | awk '{ print $1 }' | while read dll; do cp \"$(which $dll)\" '${PROJECT_BINARY_DIR}'; done"
                VERBATIM
                )
    endif ()
endmacro()

#-----------------------------------------------------------------------
# Libraries
#-----------------------------------------------------------------------

add_subdirectory(libs)
add_subdirectory(include)

#-----------------------------------------------------------------------
# Plugins
#-----------------------------------------------------------------------

if (BUILD_RADIANT)
    add_subdirectory(contrib)
endif ()

#-----------------------------------------------------------------------
# Modules
#-----------------------------------------------------------------------

if (BUILD_RADIANT)
    add_subdirectory(plugins)
endif ()

#-----------------------------------------------------------------------
# Radiant
#-----------------------------------------------------------------------

if (CMAKE_EXECUTABLE_SUFFIX)
    string(REGEX REPLACE "^[.]" "" WorldSpawn_EXECUTABLE ${CMAKE_EXECUTABLE_SUFFIX})
else ()
    execute_process(
            COMMAND uname -m
            OUTPUT_VARIABLE WorldSpawn_EXECUTABLE
            OUTPUT_STRIP_TRAILING_WHITESPACE
    )
endif ()

macro(radiant_tool name)
    add_executable(${name} ${ARGN})
    install(
            TARGETS ${name}
            RUNTIME DESTINATION .
    )
    if (NOT (CMAKE_EXECUTABLE_SUFFIX STREQUAL ".${WorldSpawn_EXECUTABLE}"))
        add_custom_command(TARGET ${name} POST_BUILD
                COMMAND ln -f -s "$<TARGET_FILE_NAME:${name}>" "${PROJECT_BINARY_DIR}/${name}.${WorldSpawn_EXECUTABLE}"
                VERBATIM
                )
        install(CODE "execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink
                ${name}${CMAKE_EXECUTABLE_SUFFIX} ${CMAKE_INSTALL_PREFIX}/${name}.${WorldSpawn_EXECUTABLE})
                ")
    endif ()
endmacro()

if (BUILD_RADIANT)
    add_subdirectory(radiant _radiant)
    set_target_properties(worldspawn PROPERTIES
            COMPILE_DEFINITIONS WorldSpawn_EXECUTABLE="${WorldSpawn_EXECUTABLE}"
            )
endif ()

#-----------------------------------------------------------------------
# Tools
#-----------------------------------------------------------------------

add_subdirectory(tools)

file(GLOB DATA_FILES "${PROJECT_SOURCE_DIR}/resources/*")

if (NOT (PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR))
    # Copy data files from sources to the build directory
    message(STATUS "Copying data files")
    file(COPY ${DATA_FILES} DESTINATION "${PROJECT_BINARY_DIR}")
endif ()

#-----------------------------------------------------------------------
# Install
#-----------------------------------------------------------------------

install(
        FILES
        "${PROJECT_BINARY_DIR}/WorldSpawn_MAJOR"
        "${PROJECT_BINARY_DIR}/WorldSpawn_MINOR"
        "${PROJECT_BINARY_DIR}/WorldSpawn_PATCH"
        DESTINATION .
)

install(
        DIRECTORY
        resources/
        DESTINATION .
)

install(
        DIRECTORY
        DESTINATION .
        OPTIONAL
)

include(cmake/scripts/package.cmake)
