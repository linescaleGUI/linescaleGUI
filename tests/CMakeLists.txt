include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        58d77fa8070e8cec2dc1ed015d66b454c8d78850 # tag release-1.12.1
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE) # needed to prevent linker errors on Windows
FetchContent_MakeAvailable(googletest)
include(GoogleTest)

# Glob source files.
file(GLOB_RECURSE UNITS_SRCS LIST_DIRECTORIES false RELATIVE "${CMAKE_CURRENT_LIST_DIR}" CONFIGURE_DEPENDS "units/*.cpp" "units/*.h")
add_executable(unit_tests ${SRCS} unit_tests.cpp ${UNITS_SRCS})

target_link_libraries(unit_tests
  PRIVATE gtest libLinescaleGUI Qt5::Core
)
set_target_properties(gtest_main gmock_main PROPERTIES EXCLUDE_FROM_ALL 1 EXCLUDE_FROM_DEFAULT_BUILD 1)
if(WIN32)
    windeployqt(unit_tests NO_TRANSLATIONS)
endif()

gtest_discover_tests(unit_tests)
