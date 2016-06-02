gCube
An open source, opengl based, fully 3d, Rubik's implementation in C.
Author: Sean Paget
Email: codeMonkey760@hotmail.com
License: GPLv3 (or later) see COPYING.txt
Version: 1.0
Release Date: Around 5/15/2016

Table of Contents:
I: Intro (supported platforms)
II: Installation
III: Usage and normal operation
IV: Removal
V: Oddities and problems

I: Introduction (and supported platforms)
GCube is an open source, opengl based, fully 3d, Rubik's cube implementation written in the C programming language.
It was developed on Linux Mint and designed to target Ubuntu based linux distrubutions. However, wider support, including windows, is planned.
At the time of this writing gCube is available from https://bitbucket.org/codeMonkey760/gcube. If you did not get it from there you should consider doing so.

II: Installation
	At the time of this writing, gCube is only available in source form. You will need to download the source code and compile it yourself. Linux is the only supported platform, however, it may be possible to compile it for other platforms.

	A: First ensure that you have git, gcc, make and the normal build tools installed. As always, be sure to run apt-get update and apt-get upgrade before beginning. Then
	apt-get install make gcc build-essential libc6
	Please note that you may need to prefix these commands with sudo or run them in a rooted shell
	apt-get update
	apt-get upgrade
	apt-get install make gcc build-essential libc6

	B: gCube, being glfw based, has many library depencies. If your linux distro is uses X11 than you should have the following packages available. Install them all with apt-get install.

	libglfw3-dev
	libglew-dev

	May also be required:
	libxcursor-dev
	libxinerama-dev

	C: After this, navigate to the desired installation directory (ensure that you have write privelleges)
	Once the desired installation directory is your current working directory run:
	git clone https://bitbucket.org/codeMonkey760/gcube.git

	D: Once this process completes navigate to the directory and run make:
	cd gcube
	make

	E: Navigate to the bin directory, and run ./gCube
	cd bin
	./gCube

III: Usage and normal operation:
	
