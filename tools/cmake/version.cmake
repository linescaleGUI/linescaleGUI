# A cmake script to generate a C++ header containing:
# - version
# - build time
# - build date
# - git revision
# - remote repository URL
# - whether the git repo is dirty

# Inputs to the script:
# - `PROJECT_VERSION`
# - `VERSION_FILE`

find_package(Git REQUIRED)
execute_process(COMMAND "${GIT_EXECUTABLE}" rev-parse HEAD OUTPUT_VARIABLE GIT_REV OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND "${GIT_EXECUTABLE}" config --get remote.origin.url OUTPUT_VARIABLE GIT_REPO_URL OUTPUT_STRIP_TRAILING_WHITESPACE)
string(TIMESTAMP BUILD_DATE "%Y-%m-%d")
string(TIMESTAMP BUILD_TIME "%H:%M:%S")

execute_process(COMMAND "${GIT_EXECUTABLE}" diff-index --quiet HEAD -- RESULT_VARIABLE ret)
if(ret EQUAL "1")
    set(GIT_REPO_DIRTY "true")
else()
    set(GIT_REPO_DIRTY "false")
endif()

configure_file("src/version.h.in" "${VERSION_FILE}")
