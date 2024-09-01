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

    Shader lightingShader { "shaders/nonlight.vert.glsl", "shaders/nonlight.frag.glsl" };
    Shader sourceShader { "shaders/light.vert.glsl", "shaders/light.frag.glsl" };

    Model cubeModel { "models/cube.obj" };
    Texture diffuseMap { "textures/box_texture_diffuse_map.png" };
    Texture specularMap { "textures/box_texture_specular_map.png" };
    Texture emissiveMap { "textures/box_texture_empty.png" };
    Material cubeMaterial { diffuseMap.getID(), specularMap.getID(), emissiveMap.getID(), 64.0f };

    glm::vec3 cubePositions [10] = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    glm::vec3 pointLightPositions[] = {
        glm::vec3( 0.7f,  0.2f,  2.0f),
        glm::vec3( 2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f,  2.0f, -12.0f),
        glm::vec3( 0.0f,  0.0f, -3.0f)
    };

    Object cubes [10];

    for (int i = 0; i < 10; ++i) {
        cubes[i] = { cubePositions[i], glm::vec3(0.5f), &cubeModel, &cubeMaterial };
    }

    SunLight sunLight {
        glm::vec3(-0.2f, -1.0f, -0.3f),
        glm::vec3(0.05f, 0.05f, 0.05f),
        glm::vec3(0.4f, 0.4f, 0.4f),
        glm::vec3(0.5f, 0.5f, 0.5f),
    };

    PointLight pointLights [4];

    for (int i = 0; i < 4; ++i) {
        pointLights[i] = {
            pointLightPositions[i],
            1.0f,
            0.09f,
            0.032f,
            glm::vec3(0.05f),
            glm::vec3(0.8f),
            glm::vec3(1.0f),
        };
    }

    Camera camera;

    SpotLight spotLight {
        camera.position,
        camera.forward,
        glm::cos(glm::radians(12.5f)),
        glm::cos(glm::radians(15.0f)),
        1.0f,
        0.09f,
        0.032f,
        glm::vec3(0.0f),
        glm::vec3(1.0f),
        glm::vec3(1.0f),
    };

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

        lightingShader.use();

        lightingShader.setVec3("viewPosition", camera.position);

        lightingShader.setFloat("material.shine", cubeMaterial.shine);
        lightingShader.setInt("material.diffuse", 0);
        lightingShader.setInt("material.specular", 1);
        lightingShader.setInt("material.emissive", 2);

        lightingShader.setVec3("sunLight.direction", sunLight.direction);
        lightingShader.setVec3("sunLight.ambient", sunLight.ambient);
        lightingShader.setVec3("sunLight.diffuse", sunLight.diffuse);
        lightingShader.setVec3("sunLight.specular", sunLight.specular);

        for (int i = 0; i < 4; ++i) {
            std::string name = "pointLights[" + std::to_string(i) + "].";
            lightingShader.setVec3(name + "position", pointLights[i].position);
            lightingShader.setFloat(name + "constant", pointLights[i].constant);
            lightingShader.setFloat(name + "linear", pointLights[i].linear);
            lightingShader.setFloat(name + "quadratic", pointLights[i].quadratic);
            lightingShader.setVec3(name + "ambient", pointLights[i].ambient);
            lightingShader.setVec3(name + "diffuse", pointLights[i].diffuse);
            lightingShader.setVec3(name + "specular", pointLights[i].specular);
        }

        lightingShader.setVec3("spotLight.position", camera.position);
        lightingShader.setVec3("spotLight.direction", camera.forward);
        lightingShader.setFloat("spotLight.cutOff", spotLight.cutOff);
        lightingShader.setFloat("spotLight.outerCutOff", spotLight.outerCutOff);
        lightingShader.setFloat("spotLight.constant", spotLight.constant);
        lightingShader.setFloat("spotLight.linear", spotLight.linear);
        lightingShader.setFloat("spotLight.quadratic", spotLight.quadratic);
        lightingShader.setVec3("spotLight.ambient", spotLight.ambient);
        lightingShader.setVec3("spotLight.diffuse", spotLight.diffuse);
        lightingShader.setVec3("spotLight.specular", spotLight.specular);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, cubeMaterial.diffuse);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, cubeMaterial.specular);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, cubeMaterial.emissive);

        glm::mat4 viewMat = camera.getLookAt();
        glm::mat4 projectionMat = glm::perspective(
            glm::radians(camera.fov),
            (float)(WINDOW_WIDTH) / (float)(WINDOW_HEIGHT),
            0.1f,
            100.0f
        );

        lightingShader.setMat4("view", viewMat);
        lightingShader.setMat4("projection", projectionMat);

        cubeModel.bindVertexBuffer();
        cubeModel.bindVertexArray();

        for (int i = 0; i < 10; ++i) {

            glm::mat4 modelMat = glm::mat4(1.0f);
            modelMat = glm::translate(modelMat, cubes[i].position);
            modelMat = glm::rotate(modelMat, glm::radians(20.0f * i), glm::vec3(1.0f, 0.3f, 0.3f));
            modelMat = glm::scale(modelMat, cubes[i].scale);

            lightingShader.setMat4("model", modelMat);

            cubeModel.drawVertexArray();
        }

        sourceShader.use();
        sourceShader.setMat4("view", viewMat);
        sourceShader.setMat4("projection", projectionMat);

        for (int i = 0; i < 4; ++i)
        {
            glm::mat4 modelMat = glm::mat4(1.0f);
            modelMat = glm::translate(modelMat, pointLightPositions[i]);
            modelMat = glm::scale(modelMat, glm::vec3(0.2f));

            sourceShader.setMat4("model", modelMat);
            sourceShader.setVec3("color", pointLights[i].specular);
            cubeModel.drawVertexArray();
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
