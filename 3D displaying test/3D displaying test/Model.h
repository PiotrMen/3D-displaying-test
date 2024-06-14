#pragma once

#include <string>
#include <vector>
#include <glm/glm.hpp>

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 color;
};

class Model
{
public:
    Model(const std::string& path) : _path(path), _VAO(0), _VBO(0), _model(glm::mat4(1.f)) {}
    void setupModel();
    void loadModel();
    void bind() const;
    const std::vector<Vertex>& getVertices() const;
    void cleanup();
    glm::mat4 getModelMatrix() const noexcept;
    glm::mat4 setModelMatrix(const glm::mat4& modelToSet) noexcept;

private:
    std::string _path;
    unsigned int _VAO, _VBO;
    std::vector<Vertex> _vertices;
    glm::mat4 _model;
};

// void setModelViewProjection(unsigned int shaderProgram, const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection);

