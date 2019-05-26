PROGRAM    = Stado
CC	   = gcc
CFLAGS     = -g -Wall -I/usr/X11R6/include -I/usr/pkg/include
LDLIBS     = -lGL -lGLU -lglut -lm

$(PROGRAM): Stado_develope_version.o image.o callbacks.o draw_objects.o light_and_materials.o sheep.o game_dynamics.o inits.o
	$(CC) $(CFLAGS) -o $(PROGRAM) Stado_develope_version.o image.o callbacks.o draw_objects.o light_and_materials.o sheep.o game_dynamics.o inits.o $(LDLIBS)

Stado_develope_version.o: Stado_develope_version.c colors.h inits.h draw_objects.h light_and_materials.h game_dynamics.h
	$(CC) $(CFLAGS) -c Stado_develope_version.c -o Stado_develope_version.o $(LDLIBS)

image.o: image.c image.h
	$(CC) $(CFLAGS) -c image.c -o image.o $(LDLIBS)

callbacks.o: callbacks.c callbacks.h inits.h 
	$(CC) $(CFLAGS) -c callbacks.c -o callbacks.o $(LDLIBS)

draw_objects.o: draw_objects.c draw_objects.h inits.h light_and_materials.h sheep.h image.h
	$(CC) $(CFLAGS) -c draw_objects.c -o draw_objects.o $(LDLIBS)

light_and_materials.o: light_and_materials.c light_and_materials.h
	$(CC) $(CFLAGS) -c light_and_materials.c -o light_and_materials.o $(LDLIBS)

sheep.o: sheep.c sheep.h
	$(CC) $(CFLAGS) -c sheep.c -o sheep.o $(LDLIBS)

game_dynamics.o: game_dynamics.c game_dynamics.h inits.h
	$(CC) $(CFLAGS) -c game_dynamics.c -o game_dynamics.o $(LDLIBS)

inits.o: inits.c inits.h callbacks.h levels.h
	$(CC) $(CFLAGS) -c inits.c -o inits.o $(LDLIBS)
	
	
.PHONY: clean 

clean: 
	-rm *.o $(PROGRAM) *core
