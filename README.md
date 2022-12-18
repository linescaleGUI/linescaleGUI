# linescaleGUI
![Banner](assets/logo/banner.svg)

[![CI](https://github.com/linescaleGUI/linescaleGUI/actions/workflows/ci.yml/badge.svg)](https://github.com/linescaleGUI/linescaleGUI/actions/workflows/ci.yml)
![GitHub](https://img.shields.io/github/license/linescaleGUI/linescaleGUI)
![Platforms](https://img.shields.io/badge/platforms-Linux%20%7C%20win7%20%7C%20win10-lightgrey)
![GitHub all releases](https://img.shields.io/github/downloads/linescaleGUI/linescaleGUI/total?color=lightgrey)

A cross platform GUI to interact with the [LineScale 3](https://www.linegrip.com/linescale-3/) from [LineGrip Corporation Ltd](https://www.linegrip.com/).

# Installation
Currently there is a zip file for Windows available unter [Release](https://github.com/linescaleGUI/linescaleGUI/releases).
Download this file and unzip it afterwards.
After a security prompt the application can be executed.

# Development
Current development is done using Visual Studio Code and Qt v5.15 on Windows.
The CI pipeline however builds successfully on Windows, Ubuntu and Linux.

If you are familiar with building C++ Qt desktop applications with Qt Creator, then this should be as simple as opening `CMakeLists.txt` and build/run the project.

A detailed build description for VS Code is available unter [BUILDING.md](BUILDING.md)


# License
This code is released under the [GNU General Public License version 3](http://www.gnu.org/licenses/gpl-3.0.txt).
