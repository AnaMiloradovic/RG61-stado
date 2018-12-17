cmake_minimum_required(VERSION 3.13)
project(RG61_stado C)

set(CMAKE_C_STANDARD 11)

link_libraries(GL)
link_libraries(GLU)
link_libraries(glut)

include_directories(.)

add_executable(RG61_stado
        colors.h
        inits.h
        Stado_BeginnerVersion_Main.c)
