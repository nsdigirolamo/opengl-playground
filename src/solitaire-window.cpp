#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <shader.hpp>

#include <iostream>

constexpr unsigned int WINDOW_WIDTH = 768;
constexpr unsigned int WINDOW_HEIGHT = 432;

void errorCallback (int error, const char* description)
{
    std::cerr << "GLFW Error: " << description << std::endl;
    glfwTerminate();
    exit(-1);
}

void framebufferSizeCallback (GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void keyCallback (GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

int main ()
{
    glfwSetErrorCallback(errorCallback);
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "opengl-solitaire", NULL, NULL);
    if (window == NULL)
    {
        std::cerr << "GLFW Error: Failed to create a window." << std::endl;
        glfwTerminate();
        exit(-1);
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetKeyCallback(window, keyCallback);

    if (!gladLoadGLLoader((GLADloadproc)(glfwGetProcAddress)))
    {
        std::cerr << "GLFW Error: Failed to initialize GLAD.";
        glfwTerminate();
        exit(-1);
    }

    Shader shader { "shaders/tri.vs", "shaders/tri.fs" };

    const float vertexData [] =
    {   // Positions        // Colors
        0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f,
       -0.5f,  0.5f, 0.0f,  0.0f, 1.0f, 0.0f,
        0.0f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f
    };

    unsigned int vertexArray, vertexBuffer;

    glGenVertexArrays(1, &vertexArray);
    glGenBuffers(1, &vertexBuffer);

    glBindVertexArray(vertexArray);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(0));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.use();
        glBindVertexArray(vertexArray);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    exit(0);
}