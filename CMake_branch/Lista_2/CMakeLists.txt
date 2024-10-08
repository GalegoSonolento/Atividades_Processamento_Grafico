cmake_minimum_required(VERSION 3.22.0)
project(Atividades_processamento_grafico VERSION 1.0.0)
cmake_policy(SET CMP0072 NEW)

find_package(OpenGL REQUIRED)

add_executable(Atividades_processamento_grafico
    #src/lista_2_ex_1.cpp
    #src/lista_2_ex_2.cpp
    #src/lista_2_ex_3.cpp
    #src/lista_2_ex_4.cpp
    src/lista_2_ex_5.cpp
    src/glad.h
    src/glad.c
)

target_link_libraries(Atividades_processamento_grafico
    glfw
    OpenGL::GL
)