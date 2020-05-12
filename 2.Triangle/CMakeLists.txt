cmake_minimum_required(VERSION 2.8)
project(triangle)

#agregar includes y librerias
include_directories(../0.openglStuff/include)
link_directories("${CMAKE_CURRENT_SOURCE_DIR}/../0.openglStuff/lib")

#crear el ejecutable
add_executable(helloWindow source.cpp ../0.openglStuff/src/glad.c)

#crear dependencias
target_link_libraries(helloWindow glfw3.lib)

#cmake -DCMAKE_GENERATOR_PLATFORM=x64