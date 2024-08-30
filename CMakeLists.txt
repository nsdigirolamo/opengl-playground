cmake_minimum_required(VERSION 3.29.5)

project(opengl-solitaire)

set(SOLSOURCES
    src/glad.c
    src/shader.cpp
    src/camera.cpp
    src/model.cpp
    src/solitaire-window.cpp
    src/texture.cpp
)

add_executable(solitaire ${SOLSOURCES})

target_include_directories(solitaire
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(solitaire -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl)
