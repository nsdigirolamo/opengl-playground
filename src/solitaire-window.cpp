#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

constexpr unsigned int WINDOW_WIDTH = 768;
constexpr unsigned int WINDOW_HEIGHT = 432;

void errorCallback (int error, const char* description)
{
    std::cerr << "GLFW Error: " << description << std::endl;
    glfwTerminate();
    exit(-1);
}

static void keyCallback (GLFWwindow* window, int key, int scancode, int action, int mods)
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

    if (!gladLoadGLLoader((GLADloadproc)(glfwGetProcAddress)))
    {
        std::cerr << "GLFW Error: Failed to initialize GLAD.";
        glfwTerminate();
        exit(-1);
    }

    glfwSetKeyCallback(window, keyCallback);

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
    }

    glfwTerminate();
    exit(0);
}