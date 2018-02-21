########################################################
#  
#  This is a CMake configuration file.
#  To use it you need CMake which can be 
#  downloaded from here: 
#    http://www.cmake.org/cmake/resources/software.html
#
#########################################################

cmake_minimum_required( VERSION 3.0.0 ) 

project( httpserver )

set( HSERVER_LIBRARIES ${PROJECT_NAME} CACHE INTERNAL "" )
set( HSERVER_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME} CACHE INTERNAL "" )

#########################################################

# Qt5 packages minimum version 5.4
find_package(Qt5 5.4 COMPONENTS Core Network Svg WebKit WebKitWidgets Widgets Xml XmlPatterns Concurrent PrintSupport LinguistTools)
set(CMAKE_AUTOMOC ON)

set( MAIN_FILES
    ${PROJECT_NAME}/httpglobal.h
    ${PROJECT_NAME}/httpglobal.cpp
    ${PROJECT_NAME}/httplistener.h
    ${PROJECT_NAME}/httplistener.cpp
    ${PROJECT_NAME}/httpconnectionhandler.h
    ${PROJECT_NAME}/httpconnectionhandler.cpp
    ${PROJECT_NAME}/httpconnectionhandlerpool.h
    ${PROJECT_NAME}/httpconnectionhandlerpool.cpp
    ${PROJECT_NAME}/httprequest.h
    ${PROJECT_NAME}/httprequest.cpp
    ${PROJECT_NAME}/httpresponse.h
    ${PROJECT_NAME}/httpresponse.cpp
    ${PROJECT_NAME}/httpcookie.h
    ${PROJECT_NAME}/httpcookie.cpp
    ${PROJECT_NAME}/httprequesthandler.h
    ${PROJECT_NAME}/httprequesthandler.cpp
    ${PROJECT_NAME}/httpsession.h
    ${PROJECT_NAME}/httpsession.cpp
    ${PROJECT_NAME}/httpsessionstore.h
    ${PROJECT_NAME}/httpsessionstore.cpp
    ${PROJECT_NAME}/staticfilecontroller.h
    ${PROJECT_NAME}/staticfilecontroller.cpp
)


set( RAW_SOURCES ${MAIN_FILES} )

#########################################################


# Define the Aquarius version string for use in source files
# set_source_files_properties( Misc/Utility.cpp PROPERTIES COMPILE_DEFINITIONS AQUARIUS_FULL_VERSION="${VIRGO_FULL_VERSION}" )
# set_source_files_properties( Dialogs/About.cpp PROPERTIES COMPILE_DEFINITIONS VIRGO_FULL_VERSION="${VIRGO_FULL_VERSION}" )
# set_property( SOURCE Dialogs/About.cpp APPEND PROPERTY COMPILE_DEFINITIONS VIRGO_FULL_VERSION="${VIRGO_FULL_VERSION}" VIRGO_CODE_NAME="${VIRGO_CODE_NAME}" )

#########################################################

# Adds folders for Visual Studio solution explorer (and for Xcode explorer)
source_group( "Main Files"        		FILES ${MAIN_FILES} )


#########################################################

set( ALL_SOURCES ${RAW_SOURCES} ${UI_FILES_H} ${QRC_FILES_CPP} ${QM_FILES} )

#########################################################

# We need to pick up the ui*.h files,
# and the headers for the linked-to libraries.
# The directories are in reverse order because we're using before to
# put our include dirs before any system ones.
include_directories( BEFORE
                     ${CMAKE_CURRENT_SOURCE_DIR}
                     ${CMAKE_CURRENT_BINARY_DIR} )

#########################################################

add_library( ${PROJECT_NAME} ${ALL_SOURCES} )

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}
)

target_link_libraries( ${PROJECT_NAME} )
qt5_use_modules(${PROJECT_NAME} Widgets Xml XmlPatterns PrintSupport Svg WebKit WebKitWidgets Network Concurrent Multimedia)

#########################################################

# Speed up stirng building
add_definitions( -DQT_USE_FAST_CONCATENATION )
add_definitions( -DQT_USE_FAST_OPERATOR_PLUS )

#########################################################

# Special compiler and linker flags for MSVC
if( MSVC )
    add_definitions( /DUNICODE /D_UNICODE /DHAVE_ROUND )
    # The /Zc:wchar_t- flag can't go into add_definitions
    # because the RC compiler picks it up too and it provokes a name clash
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zc:wchar_t- /MP")
    set( CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Oi /GL" ) 
    set_target_properties( ${PROJECT_NAME} PROPERTIES STATIC_LIBRARY_FLAGS "/LTCG" )

# "Print all warnings" flag for GCC
elseif( CMAKE_COMPILER_IS_GNUCXX )
    add_definitions( -Wall )
endif()

#########################################################
