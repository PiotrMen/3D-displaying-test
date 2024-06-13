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
#include <random>

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 color;  
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void loadModel(const std::string& path, std::vector<Vertex>& vertices);


enum RenderMode { POINTS, WIREFRAME, SOLID };
RenderMode currentMode = POINTS;
//
//void renderPoints(GLsizei count) {
//	std::random_device rd;
//	std::mt19937 gen(rd());
//	std::uniform_int_distribution<> dis(0, vertices.size() - 1);
//
//	std::vector<Vertex> selectedVertices;
//	for (int i = 0; i < count; ++i) {
//		int index = dis(gen);
//		selectedVertices.push_back(vertices[index]);
//	}
//
//	glBindVertexArray(VAO);
//	glBindBuffer(GL_ARRAY_BUFFER, VBO);
//	glBufferData(GL_ARRAY_BUFFER, selectedVertices.size() * sizeof(Vertex), &selectedVertices[0], GL_STATIC_DRAW);
//	glDrawArrays(GL_POINTS, 0, selectedVertices.size());
//	glBindVertexArray(0);
//}


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

	// Setup ImGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	// Build and compile our shader program
	ShaderProgramCreator shaderProgramCreator(Shader(GL_VERTEX_SHADER, "shaders/vertex_shader.glsl"), Shader(GL_FRAGMENT_SHADER, "shaders/fragment_shader.glsl"));

	unsigned int shaderProgram = shaderProgramCreator.createShaderProgram();

	// Load model
	std::vector<Vertex> vertices;
	loadModel("3d files/figure_Hollow_Supp.stl", vertices);

	// Set up vertex data and buffers and configure vertex attributes
	unsigned int VBO, VAO;
	//glGenVertexArrays(1, &VAO);
	//glGenBuffers(1, &VBO);

	//glBindVertexArray(VAO);

	//glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(Vertex), (void*)0);
	//glEnableVertexAttribArray(0);

	//glEnable(GL_DEPTH_TEST);

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));

	glBindVertexArray(0);

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


	float pointPercentage = 10.0;
	GLsizei pointCount;
	std::vector<Vertex> selectedVertices;
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, vertices.size() - 1);
	pointCount = static_cast<GLsizei>(vertices.size() * (pointPercentage / 100.0f));

	for (int i = 0; i < pointCount; ++i) {
		int index = dis(gen);
		selectedVertices.push_back(vertices[index]);
	}


	// Render loop
	while (!glfwWindowShouldClose(window))
	{
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
		glUseProgram(shaderProgram);

		// Create transformations
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);
		//glm::mat4 cameraPos4 = glm::mat4(1.0f);
		glm::vec3 cameraPos = glm::vec3(0.0f, 50.0f, -50.0f);

		glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		// Transformujemy wektor za pomoc¹ macierzy rotacji
		glm::vec4 rotatedV = rotationMatrix * glm::vec4(cameraPos, 1.0f);

		cameraPos =  glm::vec3(rotatedV);
		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // Ustawienie bry³y w uk³adzie
		view = glm::translate(view, glm::vec3(0.0f, 50.0f, -50.0)); // Ustawienie kamery w uk³adzie wspó³rzêdnych
		view = glm::rotate(view, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)); // Obróæ kamerê w przestrzeni
		//view = glm::rotate(cameraPos, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)); // Obróæ kamerê w przestrzeni
		//model = glm::rotate(model, (float)glfwGetTime() * glm::radians(45.f), glm::vec3(0.0f, 0.0f, 1.0f)); // Rotacja bry³y
		projection = glm::perspective(glm::radians(45.0f), static_cast<float>(mode->width) / static_cast<float>(mode->height), 0.1f, 1000.0f);

		GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");
		GLuint viewLoc = glGetUniformLocation(shaderProgram, "view");
		GLuint projLoc = glGetUniformLocation(shaderProgram, "projection");
		GLuint cameraPosLoc = glGetUniformLocation(shaderProgram, "cameraPos");

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniform3fv(cameraPosLoc, 1, glm::value_ptr(cameraPos));

		glBindVertexArray(VAO);

		switch (currentMode) {
		case POINTS:
			glBindVertexArray(VAO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, selectedVertices.size() * sizeof(Vertex), &selectedVertices[0], GL_STATIC_DRAW);
			glPointSize(1.0f);
			glDrawArrays(GL_POINTS, 0, selectedVertices.size());
			glBindVertexArray(0);
			break;
		case WIREFRAME:
			//glBindVertexArray(VAO);
			//glBindBuffer(GL_ARRAY_BUFFER, VBO);
			//glBufferData(GL_ARRAY_BUFFER, selectedVertices.size() * sizeof(Vertex), &selectedVertices[0], GL_STATIC_DRAW);
			//glPolygonMode(GL_FRONT_FACE, GL_POINTS);
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

			glDrawArrays(GL_TRIANGLES, 0, selectedVertices.size());
			glBindVertexArray(0);
			break;
		case SOLID:
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glDrawArrays(GL_TRIANGLES, 0, selectedVertices.size());
			break;
		}

		// Second pass: render the framebuffer texture to the default framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST);

		// Start the ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// Show a simple window
		ImGui::Begin("3D Model Viewer");
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
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);
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

void loadModel(const std::string& path, std::vector<Vertex>& vertices) {
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}

	aiMesh* mesh = scene->mMeshes[0];
	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		Vertex vertex;
		vertex.position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
		if (mesh->HasNormals()) {
			vertex.normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
		}
		//// Przypisz kolor na podstawie odleg³oœci od kamery
		//float distance = glm::length(vertex.position);
		//vertex.color = glm::vec3(1.0f - distance / 100.0f, 0.0f, distance / 100.0f);  // Prosty gradient kolorów, poprawiony zakres
		//vertices.push_back(vertex);

		vertex.color = glm::vec3(1.0f, 1.0f, 1.0f); // Initial color (white), will be recalculated in the vertex shader
		vertices.push_back(vertex);
	}
}

//void loadModel(const std::string& path, std::vector<float>& vertices)
//{
//	Assimp::Importer importer;
//	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
//	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
//	{
//		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
//		return;
//	}
//
//	aiMesh* mesh = scene->mMeshes[0];
//	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
//	{
//		vertices.push_back(mesh->mVertices[i].x);
//		vertices.push_back(mesh->mVertices[i].y);
//		vertices.push_back(mesh->mVertices[i].z);
//	}
//}