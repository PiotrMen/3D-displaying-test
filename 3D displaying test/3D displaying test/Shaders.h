#pragma once

#include <glad/glad.h>
#include <string>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Camera.h"
#include"Model.h"
class Shader {
public:
	explicit Shader(unsigned int type, const std::string& filePath) : _shaderPath(filePath), _type(type), _id(0) {}
	std::string readFile();

	unsigned int getType() const noexcept;
	unsigned int getID() const noexcept;
	unsigned int compileShader();

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

	virtual ~ShaderProgram();


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

	//virtual ShaderMode getMode() const = 0;

private:
	bool checkCompileErrors(unsigned int id);
	unsigned int createShaderProgram();

private:

	Shader _vertexShader;
	Shader _fragmentShader;
	unsigned int _id;
};

class GradientShaderProgram : public ShaderProgram
{
public:
	GradientShaderProgram(const Shader& vertexShader,const Shader& fragmentShader) : ShaderProgram(vertexShader, fragmentShader) {};
	void setValues(const glm::mat4& projection, const glm::mat4& view, const Camera& camera, float radius) const;
	//ShaderMode getMode() const override{ return ShaderMode::Gradient; }
};

class CubeShaderProgram : public ShaderProgram
{
public:
	CubeShaderProgram(const Shader& vertexShader, const Shader& fragmentShader) : ShaderProgram(vertexShader, fragmentShader) {};
	void setValues(const glm::mat4& projection, const glm::mat4& view, const glm::mat4& model) const;
	//ShaderMode getMode() const override { return ShaderMode::Lighting; }

};

class ShadowShaderProgram : public ShaderProgram
{
public:
	ShadowShaderProgram(const Shader& vertexShader, const Shader& fragmentShader) : ShaderProgram(vertexShader, fragmentShader) {};
	void setValues(const glm::mat4& projection, const glm::mat4& view, const Camera& camera, glm::vec3 cubePos, float ambientStrength, float diffuseStrength, float specularStrength) const;
	//ShaderMode getMode() const override { return ShaderMode::Lighting; }

};

class ShaderConfig {
public:
	virtual ~ShaderConfig() = default;
	virtual void apply(ShaderProgram* shaderProgram) = 0;
};

class ShadowShaderConfig : public ShaderConfig {
public:
	ShadowShaderConfig(const glm::mat4& projection, const glm::mat4& view, const Camera& cam, const glm::vec3& lightPos, float ambientStrength, float diffuseStrength, float specularStrength)
		: _projection(projection), _view(view), _cam(cam), _lightPos(lightPos), _ambientStrength(ambientStrength), _diffuseStrength(diffuseStrength), _specularStrength(specularStrength) {}

	void apply(ShaderProgram* shaderProgram) override {
		auto shadowShader = dynamic_cast<ShadowShaderProgram*>(shaderProgram);
		if (shadowShader) {
			shadowShader->setValues(_projection, _view, _cam, _lightPos, _ambientStrength, _diffuseStrength, _specularStrength);
		}
	}

private:
	glm::mat4 _projection;
	glm::mat4 _view;
	Camera _cam;
	glm::vec3 _lightPos;
	float _ambientStrength;
	float _diffuseStrength;
	float _specularStrength;
};

class GradientShaderConfig : public ShaderConfig {
public:
	GradientShaderConfig(const glm::mat4& projection, const glm::mat4& view, const Camera& cam, float distanceFromTarget)
		: projection(projection), view(view), _cam(cam), distanceFromTarget(distanceFromTarget) {}

	void apply(ShaderProgram* shaderProgram) override {
		auto gradientShader = dynamic_cast<GradientShaderProgram*>(shaderProgram);
		if (gradientShader) {
			gradientShader->setValues(projection, view, _cam, distanceFromTarget);
		}
	}

private:
	glm::mat4 projection;
	glm::mat4 view;
	Camera _cam;
	glm::vec3 cameraPos;
	float distanceFromTarget;
};
