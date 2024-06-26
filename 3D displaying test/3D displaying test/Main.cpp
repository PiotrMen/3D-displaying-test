#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <iostream>
#include <vector>
#include "shaders.h"
#include "Model.h"
#include "Camera.h"
#include "DisplayManager.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

bool rightMouseButtonPressed = false;
float lastX = 0.0f, lastY = 0.0f;
float yaw = -90.0f, pitch = 0.0f; // Initialize yaw and pitch to set the initial direction
bool firstMouse = true;
float distanceFromTarget = 50.0f; // Distance from the target to the camera

glm::vec3 cameraTarget(0.0f, -100.0f, 0.0f); // The point the camera will orbit around
Camera camera(glm::vec3(0.0f, 0.0f, distanceFromTarget)); // Initial camera position

int main()
{
    // Initialize GLFW
    if (!glfwInit())
        return -1;

    // Set GLFW options
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(primaryMonitor);
    GLFWwindow* window = glfwCreateWindow(1200, 720, "3D Model Viewer", NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetScrollCallback(window, scroll_callback); // Set scroll callback

    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    // Setup ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    // Build and compile our shader program
    GradientShaderProgram gradientShaderProgram(Shader(GL_VERTEX_SHADER, "shaders/Gradient_vertex_shader.glsl"), Shader(GL_FRAGMENT_SHADER, "shaders/Gradient_fragment_shader.glsl"));
    CubeShaderProgram cubeShaderProgram(Shader(GL_VERTEX_SHADER, "shaders/cube_vertex_shader.glsl"), Shader(GL_FRAGMENT_SHADER, "shaders/cube_fragment_shader.glsl"));
    ShadowShaderProgram shadowShaderProgram(Shader(GL_VERTEX_SHADER, "shaders/ligthing_vertex_shader.glsl"), Shader(GL_FRAGMENT_SHADER, "shaders/ligthing_fragment_shader.glsl"));

    // Load model
    Model modelObj("3d files/figure_Hollow_Supp.stl");
    Model lightcube(ModelType::Cube, glm::vec3(250.0f, -80.0f, 100.0f), glm::vec3(20.0f));
    Object3DDisplayer object3DDisplayer(mode->width, mode->height);

    // Ustawianie macierzy modelu
    modelObj.translate(glm::vec3(0.0f, 0.0f, 0.0f));
    modelObj.rotate(glm::radians(90.f), glm::vec3(1.0f, 0.0f, 0.0f));
    modelObj.scale(glm::vec3(1.0f));

    ShaderMode selectedShaderMode = ShaderMode::Gradient;
    RenderingMode selectedRenderingMode = RenderingMode::FILL;

    // Variables for light properties
    static float ambientStrength = 0.1f;
    static float diffuseStrength = 0.4f;
    static float specularStrength = 0.7f;

    // skala promienia
    static float radius = 400.0f;

    // Variable for percentage of used element
    static float elementUsagePercentage = 100.0f;

    // Variable to control camera rotation
    bool rotateCamera = false;
    static float currentTime = 0.0f;
    static float lastTime = 0.0f;

    // Render loop
    while (!glfwWindowShouldClose(window))
    {
        // Start the ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Show a simple window
        ImGui::Begin("Options");

        // Input
        processInput(window);

        // Clear the color and depth buffer for the outer window
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // ImGui Radio buttons do wyboru shaderów
        if (ImGui::RadioButton("Gradient Shader", selectedShaderMode == ShaderMode::Gradient)) selectedShaderMode = ShaderMode::Gradient;
        ImGui::SameLine();
        if (ImGui::RadioButton("Lighting Shader", selectedShaderMode == ShaderMode::Lighting)) selectedShaderMode = ShaderMode::Lighting;
        // Display sliders only if Lighting Shader is selected
        if (selectedShaderMode == ShaderMode::Lighting) {
            ImGui::SliderFloat("Ambient Strength", &ambientStrength, 0.0f, 1.0f);
            ImGui::SliderFloat("Diffuse Strength", &diffuseStrength, 0.0f, 1.0f);
            ImGui::SliderFloat("Specular Strength", &specularStrength, 0.0f, 1.0f);
        }

        // ImGui radio buttons for rendering mode selection
        if (selectedShaderMode == ShaderMode::Gradient) {
            if (ImGui::RadioButton("Point Cloud", selectedRenderingMode == RenderingMode::POINT)) selectedRenderingMode = RenderingMode::POINT;
            ImGui::SameLine();
            if (ImGui::RadioButton("Wireframe", selectedRenderingMode == RenderingMode::LINE)) selectedRenderingMode = RenderingMode::LINE;
            ImGui::SameLine();
            if (ImGui::RadioButton("Filled", selectedRenderingMode == RenderingMode::FILL)) selectedRenderingMode = RenderingMode::FILL;
            ImGui::PushItemWidth(1000);  // Set width to 1000 pixels
            ImGui::SliderFloat("Element Usage (%)", &elementUsagePercentage, 0.0f, 100.0f);
        }
        else {
            selectedRenderingMode = RenderingMode::FILL;
            elementUsagePercentage = 100.0f;
        }

        // Add a button to toggle camera rotation
        if (ImGui::Button("Camera Rotation")) {
            rotateCamera = !rotateCamera;
            if (rotateCamera) {
                lastTime = glfwGetTime();  // Reset current time when enabling rotation
            }
        }
        ImGui::End();

        // Tworzenie transformacji
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), static_cast<float>(mode->width) / static_cast<float>(mode->height), 0.1f, 1000.0f);
        glm::mat4 view = camera.GetViewMatrix();  // Uzyskanie macierzy widoku z kamery

        ImGui::Begin("3D Model");
        //ustawienie shaderow
        glBindFramebuffer(GL_FRAMEBUFFER, object3DDisplayer.getFrameBuffer());
        glEnable(GL_DEPTH_TEST);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        if (selectedShaderMode == ShaderMode::Lighting) {
            shadowShaderProgram.use();
            shadowShaderProgram.setValues(projection, view, camera, lightcube.getModelPos(), ambientStrength, diffuseStrength, specularStrength);
            object3DDisplayer.display(modelObj, &shadowShaderProgram, selectedRenderingMode, elementUsagePercentage);
            cubeShaderProgram.use();
            cubeShaderProgram.setValues(projection, view, lightcube.getModelMatrix());
            object3DDisplayer.display(lightcube, &cubeShaderProgram, selectedRenderingMode, elementUsagePercentage);
        }
        else if (selectedShaderMode == ShaderMode::Gradient) {
            gradientShaderProgram.use();
            gradientShaderProgram.setValues(projection, view, camera, radius);
            object3DDisplayer.display(modelObj, &gradientShaderProgram, selectedRenderingMode, elementUsagePercentage);
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        ImGui::Image((void*)(intptr_t)object3DDisplayer.getTexColorBuffer(), ImVec2(mode->width, mode->height));
        ImGui::End();

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_RIGHT)
    {
        if (action == GLFW_PRESS)
        {
            rightMouseButtonPressed = true;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            firstMouse = true; // Reset firstMouse to ensure smooth transition
        }
        else if (action == GLFW_RELEASE)
        {
            rightMouseButtonPressed = false;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
    }
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
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
        float yOffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

        lastX = xpos;
        lastY = ypos;

        float sensitivity = 0.1f; // change this value to your liking
        xOffset *= sensitivity;
        yOffset *= sensitivity;

        yaw += xOffset;
        pitch += yOffset;

        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;

        // Update camera position based on yaw, pitch, and distance from the target
        glm::vec3 newPosition;
        newPosition.x = cameraTarget.x + distanceFromTarget * cos(glm::radians(pitch)) * cos(glm::radians(yaw));
        newPosition.y = cameraTarget.y + distanceFromTarget * sin(glm::radians(pitch));
        newPosition.z = cameraTarget.z + distanceFromTarget * cos(glm::radians(pitch)) * sin(glm::radians(yaw));
        camera.Position = newPosition;

        // Always look at the target
        camera.Front = glm::normalize(cameraTarget - camera.Position);
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    distanceFromTarget -= (float)yoffset;
    if (distanceFromTarget < 1.0f)
        distanceFromTarget = 1.0f;

    // Update camera position based on the new distance
    glm::vec3 newPosition;
    newPosition.x = cameraTarget.x + distanceFromTarget * cos(glm::radians(pitch)) * cos(glm::radians(yaw));
    newPosition.y = cameraTarget.y + distanceFromTarget * sin(glm::radians(pitch));
    newPosition.z = cameraTarget.z + distanceFromTarget * cos(glm::radians(pitch)) * sin(glm::radians(yaw));
    camera.Position = newPosition;

    // Always look at the target
    camera.Front = glm::normalize(cameraTarget - camera.Position);
}
