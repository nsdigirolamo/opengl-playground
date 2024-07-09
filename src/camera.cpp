#include "camera.hpp"
#include "glm/gtc/matrix_transform.hpp"

void Camera::updateLookVectors ()
{
    glm::vec3 newForward;
    newForward.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
    newForward.y = sin(glm::radians(this->pitch));
    newForward.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
    this->forward = glm::normalize(newForward);
    this->right = glm::normalize(glm::cross(this->forward, WORLD_UP));
    this->up = glm::normalize(glm::cross(this->right, this->forward));
}

Camera::Camera ()
    : Camera(glm::vec3(0, 0, 0), WORLD_UP, DEFAULT_YAW, DEFAULT_PITCH)
{ }

Camera::Camera (glm::vec3 position, glm::vec3 up, float yaw, float pitch)
    : position(position)
    , up(up)
    , yaw(yaw)
    , pitch(pitch)
    , moveSpeed(DEFAULT_SPEED)
    , mouseSensitivity(DEFAULT_SENSITIVITY)
    , fov(DEFAULT_FOV)
{
    updateLookVectors();
}

glm::mat4 Camera::getLookAt ()
{
    return glm::lookAt(this->position, this->position + this->forward, this->up);
}

void Camera::processKeyInput (GLFWwindow* window, float deltaTime)
{
    float speed = this->moveSpeed * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        this->position += speed * this->forward;

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        this->position -= speed * this->forward;

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        this->position -= glm::normalize(glm::cross(this->forward, this->up)) * speed;

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        this->position += glm::normalize(glm::cross(this->forward, this->up)) * speed;
}

void Camera::processMouseInput (glm::vec2 offset)
{
    offset *= this->mouseSensitivity;
    this->yaw = offset.x;
    this->pitch = offset.y;

    if (89.0 < this->pitch)
    {
        this->pitch = 89.0;
    }
    else if (this->pitch < -89.0)
    {
        this->pitch = -89.0;
    }

    updateLookVectors();
}
