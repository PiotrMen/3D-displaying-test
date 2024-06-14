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
	//ShaderProgram shaderProgram(Shader(GL_VERTEX_SHADER, "shaders/Gradient_vertex_shader.glsl"), Shader(GL_FRAGMENT_SHADER, "shaders/Gradient_fragment_shader.glsl"));
	ShaderProgram shaderProgram(Shader(GL_VERTEX_SHADER, "shaders/ligthing_vertex_shader.glsl"), Shader(GL_FRAGMENT_SHADER, "shaders/ligthing_fragment_shader.glsl"));
	ShaderProgram shaderCube(Shader(GL_VERTEX_SHADER, "shaders/cube_vertex_shader.glsl"), Shader(GL_FRAGMENT_SHADER, "shaders/cube_fragment_shader.glsl"));

	// Load model
	Model modelObj("3d files/figure_Hollow_Supp.stl");
	Camera camera(glm::vec3(0.0f, 0.0f, 10.0f));

	// Define and initialize the cube model
	std::vector<Vertex> cubeVertices = {
	{{-0.5f, -0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}, {1.0f, 1.0f, 1.0f}},
	{{ 0.5f, -0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}, {1.0f, 1.0f, 1.0f}},
	{{ 0.5f,  0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}, {1.0f, 1.0f, 1.0f}},
	{{ 0.5f,  0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}, {1.0f, 1.0f, 1.0f}},
	{{-0.5f,  0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}, {1.0f, 1.0f, 1.0f}},
	{{-0.5f, -0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}, {1.0f, 1.0f, 1.0f}},

	{{-0.5f, -0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}, {1.0f, 1.0f, 1.0f}},
	{{ 0.5f, -0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}, {1.0f, 1.0f, 1.0f}},
	{{ 0.5f,  0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}, {1.0f, 1.0f, 1.0f}},
	{{ 0.5f,  0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}, {1.0f, 1.0f, 1.0f}},
	{{-0.5f,  0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}, {1.0f, 1.0f, 1.0f}},
	{{-0.5f, -0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}, {1.0f, 1.0f, 1.0f}},

	{{-0.5f,  0.5f,  0.5f}, {-1.0f,  0.0f,  0.0f}, {1.0f, 1.0f, 1.0f}},
	{{-0.5f,  0.5f, -0.5f}, {-1.0f,  0.0f,  0.0f}, {1.0f, 1.0f, 1.0f}},
	{{-0.5f, -0.5f, -0.5f}, {-1.0f,  0.0f,  0.0f}, {1.0f, 1.0f, 1.0f}},
	{{-0.5f, -0.5f, -0.5f}, {-1.0f,  0.0f,  0.0f}, {1.0f, 1.0f, 1.0f}},
	{{-0.5f, -0.5f,  0.5f}, {-1.0f,  0.0f,  0.0f}, {1.0f, 1.0f, 1.0f}},
	{{-0.5f,  0.5f,  0.5f}, {-1.0f,  0.0f,  0.0f}, {1.0f, 1.0f, 1.0f}},

	{{ 0.5f,  0.5f,  0.5f}, { 1.0f,  0.0f,  0.0f}, {1.0f, 1.0f, 1.0f}},
	{{ 0.5f,  0.5f, -0.5f}, { 1.0f,  0.0f,  0.0f}, {1.0f, 1.0f, 1.0f}},
	{{ 0.5f, -0.5f, -0.5f}, { 1.0f,  0.0f,  0.0f}, {1.0f, 1.0f, 1.0f}},
	{{ 0.5f, -0.5f, -0.5f}, { 1.0f,  0.0f,  0.0f}, {1.0f, 1.0f, 1.0f}},
	{{ 0.5f, -0.5f,  0.5f}, { 1.0f,  0.0f,  0.0f}, {1.0f, 1.0f, 1.0f}},
	{{ 0.5f,  0.5f,  0.5f}, { 1.0f,  0.0f,  0.0f}, {1.0f, 1.0f, 1.0f}},

	{{-0.5f, -0.5f, -0.5f}, { 0.0f, -1.0f,  0.0f}, {1.0f, 1.0f, 1.0f}},
	{{ 0.5f, -0.5f, -0.5f}, { 0.0f, -1.0f,  0.0f}, {1.0f, 1.0f, 1.0f}},
	{{ 0.5f, -0.5f,  0.5f}, { 0.0f, -1.0f,  0.0f}, {1.0f, 1.0f, 1.0f}},
	{{ 0.5f, -0.5f,  0.5f}, { 0.0f, -1.0f,  0.0f}, {1.0f, 1.0f, 1.0f}},
	{{-0.5f, -0.5f,  0.5f}, { 0.0f, -1.0f,  0.0f}, {1.0f, 1.0f, 1.0f}},
	{{-0.5f, -0.5f, -0.5f}, { 0.0f, -1.0f,  0.0f}, {1.0f, 1.0f, 1.0f}},

	{{-0.5f,  0.5f, -0.5f}, { 0.0f,  1.0f,  0.0f}, {1.0f, 1.0f, 1.0f}},
	{{ 0.5f,  0.5f, -0.5f}, { 0.0f,  1.0f,  0.0f}, {1.0f, 1.0f, 1.0f}},
	{{ 0.5f,  0.5f,  0.5f}, { 0.0f,  1.0f,  0.0f}, {1.0f, 1.0f, 1.0f}},
	{{ 0.5f,  0.5f,  0.5f}, { 0.0f,  1.0f,  0.0f}, {1.0f, 1.0f, 1.0f}},
	{{-0.5f,  0.5f,  0.5f}, { 0.0f,  1.0f,  0.0f}, {1.0f, 1.0f, 1.0f}},
	{{-0.5f,  0.5f, -0.5f}, { 0.0f,  1.0f,  0.0f}, {1.0f, 1.0f, 1.0f}}
	};
	Model cubeModel(cubeVertices);

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

	glm::vec3 cameraTarget = glm::vec3(0.0f, -100.0f, 0.0f);
	glm::vec3 lightPos(150.0f, -150.0f, 0.0f);
	// Ustawianie macierzy modelu
	modelObj.setModelMatrix(glm::translate(modelObj.getModelMatrix(), glm::vec3(0.0f, 0.0f, 0.0f))); // ustawienie bry³y w uk³adzie
	modelObj.setModelMatrix(glm::rotate(modelObj.getModelMatrix(), glm::radians(90.f), glm::vec3(1.0f, 0.0f, 0.0f))); // rotacja bry³y mo¿liwa równie¿ przez obrot kamery wektor "Front"

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

		// Draw the model
		shaderProgram.use();

		// Transformacja kamery
		//iloraz przyblizenia kamery do obiektu
		float radius = 300.0f;
		float camX = static_cast<float>(sin(glfwGetTime()) * radius);
		float camZ = static_cast<float>(cos(glfwGetTime()) * radius);
		float camY = static_cast<float>(sin(glfwGetTime()) * radius);
		//wartoœæ y to wysokosc kamery
		camera.Position = glm::vec3(camX, -100.0f, camZ);
		glm::vec3 cameraDirection = glm::normalize(camera.Position - cameraTarget);
		camera.Front = -cameraDirection;  // Ustawienie kierunku kamery

		// Tworzenie transformacji
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), static_cast<float>(mode->width) / static_cast<float>(mode->height), 0.1f, 1000.0f);
		glm::mat4 view = camera.GetViewMatrix();  // Uzyskanie macierzy widoku z kamery
		
		static float ambientStrength = 0.1f;
		ImGui::Begin("Light slider");
		ImGui::SliderFloat("ambientStrength", &ambientStrength, 0.0f, 1.0f);
		ImGui::Text("Value: %.3f", ambientStrength);
		//ImGui::SameLine();

		static float diffuseStrength = 0.5f;
		ImGui::SliderFloat("diffuseStrength", &diffuseStrength, 0.0f, 1.0f);
		ImGui::Text("Value: %.3f", diffuseStrength);

		static float specularStrength = 0.5f;
		ImGui::SliderFloat("specularStrength", &specularStrength, 0.0f, 1.0f);
		ImGui::Text("Value: %.3f", specularStrength);
		ImGui::End();
		modelObj.bind();
		//Gradient shader
		shaderProgram.setMat4("model", modelObj.getModelMatrix());
		shaderProgram.setMat4("view", view);
		shaderProgram.setMat4("projection", projection);
		shaderProgram.setVec3("cameraPos", camera.Position);
		shaderProgram.setFloat("maxDistance", radius);
		shaderProgram.setFloat("gradientPow", 7.0);

		//lightingShader
		shaderProgram.setVec3("lightPos", lightPos);
		shaderProgram.setVec3("viewPos", camera.Position);
		shaderProgram.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
		shaderProgram.setVec3("objectColor", glm::vec3(0.5f, 0.5f, 0.5f));
		shaderProgram.setFloat("ambientStrength", ambientStrength);
		shaderProgram.setFloat("diffuseStrength", diffuseStrength);
		shaderProgram.setFloat("specularStrength", specularStrength);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDrawArrays(GL_TRIANGLES, 0, modelObj.getVertices().size());

		shaderCube.use();
		shaderCube.setMat4("projection", projection);
		shaderCube.setMat4("view", view);
		glm::mat4 lamp = glm::mat4(1.0f);
		lamp = glm::translate(lamp, lightPos);
		lamp = glm::scale(lamp, glm::vec3(10.0f)); // wiêksza skala lampy
		shaderCube.setMat4("model", lamp);

		// Zak³adaj¹c, ¿e masz szeœcian z 36 wierzcho³kami
		cubeModel.bind();
		glDrawArrays(GL_TRIANGLES, 0, 36);

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
