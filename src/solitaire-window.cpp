#include "glad/glad.h"
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include "camera.hpp"
#include "shader.hpp"

#include <GLFW/glfw3.h>
#include <iostream>
#include <model.hpp>
#include <texture.hpp>
#include <light.hpp>

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

    Shader nonLightShader { "shaders/texture_tests/nonlight.vert.glsl", "shaders/texture_tests/nonlight.frag.glsl" };
    Shader lightShader { "shaders/texture_tests/light.vert.glsl", "shaders/texture_tests/light.frag.glsl" };

    Texture texture {
        "textures/box_texture.png"
    };

    Material material {
        texture.getID(),
        glm::vec3(0.5f, 0.5f, 0.5f),
        32.0f
    };

    Light light {
        glm::vec3(0.2f, 0.2f, 0.2f),
        glm::vec3(0.5f, 0.5f, 0.5f),
        glm::vec3(1.0f, 1.0f, 1.0f)
    };

    Model objectModel {
        "models/cube.obj",
        glm::vec3(0.0f),
        glm::vec3(1.0f)
    };

    Model lightModel {
        "models/cube.obj",
        glm::vec3(0.0f),
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
        translateMat = glm::translate(translateMat, objectModel.position);

        glm::mat4 scaleMat = glm::mat4(1.0f);
        scaleMat = glm::scale(scaleMat, objectModel.scale);

        nonLightShader.use();

        nonLightShader.setVec3("lightPosition", lightModel.position);
        nonLightShader.setVec3("viewPosition", camera.position);

        nonLightShader.setInt("objectMaterial.diffuse", 0);
        nonLightShader.setVec3("objectMaterial.specular", material.specular);
        nonLightShader.setFloat("objectMaterial.shine", material.shine);

        nonLightShader.setVec3("light.ambient", light.ambient);
        nonLightShader.setVec3("light.diffuse", light.diffuse);
        nonLightShader.setVec3("light.specular", light.specular);

        nonLightShader.setMat4("projection", projectionMat);
        nonLightShader.setMat4("view", viewMat);
        nonLightShader.setMat4("model", modelMat);
        nonLightShader.setMat4("translate", translateMat);
        nonLightShader.setMat4("scale", scaleMat);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, material.diffuse);

        objectModel.bindVertexBuffer();
        objectModel.bindVertexArray();
        objectModel.drawVertexArray();

        lightShader.use();

        lightModel.position.x = 5 * sin(currentTime);
        lightModel.position.z = 5 * cos(currentTime);

        translateMat = glm::mat4(1.0f);
        translateMat = glm::translate(translateMat, lightModel.position);

        scaleMat = glm::mat4(1.0f);
        scaleMat = glm::scale(scaleMat, lightModel.scale);

        lightShader.setMat4("projection", projectionMat);
        lightShader.setMat4("view", viewMat);
        lightShader.setMat4("model", modelMat);
        lightShader.setMat4("translate", translateMat);
        lightShader.setMat4("scale", scaleMat);

        lightShader.setVec3("light.ambient", light.ambient);
        lightShader.setVec3("light.diffuse", light.diffuse);
        lightShader.setVec3("light.specular", light.specular);

        lightModel.bindVertexBuffer();
        lightModel.bindVertexArray();
        lightModel.drawVertexArray();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
