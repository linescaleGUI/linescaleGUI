# linescaleGUI
A cross platform GUI to interact with the [LineScale 3](https://www.linegrip.com/linescale-3/) from LineGrip Corporation Ltd. 

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