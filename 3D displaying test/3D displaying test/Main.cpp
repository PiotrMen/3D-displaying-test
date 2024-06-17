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
#include "Cube.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

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
	ShaderProgram shaderGradientProgram(Shader(GL_VERTEX_SHADER, "shaders/Gradient_vertex_shader.glsl"), Shader(GL_FRAGMENT_SHADER, "shaders/Gradient_fragment_shader.glsl"));
	ShaderProgram shaderLightProgram(Shader(GL_VERTEX_SHADER, "shaders/ligthing_vertex_shader.glsl"), Shader(GL_FRAGMENT_SHADER, "shaders/ligthing_fragment_shader.glsl"));
	ShaderProgram shaderCube(Shader(GL_VERTEX_SHADER, "shaders/cube_vertex_shader.glsl"), Shader(GL_FRAGMENT_SHADER, "shaders/cube_fragment_shader.glsl"));

	// Load model
	Model modelObj("3d files/figure_Hollow_Supp.stl");
	Camera camera(glm::vec3(0.0f, 0.0f, 10.0f));

	glEnable(GL_DEPTH_TEST);

	// Create framebuffer
	unsigned int framebuffer;
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	// Create a color attachment texture
	unsigned int texColorBuffer;
	glGenTextures(1, &texColorBuffer);
	glBindTexture(GL_TEXTURE_2D, texColorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mode->width, mode->height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer, 0);

	// Create a renderbuffer object for depth and stencil attachment
	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, mode->width, mode->height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	// Check if framebuffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//Lokazja, oraz rozmiar
	Cube lightcube(250.0f, -80.0f, 100.0f, glm::vec3(20.0f));
	Cube basicCube(0.0f, 0.0f, 0.0f, glm::vec3(80.0f));
	glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
	// Ustawianie macierzy modelu
	modelObj.setModelMatrix(glm::translate(modelObj.getModelMatrix(), glm::vec3(0.0f, 0.0f, 0.0f))); // ustawienie bry�y w uk�adzie
	modelObj.setModelMatrix(glm::rotate(modelObj.getModelMatrix(), glm::radians(90.f), glm::vec3(1.0f, 0.0f, 0.0f))); // rotacja bry�y mo�liwa r�wnie� przez obrot kamery wektor "Front"
	modelObj.setModelMatrix(glm::scale(modelObj.getModelMatrix(), glm::vec3(1.0f)));

	// Array of shader names
	const char* shaderNames[] = { "Gradient Shader", "Lighting Shader" };
	int selectedShader = 0;
	// Array of object names
	const char* objectNames[] = { "Model", "Cube" };
	int selectedObject = 0;
	// Array of rendering mode names
	const char* renderingModeNames[] = { "Point Cloud", "Wireframe", "Filled" };
	int selectedRenderingMode = 2; // Default to filled
	// Variables for light properties
	static float ambientStrength = 0.1f;
	static float diffuseStrength = 0.4f;
	static float specularStrength = 0.7f;
	// Variable for percentage of used element
	static float elementUsagePercentage = 100.0f;
	// Render loop
	while (!glfwWindowShouldClose(window))
	{
		// Start the ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// Show a simple window
		ImGui::Begin("3D Model Viewer");

		// Input
		processInput(window);

		// Clear the color and depth buffer for the outer window
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// First pass: render the scene to the framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// ImGui Radio buttons do wyboru shader�w
		ImGui::PushItemWidth(150);  // Ustaw szeroko�� na 150 pikseli
		if (ImGui::RadioButton("Gradient Shader", selectedShader == 0)) selectedShader = 0;
		ImGui::SameLine();
		if (ImGui::RadioButton("Lighting Shader", selectedShader == 1)) selectedShader = 1;
		// Display sliders only if Lighting Shader is selected
		if (selectedShader == 1) {
			ImGui::SliderFloat("Ambient Strength", &ambientStrength, 0.0f, 1.0f);
			ImGui::SliderFloat("Diffuse Strength", &diffuseStrength, 0.0f, 1.0f);
			ImGui::SliderFloat("Specular Strength", &specularStrength, 0.0f, 1.0f);
		}

		// ImGui radio buttons for object selection
		ImGui::PushItemWidth(150);  // Set width to 150 pixels
		if (ImGui::RadioButton("Model", selectedObject == 0)) selectedObject = 0;
		ImGui::SameLine();
		if (ImGui::RadioButton("Cube", selectedObject == 1)) selectedObject = 1;

		// ImGui radio buttons for rendering mode selection
		if (selectedShader == 0) {
			ImGui::RadioButton("Point Cloud", &selectedRenderingMode, 0);
			ImGui::SameLine();
			ImGui::RadioButton("Wireframe", &selectedRenderingMode, 1);
			ImGui::SameLine();
			ImGui::RadioButton("Filled", &selectedRenderingMode, 2);
			ImGui::SliderFloat("Element Usage (%)", &elementUsagePercentage, 0.0f, 100.0f);
		}else {
			elementUsagePercentage = 100.0f;
		}

		ShaderProgram* currentShader = (selectedShader == 0) ? &shaderGradientProgram : &shaderLightProgram;
		// Set rendering mode
		if (selectedShader == 1) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		else {
			if (selectedRenderingMode == 0) {
				glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
			}
			else if (selectedRenderingMode == 1) {
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			}
			else if (selectedRenderingMode == 2) {
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}
		}
		// Draw the model
		currentShader->use();

		// Transformacja kamery
		//iloraz przyblizenia kamery do obiektu
		float radius = 400.0f;
		float camX = static_cast<float>(sin(glfwGetTime()) * radius);
		float camZ = static_cast<float>(cos(glfwGetTime()) * radius);
		float camY = static_cast<float>(sin(glfwGetTime()) * radius);
		//warto�� y to wysokosc kamery
		camera.Position = glm::vec3(camX, -100.0f, camZ);
		glm::vec3 cameraDirection = glm::normalize(camera.Position - cameraTarget);
		camera.Front = -cameraDirection;  // Ustawienie kierunku kamery

		// Tworzenie transformacji
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), static_cast<float>(mode->width) / static_cast<float>(mode->height), 0.1f, 1000.0f);
		glm::mat4 view = camera.GetViewMatrix();  // Uzyskanie macierzy widoku z kamery

		// Set common shader uniforms
		currentShader->setMat4("projection", projection);
		currentShader->setMat4("view", view);
		currentShader->setVec3("cameraPos", camera.Position);

		//ustawienie shaderow
		if (selectedShader == 1) {
			//lightingShader
			currentShader->setVec3("lightPos", lightcube.getCubePos());
			currentShader->setVec3("viewPos", camera.Position);
			currentShader->setVec3("lightColor", 1.0f, 1.0f, 1.0f);
			currentShader->setVec3("objectColor", glm::vec3(0.5f, 0.5f, 0.5f));
			currentShader->setFloat("ambientStrength", ambientStrength);
			currentShader->setFloat("diffuseStrength", diffuseStrength);
			currentShader->setFloat("specularStrength", specularStrength);
		}else if (selectedShader == 0) {
			//Gradient shader
			currentShader->setFloat("maxDistance", radius);
			currentShader->setFloat("gradientPow", 7.0);
		}
		


		// Render the selected object with element usage percentage
		int vertexCount = static_cast<int>(modelObj.getVertices().size() * (elementUsagePercentage / 100.0f));
		//ustawienie modeli
		if (selectedObject == 0) {
			modelObj.bind();
			currentShader->setMat4("model", modelObj.getModelMatrix());
			glDrawArrays(GL_TRIANGLES, 0, vertexCount);
		}else if (selectedObject == 1) {		
			currentShader->setMat4("model", basicCube.getCubeModel().getModelMatrix());
			basicCube.getCubeModel().bind();
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		
		if (selectedShader == 1) {
			shaderCube.use();
			shaderCube.setMat4("projection", projection);
			shaderCube.setMat4("view", view);
			shaderCube.setMat4("model", lightcube.getCubeModel().getModelMatrix());

			// Zak�adaj�c, �e masz sze�cian z 36 wierzcho�kami
			lightcube.getCubeModel().bind();
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		// Second pass: render the framebuffer texture to the default framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST);

		ImGui::Image((void*)(intptr_t)texColorBuffer, ImVec2(mode->width, mode->height));
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
	modelObj.cleanup();
	//glDeleteProgram(shaderProgram);
	glDeleteFramebuffers(1, &framebuffer);
	glDeleteTextures(1, &texColorBuffer);
	glDeleteRenderbuffers(1, &rbo);

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
