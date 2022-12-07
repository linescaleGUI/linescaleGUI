include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        58d77fa8070e8cec2dc1ed015d66b454c8d78850 # tag release-1.12.1
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Glob source files.
file(GLOB_RECURSE SRCS LIST_DIRECTORIES false "units/*.cpp" "units/*.h")
add_executable(unit_tests ${SRCS} all_unit_tests.cpp)

target_link_libraries(unit_tests
  PRIVATE gtest libLinescaleGUI Qt5::Core
)
if(WIN32)
    windeployqt(unit_tests)
endif()

add_test(unit_tests unit_tests)