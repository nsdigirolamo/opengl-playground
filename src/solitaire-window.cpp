#include "glad/glad.h"
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include "camera.hpp"
#include "shader.hpp"

#include <GLFW/glfw3.h>
#include <iostream>
#include <model.hpp>

constexpr unsigned int WINDOW_WIDTH = 1600;
constexpr unsigned int WINDOW_HEIGHT = 800;

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

    Shader nonLightShader { "shaders/nonlight.vert.glsl", "shaders/nonlight.frag.glsl" };
    Shader lightShader { "shaders/light.vert.glsl", "shaders/light.frag.glsl" };

    Model object {
        "models/cube.obj",
        glm::vec3(0.0f),
        {
            glm::vec3(1.0f, 0.5f, 0.31f),
            glm::vec3(1.0f, 0.5f, 0.31f),
            glm::vec3(0.5f, 0.5f, 0.5f),
            32.0f
        },
        glm::vec3(1.0f)
    };

    Model light {
        "models/cube.obj",
        glm::vec3(0.0f),
        {
            glm::vec3(0.2f, 0.2f, 0.2f),
            glm::vec3(0.5f, 0.5f, 0.5f),
            glm::vec3(1.0f, 1.0f, 1.0f),
            32.0f
        },
        glm::vec3(0.2f)
    };

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

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 projectionMat = glm::perspective(
            glm::radians(camera.fov),
            (float)(WINDOW_WIDTH) / (float)(WINDOW_HEIGHT),
            0.1f,
            100.0f
        );

        glm::mat4 viewMat = camera.getLookAt();

        glm::mat4 modelMat = glm::mat4(1.0f);

        glm::mat4 translateMat = glm::mat4(1.0f);
        translateMat = glm::translate(translateMat, object.position);

        glm::mat4 scaleMat = glm::mat4(1.0f);
        scaleMat = glm::scale(scaleMat, object.scale);

        nonLightShader.use();

        nonLightShader.setVec3("lightPosition", light.position);
        nonLightShader.setVec3("viewPosition", camera.position);

        nonLightShader.setVec3("objectMaterial.ambient", object.material.ambient);
        nonLightShader.setVec3("objectMaterial.diffuse", object.material.diffuse);
        nonLightShader.setVec3("objectMaterial.specular", object.material.specular);
        nonLightShader.setFloat("objectMaterial.shine", object.material.shine);

        nonLightShader.setVec3("lightMaterial.ambient", light.material.ambient);
        nonLightShader.setVec3("lightMaterial.diffuse", light.material.diffuse);
        nonLightShader.setVec3("lightMaterial.specular", light.material.specular);
        nonLightShader.setFloat("lightMaterial.shine", light.material.shine);

        nonLightShader.setMat4("projection", projectionMat);
        nonLightShader.setMat4("view", viewMat);
        nonLightShader.setMat4("model", modelMat);
        nonLightShader.setMat4("translate", translateMat);
        nonLightShader.setMat4("scale", scaleMat);

        object.bindVertexBuffer();
        object.bindVertexArray();
        object.drawVertexArray();

        lightShader.use();

        light.position.x = 5 * sin(currentTime);
        light.position.z = 5 * cos(currentTime);

        translateMat = glm::mat4(1.0f);
        translateMat = glm::translate(translateMat, light.position);

        scaleMat = glm::mat4(1.0f);
        scaleMat = glm::scale(scaleMat, light.scale);

        lightShader.setMat4("projection", projectionMat);
        lightShader.setMat4("view", viewMat);
        lightShader.setMat4("model", modelMat);
        lightShader.setMat4("translate", translateMat);
        lightShader.setMat4("scale", scaleMat);

        lightShader.setVec3("lightMaterial.ambient", light.material.ambient);
        lightShader.setVec3("lightMaterial.diffuse", light.material.diffuse);
        lightShader.setVec3("lightMaterial.specular", light.material.specular);
        lightShader.setFloat("lightMaterial.shine", light.material.shine);

        light.bindVertexBuffer();
        light.bindVertexArray();
        light.drawVertexArray();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
