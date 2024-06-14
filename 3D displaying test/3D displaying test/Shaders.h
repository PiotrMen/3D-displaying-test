#pragma once

#include <glad/glad.h>
#include <string>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Shader {
public:
	explicit Shader(unsigned int type, const std::string& filePath) : _shaderPath(filePath), _type(type), _id(0) {}
	std::string readFile();
	unsigned int compileShader();

	unsigned int getType() const noexcept;
	unsigned int getID() const noexcept;

private:

	bool checkCompileErrors() const;

private:
	std::string _shaderPath;
	unsigned int _type;
	unsigned int _id;
};


class ShaderProgram {
public:
	explicit ShaderProgram(const Shader& vertexShader, const Shader& fragmentShader);

	~ShaderProgram();

	unsigned int createShaderProgram();

	unsigned int getID() const noexcept;

	void deleteProgram() const;
	
	void use();

	void setBool(const std::string& name, bool value) const;

	void setInt(const std::string& name, int value) const;

	void setFloat(const std::string& name, float value) const;

	void setVec2(const std::string& name, const glm::vec2& value) const;
	void setVec2(const std::string& name, float x, float y) const;

	void setVec3(const std::string& name, const glm::vec3& value) const;
	void setVec3(const std::string& name, float x, float y, float z) const;

	void setVec4(const std::string& name, const glm::vec4& value) const;

	void setVec4(const std::string& name, float x, float y, float z, float w) const;

	void setMat2(const std::string& name, const glm::mat2& mat) const;

	void setMat3(const std::string& name, const glm::mat3& mat) const;

	void setMat4(const std::string& name, const glm::mat4& mat) const;

private:
	bool checkCompileErrors(unsigned int id);

private:

	Shader _vertexShader;
	Shader _fragmentShader;
	unsigned int _id;
};

