# SimpleCGUI

A simple C GUI library to learn a bit more about the system dependant APIs.
The objective is for me to use this library for other personal projects.

## Cross-platform

- Windows : use of the windows API
- Linux : use of the X11/XLib API

## How to install

For now I guess you can just copy/paste the SimpleCGUI folder into your project

## Creating a window

First call initWindow with all parameters on Windows and NULL on Linux
Then call createWindow with all parameters on Windows (check the value on microsoft doc for dwstyle) and NULL for dwstyle on Linux
From now on, the implementation should hopefully not defer between operating systems
