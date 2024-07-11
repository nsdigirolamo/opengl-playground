# OpenGl Solitaire

This is an OpenGL project I'm working on to teach myself the API. Eventually
it will be a 3D solitaire game, but for now it's a playground I'm using to teach
myself OpenGL with [this tutorial by Joey de Vries.](https://learnopengl.com/)

This repository contains the following libraries:

- glad.h [(license)](https://github.com/Dav1dde/glad/blob/glad2/LICENSE)
- glm [(license)](https://github.com/g-truc/glm/blob/master/manual.md#section0)
- khrplatform.h

The [models](https://github.com/nsdigirolamo/opengl-solitaire/tree/main/models)
directory contains a 3D model of the Utah Teapot sourced from
[this webpage.](https://users.cs.utah.edu/~dejohnso/models/teapot.html)

To build this project, make sure you have [cmake](https://cmake.org/) and
[GLFW](https://www.glfw.org/) installed on your system. Clone the repository 
and run the following commands while within the newly cloned directory:

```bash
cmake .
cmake --build .
```

And the program can be ran with:

```bash
./solitaire
```
