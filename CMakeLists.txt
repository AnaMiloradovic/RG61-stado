cmake_minimum_required(VERSION 3.13)
project(RG61_stado C)

set(CMAKE_C_STANDARD 11)

link_libraries(GL)
link_libraries(GLU)
link_libraries(glut)
link_libraries(m)

include_directories(src)

add_executable(RG61_stado
        src/callbacks.c
        src/callbacks.h
        src/colors.h
        src/draw_objects.c
        src/draw_objects.h
        src/game_dynamics.c
        src/game_dynamics.h
        src/inits.c
        src/inits.h
        src/levels.h
        src/light_and_materials.c
        src/light_and_materials.h
        src/Stado_develope_version.c)
