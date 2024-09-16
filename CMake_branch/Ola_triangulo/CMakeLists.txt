cmake_minimum_required(VERSION 3.28.0)
project(Atividades_processamento_grafico VERSION 1.0.0)
cmake_policy(SET CMP0072 NEW)

find_package(OpenGL REQUIRED)

add_executable(Atividades_processamento_grafico 
    src/main.cpp
    src/glad.h
    src/glad.c
)

target_link_libraries(Atividades_processamento_grafico 
    glfw
    OpenGL::GL
)