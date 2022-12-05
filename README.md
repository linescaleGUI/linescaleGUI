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
3. Configure CMake kits
    - **Windows**
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
        3. Select the new kit.
    - **Unix**: TBD
4. Configure the project.
5. Build the project.