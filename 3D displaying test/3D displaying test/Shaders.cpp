#include "shaders.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

std::string Shader::readFile()
{
    std::ifstream file(this->_shaderPath);
    if (!file)
    {
        std::cerr << "Failed to load shader file: " << this->_shaderPath << std::endl;
        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

unsigned int Shader::compileShader()
{
    unsigned int id = glCreateShader(this->_type);
    const char* src = this->readFile().c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = new char[length];
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile " << (this->_type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        delete[] message;
        return 0;
    }

    return id;
}


unsigned int ShaderProgramCreator::createShaderProgram()
{
    unsigned int vs = this->_vertexShader.compileShader();
    unsigned int fs = this->_fragmentShader.compileShader();
    unsigned int program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);

    int result;
    glGetProgramiv(program, GL_LINK_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
        char* message = new char[length];
        glGetProgramInfoLog(program, length, &length, message);
        std::cout << "Failed to link shader program!" << std::endl;
        std::cout << message << std::endl;
        glDeleteProgram(program);
        delete[] message;
        return 0;
    }

    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

void setModelViewProjection(unsigned int shaderProgram, const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection)
{
    unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
    unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
    unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
}
