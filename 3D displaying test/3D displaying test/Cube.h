#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include<vector>
#include"Model.h"

class Cube
{
public:
	Cube(const glm::vec3& lightPos, glm::vec3 size) : _cubePos(lightPos), _model(std::make_shared<Model>(this->_createVertices())), _size(size) {
		this->_model->translate(this->_cubePos);
		this->_model->scale(this->_size);
	}
	Cube(float x,float y,float z, glm::vec3 size) : _cubePos(x, y, z), _model(std::make_shared<Model>(this->_createVertices())), _size(size) {
		this->_model->translate(this->_cubePos);
		this->_model->scale(this->_size);
	}
	glm::vec3 getCubePos()const;
	std::shared_ptr<Model> getCubeModel()const;
private:
	std::vector<Vertex> _createVertices();
	glm::vec3 _cubePos;
	std::shared_ptr<Model> _model;
	glm::vec3 _size;
};

