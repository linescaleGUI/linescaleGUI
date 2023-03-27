## Development setup

### Visual Studio Code

1. Open the folder in [Visual Studio Code](https://code.visualstudio.com/).
2. Install the recommended extensions.  
   (When opening the folder, there will be a notification that asks whether to install the
   recommended extensions.)
    - CMake Tools
    - C/C++
3. Platform-specific setup
    <!-- - **Windows GNU**
        1. `Ctrl+P`→ `CMake: Edit User-Local CMake Kits`
        2. Add the following kit:
           ```json
            {
                "name": "Qt MinGW",
                "compilers": {
                    "C": "<Qt Install Dir>/Tools/mingw1120_64/bin/gcc.exe",
                    "CXX": "<Qt Install Dir>/Tools/mingw1120_64/bin/g++.exe"
                },
                "environmentVariables": {
                    "QT_DIR": "<Qt Install Dir>/6.3.2/mingw_64"
                }
            }
           ```
        3. Select the new kit. -->
    - **Windows MSVC**
        1. Install [Visual Studio Build
           Tools](https://visualstudio.microsoft.com/de/downloads/#build-tools-for-visual-studio-2022)
           (`Windows SDK` and `MSVC - VS 2022`).
        2. Install `Qt 5.15` including `MSVC 2019`.
        3. Set the environment variable `QT_DIR` to `<Qt Install dir>/5.15.2/msvc2019_64`.  
           This can be done by adding
           ```json
            "cmake.environment": {
                "QT_DIR": "<Qt Dir>/5.15.2/msvc2019_64"
            },
           ```
           to the vscode settings, or adding that variable globally in Windows.
        4. Open the cloned repository in vscode.
        5. `Ctrl+P`→ `CMake: Scan for Kits`  
           Some Visual Studio Kits should have been discovered.
        6. Select the appropriate kit (e.g. `Visual Studio Build Tools 2022 Release - amd64`).
            > **Note**  
            > The default generator is [ninja-build](https://ninja-build.org/) as
            > configured in the workspace settings (`cmake.preferredGenerators`). To
            > override this generator set `cmake.generator` in your settings.
            > More information [here](https://github.com/microsoft/vscode-cmake-tools/blob/main/docs/configure.md#the-cmake-tools-configure-step).
    - **Linux**: TBD
    - **MacOS**: TBD
4. Configure the project.
5. Build the project.

## Create packages

### ZIP / Installer on windows
1. Configure and build the cmake project in release mode.
2. Run `cmake --install build/Release/ --prefix <install_dir>`
    This will install all binaries into `<install_dir>`.
3. Create packages with cpack:
    1. (ZIP) `cpack -G ZIP --config build/Release/BundleConfig.cmake`
    2. (Qt Installer Framework) `cpack -G IFW --config build/Release/BundleConfig.cmake`


### AppImage on Linux
1. Download the needed tools [Linuxdeploy](https://github.com/linuxdeploy/linuxdeploy/releases) and [Qt plugin](https://github.com/linuxdeploy/linuxdeploy-plugin-qt/releases)
2. Make the tools executable and add them to the binary directory of qt.
3. Install all required qt libraries (see below)
4. `cmake -S . -B build -DCMAKE_BUILD_TYPE=debug -G Ninja`
5. `cmake --build build`
6. `cpack -G External --config build/BundleConfig.cmake`

Required libraries (download via [aqtinstall](https://github.com/miurahr/aqtinstall))
```
libgl1-mesa-dev libpulse-dev libxcb-glx0 libxcb-icccm4 libxcb-image0 libxcb-keysyms1 
libxcb-randr0 libxcb-render-util0 libxcb-render0 libxcb-shape0 libxcb-shm0 libxcb-sync1 
libxcb-util1 libxcb-xfixes0 libxcb-xinerama0 libxcb1 libxkbcommon-dev libxcb-xkb-dev 
libxcb-image0 libxcb-keysyms1 libxcb-render-util0 libxcb-xkb1 libxkbcommon-x11-0
```
