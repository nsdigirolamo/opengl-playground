#include "glad/glad.h"
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include "camera.hpp"
#include "shader.hpp"
#include "model.hpp"
#include "light.hpp"

#include <GLFW/glfw3.h>
#include <iostream>

constexpr unsigned int WINDOW_WIDTH = 960;
constexpr unsigned int WINDOW_HEIGHT = 540;

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
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetKeyCallback(window, keyCallback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)(glfwGetProcAddress)))
    {
        std::cerr << "GLFW Error: Failed to initialize GLAD.";
        glfwTerminate();
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    Shader shader { "shaders/shader.vs", "shaders/shader.fs" };

    Model model { "models/teapot_bezier0.tris", glm::vec3(1.0, 0.5, 0.31) };
    Light light { };

    Camera camera;

    double previousTime = glfwGetTime();

    while (!glfwWindowShouldClose(window))
    {
        double currentTime = glfwGetTime();
        double deltaTime = currentTime - previousTime;
        previousTime = currentTime;
        camera.processKeyInput(window, deltaTime);

        double xCursorPos = 0;
        double yCursorPos = 0;
        glfwGetCursorPos(window, &xCursorPos, &yCursorPos);
        camera.processMouseInput(glm::vec2(xCursorPos, yCursorPos));

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();

        glm::mat4 projectionMat = glm::perspective(
            glm::radians(camera.fov),
            (float)(WINDOW_WIDTH) / (float)(WINDOW_HEIGHT),
            0.1f,
            100.0f
        );

        glm::mat4 viewMat = camera.getLookAt();

        glm::mat4 modelMat = glm::mat4(1.0f);

        shader.setMat4("projection", projectionMat);
        shader.setMat4("view", viewMat);
        shader.setMat4("model", modelMat);

        model.bindVertexBuffer();
        model.bindVertexArray();
        model.drawVertexArray();

        light.bindVertexBuffer();
        light.bindVertexArray();
        light.drawVertexArray();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
