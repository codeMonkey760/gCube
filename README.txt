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
III: Usage
IV: Removal
V: Oddities and problems

I: Introduction (and supported platforms)
GCube is an open source, opengl based, fully 3d, Rubik's cube implementation written in the C programming language.
It was developed on Linux Mint and designed to target Ubuntu based linux distrubutions. However, wider support, including windows, is planned.
At the time of this writing gCube is available from https://bitbucket.org/codeMonkey760/gcube. If you did not get it from there you should consider doing so.

II: Installation
Dependencies:
Unfortunately, gCube has a long list of dependencies. The following packages should be installed via apt-get install.
make
build-essential

libglfw3

Various methods for installation are planned. 
The first is to compile the project from it's source tar ball. 
1) Download the compressed file for the version you need and decompress it. 
2) Open a terminal, navigate to the root directory of the decompressed tar ball and run make with no arguments.
3) You should have a fully compiled gcube binary sitting in the bin directory.
