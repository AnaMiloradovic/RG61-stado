PROGRAM    = Stado
CC	   = gcc
CFLAGS     = -g -Wall -I/usr/X11R6/include -I/usr/pkg/include
LDLIBS     = -lGL -lGLU -lglut -lm

$(PROGRAM): Stado_develope_version.o image.o callbacks.o draw_objects.o light_and_materials.o \
            sheep.o game_dynamics.o inits.o
	$(CC) $(CFLAGS) -o $(PROGRAM) Stado_develope_version.o image.o callbacks.o draw_objects.o light_and_materials.o sheep.o game_dynamics.o inits.o $(LDLIBS)

Stado_develope_version.o: src/Stado_develope_version.c src/sheep.h src/colors.h src/inits.h src/callbacks.h \
                          src/draw_objects.h src/light_and_materials.h src/levels.h src/game_dynamics.h
	$(CC) $(CFLAGS) -c src/lonelyroom.c -o lonelyroom.o $(LDLIBS)

image.o: sources/image.c
	$(CC) $(CFLAGS) -c sources/image.c $(LDLIBS)

ortho.o: sources/ortho.c sources/ortho.h
	$(CC) $(CFLAGS) -c sources/ortho.c -o ortho.o $(LDLIBS)

axes.o: sources/axes.c sources/axes.h
	$(CC) $(CFLAGS) -c sources/axes.c -o axes.o $(LDLIBS)

light.o: sources/light.c sources/light.h
	$(CC) $(CFLAGS) -c sources/light.c -o light.o $(LDLIBS)
	
	
.PHONY: clean 

clean: 
-rm *.o $(PROGRAM) *core
