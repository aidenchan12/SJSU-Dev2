# Getting Started with the SJTwo

## Supported Operating Systems

Need a running version of Ubuntu 16.04 LTS or above, or Mac OS X (OS X
Yosemite and above), or Windows 10 with WSL installed.

## Installing WSL for Windows 10 Users

Windows 10 users must install WSL in order to work with the development
environment.
Follow this link and follow the steps:
[Install WSL on Windows 10](https://docs.microsoft.com/en-us/windows/wsl/install-win10).

## Installing Serial Device Driver for Windows 10 and Mac Users

Windows 10 and Mac users will need to install the serial drivers on
their computer to be able to communicate with the SJTwo board. Follow
this link and download the driver for your computer.
[Link to Serial Driver Download](https://www.silabs.com/products/development-tools/software/usb-to-uart-bridge-vcp-drivers)

## Installation

### Step 0: Find a download spot
Open a terminal (also called a command line) and go to a directory
where you would like to download SJSU-Dev2.

!!! Note
    On Windows it is advised to install in the `/mnt/c/` directory as
    you will have access to the files from your desktop. To go to this
    folder type `cd /mnt/c` into your terminal and press enter.

### Step 1: Download the platform
Go to <https://github.com/kammce/SJSU-Dev2> and click on the "Clone
or download" drop down button.

Put the zip file in the directory you chose in step 0 and unzip it
there.

### Step 2: Move into SJSU-Dev2

    cd SJSU-Dev2

### Step 3: Run `setup` script.

    ./setup

This will install all of the necessary files and programs that
SJSU-Dev2 needs in order to build your code. The downloaded files
are placed in the `tools/` folder.

## Building and Loading the "Hello World" Application

### Step 0: Move into project

    cd projects/hello_world/

### Step 1: Build project

    make application

This will take all of the relevant source code files to your project
and generate a binary file that can be loaded onto your board. These
files can be found in the `build/application` folder with the
project.

!!! Tip
    use the `make` by itself to get additional information on the
    different features of the build script.

!!! Tip
    To build for a different board you can specify the platform like so:
    `make application PLATFORM=lpc17xx`. This example will build the project for
    the SJOne board.

### Step 2: Flash board
To load the built binary file into your board, run:

    make flash

!!! Tip
    If the code wasn't built before, this command will build your latest project
    then flash your board. So you want to build then immediately flash, you can
    skip the step above.

!!! Note
    If `make flash` could not find your device, try running it again. If
    it continues to fail then it could a few things such as:

    1. The serial driver hasn't yet been installed OR
    2. The board is connected to some other program like telemetry,
    Hercules, putty or minicom

### Step 3: Interacting with device
To view output messages from board and interact with it, go to the following
URL and following the setup instructions: <https://kammce.github.io/Telemetry>

Once there, use the drop down menu on the upper right hand side of the web
page to find and select the serial device that corresponds to your board
press the **Connect** button.

## Creating Your Own Project

The easiest way to get started is to simply copy one of the existing projects
and work from there.

You can find projects in the `demos/` directory as well as the `projects/`
directory.

A project by the name of `starter` is created exactly for this purpose. To make
a new project, copy and rename the **starter**. All projects must contain a
Makefile and source directory to operate properly.
