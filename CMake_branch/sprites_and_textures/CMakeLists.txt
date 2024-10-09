cmake_minimum_required(VERSION 3.22.0)
project(Atividades_processamento_grafico VERSION 1.0.0)
cmake_policy(SET CMP0072 NEW)

find_package(OpenGL REQUIRED)

add_executable(Atividades_processamento_grafico
    src/stb_image/stb_image.cpp
    src/glad.h
    src/glad.c
    #src/textures.cpp
    src/Sprites.cpp
)

target_link_libraries(Atividades_processamento_grafico
    glfw
    OpenGL::GL
)