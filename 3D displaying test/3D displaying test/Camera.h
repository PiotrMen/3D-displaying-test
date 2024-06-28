#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 30.0f; // Changed from 45.0f to 30.0f

class Camera
{
public:
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    float Yaw;
    float Pitch;
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;
    float distanceFromTarget;  // Make distanceFromTarget public

    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

    glm::mat4 GetViewMatrix();
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);
    void ProcessMouseScroll(float yoffset);

    // New methods for handling camera control
    void HandleMouseButton(int button, int action, GLFWwindow* window);
    void HandleCursorMovement(double xpos, double ypos);
    void HandleScroll(double yoffset);

    // Static callback methods
    static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void CursorPositionCallback(GLFWwindow* window, double xpos, double ypos);
    static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

    // Method to set the current camera instance
    static void SetCurrentCamera(Camera* cam);

    float GetDistanceFromTarget();  // New method to get distance from target

private:
    void updateCameraVectors();

    bool rightMouseButtonPressed = false;
    bool firstMouse = true;
    float lastX = 0.0f, lastY = 0.0f;
    glm::vec3 cameraTarget = glm::vec3(0.0f, -100.0f, 0.0f);

    // Static variable to hold the current camera instance
    static Camera* currentCamera;
};
