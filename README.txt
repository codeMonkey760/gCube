gCube
An open source, opengl based, fully 3d, Rubik's cube implementation in C.
Author: Sean Paget
Email: codeMonkey760@hotmail.com
License: GPLv3 (or later) see COPYING.txt
Version: 0.1
Release Date: Around 7/6/16

Table of Contents:
I: Intro (supported platforms)
II: Installation
III: Usage and normal operation
IV: Removal
V: Oddities and problems

I: Introduction (and supported platforms)
	gCube is an open source, opengl based, fully 3d, Rubik's cube implementation written in the C programming language.
At the time of this writing gCube is available from https://github.com/paget001/gCube. If you did not get it from there you should consider doing so.

II: Installation
	Several installation methods exists. The recommended approach is to simply download a zipped package labeled for your operating system. Currently there are two available. A 32bit binary for windows and a 64bit binary for linux. No OSX support is planned. Once the suitable package has been identified simply download and unpackage it. gCube has no installer and can be run directly from where ever it is placed provided sufficent permission. Because of permission issues, it may be best to run it out of your home directory.
	Being an open source project, you may also choose to compile it from source. My makefiles are an absolute mess so this is not the recommended approach as heavy modification will be required in order to match your system's build configuration.
	On linux, several packages may be required.
	libGL.so (usually provided by libgl1-mesa-dev)
	libglew-dev
	libglfw3-dev
You can obtain these packages by running apt-get install on debian systems or yum install on red hat systems, or whatever your package manager is.

III: Usage and normal operation:
	At program start up you will get a solved cube centered directly on the screen. You can orbit the 3d camera by holding down the right mouse button and moving the mouse. There are also several buttons surrounding the view. These buttons move the "slices" of the rubik's cube relative to how you are viewing it. When the cube is in a solved state, a button will appear with a question mark. Click it to recieve a random number of slice rotations and scramble the cube. You may also invert the cube with a button at the top of the screen.
	gCube may also attempt to create several files. It may create a directory in your home path called '.gCube'. Here is where it will store your save and config file. Deleting either will reset either the cube's state or any custom settings you created.
	
IV: Removal:
	gCube has no installer because it doesn't need to modify your system. Thus, it has no uninstaller. If you don't want it, just delete it. However, you should also track down the .gCube directory in your home path and delete that too just for neatness.
	
V: Oddities and problems:
	Settings in the configuration file are never checked. It is very possible to create a config file that makes the game unplayable. If this should happen, you can delete the config file and the game will recreate a new one with the defaults. You should follow the format in the config file as closely as possible by placing a space between the tags = and value.
	If you look closely at the build process for this project, you may notice that gCube staticly links a binary blob called images.bin directly into the executable. Since this is not normal build behavior I should explain. Images.bin is a closely packed raw representation of several textures used in the game. I didn't want to deal with image loading libraries or handle the possibility of missing textures so I lumped them all into the executable. To see how the source code interacts with the blob, simply refer to CubeletModel.c where texture initialization takes place.
