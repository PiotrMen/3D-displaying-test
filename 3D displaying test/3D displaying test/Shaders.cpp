#include "shaders.h"
#include <fstream>
#include <sstream>
#include <iostream>


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
    this->_id = glCreateShader(this->_type);
    std::string src = this->readFile();
    const char* stringToChar = src.c_str();
    glShaderSource(this->_id, 1, &stringToChar, nullptr);
    glCompileShader(this->_id);

    if (!this->checkCompileErrors())
    {
        glDeleteShader(this->_id);
        return 0;
    }

    return this->_id;
}

unsigned int Shader::getType() const noexcept
{
	return this->_type;
}

unsigned int Shader::getID() const noexcept
{
	return this->_id;
}


bool Shader::checkCompileErrors() const
{
	int success;
	char infoLog[1024];
	glGetShaderiv(this->_id, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(this->_id, 1024, NULL, infoLog);
		std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << (this->_type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " and path: " << this->_shaderPath << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		return false;
	}
	return true;
}

ShaderProgram::ShaderProgram(const Shader& vertexShader, const Shader& fragmentShader) : _vertexShader(vertexShader), _fragmentShader(fragmentShader), _id(0)
{

}

ShaderProgram::~ShaderProgram()
{
    this->deleteProgram();
}

unsigned int ShaderProgram::createShaderProgram()
{
	unsigned int vs = this->_vertexShader.compileShader();
	unsigned int fs = this->_fragmentShader.compileShader();
	this->_id = glCreateProgram();
	glAttachShader(this->_id, vs);
	glAttachShader(this->_id, fs);
	glLinkProgram(this->_id);

	if (!this->checkCompileErrors(this->_id))
	{
		glDeleteProgram(this->_id);
		return 0;
	}

	glValidateProgram(this->_id);

	glDeleteShader(vs);
	glDeleteShader(fs);

    return this->_id;
}

unsigned int ShaderProgram::getID() const noexcept
{
    return this->_id;
}

void ShaderProgram::deleteProgram() const
{
    glDeleteProgram(this->_id);
}

void ShaderProgram::use()
{
	glUseProgram(this->_id);
}

void ShaderProgram::setBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(this->_id, name.c_str()), (int)value);
}

void ShaderProgram::setInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(this->_id, name.c_str()), value);
}

void ShaderProgram::setFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(this->_id, name.c_str()), value);
}

void ShaderProgram::setVec2(const std::string& name, float x, float y) const
{
	glUniform2f(glGetUniformLocation(this->_id, name.c_str()), x, y);
}

void ShaderProgram::setVec2(const std::string& name, const glm::vec2& value) const
{
	glUniform2fv(glGetUniformLocation(this->_id, name.c_str()), 1, &value[0]);
}

void ShaderProgram::setVec3(const std::string& name, float x, float y, float z) const
{
	glUniform3f(glGetUniformLocation(this->_id, name.c_str()), x, y, z);
}

void ShaderProgram::setVec3(const std::string& name, const glm::vec3& value) const
{
	glUniform3fv(glGetUniformLocation(this->_id, name.c_str()), 1, &value[0]);
}

void ShaderProgram::setVec4(const std::string& name, float x, float y, float z, float w) const
{
	glUniform4f(glGetUniformLocation(this->_id, name.c_str()), x, y, z, w);
}

void ShaderProgram::setVec4(const std::string& name, const glm::vec4& value) const
{
	glUniform4fv(glGetUniformLocation(this->_id, name.c_str()), 1, &value[0]);
}

void ShaderProgram::setMat2(const std::string& name, const glm::mat2& mat) const
{
	glUniformMatrix2fv(glGetUniformLocation(this->_id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void ShaderProgram::setMat3(const std::string& name, const glm::mat3& mat) const
{
	glUniformMatrix3fv(glGetUniformLocation(this->_id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void ShaderProgram::setMat4(const std::string& name, const glm::mat4& mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(this->_id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

bool ShaderProgram::checkCompileErrors(unsigned int id)
{
	int success;
	char infoLog[1024];
	glGetProgramiv(id, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(id, 1024, NULL, infoLog);
		std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		return false;
	}
	return true;
}

