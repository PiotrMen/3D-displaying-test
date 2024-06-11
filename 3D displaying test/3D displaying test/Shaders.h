#pragma once

#include <glad/glad.h>
#include <string>
#include <glm/glm.hpp>
#include <iostream>

class Shader {
public:
	explicit Shader(unsigned int type,const std::string& filePath) : _shaderPath(filePath),_type(type) {}
	std::string readFile();
	unsigned int compileShader();

private:
	std::string _shaderPath;
	unsigned int _type;
};

class ShaderProgramCreator {
public:
	ShaderProgramCreator(const Shader& vertexShader, const Shader& fragmentShader) : _vertexShader(vertexShader), _fragmentShader(fragmentShader) {}

	unsigned int createShaderProgram();
private:
	Shader _vertexShader;
	Shader _fragmentShader;
};

void setModelViewProjection(unsigned int shaderProgram, const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection);
