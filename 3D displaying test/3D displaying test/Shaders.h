#pragma once

#include <glad/glad.h>
#include <string>
#include <glm/glm.hpp>
#include <iostream>

class Shader {
public:
	Shader(const std::string& filePath);
private:
	std::string _shaderPath;
	
};

unsigned int createShaderProgram(const std::string& vertexPath, const std::string& fragmentPath);

std::string readShaderFile(const std::string& filePath);
unsigned int compileShader(unsigned int type, const std::string& source);
void setModelViewProjection(unsigned int shaderProgram, const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection);
