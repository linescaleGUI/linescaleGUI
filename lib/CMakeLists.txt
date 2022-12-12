include(FetchContent)

FetchContent_Populate(qcustomplot
    URL https://www.qcustomplot.com/release/2.1.1/QCustomPlot-source.tar.gz
    URL_HASH SHA256=5e2d22dec779db8f01f357cbdb25e54fbcf971adaee75eae8d7ad2444487182f
    SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/QCustomPlot"
)

add_library(qcustomplot OBJECT "QCustomPlot/qcustomplot.cpp")
if(MSVC)
    target_compile_options(qcustomplot PRIVATE /W0)
else()
    target_compile_options(qcustomplot PRIVATE -w)
endif()

# TODO: high-dpi screens
find_package(OpenGL REQUIRED)
target_link_libraries(qcustomplot PRIVATE Qt::PrintSupport Qt::Widgets Qt::Core opengl32)
target_compile_definitions(qcustomplot PUBLIC QCUSTOMPLOT_USE_OPENGL)
