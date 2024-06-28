#include "Camera.h"

Camera* Camera::currentCamera = nullptr;

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
    : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM), distanceFromTarget(50.0f) // Initialize distanceFromTarget
{
    Position = glm::vec3(posX, posY, posZ);
    WorldUp = glm::vec3(upX, upY, upZ);
    Yaw = yaw;
    Pitch = pitch;
    updateCameraVectors();
}

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
    : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM), distanceFromTarget(50.0f) // Initialize distanceFromTarget
{
    Position = position;
    WorldUp = up;
    Yaw = yaw;
    Pitch = pitch;
    updateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix()
{
    return glm::lookAt(Position, Position + Front, Up);
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
{
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    Yaw += xoffset;
    Pitch += yoffset;

    if (constrainPitch)
    {
        if (Pitch > 89.0f)
            Pitch = 89.0f;
        if (Pitch < -89.0f)
            Pitch = -89.0f;
    }

    updateCameraVectors();
}

void Camera::ProcessMouseScroll(float yoffset)
{
    Zoom -= (float)yoffset;
    if (Zoom < 1.0f)
        Zoom = 1.0f;
    if (Zoom > 45.0f)
        Zoom = 45.0f;
}

void Camera::HandleMouseButton(int button, int action, GLFWwindow* window)
{
    if (button == GLFW_MOUSE_BUTTON_RIGHT)
    {
        if (action == GLFW_PRESS)
        {
            rightMouseButtonPressed = true;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            firstMouse = true;
        }
        else if (action == GLFW_RELEASE)
        {
            rightMouseButtonPressed = false;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
    }
}

void Camera::HandleCursorMovement(double xpos, double ypos)
{
    if (rightMouseButtonPressed)
    {
        if (firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        float xOffset = xpos - lastX;
        float yOffset = lastY - ypos;

        lastX = xpos;
        lastY = ypos;

        float sensitivity = 0.1f;
        xOffset *= sensitivity;
        yOffset *= sensitivity;

        Yaw += xOffset;
        Pitch += yOffset;

        if (Pitch > 89.0f)
            Pitch = 89.0f;
        if (Pitch < -89.0f)
            Pitch = -89.0f;

        glm::vec3 newPosition;
        newPosition.x = cameraTarget.x + distanceFromTarget * cos(glm::radians(Pitch)) * cos(glm::radians(Yaw));
        newPosition.y = cameraTarget.y + distanceFromTarget * sin(glm::radians(Pitch));
        newPosition.z = cameraTarget.z + distanceFromTarget * cos(glm::radians(Pitch)) * sin(glm::radians(Yaw));
        Position = newPosition;

        Front = glm::normalize(cameraTarget - Position);
    }
}

void Camera::HandleScroll(double yoffset)
{
    distanceFromTarget -= (float)yoffset;
    if (distanceFromTarget < 1.0f)
        distanceFromTarget = 1.0f;

    glm::vec3 newPosition;
    newPosition.x = cameraTarget.x + distanceFromTarget * cos(glm::radians(Pitch)) * cos(glm::radians(Yaw));
    newPosition.y = cameraTarget.y + distanceFromTarget * sin(glm::radians(Pitch));
    newPosition.z = cameraTarget.z + distanceFromTarget * cos(glm::radians(Pitch)) * sin(glm::radians(Yaw));
    Position = newPosition;

    Front = glm::normalize(cameraTarget - Position);
}

void Camera::updateCameraVectors()
{
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);

    Right = glm::normalize(glm::cross(Front, WorldUp));
    Up = glm::normalize(glm::cross(Right, Front));
}

void Camera::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    if (currentCamera)
    {
        currentCamera->HandleMouseButton(button, action, window);
    }
}

void Camera::CursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
{
    if (currentCamera)
    {
        currentCamera->HandleCursorMovement(xpos, ypos);
    }
}

void Camera::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    if (currentCamera)
    {
        currentCamera->HandleScroll(yoffset);
    }
}

void Camera::SetCurrentCamera(Camera* cam)
{
    currentCamera = cam;
}

float Camera::GetDistanceFromTarget()
{
    return glm::length(Position - cameraTarget);
}
