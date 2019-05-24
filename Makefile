PROGRAM    = Stado
CC	   = gcc
CFLAGS     = -g -Wall -I/usr/X11R6/include -I/usr/pkg/include
LDLIBS     = -lGL -lGLU -lglut -lm

$(PROGRAM): Stado_develope_version.o image.o callbacks.o draw_objects.o light_and_materials.o sheep.o game_dynamics.o inits.o
	$(CC) $(CFLAGS) -o $(PROGRAM) Stado_develope_version.o image.o callbacks.o draw_objects.o light_and_materials.o sheep.o game_dynamics.o inits.o $(LDLIBS)

Stado_develope_version.o: src/Stado_develope_version.c src/sheep.h src/colors.h src/inits.h src/callbacks.h src/draw_objects.h src/light_and_materials.h src/levels.h src/game_dynamics.h 
	$(CC) $(CFLAGS) -c src/Stado_develope_version.c -o Stado_develope_version.o $(LDLIBS)


game_dynamics.o: src/game_dynamics.c src/game_dynamics.h src/draw_objects.h src/colors.h src/inits.h src/callbacks.h src/light_and_materials.h src/image.h
	$(CC) $(CFLAGS) -c src/game_dynamics.c -o src/game_dynamics.o $(LDLIBS)
	
	
callbacks.o: src/callbacks.c src/callbacks.h src/colors.h src/inits.h src/game_dynamics.h
	$(CC) $(CFLAGS) -c src/callbacks.c -o callbacks.o $(LDLIBS)
	
	
inits.o: src/inits.c src/inits.h  src/levels.h src/draw_objects.h src/colors.h  src/callbacks.h
	$(CC) $(CFLAGS) -c src/inits.c -o src/inits.o $(LDLIBS)
	
draw_objects.o: src/draw_objects.c src/draw_objects.h src/colors.h src/inits.h src/callbacks.h src/light_and_materials.h src/sheep.h src/image.h
	$(CC) $(CFLAGS) -c src/draw_objects.c -o src/draw_objects.o $(LDLIBS)
	
light_and_materials.o: src/light_and_materials.c src/light_and_materials.h src/colors.h src/inits.h src/callbacks.h src/draw_objects.h
	$(CC) $(CFLAGS) -c src/draw_objects.c -o src/draw_objects.o $(LDLIBS)
	
sheep.o: src/sheep.c src/sheep.h
	$(CC) $(CFLAGS) -c src/sheep.c -o sheep.o $(LDLIBS)
	
image.o: src/image.c
	$(CC) $(CFLAGS) -c src/image.c $(LDLIBS)
	
.PHONY: clean 

clean: 
	-rm *.o $(PROGRAM) *core
