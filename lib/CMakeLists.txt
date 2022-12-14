include(FetchContent)

FetchContent_Declare(JKQTPlotter
    GIT_REPOSITORY https://github.com/jkriege2/JKQtPlotter.git
    GIT_TAG d45083ee9bdd96de1a381ad0d63c4ba8ef547e5c # tag v4.0.0
    GIT_SHALLOW 1
    GIT_PROGRESS 1
    SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/JKQtPlotter"
)
set(JKQtPlotter_BUILD_SHARED_LIBS OFF)
set(JKQtPlotter_BUILD_STATIC_LIBS ON)
set(JKQtPlotter_BUILD_EXAMPLES OFF)
set(JKQtPlotter_BUILD_TOOLS OFF)
set(JKQtPlotter_BUILD_TOOLS OFF)
FetchContent_MakeAvailable(JKQTPlotter)

set_directory_properties(PROPERTIES EXCLUDE_FROM_ALL 1 EXCLUDE_FROM_DEFAULT_BUILD 1)
if(MSVC)
    add_compile_options(/W0)
else()
    add_compile_options(-w)
endif()

# add_library(qcustomplot OBJECT "QCustomPlot/qcustomplot.cpp")
# if(MSVC)
#     target_compile_options(qcustomplot PRIVATE /W0)
# else()
#     target_compile_options(qcustomplot PRIVATE -w)
# endif()

# # TODO: high-dpi screens
# find_package(OpenGL REQUIRED)
# target_link_libraries(qcustomplot PRIVATE Qt::PrintSupport Qt::Widgets Qt::Core opengl32)
# target_compile_definitions(qcustomplot PUBLIC QCUSTOMPLOT_USE_OPENGL)
