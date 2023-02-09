cmake_minimum_required(VERSION 3.19)
list(APPEND CMAKE_PREFIX_PATH $ENV{QT_DIR} $ENV{Qt5_DIR})

project(linescaleGUI VERSION 0.2 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 REQUIRED COMPONENTS Core Widgets SerialPort Bluetooth PrintSupport)# OpenGL)

set(VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/include/version.h")

# Glob source files and add some manually.
file(GLOB_RECURSE PROJECT_SOURCES LIST_DIRECTORIES false CONFIGURE_DEPENDS "src/*.cpp" "src/*.h" "src/*.ui")
file(GLOB_RECURSE QRC_SRCS LIST_DIRECTORIES false CONFIGURE_DEPENDS "assets/*.qrc")
list(REMOVE_ITEM PROJECT_SOURCES "src/main.cpp") # remove main from library
# Add custom app icon
set(APPICON_WIN "${CMAKE_CURRENT_SOURCE_DIR}/assets/app/appIcon.rc")

# Build the bulk of the project as a library, so that we can also use it for testing.
add_library(libLinescaleGUI ${PROJECT_SOURCES})
add_executable(linescaleGUI
    src/main.cpp
    "${VERSION_FILE}"
    ${QRC_SRCS}
    ${APPICON_WIN} # add custom app icon
)

target_include_directories(libLinescaleGUI SYSTEM PUBLIC lib)
target_include_directories(libLinescaleGUI PUBLIC src include "${CMAKE_BINARY_DIR}/include")

# Enable more warnings.
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

# Add generated header include dir to `libLinescaleGUI`
get_property(_isMultiConfig GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
if(_isMultiConfig)
  set(AUTOGEN_INCLUDE_DIR ${CMAKE_CURRENT_BINARY_DIR}/libLinescaleGUI_autogen/include_$<CONFIG>)
else()
  set(AUTOGEN_INCLUDE_DIR ${CMAKE_CURRENT_BINARY_DIR}/libLinescaleGUI_autogen/include)
endif()
target_include_directories(libLinescaleGUI PUBLIC
  $<BUILD_INTERFACE:${AUTOGEN_INCLUDE_DIR}>
)

# Set up dependencies.
#
# We add most dependencies only to `libLinescaleGUI` but since they're public
# all dependent targets will get the too.
set(QT_DEPENDENCIES
    Qt::Core
    Qt::Widgets
    Qt::SerialPort
    Qt::Bluetooth
    Qt::PrintSupport)
target_link_libraries(linescaleGUI PRIVATE libLinescaleGUI)
target_compile_options(linescaleGUI PRIVATE ${warning_compile_options})
target_link_libraries(libLinescaleGUI PUBLIC ${QT_DEPENDENCIES} qcustomplot)
target_compile_options(libLinescaleGUI PRIVATE ${warning_compile_options})
target_compile_definitions(libLinescaleGUI PUBLIC $<IF:$<CONFIG:Debug>,,QT_NO_DEBUG_OUTPUT>)
target_compile_definitions(linescaleGUI PUBLIC $<IF:$<CONFIG:Debug>,,QT_NO_DEBUG_OUTPUT>)

# Disable the console window for release builds on Windows.
if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
    set_target_properties(linescaleGUI PROPERTIES
        WIN32_EXECUTABLE TRUE)
    set(CMAKE_WIN32_EXECUTABLE ON)
endif()

# Deploy Qt libraries.
include(tools/cmake/windeployqt.cmake)
if(WIN32)
    windeployqt(linescaleGUI)
endif()

enable_testing()
add_subdirectory(tests)
add_subdirectory(lib)

# Packaging.
if(APPLE)
endif()
set(target ${PROJECT_NAME})

set_target_properties(${target} PROPERTIES AUTORCC_OPTIONS "--compress;9")

if(UNIX AND NOT APPLE)
    install(TARGETS ${target} RUNTIME DESTINATION bin)
elseif(WIN32)
    install(TARGETS ${target} RUNTIME DESTINATION bin)
elseif(APPLE)
    set(CMAKE_MACOSX_BUNDLE ON)
    set(CMAKE_MACOSX_RPATH ON)
    install(TARGETS ${target} BUNDLE DESTINATION .
                              RUNTIME DESTINATION .)
    # set_source_files_properties(resources/icon.icns PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
    set_target_properties(${target}
                          PROPERTIES
                          MACOSX_BUNDLE TRUE
                          MACOSX_BUNDLE_BUNDLE_NAME "${PROJECT_NAME}"
                          MACOSX_BUNDLE_INFO_STRING "${PROJECT_NAME} Copyright (c) 2023 linescaleGUI contributors"
                          #   MACOSX_BUNDLE_ICON_FILE Icon.icns
                          MACOSX_BUNDLE_GUI_IDENTIFIER linegrip.com
                          MACOSX_BUNDLE_LONG_VERSION_STRING "${PROJECT_VERSION}"
                          MACOSX_BUNDLE_SHORT_VERSION_STRING "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}"
                          MACOSX_BUNDLE_BUNDLE_VERSION "${PROJECT_VERSION}"
                          MACOSX_BUNDLE_COPYRIGHT "Copyright (c) 2023 linescaleGUI contributors"
                          )
endif()

# set(desktop.path applications)
# set(desktop.files resources/example.desktop)
# set(icon.path icons/hicolor/64x64/apps)
# set(icon.files resources/icon64.png)
# set(iconsvg.path icons/hicolor/scalable/apps)
# set(iconsvg.files resources/icon.svg)
# foreach(items IN ITEMS desktop icon iconsvg)
#     install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/${${items}.files}
#             DESTINATION share/${${items}.path}
#             PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ)
# endforeach()

if(CMAKE_BUILD_TYPE_UPPER MATCHES "^(DEBUG|RELWITHDEBINFO)$")
    set(CPACK_STRIP_FILES FALSE)
else()
    set(CPACK_STRIP_FILES TRUE)
endif()

include(${CMAKE_CURRENT_LIST_DIR}/tools/cmake/packaging.cmake)