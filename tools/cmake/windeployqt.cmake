find_package(Qt6Core REQUIRED)

function(windeployqt target)

    # get absolute path to qmake, then use it to find windeployqt executable
    get_target_property(_qmake_executable Qt6::qmake IMPORTED_LOCATION)
    get_filename_component(_qt_bin_dir "${_qmake_executable}" DIRECTORY)

    # POST_BUILD step
    # - after build, we have a bin/lib for analyzing qt dependencies
    # - we run windeployqt on target and deploy Qt libs

    add_custom_command(TARGET ${target} POST_BUILD
        COMMAND "${_qt_bin_dir}/windeployqt.exe"         
                --verbose 1
                # --release
                --no-svg
                # --no-opengl
                --no-opengl-sw
                # --no-compiler-runtime
                --no-system-d3d-compiler
                \"$<TARGET_FILE:${target}>\"
        COMMENT "Deploying Qt libraries using windeployqt for compilation target '${target}' ..."
    )
    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        get_filename_component(compiler_dir "${CMAKE_C_COMPILER}" DIRECTORY)
        add_custom_command(TARGET ${target} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${compiler_dir}/libgcc_s_seh-1.dll $<TARGET_FILE_DIR:${target}>
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${compiler_dir}/libstdc++-6.dll $<TARGET_FILE_DIR:${target}>
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${compiler_dir}/libwinpthread-1.dll $<TARGET_FILE_DIR:${target}>
            COMMENT "Deploy mingw runtime libraries from ${QT_MINGW}/bin"
        )
    endif()

endfunction()
