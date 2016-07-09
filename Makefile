WINDIR = bin/win
LINDIR = bin/lin
WINEXE = $(WINDIR)/gcube.exe
LINEXE = $(LINDIR)/gcube

SRCS = $(wildcard src/Source/*.c)
HEADERS = $(wildcard src/Headers/*.h)
LINOBJS = $(patsubst src/Source/%.c, $(LINDIR)/%.o,$(SRCS))
LINFLAGS = -lm -lGL -lGLEW -lGLU -lglfw3 -lX11 -lXrandr -lXxf86vm -lXi -lXcursor -lXinerama -lpthread -ldl
WINOBJS = $(patsubst src/Source/%.c, $(WINDIR)/%.obj,$(SRCS))
WINFLAGS= /usr/i586-mingw32msvc/lib/glew32.lib /usr/i586-mingw32msvc/lib/glew32.dll /usr/i586-mingw32msvc/lib/glfw3.dll

.PHONY: all
all: $(WINDIR) $(LINDIR) $(LINEXE) $(WINEXE)

$(WINDIR):
	mkdir -p bin/win
	
$(LINDIR):
	mkdir -p bin/lin
	
$(LINEXE): $(LINOBJS) $(LINDIR)/images.o
	gcc -o $@ $^ $(LINFLAGS)
	
$(WINEXE): $(WINOBJS) $(WINDIR)/images.obj $(WINDIR)/gcube.res
	i586-mingw32msvc-gcc $^ -o $@ -mwindows -lmingw32 -lopengl32 $(WINFLAGS)
	cp /usr/i586-mingw32msvc/lib/glew32.dll bin/win/glew32.dll
	cp /usr/i586-mingw32msvc/lib/glfw3.dll bin/win/glfw3.dll
	
$(LINDIR)/images.o: src/Resources/images.bin
	ld -r -b binary -o $@ $^
	
bin/lin/%.o: src/Source/%.c
	gcc -c $< -I src/Headers/ -o $@
	
bin/win/%.obj: src/Source/%.c
	i586-mingw32msvc-gcc -c $< -I src/Headers/ -o $@
	
bin/win/images.obj: src/Resources/images.bin
	i586-mingw32msvc-ld -r -b binary -o $@ $^
	
bin/win/gcube.res: src/Resources/gcube.rc
	i586-mingw32msvc-windres $^ -O coff -o $@
	
.PHONY: clean
clean:
	rm -rf bin
	
.PHONY: runwin
runwin: $(WINDIR) $(WINEXE)
	WINEPREFIX=~/.wine32 wine $(WINEXE)
	
.PHONY: runlin
runlin:
	./$(LINEXE)
	
.PHONY: open
open:
	gedit $(SRCS) $(HEADERS) Makefile &
	
.PHONY: win32
win32: $(WINDIR) $(WINEXE)

.PHONY: linux
linux: $(LINDIR) $(LINEXE)

.PHONY: help
help:
	@echo "gcube: GNU Rubik's Cube"
	@echo "OpenGL based, 3D Rubik's Cube"
	@echo "AUTHOR: Sean Paget"
	@echo "EMAIL:  codeMonkey760@hotmail.com"
	@echo "Usage:  make (target)\n"
	@echo "(target):"
	@echo "    (empty): Make everything and store results in bin folder"
	@echo "    win32:   Make all windows 32bit executables"
	@echo "    linux:   Make all linux executables"
	@echo "    runwin:  Make all windows 32bit executables and execute them via wine"
	@echo "    runlin:  Make all linux executables and execute them via bash"
	@echo "    open:    Open all sources files with gedit"
	@echo "    clean:   Delete the bin folder and everything in it"
	@echo "    help:    Print this message"
