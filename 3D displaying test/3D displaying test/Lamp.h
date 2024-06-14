#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include<vector>
#include"Model.h"

class Lamp
{
public:
	Lamp cubeModel(const glm::vec3& lightPos) : _lightPos(lightPos) {}
	
private:
	void _createVertices();
	glm::vec3 _lightPos;
	std::vector<Vertex> _cubeVertices;
};

