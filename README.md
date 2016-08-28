# About
EzGraver is an open source software allowing users to use with laser engravers by NEJE. It supports Linux, OSX and Windows. It provides both a command line interface and a graphical user interface. The latest release is available on the [releases page](https://github.com/camrein/EzGraver/releases/latest).

![](screenshot.png)

# Command Line Interface
Beside the graphical user interface, EzGraver provides a pure command line interface too.
```bash
Usage: EzGraverCli <option> [arguments...]

Available options:
  a - Shows the available ports
  h <port> - Moves the engraver to the home position
  s <port> - Starts the engraving process with the burn time 60
  p <port> - Pauses the engraver
  r <port> - Resets the engraver
  u <port> <image> - Uploads the given image to the engraver
```

# Build Status
| Linux / OSX | Windows |
|-------------|---------|
|[![Build Status](https://travis-ci.org/camrein/EzGraver.svg?branch=master)](https://travis-ci.org/camrein/EzGraver)|[![Build Status](https://ci.appveyor.com/api/projects/status/g98wrysmliq4t8d9/branch/master?svg=true)](https://ci.appveyor.com/project/camrein/ezgraver/branch/master)|


# Building
EzGraver was developed with QT 5.7. The lowest known API-Requirement is [QT 5.4](http://doc.qt.io/qt-5.7/qtimer.html#singleShot-4). Continuous integration on Travis-CI and Tea-CI is done with at least QT 5.5.

## Windows
Download the latest QT release and build it using QT Creator. Builds have been tested on the following kits:
- Desktop QT 5.7.0 MinGW 32bit
- Desktop QT 5.7.0 MSVC2015_64bit

## OS X
First install at a sufficient QT version. In this example, QT 5.5 is being installed.
```bash
brew update
brew install qt55
brew link --force qt55
```

Now build EzGraver.
```bash
qmake -config release EzGraver.pro
make
```

Generate a *.dmg file for deployment. The install step is used to ensure that the dependencies (namely EzGraverLib) is found.
```bash
make install
macdeployqt EzGraverUi/EzGraverUi.app -dmg
```


## Ubuntu
Before loading QT from the official ubuntntu repositories, it is recommended to ensure that they contain the required version.
```bash
apt-get install build-essential qt5-qmake qtbase5-dev libqt5serialport5-dev
```

Ensure the desired QT version has been installed.
```bash
qmake -qt=qt5 -v
```

Now build EzGraver.
```bash
qmake -qt=qt5 -config release EzGraver.pro
make
```

Install the binaries.
```bash
make install
```

# Acknowledgment
Many thanks to [Frederik Andersson](https://github.com/Na1w) for reverse engineering the low-level protocol.


