cmake_minimum_required(VERSION 3.10...3.25)

project ("ZenonEngine" VERSION 0.1)

# C++20 standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Let us use our *.cmake files now
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/CMake")
include(CommonInc)

set(FETCHCONTENT_BASE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty" CACHE PATH "Base directory for FetchContent" FORCE)

configure_file(Config/RootDirectory.h.in Config/RootDirectory.h)
include_directories(${CMAKE_BINARY_DIR}/Config)

#set(CMAKE_SUPPRESS_REGENERATION true)

if(WIN32)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/Build/lib/Win32/$<CONFIG>")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/Build/lib/Win32/$<CONFIG>")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/Build/bin/Win32/$<CONFIG>")
    set(CMAKE_COMPILE_PDB_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/Build/bin-int/Win32/$<CONFIG>")
endif()

if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
	if (NOT DEFINED NOMINMAX)
		add_definitions(-DNOMINMAX)	# Entt warning, see https://github.com/skypjack/entt/wiki/Frequently-Asked-Questions#warning-C4003-the-min-the-max-and-the-macro
    endif()
    
    # Enable multiple processor compilation
    if(ENABLE_MULTICORE)
        SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /MP")
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
    endif()
endif()

# Add the engine and tests
add_subdirectory ("Engine")

# Add a subdirectory that uses the ZenonEngine and produces an executeable
add_subdirectory ("Sandbox")

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Sandbox)

zn_log("Compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
zn_log("CMake module path: ${CMAKE_MODULE_PATH}")