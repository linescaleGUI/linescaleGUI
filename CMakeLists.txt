cmake_minimum_required(VERSION 3.19)
list(APPEND CMAKE_PREFIX_PATH $ENV{QT_DIR})

project(linescaleGUI VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 REQUIRED COMPONENTS Core Widgets Charts SerialPort Bluetooth)

set(VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/include/version.h")
include_directories("${CMAKE_CURRENT_BINARY_DIR}/include" "${CMAKE_CURRENT_BINARY_DIR}/src")

# Glob source files and add some manually.
file(GLOB_RECURSE PROJECT_SOURCES LIST_DIRECTORIES false CONFIGURE_DEPENDS "src/*.cpp" "src/*.h" "src/*.ui")
file(GLOB_RECURSE QRC_SRCS LIST_DIRECTORIES false CONFIGURE_DEPENDS "assets/*.qrc")
list(REMOVE_ITEM PROJECT_SOURCES "src/main.cpp") # remove main from library
# Add custom app icon
set(APPICON_WIN "${CMAKE_CURRENT_SOURCE_DIR}/assets/app/appIcon.rc")
set(PROJECT_SOURCES
    ${PROJECT_SOURCES}
    ${APPICON_WIN} # add custom app icon
)

add_library(libLinescaleGUI ${PROJECT_SOURCES})
add_executable(linescaleGUI
    src/main.cpp
    "${VERSION_FILE}"
    ${QRC_SRCS}
)

install(TARGETS linescaleGUI RUNTIME BUNDLE )

if(DEFINED WARNINGS_AS_ERRORS)
    if(MSVC)
        set(WARNINGS_AS_ERRORS "/WX")
    else()
        set(WARNINGS_AS_ERRORS "-Werror")
    endif()
endif()

if(MSVC)
    set(warning_compile_options /W4 ${WARNINGS_AS_ERRORS})
else()
    set(warning_compile_options -Wall -Wpedantic ${WARNINGS_AS_ERRORS})
endif()

# Custom target for populating `src/version.h.in`.
add_custom_target(VersionFile ALL
    COMMAND ${CMAKE_COMMAND}
    -D "PROJECT_VERSION=${PROJECT_VERSION}"
    -D "VERSION_FILE=${VERSION_FILE}"
    -P "${CMAKE_CURRENT_LIST_DIR}/tools/cmake/version.cmake"
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Creating ${VERSION_FILE}")
include(tools/cmake/version.cmake)

set(QT_DEPENDENCIES
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Charts
    Qt${QT_VERSION_MAJOR}::SerialPort
    Qt${QT_VERSION_MAJOR}::Bluetooth)

get_property(_isMultiConfig GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
if(_isMultiConfig)
  set(AUTOGEN_INCLUDE_DIR ${CMAKE_CURRENT_BINARY_DIR}/libLinescaleGUI_autogen/include_$<CONFIG>)
else()
  set(AUTOGEN_INCLUDE_DIR ${CMAKE_CURRENT_BINARY_DIR}/libLinescaleGUI_autogen/include)
endif()
target_include_directories(libLinescaleGUI PUBLIC
  $<BUILD_INTERFACE:${AUTOGEN_INCLUDE_DIR}>
)

target_link_libraries(linescaleGUI PRIVATE Qt${QT_VERSION_MAJOR}::Widgets libLinescaleGUI)
target_link_libraries(libLinescaleGUI PRIVATE ${QT_DEPENDENCIES})

target_compile_options(linescaleGUI PRIVATE ${warning_compile_options})
target_compile_options(libLinescaleGUI PRIVATE ${warning_compile_options})

set_target_properties(linescaleGUI PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER linegrip.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    # WIN32_EXECUTABLE TRUE # Comment for std::cout and qDebug
)

include(tools/cmake/windeployqt.cmake)
if(WIN32)
    windeployqt(linescaleGUI)
endif()

# # Copy LICENSE.md into build directory
add_custom_command(
    TARGET linescaleGUI POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_SOURCE_DIR}/LICENSE.md
    ${CMAKE_CURRENT_BINARY_DIR}/LICENSE.md
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.md)

enable_testing()
add_subdirectory(tests)