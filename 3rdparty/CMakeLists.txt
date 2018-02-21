cmake_minimum_required(VERSION 3.0)

project(3rdparty)

# Note: All files in the cmake dir will be included as if they were in this file.
#       The paths in each .cmake file are relative to this directory not the
#       cmake directory.

IF(MSVC)
    add_definitions( /D_CRT_SECURE_NO_WARNINGS /DUNICODE /D_UNICODE /Zc:wchar_t-  )
ENDIF()

if( NOT USE_SYSTEM_LIBS OR NOT HTTPSERVER_FOUND )
    MESSAGE(STATUS "Using Bundled HttpServer")
    include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/httpserver.cmake)
else ()
    MESSAGE(STATUS "Using System HttpServer")
endif()


if (SYSTEM_LIBS_REQUIRED)
    if (NOT HTTPSERVER_FOUND )
        MESSAGE(FATAL_ERROR "Could not find all required system libraries...")
    endif()
endif()
