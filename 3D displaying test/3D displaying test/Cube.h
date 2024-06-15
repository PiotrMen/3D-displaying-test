#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <glad/glad.h>
#include<vector>
#include"Model.h"

class Cube
{
public:
	Cube(const glm::vec3& lightPos, glm::vec3 size) : _cubePos(lightPos), _model(this->_createVertices()), _size(size) {
		this->_model.setModelMatrix(glm::translate(this->_model.getModelMatrix(), this->_cubePos));
		this->_model.setModelMatrix(glm::scale(this->_model.getModelMatrix(), _size));
	}
	Cube(float x,float y,float z, glm::vec3 size) : _cubePos(x, y, z), _model(this->_createVertices()), _size(size) {
		this->_model.setModelMatrix(glm::translate(this->_model.getModelMatrix(), this->_cubePos));
		this->_model.setModelMatrix(glm::scale(this->_model.getModelMatrix(), _size));
	}
	glm::vec3 getCubePos()const;
	Model getCubeModel();
private:
	std::vector<Vertex> _createVertices();
	glm::vec3 _cubePos;
	Model _model;
	glm::vec3 _size;
};

