cmake_minimum_required(VERSION 3.5)
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
include_directories("${CMAKE_CURRENT_BINARY_DIR}/include")

# Glob source files.
file(GLOB_RECURSE PROJECT_SOURCES LIST_DIRECTORIES false "src/*.cpp" "src/*.h" "src/*.qrc" "src/*.ui")

add_executable(linescaleGUI
    ${PROJECT_SOURCES}
    "${VERSION_FILE}"
)

add_custom_target(VersionFile ALL
    COMMAND ${CMAKE_COMMAND}
    -D "PROJECT_VERSION=${PROJECT_VERSION}"
    -D "VERSION_FILE=${VERSION_FILE}"
    -P "${CMAKE_CURRENT_LIST_DIR}/tools/cmake/version.cmake"
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Creating ${VERSION_FILE}"
)
include(tools/cmake/version.cmake)

target_link_libraries(linescaleGUI PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Charts
    Qt${QT_VERSION_MAJOR}::SerialPort
    Qt${QT_VERSION_MAJOR}::Bluetooth)

set_target_properties(linescaleGUI PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER linegrip.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    # WIN32_EXECUTABLE TRUE # Comment for std::cout and qDebug
)

# qt_finalize_executable(linescaleGUI)
include(tools/cmake/windeployqt.cmake)

if(WIN32)
    windeployqt(linescaleGUI)
endif()

# # Copy files into build directory
add_custom_command(
    TARGET linescaleGUI POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_SOURCE_DIR}/LICENSE.md
    ${CMAKE_CURRENT_BINARY_DIR}/LICENSE.md
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.md)
