markdown# Image2Mosaic

Image2Mosaic is a Mac application built with Qt that processes images into beautiful mosaics. This application is configured to build natively as a Universal Binary, supporting both Intel (`x86_64`) and Apple Silicon (`arm64`) Macs.

## Features
* Native Mac interface using Qt Widgets.
* Exif metadata management powered by Exiv2.
* Universal binary support for the Mac App Store.

## Prerequisites & Installation

To compile this project, your system must have the required development libraries installed as Universal Binaries inside `/usr/local/universal/`.

### 1. Required Libraries
The project links statically against the following libraries:
* `exiv2`
* `inih` (with `inireader`)
* `expat`

### 2. Standard Local Compilation (Apple Silicon Only)
If you only wish to compile and test the application locally on an Apple Silicon Mac without building an Intel slice, modify the `Image2Mosaic.pro` file to target `arm64` only and use standard Homebrew paths:

```qmake
QMAKE_APPLE_DEVICE_ARCHS = arm64
INCLUDEPATH += /opt/homebrew/include
LIBS += -L/opt/homebrew/lib -lexiv2 -linih -lexpat
```

Ensure you have installed the dependencies via Homebrew first:
```bash
brew install exiv2 inih expat
```

## How to Build

1. Clone this repository to your local machine.
2. Open `Image2Mosaic.pro` in **Qt Creator**.
3. Select **Build -> Run QMake**.
4. Click **Build** or **Run**.

## License
This project is open-source and licensed under the terms of the **GNU General Public License v3.0 (GPLv3)**. See the `LICENSE` file for details.
