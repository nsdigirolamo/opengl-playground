#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "glm/glm.hpp"

#include <GLFW/glfw3.h>

#define WORLD_UP glm::vec3(0, 1, 0)
#define DEFAULT_SPEED 5.0f
#define DEFAULT_SENSITIVITY 0.1f
#define DEFAULT_YAW -90.0f
#define DEFAULT_PITCH 0.0f
#define DEFAULT_FOV 90.0f

class Camera
{
    private:

        void updateLookVectors ();

    public:

        glm::vec3 position;
        glm::vec3 forward;
        glm::vec3 up;
        glm::vec3 right;

        float yaw;
        float pitch;

        float moveSpeed;
        float mouseSensitivity;
        float fov;

        Camera ();
        Camera (glm::vec3 position, glm::vec3 up, float yaw, float pitch);

        glm::mat4 getLookAt ();

        void processKeyInput (GLFWwindow* window, float deltaTime);
        void processMouseInput (glm::vec2 offset);
};

#endif
