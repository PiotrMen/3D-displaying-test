#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <glad/glad.h>
#include<vector>
#include"Model.h"

class Lamp
{
public:
	Lamp(const glm::vec3& lightPos, glm::vec3 size) : _lightPos(lightPos), _model(this->_createVertices()), _size(size) {
		this->_lamp = glm::translate(this->_lamp, this->_lightPos);
		this->_lamp = glm::scale(this->_lamp, _size); 
	}
	Lamp(float x,float y,float z, glm::vec3 size) : _lightPos(x, y, z), _model(this->_createVertices()), _size(size) {
		this->_lamp = glm::translate(this->_lamp, this->_lightPos);
		this->_lamp = glm::scale(this->_lamp, _size);
	}
	glm::vec3 getLightPos()const;
	Model getModel();
	glm::mat4 getLamp()const;
private:
	std::vector<Vertex> _createVertices();
	glm::vec3 _lightPos;
	Model _model;
	glm::vec3 _size;
	glm::mat4 _lamp = glm::mat4(1.0f);
};

