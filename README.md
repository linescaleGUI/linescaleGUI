# linescaleGUI
![Banner](assets/logo/banner.svg)

[![CI](https://github.com/linescaleGUI/linescaleGUI/actions/workflows/ci.yml/badge.svg)](https://github.com/linescaleGUI/linescaleGUI/actions/workflows/ci.yml)
[![Docs](https://github.com/linescaleGUI/linescaleGUI/actions/workflows/documentation.yml/badge.svg)](https://github.com/linescaleGUI/linescaleGUI/actions/workflows/documentation.yml)
![GitHub](https://img.shields.io/github/license/linescaleGUI/linescaleGUI?color=success)
![Platforms](https://img.shields.io/badge/platforms-win7%20%7C%20win10-lightgrey)
![GitHub all releases](https://img.shields.io/github/downloads/linescaleGUI/linescaleGUI/total?color=lightgrey)

A cross-platform GUI to interact with the [LineScale 3](https://www.linegrip.com/linescale-3/) from [LineGrip Corporation Ltd](https://www.linegrip.com/).

> **Note**  
> This project is a work-in-progress.

## Features
- Connecting and controlling a single LineScale 3 device.
- One interactive line graph
- USB and Bluetooth LE support.
- Currently tested platforms:
    - Windows 10/11
    - Windows 7 (USB only)

# Installation
Currently there is a zip file for Windows available unter [Release](https://github.com/linescaleGUI/linescaleGUI/releases).
Download this file and unzip it afterwards.
After a security prompt the application can be executed.

## from source

1. Configure and build the cmake project in realease mode.
2. Run `cmake --install build/Release/ --prefix <install_dir>`
    This will install all binaries into `<install_dir>`.
3. Create packages with cpack (Windows only, currently):
    1. (ZIP) `cpack -G ZIP --config build/Release/BundleConfig.cmake`
    2. (Qt Installer Framework) `cpack -G IFW --config build/Release/BundleConfig.cmake`

# Development
Current development is done using Visual Studio Code and Qt v5.15 on Windows.
The CI pipeline however builds successfully on Windows, Ubuntu and Linux.

If you are familiar with building C++ Qt desktop applications with Qt Creator, then this should be as simple as opening `CMakeLists.txt` and build/run the project.

A detailed build description for VS Code is available unter [BUILDING.md](BUILDING.md). The doxygen documentation of the codebase is available unter [github.io](https://linescaleGUI.github.io/linescaleGUI/doxygen).

# License
This code is released under the [GNU General Public License version 3](http://www.gnu.org/licenses/gpl-3.0.txt).
