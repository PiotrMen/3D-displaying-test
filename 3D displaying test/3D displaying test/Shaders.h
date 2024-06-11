#pragma once

#include <glad/glad.h>
#include <string>
#include <glm/glm.hpp>
#include <iostream>

std::string readShaderFile(const std::string& filePath);
unsigned int compileShader(unsigned int type, const std::string& source);
unsigned int createShaderProgram(const std::string& vertexPath, const std::string& fragmentPath);
void setModelViewProjection(unsigned int shaderProgram, const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection);
