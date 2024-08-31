#include "glad/glad.h"
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include "camera.hpp"
#include "shader.hpp"
#include "object.hpp"

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

    Shader nonLightShader { "shaders/nonlight.vert.glsl", "shaders/nonlight.frag.glsl" };
    Shader lightShader { "shaders/light.vert.glsl", "shaders/light.frag.glsl" };

    Model cubeModel { "models/cube.obj" };
    Texture diffuseMap { "textures/box_texture_diffuse_map.png" };
    Texture specularMap { "textures/box_texture_specular_map.png" };
    Texture emissiveMap { "textures/box_texture_empty.png" };
    Material cubeMaterial { diffuseMap.getID(), specularMap.getID(), emissiveMap.getID(), 64.0f };
    Object cube { glm::vec3(0.0f), glm::vec3(1.0f), &cubeModel, &cubeMaterial };

    Model lightModel { "models/cube.obj" };
    Light light {
        glm::vec3(0.0f),
        glm::vec3(0.2f),
        &lightModel,
        glm::vec3(0.2f, 0.2f, 0.2f),
        glm::vec3(0.5f, 0.5f, 0.5f),
        glm::vec3(1.0f, 1.0f, 1.0f),
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
        translateMat = glm::translate(translateMat, cube.position);

        glm::mat4 scaleMat = glm::mat4(1.0f);
        scaleMat = glm::scale(scaleMat, cube.scale);

        nonLightShader.use();

        nonLightShader.setVec3("lightPosition", light.position);
        nonLightShader.setVec3("viewPosition", camera.position);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, cube.material->diffuse);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, cube.material->specular);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, cube.material->emissive);

        nonLightShader.setInt("objectMaterial.diffuse", 0);
        nonLightShader.setInt("objectMaterial.specular", 1);
        nonLightShader.setInt("objectMaterial.emissive", 2);
        nonLightShader.setFloat("objectMaterial.shine", cube.material->shine);

        nonLightShader.setVec3("light.ambient", light.ambient);
        nonLightShader.setVec3("light.diffuse", light.diffuse);
        nonLightShader.setVec3("light.specular", light.specular);

        nonLightShader.setMat4("projection", projectionMat);
        nonLightShader.setMat4("view", viewMat);
        nonLightShader.setMat4("model", modelMat);
        nonLightShader.setMat4("translate", translateMat);
        nonLightShader.setMat4("scale", scaleMat);

        cube.model->bindVertexBuffer();
        cube.model->bindVertexArray();
        cube.model->drawVertexArray();

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
