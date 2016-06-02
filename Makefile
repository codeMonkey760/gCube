SRCS = $(wildcard src/Source/*.c)
OBJS = $(patsubst src/Source/%.c, bin/%.o,$(SRCS))
LIBS = -lm -lGL -lGLEW -lGLU -lglfw -lX11 -lXrandr -lXxf86vm -lXi -lXcursor -lXinerama -lpthread -ldl

.PHONY: bin

all: bin bin/gCube

bin/gCube: $(OBJS) bin/images.o 
	gcc -o $@ $^ $(LIBS)
	
bin:
	mkdir -p bin
	
bin/images.o: src/Resources/images.bin
	ld -r -b binary -o $@ $^
	
bin/%.o: src/Source/%.c
	gcc -c $< -I src/Headers/ -o $@
	
clean: 
	rm -rf bin
